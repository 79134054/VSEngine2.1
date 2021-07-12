#pragma once
#include "VSMemManager.h"
#include "VSMath.h"
#include "VSObject.h"
#include "VSBufferResource.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSUserBuffer : public VSObject
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSUserBuffer(const VSUsedName & Name,unsigned int uiDataType, unsigned int uiRegisterIndex, unsigned int uiRegisterNum);
		virtual ~VSUserBuffer();

		inline unsigned int GetDataType()const
		{
			return m_uiDT;
		}

		inline const VSUsedName & GetName()const
		{
			return m_Name;
		}
		inline void SetBufferResource(VSBufferResource * pBufferResource, unsigned int uiIndex = 0)
		{
			if (uiIndex >= m_uiRegisterNum)
			{
				return;
			}
			m_pBufferResourceArray[uiIndex] = pBufferResource;
		}
		inline VSBufferResource * GetBufferResource(unsigned int uiIndex)const
		{
			return m_pBufferResourceArray[uiIndex];
		}
		inline unsigned int GetRegisterIndex()const
		{
			return m_uiRegisterIndex;
		}
		inline unsigned int GetRegisterNum()const
		{
			return m_uiRegisterNum;
		}
		virtual bool PostLoad(VSStream* pStream);
		virtual bool PostClone(VSObject * pObjectSrc);
	protected:
		friend class VSRenderer;
		VSUserBuffer();
	private:

		unsigned int m_uiDT;
		VSArray <VSBufferResourcePtr> m_pBufferResourceArray;
		VSUsedName		m_Name;
		unsigned int m_uiRegisterIndex;
		unsigned int m_uiRegisterNum;
	};
	VSTYPE_MARCO(VSUserBuffer);
	DECLARE_Ptr(VSUserBuffer);
}

