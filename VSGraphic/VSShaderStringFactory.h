#pragma once
#include "VSMaterial.h"
#include "VSVShader.h"
#include "VSPShader.h"
#include "VSGShader.h"
#include "VSDShader.h"
#include "VSHShader.h"
#include "VSLight.h"
#include "VSResourceManager.h"
namespace VSEngine2
{
#define ms_LightShadowMatrix ms_ViewProjectMatrix
	class VSGRAPHIC_API VSShaderStringFactory
	{
	public:	

		static VSString ms_TextureInputCoordValue[UV_LEVEL];
		static VSString ms_PSOutputColorValue;
		static VSString ms_PSOutputColorValue1;

		static VSString ms_InputVertexID;
		static VSString ms_InputColor[2];
		static VSString ms_InputProjectZ;
		static VSString ms_InputLocalNormal;

		static VSString ms_InputLocalBinormal;
		static VSString ms_InputLocalTangent;
		static VSString ms_InputLocalPos;
		static VSString ms_InputWorldPos;
		static VSString ms_InputTessellationValue;
		static VSString ms_InputInstancePos[3];

		static VSString ms_InputInstanceAnimData;
		static VSString ms_InputInstanceAnimNum;

		static VSArray<VSString> ms_InputMaterialInstance;

		static VSUsedName ms_PSConstantShadowResource[MAX_DYNAMIC_LIGHT];
		static VSUsedName ms_PSConstantLightFunResource[MAX_DYNAMIC_LIGHT];
		static VSUsedName ms_PSOrenNayarLookUpTableResource;
		enum 
		{
			SS_Shadow = VSEngineFlag::SS_SHARE_2L_B1 - 1,
			SS_LightFun = VSEngineFlag::SS_SHARE_2L_B0 - 1,
			SS_OrenNayarLookUpTable = VSEngineFlag::SS_SHARE_3L_W - 1,
		};

		static VSUsedName ms_VSMorphVDataResource;
		static VSUsedName ms_VSMorphNDataResource;
		static VSUsedName ms_VSMorphTDataResource;
		static VSUsedName ms_VSMorphBDataResource;
		
		static VSUsedName ms_VSAnimDataResource;

		static VSUsedName ms_PSColorBufferResource;
		static VSUsedName ms_PSDepthNormalBufferResource;

		static VSUsedName ms_ViewProjectMatrix;
		static VSUsedName ms_ViewMatrix;
		static VSUsedName ms_BoneMatrix;
		static VSUsedName ms_WorldMatrix;
		static VSUsedName ms_ProjectMatrix;

		static VSUsedName ms_MorphData;// x is Layer , y is Num
		static VSUsedName ms_MorphValue;

		static VSUsedName ms_AnimAllInfo; //x is bone Num, y is AnimDataTextureWidth,z is AnimDataTextureHeight

		static VSUsedName ms_LightWorldPos;
		static VSUsedName ms_LightWorldDirection;

		static VSUsedName ms_WorldNormal;
		static VSUsedName ms_VertexWorldNormal;

		static VSUsedName ms_ViewNormal;
		static VSUsedName ms_VertexViewNormal;

		static VSUsedName ms_TessellationValue;

		static VSUsedName ms_WorldPos;
		static VSUsedName ms_ViewPos;
		static VSUsedName ms_ProjectPos;

		static VSUsedName ms_ViewWorldDir;
		static VSUsedName ms_CameraWorldPos;

		static VSUsedName ms_LightColor;
		static VSUsedName ms_LightSpecular;
		static VSUsedName ms_LightDir;
		static VSUsedName ms_LightName[VSLight::LT_MAX];
		static VSUsedName ms_LightNameTemp[VSLight::LT_MAX];

		static VSUsedName ms_FarZ;
		static VSUsedName ms_PointLightRange;

		static VSUsedName ms_InvRTWidth;

		static VSUsedName ms_SkyLightUpColor;
		static VSUsedName ms_SkyLightDownColor;
		
		static VSUsedName ms_ShareSampler[VSEngineFlag::ms_uiShaderSamplerNum];
		static unsigned int ms_uiShareSamplerRegisterID[VSEngineFlag::ms_uiShaderSamplerNum];
		static unsigned int GetMaxShareSamplerRegisterID();

		static void AddShaderSamplerRegisterID(VSShader * pShader, unsigned int uiTexRegisterID, unsigned int ShareSamplerType);
		static unsigned int ms_uiCreateVShaderNum;
		static unsigned int ms_uiCreatePShaderNum;
		static unsigned int ms_uiCreateGShaderNum;
		static unsigned int ms_uiCreateHShaderNum;
		static unsigned int ms_uiCreateDShaderNum;

		static unsigned int ms_ShaderValueIndex;

		
		//Create Shader


		static bool CreateVShaderString(VSVShader * pVShader,MaterialShaderPara &MSPara,unsigned int uiShaderID,VSString & VShaderString
#if _DEBUG 
			,VSString &OutShaderFileName);
#else
			);
#endif
		static bool CreatePShaderString(VSPShader * pPShader,MaterialShaderPara &MSPara,unsigned int uiShaderID,VSString & PShaderString
#if _DEBUG 
			,VSString &OutShaderFileName);
#else
			);
#endif
		
		static bool CreateGShaderString(VSGShader * pGShader, MaterialShaderPara &MSPara,unsigned int uiShaderID, VSString & GShaderString
#if _DEBUG 
			, VSString &OutShaderFileName);
#else
			);
#endif
		static bool CreateHShaderString(VSHShader * pHShader, MaterialShaderPara &MSPara, unsigned int uiShaderID, VSString & HShaderString
#if _DEBUG 
			, VSString &OutShaderFileName);
#else
			);
#endif
		static bool CreateDShaderString(VSDShader * pDShader, MaterialShaderPara &MSPara, unsigned int uiShaderID, VSString & DShaderString
#if _DEBUG 
			, VSString &OutShaderFileName);
#else
			);
#endif

	protected:
 		static void ClearAllString();
#if _DEBUG 
		static void OutPutShaderLog(MaterialShaderPara &MSPara, unsigned int uiShaderID,
			const VSString & ShaderCode, VSString &OutShaderTextName);
#endif
	};

}
