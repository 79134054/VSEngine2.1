#pragma once
#include "VSSynchronize.h"
namespace VSEngine2
{
	
	template <class T>
	class VSPointer
	{
	public:
		
		VSPointer (T* pObject = 0);
		VSPointer (const VSPointer& rPointer);
		~VSPointer ();
		
		
		T& operator* () const;
		T* operator-> () const;
		
		
		VSPointer& operator= (T* pObject);
		VSPointer& operator= (const VSPointer& rReference);
		
		
		bool operator== (T* pObject) const;
		bool operator!= (T* pObject) const;
		bool operator== (const VSPointer& rReference) const;
		bool operator!= (const VSPointer& rReference) const;

		bool operator > (T* pObject) const;
		bool operator < (T* pObject) const;
		bool operator > (const VSPointer& rReference) const;
		bool operator < (const VSPointer& rReference) const;

		operator T*()const;
		
		inline T * GetObject()const
		{
			return m_pObject;
		}
		inline void * GetObjectAddress()const
		{
			return (void *)&m_pObject;
		}
		friend class VSStream;
	private:
		T* m_pObject;
		inline void SetObject(T *  pObject)
		{
			m_pObject = pObject;
		}
		
	};

#include"VSPointer.inl"
#define DECLARE_Ptr(ClassName)\
	class ClassName;\
	typedef VSPointer<ClassName> ##ClassName##Ptr;
}
