#pragma once
#include "VSInheritBind.h"
namespace VSEngine2
{

	class VSGRAPHIC_API VSMemBind : public VSInheritBind
	{

		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY;
	public:
		
		enum	//Mem Type
		{
			MT_BOTH,
			MT_RAM,
			MT_VRAM,
			MT_MAX
		};

		VSMemBind(unsigned int uiMemType = MT_VRAM);
		virtual ~VSMemBind() = 0;

		virtual VSResourceIdentifier* GetIdentifier();
		//�������Դ��Ҫlock�������Ƕ��߳���Ⱦ��Ϊ����Ч���ö���Ч�ʣ�lock������������߳�ʹ��������bmulthreadUseΪtrue
		virtual void SetStatic(bool bIsStatic, bool bMulThreadUse = false);
		void ExChange();
	protected:
		virtual void ClearInfo() = 0;
		
		unsigned int m_uiSwapChainNum;
		unsigned int m_uiCurID;
		

		unsigned int m_uiMemType;
	public:
		virtual	bool LoadResource(VSRenderer * pRender);
		virtual unsigned int GetByteSize()const = 0;

		inline void SetMemType(unsigned int uiMemType)
		{
			if (uiMemType < MT_MAX)
			{
				m_uiMemType = uiMemType;
			}
		}
		inline unsigned int GetMemType()const
		{
			return m_uiMemType;
		}
		inline unsigned int GetSwapChainNum()const
		{
			return m_uiSwapChainNum;
		}
	};
	DECLARE_Ptr(VSMemBind);
	VSTYPE_MARCO(VSMemBind);

}
