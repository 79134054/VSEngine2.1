#pragma once
#include "VSPass.h"
namespace VSEngine2
{
	class VSPointLight;
	class VSGRAPHIC_API VSCubeShadowPass : public VSPass
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
	public:
		virtual ~VSCubeShadowPass();
		VSCubeShadowPass();
	public:


	protected:

		DECLARE_INITIAL
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
	protected:
		VSRenderState m_RenderState;
	public:
		virtual bool Draw(VSRenderer * pRenderer);
		inline void SetDrawContent(VSMaterialInstance * pMaterialInstance, VSGeometry * pGeometry,
			VSCamera * pCamera, VSLight * pPointLight, unsigned int uiPassId)
		{
			MSPara.pShadowLight = pPointLight;
			VSPass::SetDrawContent(pMaterialInstance,pGeometry,pCamera,uiPassId);			
		}
		virtual unsigned int GetPassType()
		{
			return PT_POINT_CUBE_SHADOW;
		}


	};

	class VSGRAPHIC_API VSVolumeShadowPass : public VSPass
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
	public:
		virtual ~VSVolumeShadowPass();
		VSVolumeShadowPass();

	protected:

		DECLARE_INITIAL
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
	protected:
		VSRenderState m_RenderState;
		void SetCustomConstant(MaterialShaderPara &MSPara, VSGShader * pGShader);
	public:
		virtual bool Draw(VSRenderer * pRenderer);
		virtual unsigned int GetPassType()
		{
			if (MSPara.pShadowLight->GetLightType() == VSLight::LT_POINT)
			{
				return PT_POINT_VOLUME_SHADOW;
			} 
			else
			{
				return PT_DIRECT_VOLUME_SHADOW;
			}
			
		}
		inline void SetDrawContent(VSMaterialInstance * pMaterialInstance, VSGeometry * pGeometry,
			VSCamera * pCamera, VSLight * pLocalLight, unsigned int uiPassId)
		{
			MSPara.pShadowLight = pLocalLight;
			VSPass::SetDrawContent(pMaterialInstance, pGeometry, pCamera, uiPassId);			
		}


	};
	class VSGRAPHIC_API VSShadowPass : public VSPass
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
	public:
		virtual ~VSShadowPass();
		VSShadowPass();
	public:


	protected:

		DECLARE_INITIAL
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
	protected:
		VSRenderState m_RenderState;
	public:
		virtual bool Draw(VSRenderer * pRenderer);

		virtual unsigned int GetPassType()
		{
			return PT_SHADOW;
		}
		inline void SetDrawContent(VSMaterialInstance * pMaterialInstance, VSGeometry * pGeometry,
			VSCamera * pCamera, VSLight * pLocalLight, unsigned int uiPassId)
		{
			MSPara.pShadowLight = pLocalLight;
			VSPass::SetDrawContent(pMaterialInstance, pGeometry, pCamera, uiPassId);
		}
	}; 

	class VSGRAPHIC_API VSDualParaboloidShadowPass : public VSPass
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
	public:
		virtual ~VSDualParaboloidShadowPass();
		VSDualParaboloidShadowPass();
	public:


	protected:

		DECLARE_INITIAL
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
	protected:
		VSRenderState m_RenderState;
	public:
		virtual bool Draw(VSRenderer * pRenderer);

		virtual unsigned int GetPassType()
		{
			return PT_DUAL_PARABOLOID_SHADOW;
		}
		inline void SetDrawContent(VSMaterialInstance * pMaterialInstance, VSGeometry * pGeometry,
			VSCamera * pCamera, VSLight * pLocalLight, unsigned int uiPassId)
		{
			MSPara.pShadowLight = pLocalLight;
			VSPass::SetDrawContent(pMaterialInstance, pGeometry, pCamera, uiPassId);
		}
	}; 

}
