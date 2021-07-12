#pragma once
#include "VS2DTexture.h"
#include "VSDepthStencil.h"
#include "VSSpatial.h"
#include "VSScene.h"
#include "VSPostEffectSet.h"
#include "VSDebugDraw.h"
#include "VSRenderTarget.h"
#include "VSMaterial.h"
namespace VSEngine2
{
	DECLARE_Ptr(VSCustomMaterial);
	class VSGRAPHIC_API VSSceneRenderInterface : public VSReference,public VSMemObject
	{
	public:
		VSSceneRenderInterface();
		virtual ~VSSceneRenderInterface() = 0;

		virtual VSRenderTarget * GetRenderTarget(unsigned int uiIndex);

		inline void SetParam(unsigned int uiClearFlag,VSColorRGBA ClearColorRGBA,VSREAL fClearDepth,
						unsigned int uiClearStencil,bool bUseViewClear = false)
		{
			m_uiClearFlag = uiClearFlag;
			m_ClearColorRGBA = ClearColorRGBA;
			m_fClearDepth = fClearDepth;
			m_uiClearStencil = uiClearStencil;
			m_bUseViewPortClear = bUseViewClear;

		}
		virtual bool AddRenderTarget(VSRenderTarget * pTarget);
		
		void SetRenderTargets();
		void EndRenderTargets();
		virtual void ClearRTAndDepth() = 0;
	protected:
		VSArray<VSRenderTargetPtr> m_pTargetArray;
		
		unsigned int m_uiClearFlag;
		VSColorRGBA m_ClearColorRGBA;
		VSREAL m_fClearDepth;
		unsigned int m_uiClearStencil;
		bool  m_bUseViewPortClear;
		
		unsigned int m_uiRTWidth;
		unsigned int m_uiRTHeight;
		unsigned int m_uiMulSample;
	};
	DECLARE_Ptr(VSSceneRenderInterface);
	
	class VSGRAPHIC_API VSSceneRender : public VSSceneRenderInterface
	{
	public:
		VSSceneRender();
		virtual ~VSSceneRender();
		
		inline VSDepthStencil * GetDepthStencil(unsigned int uiRenderGroup)const
		{
			if (uiRenderGroup >= VSCuller::RG_MAX)
			{
				return false;
			}
			return m_pDepthStencil[uiRenderGroup];
		}	
		virtual bool SetDepthStencil(VSDepthStencil * pDepthStencil,unsigned int uiRenderGroup);
		virtual bool Draw(VSCuller & Culler,double dAppTime);
		virtual void ClearRTAndDepth();
		virtual void DrawUseCurView(VSCuller & Culler,double dAppTime);
		virtual void DisableUseCurView(VSCuller & Culler,double dAppTime);
		void SetUseState(VSRenderState & RenderState, unsigned int uiRenderStateInheritFlag);
		void ClearUseState();
	protected:
		virtual bool OnDraw(VSCuller & Culler,unsigned int uiRenderGroup,double dAppTime) = 0;
		VSDepthStencilPtr m_pDepthStencil[VSCuller::RG_MAX];
		VSRenderState m_SaveRenderState;
		VSRenderState m_UseState;
		unsigned int m_uiRenderStateInheritFlag;
	};
	DECLARE_Ptr(VSSceneRender);
	class VSGRAPHIC_API VSOcclusionQuerySceneRender : public VSSceneRender
	{
	public:
		VSOcclusionQuerySceneRender(VSCuller & Culler, VSDepthStencilPtr pDepthStencil[VSCuller::RG_MAX]);
		virtual ~VSOcclusionQuerySceneRender();
		virtual bool Draw(VSCuller & Culler, double dAppTime);
	protected:
		virtual bool OnDraw(VSCuller & Culler, unsigned int uiRenderGroup, double dAppTime);
	};
	DECLARE_Ptr(VSOcclusionQuerySceneRender);

	class VSGRAPHIC_API VSMaterialSceneRender : public VSSceneRender
	{
	public:
		VSMaterialSceneRender();
		virtual ~VSMaterialSceneRender();
		virtual void DrawUseCurView(VSCuller & Culler,double dAppTime);
		virtual void DisableUseCurView(VSCuller & Culler,double dAppTime);
		void SetNormalDepthTexture(VS2DTexture * pNormalDepthTexture)
		{
			m_pNormalDepthTexture = pNormalDepthTexture;
		}
	protected:
		virtual bool OnDraw(VSCuller & Culler,unsigned int uiRenderGroup,double dAppTime);
		VS2DTexturePtr m_pNormalDepthTexture;

		VSArray<VSRenderContext *> m_NormalAndDepth;
		VSArray<VSRenderContext *> m_NoNormalOrDepth;
		VSArray<VSRenderContext *> m_Combine;
		VSArray<VSRenderContext *> m_AlphaBlend;
		void GetGroup(VSCuller & Culler, unsigned int uiRenderGroup);
		void DrawGroup(VSCuller & Culler, unsigned int uiRenderGroup, VSArray<VSRenderContext *> & Group, double dAppTime);
		void DrawProjectShadow(VSCuller & Culler, unsigned int uiRenderGroup, double dAppTime);
		void DrawVolumeShadow(VSCuller & Culler, unsigned int uiRenderGroup, double dAppTime);
	};
	DECLARE_Ptr(VSMaterialSceneRender);

	class VSGRAPHIC_API VSDebugDrawSceneRender : public VSSceneRender
	{
	public:
		VSDebugDrawSceneRender();
		virtual ~VSDebugDrawSceneRender();
		inline VSDebugDraw * GetDebugDraw(unsigned int uiRenderGroup)
		{
			if (uiRenderGroup >= VSCuller::RG_MAX)
			{
				return false;
			}
			return m_pDebugDraw[uiRenderGroup];
		}
	protected:
		virtual bool OnDraw(VSCuller & Culler,unsigned int uiRenderGroup,double dAppTime);
		VSDebugDrawPtr m_pDebugDraw[VSCuller::RG_MAX];
	};
	DECLARE_Ptr(VSDebugDrawSceneRender);


	class VSGRAPHIC_API VSNormalDepthSceneRender : public VSSceneRender
	{
	public:
		VSNormalDepthSceneRender();
		virtual ~VSNormalDepthSceneRender();
	protected:
		virtual bool OnDraw(VSCuller & Culler,unsigned int uiRenderGroup,double dAppTime);
	};
	DECLARE_Ptr(VSNormalDepthSceneRender);


	class VSGRAPHIC_API VSPreZSceneRender : public VSSceneRender
	{
	public:
		VSPreZSceneRender();
		virtual ~VSPreZSceneRender();
	protected:
		virtual bool OnDraw(VSCuller & Culler,unsigned int uiRenderGroup,double dAppTime);
	};
	DECLARE_Ptr(VSPreZSceneRender);

	
	class VSGRAPHIC_API VSPostEffectSceneRender : public VSSceneRenderInterface
	{
	protected:
		VSRenderTarget * m_pFinalRenderTarget;
		virtual bool OnDraw(VSCuller & Culler, double dAppTime);
		VSCustomMaterialPtr m_pCustomMaterial;
		VSDepthStencilPtr m_pDepthStencil;
	public:
		virtual bool SetDepthStencil(VSDepthStencil * pDepthStencil);
		VSDepthStencil * GetDepthStencil()const
		{
			return m_pDepthStencil;
		}
		VSPostEffectSceneRender();
		virtual ~VSPostEffectSceneRender();
		virtual bool Draw(VSCuller & Culler,double dAppTime);
		virtual void ClearRTAndDepth();
		virtual void SetNoUseRTRenderSize(unsigned int uiWidth,unsigned int uiHeight);
		virtual bool AddRenderTarget(VSRenderTarget * pTarget);

		//Use By PostEffectFunction
		virtual void GetRT(unsigned int uiWidth, unsigned int uiHeight);
		virtual void DisableRT();
	};
	DECLARE_Ptr(VSPostEffectSceneRender);

	class VSGRAPHIC_API VSCommonPESceneRender : public VSPostEffectSceneRender
	{
	protected:
		VSTexAllStatePtr m_pTexAllState[POSTEFFECT_MAX_INPUT];
		virtual void OnUpdate(double dAppTime);
		virtual bool OnDraw(VSCuller & Culler, double dAppTime);
		float m_fGetRTScale = 1.0f;
	public:
		VSCommonPESceneRender();
		virtual ~VSCommonPESceneRender();
		virtual void SetSourceTarget(VSTexture * pTexture,unsigned int i = 0);
		virtual void GetRT(unsigned int uiWidth, unsigned int uiHeight);
		virtual void ClearRTAndDepth();
		VSTexture * GetInputTexture(unsigned int i) const;
	};
	DECLARE_Ptr(VSCommonPESceneRender);

	class VSGRAPHIC_API VSCombinePostEffectSceneRender : public VSCommonPESceneRender
	{
	public:
		VSCombinePostEffectSceneRender(unsigned int uiPassNum)
		{
			m_CPEArray.SetBufferNum(uiPassNum);
		}
		~VSCombinePostEffectSceneRender();
		virtual bool Draw(VSCuller & Culler, double dAppTime);
		virtual void GetRT(unsigned int uiWidth, unsigned int uiHeight);
		virtual void DisableRT();
		virtual bool AddRenderTarget(VSRenderTarget * pTarget);
		virtual VSRenderTarget * GetRenderTarget(unsigned int uiIndex);
		virtual void OnUpdate(unsigned int uiPassID,double dAppTime) = 0;
	protected:
		VSArray<VSCommonPESceneRenderPtr> m_CPEArray;
	};
	DECLARE_Ptr(VSCombinePostEffectSceneRender);

// 	class VSGRAPHIC_API VSPESSRSceneRender : public VSPostEffectSceneRender
// 	{
// 
// 	public:
// 		VSPESSRSceneRender();
// 		virtual ~VSPESSRSceneRender();	
// 		virtual void SetUseTexture(VSTexture * pNormalDepthTexture, VSTexture *pSceneColorTexture);
// 		virtual bool OnDraw(VSCuller & Culler, double dAppTime);
// 	protected:
// 		VSTexAllStatePtr m_pNormalDepthTexture;
// 		VSTexAllStatePtr m_pSceneColorTexture;
// 	};
// 	DECLARE_Ptr(VSPESSRSceneRender);

	class VSGRAPHIC_API VSPEMaterialSceneRender : public VSCommonPESceneRender
	{
	public:
		VSPEMaterialSceneRender();
		virtual ~VSPEMaterialSceneRender();
		VSMaterialInstance * m_pMaterialInstacne;
	protected:
		
		virtual bool OnDraw(VSCuller & Culler, double dAppTime);
	};
	DECLARE_Ptr(VSPEMaterialSceneRender);
	class VSGRAPHIC_API VSPEVolumeShadowMapSceneRender : public VSPostEffectSceneRender
	{
	public:
		VSPEVolumeShadowMapSceneRender();
		virtual ~VSPEVolumeShadowMapSceneRender();
	};
	DECLARE_Ptr(VSPEVolumeShadowMapSceneRender);

	class VSGRAPHIC_API VSShadowMapSceneRender : public VSSceneRender
	{
	public:
		enum //
		{
			SMT_CUB,
			SMT_SHADOWMAP,
			SMT_DUAL_PARABOLOID,
			SMT_MAX
		};
		VSShadowMapSceneRender(unsigned int uiShadowMapType);
		virtual ~VSShadowMapSceneRender();	
		VSLocalLight * m_pLocalLight;
		virtual bool Draw(VSCuller & Culler,double dAppTime);
	protected:
		virtual bool OnDraw(VSCuller & Culler,unsigned int uiRenderGroup,double dAppTime);
		unsigned int m_uiShadowMapType;
	};
	DECLARE_Ptr(VSShadowMapSceneRender);

	class VSGRAPHIC_API VSLightMaterialSceneRender : public VSSceneRenderInterface
	{
	public:

		VSLightMaterialSceneRender();
		virtual ~VSLightMaterialSceneRender();	
		class VSMaterialInstance * m_pMaterialInstacne;
		virtual bool Draw(VSCuller & Culler,double dAppTime);
		virtual bool SetDepthStencil(VSDepthStencil * pDepthStencil);
		VSDepthStencil * GetDepthStencil()const
		{
			return m_pDepthStencil;
		}
		virtual void ClearRTAndDepth();

		VSLight * m_pLight;
	protected:
		VSDepthStencilPtr m_pDepthStencil;
		virtual bool OnDraw(VSCuller & Culler, double dAppTime);
	};
	DECLARE_Ptr(VSLightMaterialSceneRender);

	class VSGRAPHIC_API VSProjectShadowSceneRender : public VSSceneRender
	{
	public:

		VSProjectShadowSceneRender();
		virtual ~VSProjectShadowSceneRender();	
		VSLocalLight * m_pLocalLight;
		virtual bool Draw(VSCuller & Culler,double dAppTime);
		VS2DTexture * m_pNormalDepthTexture;
		VSREAL m_fLightRange;
		VSVector3 m_LightWorldDirection;
		VSREAL m_Falloff;
		VSREAL m_Theta;
		VSREAL m_Phi;
	protected:
		virtual bool OnDraw(VSCuller & Culler,unsigned int uiRenderGroup,double dAppTime);
		VSTexAllStatePtr m_pTexAllState;
		VSShaderKey m_DirectionLightShaderKey;
		VSShaderKey m_PointLightShaderKey;
		VSShaderKey m_SpotLightShaderKey;

	};
	DECLARE_Ptr(VSProjectShadowSceneRender);
	class VSGRAPHIC_API VSVolumeShadowSceneRender : public VSSceneRender
	{
	public:

		VSVolumeShadowSceneRender();
		virtual ~VSVolumeShadowSceneRender();
		VSLocalLight * m_pLocalLight;
		virtual bool Draw(VSCuller & Culler, double dAppTime);
	protected:
		virtual bool OnDraw(VSCuller & Culler, unsigned int uiRenderGroup, double dAppTime);

	};
	DECLARE_Ptr(VSVolumeShadowSceneRender);
#include "VSSceneRender.marc"
	COMMON_POST_EFFECT_SCENE_RENDER_DECLARE_BEGIN(Gray,GetPostGray)
	POST_EFFECT_SCENE_RENDER_END

	COMMON_POST_EFFECT_SCENE_RENDER_DECLARE_BEGIN(GammaCorrect,GetGammaCorrect)
	POST_EFFECT_SCENE_RENDER_END

	COMMON_POST_EFFECT_SCENE_RENDER_DECLARE_BEGIN(ScreenQuad,GetPostScreenQuad)
	POST_EFFECT_SCENE_RENDER_END

	COMMON_POST_EFFECT_SCENE_RENDER_DECLARE_BEGIN(Saturate,GetPostSaturation)
	float Saturation = 0.5f;
	POST_EFFECT_SCENE_RENDER_END

	COMMON_POST_EFFECT_SCENE_RENDER_DECLARE_BEGIN(OldPhoto,GetOldPhoto)
	POST_EFFECT_SCENE_RENDER_END

	COMMON_POST_EFFECT_SCENE_RENDER_DECLARE_BEGIN(GuassBlurH,GetGuassBlurH)
	POST_EFFECT_SCENE_RENDER_END

	COMMON_POST_EFFECT_SCENE_RENDER_DECLARE_BEGIN(GuassBlurV,GetGuassBlurV)
	POST_EFFECT_SCENE_RENDER_END

	COMMON_POST_EFFECT_SCENE_RENDER_DECLARE_BEGIN(Filter3X3, GetFilter3X3)
	POST_EFFECT_SCENE_RENDER_END

	POST_EFFECT_SCENE_RENDER_SCALE(DownFilter4, VSPEFilter3X3SceneRender, 0.25f)
	POST_EFFECT_SCENE_RENDER_END

	COMMON_POST_EFFECT_SCENE_RENDER_DECLARE_BEGIN(TexAdd,GetTexAdd)
	POST_EFFECT_SCENE_RENDER_END

	COMMON_POST_EFFECT_SCENE_RENDER_DECLARE_BEGIN(Bright,GetBright)
		float MiddleGray = 0.08f;
		float WhiteCutoff = 0.18f;
		float Luminance = 0.8f;
	POST_EFFECT_SCENE_RENDER_END
	
	POST_EFFECT_SCENE_RENDER_SCALE(BloomBright, VSPEBrightSceneRender, 0.25f)
	POST_EFFECT_SCENE_RENDER_END

	COMBINE_POST_EFFECT_SCENE_RENDER_DECLARE_TWO_BEGIN(GuassBlur,VSPEGuassBlurHSceneRender, VSPEGuassBlurVSceneRender)
	POST_EFFECT_SCENE_RENDER_END
	
	POST_EFFECT_SCENE_RENDER_SCALE(BloomGuassBlur, VSPEGuassBlurSceneRender, 0.25f)
	POST_EFFECT_SCENE_RENDER_END

	COMBINE_POST_EFFECT_SCENE_RENDER_DECLARE_FOUR_BEGIN(Bloom, VSPEDownFilter4SceneRender, VSPEBloomBrightSceneRender, VSPEBloomGuassBlurSceneRender,VSPETexAddSceneRender)
		float MiddleGray = 0.08f;
		float WhiteCutoff = 0.18f;
		float Luminance = 0.8f;
	POST_EFFECT_SCENE_RENDER_END
}