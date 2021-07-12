#pragma once
#include "VSMemManager.h"
#include "VSMath.h"
#include "VSObject.h"
#include "VSTexAllState.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSUserSampler : public VSObject
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		
		VSUserSampler(const VSUsedName & Name,unsigned int uiTexType,unsigned int uiRegisterIndex,unsigned int uiRegisterNum);
		virtual ~VSUserSampler();

		inline unsigned int GetTexType()const
		{
			return m_uiTexType;
		}
		
		inline VSTexAllState * GetTex(unsigned int uiIndex)const
		{
			return m_pTextureArray[uiIndex];
		}
		inline const VSUsedName & GetName()const
		{
			return m_Name;
		}
		inline void SetTex(VSTexAllState * pTex,unsigned int uiIndex = 0)
		{
			if (uiIndex >= m_uiRegisterNum)
			{
				return ;
			}
			m_pTextureArray[uiIndex] = pTex;
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
		VSUserSampler();
	private:

		unsigned int m_uiTexType;
		VSArray<VSTexAllStatePtr> m_pTextureArray;
		VSUsedName		m_Name;
		unsigned int m_uiRegisterIndex;
		unsigned int m_uiRegisterNum;
	};
	VSTYPE_MARCO(VSUserSampler);
	DECLARE_Ptr(VSUserSampler);
}
