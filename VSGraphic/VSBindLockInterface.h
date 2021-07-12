#pragma once
#include "VSSystem.h"
#include "VSGraphic.h"
namespace VSEngine2
{
	class VSGRAPHIC_API VSBindLockInterface 
	{
	public:		
		VSBindLockInterface();
		virtual ~VSBindLockInterface() = 0;
		virtual void *Lock() = 0;
		virtual void UnLock() = 0;
		inline void * GetLockDataPtr()const
		{
			return m_pLockData;
		}
	protected:
		void * m_pLockData;
	};

}

