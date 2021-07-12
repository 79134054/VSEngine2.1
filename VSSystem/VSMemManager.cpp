#include"VSMemManager.h"
#include "VSSystem.h"
using namespace VSEngine2;
VSMemManager::VSMemManager()
{

}
VSMemManager::~VSMemManager()
{

}
VSCriticalSection VSMemManager::ms_MemLock;
#if _DEBUG
#if WINDOWS_PLATFORM
#include <DbgHelp.h>
typedef BOOL
(WINAPI
	*tFSymInitializeW)(
		_In_ HANDLE hProcess,
		_In_opt_ PCWSTR UserSearchPath,
		_In_ BOOL fInvadeProcess
		);
typedef BOOL
(WINAPI
	*tFSymGetLineFromAddr64)
	(
		IN  HANDLE                  hProcess,
		IN  DWORD64                 qwAddr,
		OUT PDWORD                  pdwDisplacement,
		OUT PIMAGEHLP_LINE64        Line64
		);

typedef DWORD
(WINAPI
	*tFSymGetOptions)
	(
		VOID
		);

typedef DWORD
(WINAPI
	*tFSymSetOptions)
	(
		IN DWORD   SymOptions
		);
static tFSymGetLineFromAddr64 fnSymGetLineFromAddr64 = NULL;
static tFSymGetOptions fnSymGetOptions = NULL;
static tFSymSetOptions fnSymSetOptions = NULL;
static tFSymInitializeW fnSymInitializeW = NULL;
static HMODULE s_DbgHelpLib = NULL;
bool VSDebugMem::InitDbgHelpLib()
{
	TCHAR szDbgName[MAX_PATH];
	GetModuleFileName(NULL, szDbgName, MAX_PATH);
	TCHAR *p = (TCHAR *)VSCsrchr(szDbgName, _T('\\'));
	if (p)
		*p = 0;
	VSStrcat(szDbgName, MAX_PATH, _T("\\dbghelp.dll"));

	// 查找当前目录的DLL
	s_DbgHelpLib = LoadLibrary(szDbgName);
	
	fnSymGetLineFromAddr64 = (tFSymGetLineFromAddr64)GetProcAddress(s_DbgHelpLib, "SymGetLineFromAddr64");
	fnSymGetOptions = (tFSymGetOptions)GetProcAddress(s_DbgHelpLib, "SymGetOptions");
	fnSymSetOptions = (tFSymSetOptions)GetProcAddress(s_DbgHelpLib, "SymSetOptions");
	fnSymInitializeW = (tFSymInitializeW)GetProcAddress(s_DbgHelpLib, "SymInitializeW");
	DWORD SymOpts = fnSymGetOptions();

	SymOpts |= SYMOPT_LOAD_LINES;
	SymOpts |= SYMOPT_FAIL_CRITICAL_ERRORS;
	SymOpts |= SYMOPT_DEFERRED_LOADS;
	SymOpts |= SYMOPT_EXACT_SYMBOLS;

	// This option allows for undecorated names to be handled by the symbol engine.
	SymOpts |= SYMOPT_UNDNAME;

	// Disable by default as it can be very spammy/slow.  Turn it on if you are debugging symbol look-up!
	//		SymOpts |= SYMOPT_DEBUG;

	// Not sure these are important or desirable
	//		SymOpts |= SYMOPT_ALLOW_ABSOLUTE_SYMBOLS;
	//		SymOpts |= SYMOPT_CASE_INSENSITIVE;

	fnSymSetOptions(SymOpts);
	bool Temp = fnSymInitializeW(GetCurrentProcess(), NULL, true);
	return Temp;
}
void VSDebugMem::FreeDbgHelpLib()
{
	if (s_DbgHelpLib != NULL)
	{
		FreeLibrary(s_DbgHelpLib);
		s_DbgHelpLib = NULL;
	}


	fnSymGetLineFromAddr64 = NULL;
	fnSymGetOptions = NULL;
	fnSymSetOptions = NULL;
	fnSymInitializeW = NULL;
}
bool VSDebugMem::GetFileAndLine(const void *pAddress, TCHAR szFile[MAX_PATH], int &line)
{

	IMAGEHLP_LINE64 Line;
	Line.SizeOfStruct = sizeof(Line);
	VSMemset(&Line, 0, sizeof(Line));
	DWORD Offset = 0;

	if(fnSymGetLineFromAddr64(GetCurrentProcess(), (DWORD64)pAddress, &Offset, &Line))
	{
#ifdef  _UNICODE
	VSMbsToWcs(szFile,MAX_PATH,Line.FileName,MAX_PATH);
#else
	VSStrCopy(szFile, MAX_PATH,Line.FileName);
#endif
	line = Line.LineNumber;

		return true;
	}
	else
	{
		DWORD error = GetLastError();
// 		if (error == 487)
// 		{
// 			VSOutputDebugString(_T("No debug info in current module\n"));
// 		}
// 		else if (error == 126)
// 		{
// 			VSOutputDebugString(_T("Debug info in current module has not loaded\n"));
// 		}
// 		else 
// 		{
// 			VSOutputDebugString(_T("SymGetLineFromAddr64 failed\n"));
// 		}
		return false;
	}
}
#endif
VSDebugMem::VSDebugMem()
{
	m_uiNumNewCalls = 0;
	m_uiNumDeleteCalls = 0;

	m_uiNumBlocks = 0;
	m_uiNumBytes = 0;
	m_uiMaxNumBytes = 0;
	m_uiMaxNumBlocks = 0;

	m_pHead = 0;
	m_pTail = 0;

	for (unsigned int i = 0 ; i < RECORD_NUM ;i++)
	{
		m_uiSizeRecord[i] = 0;
	}

}
VSDebugMem::~VSDebugMem()
{
#if WINDOWS_PLATFORM
	InitDbgHelpLib();
#endif
	PrintInfo();
#if WINDOWS_PLATFORM
	FreeDbgHelpLib();
#endif
	FreeLeakMem();
}
void* VSDebugMem::Allocate (USIZE_TYPE uiSize, USIZE_TYPE uiAlignment,bool bIsArray)
{

	VSCriticalSection::Locker Temp(ms_MemLock);
	VSMAC_ASSERT(uiSize);
	
	m_uiNumNewCalls++;

	USIZE_TYPE uiExtendedSize = sizeof(Block)+ sizeof(unsigned int) + uiSize + sizeof(unsigned int);

	char* pcAddr = (char*)VSMemObject::GetCMemManager().Allocate(uiExtendedSize, uiAlignment, bIsArray);

	VSMAC_ASSERT(pcAddr);

	Block* pBlock = (Block*)pcAddr;
	pBlock->m_uiSize = uiSize;
	pBlock->m_bIsArray = bIsArray;
	
	bool bAlignment = (uiAlignment > 0) ? true : false;
	pBlock->m_bAlignment = bAlignment;
	pBlock->m_uiStackInfoNum = 0;
#if WINDOWS_PLATFORM	
	PVOID WinBackTrace[CALLSTACK_NUM];
	short NumFrames = RtlCaptureStackBackTrace(0, CALLSTACK_NUM, WinBackTrace, NULL);

#if _WIN64
	NumFrames -= 6;
#else
	NumFrames -= 7;
#endif

	for (short i = 1 ; i < NumFrames ; i++)
	{
		pBlock->pAddr[i-1] = WinBackTrace[i];
		pBlock->m_uiStackInfoNum++;
	}
#endif
	InsertBlock(pBlock);

	pcAddr += sizeof(Block);

	unsigned int * pBeginMask = (unsigned int *)(pcAddr);
	*pBeginMask = BEGIN_MASK;
	pcAddr += sizeof(unsigned int);
	
	unsigned int * pEndMask = (unsigned int *)(pcAddr + uiSize);
	*pEndMask = END_MASK;

	m_uiNumBlocks++;
	m_uiNumBytes += uiSize;

	if (m_uiNumBytes > m_uiMaxNumBytes)
	{
		m_uiMaxNumBytes = m_uiNumBytes;
	}
	if (m_uiNumBlocks > m_uiMaxNumBlocks)
	{
		m_uiMaxNumBlocks = m_uiNumBlocks;
	}


	unsigned int uiTwoPowerI = 1;
	int i;
	for (i = 0; i <= RECORD_NUM - 2 ; i++, uiTwoPowerI <<= 1)
	{
		if (uiSize <= uiTwoPowerI)
		{
			m_uiSizeRecord[i]++;
			break;
		}
	}
	if (i == RECORD_NUM - 1)
	{
		m_uiSizeRecord[i]++;
	}

	return (void*)pcAddr;
}

void VSDebugMem::Deallocate (char* pcAddr, USIZE_TYPE uiAlignment, bool bIsArray)
{
	VSCriticalSection::Locker Temp(ms_MemLock);
	m_uiNumDeleteCalls++;
	VSMAC_ASSERT(pcAddr);
	pcAddr -= sizeof(unsigned int);

	unsigned int *pBeginMask = (unsigned int *)(pcAddr);
	VSMAC_ASSERT(*pBeginMask == BEGIN_MASK);

	pcAddr -= sizeof(Block);

	Block* pBlock = (Block*)pcAddr;
	RemoveBlock(pBlock);

	VSMAC_ASSERT(pBlock->m_bIsArray == bIsArray);
	VSMAC_ASSERT(m_uiNumBlocks > 0 && m_uiNumBytes >= pBlock->m_uiSize);
	bool bAlignment = (uiAlignment > 0) ? true : false;
	VSMAC_ASSERT(pBlock->m_bAlignment == bAlignment);
	unsigned int * pEndMask = (unsigned int *)(pcAddr + sizeof(Block) + sizeof(unsigned int) + pBlock->m_uiSize);
	VSMAC_ASSERT( *pEndMask == END_MASK);

	m_uiNumBlocks--;
	m_uiNumBytes -= pBlock->m_uiSize;

	VSMemObject::GetCMemManager().Deallocate(pcAddr, uiAlignment,bIsArray);

}
void VSDebugMem::InsertBlock (Block* pBlock)
{

	if (m_pTail)
	{
		pBlock->m_pPrev = m_pTail;
		pBlock->m_pNext = 0;
		m_pTail->m_pNext = pBlock;
		m_pTail = pBlock;
	}
	else
	{
		pBlock->m_pPrev = 0;
		pBlock->m_pNext = 0;
		m_pHead = pBlock;
		m_pTail = pBlock;
	}
}
void VSDebugMem::RemoveBlock (Block* pBlock)
{
	if (pBlock->m_pPrev)
	{
		pBlock->m_pPrev->m_pNext = pBlock->m_pNext;
	}
	else
	{
		m_pHead = pBlock->m_pNext;
	}

	if (pBlock->m_pNext)
	{
		pBlock->m_pNext->m_pPrev = pBlock->m_pPrev;
	}
	else
	{
		m_pTail = pBlock->m_pPrev;
	}
}
void VSDebugMem::FreeLeakMem()
{
	Block * pBlock = m_pHead;
	while(pBlock)
	{
		Block * Temp = pBlock;
		pBlock = pBlock->m_pNext;
		free((void*)Temp);
	}
}
void VSDebugMem::PrintInfo()
{


	VSOutputDebugString(_T("#########################  begin print leak mem  ######################\n"));

	VSOutputDebugString(_T("Max Byte Num: %d\n"),m_uiMaxNumBytes);

	VSOutputDebugString(_T("Max Block Num: %d\n"),m_uiMaxNumBlocks);

	VSOutputDebugString(_T("Total Size: %d\n"),m_uiNumBytes);

	VSOutputDebugString(_T("Block Num: %d\n"),m_uiNumBlocks);


	VSOutputDebugString(_T("New Call: %d\n"),m_uiNumNewCalls);

	VSOutputDebugString(_T("Delete Call: %d\n"),m_uiNumDeleteCalls);

	if (m_pHead)
	{
		VSOutputDebugString(_T("Memory Leak:\n"));
	}
	else
	{
		VSOutputDebugString(_T("No Memory Leak\n"));
	}
	Block * pBlock = m_pHead;
	static unsigned int uiLeakNum = 0;
	while (pBlock)
	{

		uiLeakNum++;
		VSOutputDebugString(_T("$$$$$$$$$$$$$$$$  Leak %d  $$$$$$$$$$$$$$$$$\n"),uiLeakNum);
		VSOutputDebugString(_T("Size: %d\n"),pBlock->m_uiSize);
		VSOutputDebugString(_T("Is Array:%d\n"),pBlock->m_bIsArray);
#if WINDOWS_PLATFORM
		TCHAR szFile[MAX_PATH];
		int	  line;
		for (unsigned int i = 0 ; i < pBlock->m_uiStackInfoNum ; i++)
		{

			if (!GetFileAndLine(pBlock->pAddr[i], szFile, line))
			{
				break;
			}
			VSOutputDebugString(_T("%s(%d)\n"),szFile,line);
		
		}
#endif
		VSOutputDebugString(_T("$$$$$$$$$$$$$$$$$ Leak %d  $$$$$$$$$$$$$$$$$$$\n"),uiLeakNum);
		pBlock = pBlock->m_pNext;
	}
	VSOutputDebugString(_T("leak block total num : %d\n"),uiLeakNum);

	VSOutputDebugString(_T("#########################  end print leak mem  ######################\n"));
}
#endif
#if WINDOWS_PLATFORM && !_WIN64 && !_DEBUG
VSMemWin32::VSMemWin32()
{

	PageSize = 0 ;

	// Get OS page size.
	SYSTEM_INFO SI;
	GetSystemInfo( &SI );
	PageSize = SI.dwPageSize;
	VSMAC_ASSERT(!(PageSize&(PageSize-1)));

	// Init tables.
	OsTable.FirstPool    = NULL;
	OsTable.ExhaustedPool = NULL;
	OsTable.BlockSize    = 0;

	PoolTable[0].FirstPool    = NULL;
	PoolTable[0].ExhaustedPool = NULL;
	PoolTable[0].BlockSize    = 8;
	for( DWORD i=1; i<5; i++ )
	{
		PoolTable[i].FirstPool    = NULL;
		PoolTable[i].ExhaustedPool = NULL;
		PoolTable[i].BlockSize    = (8<<((i+1)>>2)) + (2<<i);
	}
	for( DWORD i=5; i<POOL_COUNT; i++ )
	{
		PoolTable[i].FirstPool    = NULL;
		PoolTable[i].ExhaustedPool = NULL;
		PoolTable[i].BlockSize    = (4+((i+7)&3)) << (1+((i+7)>>2));
	}
	for( DWORD i=0; i < POOL_MAX; i++ )
	{
		DWORD Index;
		for( Index=0; PoolTable[Index].BlockSize<i; Index++ );
		VSMAC_ASSERT(Index < POOL_COUNT);
		MemSizeToPoolTable[i] = &PoolTable[Index];
	}
	for( DWORD i=0; i < 32 ; i++ )
	{
		PoolIndirect[i] = NULL;
	}
	VSMAC_ASSERT(POOL_MAX-1==PoolTable[POOL_COUNT-1].BlockSize);
}
VSMemWin32::~VSMemWin32()
{
	for (unsigned int i = 0 ; i < 32 ; i++)
	{
		for (unsigned int j = 0 ; j < 2048 ; j++)
		{
			if (PoolIndirect[i])
			{		
				if (PoolIndirect[i][j].Mem)
				{
					VirtualFree( PoolIndirect[i][j].Mem, 0, MEM_RELEASE );
					PoolIndirect[i][j].Mem = NULL;
				}

				VirtualFree( PoolIndirect[i], 0, MEM_RELEASE );
				PoolIndirect[i] = NULL;
			}
		}
	}
}
VSMemWin32::FPoolInfo* VSMemWin32::FFreeMem::GetPool()
{
	return (FPoolInfo*)((INT)this & 0xffff0000);
}
void* VSMemWin32::Allocate (USIZE_TYPE uiSize, USIZE_TYPE uiAlignment,bool bIsArray)
{
	VSCriticalSection::Locker Temp(ms_MemLock);
	FFreeMem* Free;
	if( uiSize<POOL_MAX )
	{
		// Allocate from pool.
		FPoolTable* Table = MemSizeToPoolTable[uiSize];
		VSMAC_ASSERT(uiSize<=Table->BlockSize);
		FPoolInfo* Pool = Table->FirstPool;
		if( !Pool )
		{
			// Must create a new pool.
			DWORD Blocks  = 65536 / Table->BlockSize;
			DWORD Bytes   = Blocks * Table->BlockSize;
			VSMAC_ASSERT(Blocks>=1);
			VSMAC_ASSERT(Blocks*Table->BlockSize<=Bytes);

			// Allocate memory.
			Free = (FFreeMem*)VirtualAlloc( NULL, Bytes, MEM_COMMIT, PAGE_READWRITE );
			if( !Free )
			{
				return NULL;
			}

			// Create pool in the indirect table.
			FPoolInfo*& Indirect = PoolIndirect[((DWORD)Free>>27)];
			if( !Indirect )
			{
				Indirect = CreateIndirect();
			}
			Pool = &Indirect[((DWORD)Free>>16)&2047];

			// Init pool.
			Pool->Link( Table->FirstPool );
			Pool->Mem            = (BYTE*)Free;
			Pool->Bytes		     = Bytes;
			Pool->OsBytes		 = Align(Bytes,PageSize);
			Pool->Table		     = Table;
			Pool->Taken			 = 0;
			Pool->FirstMem       = Free;

			// Create first free item.
			Free->Blocks         = Blocks;
			Free->Next           = NULL;
		}

		// Pick first available block and unlink it.
		Pool->Taken++;
		VSMAC_ASSERT(Pool->FirstMem);
		VSMAC_ASSERT(Pool->FirstMem->Blocks>0);
		Free = (FFreeMem*)((BYTE*)Pool->FirstMem + --Pool->FirstMem->Blocks * Table->BlockSize);
		if( Pool->FirstMem->Blocks==0 )
		{
			Pool->FirstMem = Pool->FirstMem->Next;
			if( !Pool->FirstMem )
			{
				// Move to exhausted list.
				Pool->Unlink();
				Pool->Link( Table->ExhaustedPool );
			}
		}
	}
	else
	{
		// Use OS for large allocations.
		INT AlignedSize = Align(uiSize,PageSize);
		Free = (FFreeMem*)VirtualAlloc( NULL, AlignedSize, MEM_COMMIT, PAGE_READWRITE );
		if( !Free )
		{
			return NULL;
		}
		VSMAC_ASSERT(!((SIZE_T)Free&65535));

		// Create indirect.
		FPoolInfo*& Indirect = PoolIndirect[((DWORD)Free>>27)];
		if( !Indirect )
		{
			Indirect = CreateIndirect();
		}

		// Init pool.
		FPoolInfo* Pool = &Indirect[((DWORD)Free>>16)&2047];
		Pool->Mem		= (BYTE*)Free;
		Pool->Bytes		= uiSize;
		Pool->OsBytes	= AlignedSize;
		Pool->Table		= &OsTable;

	}

	return Free;
}
void VSMemWin32::Deallocate (char* pcAddr, USIZE_TYPE uiAlignment,bool bIsArray)
{
	VSCriticalSection::Locker Temp(ms_MemLock);
	VSMAC_ASSERT(pcAddr);
	// Windows version.
	FPoolInfo* Pool = &PoolIndirect[(DWORD)pcAddr>>27][((DWORD)pcAddr>>16)&2047];
	VSMAC_ASSERT(Pool->Bytes!=0);
	if( Pool->Table!=&OsTable )
	{
		// If this pool was exhausted, move to available list.
		if( !Pool->FirstMem )
		{
			Pool->Unlink();
			Pool->Link( Pool->Table->FirstPool );
		}

		// Free a pooled allocation.
		FFreeMem* Free		= (FFreeMem *)pcAddr;
		Free->Blocks		= 1;
		Free->Next			= Pool->FirstMem;
		Pool->FirstMem		= Free;


		// Free this pool.
		VSMAC_ASSERT(Pool->Taken>=1);
		if( --Pool->Taken == 0 )
		{
			// Free the OS memory.
			Pool->Unlink();
			VirtualFree( Pool->Mem, 0, MEM_RELEASE );
			Pool->Mem = NULL;
		}
	}
	else
	{
		// Free an OS allocation.

		VirtualFree( pcAddr, 0, MEM_RELEASE );
		Pool->Mem = NULL;
	}

}
#elif !_DEBUG
#if defined(_DEBUG)
#undef _DEBUG
#endif
#include <scalable_allocator.h>
VSMemWin64::VSMemWin64()
{
	
}
VSMemWin64::~VSMemWin64()
{

}

void* VSMemWin64::Allocate(USIZE_TYPE uiSize, USIZE_TYPE uiAlignment, bool bIsArray)
{
	if (uiAlignment != 0)
	{
		uiAlignment = Max(uiSize >= 16 ? (USIZE_TYPE)16 : (USIZE_TYPE)8, uiAlignment);
		return scalable_aligned_malloc(uiSize, uiAlignment);
	}
	else
	{
		return scalable_malloc(uiSize);
	}
	
}
void VSMemWin64::Deallocate(char* pcAddr, USIZE_TYPE uiAlignment, bool bIsArray)
{
	if (!pcAddr)
	{
		return;
	}
	if (uiAlignment != 0)
	{
		scalable_aligned_free(pcAddr);
	}
	else
	{
		scalable_free(pcAddr);
	}
}
#endif
VSCMem::VSCMem()
{

}
VSCMem::~VSCMem()
{

}
void* VSCMem::Allocate (USIZE_TYPE uiSize, USIZE_TYPE uiAlignment,bool bIsArray)
{
	VSCriticalSection::Locker Temp(ms_MemLock);
	if (uiAlignment == 0)
	{
		return malloc(uiSize);

	}
	else
	{
		return _aligned_malloc(uiSize,uiAlignment);
	}
	return NULL;
}
void VSCMem::Deallocate (char* pcAddr, USIZE_TYPE uiAlignment,bool bIsArray)
{
	VSCriticalSection::Locker Temp(ms_MemLock);
	if (uiAlignment == 0)
	{
		free(pcAddr);
	}
	else
	{
		_aligned_free(pcAddr);
	}

}
VSStackMem::VSStackMem(USIZE_TYPE uiDefaultChunkSize)
{
	VSMAC_ASSERT(uiDefaultChunkSize > sizeof(FTaggedMemory));
	Top = NULL;				
	End = NULL;				
	DefaultChunkSize = uiDefaultChunkSize;	
	TopChunk = NULL;			
	UnusedChunks = NULL;
	NumMarks = 0;	
}
VSStackMem::~VSStackMem()
{
	FreeChunks(NULL);
	while( UnusedChunks )
	{
		void* Old = UnusedChunks;
		UnusedChunks = UnusedChunks->Next;
		VSMemObject::GetMemManager().Deallocate((char *)Old,0,true);
	}
	VSMAC_ASSERT(NumMarks == 0);
}

BYTE* VSStackMem::AllocateNewChunk(USIZE_TYPE MinSize )
{
	FTaggedMemory* Chunk=NULL;
	for( FTaggedMemory** Link=&UnusedChunks; *Link; Link=&(*Link)->Next )
	{
		// Find existing chunk.
		if( (*Link)->DataSize >= MinSize )
		{
			Chunk = *Link;
			*Link = (*Link)->Next;
			break;
		}
	}
	if( !Chunk )
	{
		// Create new chunk.
		USIZE_TYPE DataSize    = Max( MinSize, DefaultChunkSize - sizeof(FTaggedMemory) );
		Chunk           = (FTaggedMemory*)VSMemObject::GetMemManager().Allocate( DataSize + sizeof(FTaggedMemory),0,true);
		Chunk->DataSize = DataSize;
	}

	Chunk->Next = TopChunk;
	TopChunk    = Chunk;
	Top         = Chunk->Data;
	End         = Top + Chunk->DataSize;
	return Top;
}

void VSStackMem::FreeChunks( FTaggedMemory* NewTopChunk )
{
	while( TopChunk!=NewTopChunk )
	{
		FTaggedMemory* RemoveChunk = TopChunk;
		TopChunk                   = TopChunk->Next;
		RemoveChunk->Next          = UnusedChunks;
		UnusedChunks               = RemoveChunk;
	}
	Top = NULL;
	End = NULL;
	if( TopChunk )
	{
		Top = TopChunk->Data;
		End = Top + TopChunk->DataSize;
	}
}

void* VSStackMem::Allocate (USIZE_TYPE uiSize, USIZE_TYPE uiAlignment,bool bIsArray)
{
	// Debug checks.
	VSMAC_ASSERT(uiSize>=0);
#ifdef _DEBUG
	if (uiAlignment > 0)
	{
		VSMAC_ASSERT((uiAlignment&(uiAlignment-1))==0);
	}
#endif
	VSMAC_ASSERT(Top<=End);
	VSMAC_ASSERT(NumMarks > 0);

	// Try to get memory from the current chunk.
	BYTE* Result = Top;
	if (uiAlignment > 0)
	{
		Result = (BYTE *)(((USIZE_TYPE)Top + (uiAlignment - 1)) & ~(uiAlignment - 1));
	}
	Top = Result + uiSize;

	// Make sure we didn't overflow.
	if( Top > End )
	{
		// We'd pass the end of the current chunk, so allocate a new one.
		AllocateNewChunk( uiSize + uiAlignment );
		Result = Top;
		if (uiAlignment > 0)
		{
			Result = (BYTE *)(((USIZE_TYPE)Top + (uiAlignment - 1)) & ~(uiAlignment - 1));
		}
		Top    = Result + uiSize;
	}
	return Result;
}
void VSStackMem::Clear()
{
	FreeChunks(NULL);
	
}
VSMemManager& VSMemObject::GetMemManager()
{	
#if WINDOWS_PLATFORM &&!_WIN64 &&!_DEBUG	
	static VSMemWin32 g_MemManager;
#elif _DEBUG
	static VSDebugMem g_MemManager;
#else
	static VSMemWin64 g_MemManager;
#endif 

	return g_MemManager;
}
VSMemManager& VSMemObject::GetCMemManager()
{
	static VSCMem g_MemManager;
	return g_MemManager;
}
VSStackMem& VSMemObject::GetStackMemManager ()
{
	static VSTlsValue g_TlsValue;
	void* pTlsValue = g_TlsValue.GetThreadValue();
	if (!pTlsValue)
	{
		pTlsValue = VS_NEW VSStackMem();
		g_TlsValue.SetThreadValue(pTlsValue);
	}
	return *((VSStackMem*)pTlsValue);
	//static VSStackMem g_StackMemManager;
	//return g_StackMemManager;
}
VSMemObject::VSMemObject()
{
	GetCMemManager();
	GetMemManager();
	GetStackMemManager();
}
VSMemObject::~VSMemObject()
{

}
