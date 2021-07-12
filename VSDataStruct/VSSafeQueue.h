#pragma once
#include "VSQueue.h"
#include "VSSynchronize.h"
namespace VSEngine2
{

	template <class T, class VSMemManagerClass = VSDefaultContainerMem>
	class VSSafeQueue : public VSMemObject
	{
	public:

		VSSafeQueue(bool bUnique = false);
		~VSSafeQueue(); 
		void Enqueue(const T & Element);
		void Dequeue(T & Element);
		void GetTop(T & Element);
		void Clear();
		void Erase(const T & Element);
		bool IsEmpty();

		bool TryEnqueue(const T& Element);
		bool TryDequeue(T& Element);
		bool TryGetTop(T& Element);
		bool TryErase(const T& Element);
		bool TryIsEmpty();
	protected:
		VSQueue<T, VSMemManagerClass> m_Queue;
		VSCriticalSection m_CriticalSec;

	private:

	};
	template <class T, class VSMemManagerClass>
	VSSafeQueue<T, VSMemManagerClass>::~VSSafeQueue()
	{
		
	}

	template <class T, class VSMemManagerClass>
	VSSafeQueue<T, VSMemManagerClass>::VSSafeQueue(bool bUnique)
	{
	}
	template <class T, class VSMemManagerClass>
	void VSSafeQueue<T, VSMemManagerClass>::Clear()
	{
		VSCriticalSection::Locker Temp(m_CriticalSec);
		m_Queue.Clear();
	}

	template <class T, class VSMemManagerClass>
	void VSSafeQueue<T, VSMemManagerClass>::Enqueue(const T & Element)
	{
		VSCriticalSection::Locker Temp(m_CriticalSec);
		m_Queue.Enqueue(Element);
	}
	template <class T, class VSMemManagerClass>
	bool VSSafeQueue<T, VSMemManagerClass>::TryEnqueue(const T& Element)
	{
		bool bLocked = m_CriticalSec.TryLock();
		if (bLocked)
		{
			m_Queue.Enqueue(Element);
			m_CriticalSec.Unlock();
		}
		return bLocked;
	}
	template <class T, class VSMemManagerClass>
	void VSSafeQueue<T, VSMemManagerClass>::GetTop(T & Element)
	{
		VSCriticalSection::Locker Temp(m_CriticalSec);
		m_Queue.GetTop(Element);
	}
	template <class T, class VSMemManagerClass>
	bool VSSafeQueue<T, VSMemManagerClass>::TryGetTop(T& Element)
	{
		bool bLocked = m_CriticalSec.TryLock();
		if (bLocked)
		{
			bLocked = m_Queue.GetTop(Element);
			m_CriticalSec.Unlock();
		}
		return bLocked;
	}
	template <class T, class VSMemManagerClass>
	void VSSafeQueue<T, VSMemManagerClass>::Dequeue(T & Element)
	{
		VSCriticalSection::Locker Temp(m_CriticalSec);
		m_Queue.Dequeue(Element);
	}
	template <class T, class VSMemManagerClass>
	bool VSSafeQueue<T, VSMemManagerClass>::TryDequeue(T& Element)
	{
		bool bLocked = m_CriticalSec.TryLock();
		if (bLocked)
		{
			bLocked = m_Queue.Dequeue(Element);
			m_CriticalSec.Unlock();
		}
		return bLocked;
	}
	template <class T, class VSMemManagerClass>
	void VSSafeQueue<T, VSMemManagerClass>::Erase(const T & Element)
	{
		VSCriticalSection::Locker Temp(m_CriticalSec);
		m_Queue.Erase(Element);
	}
	template <class T, class VSMemManagerClass>
	bool VSSafeQueue<T, VSMemManagerClass>::TryErase(const T& Element)
	{
		bool bLocked = m_CriticalSec.TryLock();
		if (bLocked)
		{
			m_Queue.Erase(Element);
			m_CriticalSec.Unlock();
		}
		return bLocked;		
	}
	template <class T, class VSMemManagerClass>
	bool VSSafeQueue<T, VSMemManagerClass>::IsEmpty()
	{
		VSCriticalSection::Locker Temp(m_CriticalSec);
		bool bEmpty = false;		
		if (m_Queue.GetNum() == 0)
		{
			bEmpty = true;
		}
		return bEmpty;
		
	}
	template <class T, class VSMemManagerClass>
	bool VSSafeQueue<T, VSMemManagerClass>::TryIsEmpty()
	{
		bool bLocked = m_CriticalSec.TryLock();
		bool bEmpty = false;
		if (bLocked)
		{
			if (m_Queue.GetNum() == 0)
			{
				bEmpty = true;
			}
			m_CriticalSec.Unlock();
		}
		return bEmpty;

	}
}
