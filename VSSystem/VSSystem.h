#pragma once
#include "VSPlatform.h"
#if WINDOWS_PLATFORM
#include <stdio.h>
#include <tchar.h>
#include <memory.h>
#include <assert.h>
#include <sys/stat.h>
#include <atlsimpstr.h>
#include <intrin.h>
#pragma warning(disable:4251) //去除模板导出编译的警告
#pragma warning(disable:4595) 
#endif
#ifdef VSSYSTEM_EXPORTS
	#define VSSYSTEM_API __declspec(dllexport) 
#else 
	#define VSSYSTEM_API __declspec(dllimport) 
#endif
namespace VSEngine2
{
#ifndef _DEBUG
#define _DEBUG 0
#endif 

#ifdef _DEBUG
#define VSMAC_ASSERT(Expression)\
	{\
		 assert(Expression);\
	}
#else
#define VSMAC_ASSERT(Expression)
#endif

#define THREAD_CALLBACK __stdcall
	inline unsigned int SizeTypeToGUID32(USIZE_TYPE Size_Type)
	{
		return (Size_Type & 0xffffffff);
	}
	inline USIZE_TYPE GUID32ToSizeType(unsigned int uiGUID)
	{
#ifdef WINDOWS_PLATFORM
	#if _WIN64
		return (uiGUID & 0x00000000ffffffff);
	#else
		return (uiGUID & 0xffffffff);
	#endif
#else
		static_assert(0, "No Implement!");;
#endif
	}
	inline int ComparePointer(void * Point1, void *Point2)
	{
#if WINDOWS_PLATFORM

		if (((USIZE_TYPE)Point1) == ((USIZE_TYPE)Point2))
		{
			return 0;
		}
		else if (((USIZE_TYPE)Point1) > ((USIZE_TYPE)Point2))
		{
			return 1;
		}
		else
		{
			return -1;
		}		
#else
		static_assert(0, "No Implement!");;
#endif
	}

	#define LOG_BUFFER_SIZE 65536	 
	class VSSYSTEM_API VSSystem
	{
	public:
		static TCHAR ms_sLogBuffer[LOG_BUFFER_SIZE];
		static DWORD ms_dwMainThreadID;
	};
	inline bool VSMemcpy(void * pDest,const void * pSrc, USIZE_TYPE uiCountSize, USIZE_TYPE uiDestBufferSize = 0)
	{
		if (!uiDestBufferSize)
		{
			uiDestBufferSize = uiCountSize;
		}
#ifdef WINDOWS_PLATFORM
		return (memcpy_s(pDest,uiDestBufferSize,pSrc,uiCountSize) == 0);
#else
		static_assert(0, "No Implement!");;
		return false;
#endif

	}
	inline unsigned int VSStrLen(const TCHAR * pStr)
	{
#ifdef WINDOWS_PLATFORM
		return (unsigned int)_tcslen(pStr);
#else
		static_assert(0, "No Implement!");;
		return 0;
#endif
	}
	inline long VSlockedCompareExchange(long* pDestination,long Exchange, long Comparand)
	{
#ifdef WINDOWS_PLATFORM
		return _InterlockedCompareExchange(pDestination, Exchange, Comparand);
#else
		static_assert(0, "No Implement!");;
		return 0;
#endif
	}
	inline long VSlockedAdd(long* pRefCount, long Value)
	{
#ifdef WINDOWS_PLATFORM
		return _InterlockedExchangeAdd(pRefCount, Value);
#else
		static_assert(0, "No Implement!");;
		return 0;
#endif
	}
	inline long VSlockedExchange(long* pRefCount, long Value)
	{
#ifdef WINDOWS_PLATFORM
		return _InterlockedExchange(pRefCount, Value);
#else
		static_assert(0, "No Implement!");;
		return 0;
#endif
	}
	inline long VSlockedAnd(long* pRefCount, long Value)
	{
#ifdef WINDOWS_PLATFORM
		return _InterlockedAnd(pRefCount, Value);
#else
		static_assert(0, "No Implement!");;
		return 0;
#endif
	}
	inline long VSlockedOr(long* pRefCount, long Value)
	{
#ifdef WINDOWS_PLATFORM
		return _InterlockedOr(pRefCount, Value);
#else
		static_assert(0, "No Implement!");;
		return 0;
#endif
	}
	inline long VSlockedXor(long* pRefCount, long Value)
	{
#ifdef WINDOWS_PLATFORM
		return _InterlockedXor(pRefCount, Value);
#else
		static_assert(0, "No Implement!");;
		return 0;
#endif
	}
	inline long VSAtomicRead(long* pRefCount)
	{
#ifdef WINDOWS_PLATFORM
		return VSlockedCompareExchange(pRefCount, 0,0);
#else
		static_assert(0, "No Implement!");;
		return 0;
#endif
	}
	inline long VSAtomicWrite(long* pRefCount, long Value)
	{
#ifdef WINDOWS_PLATFORM
		return VSlockedExchange(pRefCount, Value);
#else
		static_assert(0, "No Implement!");;
		return 0;
#endif
	}
	inline long VSLockedIncrement(long * pRefCount)
	{
#ifdef WINDOWS_PLATFORM
		return _InterlockedIncrement(pRefCount);
#else
		static_assert(0, "No Implement!");;
		return 0;
#endif
	}
	inline long VSLockedDecrement(long * pRefCount)
	{
#ifdef WINDOWS_PLATFORM
		return _InterlockedDecrement(pRefCount);
#else
		static_assert(0, "No Implement!");;
		return 0;
#endif
	}
	inline void VSMemset(void *pDest,int iC, USIZE_TYPE uiCount)
	{
#ifdef WINDOWS_PLATFORM
		memset(pDest,iC,uiCount);
#else
		static_assert(0, "No Implement!");;
		return;
#endif
	}
	inline bool VSIsSpace(int c)
	{
#ifdef WINDOWS_PLATFORM
		return _istspace(c);
#else
		static_assert(0, "No Implement!");;
		return false;
#endif
	}
	inline void VSStrcat(TCHAR * pDest,unsigned int uiCount,const TCHAR * pSource)
	{
#ifdef WINDOWS_PLATFORM
		_tcscat_s(pDest,uiCount,pSource);
#else
		static_assert(0, "No Implement!");;
		return ;
#endif
	}
	//Find the next token in a string.
	inline TCHAR * VSStrtok(TCHAR *strToken, const TCHAR *strDelimit ,TCHAR ** pContext)
	{
#ifdef WINDOWS_PLATFORM
		return _tcstok_s(strToken, strDelimit,pContext);
#else
		static_assert(0, "No Implement!");;
		return NULL;
#endif
	}
	//Scan a string for the last occurrence of a character.
	inline const TCHAR * VSCsrchr(const TCHAR *pString,int c )
	{
#ifdef WINDOWS_PLATFORM
		return _tcsrchr(pString,c);
#else
		static_assert(0, "No Implement!");;
		return NULL;
#endif
	}
	inline void VSOutputDebugString(const TCHAR * pcString, ...)
	{
#ifdef WINDOWS_PLATFORM
		char *pArgs;
		pArgs = (char*) &pcString + sizeof(pcString);
		_vstprintf_s(VSSystem::ms_sLogBuffer, LOG_BUFFER_SIZE,pcString, pArgs) ;
		OutputDebugString(VSSystem::ms_sLogBuffer);
#else
		static_assert(0, "No Implement!");;
		return ;
#endif
	}
	inline void VSSprintf(TCHAR * _DstBuf, unsigned int _SizeInBytes,const TCHAR * _Format, ...)
	{
#ifdef WINDOWS_PLATFORM
		char *pArgs;
		pArgs = (char*) &_Format + sizeof(_Format);
		_vstprintf_s(_DstBuf, _SizeInBytes,_Format, pArgs) ;
#else
		static_assert(0, "No Implement!");;
		return ;
#endif
	}
	inline void VSSprintf(TCHAR * _DstBuf, unsigned int _SizeInBytes, const TCHAR * _Format, va_list pArgs)
	{
#ifdef WINDOWS_PLATFORM
		_vstprintf_s(_DstBuf, _SizeInBytes, _Format, pArgs);
#else
		static_assert(0, "No Implement!");;
		return;
#endif
	}
	inline void VSScanf(TCHAR * Buf, const TCHAR * _Format, va_list pArgs)
	{
#ifdef WINDOWS_PLATFORM
		_stscanf_s(Buf, _Format, pArgs);
#else
		static_assert(0, "No Implement!");;
		return;
#endif
	}
	inline void VSStrCopy(TCHAR * pDest,unsigned int uiCount,const TCHAR * pSource)
	{
#ifdef WINDOWS_PLATFORM
		_tcscpy_s(pDest,uiCount,pSource);
#else
		static_assert(0, "No Implement!");;
		return;
#endif
	}
	inline int VSStrCmp(const TCHAR *String1,const TCHAR *String2)
	{
#ifdef WINDOWS_PLATFORM
		return _tcscmp(String1,String2);
#else
		static_assert(0, "No Implement!");;
		return;
#endif
	}
	inline int VSStrnCmp(const TCHAR *String1, const TCHAR *String2,unsigned int uiMaxNum)
	{
#ifdef WINDOWS_PLATFORM
		return _tcsncmp(String1, String2, uiMaxNum);
#else
		static_assert(0, "No Implement!");;
		return;
#endif
	}
	inline void VSMbsToWcs(wchar_t * Dest,unsigned int uiSizeInWord,const char * Source,unsigned int uiSizeInByte)
	{
#ifdef WINDOWS_PLATFORM
		mbstowcs_s(0,Dest,uiSizeInWord,Source,uiSizeInByte);
#else
		static_assert(0, "No Implement!");;
		return;
#endif
	}
	inline void VSWcsToMbs(char * Dest,unsigned int uiSizeInByte,const wchar_t * Source,unsigned int uiSizeInWord)
	{
#ifdef WINDOWS_PLATFORM
		wcstombs_s(0,Dest,uiSizeInByte,Source,uiSizeInWord);
#else
		static_assert(0, "No Implement!");;
		return;
#endif
	}
	inline bool VSIstalnum(char c)
	{
#ifdef WINDOWS_PLATFORM
		return _istalnum(c);
#else
		static_assert(0, "No Implement!");;
		return false;
#endif
	}
	inline unsigned int VSGetCpuNum()
	{
#ifdef WINDOWS_PLATFORM
		SYSTEM_INFO SystemInfo;
#if _WIN64
		GetNativeSystemInfo(&SystemInfo);
#else
		GetSystemInfo(&SystemInfo);
#endif
		return SystemInfo.dwNumberOfProcessors;
#else
		static_assert(0, "No Implement!");;
		return 1;
#endif
	}
	inline bool VSIsMainThread()
	{
#ifdef WINDOWS_PLATFORM
		return VSSystem::ms_dwMainThreadID == GetCurrentThreadId();
#else
		static_assert(0, "No Implement!");;
		return false;
#endif
	}
	inline void VSGetCurrentDirectory(TCHAR * CurDir)
	{
#ifdef WINDOWS_PLATFORM
		GetCurrentDirectory(MAX_PATH, CurDir);
#else
		static_assert(0, "No Implement!");;
		return;
#endif
	}
	inline TCHAR* VSGetExeFolderName()
	{
#if WINDOWS_PLATFORM
	#if _WIN64
		return _T("x64"); 
	#else
		return _T("Win32");
	#endif
#else
		static_assert(0, "No Implement!");;
#endif

	}
	inline EPLATRORM GetCurPlatform()
	{
#if WINDOWS_PLATFORM
		return EP_Windows;
#else
		static_assert(0, "No Implement!");
		return EP_MAX;
#endif
	}
	inline TCHAR * GetCurPlatformName()
	{
#if WINDOWS_PLATFORM
		return _T("Windows");
#else
		static_assert(0, "No Implement!");
		return _T("");
#endif
	}
	inline void VSSetCurrentDirectory(TCHAR * CurDir)
	{
#ifdef WINDOWS_PLATFORM
		SetCurrentDirectory(CurDir);
#else
		static_assert(0, "No Implement!");
		return;
#endif
	}
	inline void VSInitSystem()
	{
#ifdef WINDOWS_PLATFORM
		VSSystem::ms_dwMainThreadID = GetCurrentThreadId();
#else
		static_assert(0, "No Implement!");
		return;
#endif
	}
	inline unsigned int VSTlsAlloc()
	{
#ifdef WINDOWS_PLATFORM
		return TlsAlloc();
#else
		static_assert(0, "No Implement!");
		return 0;
#endif
	}
	inline void* VSGetTlsValue(unsigned int TlsSolt)
	{
#ifdef WINDOWS_PLATFORM
		return TlsGetValue(TlsSolt);
#else
		static_assert(0, "No Implement!");
		return NULL;
#endif
	}
	inline bool VSSetTlsValue(unsigned int TlsSolt,void * TlsValue)
	{
#ifdef WINDOWS_PLATFORM
		return TlsSetValue(TlsSolt, TlsValue);
#else
		static_assert(0, "No Implement!");
		return false;
#endif
	}
	inline bool VSTlsFree(unsigned int TlsSolt)
	{
#ifdef WINDOWS_PLATFORM
		return TlsFree(TlsSolt);
#else
		static_assert(0, "No Implement!");
		return false;
#endif
	}
#ifdef WINDOWS_PLATFORM
#define ALIGN(n) __declspec(align(n))
#else
#define ALIGN(n)
#endif
	template<typename T>
	inline T ABS(T t)
	{
		return t < 0 ? -t : t;
	}
	template<typename T>
	inline T Min(T t0, T t1)
	{
		return t0 < t1 ? t0 : t1;
	}
	template<typename T>
	inline T Max(T t0, T t1)
	{
		return t0 > t1 ? t0 : t1;
	}
	template<typename T>
	inline T Clamp(T Value, T Max, T Min)
	{
		if (Value >= Max)
		{
			return Max;
		}
		if (Value <= Min)
		{
			return Min;
		}
		return Value;
	}
	template <class T>
	inline void Swap(T &t1, T &t2)
	{
		T temp;
		temp = t1;
		t1 = t2;
		t2 = temp;
	}
#define BIT(i) (1 << i)
}