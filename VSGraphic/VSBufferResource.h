#pragma once
#include "VSObject.h"
#include "VSMemBind.h"
#include "VSBindLockInterface.h"
#include "VSDataBuffer.h"
#include "VSSlot.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSBufferResource : public VSMemBind, public VSBindLockInterface, public VSSlot
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		virtual ~VSBufferResource();
		VSBufferResource();
		VSBufferResource(unsigned int uiNum, unsigned int uiDT,unsigned int uiStructStride = 0);
		bool SetData(VSDataBuffer * pData);
	public:


		virtual void *Lock();
		virtual void UnLock();

		virtual void ClearInfo();
		inline VSDataBuffer *GetDataBuffer()const { return m_pData; }

		inline unsigned int GetNum()const
		{
			return m_uiNum;
		}
		virtual unsigned int GetByteSize()const;
		inline unsigned int GetDataType()const
		{
			return m_uiDT;
		}
		inline unsigned int GetStructStride()const
		{
			return m_uiStructStride;
		}
	protected:
		friend class VSBufferUnorderAccess;
		bool SetOutput(class VSBufferUnorderAccess *pUnorderAccess);
		virtual bool OnLoadResource(VSResourceIdentifier *&pID);
		VSDataBufferPtr m_pData;
		unsigned int m_uiNum;
		unsigned int m_uiDT;
		unsigned int m_uiStructStride;
		class VSBufferUnorderAccess *m_pCurUnorderAccess;
	};
	DECLARE_Ptr(VSBufferResource);
	VSTYPE_MARCO(VSBufferResource);
}

