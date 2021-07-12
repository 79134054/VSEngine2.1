#pragma once
#include "VSArray.h"
namespace VSEngine2
{
	template <class T, class VSMemManagerClass = VSDefaultContainerMem>
	class VSStack
	{
	public:
		VSStack();
		~VSStack();
		bool GetTop(T & Element);
		void Push(const T & Element);
		bool Pop(T & Element);
		unsigned int GetNum();
		void Clear();
		void Destroy();
	protected:
		VSArray<T, VSMemManagerClass> m_Array;
	};
	template <class T,class VSMemManagerClass>
	VSStack<T,VSMemManagerClass>::VSStack()
	{
	}
	template <class T,class VSMemManagerClass>
	VSStack<T,VSMemManagerClass>::~VSStack()
	{

	}
	template <class T,class VSMemManagerClass>
	bool VSStack<T,VSMemManagerClass>::GetTop(T & Element)
	{
		if (m_Array.GetNum() == 0)
		{
			return false;
		}
		Element = m_Array[m_Array.GetNum() - 1];
		return true;
	}
	template <class T,class VSMemManagerClass>
	void VSStack<T,VSMemManagerClass>::Push(const T & Element)
	{
		m_Array.AddElement(Element);
	}
	template <class T,class VSMemManagerClass>
	bool VSStack<T,VSMemManagerClass>::Pop(T & Element)
	{
		if (m_Array.GetNum() == 0)
		{
			return false;
		}
		Element = m_Array[m_Array.GetNum() - 1];
		m_Array.Erase(m_Array.GetNum() - 1);
		return true;
	}
	template <class T,class VSMemManagerClass>
	unsigned int VSStack<T,VSMemManagerClass>::GetNum()
	{
		return m_Array.GetNum();
	}
	template <class T, class VSMemManagerClass>
	void VSStack<T, VSMemManagerClass>::Clear()
	{
		m_Array.Clear();
	}
	template <class T, class VSMemManagerClass>
	void VSStack<T, VSMemManagerClass>::Destroy()
	{
		m_Array.Destroy();
	}
}
