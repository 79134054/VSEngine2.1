#pragma once
#include "VSSystem.h"
#include "VSSynchronize.h"
namespace VSEngine2
{

	//只允许一个线程控制这个线程类，没有线程安全性，2个线程同时控制这个线程类有些地方会出错
	class VSSYSTEM_API VSThread
	{
	public:
		enum Priority
		{
			Low,
			Normal,
			High,
		};
		enum ThreadState
		{
			TS_START,
			TS_SUSPEND,
			TS_STOP,
		};
	public:
		VSThread();
		virtual ~VSThread();
		
		void SetPriority(Priority p);
		
		Priority GetPriority() const;
		
		void SetStackSize(unsigned int uiSize);
		
		unsigned int GetStackSize() const;


		void Start();

		void Suspend();

		bool IsRunning() const;
		static void Sleep(DWORD dwMillseconds);

		bool IsStopTrigger();
		void Stop();
	public:
		
		static void SetThreadName(const char* name);
		inline ThreadState GetThreadState()
		{
			return m_ThreadState;
		}
	protected:
		virtual void Run() = 0;
		virtual const TCHAR* GetThreadName();
	private:
		static DWORD THREAD_CALLBACK ThreadProc(void* t);
	private:
		void* m_hThread;
		Priority m_priority;
		unsigned int m_stackSize;
	protected:
		ThreadState m_ThreadState;
		VSEvent m_StopEvent;
	};
}

