#pragma once
#include "VSPass.h"
namespace VSEngine2
{

	class VSGRAPHIC_API VSLightFunPass : public VSPass
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
	public:
		virtual ~VSLightFunPass();
		VSLightFunPass();
	public:


	protected:

		DECLARE_INITIAL
		static bool InitialDefaultState();
		static bool TerminalDefaultState();

	public:
		virtual bool Draw(VSRenderer * pRenderer);

		virtual unsigned int GetPassType()
		{
			return PT_LIGHT_FUNCTION;
		}
		inline void SetDrawContent(VSMaterialInstance * pMaterialInstance, VSGeometry * pGeometry,
			VSCamera * pCamera, VSLight * pLight,unsigned int uiPassId)
		{
			VSPass::SetDrawContent(pMaterialInstance, pGeometry, pCamera, uiPassId);
			MSPara.pShadowLight = pLight;

		}
		VSRenderState m_RenderState;
	};
	DECLARE_Ptr(VSLightFunPass);
	VSTYPE_MARCO(VSLightFunPass);
}
