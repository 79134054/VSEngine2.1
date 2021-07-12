#include "VSMaterial.h"
#include "VSTexSampler.h"
#include "VS2DTexSampler.h"
#include "VSSpatial.h"
#include "VSShaderStringFactory.h"
#include "VSConstFloatValue.h"
#include "VSBoneNode.h"
#include "VSResourceManager.h"
#include "VSVertexColor.h"
#include "VSGraphicInclude.h"
#include "VSPhoneShaderFunction.h"
#include "VSOrenNayarShaderFunction.h"
#include "VSStream.h"
#include "VSCustomShaderFunction.h"
#include "VSLightShaderFunction.h"
#include "VSPosShaderFunction.h"
#include "VSCameraShaderFunction.h"
#include "VSNormalFunction.h"
#include "VSArithmeticShaderFunction.h"
#include "VSColorBuffer.h"
using namespace VSEngine2;
IMPLEMENT_RTTI_NoCreateFun(MaterialCustomPara, VSObject)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(MaterialCustomPara)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(MaterialCustomPara,VSObject)
REGISTER_PROPERTY(ConstValueName,ConstValueName,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY

MaterialCustomPara::MaterialCustomPara()
{

}
MaterialCustomPara::~MaterialCustomPara()
{

}
IMPLEMENT_RTTI(CustomTexSampler,MaterialCustomPara)
BEGIN_ADD_PROPERTY(CustomTexSampler,MaterialCustomPara)
REGISTER_PROPERTY(m_pTexture,Texture,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(CustomTexSampler)
IMPLEMENT_INITIAL_END
CustomTexSampler::CustomTexSampler()
{
	m_pTexture = NULL;
}
CustomTexSampler::~CustomTexSampler()
{
	m_pTexture = NULL;
}
IMPLEMENT_RTTI(CustomFloatValue,MaterialCustomPara)
BEGIN_ADD_PROPERTY(CustomFloatValue,MaterialCustomPara)
REGISTER_PROPERTY(Value,Value,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(CustomFloatValue)
IMPLEMENT_INITIAL_END
CustomFloatValue::CustomFloatValue()
{

}
CustomFloatValue::~CustomFloatValue()
{

}
IMPLEMENT_RTTI(CustomBuffer, MaterialCustomPara)
BEGIN_ADD_PROPERTY(CustomBuffer, MaterialCustomPara)
REGISTER_PROPERTY(m_pBufferResource, BufferResource, VSProperty::F_SAVE_LOAD_COPY)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(CustomBuffer)
IMPLEMENT_INITIAL_END
CustomBuffer::CustomBuffer()
{

}
CustomBuffer::~CustomBuffer()
{

}
IMPLEMENT_RTTI_NoCreateFun(VSCustomMaterialInterface,VSObject)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSCustomMaterialInterface)
ADD_INITIAL_FUNCTION(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(VSCustomMaterialInterface,VSObject)
END_ADD_PROPERTY
VSCustomMaterialInterface::VSCustomMaterialInterface()
{
	
}
VSCustomMaterialInterface::~VSCustomMaterialInterface()
{
	
	
}
bool VSCustomMaterialInterface::InitialDefaultState()
{
	return 1;
}
bool VSCustomMaterialInterface::TerminalDefaultState()
{
	return 1;
}
IMPLEMENT_RTTI(VSCustomCSMaterial, VSCustomMaterialInterface)
BEGIN_ADD_PROPERTY(VSCustomCSMaterial, VSCustomMaterialInterface)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSCustomCSMaterial)
ADD_PRIORITY(VSResourceManager)
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
VSPointer<VSCustomCSMaterial> VSCustomCSMaterial::ms_pTestStruct;
VSPointer<VSCustomCSMaterial> VSCustomCSMaterial::ms_pTestByteBuffer;
VSPointer<VSCustomCSMaterial> VSCustomCSMaterial::ms_pTestTexture;
bool VSCustomCSMaterial::InitialDefaultState()
{
	ms_pTestStruct = VS_NEW VSCustomCSMaterial();
	ms_pTestByteBuffer = VS_NEW VSCustomCSMaterial();
	ms_pTestTexture = VS_NEW VSCustomCSMaterial();
	LoadDefault();
	return 1;
}
bool VSCustomCSMaterial::TerminalDefaultState()
{
	ms_pTestStruct = NULL;
	ms_pTestByteBuffer = NULL;
	ms_pTestTexture = NULL;
	return 1;
}
void VSCustomCSMaterial::LoadAllDeviceShader()
{
	ms_pTestStruct->LoadDeviceShader();
	ms_pTestByteBuffer->LoadDeviceShader();
	ms_pTestTexture->LoadDeviceShader();
}
void VSCustomCSMaterial::LoadDeviceShader()
{
	GetCurCShader(*VSShaderKey::ms_NULLShaderKey);
}
bool VSCustomCSMaterial::LoadDefault()
{
	if (!ms_pTestStruct)
	{
		return 0;
	}
	ms_pTestStruct->PreLoad(_T("TestStructCShader"), _T("TestStructCShader.txt"), _T("CSMain"));
	if (!ms_pTestByteBuffer)
	{
		return 0;
	}
	ms_pTestByteBuffer->PreLoad(_T("TestByteBufferCShader"), _T("TestByteBufferCShader.txt"), _T("CSMain"));
	if (!ms_pTestTexture)
	{
		return 0;
	}
	ms_pTestTexture->PreLoad(_T("TestTextureCShader"), _T("TestTextureCShader.txt"), _T("CSMain"));
	return 1;
}
void VSCustomCSMaterial::SetCShaderBuffer(const VSUsedName & Name, VSBufferResource *pBuffer)
{
	if (m_pCShader)
	{
		m_pCShader->SetParam(Name, pBuffer);
	}
}
void VSCustomCSMaterial::SetCShaderValue(const VSUsedName & Name, void *fValue)
{
	if (m_pCShader)
	{
		m_pCShader->SetParam(Name, fValue);
	}
}
void VSCustomCSMaterial::SetCShaderTexture(const VSUsedName & TexSamplerNodeName, VSTexAllState * pTex)
{
	if (m_pCShader)
	{
		m_pCShader->SetParam(TexSamplerNodeName, pTex);
	}
}
VSCustomCSMaterial::VSCustomCSMaterial(const VSUsedName & ResourceName,
	const TCHAR * pVShaderFileName, const TCHAR * pVMainFunName)
{
	m_pCShaderSet = NULL;
	m_pCShader = NULL;
	PreLoad(ResourceName,pVShaderFileName, pVMainFunName);
}
VSCustomCSMaterial::VSCustomCSMaterial()
{
	m_pCShader = NULL;
	m_pCShaderSet = NULL;
}
VSCustomCSMaterial::~VSCustomCSMaterial()
{
	m_pCShader = NULL;
}
void VSCustomCSMaterial::PreLoad(const VSUsedName & ResourceName,
	const TCHAR * pCShaderFileName, const TCHAR * pCMainFunName)
{
	m_CShaderName = pCShaderFileName;
	m_CMainFunName = pCMainFunName;
	m_ResourceName = ResourceName;

}
bool VSCustomCSMaterial::GetCShader(VSShaderKey & CShaderKey)
{
	unsigned int uiCShaderNum = 0;
	VSCShader * pCShader = NULL;
	if (m_CShaderName.GetBuffer() == NULL)
	{
		return false;
	}
	if (!m_pCShaderSet)
	{
		m_pCShaderSet = VSResourceManager::GetCustomComputeShaderMap().GetShaderSet(m_CShaderName);
	}



	if (m_pCShaderSet)
	{
		uiCShaderNum = m_pCShaderSet->GetNum();
	}


	if (m_pCShader == NULL)
	{
		if (m_pCShaderSet)
		{
			unsigned int uiIndex = m_pCShaderSet->Find(CShaderKey);

			if (uiIndex != m_pCShaderSet->GetNum())
			{
				VSShader * pTemp = (*m_pCShaderSet)[uiIndex].Value;
				pCShader = (VSCShader *)(pTemp);
			}
		}
		if (pCShader == NULL)
		{
			pCShader = VS_NEW VSCShader(m_CShaderName.GetBuffer(), m_CMainFunName.GetBuffer(), true);

			if (!pCShader)
			{
				return 0;
			}
			pCShader->m_ShaderKey = CShaderKey;
			VSRenderer::ms_pRenderer->LoadCShaderProgram(pCShader);
			if (m_pCShaderSet)
			{
				m_pCShaderSet->AddElement(CShaderKey, pCShader);
			}
		}
		m_pCShader = pCShader;

	}
	else
	{
		if (m_pCShader->m_ShaderKey == CShaderKey)
		{

		}
		else
		{
			if (m_pCShaderSet)
			{
				unsigned int uiIndex = m_pCShaderSet->Find(CShaderKey);

				if (uiIndex != m_pCShaderSet->GetNum())
				{
					VSShader * pTemp = (*m_pCShaderSet)[uiIndex].Value;
					pCShader = (VSCShader *)(pTemp);
				}
			}
			if (pCShader == NULL)
			{
				pCShader = VS_NEW VSCShader(m_CShaderName.GetBuffer(), m_CMainFunName.GetBuffer(), true);

				if (!pCShader)
				{
					return 0;
				}
				pCShader->m_ShaderKey = CShaderKey;
				VSRenderer::ms_pRenderer->LoadCShaderProgram(pCShader);
				if (m_pCShaderSet)
				{
					m_pCShaderSet->AddElement(CShaderKey, pCShader);
				}
			}
			m_pCShader = pCShader;
		}
	}

	if (!m_pCShaderSet)
	{
		VSResourceManager::GetCustomComputeShaderMap().SetShader(m_CShaderName, CShaderKey, pCShader);
		m_pCShaderSet = VSResourceManager::GetCustomComputeShaderMap().GetShaderSet(m_CShaderName);
	}
	return 1;
}
VSCShader *VSCustomCSMaterial::GetCurCShader(VSShaderKey & CShaderKey)
{
	if (GetCShader(CShaderKey))
	{
		return m_pCShader;
	}
	return NULL;
}
bool VSCustomCSMaterial::AddUnorderAccess(VSUnorderAccess * pUnorderAccess)
{
	m_pUnorderAccessArray.AddElement(pUnorderAccess);
	return 1;
}
void VSCustomCSMaterial::ClearUnorderAccess()
{
	m_pUnorderAccessArray.Clear();
}
void VSCustomCSMaterial::SetUnorderAccesses()
{
	VSUnorderAccess * pUnorderAccess[UNORDERACCESSS_LEVEL] = { NULL };
	unsigned int uiUAVNum = m_pUnorderAccessArray.GetNum() > UNORDERACCESSS_LEVEL ? UNORDERACCESSS_LEVEL : m_pUnorderAccessArray.GetNum();

	for (unsigned int i = 0; i < uiUAVNum; i++)
	{
		pUnorderAccess[i] = m_pUnorderAccessArray[i];
	}
	if (uiUAVNum > 0)
	{
		VSRenderer::ms_pRenderer->SetCSUnorderAccesses(pUnorderAccess, uiUAVNum);
	}

}
void VSCustomCSMaterial::EndUnorderAccesses()
{
	VSUnorderAccess * pUnorderAccess[UNORDERACCESSS_LEVEL] = { NULL };
	unsigned int uiUAVNum = m_pUnorderAccessArray.GetNum() > UNORDERACCESSS_LEVEL ? UNORDERACCESSS_LEVEL : m_pUnorderAccessArray.GetNum();

	for (unsigned int i = 0; i < uiUAVNum; i++)
	{
		pUnorderAccess[i] = m_pUnorderAccessArray[i];
	}
	if (uiUAVNum > 0)
	{
		VSRenderer::ms_pRenderer->EndCSUnorderAccesses(pUnorderAccess, uiUAVNum);
	}
}
bool VSCustomCSMaterial::UseMaterial(unsigned int uiThreadGroupCountX, unsigned int uiThreadGroupCountY, unsigned int uiThreadGroupCountZ)
{
	if (!m_pUnorderAccessArray.GetNum())
	{
		return false;
	}
	SetUnorderAccesses();
	GetCurCShader(*VSShaderKey::ms_NULLShaderKey);
	if (!VSRenderer::ms_pRenderer->SetCShader(m_pCShader))
		return 0;
	VSRenderer::ms_pRenderer->Dispath(uiThreadGroupCountX, uiThreadGroupCountY, uiThreadGroupCountZ);
	EndUnorderAccesses();
	ClearUnorderAccess();
	return 1;
}
IMPLEMENT_RTTI(VSCustomMaterial, VSCustomMaterialInterface)
BEGIN_ADD_PROPERTY(VSCustomMaterial, VSCustomMaterialInterface)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSCustomMaterial)
ADD_PRIORITY(VSResourceManager)
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
VSPointer<VSCustomMaterial> VSCustomMaterial::ms_pPostGray;
VSPointer<VSCustomMaterial> VSCustomMaterial::ms_pGammaCorrect;
VSPointer<VSCustomMaterial> VSCustomMaterial::ms_pPostScreenQuad;
VSPointer<VSCustomMaterial> VSCustomMaterial::ms_pPostSaturation;
VSPointer<VSCustomMaterial> VSCustomMaterial::ms_pOldPhoto;
VSPointer<VSCustomMaterial> VSCustomMaterial::ms_pGuassBlurH;
VSPointer<VSCustomMaterial> VSCustomMaterial::ms_pGuassBlurV;
VSPointer<VSCustomMaterial> VSCustomMaterial::ms_pFilter3X3;
VSPointer<VSCustomMaterial> VSCustomMaterial::ms_pBright;
VSPointer<VSCustomMaterial> VSCustomMaterial::ms_pTexAdd;

VSPointer<VSCustomMaterial> VSCustomMaterial::ms_pPostVolumeShadowMap;
VSPointer<VSCustomMaterial> VSCustomMaterial::ms_pProjectShadowPre;
VSPointer<VSCustomMaterial> VSCustomMaterial::ms_pProjectShadow;
VSPointer<VSCustomMaterial> VSCustomMaterial::ms_pSSR;
VSPointer<VSCustomMaterial> VSCustomMaterial::ms_pFont;

VSPointer<VSCustomMaterial> VSCustomMaterial::ms_pClearColorRange;
VSPointer<VSCustomMaterial> VSCustomMaterial::ms_pClearDepthRange;
VSPointer<VSCustomMaterial> VSCustomMaterial::ms_pClearColorDepthRange;
VSPointer<VSCustomMaterial> VSCustomMaterial::ms_pClearAllRange;
VSPointer<VSCustomMaterial> VSCustomMaterial::ms_pClearDepthStencilRange;
VSPointer<VSCustomMaterial> VSCustomMaterial::ms_pOcclusionCull;
bool VSCustomMaterial::InitialDefaultState()
{
	ms_pPostGray = VS_NEW VSCustomMaterial();
	ms_pGammaCorrect = VS_NEW VSCustomMaterial();
	ms_pPostScreenQuad = VS_NEW VSCustomMaterial();
	ms_pPostSaturation = VS_NEW VSCustomMaterial();
	ms_pOldPhoto = VS_NEW VSCustomMaterial();
	ms_pGuassBlurH = VS_NEW VSCustomMaterial();
	ms_pGuassBlurV = VS_NEW VSCustomMaterial();
	ms_pFilter3X3 = VS_NEW VSCustomMaterial();
	ms_pBright = VS_NEW VSCustomMaterial();
	ms_pTexAdd = VS_NEW VSCustomMaterial();

	ms_pPostVolumeShadowMap = VS_NEW VSCustomMaterial();
	ms_pProjectShadowPre = VS_NEW VSCustomMaterial();
	ms_pProjectShadow = VS_NEW VSCustomMaterial();
	ms_pSSR = VS_NEW VSCustomMaterial();
	ms_pFont = VS_NEW VSCustomMaterial();

	ms_pClearColorRange = VS_NEW VSCustomMaterial();
	ms_pClearDepthRange = VS_NEW VSCustomMaterial();
	ms_pClearColorDepthRange = VS_NEW VSCustomMaterial();
	ms_pClearAllRange = VS_NEW VSCustomMaterial();
	ms_pClearDepthStencilRange = VS_NEW VSCustomMaterial();
	ms_pOcclusionCull = VS_NEW VSCustomMaterial();
	
	{
		VSDepthStencilDesc DepthStencilDesc;
		DepthStencilDesc.m_bDepthEnable = false;
		VSDepthStencilState * pDepthStencilState = VSResourceManager::CreateDepthStencilState(DepthStencilDesc);
		ms_pPostGray->m_RenderState.SetDepthStencilState(pDepthStencilState);
		ms_pPostScreenQuad->m_RenderState.SetDepthStencilState(pDepthStencilState);
		ms_pGammaCorrect->m_RenderState.SetDepthStencilState(pDepthStencilState);
		ms_pPostSaturation->m_RenderState.SetDepthStencilState(pDepthStencilState);
		ms_pOldPhoto->m_RenderState.SetDepthStencilState(pDepthStencilState);
		ms_pGuassBlurH->m_RenderState.SetDepthStencilState(pDepthStencilState);
		ms_pGuassBlurV->m_RenderState.SetDepthStencilState(pDepthStencilState);
		ms_pFilter3X3->m_RenderState.SetDepthStencilState(pDepthStencilState);
		ms_pBright->m_RenderState.SetDepthStencilState(pDepthStencilState);
		ms_pTexAdd->m_RenderState.SetDepthStencilState(pDepthStencilState);
		ms_pFont->m_RenderState.SetDepthStencilState(pDepthStencilState);
	}
	{
		VSDepthStencilDesc DepthStencilDesc;
		DepthStencilDesc.m_bDepthEnable = false;
		DepthStencilDesc.m_bStencilEnable = true;
		DepthStencilDesc.m_uiStencilCompareMethod = VSDepthStencilDesc::CM_LESSEQUAL;
		DepthStencilDesc.m_uiReference = 0x10;
		VSDepthStencilState * pDepthStencilState = VSResourceManager::CreateDepthStencilState(DepthStencilDesc);
		ms_pPostVolumeShadowMap->m_RenderState.SetDepthStencilState(pDepthStencilState);
	}

	{
		VSDepthStencilDesc DepthStencilDesc;
		DepthStencilDesc.m_bDepthWritable = false;
		DepthStencilDesc.m_uiDepthCompareMethod = VSDepthStencilDesc::CM_LESS;
		DepthStencilDesc.m_bStencilEnable = true;
		DepthStencilDesc.m_bTwoSideStencilMode = true;
		DepthStencilDesc.m_uiSPassZFailOP = VSDepthStencilDesc::OT_DECREMENT;
		DepthStencilDesc.m_uiCCW_SPassZFailOP = VSDepthStencilDesc::OT_INCREMENT;
		VSDepthStencilState * pDepthStencilState = VSResourceManager::CreateDepthStencilState(DepthStencilDesc);
		ms_pProjectShadowPre->m_RenderState.SetDepthStencilState(pDepthStencilState);

		VSRasterizerDesc RasterizerDesc;
		RasterizerDesc.m_uiCullType = VSRasterizerDesc::CT_NONE;
		VSRasterizerState * pRasterizerState= VSResourceManager::CreateRasterizerState(RasterizerDesc);
		ms_pProjectShadowPre->m_RenderState.SetRasterizerState(pRasterizerState);

		VSBlendDesc BlendDesc;
		BlendDesc.ucWriteMask[0] = VSBlendDesc::WM_NONE;
		VSBlendState * pBlendState = VSResourceManager::CreateBlendState(BlendDesc);
		ms_pProjectShadowPre->m_RenderState.SetBlendState(pBlendState);
	}


	{
		VSDepthStencilDesc DepthStencilDesc;
		DepthStencilDesc.m_bDepthEnable = false;
		DepthStencilDesc.m_bStencilEnable = true;
		DepthStencilDesc.m_uiStencilCompareMethod = VSDepthStencilDesc::CM_LESSEQUAL;
		DepthStencilDesc.m_uiReference = 0x10;
		VSDepthStencilState * pDepthStencilState = VSResourceManager::CreateDepthStencilState(DepthStencilDesc);
		ms_pProjectShadow->m_RenderState.SetDepthStencilState(pDepthStencilState);

		VSRasterizerDesc RasterizerDesc;
		RasterizerDesc.m_uiCullType = VSRasterizerDesc::CT_CW;
		VSRasterizerState * pRasterizerState= VSResourceManager::CreateRasterizerState(RasterizerDesc);
		ms_pProjectShadow->m_RenderState.SetRasterizerState(pRasterizerState);

		VSBlendDesc BlendDesc;
		BlendDesc.ucSrcBlend[0] = VSBlendDesc::BP_DESTCOLOR;
		BlendDesc.ucDestBlend[0] = VSBlendDesc::BP_ZERO;

		BlendDesc.bBlendEnable[0] = true;		
		VSBlendState * pBlendState = VSResourceManager::CreateBlendState(BlendDesc);
		ms_pProjectShadow->m_RenderState.SetBlendState(pBlendState);
	}

	{
		VSDepthStencilDesc DepthStencilDesc;
		DepthStencilDesc.m_bDepthEnable = false;
		VSDepthStencilState * pDepthStencilState = VSResourceManager::CreateDepthStencilState(DepthStencilDesc);
		ms_pClearColorRange->m_RenderState.SetDepthStencilState(pDepthStencilState);
	}
	{
		VSDepthStencilDesc DepthStencilDesc;
		DepthStencilDesc.m_uiDepthCompareMethod = VSDepthStencilDesc::CM_ALWAYS;
		VSDepthStencilState * pDepthStencilState = VSResourceManager::CreateDepthStencilState(DepthStencilDesc);

		VSBlendDesc BlendDesc;
		BlendDesc.ucWriteMask[0] = VSBlendDesc::WM_NONE;
		VSBlendState * pBlendState = VSResourceManager::CreateBlendState(BlendDesc);

		ms_pClearDepthRange->m_RenderState.SetDepthStencilState(pDepthStencilState);
		ms_pClearDepthRange->m_RenderState.SetBlendState(pBlendState);
	}

	{
	
		VSBlendDesc BlendDesc;
		BlendDesc.ucWriteMask[0] = VSBlendDesc::WM_NONE;
		VSBlendState * pBlendState = VSResourceManager::CreateBlendState(BlendDesc);
		ms_pClearDepthStencilRange->m_RenderState.SetBlendState(pBlendState);
	}

	{
		VSBlendDesc BlendDesc;
		BlendDesc.ucWriteMask[0] = VSBlendDesc::WM_NONE;
		VSBlendState * pBlendState = VSResourceManager::CreateBlendState(BlendDesc);
		ms_pOcclusionCull->m_RenderState.SetBlendState(pBlendState);		
	}
	LoadDefault();
	return 1;
}
bool VSCustomMaterial::TerminalDefaultState()
{
	ms_pPostGray = NULL;
	ms_pGammaCorrect = NULL;
	ms_pPostScreenQuad = NULL;
	ms_pPostSaturation = NULL;
	ms_pOldPhoto = NULL;
	ms_pGuassBlurH = NULL;
	ms_pGuassBlurV = NULL;
	ms_pFilter3X3 = NULL;
	ms_pBright = NULL;
	ms_pTexAdd = NULL;

	ms_pPostVolumeShadowMap = NULL;
	ms_pProjectShadowPre = NULL;
	ms_pProjectShadow = NULL;
	ms_pSSR = NULL;
	
	ms_pFont = NULL;

	ms_pClearColorRange = NULL;
	ms_pClearDepthRange = NULL;
	ms_pClearColorDepthRange = NULL;
	ms_pClearAllRange = NULL;
	ms_pClearDepthStencilRange = NULL;

	ms_pOcclusionCull = NULL;
	return 1;
}
void VSCustomMaterial::LoadAllDeviceShader()
{
	ms_pPostGray->LoadDeviceShader();
	ms_pGammaCorrect->LoadDeviceShader();
	ms_pPostScreenQuad->LoadDeviceShader();
	ms_pPostSaturation->LoadDeviceShader();
	ms_pOldPhoto->LoadDeviceShader();
	ms_pGuassBlurH->LoadDeviceShader();
	ms_pGuassBlurV->LoadDeviceShader();
	ms_pFilter3X3->LoadDeviceShader();
	ms_pBright->LoadDeviceShader();
	ms_pTexAdd->LoadDeviceShader();

	ms_pPostVolumeShadowMap->LoadDeviceShader();
	ms_pProjectShadowPre->LoadDeviceShader();
	ms_pProjectShadow->LoadDeviceShader();
	ms_pSSR->LoadDeviceShader();
	ms_pFont->LoadDeviceShader();

	if (VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_ClearRange) == false)
	{
		ms_pClearColorRange->LoadDeviceShader();
		ms_pClearDepthRange->LoadDeviceShader();
		ms_pClearColorDepthRange->LoadDeviceShader();
		ms_pClearAllRange->LoadDeviceShader();
		ms_pClearDepthStencilRange->LoadDeviceShader();
	}
	ms_pOcclusionCull->LoadDeviceShader();
}
void VSCustomMaterial::LoadDeviceShader()
{
	GetCurVShader(*VSShaderKey::ms_NULLShaderKey);
	GetCurPShader(*VSShaderKey::ms_NULLShaderKey);
}
bool VSCustomMaterial::LoadDefault()
{
	if (!ms_pPostGray)
	{
		return 0;
	}

	ms_pPostGray->PreLoad(VSUsedName::ms_cPostGray,_T("PostEffectVShader.txt"),_T("VSMain"),_T("GrayPShader.txt"),_T("PSMain"));

	if(!ms_pGammaCorrect)
	{
		return 0;
	}

	ms_pGammaCorrect->PreLoad(VSUsedName::ms_cGammaCorrect,_T("PostEffectVShader.txt"),_T("VSMain"),_T("GammaCorrectPShader.txt"),_T("PSMain"));

	if (!ms_pPostScreenQuad)
	{
		return 0;
	}
	ms_pPostScreenQuad->PreLoad(VSUsedName::ms_cPostScreenQuad,_T("PostEffectVShader.txt"),_T("VSMain"),_T("ScreenQuadPShader.txt"),_T("PSMain"));

	if (!ms_pPostSaturation)
	{
		return 0;
	}
	ms_pPostSaturation->PreLoad(VSUsedName::ms_cPostSaturation, _T("PostEffectVShader.txt"), _T("VSMain"), _T("SaturationPShader.txt"), _T("PSMain"));

	if (!ms_pOldPhoto)
	{
		return 0;
	}
	ms_pOldPhoto->PreLoad(VSUsedName::ms_cOldPhoto, _T("PostEffectVShader.txt"), _T("VSMain"), _T("OldPhotoPShader.txt"), _T("PSMain"));

	if (!ms_pGuassBlurH)
	{
		return 0;
	}
	ms_pGuassBlurH->PreLoad(VSUsedName::ms_cGuassBlurH, _T("PostEffectVShader.txt"), _T("VSMain"), _T("GuassBlurHPShader.txt"), _T("PSMain"));

	if (!ms_pGuassBlurV)
	{
		return 0;
	}
	ms_pGuassBlurV->PreLoad(VSUsedName::ms_cGuassBlurV, _T("PostEffectVShader.txt"), _T("VSMain"), _T("GuassBlurVPShader.txt"), _T("PSMain"));

	if (!ms_pFilter3X3)
	{
		return 0;
	}
	ms_pFilter3X3->PreLoad(VSUsedName::ms_cFilter3X3, _T("PostEffectVShader.txt"), _T("VSMain"), _T("Filter3X3PShader.txt"), _T("PSMain"));
	
	
	if (!ms_pBright)
	{
		return 0;
	}
	ms_pBright->PreLoad(VSUsedName::ms_cBright, _T("PostEffectVShader.txt"), _T("VSMain"), _T("BrightPShader.txt"), _T("PSMain"));

	if (!ms_pTexAdd)
	{
		return 0;
	}
	ms_pTexAdd->PreLoad(VSUsedName::ms_cTexAdd, _T("PostEffectVShader.txt"), _T("VSMain"), _T("TexAddPShader.txt"), _T("PSMain"));

	if (!ms_pPostVolumeShadowMap)
	{
		return 0;
	}
	ms_pPostVolumeShadowMap->PreLoad(VSUsedName::ms_cPostVolumeShadowMap,_T("PostEffectVShader.txt"),_T("VSMain"),_T("VolumeShadowMapPShader.txt"),_T("PSMain"));

	if (!ms_pProjectShadowPre)
	{
		return 0;
	}
	ms_pProjectShadowPre->PreLoad(_T("ProjectShadowPre"),_T("ProjectShadowPreVS.txt"),_T("VSMain"),_T("DefaultPS.txt"),_T("PSMain"));

	if (!ms_pProjectShadow)
	{
		return 0;
	}
	ms_pProjectShadow->PreLoad(_T("ProjectShadow"),_T("ProjectShadowVS.txt"),_T("VSMain"),_T("ProjectShadowPS.txt"),_T("PSMain"));

	if (!ms_pSSR)
	{
		return 0;
	}

	ms_pSSR->PreLoad(_T("ScreenSpaceReflection"),_T("PostEffectVShader.txt"),_T("VSMain"),_T("SSRPShader.txt"),_T("PSMain"));

	if (!ms_pFont)
	{
		return 0;
	}
	ms_pFont->PreLoad(_T("FontShader"), _T("FontVShader.txt"), _T("VSMain"), _T("FontPShader.txt"), _T("PSMain"));

	if (!ms_pClearColorRange)
	{
		return 0;
	}
	ms_pClearColorRange->PreLoad(_T("ClearColorRange"), _T("ClearRangeVShader.txt"), _T("VSMain"), _T("ClearRangePShader.txt"), _T("PSMain"));
	
	if (!ms_pClearDepthRange)
	{
		return 0;
	}
	ms_pClearDepthRange->PreLoad(_T("ClearDepthRange"), _T("ClearRangeVShader.txt"), _T("VSMain"), _T("ClearRangePShader.txt"), _T("PSMain"));



	if (!ms_pClearColorDepthRange)
	{
		return 0;
	}
	ms_pClearColorDepthRange->PreLoad(_T("ClearColorDepthRange"), _T("ClearRangeVShader.txt"), _T("VSMain"), _T("ClearRangePShader.txt"), _T("PSMain"));

	if (!ms_pClearAllRange)
	{
		return 0;
	}
	ms_pClearAllRange->PreLoad(_T("ClearAllRange"), _T("ClearRangeVShader.txt"), _T("VSMain"), _T("ClearRangePShader.txt"), _T("PSMain"));

	if (!ms_pClearDepthStencilRange)
	{
		return 0;
	}
	ms_pClearDepthStencilRange->PreLoad(_T("ClearDepthStencilRange"), _T("ClearRangeVShader.txt"), _T("VSMain"), _T("ClearRangePShader.txt"), _T("PSMain"));

	if (!ms_pOcclusionCull)
	{
		return 0;
	}

	ms_pOcclusionCull->PreLoad(_T("OcclusionCull"), _T("OcclusionVS.txt"), _T("VSMain"), _T("DefaultPS.txt"), _T("PSMain"));
	return 1;
}
void VSCustomMaterial::SetVShaderValue(const VSUsedName & Name,void *fValue)
{
	if (m_pVShader)
	{
		m_pVShader->SetParam(Name,fValue);
	}
}
void VSCustomMaterial::SetVShaderTexture(const VSUsedName & TexSamplerNodeName,VSTexAllState * pTex)
{	
	if (m_pVShader)
	{
		m_pVShader->SetParam(TexSamplerNodeName,pTex);
	}
}
void VSCustomMaterial::SetPShaderValue(const VSUsedName & Name,void *fValue)
{
	if (m_pPShader)
	{
		m_pPShader->SetParam(Name,fValue);
	}
}
void VSCustomMaterial::SetPShaderTexture(const VSUsedName & TexSamplerNodeName,VSTexAllState * pTex)
{
	if (m_pPShader)
	{
		m_pPShader->SetParam(TexSamplerNodeName,pTex);
	}
}
void VSCustomMaterial::SetGShaderValue(const VSUsedName & Name, void *fValue)
{
	if (m_pGShader)
	{
		m_pGShader->SetParam(Name, fValue);
	}
}
void VSCustomMaterial::SetGShaderTexture(const VSUsedName & TexSamplerNodeName, VSTexAllState * pTex)
{
	if (m_pGShader)
	{
		m_pGShader->SetParam(TexSamplerNodeName, pTex);
	}
}
void VSCustomMaterial::SetHShaderValue(const VSUsedName & Name, void *fValue)
{
	if (m_pHShader)
	{
		m_pHShader->SetParam(Name, fValue);
	}
}
void VSCustomMaterial::SetHShaderTexture(const VSUsedName & TexSamplerNodeName, VSTexAllState * pTex)
{
	if (m_pHShader)
	{
		m_pHShader->SetParam(TexSamplerNodeName, pTex);
	}
}
void VSCustomMaterial::SetDShaderValue(const VSUsedName & Name, void *fValue)
{
	if (m_pDShader)
	{
		m_pDShader->SetParam(Name, fValue);
	}
}
void VSCustomMaterial::SetDShaderTexture(const VSUsedName & TexSamplerNodeName, VSTexAllState * pTex)
{
	if (m_pDShader)
	{
		m_pDShader->SetParam(TexSamplerNodeName, pTex);
	}
}
VSRenderState & VSCustomMaterial::GetRenderState()
{
	return m_RenderState;
}
void VSCustomMaterial::SetBlendState(VSBlendState * pBlendState)
{
	m_RenderState.SetBlendState(pBlendState);
}
void VSCustomMaterial::SetDepthStencilState(VSDepthStencilState *pDepthStencilState)
{
	m_RenderState.SetDepthStencilState(pDepthStencilState);
}
void VSCustomMaterial::SetRasterizerState(VSRasterizerState *pRasterizerState)
{
	m_RenderState.SetRasterizerState(pRasterizerState);
}
void VSCustomMaterial::AddClipPlane(const VSPlane3 & Plane)
{
	m_RenderState.AddPlane(Plane);
}
void VSCustomMaterial::AddScissorRect(const VSRect2 & Rect)
{
	m_RenderState.AddRect(Rect);
}
VSCustomMaterial::VSCustomMaterial(const VSUsedName & ResourceName,
	const TCHAR * pVShaderFileName,const TCHAR * pVMainFunName,
	const TCHAR * pPShaderFileName, const TCHAR * pPMainFunName,
	const TCHAR * pGShaderFileName, const TCHAR * pGMainFunName,
	const TCHAR * pHShaderFileName, const TCHAR * pHMainFunName,
	const TCHAR * pDShaderFileName, const TCHAR * pDMainFunName)
{
		m_pVShaderSet = NULL;
		m_pPShaderSet = NULL;
		m_pGShaderSet = NULL;
		m_pHShaderSet = NULL;
		m_pDShaderSet = NULL;

		m_pVShader = NULL;
		m_pPShader = NULL;
		m_pGShader = NULL;
		m_pHShader = NULL;
		m_pDShader = NULL;
		PreLoad(ResourceName, 
			pVShaderFileName, pVMainFunName, 
			pPShaderFileName, pPMainFunName,
			pGShaderFileName,pGMainFunName,
			pHShaderFileName,pHMainFunName,
			pDShaderFileName,pDMainFunName);
	
}
VSCustomMaterial::VSCustomMaterial()
{
	m_pVShader = NULL;
	m_pPShader = NULL;
	m_pGShader = NULL;
	m_pHShader = NULL;
	m_pDShader = NULL;

	m_pVShaderSet = NULL;
	m_pPShaderSet = NULL;
	m_pGShaderSet = NULL;
	m_pHShaderSet = NULL;
	m_pDShaderSet = NULL;
}
VSCustomMaterial::~VSCustomMaterial()
{
	m_pVShader = NULL;
	m_pPShader = NULL;
	m_pGShader = NULL;
	m_pHShader = NULL;
	m_pDShader = NULL;
}
void VSCustomMaterial::PreLoad(const VSUsedName & ResourceName,
							const TCHAR * pVShaderFileName,const TCHAR * pVMainFunName,
							  const TCHAR * pPShaderFileName,const TCHAR * pPMainFunName,
							  const TCHAR * pGShaderFileName, const TCHAR * pGMainFunName,
							  const TCHAR * pHShaderFileName, const TCHAR * pHMainFunName,
							  const TCHAR * pDShaderFileName, const TCHAR * pDMainFunName)
{
	m_VShaderName = pVShaderFileName;
	m_PShaderName = pPShaderFileName;
	m_GShaderName = pGShaderFileName;
	m_HShaderName = pHShaderFileName;
	m_DShaderName = pDShaderFileName;

	m_VMainFunName = pVMainFunName;
	m_PMainFunName = pPMainFunName;
	m_GMainFunName = pGMainFunName;
	m_HMainFunName = pHMainFunName;
	m_DMainFunName = pDMainFunName;

	m_ResourceName = ResourceName;

}
bool VSCustomMaterial::GetVShader(VSShaderKey & VShaderKey)
{
	VSVShader * pVertexShader = NULL;

	if (!m_pVShaderSet)
	{
		m_pVShaderSet = VSResourceManager::GetCustomVertexShaderMap().GetShaderSet(m_VShaderName);
	}
	
	
	
	if (m_pVShader == NULL)
	{
		if (m_pVShaderSet)
		{
			unsigned int uiIndex = m_pVShaderSet->Find(VShaderKey);

			if (uiIndex != m_pVShaderSet->GetNum())
			{
				VSShader * pTemp = (*m_pVShaderSet)[uiIndex].Value;
				pVertexShader = (VSVShader *)(pTemp);
			}

		}
		if (pVertexShader == NULL)
		{
			pVertexShader = VS_NEW VSVShader(m_VShaderName.GetBuffer(),m_VMainFunName.GetBuffer(),true);
			if (!pVertexShader)
			{
				return 0;
			}
			pVertexShader->m_ShaderKey = VShaderKey;
			VSRenderer::ms_pRenderer->LoadVShaderProgram(pVertexShader);
			if (m_pVShaderSet)
			{
				m_pVShaderSet->AddElement(VShaderKey,pVertexShader);
			}
		}
		m_pVShader = pVertexShader;
	}
	else
	{
		if (m_pVShader->m_ShaderKey == VShaderKey)
		{

		}
		else
		{

			if (m_pVShaderSet)
			{
				unsigned int uiIndex = m_pVShaderSet->Find(VShaderKey);

				if (uiIndex != m_pVShaderSet->GetNum())
				{
					VSShader * pTemp = (*m_pVShaderSet)[uiIndex].Value;
					pVertexShader = (VSVShader *)(pTemp);
				}
			}
			if (pVertexShader == NULL)
			{
				pVertexShader = VS_NEW VSVShader(m_VShaderName.GetBuffer(),m_VMainFunName.GetBuffer(),true);
				if (!pVertexShader)
				{
					return 0;
				}
				pVertexShader->m_ShaderKey = VShaderKey;
				VSRenderer::ms_pRenderer->LoadVShaderProgram(pVertexShader);
				if (m_pVShaderSet)
				{
					m_pVShaderSet->AddElement(VShaderKey,pVertexShader);
				}
			}
			m_pVShader = pVertexShader;
		}
	}

	if (!m_pVShaderSet)
	{
		VSResourceManager::GetCustomVertexShaderMap().SetShader(m_VShaderName,VShaderKey,pVertexShader);
		m_pVShaderSet = VSResourceManager::GetCustomVertexShaderMap().GetShaderSet(m_VShaderName);
	}
	return 1;
}
bool VSCustomMaterial::GetHShader(VSShaderKey & HShaderKey)
{
	unsigned int uiHShaderNum = 0;
	VSHShader * pHShader = NULL;
	if (m_HShaderName.GetBuffer() == NULL)
	{
		return false;
	}
	if (!m_pHShaderSet)
	{
		m_pHShaderSet = VSResourceManager::GetCustomHullShaderMap().GetShaderSet(m_HShaderName);
	}



	if (m_pHShaderSet)
	{
		uiHShaderNum = m_pHShaderSet->GetNum();
	}


	if (m_pHShader == NULL)
	{
		if (m_pHShaderSet)
		{
			unsigned int uiIndex = m_pHShaderSet->Find(HShaderKey);

			if (uiIndex != m_pHShaderSet->GetNum())
			{
				VSShader * pTemp = (*m_pHShaderSet)[uiIndex].Value;
				pHShader = (VSHShader *)(pTemp);
			}
		}
		if (pHShader == NULL)
		{
			pHShader = VS_NEW VSHShader(m_HShaderName.GetBuffer(), m_HMainFunName.GetBuffer(), true);

			if (!pHShader)
			{
				return 0;
			}
			pHShader->m_ShaderKey = HShaderKey;
			VSRenderer::ms_pRenderer->LoadHShaderProgram(pHShader);
			if (m_pHShaderSet)
			{
				m_pHShaderSet->AddElement(HShaderKey, pHShader);
			}
		}
		m_pHShader = pHShader;

	}
	else
	{
		if (m_pDShader->m_ShaderKey == HShaderKey)
		{

		}
		else
		{
			if (m_pHShaderSet)
			{
				unsigned int uiIndex = m_pHShaderSet->Find(HShaderKey);

				if (uiIndex != m_pHShaderSet->GetNum())
				{
					VSShader * pTemp = (*m_pHShaderSet)[uiIndex].Value;
					pHShader = (VSHShader *)(pTemp);
				}
			}
			if (pHShader == NULL)
			{
				pHShader = VS_NEW VSHShader(m_HShaderName.GetBuffer(), m_HMainFunName.GetBuffer(), true);

				if (!pHShader)
				{
					return 0;
				}
				pHShader->m_ShaderKey = HShaderKey;
				VSRenderer::ms_pRenderer->LoadHShaderProgram(pHShader);
				if (m_pHShaderSet)
				{
					m_pHShaderSet->AddElement(HShaderKey, pHShader);
				}
			}
			m_pHShader = pHShader;
		}
	}

	if (!m_pHShaderSet)
	{
		VSResourceManager::GetCustomHullShaderMap().SetShader(m_HShaderName, HShaderKey, pHShader);
		m_pHShaderSet = VSResourceManager::GetCustomHullShaderMap().GetShaderSet(m_HShaderName);
	}
	return 1;
}
bool VSCustomMaterial::GetDShader(VSShaderKey & DShaderKey)
{
	unsigned int uiDShaderNum = 0;
	VSDShader * pDShader = NULL;
	if (m_DShaderName.GetBuffer() == NULL)
	{
		return false;
	}
	if (!m_pDShaderSet)
	{
		m_pDShaderSet = VSResourceManager::GetCustomDomainShaderMap().GetShaderSet(m_DShaderName);
	}



	if (m_pDShaderSet)
	{
		uiDShaderNum = m_pDShaderSet->GetNum();
	}


	if (m_pDShader == NULL)
	{
		if (m_pDShaderSet)
		{
			unsigned int uiIndex = m_pDShaderSet->Find(DShaderKey);

			if (uiIndex != m_pDShaderSet->GetNum())
			{
				VSShader * pTemp = (*m_pDShaderSet)[uiIndex].Value;
				pDShader = (VSDShader *)(pTemp);
			}
		}
		if (pDShader == NULL)
		{
			pDShader = VS_NEW VSDShader(m_DShaderName.GetBuffer(), m_DMainFunName.GetBuffer(), true);

			if (!pDShader)
			{
				return 0;
			}
			pDShader->m_ShaderKey = DShaderKey;
			VSRenderer::ms_pRenderer->LoadDShaderProgram(pDShader);
			if (m_pDShaderSet)
			{
				m_pDShaderSet->AddElement(DShaderKey, pDShader);
			}
		}
		m_pDShader = pDShader;

	}
	else
	{
		if (m_pDShader->m_ShaderKey == DShaderKey)
		{

		}
		else
		{
			if (m_pDShaderSet)
			{
				unsigned int uiIndex = m_pDShaderSet->Find(DShaderKey);

				if (uiIndex != m_pDShaderSet->GetNum())
				{
					VSShader * pTemp = (*m_pDShaderSet)[uiIndex].Value;
					pDShader = (VSDShader *)(pTemp);
				}
			}
			if (pDShader == NULL)
			{
				pDShader = VS_NEW VSDShader(m_DShaderName.GetBuffer(), m_DMainFunName.GetBuffer(), true);

				if (!pDShader)
				{
					return 0;
				}
				pDShader->m_ShaderKey = DShaderKey;
				VSRenderer::ms_pRenderer->LoadDShaderProgram(pDShader);
				if (m_pDShaderSet)
				{
					m_pDShaderSet->AddElement(DShaderKey, pDShader);
				}
			}
			m_pDShader = pDShader;
		}
	}

	if (!m_pDShaderSet)
	{
		VSResourceManager::GetCustomDomainShaderMap().SetShader(m_DShaderName, DShaderKey, pDShader);
		m_pDShaderSet = VSResourceManager::GetCustomDomainShaderMap().GetShaderSet(m_DShaderName);
	}
	return 1;
}
bool VSCustomMaterial::GetGShader(VSShaderKey & GShaderKey)
{
	unsigned int uiGShaderNum = 0;
	VSGShader * pGShader = NULL;
	if (m_GShaderName.GetBuffer() == NULL)
	{
		return false;
	}
	if (!m_pGShaderSet)
	{
		m_pGShaderSet = VSResourceManager::GetCustomGeometryShaderMap().GetShaderSet(m_GShaderName);
	}



	if (m_pGShaderSet)
	{
		uiGShaderNum = m_pGShaderSet->GetNum();
	}


	if (m_pGShader == NULL)
	{
		if (m_pGShaderSet)
		{
			unsigned int uiIndex = m_pGShaderSet->Find(GShaderKey);

			if (uiIndex != m_pGShaderSet->GetNum())
			{
				VSShader * pTemp = (*m_pGShaderSet)[uiIndex].Value;
				pGShader = (VSGShader *)(pTemp);
			}
		}
		if (pGShader == NULL)
		{
			pGShader = VS_NEW VSGShader(m_GShaderName.GetBuffer(), m_GMainFunName.GetBuffer(), true);

			if (!pGShader)
			{
				return 0;
			}
			pGShader->m_ShaderKey = GShaderKey;
			VSRenderer::ms_pRenderer->LoadGShaderProgram(pGShader);
			if (m_pGShaderSet)
			{
				m_pGShaderSet->AddElement(GShaderKey, pGShader);
			}
		}
		m_pGShader = pGShader;

	}
	else
	{
		if (m_pPShader->m_ShaderKey == GShaderKey)
		{

		}
		else
		{
			if (m_pGShaderSet)
			{
				unsigned int uiIndex = m_pGShaderSet->Find(GShaderKey);

				if (uiIndex != m_pGShaderSet->GetNum())
				{
					VSShader * pTemp = (*m_pGShaderSet)[uiIndex].Value;
					pGShader = (VSGShader *)(pTemp);
				}
			}
			if (pGShader == NULL)
			{
				pGShader = VS_NEW VSGShader(m_GShaderName.GetBuffer(), m_GMainFunName.GetBuffer(), true);

				if (!pGShader)
				{
					return 0;
				}
				pGShader->m_ShaderKey = GShaderKey;
				VSRenderer::ms_pRenderer->LoadGShaderProgram(pGShader);
				if (m_pGShaderSet)
				{
					m_pGShaderSet->AddElement(GShaderKey, pGShader);
				}
			}
			m_pGShader = pGShader;
		}
	}

	if (!m_pGShaderSet)
	{
		VSResourceManager::GetCustomGeometryShaderMap().SetShader(m_GShaderName, GShaderKey, pGShader);
		m_pGShaderSet = VSResourceManager::GetCustomGeometryShaderMap().GetShaderSet(m_GShaderName);
	}
	return 1;
}
bool VSCustomMaterial::GetPShader(VSShaderKey & PShaderKey)
{
	unsigned int uiPShaderNum = 0;
	VSPShader * pPixelShader = NULL;

	if (!m_pPShaderSet)
	{
		m_pPShaderSet = VSResourceManager::GetCustomPixelShaderMap().GetShaderSet(m_PShaderName);
	}
	
	

	if (m_pPShaderSet)
	{
		uiPShaderNum = m_pPShaderSet->GetNum();
	}


	if (m_pPShader == NULL)
	{
		if (m_pPShaderSet)
		{
			unsigned int uiIndex = m_pPShaderSet->Find(PShaderKey);

			if (uiIndex != m_pPShaderSet->GetNum())
			{
				VSShader * pTemp = (*m_pPShaderSet)[uiIndex].Value;
				pPixelShader = (VSPShader *)(pTemp);
			}
		}
		if (pPixelShader == NULL)
		{
			pPixelShader = VS_NEW VSPShader(m_PShaderName.GetBuffer(),m_PMainFunName.GetBuffer(),true);
			
			if (!pPixelShader)
			{
				return 0;
			}
			pPixelShader->m_ShaderKey = PShaderKey;
			VSRenderer::ms_pRenderer->LoadPShaderProgram(pPixelShader);
			if (m_pPShaderSet)
			{
				m_pPShaderSet->AddElement(PShaderKey,pPixelShader);
			}
		}
		m_pPShader = pPixelShader;

	}
	else
	{
		if (m_pPShader->m_ShaderKey == PShaderKey)
		{

		}
		else
		{
			if (m_pPShaderSet)
			{
				unsigned int uiIndex = m_pPShaderSet->Find(PShaderKey);

				if (uiIndex != m_pPShaderSet->GetNum())
				{
					VSShader * pTemp = (*m_pPShaderSet)[uiIndex].Value;
					pPixelShader = (VSPShader *)(pTemp);
				}
			}
			if (pPixelShader == NULL)
			{
				pPixelShader = VS_NEW VSPShader(m_PShaderName.GetBuffer(),m_PMainFunName.GetBuffer(),true);
				
				if (!pPixelShader)
				{
					return 0;
				}
				pPixelShader->m_ShaderKey = PShaderKey;
				VSRenderer::ms_pRenderer->LoadPShaderProgram(pPixelShader);
				if (m_pPShaderSet)
				{
					m_pPShaderSet->AddElement(PShaderKey,pPixelShader);
				}
			}
			m_pPShader = pPixelShader;
		}
	}

	if (!m_pPShaderSet)
	{
		VSResourceManager::GetCustomPixelShaderMap().SetShader(m_PShaderName,PShaderKey,pPixelShader);
		m_pPShaderSet = VSResourceManager::GetCustomPixelShaderMap().GetShaderSet(m_PShaderName);
	}
	return 1;
}
VSHShader *VSCustomMaterial::GetCurHShader(VSShaderKey & HShaderKey)
{
	if (GetHShader(HShaderKey))
	{
		return m_pHShader;
	}
	return NULL;
}
VSDShader *VSCustomMaterial::GetCurDShader(VSShaderKey & DShaderKey)
{
	if (GetDShader(DShaderKey))
	{
		return m_pDShader;
	}
	return NULL;
}
VSGShader *VSCustomMaterial::GetCurGShader(VSShaderKey & GShaderKey)
{
	if (GetGShader(GShaderKey))
	{
		return m_pGShader;
	}
	return NULL;
}
VSVShader *VSCustomMaterial::GetCurVShader(VSShaderKey & VShaderKey)
{
	if (GetVShader(VShaderKey))
	{
		return m_pVShader;
	}
	return NULL;
}
VSPShader *VSCustomMaterial::GetCurPShader(VSShaderKey & PShaderKey)
{
	if(GetPShader(PShaderKey))
	{
		return m_pPShader;
	}
	return NULL;
}
bool  VSCustomMaterial::UseMaterial()
{

	GetCurVShader(*VSShaderKey::ms_NULLShaderKey);
	GetCurHShader(*VSShaderKey::ms_NULLShaderKey);
	GetCurDShader(*VSShaderKey::ms_NULLShaderKey);
	GetCurGShader(*VSShaderKey::ms_NULLShaderKey);
	GetCurPShader(*VSShaderKey::ms_NULLShaderKey);
	if(!VSRenderer::ms_pRenderer->SetVShader(m_pVShader))
		return 0;
	if (!VSRenderer::ms_pRenderer->SetHShader(m_pHShader))
		return 0;
	if (!VSRenderer::ms_pRenderer->SetDShader(m_pDShader))
		return 0;
	if (!VSRenderer::ms_pRenderer->SetGShader(m_pGShader))
		return 0;
	if(!VSRenderer::ms_pRenderer->SetPShader(m_pPShader))
		return 0;
	VSRenderer::ms_pRenderer->SetRenderState(m_RenderState);
	return 1;
}
void VSMaterial::SetPShaderValue(const VSUsedName & Name,void *fValue)
{
	if(!fValue)
		return;

	static CustomFloatValue TempValue;
	TempValue.ConstValueName = Name;
	unsigned int uiIndex = m_ShaderCustomValue.FindElement(TempValue);
	if (uiIndex != m_ShaderCustomValue.GetNum())
	{
		VSMemcpy(&m_ShaderCustomValue[uiIndex].Value[0], fValue, sizeof(VSREAL)* m_ShaderCustomValue[uiIndex].Value.GetNum());
	}

	return ;
}


void VSMaterial::SetPShaderTexture(const VSUsedName & TexSamplerNodeName,VSTexAllStateR * pTex)
{
	static CustomTexSampler TempValue;
	TempValue.ConstValueName = TexSamplerNodeName;
	unsigned int uiIndex = m_ShaderCustomTex.FindElement(TempValue);
	if (uiIndex != m_ShaderCustomTex.GetNum())
	{
		m_ShaderCustomTex[uiIndex].m_pTexture = pTex;
	}

}
IMPLEMENT_RESOURCE(VSMaterial)
VSPointer<VSMaterial> VSMaterial::ms_DefaultOnlyColor;
VSPointer<VSMaterial> VSMaterial::ms_DefaultOnlyVertexColor;
VSPointer<VSMaterial> VSMaterial::ms_DefaultOnlyColorDisableDepth;
VSPointer<VSMaterial> VSMaterial::ms_DefaultOnlyVertexColorDisableDepth;
VSMaterialRPtr VSMaterial::ms_DefaultOnlyColorResource = NULL;
IMPLEMENT_RTTI(VSMaterial,VSObject)
BEGIN_ADD_PROPERTY(VSMaterial, VSObject)
REGISTER_PROPERTY(m_ShowName,ShowName,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pShaderMainFunction,ShaderMainFunction,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pShaderFunctionArray,ShaderFunctionArray,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pLightShaderFunction, LightShaderFunction, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pPostEffectShaderFunction, PostEffectShaderFunction, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_bIsCombine,IsCombine,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiCustomLayer,CustomLayer,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_ENUM_PROPERTY(m_uiTessellationPartitioning, TessellationPartitioning, TessellationPartitioningType, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
BEGIN_ADD_ENUM
ADD_ENUM(TessellationPartitioningType, TP_FRACTIONAL_EVEN)
ADD_ENUM(TessellationPartitioningType, TP_FRACTIONAL_ODD)
ADD_ENUM(TessellationPartitioningType, TP_INTEGER)
ADD_ENUM(TessellationPartitioningType, TP_POW2)
END_ADD_ENUM
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSMaterial)
ADD_PRIORITY(VSTexAllState)
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION_WITH_PRIORITY(TerminalDefaultState)
IMPLEMENT_INITIAL_END
bool VSMaterial::InitialDefaultState()
{
	ms_Default = VS_NEW VSMaterial(_T("DefaultMaterial"));
	if(!ms_Default)
		return 0;
	
	ms_DefaultOnlyColor = VS_NEW VSMaterial(_T("DefaultOnlyColorMaterial"));

	if(!ms_DefaultOnlyColor)
		return 0;

	ms_DefaultOnlyVertexColor = VS_NEW VSMaterial(_T("DefaultOnlyVertexColorMaterial"));

	if (!ms_DefaultOnlyVertexColor)
	{
		return 0;
	}

	ms_DefaultOnlyColorDisableDepth = VS_NEW VSMaterial(_T("DefaultOnlyColorDisableDepthMaterial"));

	if(!ms_DefaultOnlyColorDisableDepth)
		return 0;

	ms_DefaultOnlyVertexColorDisableDepth = VS_NEW VSMaterial(_T("DefaultOnlyVertexColorDisableDepthMaterial"));

	if (!ms_DefaultOnlyVertexColorDisableDepth)
	{
		return 0;
	}
	
	if (!LoadDefault())
	{
		return 0;
	}
	return 1;
}
bool VSMaterial::TerminalDefaultState()
{
	ms_DefaultResource = NULL;
	ms_DefaultOnlyColorResource = NULL;
	ms_Default = NULL;
	ms_DefaultOnlyColor = NULL;
	ms_DefaultOnlyVertexColor = NULL;
	ms_DefaultOnlyColorDisableDepth = NULL;
	ms_DefaultOnlyVertexColorDisableDepth = NULL;
	return 1;
}
bool VSMaterial::LoadDefault()
{
	{
		if(!ms_Default)
		return 0;
		VS2DTexSampler* p2DTexSamplerNode = VS_NEW VS2DTexSampler(_T("TexSampler"),ms_Default);
		p2DTexSamplerNode->SetTexture((VSTexAllStateR *)VSTexAllState::GetDefaultResource());
		VSConstFloatValue * pSpecularColor = VS_NEW VSConstFloatValue(_T("SpecluarColor"),ms_Default,4,false);
		pSpecularColor->SetValue(0,0.5f);
		pSpecularColor->SetValue(1,0.5f);
		pSpecularColor->SetValue(2,0.5f);
		pSpecularColor->SetValue(3,1.0f);
		VSConstFloatValue * pSpecularPow = VS_NEW VSConstFloatValue(_T("SpecularPow"),ms_Default,1,false);
		pSpecularPow ->SetValue(0,5);
		ms_Default->m_pShaderMainFunction[0]->GetInputNode(_T("DiffuseColor"))->Connection(p2DTexSamplerNode->GetOutputNode(0));
		ms_Default->m_pShaderMainFunction[0]->GetInputNode(_T("SpecularColor"))->Connection(pSpecularColor->GetOutputNode(0));
		ms_Default->m_pShaderMainFunction[0]->GetInputNode(_T("SpecularPow"))->Connection(pSpecularPow->GetOutputNode(0));
		ms_Default->m_ResourceName = _T("_Default");
	}
	{
		if(!ms_DefaultOnlyColor)
			return 0;

		VSConstFloatValue * pEmissiveColor = VS_NEW VSConstFloatValue(_T("EmissiveColor"), ms_DefaultOnlyColor,4,true);
		pEmissiveColor->SetValue(0,1.0f);
		pEmissiveColor->SetValue(1,1.0f);
		pEmissiveColor->SetValue(2,1.0f);
		pEmissiveColor->SetValue(3,1.0f);

		ms_DefaultOnlyColor->m_pShaderMainFunction[0]->GetInputNode(VSPhoneShaderFunction::IN_EMISSIVE_COLOR)->Connection(pEmissiveColor->GetOutputNode(VSOutputNode::ONI_VALUE));
		ms_DefaultOnlyColor->m_ResourceName = _T("_DefaultOnlyColor");
	}

	{
		if(!ms_DefaultOnlyVertexColor)
			return 0;

		VSVertexColor * pVertexColor = VS_NEW VSVertexColor(_T("VertexColor"), ms_DefaultOnlyVertexColor);

		ms_DefaultOnlyVertexColor->m_pShaderMainFunction[0]->GetInputNode(VSPhoneShaderFunction::IN_EMISSIVE_COLOR)->Connection(pVertexColor->GetOutputNode(VSOutputNode::ONI_COLOR));
		ms_DefaultOnlyVertexColor->m_ResourceName = _T("_DefaultOnlyVertexColor");
	}


	{
		if(!ms_DefaultOnlyColorDisableDepth)
			return 0;
		VSDepthStencilDesc DepthStencilDest;
		DepthStencilDest.m_bDepthEnable = false;
		VSDepthStencilState * pState = VSResourceManager::CreateDepthStencilState(DepthStencilDest);
		ms_DefaultOnlyColorDisableDepth->SetDepthStencilState(pState);
		VSConstFloatValue * pEmissiveColor = VS_NEW VSConstFloatValue(_T("EmissiveColor"), ms_DefaultOnlyColorDisableDepth,4,true);
		pEmissiveColor->SetValue(0,1.0f);
		pEmissiveColor->SetValue(1,1.0f);
		pEmissiveColor->SetValue(2,1.0f);
		pEmissiveColor->SetValue(3,1.0f);

		ms_DefaultOnlyColorDisableDepth->m_pShaderMainFunction[0]->GetInputNode(VSPhoneShaderFunction::IN_EMISSIVE_COLOR)->Connection(pEmissiveColor->GetOutputNode(VSOutputNode::ONI_VALUE));
		ms_DefaultOnlyColorDisableDepth->m_ResourceName = _T("_DefaultOnlyColorDisableDepth");
	}

	{
		if(!ms_DefaultOnlyVertexColorDisableDepth)
			return 0;

		VSDepthStencilDesc DepthStencilDest;
		DepthStencilDest.m_bDepthEnable = false;
		VSDepthStencilState * pState = VSResourceManager::CreateDepthStencilState(DepthStencilDest);
		ms_DefaultOnlyVertexColorDisableDepth->SetDepthStencilState(pState);

		VSVertexColor * pVertexColor = VS_NEW VSVertexColor(_T("VertexColor"), ms_DefaultOnlyVertexColorDisableDepth);

		ms_DefaultOnlyVertexColorDisableDepth->m_pShaderMainFunction[0]->GetInputNode(VSPhoneShaderFunction::IN_EMISSIVE_COLOR)->Connection(pVertexColor->GetOutputNode(VSOutputNode::ONI_COLOR));
		ms_DefaultOnlyVertexColorDisableDepth->m_ResourceName = _T("_DefaultOnlyVertexColorDisableDepth");
	}

	ms_DefaultResource = VSMaterialR::Create(ms_Default);
	ms_DefaultOnlyColorResource = VSMaterialR::Create(ms_DefaultOnlyColor);
	return 1;
}
VSMaterial::VSMaterial()
{	
	m_pShaderMainFunction.Clear();
	m_pLightShaderFunction = NULL;
	m_pPostEffectShaderFunction = NULL;
	m_bIsCombine = false;
	m_pShaderFunctionArray.Clear();
	m_bIsAllReadyLink = false;
	m_pPass[VSPass::PT_MATERIAL] = VS_NEW VSMaterialPass();
	m_pPass[VSPass::PT_NORMALDEPTH] = VS_NEW VSNormalDepthPass();
	m_pPass[VSPass::PT_POINT_CUBE_SHADOW] = VS_NEW VSCubeShadowPass();
	m_pPass[VSPass::PT_POINT_VOLUME_SHADOW] = VS_NEW VSVolumeShadowPass();
	m_pPass[VSPass::PT_PREZ] = VSPreZPass::GetDefault();
	m_pPass[VSPass::PT_SHADOW] = VS_NEW VSShadowPass();
	m_pPass[VSPass::PT_DUAL_PARABOLOID_SHADOW] = VS_NEW VSDualParaboloidShadowPass();
	m_pPass[VSPass::PT_LIGHT_FUNCTION] = VS_NEW VSLightFunPass();
	m_pPass[VSPass::PT_POSTEFFECT] = VS_NEW VSPostEffectPass();
	m_pPass[VSPass::PT_INDIRECT] = VS_NEW VSIndirectRenderPass();
	VSMAC_ASSERT(m_pPass[VSPass::PT_MATERIAL] && m_pPass[VSPass::PT_INDIRECT]);
	m_uiCustomLayer = 10;
	m_uiTessellationPartitioning = 0;
	m_ShaderCustomValue.Clear();
	m_ShaderCustomTex.Clear();
}
VSMaterial::~VSMaterial()
{
	//由于释放函数节点，会调VSMaterial::DeleteShaderFunction从m_pShaderFunctionArray中删除,这样
	//m_pShaderFunctionArray数量就会减少，这个FOR循环中i继续加1，而m_pShaderFunctionArray.size()却减少
	//导致有些节点不能删除
	//所以用一个临时变量替代，然后把这个m_pShaderFunctionArray对应的设置为空，这样删除时找不到节点，能把正节点
	//正常释放，而且m_pShaderFunctionArray.size()保持不变。
	for(unsigned int i = 0 ; i < m_pShaderFunctionArray.GetNum() ; i++)
	{
		VSShaderFunction * pShaderFunction = m_pShaderFunctionArray[i];
		m_pShaderFunctionArray[i] = NULL;
		VSMAC_DELETE(pShaderFunction);
	}
	m_pShaderFunctionArray.Clear();
	
	m_pLightShaderFunction = NULL;
	m_pPostEffectShaderFunction = NULL;
	m_pShaderMainFunction.Clear();
	m_bIsAllReadyLink = false;
	
	for (unsigned int i = 0 ; i < VSPass::PT_MAX ; i++)
	{	
		m_pPass[i] = NULL;
	}
	m_ShaderCustomValue.Clear();
	m_ShaderCustomTex.Clear();

}
VSMaterial::VSMaterial(const VSUsedName &ShowName, unsigned int uiMUT)
{

	m_ShowName = ShowName;
	//Clear必须要在shaderFunction创建前面
	m_pShaderFunctionArray.Clear();
	m_pShaderMainFunction.Clear();
	m_pLightShaderFunction = NULL;
	m_pPostEffectShaderFunction = NULL;
	if (uiMUT == MUT_PHONE)
	{
		m_pShaderMainFunction.AddElement(VS_NEW VSPhoneShaderFunction(_T("PSMain"),this));
		
	}
	else if (uiMUT == MUT_OREN_NAYAR)
	{
		m_pShaderMainFunction.AddElement(VS_NEW VSOrenNayarShaderFunction(_T("PSMain"),this));
	}
	else if (uiMUT == MUT_CUSTOM)
	{
		m_pShaderMainFunction.AddElement(VS_NEW VSCustomShaderFunction(_T("PSMain"),this));
	}
	else if (uiMUT == MUT_LIGHT)
	{
		m_pLightShaderFunction = VS_NEW VSLightShaderFunction(_T("PSMain"),this);
	}
	else if (uiMUT == MUT_POSTEFFECT)
	{
		m_pPostEffectShaderFunction = VS_NEW VSPostEffectShaderFunction(_T("PSMain"), this);
	}
	m_bIsAllReadyLink = false;
	m_bIsCombine = false;
	m_pPass[VSPass::PT_MATERIAL] = VS_NEW VSMaterialPass();
	m_pPass[VSPass::PT_NORMALDEPTH] = VS_NEW VSNormalDepthPass();
	m_pPass[VSPass::PT_POINT_CUBE_SHADOW] = VS_NEW VSCubeShadowPass();
	m_pPass[VSPass::PT_POINT_VOLUME_SHADOW] = VS_NEW VSVolumeShadowPass();
	m_pPass[VSPass::PT_PREZ] = VSPreZPass::GetDefault();
	m_pPass[VSPass::PT_SHADOW] = VS_NEW VSShadowPass();
	m_pPass[VSPass::PT_DUAL_PARABOLOID_SHADOW] = VS_NEW VSDualParaboloidShadowPass();
	m_pPass[VSPass::PT_LIGHT_FUNCTION] = VS_NEW VSLightFunPass();
	m_pPass[VSPass::PT_POSTEFFECT] = VS_NEW VSPostEffectPass();
	m_pPass[VSPass::PT_INDIRECT] = VS_NEW VSIndirectRenderPass();
	VSMAC_ASSERT(m_pPass[VSPass::PT_MATERIAL] && m_pPass[VSPass::PT_INDIRECT]);
	m_uiCustomLayer = 10;
	m_uiTessellationPartitioning = 0;
	m_ShaderCustomValue.Clear();
	m_ShaderCustomTex.Clear();
}
void VSMaterial::AddPass(unsigned int uiMUT)
{
	if (m_pLightShaderFunction || m_pPostEffectShaderFunction)
	{
		return ;
	}
	if (uiMUT == MUT_PHONE)
	{
		m_pShaderMainFunction.AddElement(VS_NEW VSPhoneShaderFunction(_T("PSMain"),this));

	}
	else if (uiMUT == MUT_OREN_NAYAR)
	{
		m_pShaderMainFunction.AddElement(VS_NEW VSOrenNayarShaderFunction(_T("PSMain"),this));
	}
}
void VSMaterial::SetTessellationPartitioning(unsigned int uiTessellationPartitioning)
{
	VSMAC_ASSERT(uiTessellationPartitioning < TP_MAX);
	m_uiTessellationPartitioning = uiTessellationPartitioning;
}
void VSMaterial::SetBlendState(VSBlendState * pBlendState,unsigned int uiPassId)
{
	VSMAC_ASSERT(uiPassId < m_pShaderMainFunction.GetNum());
	m_pShaderMainFunction[uiPassId]->SetBlendState(pBlendState);
}
void VSMaterial::SetDepthStencilState(VSDepthStencilState *pDepthStencilState,unsigned int uiPassId)
{
	VSMAC_ASSERT(uiPassId < m_pShaderMainFunction.GetNum());
	m_pShaderMainFunction[uiPassId]->SetDepthStencilState(pDepthStencilState);
}
void VSMaterial::SetRasterizerState(VSRasterizerState *pRasterizerState,unsigned int uiPassId)
{
	VSMAC_ASSERT(uiPassId < m_pShaderMainFunction.GetNum());
	m_pShaderMainFunction[uiPassId]->SetRasterizerState(pRasterizerState);
}
void VSMaterial::AddClipPlane(const VSPlane3 & Plane,unsigned int uiPassId)
{
	VSMAC_ASSERT(uiPassId < m_pShaderMainFunction.GetNum());
	m_pShaderMainFunction[uiPassId]->AddClipPlane(Plane);
}
void VSMaterial::AddScissorRect(const VSRect2 & Rect,unsigned int uiPassId)
{
	VSMAC_ASSERT(uiPassId < m_pShaderMainFunction.GetNum());
	m_pShaderMainFunction[uiPassId]->AddScissorRect(Rect);
}
void VSMaterial::AddShaderFunction(VSShaderFunction *pShaderFunction)
{
	if(pShaderFunction)
	{
		m_pShaderFunctionArray.AddElement(pShaderFunction);
	}

}
void VSMaterial::DeleteShaderFunction(VSShaderFunction *pShaderFunction)
{
	if(pShaderFunction)
	{
		if (m_pLightShaderFunction && m_pLightShaderFunction == pShaderFunction)
		{
			// can't delete LightShaderFunction;
			return ;
		}

		if (m_pPostEffectShaderFunction && m_pPostEffectShaderFunction == pShaderFunction)
		{
			// can't delete PostEffectShaderFunction;
			return;
		}
		if (m_pShaderMainFunction.GetNum() > 0 && pShaderFunction == m_pShaderMainFunction[0])
		{
			//can't delete first 
			return ;
		}
		for (unsigned int i = 0 ; i < m_pShaderFunctionArray.GetNum() ; i++)
		{
			if(m_pShaderFunctionArray[i] == pShaderFunction)
			{
				for (unsigned int j = 0 ; j < pShaderFunction->GetOutputNodeNum() ;j++)
				{
					VSOutputNode * pOutputNode = pShaderFunction->GetOutputNode(j);
					pOutputNode->UnConnection();
				}				
				m_pShaderFunctionArray.Erase(i);
				VSMAC_DELETE(pShaderFunction);
				return ;
			}
		}
		
	}

}


void VSMaterial::LinkAllResource()
{
	if(m_bIsAllReadyLink)
		return ;
	m_bIsAllReadyLink = true;
	m_ShaderCustomTex.Clear();
	for(unsigned int i = 0 ; i < m_pShaderFunctionArray.GetNum() ; i++)
	{
		VSTexSampler *Temp = DynamicCast<VSTexSampler>(m_pShaderFunctionArray[i]);
		if(!Temp)
			continue;
		
		CustomTexSampler CTex;
		CTex.ConstValueName = Temp->GetShowName();
		CTex.m_pTexture = Temp->GetTexture();
		m_ShaderCustomTex.AddElement(CTex);
		
	}
	m_ShaderCustomValue.Clear();
	for(unsigned int i = 0 ; i < m_pShaderFunctionArray.GetNum() ; i++)
	{
		VSConstValue *Temp = DynamicCast<VSConstValue>(m_pShaderFunctionArray[i]);
		if(!Temp || !Temp->IsCustom())
			continue;
		if (Temp->GetType() == VSUserConstant::VT_FLOAT)
		{
			CustomFloatValue CustomValueTemp;
			CustomValueTemp.ConstValueName = Temp->GetShowName();
			CustomValueTemp.Value = ((VSConstFloatValue *)Temp)->GetValue();
			m_ShaderCustomValue.AddElement(CustomValueTemp);
		}
		
	}

}
int VSMaterial::GetInstanceIndex(const VSConstFloatValue * pCFValue)
{
	LinkAllResource();
	for (unsigned int i = 0; i < m_ShaderCustomValue.GetNum(); i++)
	{
		if (m_ShaderCustomValue[i].ConstValueName == pCFValue->GetShowName())
		{
			return i;
		}
	}
	VSMAC_ASSERT(0);
	return -1;
}
void VSMaterial::GetInstanceVertexFormat(VSArray<VSVertexFormat::VERTEXFORMAT_TYPE>& FormatArray, unsigned int &uiOffset)
{
	LinkAllResource();
	unsigned int TypeMap[] = { VSDataBuffer::DT_FLOAT32_1, VSDataBuffer::DT_FLOAT32_2, VSDataBuffer::DT_FLOAT32_3, VSDataBuffer::DT_FLOAT32_4 };
	for (unsigned int i = 0; i < m_ShaderCustomValue.GetNum(); i++)
	{
		CustomFloatValue & FloatValue = m_ShaderCustomValue[i];
		VSVertexFormat::VERTEXFORMAT_TYPE MaterialInstance_FT;
		MaterialInstance_FT.Stream = 1;
		MaterialInstance_FT.DataType = TypeMap[FloatValue.Value.GetNum() - 1];
		MaterialInstance_FT.Offset = uiOffset;
		MaterialInstance_FT.Semantics = VSVertexFormat::VF_MATERIAL_INSTANCE;
		MaterialInstance_FT.SemanticsIndex = i;
		MaterialInstance_FT.InstanceDataStepRate = VSVertexFormat::VERTEXFORMAT_TYPE::IDSR_PER_INSTANCE_DATA;
		FormatArray.AddElement(MaterialInstance_FT);
		uiOffset += VSDataBuffer::ms_uiDataTypeByte[MaterialInstance_FT.DataType];
	}
}
void VSMaterial::ClearShaderFunctionVisitFlagFalse()
{
	for(unsigned int i = 0 ; i < m_pShaderFunctionArray.GetNum() ; i++)
	{
		m_pShaderFunctionArray[i]->m_bIsVisited = 0;
	}
}
bool VSMaterial::GetCustomDeclareString(VSString & OutString,MaterialShaderPara &MSPara)
{
	VSCustomShaderFunction * pCustomShaderFunction = DynamicCast<VSCustomShaderFunction>(m_pShaderMainFunction[MSPara.uiPassId]);
	if (pCustomShaderFunction)
	{
		OutString = pCustomShaderFunction->m_LightFunctionString;
		return true;
	}
	return false;
}
bool VSMaterial::GetVShaderTreeString(VSString & OutString, MaterialShaderPara &MSPara)
{
	
	VSMap<VSShaderFunction *, VSArray<VSShaderFunction *>> NoValidMap;
	VSShaderStringFactory::ms_ShaderValueIndex = 0;
	unsigned char uPassId = MSPara.uiPassId;
	for (unsigned int i = 0; i < m_pShaderFunctionArray.GetNum(); i++)
	{
		m_pShaderFunctionArray[i]->ResetInShaderName(MSPara);
	}
	if (!m_pLightShaderFunction && !m_pPostEffectShaderFunction)
	{
		bool Temp = m_pShaderMainFunction[uPassId]->CheckChildNodeValidAll(NoValidMap, MSPara);
		if (!Temp)
		{
			return false;
		}
		m_pShaderMainFunction[uPassId]->ClearShaderTreeStringFlag();
		return m_pShaderMainFunction[uPassId]->GetVShaderTreeString(OutString,MSPara);
	}
	return true;
}
bool VSMaterial::GetDShaderTreeString(VSString & OutString, MaterialShaderPara &MSPara)
{

	VSMap<VSShaderFunction *, VSArray<VSShaderFunction *>> NoValidMap;
	VSShaderStringFactory::ms_ShaderValueIndex = 0;
	unsigned char uPassId = MSPara.uiPassId;
	for (unsigned int i = 0; i < m_pShaderFunctionArray.GetNum(); i++)
	{
		m_pShaderFunctionArray[i]->ResetInShaderName(MSPara);
	}
	if (!m_pLightShaderFunction && !m_pPostEffectShaderFunction)
	{
		bool Temp = m_pShaderMainFunction[uPassId]->CheckChildNodeValidAll(NoValidMap, MSPara);
		if (!Temp)
		{
			return false;
		}
		m_pShaderMainFunction[uPassId]->ClearShaderTreeStringFlag();
		return m_pShaderMainFunction[uPassId]->GetDShaderTreeString(OutString, MSPara);
	}
	return true;
}
bool VSMaterial::GetPShaderTreeString(VSString & OutString,MaterialShaderPara &MSPara)
{
	
	VSMap<VSShaderFunction *, VSArray<VSShaderFunction *>> NoValidMap;
	VSShaderStringFactory::ms_ShaderValueIndex = 0;
	unsigned char uPassId = MSPara.uiPassId;
	for (unsigned int i = 0 ; i < m_pShaderFunctionArray.GetNum() ; i++)
	{
		m_pShaderFunctionArray[i]->ResetInShaderName(MSPara);
	}
	if (m_pLightShaderFunction)
	{
		bool Temp = m_pLightShaderFunction->CheckChildNodeValidAll(NoValidMap, MSPara);
		if (!Temp)
		{
			return false;
		}
		m_pLightShaderFunction->ClearShaderTreeStringFlag();
		return m_pLightShaderFunction->GetShaderTreeString(OutString, MSPara);
	}
	else if (m_pPostEffectShaderFunction)
	{
		bool Temp = m_pPostEffectShaderFunction->CheckChildNodeValidAll(NoValidMap, MSPara);
		if (!Temp)
		{
			return false;
		}
		m_pPostEffectShaderFunction->ClearShaderTreeStringFlag();
		return m_pPostEffectShaderFunction->GetShaderTreeString(OutString, MSPara);
	}
	else
	{
		bool Temp = m_pShaderMainFunction[uPassId]->CheckChildNodeValidAll(NoValidMap, MSPara);
		if (!Temp)
		{
			return false;
		}
		m_pShaderMainFunction[uPassId]->ClearShaderTreeStringFlag();

		VSCustomShaderFunction * pCustomShaderFunction = DynamicCast<VSCustomShaderFunction>(m_pShaderMainFunction[uPassId]);
		if (pCustomShaderFunction)
		{
			pCustomShaderFunction->CreatLightFunctionString(MSPara);
		}	
		return m_pShaderMainFunction[uPassId]->GetPShaderTreeString(OutString,MSPara);
	}
	
}
bool VSMaterial::PostClone(VSObject * pObjectSrc)
{
	LinkAllResource();
	return true;
}

bool VSMaterial::PostLoad(VSStream* pStream)
{
	LinkAllResource();
	return true;
}
void VSMaterial::SetGlobleValue( MaterialShaderPara &MSPara,
	VSVShader * pVShader , VSPShader * pPShader,
	VSGShader * pGShader, VSHShader * pHShader, VSDShader * pDShader)
{

	if (m_pShaderMainFunction.GetNum())
	{
		m_pShaderMainFunction[MSPara.uiPassId]->SetGlobleValue(MSPara, pVShader, pPShader, pGShader, pHShader, pDShader);
	}
}
void VSMaterial::CreateCustomTexture(VSShader * pShader)
{
	
	for(unsigned int i = 0 ; i < m_pShaderFunctionArray.GetNum(); i++)
	{
		VSTexSampler *Temp = DynamicCast<VSTexSampler>(m_pShaderFunctionArray[i]);
		if(!Temp || !Temp->m_bIsVisited)
			continue;
		unsigned int RegisterID = 0;
		if (pShader->m_pUserSampler.GetNum())
		{
			VSUserSampler * Last = pShader->m_pUserSampler[pShader->m_pUserSampler.GetNum() - 1];
			if (Last)
			{
				RegisterID = Last->GetRegisterIndex() + Last->GetRegisterNum();
			}
		}
		VSUserSampler * pSampler = VS_NEW VSUserSampler(Temp->GetShowName(),Temp->GetTexType(),RegisterID,1);
		pShader->m_pUserSampler.AddElement(pSampler);
	}
}
void VSMaterial::CreateTextureDeclare(VSString & OutString, unsigned int& uiRegisterID, MaterialShaderPara &MSPara)
{
	VSArray<VSTexSampler *> TexSampler[VSEngineFlag::SS_MAX];
	for(unsigned int i = 0 ; i < m_pShaderFunctionArray.GetNum() ; i++)
	{
		VSTexSampler *Temp = DynamicCast<VSTexSampler>(m_pShaderFunctionArray[i]);
		if(!Temp|| !Temp->m_bIsVisited)
			continue;
		TexSampler[Temp->GetSamplerSource()].AddElement(Temp);
		
	}
	
	unsigned int uiShaderSamplerRegisterID = 0;
	for (unsigned int i = 0 ; i < VSEngineFlag::SS_MAX ; i++)
	{
		for (unsigned int j = 0 ; j < TexSampler[i].GetNum() ; j++)
		{
			TexSampler[i][j]->GetDeclareString(OutString, uiRegisterID, MSPara);
			uiRegisterID++;
		}
		if (i == 0)
		{
			uiShaderSamplerRegisterID = uiRegisterID;
		}
	}
	for (unsigned int i = 1; i < VSEngineFlag::SS_MAX; i++)
	{
		if (TexSampler[i].GetNum() > 0)
		{
			VSShaderStringFactory::ms_uiShareSamplerRegisterID[i - 1] = uiShaderSamplerRegisterID;
			uiShaderSamplerRegisterID++;
		}
	}
}
void VSMaterial::CreateCustomValue(VSShader * pShader)
{
	for(unsigned int i = 0 ; i < m_pShaderFunctionArray.GetNum(); i++)
	{
		VSConstValue *Temp = DynamicCast<VSConstValue>(m_pShaderFunctionArray[i]);
		if(!Temp || !Temp->IsCustom() || !Temp->m_bIsVisited)
			continue;
		VSUserConstant * UserConstantTemp = NULL;
		unsigned int uiRegisterIndex = 0;
		if (pShader->m_pUserConstant.GetNum())
		{
			VSUserConstant * Last = pShader->m_pUserConstant[pShader->m_pUserConstant.GetNum() - 1];
			if (Last)
			{
				uiRegisterIndex = Last->GetRegisterIndex() + Last->GetRegisterNum();
			}
		}
		UserConstantTemp = VS_NEW VSUserConstant(Temp->GetShowName(), Temp->GetOutputNode(0)->GetNodeName(),Temp->GetSize(), uiRegisterIndex, 1, Temp->GetType());
		pShader->m_pUserConstant.AddElement(UserConstantTemp);
	}
}


void VSMaterial::CreateConstValueDeclare(VSString & OutString, unsigned int& uiRegisterID, MaterialShaderPara &MSPara)
{

	for(unsigned int i = 0 ; i < m_pShaderFunctionArray.GetNum() ; i++)
	{
		VSConstValue *Temp = DynamicCast<VSConstValue>(m_pShaderFunctionArray[i]);
		if (!Temp || !Temp->m_bIsVisited || !Temp->IsCustom())
			continue;
		Temp->GetDeclareString(OutString, uiRegisterID, MSPara);
		uiRegisterID++;
	}
}
IMPLEMENT_RTTI(VSMaterialInstance,VSObject)
BEGIN_ADD_PROPERTY(VSMaterialInstance,VSObject)
REGISTER_PROPERTY(m_ShaderCustomValue, ShaderCustomValue, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_ShaderCustomTex,ShaderCustomTex,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pMaterial,Material,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSMaterialInstance)
IMPLEMENT_INITIAL_END
VSMaterialInstance::VSMaterialInstance()
{
	m_pMaterial = NULL;

	m_ShaderCustomValue.Clear();
	m_ShaderCustomTex.Clear();

	for (unsigned int i = 0 ; i < VSPass::PT_MAX ;i++)
	{
		m_pCurVShader[i] = NULL;
		m_pCurPShader[i] = NULL;
		m_pCurGShader[i] = NULL;
		m_pCurHShader[i] = NULL;
		m_pCurDShader[i] = NULL;
	}
}
VSMaterialInstance::VSMaterialInstance(VSMaterialR * pMaterial)
{
	VSMAC_ASSERT(pMaterial);
	m_pMaterial = pMaterial;
	m_pMaterial->GetResource()->LinkAllResource();
	GetAllMaterialPara();
	m_pMaterial->AddLoadEventObject(this);
}
void VSMaterialInstance::GetAllMaterialPara()
{
	VSMAC_ASSERT(m_pMaterial);
	m_ShaderCustomValue = m_pMaterial->GetResource()->m_ShaderCustomValue;	
	m_ShaderCustomTex = m_pMaterial->GetResource()->m_ShaderCustomTex;

}
bool VSMaterialInstance::PostLoad(VSStream* pStream)
{
	if (!m_pMaterial)
	{
		m_pMaterial = (VSMaterialR *)VSMaterial::GetDefaultResource();
		GetAllMaterialPara();		
	}
	m_pMaterial->AddLoadEventObject(this);
	return true;
}
bool VSMaterialInstance::PostClone(VSObject * pObjectSrc)
{
	if (!m_pMaterial)
	{
		m_pMaterial = (VSMaterialR *)VSMaterial::GetDefaultResource();
		GetAllMaterialPara();
	}
	m_pMaterial->AddLoadEventObject(this);
	return true;
}
void VSMaterialInstance::LoadedEvent(VSResourceProxyBase * pResourceProxy, void * Data)
{	
	for (unsigned int i = 0; i < VSPass::PT_MAX;i++)
	{
		m_pCurVShader[i] = NULL;
		m_pCurPShader[i] = NULL;
		m_pCurGShader[i] = NULL;
		m_pCurHShader[i] = NULL;
		m_pCurDShader[i] = NULL;
	}
}
void VSMaterialInstance::GetStreamResource(VSArray<VSResourceProxyBase*>& pResourceProxy, StreamInformation_TYPE& StreamInformation)const
{
	for (const auto& Temp : m_ShaderCustomTex)
	{
		if (Temp.m_pTexture != VSTexAllState::GetDefaultResource())
		{
			if(Temp.m_pTexture->CanWriteStream(StreamInformation))
			{
				pResourceProxy.AddElement(Temp.m_pTexture);
			}
		}
	}
}
VSMaterialInstance::~VSMaterialInstance()
{
	if (m_pMaterial)
	{
		m_pMaterial->DeleteLoadEventObject(this);
	}
	m_pMaterial = NULL;

	m_ShaderCustomValue.Clear();
	m_ShaderCustomTex.Clear();

	for (unsigned int i = 0 ; i < VSPass::PT_MAX ;i++)
	{
		m_pCurVShader[i] = NULL;
		m_pCurPShader[i] = NULL;
		m_pCurGShader[i] = NULL;
		m_pCurHShader[i] = NULL;
		m_pCurDShader[i] = NULL;
	}
	
}
void VSMaterialInstance::DeleteShaderValue(const VSUsedName &Name)
{
	for(unsigned int i = 0 ; i < m_ShaderCustomValue.GetNum() ; i++)
	{
		if(m_ShaderCustomValue[i].ConstValueName == Name)
		{
			m_ShaderCustomValue.Erase(i);
			return ;
		}
	}
}

void VSMaterialInstance::SetShaderValue(const VSUsedName & Name,void *fValue,unsigned int uiSize)
{
	VSMAC_ASSERT(uiSize && fValue)
	for(unsigned int i = 0 ; i < m_ShaderCustomValue.GetNum() ; i++)
	{
		if(m_ShaderCustomValue[i].ConstValueName == Name)
		{
			VSMemcpy(&m_ShaderCustomValue[i].Value[0],fValue,sizeof(VSREAL) * m_ShaderCustomValue[i].Value.GetNum());
			return;
		}
	}
	//如果没有
	CustomFloatValue Temp;
	Temp.ConstValueName = Name;
	Temp.Value.SetBufferNum(uiSize);
	VSMemcpy(&Temp.Value[0],fValue,sizeof(VSREAL) * uiSize);
	m_ShaderCustomValue.AddElement(Temp);
	return ;
}
void VSMaterialInstance::DeleteShaderTexture(const VSUsedName & TexSamplerNodeName)
{
	for(unsigned int i = 0 ; i < m_ShaderCustomTex.GetNum() ; i++)
	{
		if(TexSamplerNodeName == m_ShaderCustomTex[i].ConstValueName)
		{
			m_ShaderCustomTex.Erase(i);
			return ;
		}
	}
}
void VSMaterialInstance::GetInstanceData(unsigned char* pInstanceDataBuffer,unsigned int &GetSize)
{
	
	VSArrayOrder<CustomFloatValue> & MaterialShaderCustomValue = m_pMaterial->GetResource()->m_ShaderCustomValue;
	for (unsigned int i = 0; i < MaterialShaderCustomValue.GetNum(); i++)
	{
		bool bFind = false;
		for (unsigned int j = 0; j < m_ShaderCustomValue.GetNum(); j++)
		{
			if (MaterialShaderCustomValue[i].ConstValueName == m_ShaderCustomValue[j].ConstValueName)
			{
				unsigned int uiSize = m_ShaderCustomValue[j].Value.GetNum() * sizeof(VSREAL);
				VSMemcpy(pInstanceDataBuffer, 
					m_ShaderCustomValue[j].Value.GetBuffer(), uiSize);
				GetSize += uiSize;
				pInstanceDataBuffer += uiSize;
				bFind = true;
				break;
			}
		}
		if (!bFind)
		{
			unsigned int uiSize = MaterialShaderCustomValue[i].Value.GetNum() * sizeof(VSREAL);
			VSMemcpy(pInstanceDataBuffer,
				MaterialShaderCustomValue[i].Value.GetBuffer(), uiSize);
			GetSize += uiSize;
			pInstanceDataBuffer += uiSize;
		}
	}
}
void VSMaterialInstance::SetShaderTexture(const VSUsedName & TexSamplerNodeName,VSTexAllStateR * pTex)
{
	for(unsigned int i = 0 ; i < m_ShaderCustomTex.GetNum() ; i++)
	{
		if(TexSamplerNodeName == m_ShaderCustomTex[i].ConstValueName)
		{
			m_ShaderCustomTex[i].m_pTexture = pTex;
			return ;
		}
	}
	CustomTexSampler CTex;
	CTex.ConstValueName = TexSamplerNodeName;
	CTex.m_pTexture = pTex;
	m_ShaderCustomTex.AddElement(CTex);
}

void VSMaterialInstance::SetShaderValue(VSShader * pShader)
{
	VSMAC_ASSERT(pShader);
	for (unsigned int i = 0 ; i < m_ShaderCustomValue.GetNum() ; i++)
	{
		pShader->SetParam(m_ShaderCustomValue[i].ConstValueName, m_ShaderCustomValue[i].Value.GetBuffer());
	}
	for (unsigned int i = 0 ; i < m_ShaderCustomTex.GetNum() ; i++)
	{
		if (m_ShaderCustomTex[i].m_pTexture)
		{
			pShader->SetParam(m_ShaderCustomTex[i].ConstValueName, m_ShaderCustomTex[i].m_pTexture->GetResource());
		}
		else
		{
			pShader->SetParam(m_ShaderCustomTex[i].ConstValueName, (VSTexAllState *)NULL);
		}
		
	}
}
VSMaterialOnlyTexture::VSMaterialOnlyTexture()
{

}
VSMaterialOnlyTexture::VSMaterialOnlyTexture(const VSUsedName &ShowName,VSTexAllStateR * pTexture)
:VSMaterial(ShowName)
{
	VS2DTexSampler* p2DTexSamplerNode = VS_NEW VS2DTexSampler(_T("DiffuseTexture"),this);
	p2DTexSamplerNode->SetTexture(pTexture);
	//p2DTexSamplerNode->SetVESRGB(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VSConstFloatValue * pSpecularColor = VS_NEW VSConstFloatValue(_T("SpecluarColor"),this,4,false);
	pSpecularColor->SetValue(0,1.0f);
	pSpecularColor->SetValue(1,1.0f);
	pSpecularColor->SetValue(2,1.0f);
	pSpecularColor->SetValue(3,1.0f);
	VSConstFloatValue * pSpecularPow = VS_NEW VSConstFloatValue(_T("SpecularPow"),this,1,false);
	pSpecularPow->SetValue(0,300);
	m_pShaderMainFunction[0]->GetInputNode(_T("DiffuseColor"))->Connection(p2DTexSamplerNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularColor"))->Connection(pSpecularColor->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularPow"))->Connection(pSpecularPow->GetOutputNode(0));
	m_ResourceName = _T("_OnlyTexture");

}
VSMaterialOnlyTexture::~VSMaterialOnlyTexture()
{

}
VSMaterialTextureAndNormal::VSMaterialTextureAndNormal()
{

}
VSMaterialTextureAndNormal::VSMaterialTextureAndNormal(const VSUsedName &ShowName,VSTexAllStateR * pDiffuseTexture,VSTexAllStateR * pNormalTexture)
:VSMaterial(ShowName)
{
	VS2DTexSampler* p2DTexSamplerNode = VS_NEW VS2DTexSampler(_T("DiffuseTexture"),this);
	p2DTexSamplerNode->SetTexture(pDiffuseTexture);

	VS2DTexSampler *pNormalNode = VS_NEW VS2DTexSampler(_T("NormalTexture"),this);
	pNormalNode->SetTexture(pNormalTexture);
	pNormalNode->SetVEDecode(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VSConstFloatValue * pSpecularColor = VS_NEW VSConstFloatValue(_T("SpecluarColor"),this,4,false);
	pSpecularColor->SetValue(0,0.5f);
	pSpecularColor->SetValue(1,0.5f);
	pSpecularColor->SetValue(2,0.5f);
	pSpecularColor->SetValue(3,1.0f);
	VSConstFloatValue * pSpecularPow = VS_NEW VSConstFloatValue(_T("SpecularPow"),this,1,false);
	pSpecularPow ->SetValue(0,100.0f);
	m_pShaderMainFunction[0]->GetInputNode(_T("DiffuseColor"))->Connection(p2DTexSamplerNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("Normal"))->Connection(pNormalNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularColor"))->Connection(pSpecularColor->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularPow"))->Connection(pSpecularPow->GetOutputNode(0));
	m_ResourceName = _T("TextureAndNormal");
}
VSMaterialTextureAndNormal::~VSMaterialTextureAndNormal()
{

}

VSMaterialInstanceTest::VSMaterialInstanceTest()
{

}
VSMaterialInstanceTest::VSMaterialInstanceTest(const VSUsedName &ShowName, VSTexAllStateR * pDiffuseTexture, VSTexAllStateR * pNormalTexture)
:VSMaterial(ShowName)
{
	VS2DTexSampler* p2DTexSamplerNode = VS_NEW VS2DTexSampler(_T("DiffuseTexture"), this);
	p2DTexSamplerNode->SetTexture(pDiffuseTexture);

	VS2DTexSampler *pNormalNode = VS_NEW VS2DTexSampler(_T("NormalTexture"), this);
	pNormalNode->SetTexture(pNormalTexture);
	pNormalNode->SetVEDecode(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VSConstFloatValue * pDiffuseColor = VS_NEW VSConstFloatValue(_T("DiffuseColor"), this, 4, true);
	pDiffuseColor->SetValue(0, 1.0f);
	pDiffuseColor->SetValue(1, 1.0f);
	pDiffuseColor->SetValue(2, 1.0f);
	pDiffuseColor->SetValue(3, 1.0f);

	VSMul * pMul = VS_NEW VSMul(_T("Mul"), this);
	pMul->GetInputNode(VSInputNode::INI_A)->Connection(pDiffuseColor->GetOutputNode(VSOutputNode::ONI_VALUE));
	pMul->GetInputNode(VSInputNode::INI_B)->Connection(p2DTexSamplerNode->GetOutputNode(VSOutputNode::ONI_COLOR));

	VSConstFloatValue * pSpecularColor = VS_NEW VSConstFloatValue(_T("SpecluarColor"), this, 4, false);
	pSpecularColor->SetValue(0, 0.5f);
	pSpecularColor->SetValue(1, 0.5f);
	pSpecularColor->SetValue(2, 0.5f);
	pSpecularColor->SetValue(3, 1.0f);
	VSConstFloatValue * pSpecularPow = VS_NEW VSConstFloatValue(_T("SpecularPow"), this, 1, false);
	pSpecularPow->SetValue(0, 100.0f);
	m_pShaderMainFunction[0]->GetInputNode(_T("DiffuseColor"))->Connection(pMul->GetOutputNode(VSOutputNode::ONI_VALUE));
	m_pShaderMainFunction[0]->GetInputNode(_T("Normal"))->Connection(pNormalNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularColor"))->Connection(pSpecularColor->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularPow"))->Connection(pSpecularPow->GetOutputNode(0));
	m_ResourceName = _T("InstanceTest");
}
VSMaterialInstanceTest::~VSMaterialInstanceTest()
{

}

VSMaterialOnlyEmissive::VSMaterialOnlyEmissive(const VSUsedName &ShowName)
:VSMaterial(ShowName)
{

	VSConstFloatValue * pEmissiveColor = VS_NEW VSConstFloatValue(_T("EmissiveColor"),this,4,true);
	pEmissiveColor->SetValue(0,1.0f);
	pEmissiveColor->SetValue(1,1.0f);
	pEmissiveColor->SetValue(2,1.0f);
	pEmissiveColor->SetValue(3,1.0f);

	m_pShaderMainFunction[0]->GetInputNode(VSPhoneShaderFunction::IN_EMISSIVE_COLOR)->Connection(pEmissiveColor->GetOutputNode(VSOutputNode::ONI_VALUE));
	m_ResourceName = _T("OnlyEmissive");
}
VSMaterialOnlyEmissive::~VSMaterialOnlyEmissive()
{

}
VSMaterialNoLight::VSMaterialNoLight(const VSUsedName &ShowName)
:VSMaterial(ShowName)
{
	// float One = 1.0f;
	VSConstFloatValue * pOne = VS_NEW VSConstFloatValue(_T("One"), this, 1, false);
	pOne->SetValue(0, 1.0f);

	// WorldPos
	VSWorldPos * pWorldPos = VS_NEW VSWorldPos(_T("WorldPos"), this);

	//WorldPos_One = float4(WorldPos,1.0f);
	VSMakeValue * pWorldPos_One = VS_NEW VSMakeValue(_T("WorldPos_One"), this, 2);
	pWorldPos_One->GetInputNode(VSInputNode::INI_A)->Connection(pWorldPos->GetOutputNode(VSOutputNode::ONI_VALUE));
	pWorldPos_One->GetInputNode(VSInputNode::INI_B)->Connection(pOne->GetOutputNode(VSOutputNode::ONI_VALUE));

	//float4X4 ReflectViewProject;
	VSConstFloatValue * pReflectViewProject_1 = VS_NEW VSConstFloatValue(_T("ReflectViewProject_1 "), this, 4, true);
	VSConstFloatValue * pReflectViewProject_2 = VS_NEW VSConstFloatValue(_T("ReflectViewProject_2 "), this, 4, true);
	VSConstFloatValue * pReflectViewProject_3 = VS_NEW VSConstFloatValue(_T("ReflectViewProject_3 "), this, 4, true);
	VSConstFloatValue * pReflectViewProject_4 = VS_NEW VSConstFloatValue(_T("ReflectViewProject_4 "), this, 4, true);

	//float Dot_1 = dot(WorldPos_One,ReflectViewProject_1);
	VSDot * pDot_1 = VS_NEW VSDot(_T("Dot_1"), this);
	pDot_1->GetInputNode(VSInputNode::INI_A)->Connection(pWorldPos_One->GetOutputNode(VSOutputNode::ONI_VALUE));
	pDot_1->GetInputNode(VSInputNode::INI_B)->Connection(pReflectViewProject_1->GetOutputNode(VSOutputNode::ONI_VALUE));

	//float Dot_2 = dot(WorldPos_One,ReflectViewProject_2);
	VSDot * pDot_2 = VS_NEW VSDot(_T("Dot_2"), this);
	pDot_2->GetInputNode(VSInputNode::INI_A)->Connection(pWorldPos_One->GetOutputNode(VSOutputNode::ONI_VALUE));
	pDot_2->GetInputNode(VSInputNode::INI_B)->Connection(pReflectViewProject_2->GetOutputNode(VSOutputNode::ONI_VALUE));

	//float Dot_3 = dot(WorldPos_One,ReflectViewProject_3);
	VSDot * pDot_3 = VS_NEW VSDot(_T("Dot_3"), this);
	pDot_3->GetInputNode(VSInputNode::INI_A)->Connection(pWorldPos_One->GetOutputNode(VSOutputNode::ONI_VALUE));
	pDot_3->GetInputNode(VSInputNode::INI_B)->Connection(pReflectViewProject_3->GetOutputNode(VSOutputNode::ONI_VALUE));

	//float Dot_4 = dot(WorldPos_One,ReflectViewProject_4);
	VSDot * pDot_4 = VS_NEW VSDot(_T("Dot_4"), this);
	pDot_4->GetInputNode(VSInputNode::INI_A)->Connection(pWorldPos_One->GetOutputNode(VSOutputNode::ONI_VALUE));
	pDot_4->GetInputNode(VSInputNode::INI_B)->Connection(pReflectViewProject_4->GetOutputNode(VSOutputNode::ONI_VALUE));

	//float2 Dot_12 = float2(Dot_1,Dot_2);
	VSMakeValue * pDot_12 = VS_NEW VSMakeValue(_T("Dot_12"), this, 2);
	pDot_12->GetInputNode(VSInputNode::INI_A)->Connection(pDot_1->GetOutputNode(VSOutputNode::ONI_VALUE));
	pDot_12->GetInputNode(VSInputNode::INI_B)->Connection(pDot_2->GetOutputNode(VSOutputNode::ONI_VALUE));

	//float2 Dot_44 = float2(Dot_4,Dot_4);
	VSMakeValue * pDot_44 = VS_NEW VSMakeValue(_T("Dot_44"), this, 2);
	pDot_44->GetInputNode(VSInputNode::INI_A)->Connection(pDot_4->GetOutputNode(VSOutputNode::ONI_VALUE));
	pDot_44->GetInputNode(VSInputNode::INI_B)->Connection(pDot_4->GetOutputNode(VSOutputNode::ONI_VALUE));

	// float2 Div = Dot_12 / Dot_44;
	VSDiv *pDiv = VS_NEW VSDiv(_T("Div"), this);
	pDiv->GetInputNode(VSInputNode::INI_A)->Connection(pDot_12->GetOutputNode(VSOutputNode::ONI_VALUE));
	pDiv->GetInputNode(VSInputNode::INI_B)->Connection(pDot_44->GetOutputNode(VSOutputNode::ONI_VALUE));


	VSConstFloatValue * pPointFive = VS_NEW VSConstFloatValue(_T("PointFive"), this, 1, false);
	pPointFive->SetValue(0, 0.5f);

	VSConstFloatValue * pNegPointFive = VS_NEW VSConstFloatValue(_T("NegPointFive"), this, 1, false);
	pNegPointFive->SetValue(0, -0.5f);

	//float2 FiveNegFive = float2(0.5f,-0.5f);
	VSMakeValue * pFiveNegFive = VS_NEW VSMakeValue(_T("FiveNegFive"), this, 2);
	pFiveNegFive->GetInputNode(VSInputNode::INI_A)->Connection(pPointFive->GetOutputNode(VSOutputNode::ONI_VALUE));
	pFiveNegFive->GetInputNode(VSInputNode::INI_B)->Connection(pNegPointFive->GetOutputNode(VSOutputNode::ONI_VALUE));

	// Div * FiveFive
	VSMul * pMul = VS_NEW VSMul(_T("Mul"), this);
	pMul->GetInputNode(VSInputNode::INI_A)->Connection(pDiv->GetOutputNode(VSOutputNode::ONI_VALUE));
	pMul->GetInputNode(VSInputNode::INI_B)->Connection(pFiveNegFive->GetOutputNode(VSOutputNode::ONI_VALUE));


	//float2 FiveFive = float2(0.5f,0.5f);
	VSMakeValue * pFiveFive = VS_NEW VSMakeValue(_T("FiveFive"), this, 2);
	pFiveFive->GetInputNode(VSInputNode::INI_A)->Connection(pPointFive->GetOutputNode(VSOutputNode::ONI_VALUE));
	pFiveFive->GetInputNode(VSInputNode::INI_B)->Connection(pPointFive->GetOutputNode(VSOutputNode::ONI_VALUE));

	VSAdd *pAdd = VS_NEW VSAdd(_T("Add"), this);
	pAdd->GetInputNode(VSInputNode::INI_A)->Connection(pMul->GetOutputNode(VSOutputNode::ONI_VALUE));
	pAdd->GetInputNode(VSInputNode::INI_B)->Connection(pFiveFive->GetOutputNode(VSOutputNode::ONI_VALUE));

	

	VS2DTexSampler* p2DTexSamplerNode = VS_NEW VS2DTexSampler(_T("EmissiveTexture"), this);

	p2DTexSamplerNode->GetInputNode(VSInputNode::INI_TEXCOORD)->Connection(pAdd->GetOutputNode(VSOutputNode::ONI_VALUE));

	m_pShaderMainFunction[0]->GetInputNode(VSPhoneShaderFunction::IN_EMISSIVE_COLOR)->Connection(p2DTexSamplerNode->GetOutputNode(VSOutputNode::ONI_COLOR));
	m_ResourceName = _T("NoLight");
}
VSMaterialNoLight::~VSMaterialNoLight()
{

}
VSMaterialOrenNayar::VSMaterialOrenNayar()
{

}
VSMaterialOrenNayar::VSMaterialOrenNayar(const VSUsedName &ShowName,VSTexAllStateR * pDiffuseTexture,VSTexAllStateR * pNormalTexture,
							   VSTexAllStateR * pEmissiveTexture):VSMaterial(ShowName,MUT_OREN_NAYAR)
{
	VS2DTexSampler* p2DTexSamplerNode = VS_NEW VS2DTexSampler(_T("DiffuseTexture"),this);
	p2DTexSamplerNode->SetTexture(pDiffuseTexture);
	//p2DTexSamplerNode->SetVESRGB(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VS2DTexSampler *pNormalNode = VS_NEW VS2DTexSampler(_T("NormalTexture"),this);
	pNormalNode->SetTexture(pNormalTexture);
	pNormalNode->SetVEDecode(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);


	VS2DTexSampler *pEmissiveNode = VS_NEW VS2DTexSampler(_T("EmissiveTexture"),this);
	pEmissiveNode->SetTexture(pEmissiveTexture);

	VSConstFloatValue * pRoughnessSquared = VS_NEW VSConstFloatValue(_T("RoughnessSquared"),this,1,true);
	pRoughnessSquared ->SetValue(0,0.5f);
	m_pShaderMainFunction[0]->GetInputNode(_T("DiffuseColor"))->Connection(p2DTexSamplerNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("Normal"))->Connection(pNormalNode->GetOutputNode(0));
 	m_pShaderMainFunction[0]->GetInputNode(_T("RoughnessSquared"))->Connection(pRoughnessSquared->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("EmissiveColor"))->Connection(pEmissiveNode->GetOutputNode(0));
	//m_pShaderMainFunction[0]->SetSRGBWrite(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VSConstFloatValue * pFinalAlpah = VS_NEW VSConstFloatValue(_T("FinalAlpha"),this,1,false);
 	pFinalAlpah->SetValue(0,0.6f);
 
 	m_pShaderMainFunction[0]->GetInputNode(_T("Alpha"))->Connection(pFinalAlpah->GetOutputNode(0));

	m_ResourceName = _T("_MaterialOrenNayar");
}
VSMaterialOrenNayar::~VSMaterialOrenNayar()
{

}
VSMaterialPhone::VSMaterialPhone()
{

}
VSMaterialPhone::VSMaterialPhone(const VSUsedName &ShowName,VSTexAllStateR * pDiffuseTexture,VSTexAllStateR * pNormalTexture,
										 VSTexAllStateR * pSpecularTexture, VSTexAllStateR * pEmissiveTexture,bool bPhoneSpecular):VSMaterial(ShowName,MUT_PHONE)
{
	VS2DTexSampler* p2DTexSamplerNode = VS_NEW VS2DTexSampler(_T("DiffuseTexture"),this);
	p2DTexSamplerNode->SetTexture(pDiffuseTexture);
	//p2DTexSamplerNode->SetVESRGB(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VS2DTexSampler *pNormalNode = VS_NEW VS2DTexSampler(_T("NormalTexture"),this);
	pNormalNode->SetTexture(pNormalTexture);
	pNormalNode->SetVEDecode(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VS2DTexSampler *pSpecularNode = VS_NEW VS2DTexSampler(_T("SpecularTexture"),this);
	pSpecularNode->SetTexture(pSpecularTexture);

	VS2DTexSampler *pEmissiveNode = VS_NEW VS2DTexSampler(_T("EmissiveTexture"),this);
	pEmissiveNode->SetTexture(pEmissiveTexture);

	VSConstFloatValue * pSpecularPow = VS_NEW VSConstFloatValue(_T("SpecularPow"),this,1,true);
	pSpecularPow ->SetValue(0,50.0f);
	m_pShaderMainFunction[0]->GetInputNode(_T("DiffuseColor"))->Connection(p2DTexSamplerNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("Normal"))->Connection(pNormalNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularColor"))->Connection(pSpecularNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularPow"))->Connection(pSpecularPow->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("EmissiveColor"))->Connection(pEmissiveNode->GetOutputNode(0));
	//m_pShaderMainFunction[0]->SetSRGBWrite(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VSConstFloatValue * pFinalAlpah = VS_NEW VSConstFloatValue(_T("FinalAlpha"),this,1,false);
	pFinalAlpah->SetValue(0,0.6f);

	m_pShaderMainFunction[0]->GetInputNode(_T("Alpha"))->Connection(pFinalAlpah->GetOutputNode(0));


	if(bPhoneSpecular)
	{
		((VSPhoneShaderFunction *)m_pShaderMainFunction[0])->SetSpecularType(VSPhoneShaderFunction::ST_Phong);
	}
	m_ResourceName = _T("_MaterialPhone");

}
VSMaterialPhone::~VSMaterialPhone()
{

}
VSMaterialPhoneTwoPass::VSMaterialPhoneTwoPass()
{

}
VSMaterialPhoneTwoPass::VSMaterialPhoneTwoPass(const VSUsedName &ShowName, VSTexAllStateR * pDiffuseTexture, VSTexAllStateR * pNormalTexture,
	VSTexAllStateR * pSpecularTexture, VSTexAllStateR * pEmissiveTexture, bool bPhoneSpecular) :VSMaterial(ShowName, MUT_PHONE)
{
	VS2DTexSampler* p2DTexSamplerNode = VS_NEW VS2DTexSampler(_T("DiffuseTexture"), this);
	p2DTexSamplerNode->SetTexture(pDiffuseTexture);
	//p2DTexSamplerNode->SetVESRGB(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VS2DTexSampler *pNormalNode = VS_NEW VS2DTexSampler(_T("NormalTexture"), this);
	pNormalNode->SetTexture(pNormalTexture);
	pNormalNode->SetVEDecode(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VS2DTexSampler *pSpecularNode = VS_NEW VS2DTexSampler(_T("SpecularTexture"), this);
	pSpecularNode->SetTexture(pSpecularTexture);

	VS2DTexSampler *pEmissiveNode = VS_NEW VS2DTexSampler(_T("EmissiveTexture"), this);
	pEmissiveNode->SetTexture(pEmissiveTexture);

	VSConstFloatValue * pSpecularPow = VS_NEW VSConstFloatValue(_T("SpecularPow"), this, 1, true);
	pSpecularPow->SetValue(0, 50.0f);
	m_pShaderMainFunction[0]->GetInputNode(_T("DiffuseColor"))->Connection(p2DTexSamplerNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("Normal"))->Connection(pNormalNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularColor"))->Connection(pSpecularNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularPow"))->Connection(pSpecularPow->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("EmissiveColor"))->Connection(pEmissiveNode->GetOutputNode(0));
	//m_pShaderMainFunction[0]->SetSRGBWrite(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);


	if (bPhoneSpecular)
	{
		((VSPhoneShaderFunction *)m_pShaderMainFunction[0])->SetSpecularType(VSPhoneShaderFunction::ST_Phong);
	}
	m_ResourceName = _T("_MaterialPhoneTwoPass");


	AddPass(MUT_PHONE);
	VSConstFloatValue * pFinalColor = VS_NEW VSConstFloatValue(_T("FinalColor"), this, 3, false);
	pFinalColor->SetValue(0, 1.0f);
	pFinalColor->SetValue(1, 0.0f);
	pFinalColor->SetValue(2, 0.0f);
	m_pShaderMainFunction[1]->GetInputNode(_T("EmissiveColor"))->Connection(pFinalColor->GetOutputNode(0));

	VSConstFloatValue * pFinalAlpah = VS_NEW VSConstFloatValue(_T("FinalAlpha"), this, 1, false);
	pFinalAlpah->SetValue(0, 0.5f);

	m_pShaderMainFunction[1]->GetInputNode(_T("Alpha"))->Connection(pFinalAlpah->GetOutputNode(0));

	VSBlendDesc BlendDesc;
	BlendDesc.bBlendEnable[0] = true;
	BlendDesc.ucBlendOp[0] = VSBlendDesc::BO_ADD;
	BlendDesc.ucDestBlend[0] = VSBlendDesc::BP_INVSRCALPHA;
	BlendDesc.ucSrcBlend[0] = VSBlendDesc::BP_SRCALPHA;
	VSBlendState * pBlendState = VSResourceManager::CreateBlendState(BlendDesc);
	SetBlendState(pBlendState, 1);
}
VSMaterialPhoneTwoPass::~VSMaterialPhoneTwoPass()
{

}
VSMaterialCustom::VSMaterialCustom()
{

}
VSMaterialCustom::VSMaterialCustom(const VSUsedName &ShowName, VSTexAllStateR * pDiffuseTexture, VSTexAllStateR * pNormalTexture, VSTexAllStateR * pEmissiveTexture)
:VSMaterial(ShowName, MUT_CUSTOM)
{
	VSLightDir * pLightDir = VS_NEW VSLightDir(_T("LightDir"), this);

	VSConstFloatValue * pValue = VS_NEW VSConstFloatValue(_T("ValueTest"), this, 1, false);
	pValue->SetValue(0, -1.0f);

	VSMul * pMul1 = VS_NEW VSMul(_T("Mul1"), this);

	pMul1->GetInputNode(0)->Connection(pLightDir->GetOutputNode(0));
	pMul1->GetInputNode(1)->Connection(pValue->GetOutputNode(0));

	VSWorldNormal * pWorldNormal = VS_NEW VSWorldNormal(_T("WorldNormal"), this);

	VSDot * pDot = VS_NEW VSDot(_T("Dot"), this);

	pDot->GetInputNode(0)->Connection(pWorldNormal->GetOutputNode(0));
	pDot->GetInputNode(1)->Connection(pMul1->GetOutputNode(0));

	VSSaturate *pSaturate = VS_NEW VSSaturate(_T("Saturate"), this);
	pSaturate->GetInputNode(0)->Connection(pDot->GetOutputNode(0));

	VS2DTexSampler* p2DTexSamplerNode = VS_NEW VS2DTexSampler(_T("DiffuseTexture"), this);
	p2DTexSamplerNode->SetTexture(pDiffuseTexture);
	//p2DTexSamplerNode->SetVESRGB(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VS2DTexSampler *pNormalNode = VS_NEW VS2DTexSampler(_T("NormalTexture"), this);
	pNormalNode->SetTexture(pNormalTexture);
	pNormalNode->SetVEDecode(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);


	VS2DTexSampler *pEmissiveNode = VS_NEW VS2DTexSampler(_T("EmissiveTexture"), this);
	pEmissiveNode->SetTexture(pEmissiveTexture);

	VSMul* pMul = VS_NEW VSMul(_T("Mul"), this);
	pMul->GetInputNode(0)->Connection(pSaturate->GetOutputNode(0));
	pMul->GetInputNode(1)->Connection(p2DTexSamplerNode->GetOutputNode(0));

	m_pShaderMainFunction[0]->GetInputNode(_T("Normal"))->Connection(pNormalNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("Custom"))->Connection(pMul->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("EmissiveColor"))->Connection(pEmissiveNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("Diffuse"))->Connection(p2DTexSamplerNode->GetOutputNode(0));
	//m_pShaderMainFunction[0]->SetSRGBWrite(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VSConstFloatValue * pFinalAlpah = VS_NEW VSConstFloatValue(_T("FinalAlpha"), this, 1, false);
	pFinalAlpah->SetValue(0, 0.6f);

	m_pShaderMainFunction[0]->GetInputNode(_T("Alpha"))->Connection(pFinalAlpah->GetOutputNode(0));

	m_ResourceName = _T("_MaterialCustom");

	//m_pShaderMainFunction[0]->SetAlphaTestValue(0.9f);

}
VSMaterialCustom::~VSMaterialCustom()
{

}

VSLightMaterial::VSLightMaterial()
{

}
VSLightMaterial::VSLightMaterial(const VSUsedName &ShowName, VSTexAllStateR * pDiffuseTexture)
:VSMaterial(ShowName, MUT_LIGHT)
{




	VS2DTexSampler* p2DTexSamplerNode = VS_NEW VS2DTexSampler(_T("DiffuseTexture"), this);
	p2DTexSamplerNode->SetTexture(pDiffuseTexture);
	//p2DTexSamplerNode->SetVESRGB(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);



	m_pLightShaderFunction->GetInputNode(_T("DiffuseColor"))->Connection(p2DTexSamplerNode->GetOutputNode(0));


	VSConstFloatValue * pFinalAlpah = VS_NEW VSConstFloatValue(_T("FinalAlpha"), this, 1, false);
	pFinalAlpah->SetValue(0, 0.6f);

	m_pLightShaderFunction->GetInputNode(_T("Alpha"))->Connection(p2DTexSamplerNode->GetOutputNode(4));

	m_ResourceName = _T("_LightMaterial");

	//m_pShaderMainFunction[0]->SetAlphaTestValue(0.9f);

}
VSLightMaterial::~VSLightMaterial()
{

}

VSPostEffectMaterial::VSPostEffectMaterial()
{

}
VSPostEffectMaterial::VSPostEffectMaterial(const VSUsedName &ShowName)
:VSMaterial(ShowName, MUT_POSTEFFECT)
{




	VSColorBuffer* pColorBuffer = VS_NEW VSColorBuffer(_T("ColorBuffer"), this);
	VSConstFloatValue * pMulColor = VS_NEW VSConstFloatValue(_T("Color"), this, 4, false);
	pMulColor->SetValue(0, 1.0f);
	pMulColor->SetValue(1, 0.0f);
	pMulColor->SetValue(2, 0.0f);
	pMulColor->SetValue(3, 1.0f);

	VSMul* pMul = VS_NEW VSMul(_T("Mul"), this);
	pMul->GetInputNode(0)->Connection(pColorBuffer->GetOutputNode(0));
	pMul->GetInputNode(1)->Connection(pMulColor->GetOutputNode(0));

	m_pPostEffectShaderFunction->GetInputNode(VSPostEffectShaderFunction::IN_COLOR)->Connection(pMul->GetOutputNode(0));
	m_ResourceName = _T("_PostEffectMaterial");

}
VSPostEffectMaterial::~VSPostEffectMaterial()
{

}


VSMaterialPhoneWolrdOffset::VSMaterialPhoneWolrdOffset()
{

}
VSMaterialPhoneWolrdOffset::VSMaterialPhoneWolrdOffset(const VSUsedName &ShowName, VSTexAllStateR * pDiffuseTexture, VSTexAllStateR * pNormalTexture,
	VSTexAllStateR * pSpecularTexture, VSTexAllStateR * pEmissiveTexture, bool bPhoneSpecular) :VSMaterial(ShowName, MUT_PHONE)
{
	VS2DTexSampler* p2DTexSamplerNode = VS_NEW VS2DTexSampler(_T("DiffuseTexture"), this);
	p2DTexSamplerNode->SetTexture(pDiffuseTexture);
	//p2DTexSamplerNode->SetVESRGB(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VS2DTexSampler *pNormalNode = VS_NEW VS2DTexSampler(_T("NormalTexture"), this);
	pNormalNode->SetTexture(pNormalTexture);
	pNormalNode->SetVEDecode(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VS2DTexSampler *pSpecularNode = VS_NEW VS2DTexSampler(_T("SpecularTexture"), this);
	pSpecularNode->SetTexture(pSpecularTexture);

	VS2DTexSampler *pEmissiveNode = VS_NEW VS2DTexSampler(_T("EmissiveTexture"), this);
	pEmissiveNode->SetTexture(pEmissiveTexture);

	VSConstFloatValue * pSpecularPow = VS_NEW VSConstFloatValue(_T("SpecularPow"), this, 1, true);
	pSpecularPow->SetValue(0, 50.0f);

	VSWorldNormal * pWorldNormal = VS_NEW VSWorldNormal(_T("WorldNormal"), this);
	pWorldNormal->m_uiNormalType = VSWorldNormal::WNT_VERTEX;
	VSConstFloatValue * pExtendLength = VS_NEW VSConstFloatValue(_T("ExtendLength"), this, 1, true);
	pExtendLength->SetValue(0, 10.0f);

	VSMul * pWorldNormal_Mul_ExtendLength = VS_NEW VSMul(_T("WorldNormal_Mul_ExtendLength"), this);
	pWorldNormal_Mul_ExtendLength->GetInputNode(VSInputNode::INI_A)->Connection(pWorldNormal->GetOutputNode(VSOutputNode::ONI_VALUE));
	pWorldNormal_Mul_ExtendLength->GetInputNode(VSInputNode::INI_B)->Connection(pExtendLength->GetOutputNode(VSOutputNode::ONI_VALUE));

	m_pShaderMainFunction[0]->GetInputNode(_T("DiffuseColor"))->Connection(p2DTexSamplerNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("Normal"))->Connection(pNormalNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularColor"))->Connection(pSpecularNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularPow"))->Connection(pSpecularPow->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("EmissiveColor"))->Connection(pEmissiveNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("WorldOffset"))->Connection(pWorldNormal_Mul_ExtendLength->GetOutputNode(0));
	//m_pShaderMainFunction[0]->SetSRGBWrite(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VSConstFloatValue * pFinalAlpah = VS_NEW VSConstFloatValue(_T("FinalAlpha"), this, 1, false);
	pFinalAlpah->SetValue(0, 0.6f);

	m_pShaderMainFunction[0]->GetInputNode(_T("Alpha"))->Connection(pFinalAlpah->GetOutputNode(0));


	if (bPhoneSpecular)
	{
		((VSPhoneShaderFunction *)m_pShaderMainFunction[0])->SetSpecularType(VSPhoneShaderFunction::ST_Phong);
	}
	m_ResourceName = _T("_MaterialPhoneWolrdOffset");

}
VSMaterialPhoneWolrdOffset::~VSMaterialPhoneWolrdOffset()
{

}
VSMaterialTessellation::VSMaterialTessellation()
{

}
VSMaterialTessellation::VSMaterialTessellation(const VSUsedName &ShowName, 
	VSTexAllStateR * pDiffuseTexture, VSTexAllStateR * pNormalTexture,VSTexAllStateR * pWorldDisplacementTexture)
	:VSMaterial(ShowName)
{
	VS2DTexSampler* p2DTexSamplerNode = VS_NEW VS2DTexSampler(_T("DiffuseTexture"), this);
	p2DTexSamplerNode->SetTexture(pDiffuseTexture);

	VS2DTexSampler *pNormalNode = VS_NEW VS2DTexSampler(_T("NormalTexture"), this);
	pNormalNode->SetTexture(pNormalTexture);
	pNormalNode->SetVEDecode(VSRenderer::VE_R | VSRenderer::VE_G | VSRenderer::VE_B);

	VS2DTexSampler * pWorldDisplacementNode = VS_NEW VS2DTexSampler(_T("WorldDisplacementTexture"), this);
	pWorldDisplacementNode->SetTexture(pWorldDisplacementTexture);

	VSConstFloatValue * pSpecularColor = VS_NEW VSConstFloatValue(_T("SpecluarColor"), this, 4, false);
	pSpecularColor->SetValue(0, 0.5f);
	pSpecularColor->SetValue(1, 0.5f);
	pSpecularColor->SetValue(2, 0.5f);
	pSpecularColor->SetValue(3, 1.0f);
	VSConstFloatValue * pSpecularPow = VS_NEW VSConstFloatValue(_T("SpecularPow"), this, 1, false);
	pSpecularPow->SetValue(0, 100.0f);

	VSConstFloatValue * pDetaDistance = VS_NEW VSConstFloatValue(_T("MaxDistance"), this, 1, false);
	pDetaDistance->SetValue(0, -2000.0f);

	VSConstFloatValue * pMinDistance = VS_NEW VSConstFloatValue(_T("MinDistance"), this, 1, false);
	pMinDistance->SetValue(0, 2500.0f);

	VSCameraWorldPos * pCameraWorldPos = VS_NEW VSCameraWorldPos(_T("CameraWorldPos"), this);
	VSWorldPos * pWorldPos = VS_NEW VSWorldPos(_T("WorldPos"), this);
	VSSub * pSub = VS_NEW VSSub(_T("Sub"), this);
	pSub->GetInputNode(VSInputNode::INI_A)->Connection(pCameraWorldPos->GetOutputNode(VSOutputNode::ONI_VALUE));
	pSub->GetInputNode(VSInputNode::INI_B)->Connection(pWorldPos->GetOutputNode(VSOutputNode::ONI_VALUE));

	VSLength * pLength = VS_NEW VSLength(_T("Length"), this);
	pLength->GetInputNode(VSInputNode::INI_VALUE)->Connection(pSub->GetOutputNode(VSOutputNode::ONI_VALUE));

	VSSub * pSubDeta = VS_NEW VSSub(_T("SubDeta"), this);
	pSubDeta->GetInputNode(VSInputNode::INI_A)->Connection(pLength->GetOutputNode(VSOutputNode::ONI_VALUE));
	pSubDeta->GetInputNode(VSInputNode::INI_B)->Connection(pMinDistance->GetOutputNode(VSOutputNode::ONI_VALUE));

	VSDiv * pDiv = VS_NEW VSDiv(_T("Div"), this);
	pDiv->GetInputNode(VSInputNode::INI_A)->Connection(pSubDeta->GetOutputNode(VSOutputNode::ONI_VALUE));
	pDiv->GetInputNode(VSInputNode::INI_B)->Connection(pDetaDistance->GetOutputNode(VSOutputNode::ONI_VALUE));

	VSConstFloatValue * pDisplacementScalar = VS_NEW VSConstFloatValue(_T("DisplacementScalar"), this, 1, false);
	pDisplacementScalar->SetValue(0, 100.0f);

	VSMul * pMulDisplacementScalar = VS_NEW VSMul(_T("MulDisplacementScalar"), this);
	pMulDisplacementScalar->GetInputNode(VSInputNode::INI_A)->Connection(pDisplacementScalar->GetOutputNode(VSOutputNode::ONI_VALUE));
	pMulDisplacementScalar->GetInputNode(VSInputNode::INI_B)->Connection(pWorldDisplacementNode->GetOutputNode(VSOutputNode::ONI_A));

	VSWorldNormal * pWorldNormal = VS_NEW VSWorldNormal(_T("WorldNormal"), this);
	pWorldNormal->m_uiNormalType = VSWorldNormal::WNT_VERTEX;

	VSMul * pWorldNormal_Mul_ExtendLength = VS_NEW VSMul(_T("WorldNormal_Mul_ExtendLength"), this);
	pWorldNormal_Mul_ExtendLength->GetInputNode(VSInputNode::INI_A)->Connection(pWorldNormal->GetOutputNode(VSOutputNode::ONI_VALUE));
	pWorldNormal_Mul_ExtendLength->GetInputNode(VSInputNode::INI_B)->Connection(pMulDisplacementScalar->GetOutputNode(VSOutputNode::ONI_VALUE));

	m_pShaderMainFunction[0]->GetInputNode(_T("DiffuseColor"))->Connection(p2DTexSamplerNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("Normal"))->Connection(pNormalNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularColor"))->Connection(pSpecularColor->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularPow"))->Connection(pSpecularPow->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("TessellationValue"))->Connection(pDiv->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("WorldDisplacement"))->Connection(pWorldNormal_Mul_ExtendLength->GetOutputNode(0));

	m_ResourceName = _T("MaterialTessellation");
}
VSMaterialTessellation::~VSMaterialTessellation()
{

}