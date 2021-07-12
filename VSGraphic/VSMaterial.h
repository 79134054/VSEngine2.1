#pragma once
#include "VSObject.h"
#include "VSShaderMainFunction.h"
#include "VSTexAllState.h"
#include "VSLight.h"
#include "VSResource.h"
#include "VSMaterialPass.h"
#include "VSNormalDepthPass.h"
#include "VSShaderKey.h"
#include "VSRenderState.h"
#include "VSPass.h"
#include "VSShadowPass.h"
#include "VSPreZPass.h"
#include "VSLightShaderFunction.h"
#include "VSPostEffectShaderFunction.h"
#include "VSLightFunPass.h"
#include "VSIndirectRenderPass.h"
#include "VSPostEffectPass.h"
#include "VSBufferResource.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API MaterialCustomPara : public VSObject
	{
	public:
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY;
		MaterialCustomPara();
		virtual ~MaterialCustomPara() = 0;

		VSUsedName ConstValueName;

		MaterialCustomPara & operator =(const MaterialCustomPara &Para)
		{
			ConstValueName = Para.ConstValueName;
			return *this;
		}
		VSGRAPHIC_API friend bool operator >(const MaterialCustomPara &Para1,const MaterialCustomPara &Para2)
		{
			return Para2.ConstValueName > Para1.ConstValueName;
		}
		VSGRAPHIC_API friend bool operator <(const MaterialCustomPara &Para1,const MaterialCustomPara &Para2)
		{
			return Para2.ConstValueName < Para1.ConstValueName;
		}
		VSGRAPHIC_API friend bool operator ==(const MaterialCustomPara &Para1,const MaterialCustomPara &Para2)
		{
			return Para2.ConstValueName == Para1.ConstValueName;
		}
	};
	DECLARE_Ptr(MaterialCustomPara);
	VSTYPE_MARCO(MaterialCustomPara);
	class VSGRAPHIC_API CustomFloatValue : public MaterialCustomPara
	{
	public:
		DECLARE_RTTI;
		//PRIORITY

		DECLARE_INITIAL
		CustomFloatValue();
		virtual ~CustomFloatValue();
		VSArray<VSREAL> Value;
		CustomFloatValue & operator =(const CustomFloatValue &Para)
		{
			MaterialCustomPara::operator =(Para);
			Value = Para.Value;
			return *this;
		}
		
	};
	DECLARE_Ptr(CustomFloatValue);
	VSTYPE_MARCO(CustomFloatValue);
	class CustomTexSampler : public MaterialCustomPara
	{
	public:
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
		CustomTexSampler();
		virtual ~CustomTexSampler();
		VSTexAllStateRPtr m_pTexture;
		CustomTexSampler & operator =(const CustomTexSampler &Para)
		{
			MaterialCustomPara::operator =(Para);
			m_pTexture = Para.m_pTexture;
			return *this;
		}
	};
	DECLARE_Ptr(CustomTexSampler);
	VSTYPE_MARCO(CustomTexSampler);
	class CustomBuffer : public MaterialCustomPara
	{
	public:
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
		CustomBuffer();
		virtual ~CustomBuffer();
		VSBufferResourcePtr m_pBufferResource;
		CustomBuffer & operator =(const CustomBuffer &Para)
		{
			MaterialCustomPara::operator =(Para);
			m_pBufferResource = Para.m_pBufferResource;
			return *this;
		}
	};
	DECLARE_Ptr(CustomBuffer);
	VSTYPE_MARCO(CustomBuffer);
	class VSGRAPHIC_API VSCustomMaterialInterface : public VSObject
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY;
	public:
		
		VSCustomMaterialInterface();
		virtual ~VSCustomMaterialInterface() = 0;
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
	protected:
		VSUsedName	m_ResourceName;
	};
	DECLARE_Ptr(VSCustomMaterialInterface);
	VSTYPE_MARCO(VSCustomMaterialInterface);
	class VSGRAPHIC_API VSCustomCSMaterial : public VSCustomMaterialInterface
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSCustomCSMaterial(const VSUsedName & ResourceName,
			const TCHAR * pCShaderFileName, const TCHAR * pCMainFunName);
		virtual ~VSCustomCSMaterial();
		void PreLoad(const VSUsedName & ResourceName,
			const TCHAR * pCShaderFileName, const TCHAR * pCMainFunName);
		friend class VSRenderer;
		virtual void SetCShaderValue(const VSUsedName & Name, void *fValue);
		virtual void SetCShaderTexture(const VSUsedName & TexSamplerNodeName, VSTexAllState * pTex);
		virtual void SetCShaderBuffer(const VSUsedName & Name, VSBufferResource *pBuffer);
		VSCShader *GetCurCShader(VSShaderKey & CShaderKey);
	protected:

		bool GetCShader(VSShaderKey & CShaderKey);
		VSCustomCSMaterial();

		VSCShaderPtr m_pCShader;
		VSUsedName m_CShaderName;
		VSString m_CMainFunName;
		VSShaderSet * m_pCShaderSet;
	public:
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
		static void LoadAllDeviceShader();
		static VSCustomCSMaterial * GetTestStruct()
		{
			return ms_pTestStruct;
		}
		static VSCustomCSMaterial * GetTestByteBuffer()
		{
			return ms_pTestByteBuffer;
		}
		static VSCustomCSMaterial * GetTestTexture()
		{
			return ms_pTestTexture;
		}
		bool UseMaterial(unsigned int uiThreadGroupCountX, unsigned int uiThreadGroupCountY, unsigned int uiThreadGroupCountZ);
		virtual bool AddUnorderAccess(VSUnorderAccess * pUnorderAccess);		
		virtual void ClearUnorderAccess();
	protected:
		void SetUnorderAccesses();
		void EndUnorderAccesses();
		static VSPointer<VSCustomCSMaterial> ms_pTestStruct;
		static VSPointer<VSCustomCSMaterial> ms_pTestByteBuffer;
		static VSPointer<VSCustomCSMaterial> ms_pTestTexture;
		static bool LoadDefault();
		void LoadDeviceShader();
		VSArray<VSUnorderAccessPtr> m_pUnorderAccessArray;
		
	};
	DECLARE_Ptr(VSCustomCSMaterial);
	VSTYPE_MARCO(VSCustomCSMaterial);
	class VSGRAPHIC_API VSCustomMaterial : public VSCustomMaterialInterface
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSCustomMaterial(const VSUsedName & ResourceName,
						const TCHAR * pVShaderFileName,const TCHAR * pVMainFunName,
						const TCHAR * pPShaderFileName,const TCHAR * pPMainFunName,
						const TCHAR * pGShaderFileName = NULL, const TCHAR * pGMainFunName = NULL,
						const TCHAR * pHShaderFileName = NULL, const TCHAR * pHMainFunName = NULL,
						const TCHAR * pDShaderFileName = NULL, const TCHAR * pDMainFunName = NULL);
		virtual ~VSCustomMaterial();
		bool UseMaterial();
		void PreLoad(const VSUsedName & ResourceName,
			const TCHAR * pVShaderFileName,const TCHAR * pVMainFunName,
			const TCHAR * pPShaderFileName,const TCHAR * pPMainFunName,
			const TCHAR * pGShaderFileName = NULL, const TCHAR * pGMainFunName = NULL,
			const TCHAR * pHShaderFileName = NULL, const TCHAR * pHMainFunName = NULL,
			const TCHAR * pDShaderFileName = NULL, const TCHAR * pDMainFunName = NULL);
		friend class VSRenderer;
		static VSCustomMaterial * GetPostGray()
		{
			return ms_pPostGray;
		}
		static VSCustomMaterial * GetGammaCorrect()
		{
			return ms_pGammaCorrect;
		}
		static VSCustomMaterial * GetPostScreenQuad()
		{
			return ms_pPostScreenQuad;
		}
		static VSCustomMaterial * GetPostSaturation()
		{
			return ms_pPostSaturation;
		}
		static VSCustomMaterial * GetOldPhoto()
		{
			return ms_pOldPhoto;
		}
		static VSCustomMaterial * GetGuassBlurH()
		{
			return ms_pGuassBlurH;
		}
		static VSCustomMaterial * GetGuassBlurV()
		{
			return ms_pGuassBlurV;
		}
		static VSCustomMaterial * GetFilter3X3()
		{
			return ms_pFilter3X3;
		}
		static VSCustomMaterial * GetBright()
		{
			return ms_pBright;
		}
		static VSCustomMaterial * GetTexAdd()
		{
			return ms_pTexAdd;
		}


		static VSCustomMaterial * GetPostVolumeShadowMap()
		{
			return ms_pPostVolumeShadowMap;
		}
		static VSCustomMaterial * GetSSR()
		{
			return ms_pSSR;
		}
		static VSCustomMaterial * GetProjectShadowPre()
		{
			return ms_pProjectShadowPre;
		}
		static VSCustomMaterial * GetProjectShadow()
		{
			return ms_pProjectShadow;
		}
		static VSCustomMaterial * GetFont()
		{
			return ms_pFont;
		}

		static VSCustomMaterial * GetClearColorRange()
		{
			return ms_pClearColorRange;
		}
		static VSCustomMaterial * GetClearDepthRange()
		{
			return ms_pClearDepthRange;
		}
		static VSCustomMaterial * GetClearColorDepthRange()
		{
			return ms_pClearColorDepthRange;
		}
		static VSCustomMaterial * GetClearAllRange()
		{
			return ms_pClearAllRange;
		}
		static VSCustomMaterial * GetClearDepthStencilRange()
		{
			return ms_pClearDepthStencilRange;
		}
		
		static VSCustomMaterial * GetOcclustionCullPass()
		{
			return ms_pOcclusionCull;
		}

		virtual void SetVShaderValue(const VSUsedName & Name,void *fValue);
		virtual void SetVShaderTexture(const VSUsedName & TexSamplerNodeName,VSTexAllState * pTex);

		virtual void SetPShaderValue(const VSUsedName & Name,void *fValue);
		virtual void SetPShaderTexture(const VSUsedName & TexSamplerNodeName,VSTexAllState * pTex);

		virtual void SetGShaderValue(const VSUsedName & Name, void *fValue);
		virtual void SetGShaderTexture(const VSUsedName & TexSamplerNodeName, VSTexAllState * pTex);

		virtual void SetHShaderValue(const VSUsedName & Name, void *fValue);
		virtual void SetHShaderTexture(const VSUsedName & TexSamplerNodeName, VSTexAllState * pTex);

		virtual void SetDShaderValue(const VSUsedName & Name, void *fValue);
		virtual void SetDShaderTexture(const VSUsedName & TexSamplerNodeName, VSTexAllState * pTex);

		VSVShader *GetCurVShader(VSShaderKey & VShaderKey);
		VSPShader *GetCurPShader(VSShaderKey & PShaderKey);
		VSGShader *GetCurGShader(VSShaderKey & GShaderKey);
		VSHShader *GetCurHShader(VSShaderKey & HShaderKey);
		VSDShader *GetCurDShader(VSShaderKey & DShaderKey);

		VSRenderState & GetRenderState();
		void SetBlendState(VSBlendState * pBlendState);
		void SetDepthStencilState(VSDepthStencilState *pDepthStencilState);
		void SetRasterizerState(VSRasterizerState *pRasterizerState);
		void AddClipPlane(const VSPlane3 & Plane);
		void AddScissorRect(const VSRect2 & Rect);
	protected:
		bool GetVShader(VSShaderKey & VShaderKey);
		bool GetPShader(VSShaderKey & PShaderKey);
		bool GetGShader(VSShaderKey & GShaderKey);
		bool GetHShader(VSShaderKey & HShaderKey);
		bool GetDShader(VSShaderKey & DShaderKey);

		VSCustomMaterial();
		VSVShaderPtr m_pVShader;
		VSPShaderPtr m_pPShader;
		VSGShaderPtr m_pGShader;
		VSHShaderPtr m_pHShader;
		VSDShaderPtr m_pDShader;
		VSRenderState	m_RenderState;

		VSUsedName m_VShaderName;
		VSUsedName m_PShaderName;
		VSUsedName m_GShaderName;
		VSUsedName m_HShaderName;
		VSUsedName m_DShaderName;

		VSString m_VMainFunName;
		VSString m_PMainFunName;
		VSString m_GMainFunName;
		VSString m_HMainFunName;
		VSString m_DMainFunName;

		VSShaderSet * m_pVShaderSet;
		VSShaderSet * m_pPShaderSet;
		VSShaderSet * m_pGShaderSet;
		VSShaderSet * m_pHShaderSet;
		VSShaderSet * m_pDShaderSet;
	public:
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
		static void LoadAllDeviceShader();
	protected:
		static VSPointer<VSCustomMaterial> ms_pPostGray;
		static VSPointer<VSCustomMaterial> ms_pGammaCorrect;
		static VSPointer<VSCustomMaterial> ms_pPostScreenQuad;
		static VSPointer<VSCustomMaterial> ms_pPostSaturation;
		static VSPointer<VSCustomMaterial> ms_pOldPhoto;
		static VSPointer<VSCustomMaterial> ms_pGuassBlurH;
		static VSPointer<VSCustomMaterial> ms_pGuassBlurV;
		static VSPointer<VSCustomMaterial> ms_pFilter3X3;
		static VSPointer<VSCustomMaterial> ms_pBright;
		static VSPointer<VSCustomMaterial> ms_pTexAdd;

		static VSPointer<VSCustomMaterial> ms_pPostVolumeShadowMap;
		static VSPointer<VSCustomMaterial> ms_pProjectShadowPre;
		static VSPointer<VSCustomMaterial> ms_pProjectShadow;		
		static VSPointer<VSCustomMaterial> ms_pSSR;
		static VSPointer<VSCustomMaterial> ms_pFont;

		static VSPointer<VSCustomMaterial> ms_pClearColorRange;
		static VSPointer<VSCustomMaterial> ms_pClearDepthRange;
		static VSPointer<VSCustomMaterial> ms_pClearColorDepthRange;
		static VSPointer<VSCustomMaterial> ms_pClearAllRange;
		static VSPointer<VSCustomMaterial> ms_pClearDepthStencilRange;

		static VSPointer<VSCustomMaterial> ms_pOcclusionCull;
		static bool LoadDefault();
		void LoadDeviceShader();
	};
	DECLARE_Ptr(VSCustomMaterial);
	VSTYPE_MARCO(VSCustomMaterial);


	class VSGRAPHIC_API VSMaterial : public VSObject, public VSResource
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		
		DECLARE_INITIAL
	protected:
		VSMaterial();
		VSUsedName m_ShowName;
		VSArray<VSShaderMainFunction*> m_pShaderMainFunction;
		VSArray<VSShaderFunction *> m_pShaderFunctionArray;
		VSLightShaderFunction * m_pLightShaderFunction;
		VSPostEffectShaderFunction * m_pPostEffectShaderFunction;

		bool m_bIsAllReadyLink;
		
		VSPassPtr m_pPass[VSPass::PT_MAX];
		bool m_bIsCombine;
		
		unsigned int m_uiTessellationPartitioning;

		VSArrayOrder<CustomFloatValue> m_ShaderCustomValue;
		VSArrayOrder<CustomTexSampler>m_ShaderCustomTex;


		virtual void SetPShaderValue(const VSUsedName & Name, void *fValue);
		virtual void SetPShaderTexture(const VSUsedName & TexSamplerNodeName, VSTexAllStateR * pTex);

	public:
		DECLARE_RESOURCE(VSMaterial, RA_ASYN_LOAD | RA_ASYN_POSTLOAD | RA_ENABLE_GC, RT_MATERIAL, MATERIAL, Resource/Material, VSCacheResource)
		enum //TessellationPartition
		{
			TP_FRACTIONAL_EVEN,
			TP_FRACTIONAL_ODD,
			TP_INTEGER,
			TP_POW2,
			TP_MAX
		};
		void SetTessellationPartitioning(unsigned int uiTessellationPartitioning);
		inline unsigned int GetTessellationPartitioning() const { return m_uiTessellationPartitioning; }
		void SetBlendState(VSBlendState * pBlendState,unsigned int uiPassId = 0);
		void SetDepthStencilState(VSDepthStencilState *pDepthStencilState,unsigned int uiPassId = 0);
		void SetRasterizerState(VSRasterizerState *pRasterizerState,unsigned int uiPassId = 0);
		void AddClipPlane(const VSPlane3 & Plane,unsigned int uiPassId = 0);
		void AddScissorRect(const VSRect2 & Rect,unsigned int uiPassId = 0);
		void AddPass(unsigned int uiMUT);
		inline VSRenderState & GetRenderState(unsigned int uiPassId)
		{
			return m_pShaderMainFunction[uiPassId]->GetRenderState();
		}
		inline void SetCombine(bool IsCombine)
		{
			m_bIsCombine = IsCombine;
		}
		inline bool GetCombine()const
		{
			return m_bIsCombine;
		}
		friend class VSRenderer;
		friend class VSMaterialInstance;
		enum //MATERIAL USED TYPE
		{
			MUT_PHONE,
			MUT_OREN_NAYAR,
			MUT_CUSTOM,
			MUT_LIGHT,
			MUT_POSTEFFECT,
			MUT_MAX
		};
		VSMaterial(const VSUsedName &ShowName, unsigned int uiMUT = MUT_PHONE);
		virtual ~VSMaterial();
		inline VSIndirectRenderPass * GetIndirectRenderPass()const
		{
			VSPass * pPass = m_pPass[VSPass::PT_INDIRECT];
			return (VSIndirectRenderPass *)pPass;
		}
		inline VSMaterialPass * GetMaterialPass()const
		{
			VSPass * pPass = m_pPass[VSPass::PT_MATERIAL];
			return (VSMaterialPass *)pPass;
		}
		inline VSNormalDepthPass * GetNormalDepthPass()const
		{
			VSPass * pPass = m_pPass[VSPass::PT_NORMALDEPTH];
			return (VSNormalDepthPass *)pPass;
		}
		inline VSCubeShadowPass *GetCubeShadowPass()const
		{
			VSPass * pPass = m_pPass[VSPass::PT_POINT_CUBE_SHADOW];
			return (VSCubeShadowPass *)pPass;

		}
		inline VSPreZPass * GetPreZPass()const
		{
			VSPass * pPass = m_pPass[VSPass::PT_PREZ];
			return (VSPreZPass *)pPass;
		}
		inline VSVolumeShadowPass * GetVolumeShadowPass()const
		{
			VSPass * pPass = m_pPass[VSPass::PT_POINT_VOLUME_SHADOW];
			return (VSVolumeShadowPass *)pPass;
		}
		inline VSShadowPass * GetShadowMapPass()const
		{
			VSPass * pPass = m_pPass[VSPass::PT_SHADOW];
			return (VSShadowPass *)pPass;
		}
		inline VSDualParaboloidShadowPass * GetDualParaboloidShadowPass()const
		{
			VSPass * pPass = m_pPass[VSPass::PT_DUAL_PARABOLOID_SHADOW];
			return (VSDualParaboloidShadowPass *)pPass;
		}
		inline VSLightFunPass * GetLightFunPass()const
		{
			VSPass * pPass = m_pPass[VSPass::PT_LIGHT_FUNCTION];
			return (VSLightFunPass *)pPass;
		}
		inline VSPostEffectPass * GetPostEffectPass()const
		{
			VSPass * pPass = m_pPass[VSPass::PT_POSTEFFECT];
			return (VSPostEffectPass *)pPass;
		}
		void AddShaderFunction(VSShaderFunction *pShaderFunction);
		void DeleteShaderFunction(VSShaderFunction *pShaderFunction);
		inline VSShaderMainFunction * GetShaderMainFunction(unsigned char uPassId = 0)const
		{
			return m_pShaderMainFunction[uPassId];
		}
		inline VSLightShaderFunction * GetLightShaderFunction()const
		{
			return m_pLightShaderFunction;
		}

		inline VSPostEffectShaderFunction * GetPostEffectShaderFunction()const
		{
			return m_pPostEffectShaderFunction;
		}
		unsigned int GetShaderMainFunctionNum()const
		{
			return m_pShaderMainFunction.GetNum();
		}
		inline const VSUsedName & GetShowName()const
		{
			return m_ShowName;
		}
		inline bool IsAllReadyLink()const
		{
			return m_bIsAllReadyLink;
		}

		void LinkAllResource();
		void ClearShaderFunctionVisitFlagFalse();
		bool GetPShaderTreeString(VSString & OutString,MaterialShaderPara &MSPara);
		bool GetVShaderTreeString(VSString & OutString, MaterialShaderPara &MSPara);
		bool GetDShaderTreeString(VSString & OutString, MaterialShaderPara &MSPara);
		bool GetCustomDeclareString(VSString & OutString, MaterialShaderPara &MSPara);

		void CreateCustomValue(VSShader * pShader);
		void CreateConstValueDeclare(VSString & OutString, unsigned int& uiRegisterID, MaterialShaderPara &MSPara);

		void CreateCustomTexture(VSShader * pShader);
		void CreateTextureDeclare(VSString & OutString, unsigned int& uiRegisterID, MaterialShaderPara &MSPara);

		void SetGlobleValue(MaterialShaderPara &MSPara,
			VSVShader * pVShader , VSPShader * pPShader,
			VSGShader * pGShader, VSHShader * pHShader, VSDShader * pDShader);
		
	public:
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
	public:
		
		
		virtual bool PostLoad(VSStream* pStream);
		virtual bool PostClone(VSObject * pObjectSrc);
	protected:
		static VSPointer<VSMaterial> ms_DefaultOnlyColor;
		static VSPointer<VSMaterial> ms_DefaultOnlyVertexColor;

		static VSPointer<VSMaterial> ms_DefaultOnlyColorDisableDepth;
		static VSPointer<VSMaterial> ms_DefaultOnlyVertexColorDisableDepth;

		static VSPointer<VSResourceProxy<VSMaterial>> ms_DefaultOnlyColorResource;

		static bool LoadDefault();
	public:
		friend class VSRenderer;
		friend class VSResourceManager;

		static const VSResourceProxy<VSMaterial> *GetDefaultOnlyColorResource()
		{
			return ms_DefaultOnlyColorResource;
		}

		static const VSMaterial *GetDefaultOnlyColor()
		{
			return ms_DefaultOnlyColor;
		}
		
		static const VSMaterial *GetDefaultOnlyVertexColor()
		{
			return ms_DefaultOnlyVertexColor;
		}
		
		static const VSMaterial *GetDefaultOnlyColorDisableDepth()
		{
			return ms_DefaultOnlyColorDisableDepth;
		}

		static const VSMaterial *GetDefaultOnlyVertexColorDisableDepth()
		{
			return ms_DefaultOnlyVertexColorDisableDepth;
		}
		// more bigger more later render
		unsigned int m_uiCustomLayer;
		void GetInstanceVertexFormat(VSArray<VSVertexFormat::VERTEXFORMAT_TYPE>& FormatArray, unsigned int &uiOffset);
		int GetInstanceIndex(const class VSConstFloatValue * pCFValue);
	};
	DECLARE_Ptr(VSMaterial);
	DECLARE_Proxy(VSMaterial);
	VSTYPE_MARCO(VSMaterial);

	class VSGRAPHIC_API VSMaterialInstance: public VSObject
	{
		//RTTI
		DECLARE_RTTI;

		DECLARE_INITIAL
		
	public:
		VSMaterialInstance(VSMaterialR * pMaterial);
		virtual ~VSMaterialInstance();
	protected:
		VSMaterialInstance();	

		VSArrayOrder<CustomFloatValue> m_ShaderCustomValue;
		VSArrayOrder<CustomTexSampler>	m_ShaderCustomTex;		

		VSVShaderPtr m_pCurVShader[VSPass::PT_MAX];
		VSPShaderPtr m_pCurPShader[VSPass::PT_MAX];
		VSGShaderPtr m_pCurGShader[VSPass::PT_MAX];
		VSHShaderPtr m_pCurHShader[VSPass::PT_MAX];
		VSDShaderPtr m_pCurDShader[VSPass::PT_MAX];
		VSMaterialRPtr m_pMaterial;

	public:
		friend class VSSpatial;
		const inline VSArrayOrder<CustomTexSampler>& GetCustomTex()const
		{
			return m_ShaderCustomTex;
		}
		void SetShaderValue(VSShader * pShader);
		void SetShaderValue(const VSUsedName & Name,void *fValue,unsigned int uiSize);
		void DeleteShaderValue(const VSUsedName &Name);
		void SetShaderTexture(const VSUsedName & TexSamplerNodeName,VSTexAllStateR * pTex);
		void DeleteShaderTexture(const VSUsedName & TexSamplerNodeName);
		void GetInstanceData(unsigned char* pInstanceDataBuffer, unsigned int &GetSize);


		void GetAllMaterialPara();
		virtual bool PostLoad(VSStream* pStream);
		virtual bool PostClone(VSObject * pObjectSrc);
		virtual void LoadedEvent(VSResourceProxyBase * pResourceProxy, void * Data = NULL);
		inline VSMaterial* GetMaterial()const
		{
			return m_pMaterial->GetResource();
		}
		inline VSMaterialR * GetMaterialR()const
		{
			return m_pMaterial;
		}
		virtual void GetStreamResource(VSArray<VSResourceProxyBase*>& pResourceProxy, StreamInformation_TYPE& StreamInformation)const;
		friend class VSMaterialPass;
		friend class VSNormalDepthPass;
		friend class VSPreZPass;
		friend class VSCubeShadowPass;
		friend class VSVolumeShadowPass;
		friend class VSShadowPass;
		friend class VSDualParaboloidShadowPass;
		friend class VSLightFunPass;
		friend class VSProjectPreShadowPass;
		friend class VSProjectShadowPass;
		friend class VSPass;
		friend class VSIndirectRenderPass;
		friend class VSPostEffectPass;
	};
	DECLARE_Ptr(VSMaterialInstance);
	VSTYPE_MARCO(VSMaterialInstance);
	class VSGRAPHIC_API VSMaterialOnlyTexture : public VSMaterial
	{
	protected:
		VSMaterialOnlyTexture();
	public:
		VSMaterialOnlyTexture(const VSUsedName &ShowName,VSTexAllStateR * pTexture);
		virtual ~VSMaterialOnlyTexture();
		
	};
	DECLARE_Ptr(VSMaterialOnlyTexture);
	VSTYPE_MARCO(VSMaterialOnlyTexture);
	class VSGRAPHIC_API VSMaterialTextureAndNormal : public VSMaterial
	{
	protected:
		VSMaterialTextureAndNormal();
	public:
		VSMaterialTextureAndNormal(const VSUsedName &ShowName,VSTexAllStateR * pDiffuseTexture,VSTexAllStateR * pNormalTexture);
		virtual ~VSMaterialTextureAndNormal();
	};
	DECLARE_Ptr(VSMaterialTextureAndNormal);
	VSTYPE_MARCO(VSMaterialTextureAndNormal);
	class VSGRAPHIC_API VSMaterialInstanceTest : public VSMaterial
	{
	protected:
		VSMaterialInstanceTest();
	public:
		VSMaterialInstanceTest(const VSUsedName &ShowName, VSTexAllStateR * pDiffuseTexture, VSTexAllStateR * pNormalTexture);
		virtual ~VSMaterialInstanceTest();
	};
	DECLARE_Ptr(VSMaterialInstanceTest);
	VSTYPE_MARCO(VSMaterialInstanceTest);
	class VSGRAPHIC_API VSMaterialOnlyEmissive : public VSMaterial
	{
	protected:
		VSMaterialOnlyEmissive();
	public:
		VSMaterialOnlyEmissive(const VSUsedName &ShowName);
		virtual ~VSMaterialOnlyEmissive();

	};
	DECLARE_Ptr(VSMaterialOnlyEmissive);
	VSTYPE_MARCO(VSMaterialOnlyEmissive);
	class VSGRAPHIC_API VSMaterialNoLight : public VSMaterial
	{
	protected:
		VSMaterialNoLight();
	public:
		VSMaterialNoLight(const VSUsedName &ShowName);
		virtual ~VSMaterialNoLight();

	};
	DECLARE_Ptr(VSMaterialNoLight);
	VSTYPE_MARCO(VSMaterialNoLight);
	class VSGRAPHIC_API VSMaterialOrenNayar : public VSMaterial
	{
	protected:
		VSMaterialOrenNayar();
	public:
		VSMaterialOrenNayar(const VSUsedName &ShowName,VSTexAllStateR * pDiffuseTexture,VSTexAllStateR * pNormalTexture,
			VSTexAllStateR * pEmissiveTexture);
		virtual ~VSMaterialOrenNayar();

	};
	DECLARE_Ptr(VSMaterialOrenNayar);
	VSTYPE_MARCO(VSMaterialOrenNayar);
	class VSGRAPHIC_API VSMaterialPhone : public VSMaterial
	{
	protected:
		VSMaterialPhone();
	public:
		VSMaterialPhone(const VSUsedName &ShowName,VSTexAllStateR * pDiffuseTexture,VSTexAllStateR * pNormalTexture,
			VSTexAllStateR * pSpecularTexture, VSTexAllStateR * pEmissiveTexture,bool bPhoneSpecular);
		virtual ~VSMaterialPhone();

	};
	DECLARE_Ptr(VSMaterialPhone);
	VSTYPE_MARCO(VSMaterialPhone);
	class VSGRAPHIC_API VSMaterialPhoneTwoPass : public VSMaterial
	{
	protected:
		VSMaterialPhoneTwoPass();
	public:
		VSMaterialPhoneTwoPass(const VSUsedName &ShowName, VSTexAllStateR * pDiffuseTexture, VSTexAllStateR * pNormalTexture,
			VSTexAllStateR * pSpecularTexture, VSTexAllStateR * pEmissiveTexture, bool bPhoneSpecular);
		virtual ~VSMaterialPhoneTwoPass();

	};
	DECLARE_Ptr(VSMaterialPhoneTwoPass);
	VSTYPE_MARCO(VSMaterialPhoneTwoPass)
	class VSGRAPHIC_API VSMaterialCustom : public VSMaterial
	{
	protected:
		VSMaterialCustom();
	public:
		VSMaterialCustom(const VSUsedName &ShowName,VSTexAllStateR * pDiffuseTexture,VSTexAllStateR * pNormalTexture,VSTexAllStateR * pEmissiveTexture);
		virtual ~VSMaterialCustom();

	};
	DECLARE_Ptr(VSMaterialCustom);
	VSTYPE_MARCO(VSMaterialCustom);
	class VSGRAPHIC_API VSLightMaterial : public VSMaterial
	{
	protected:
		VSLightMaterial();
	public:
		VSLightMaterial(const VSUsedName &ShowName,VSTexAllStateR * pDiffuseTexture);
		virtual ~VSLightMaterial();

	};
	DECLARE_Ptr(VSLightMaterial);
	VSTYPE_MARCO(VSLightMaterial);
	class VSGRAPHIC_API VSPostEffectMaterial : public VSMaterial
	{
	protected:
		VSPostEffectMaterial();
	public:
		VSPostEffectMaterial(const VSUsedName &ShowName);
		virtual ~VSPostEffectMaterial();

	};
	DECLARE_Ptr(VSPostEffectMaterial);
	VSTYPE_MARCO(VSPostEffectMaterial);

	class VSGRAPHIC_API VSMaterialPhoneWolrdOffset : public VSMaterial
	{
	protected:
		VSMaterialPhoneWolrdOffset();
	public:
		VSMaterialPhoneWolrdOffset(const VSUsedName &ShowName, VSTexAllStateR * pDiffuseTexture, VSTexAllStateR * pNormalTexture,
			VSTexAllStateR * pSpecularTexture, VSTexAllStateR * pEmissiveTexture, bool bPhoneSpecular);
		virtual ~VSMaterialPhoneWolrdOffset();

	};
	DECLARE_Ptr(VSMaterialPhoneWolrdOffset);
	VSTYPE_MARCO(VSMaterialPhoneWolrdOffset);
	class VSGRAPHIC_API VSMaterialTessellation : public VSMaterial
	{
	protected:
		VSMaterialTessellation();
	public:
		VSMaterialTessellation(const VSUsedName &ShowName, VSTexAllStateR * pDiffuseTexture, 
			VSTexAllStateR * pNormalTexture, VSTexAllStateR * pWorldDisplacementTexture);
		virtual ~VSMaterialTessellation();
	};
	DECLARE_Ptr(VSMaterialTessellation);
	VSTYPE_MARCO(VSMaterialTessellation);
}
