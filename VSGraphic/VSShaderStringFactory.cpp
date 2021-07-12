#include "VSShaderStringFactory.h"
#include "VSGeometry.h"
#include "VSLog.h"
#include "VSResourceManager.h"
#include "VSBoneNode.h"
#include "VSDirectionLight.h"
#include "VSPointLight.h"
#include "VSSpotLight.h"
#include "VSGraphicInclude.h"
#include "VSConfig.h"
using namespace VSEngine2;
unsigned int VSShaderStringFactory::ms_ShaderValueIndex = 0;
VSString VSShaderStringFactory::ms_TextureInputCoordValue[UV_LEVEL];
VSString VSShaderStringFactory::ms_InputVertexID;
VSString VSShaderStringFactory::ms_InputColor[2];
VSString VSShaderStringFactory::ms_PSOutputColorValue;
VSString VSShaderStringFactory::ms_PSOutputColorValue1;
VSString VSShaderStringFactory::ms_InputLocalNormal;
VSString VSShaderStringFactory::ms_InputLocalBinormal;
VSString VSShaderStringFactory::ms_InputLocalTangent;
VSString VSShaderStringFactory::ms_InputLocalPos;
VSString VSShaderStringFactory::ms_InputWorldPos;
VSString VSShaderStringFactory::ms_InputTessellationValue;
VSString VSShaderStringFactory::ms_InputProjectZ;
VSString VSShaderStringFactory::ms_InputInstancePos[3];
VSString VSShaderStringFactory::ms_InputInstanceAnimData;
VSString VSShaderStringFactory::ms_InputInstanceAnimNum;
VSArray<VSString> VSShaderStringFactory::ms_InputMaterialInstance;

VSUsedName VSShaderStringFactory::ms_PSOrenNayarLookUpTableResource = _T("Tex_OrenNayarLookUpTable");
VSUsedName VSShaderStringFactory::ms_VSMorphVDataResource = _T("Tex_MorphVData");
VSUsedName VSShaderStringFactory::ms_VSMorphNDataResource = _T("Tex_MorphNData");
VSUsedName VSShaderStringFactory::ms_VSMorphTDataResource = _T("Tex_MorphTData");
VSUsedName VSShaderStringFactory::ms_VSMorphBDataResource = _T("Tex_MorphBData");
VSUsedName VSShaderStringFactory::ms_VSAnimDataResource = _T("Tex_AnimData");
unsigned int VSShaderStringFactory::ms_uiCreateVShaderNum = 0;
unsigned int VSShaderStringFactory::ms_uiCreatePShaderNum = 0;
unsigned int VSShaderStringFactory::ms_uiCreateGShaderNum = 0;
unsigned int VSShaderStringFactory::ms_uiCreateHShaderNum = 0;
unsigned int VSShaderStringFactory::ms_uiCreateDShaderNum = 0;

VSUsedName VSShaderStringFactory::ms_ViewProjectMatrix = _T("ViewProjectMatrix");
VSUsedName VSShaderStringFactory::ms_ViewMatrix = _T("ViewMatrix");
VSUsedName VSShaderStringFactory::ms_BoneMatrix = _T("BoneVector");
VSUsedName VSShaderStringFactory::ms_WorldMatrix = _T("WorldMatrix");
VSUsedName VSShaderStringFactory::ms_ProjectMatrix = _T("ProjectMatrix");

VSUsedName VSShaderStringFactory::ms_LightName[VSLight::LT_MAX] = {_T("PointLight"),_T("SpotLight"),_T("DirLight")};
VSUsedName VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_MAX] = {_T("PointLightTemp"),_T("SpotLightTemp"),_T("DirLightTemp")};
VSUsedName VSShaderStringFactory::ms_LightColor = _T("LightColor");
VSUsedName VSShaderStringFactory::ms_LightSpecular = _T("LightSpecular");
VSUsedName VSShaderStringFactory::ms_LightDir = _T("LightDir");
VSUsedName VSShaderStringFactory::ms_FarZ = _T("FarZ");
VSUsedName VSShaderStringFactory::ms_PointLightRange =_T("PointLightRange");
VSUsedName VSShaderStringFactory::ms_LightWorldPos = _T("LightWorldPos");
VSUsedName VSShaderStringFactory::ms_LightWorldDirection = _T("LightWorldDirection");
VSUsedName VSShaderStringFactory::ms_WorldNormal = _T("WorldNormal");
VSUsedName VSShaderStringFactory::ms_VertexWorldNormal = _T("VertexWorldNormal");
VSUsedName VSShaderStringFactory::ms_ViewNormal = _T("ViewNormal");
VSUsedName VSShaderStringFactory::ms_VertexViewNormal = _T("VertexViewNormal");
VSUsedName VSShaderStringFactory::ms_WorldPos = _T("WorldPos");
VSUsedName VSShaderStringFactory::ms_ViewPos = _T("ViewPos");
VSUsedName VSShaderStringFactory::ms_ProjectPos = _T("ProjectPos");
VSUsedName VSShaderStringFactory::ms_ViewWorldDir = _T("ViewWorldDir");
VSUsedName VSShaderStringFactory::ms_CameraWorldPos = _T("CameraWorldPos");
VSUsedName VSShaderStringFactory::ms_TessellationValue = _T("TempTessellationValue");
VSUsedName VSShaderStringFactory::ms_MorphData = _T("MorphData");
VSUsedName VSShaderStringFactory::ms_MorphValue = _T("MorphValue");
VSUsedName VSShaderStringFactory::ms_AnimAllInfo = _T("AnimAllInfo");
VSUsedName VSShaderStringFactory::ms_PSConstantShadowResource[] = { _T("Tex_ConstantShadow0"),_T("Tex_ConstantShadow1"),_T("Tex_ConstantShadow2") };
VSUsedName VSShaderStringFactory::ms_PSConstantLightFunResource[] = { _T("Tex_PSConstantLightFun0"),_T("Tex_PSConstantLightFun1"),_T("Tex_PSConstantLightFun2") };
VSUsedName VSShaderStringFactory::ms_InvRTWidth = _T("InvRTWidth");
VSUsedName VSShaderStringFactory::ms_SkyLightUpColor = _T("SkyLightUpColor");
VSUsedName VSShaderStringFactory::ms_SkyLightDownColor = _T("SkyLightDownColor");

VSUsedName VSShaderStringFactory::ms_PSColorBufferResource = _T("Tex_ColorBufferSampler");
VSUsedName VSShaderStringFactory::ms_PSDepthNormalBufferResource = _T("Tex_DepthNormalBufferSampler");

VSUsedName VSShaderStringFactory::ms_ShareSampler[]
= { _T("Tex_Share_3L_C_Sampler") ,_T("Tex_Share_3L_W_Sampler"), _T("Tex_Share_2L_B1_Sampler") ,_T("Tex_Share_2L_B0_Sampler")};

unsigned int VSShaderStringFactory::ms_uiShareSamplerRegisterID[];
void VSShaderStringFactory::ClearAllString()
{
	for(unsigned int i = 0 ; i < UV_LEVEL; i++)
		ms_TextureInputCoordValue[i] = VSRenderer::ms_pRenderer->Float2Const(_T("-1"),_T("-1"));
	ms_PSOutputColorValue.Clear();
	ms_PSOutputColorValue1.Clear();
	ms_InputLocalNormal.Clear();
	ms_InputLocalBinormal.Clear();
	ms_InputLocalTangent.Clear();
	ms_InputLocalPos.Clear();

	ms_InputInstancePos[0].Clear();
	ms_InputInstancePos[1].Clear();
	ms_InputInstancePos[2].Clear();


	ms_InputInstanceAnimData.Clear();
	ms_InputInstanceAnimNum.Clear();

	ms_InputColor[0].Clear();
	ms_InputColor[1].Clear();

	ms_InputProjectZ.Clear();

	ms_InputMaterialInstance.Clear();

	for (unsigned int i = 0 ; i < VSEngineFlag::ms_uiShaderSamplerNum ; i++)
	{
		ms_uiShareSamplerRegisterID[i] = INVALID_SIMPLAR_REGISTER;
	}
}
void VSShaderStringFactory::AddShaderSamplerRegisterID(VSShader * pShader,unsigned int uiTexRegisterID, unsigned int ShareSamplerType)
{
	if (pShader->m_uiShareSamplerRegister.GetNum() == 0)
	{
		unsigned int CurMaxShareSamplerRegisterID = GetMaxShareSamplerRegisterID();
		if (CurMaxShareSamplerRegisterID == INVALID_SIMPLAR_REGISTER)
		{
			VSMAC_ASSERT(uiTexRegisterID < VSRenderer::ms_pRenderer->GetMaxSampler(pShader->GetShaderType()));
			pShader->AddShareSampler(uiTexRegisterID, ShareSamplerType);
		}
		else
		{
			pShader->AddShareSampler(CurMaxShareSamplerRegisterID + 1, ShareSamplerType);
		}
	}
	else
	{
		unsigned int CurMaxShareSamplerRegisterID = pShader->GetMaxShareSampler();
		pShader->AddShareSampler(CurMaxShareSamplerRegisterID + 1, ShareSamplerType);
	}
}
unsigned int VSShaderStringFactory::GetMaxShareSamplerRegisterID()
{
	unsigned int Min = INVALID_SIMPLAR_REGISTER;
	for (unsigned int i = 0; i < VSEngineFlag::ms_uiShaderSamplerNum; i++)
	{
		if (ms_uiShareSamplerRegisterID[i] != INVALID_SIMPLAR_REGISTER)
		{
			if (Min == INVALID_SIMPLAR_REGISTER || Min < ms_uiShareSamplerRegisterID[i])
			{
				Min = ms_uiShareSamplerRegisterID[i];
			}
		}
	}
	return Min;
}
bool VSShaderStringFactory::CreateVShaderString(VSVShader * pVShader,MaterialShaderPara &MSPara,unsigned int uiShaderID,VSString & VShaderString
#if _DEBUG 
												,VSString &OutShaderFileName)
#else
												)
#endif
{
	if (!MSPara.pGeometry || !pVShader || !VSRenderer::ms_pRenderer || !MSPara.pMaterialInstance)
	{
		return 0;
	}
	VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
	if (!pMaterial)
	{
		return 0;
	}
	MSPara.uiCompileShaderType = VSEngineFlag::ST_VERTEX;
	ClearAllString();
	ms_uiCreateVShaderNum++;

	VSString VInclude;
	VSString VDynamic;
	VSString VInputDeclare;
	VSString VOutputDeclare;
	VSString VUserConstantString;
	VSString VFunctionString;
	
	VSRenderer::ms_pRenderer->GetIncludeShader(VInclude);
	VSRenderer::ms_pRenderer->GetDynamicShader(VDynamic);
	VSRenderer::ms_pRenderer->CreateVInputDeclare(MSPara,VInputDeclare);
	VSRenderer::ms_pRenderer->CreateVOutputDeclare(MSPara,VOutputDeclare);
	VSRenderer::ms_pRenderer->CreateVFunction(MSPara,VFunctionString);
	VSRenderer::ms_pRenderer->CreateVUserConstant(pVShader, MSPara, VUserConstantString);
	VSRenderer::ms_pRenderer->GetShareSamplerDeclare(pVShader, VUserConstantString);
	VShaderString = VInclude + VDynamic + VUserConstantString + VInputDeclare + VOutputDeclare + VFunctionString;
	
#if _DEBUG 
	OutPutShaderLog(MSPara, uiShaderID, VShaderString, OutShaderFileName);
#endif
	return 1;
}
bool VSShaderStringFactory::CreatePShaderString(VSPShader * pPShader,MaterialShaderPara &MSPara,unsigned int uiShaderID,VSString & PShaderString
#if _DEBUG 
												,VSString &OutShaderFileName)
#else
												)
#endif
{
	if (!pPShader || !MSPara.pGeometry || !MSPara.pMaterialInstance || !VSRenderer::ms_pRenderer)
	{
		return 0;
	}
	VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
	MSPara.uiCompileShaderType = VSEngineFlag::ST_PIXEL;
	ClearAllString();
	ms_uiCreatePShaderNum++;
	VSString PInclude;
	VSString PDynamic;
	VSString PInputDeclare;
	VSString POutputDeclare;
	VSString PUserConstantstring;
	VSString PFunctionString;
	
	VSRenderer::ms_pRenderer->GetIncludeShader(PInclude);
	VSRenderer::ms_pRenderer->GetDynamicShader(PDynamic);	
	VSRenderer::ms_pRenderer->CreatePInputDeclare(MSPara,PInputDeclare);
	VSRenderer::ms_pRenderer->CreatePOutputDeclare(MSPara,POutputDeclare);
	VSRenderer::ms_pRenderer->CreatePFunction(MSPara,PFunctionString);
	VSRenderer::ms_pRenderer->CreatePUserConstant(pPShader,MSPara,PUserConstantstring);
	VSRenderer::ms_pRenderer->GetShareSamplerDeclare(pPShader, PUserConstantstring);
	PShaderString = PInclude + PDynamic + PUserConstantstring + PInputDeclare + POutputDeclare + PFunctionString;
	

#if _DEBUG 
	OutPutShaderLog(MSPara, uiShaderID, PShaderString, OutShaderFileName);
#endif
	return 1;
}
bool VSShaderStringFactory::CreateGShaderString(VSGShader * pGShader, MaterialShaderPara &MSPara,unsigned int uiShaderID, VSString & GShaderString
#if _DEBUG 
	, VSString &OutShaderFileName)
#else
	)
#endif
{
	if (!pGShader || !MSPara.pGeometry || !MSPara.pMaterialInstance || !VSRenderer::ms_pRenderer)
	{
		return 0;
	}
	VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
	MSPara.uiCompileShaderType = VSEngineFlag::ST_GEOMETRY;
	ClearAllString();
	ms_uiCreateGShaderNum++;
	VSString GInclude;
	VSString GDynamic;
	VSString GInputDeclare;
	VSString GOutputDeclare;
	VSString GUserConstantstring;
	VSString GFunctionString;
	VSRenderer::ms_pRenderer->GetIncludeShader(GInclude);
	VSRenderer::ms_pRenderer->GetDynamicShader(GDynamic);	
	VSRenderer::ms_pRenderer->CreateGInputDeclare(MSPara,GInputDeclare);
	VSRenderer::ms_pRenderer->CreateGOutputDeclare(MSPara,GOutputDeclare);
	VSRenderer::ms_pRenderer->CreateGFunction(MSPara,GFunctionString);
	VSRenderer::ms_pRenderer->CreateGUserConstant(pGShader, MSPara, GUserConstantstring);
	GShaderString = GInclude + GDynamic + GUserConstantstring + GInputDeclare + GOutputDeclare + GFunctionString;
	for (unsigned int i = 0; i < VSEngineFlag::ms_uiShaderSamplerNum; i++)
	{
		if (ms_uiShareSamplerRegisterID[i] != INVALID_SIMPLAR_REGISTER)
		{
			pGShader->AddShareSampler(ms_uiShareSamplerRegisterID[i], i);
		}
	}

#if _DEBUG 
	OutPutShaderLog(MSPara, uiShaderID, GShaderString, OutShaderFileName);
#endif
	return 1;
}
bool VSShaderStringFactory::CreateHShaderString(VSHShader * pHShader, MaterialShaderPara &MSPara, unsigned int uiShaderID, VSString & HShaderString
#if _DEBUG 
	, VSString &OutShaderFileName)
#else
	)
#endif
{
	if (!pHShader || !MSPara.pGeometry || !MSPara.pMaterialInstance || !VSRenderer::ms_pRenderer)
	{
		return 0;
	}
	VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
	MSPara.uiCompileShaderType = VSEngineFlag::ST_HULL;
	ClearAllString();
	ms_uiCreateHShaderNum++;
	VSString HInclude;
	VSString HDynamic;
	VSString HInputDeclare;
	VSString HOutputDeclare;
	VSString HUserConstantstring;
	VSString HFunctionString;
	
	VSRenderer::ms_pRenderer->GetIncludeShader(HInclude);
	VSRenderer::ms_pRenderer->GetDynamicShader(HDynamic); 	
 	VSRenderer::ms_pRenderer->CreateHInputDeclare(MSPara, HInputDeclare);
 	VSRenderer::ms_pRenderer->CreateHOutputDeclare(MSPara, HOutputDeclare);
 	VSRenderer::ms_pRenderer->CreateHFunction(MSPara, HFunctionString);
	VSRenderer::ms_pRenderer->CreateHUserConstant(pHShader, MSPara, HUserConstantstring);
	VSRenderer::ms_pRenderer->GetShareSamplerDeclare(pHShader, HUserConstantstring);
	HShaderString = HInclude + HDynamic + HUserConstantstring + HInputDeclare + HOutputDeclare + HFunctionString;
	
#if _DEBUG 
	OutPutShaderLog(MSPara, uiShaderID, HShaderString, OutShaderFileName);
#endif
		return 1;
}
bool VSShaderStringFactory::CreateDShaderString(VSDShader * pDShader, MaterialShaderPara &MSPara, unsigned int uiShaderID, VSString & DShaderString
#if _DEBUG 
	, VSString &OutShaderFileName)
#else
	)
#endif
{
	if (!pDShader || !MSPara.pGeometry || !MSPara.pMaterialInstance || !VSRenderer::ms_pRenderer)
	{
		return 0;
	}
	VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
	MSPara.uiCompileShaderType = VSEngineFlag::ST_DOMAIN;
	ClearAllString();
	ms_uiCreateDShaderNum++;
	VSString DInclude;
	VSString DDynamic;
	VSString DInputDeclare;
	VSString DOutputDeclare;
	VSString DUserConstantstring;
	VSString DFunctionString;
	
	VSRenderer::ms_pRenderer->GetIncludeShader(DInclude);
	VSRenderer::ms_pRenderer->GetDynamicShader(DDynamic);	
	VSRenderer::ms_pRenderer->CreateDInputDeclare(MSPara, DInputDeclare);
	VSRenderer::ms_pRenderer->CreateDOutputDeclare(MSPara, DOutputDeclare);
	VSRenderer::ms_pRenderer->CreateDFunction(MSPara, DFunctionString);
	VSRenderer::ms_pRenderer->CreateDUserConstant(pDShader, MSPara, DUserConstantstring);
	VSRenderer::ms_pRenderer->GetShareSamplerDeclare(pDShader, DUserConstantstring);
	DShaderString = DInclude + DDynamic + DUserConstantstring + DInputDeclare + DOutputDeclare + DFunctionString;
	

#if _DEBUG 
	OutPutShaderLog(MSPara, uiShaderID, DShaderString, OutShaderFileName);
#endif
	return 1;
}
#if _DEBUG 
void VSShaderStringFactory::OutPutShaderLog(MaterialShaderPara &MSPara, unsigned int uiShaderID,
	const VSString & ShaderCode, VSString &OutShaderTextName)
{
	VSString ShaderType[VSEngineFlag::ST_MAX] = {_T("VShader.txt"),_T("PShader.txt"),_T("GShader.txt"),_T("HShader.txt"),_T("DShader.txt") };
	VSLog ShaderText;
	OutShaderTextName = VSConfig::ms_OutputShaderCodePath + VSRenderer::ms_pRenderer->GetRenderTypeShaderPath();
	VSString ShaderID = IntToString(uiShaderID);
	unsigned int uiPassType = MSPara.uiPassType;
	VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
	if (uiPassType == VSPass::PT_MATERIAL)
	{
		OutShaderTextName += pMaterial->GetShowName().GetString() + ShaderID + ShaderType[MSPara.uiCompileShaderType];
	}
	else if (uiPassType == VSPass::PT_NORMALDEPTH)
	{
		OutShaderTextName += pMaterial->GetShowName().GetString() + _T("_NormalDepth") + ShaderID + ShaderType[MSPara.uiCompileShaderType];
	}
	else if (uiPassType == VSPass::PT_PREZ)
	{
		OutShaderTextName += pMaterial->GetShowName().GetString() + _T("_PreZ") + ShaderID + ShaderType[MSPara.uiCompileShaderType];
	}
	else if (uiPassType == VSPass::PT_POINT_CUBE_SHADOW)
	{
		OutShaderTextName += pMaterial->GetShowName().GetString() + _T("_CubeShadow") + ShaderID + ShaderType[MSPara.uiCompileShaderType];
	}
	else if (uiPassType == VSPass::PT_POINT_VOLUME_SHADOW)
	{
		OutShaderTextName += pMaterial->GetShowName().GetString() + _T("_PointVolumeShadow") + ShaderID + ShaderType[MSPara.uiCompileShaderType];
	}
	else if (uiPassType == VSPass::PT_DIRECT_VOLUME_SHADOW)
	{
		OutShaderTextName += pMaterial->GetShowName().GetString() + _T("_DirectVolumeShadow") + ShaderID + ShaderType[MSPara.uiCompileShaderType];
	}
	else if (uiPassType == VSPass::PT_SHADOW)
	{
		OutShaderTextName += pMaterial->GetShowName().GetString() + _T("_Shadow") + ShaderID + ShaderType[MSPara.uiCompileShaderType];
	}
	else if (uiPassType == VSPass::PT_DUAL_PARABOLOID_SHADOW)
	{
		OutShaderTextName += pMaterial->GetShowName().GetString() + _T("_DualParaboloidShadow") + ShaderID + ShaderType[MSPara.uiCompileShaderType];
	}
	else if (uiPassType == VSPass::PT_LIGHT_FUNCTION)
	{
		OutShaderTextName += pMaterial->GetShowName().GetString() + _T("_LightFun") + ShaderID + ShaderType[MSPara.uiCompileShaderType];
	}
	else if (uiPassType == VSPass::PT_INDIRECT)
	{
		OutShaderTextName += pMaterial->GetShowName().GetString() + _T("_Indirect") + ShaderID + ShaderType[MSPara.uiCompileShaderType];
	}
	else if (uiPassType == VSPass::PT_POSTEFFECT)
	{
		OutShaderTextName += pMaterial->GetShowName().GetString() + _T("_PostEffect") + ShaderID + ShaderType[MSPara.uiCompileShaderType];
	}
	ShaderText.Open(OutShaderTextName.GetBuffer());
	ShaderText.WriteInfo(ShaderCode.GetBuffer());
}
#endif


