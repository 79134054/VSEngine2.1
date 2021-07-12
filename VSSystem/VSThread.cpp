#include "VSThread.h"
using namespace VSEngine2;


//------------------------------------------------------------------------------
DWORD THREAD_CALLBACK VSThread::ThreadProc(void* t)
{
	VSThread* pThread = (VSThread*)(t);
	SetThreadName(pThread->GetThreadName());
	pThread->Run();
	return 0;
}

//------------------------------------------------------------------------------
VSThread::VSThread()
	: m_hThread(NULL)
	, m_priority(Normal)
	, m_stackSize(0)
{
	VSMAC_ASSERT(!IsRunning());
#if WINDOWS_PLATFORM
	m_hThread = ::CreateThread(0, m_stackSize, ThreadProc, this, CREATE_SUSPENDED, NULL);
#else
	static_assert(0, "no thread implement");
#endif
	VSMAC_ASSERT(m_hThread);
	m_ThreadState = TS_SUSPEND;
	SetPriority(m_priority);
	m_StopEvent.Create(true);
	m_StopEvent.Reset();
}

//------------------------------------------------------------------------------
VSThread::~VSThread()
{
	if (IsRunning())
	{
#if WINDOWS_PLATFORM
		// force to exit
		TerminateThread(m_hThread, 0);
#else
		static_assert(0, "no thread implement");
#endif
	}
	if (m_hThread)
	{
#if WINDOWS_PLATFORM
		CloseHandle(m_hThread);
#else
		static_assert(0, "no thread implement");
#endif
	}
}

//------------------------------------------------------------------------------
void VSThread::SetPriority(Priority p)
{
	int nPriority = THREAD_PRIORITY_NORMAL;

	if (p == Low)
		nPriority = THREAD_PRIORITY_BELOW_NORMAL;
	else if (p == Normal)
		nPriority = THREAD_PRIORITY_NORMAL;
	else if (p == High)
		nPriority = THREAD_PRIORITY_ABOVE_NORMAL;
#if WINDOWS_PLATFORM
	::SetThreadPriority(m_hThread, nPriority);
#else
	static_assert(0, "no thread implement");
#endif
}

//------------------------------------------------------------------------------
void VSThread::Start()
{	
	if (m_ThreadState == TS_SUSPEND)
	{
#if WINDOWS_PLATFORM
		ResumeThread((HANDLE)m_hThread);
#else
		static_assert(0, "no thread implement");
#endif
		m_ThreadState = TS_START;
	}
}
//------------------------------------------------------------------------------
void VSThread::Suspend()
{
	if (m_ThreadState == TS_START)
	{
#if WINDOWS_PLATFORM
		SuspendThread((HANDLE)m_hThread);
#else
		static_assert(0, "no thread implement");
#endif
		m_ThreadState = TS_SUSPEND;
	}
	
}
void VSThread::Sleep(DWORD dwMillseconds)
{
#if WINDOWS_PLATFORM
		::Sleep(dwMillseconds);
#else
		static_assert(0, "no thread implement");
#endif
}
//------------------------------------------------------------------------------
bool VSThread::IsRunning() const
{
	if (NULL != m_hThread)
	{
#if WINDOWS_PLATFORM
		DWORD exitCode = 0;
		if (GetExitCodeThread(m_hThread, &exitCode))
		{
			if (STILL_ACTIVE == exitCode)
			{
				return true;
			}
		}
#else
		static_assert(0, "no thread implement");
#endif
	}

	return false;
}

//------------------------------------------------------------------------------
void VSThread::SetThreadName(const TCHAR* name)
{
#if WINDOWS_PLATFORM
	// update the Windows thread name so that it shows up correctly
	// in the Debugger
	struct THREADNAME_INFO
	{
		DWORD dwType;     // must be 0x1000
		LPCSTR szName;    // pointer to name (in user address space)
		DWORD dwThreadID; // thread ID (-1 = caller thread)
		DWORD dwFlags;    // reserved for future use, must be zero
	};

	THREADNAME_INFO info;
	info.dwType     = 0x1000;
	info.szName     = name;
	info.dwThreadID = ::GetCurrentThreadId();
	info.dwFlags    = 0;
	__try
	{
		RaiseException(0x406D1388, 0, sizeof(info) / sizeof(DWORD), (ULONG_PTR*)&info);
	}
	__except(EXCEPTION_CONTINUE_EXECUTION)
	{
	}
#else
	static_assert(0, "no thread implement");
#endif
}

//------------------------------------------------------------------------------
const TCHAR* VSThread::GetThreadName()
{
	return _T("VSThread");
}
bool VSThread::IsStopTrigger()
{
	return m_StopEvent.IsTrigger();
}
//------------------------------------------------------------------------------
void VSThread::Stop()
{
	if (m_ThreadState == TS_START)
	{
		VSMAC_ASSERT(this->IsRunning());
		VSMAC_ASSERT(NULL != m_hThread);

		m_StopEvent.Trigger();
		m_ThreadState = TS_STOP;
#if WINDOWS_PLATFORM
		// wait for the thread to terminate
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
#else
		static_assert(0, "no thread implement");
#endif
		m_hThread = NULL;
	}
}