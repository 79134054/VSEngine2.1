#pragma once
#include "VSBufferResource.h"
#include "VSUnorderAccess.h"
namespace VSEngine2
{
	class VSGRAPHIC_API VSBufferUnorderAccess : public VSUnorderAccess
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		friend class VSResourceManager;
		friend class VSBufferResource;
		virtual ~VSBufferUnorderAccess();
		virtual void *Lock();
		virtual void UnLock();
		virtual unsigned int GetOutputType()const { return OT_BUFFER_UNORDER_ACCESS; }
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
		VSBufferUnorderAccess(unsigned int uiNum, unsigned int uiDT, unsigned int uiStructStride = 0, bool CPURead = false);
		VSBufferUnorderAccess(VSBufferResource * pCreateBy, bool CPURead = false);
		VSBufferUnorderAccess();
		
		virtual bool OnLoadResource(VSResourceIdentifier *&pID);
		
		unsigned int m_uiNum;
		unsigned int m_uiDT;
		unsigned int m_uiStructStride;
	};
	DECLARE_Ptr(VSBufferUnorderAccess);
	VSTYPE_MARCO(VSBufferUnorderAccess);
}

