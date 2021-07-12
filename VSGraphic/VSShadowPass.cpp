#include "VSShadowPass.h"
#include "VSSpatial.h"
#include "VSBoneNode.h"
#include "VSShaderStringFactory.h"
#include "VSConstValue.h"
#include "VSGeometry.h"
#include "VSResourceManager.h"
#include "VSGraphicInclude.h"
#include "VSMaterial.h"
#include "VSProfiler.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSCubeShadowPass,VSPass)
BEGIN_ADD_PROPERTY(VSCubeShadowPass,VSPass)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSCubeShadowPass)
ADD_PRIORITY(VSBlendState)
ADD_PRIORITY(VSDepthStencilState)
ADD_PRIORITY(VSRasterizerState)
ADD_INITIAL_FUNCTION(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
DECLEAR_TIME_PROFILENODE(CubeShadowPassDraw, )
bool VSCubeShadowPass::InitialDefaultState()
{

	return 1;
}
bool VSCubeShadowPass::TerminalDefaultState()
{

	return 1;
}
VSCubeShadowPass::VSCubeShadowPass()
{



}
VSCubeShadowPass::~VSCubeShadowPass()
{


}


bool VSCubeShadowPass::Draw(VSRenderer * pRenderer)
{
	ADD_TIME_PROFILE(CubeShadowPassDraw)


	VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();


	if (pMaterial && pMaterial->GetShaderMainFunction(MSPara.uiPassId)->HasTessellation())
	{
		MSPara.bUseTessellation = true;
	}
	else
	{
		MSPara.bUseTessellation = false;
	}
	m_VShaderkey.Clear();
	m_PShaderkey.Clear();
	m_GShaderkey.Clear();
	m_HShaderkey.Clear();
	m_DShaderkey.Clear();
	if (!GetVShader(VSResourceManager::GetCubShadowVertexShaderMap(), pMaterial->GetResourceName()))
	{
		return 0;
	}
	if (!GetHShader(VSResourceManager::GetInnerHullShaderMap(), VSUsedName::ms_cHullShader))
	{
		return 0;
	}
	if (!GetDShader(VSResourceManager::GetShadowDomainShaderMap(), pMaterial->GetResourceName()))
	{
		return 0;
	}
	if (!GetPShader(VSResourceManager::GetCubShadowPixelShaderMap(),pMaterial->GetResourceName()))
	{
		return 0;
	}
	
	pRenderer->SetMaterialVShaderConstant(MSPara,MSPara.pMaterialInstance->m_pCurVShader[GetPassType()]);
	if (MSPara.bUseTessellation)
	{
		pRenderer->SetMaterialHShaderConstant(MSPara, MSPara.pMaterialInstance->m_pCurHShader[GetPassType()]);
		pRenderer->SetMaterialDShaderConstant(MSPara, MSPara.pMaterialInstance->m_pCurDShader[GetPassType()]);
	}
	pRenderer->SetMaterialPShaderConstant(MSPara,MSPara.pMaterialInstance->m_pCurPShader[GetPassType()]);

	if(!pRenderer->DrawMesh(MSPara.pGeometry,&m_RenderState,
		MSPara.pMaterialInstance->m_pCurVShader[GetPassType()],
		MSPara.pMaterialInstance->m_pCurPShader[GetPassType()],
		MSPara.pMaterialInstance->m_pCurGShader[GetPassType()],
		MSPara.pMaterialInstance->m_pCurHShader[GetPassType()],
		MSPara.pMaterialInstance->m_pCurDShader[GetPassType()]))
	{
		return false;
	}
	return 1;
}
IMPLEMENT_RTTI(VSVolumeShadowPass,VSPass)
BEGIN_ADD_PROPERTY(VSVolumeShadowPass,VSPass)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSVolumeShadowPass)
ADD_PRIORITY(VSBlendState)
ADD_PRIORITY(VSDepthStencilState)
ADD_PRIORITY(VSRasterizerState)
ADD_INITIAL_FUNCTION(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
DECLEAR_TIME_PROFILENODE(VolumeShadowRenderPassDraw, )
bool VSVolumeShadowPass::InitialDefaultState()
{

	return 1;
}
bool VSVolumeShadowPass::TerminalDefaultState()
{

	return 1;
}
VSVolumeShadowPass::VSVolumeShadowPass()
{



	
	VSDepthStencilDesc DepthStencilDesc;
	DepthStencilDesc.m_bDepthWritable = false;
	DepthStencilDesc.m_uiDepthCompareMethod = VSDepthStencilDesc::CM_LESS;
	DepthStencilDesc.m_bStencilEnable = true;
	DepthStencilDesc.m_bTwoSideStencilMode = true;
	DepthStencilDesc.m_uiSPassZFailOP = VSDepthStencilDesc::OT_DECREMENT;
	DepthStencilDesc.m_uiCCW_SPassZFailOP = VSDepthStencilDesc::OT_INCREMENT;
	VSDepthStencilState * pDepthStencilState = VSResourceManager::CreateDepthStencilState(DepthStencilDesc);
	m_RenderState.SetDepthStencilState(pDepthStencilState);

	VSRasterizerDesc RasterizerDesc;
	RasterizerDesc.m_uiCullType = VSRasterizerDesc::CT_NONE;
	VSRasterizerState * pRasterizerState= VSResourceManager::CreateRasterizerState(RasterizerDesc);
	m_RenderState.SetRasterizerState(pRasterizerState);

	VSBlendDesc BlendDesc;
	BlendDesc.ucWriteMask[0] = VSBlendDesc::WM_NONE;
	VSBlendState * pBlendState = VSResourceManager::CreateBlendState(BlendDesc);
	m_RenderState.SetBlendState(pBlendState);

	MSPara.GSShaderPath = _T("ShadowVolume.txt");
	MSPara.GMainFunName = _T("GSMain");

}
VSVolumeShadowPass::~VSVolumeShadowPass()
{



}
void VSVolumeShadowPass::SetCustomConstant(MaterialShaderPara &MSPara, VSGShader * pGShader)
{
	if (!pGShader)
	{
		return;
	}
	static VSUsedName cViewProjectMatrix = _T("ViewProjectMatrix");
	static VSUsedName cLightInfo = _T("LightInfo");
	VSMatrix3X3W ViewProjectMatrix =  MSPara.pCamera->GetViewMatrix() * MSPara.pCamera->GetProjMatrix();
	pGShader->SetParam(cViewProjectMatrix, &ViewProjectMatrix);
	VSVector3 LightInfo;
	if (MSPara.pShadowLight->GetLightType() == VSLight::LT_POINT)
	{
		LightInfo = MSPara.pShadowLight->GetWorldTranslate();
	}
	else
	{
		const VSMatrix3X3 &Rotator = MSPara.pShadowLight->GetWorldRotate();
		VSVector3 U, V, N;
		Rotator.GetUVN(U, V, N);

		LightInfo = N;
	}
	pGShader->SetParam(cLightInfo, &LightInfo);
}
bool VSVolumeShadowPass::Draw(VSRenderer * pRenderer)
{
	ADD_TIME_PROFILE(VolumeShadowRenderPassDraw)


	VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();



	m_VShaderkey.Clear();
	m_PShaderkey.Clear();
	m_GShaderkey.Clear();
	m_HShaderkey.Clear();
	m_DShaderkey.Clear();
	if (!GetVShader(VSResourceManager::GetVolumeShadowVertexShaderMap(), pMaterial->GetResourceName()))
	{
		return 0;
	}
	if (!GetGShader( VSResourceManager::GetInnerGeometryShaderMap(), VSUsedName::ms_cVolumeShadowShader))
	{
		return false;
	}
	if (!GetPShader(VSResourceManager::GetVolumeShadowPixelShaderMap(), pMaterial->GetResourceName()))
	{
		return 0;
	}

	pRenderer->SetMaterialVShaderConstant(MSPara,MSPara.pMaterialInstance->m_pCurVShader[GetPassType()]);
	if (VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_GS))
	{
		pRenderer->SetMaterialGShaderConstant(MSPara, MSPara.pMaterialInstance->m_pCurGShader[GetPassType()]);
	}
	pRenderer->SetMaterialPShaderConstant(MSPara,MSPara.pMaterialInstance->m_pCurPShader[GetPassType()]);
	SetCustomConstant(MSPara,MSPara.pMaterialInstance->m_pCurGShader[GetPassType()]);
	if(!pRenderer->DrawMesh(MSPara.pGeometry,&m_RenderState,
		MSPara.pMaterialInstance->m_pCurVShader[GetPassType()],
		MSPara.pMaterialInstance->m_pCurPShader[GetPassType()],
		MSPara.pMaterialInstance->m_pCurGShader[GetPassType()],
		MSPara.pMaterialInstance->m_pCurHShader[GetPassType()],
		MSPara.pMaterialInstance->m_pCurDShader[GetPassType()]))
	{
		return false;
	}
	return 1;
}




IMPLEMENT_RTTI(VSShadowPass,VSPass)
BEGIN_ADD_PROPERTY(VSShadowPass,VSPass)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSShadowPass)
ADD_PRIORITY(VSBlendState)
ADD_PRIORITY(VSDepthStencilState)
ADD_PRIORITY(VSRasterizerState)
ADD_INITIAL_FUNCTION(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
DECLEAR_TIME_PROFILENODE(ShadowRenderPassDraw, )
bool VSShadowPass::InitialDefaultState()
{

	return 1;
}
bool VSShadowPass::TerminalDefaultState()
{

	return 1;
}
VSShadowPass::VSShadowPass()
{




}
VSShadowPass::~VSShadowPass()
{




}


bool VSShadowPass::Draw(VSRenderer * pRenderer)
{
	ADD_TIME_PROFILE(ShadowRenderPassDraw)


	VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();

	if (pMaterial && pMaterial->GetShaderMainFunction(MSPara.uiPassId)->HasTessellation())
	{
		MSPara.bUseTessellation = true;
	}
	else
	{
		MSPara.bUseTessellation = false;
	}
	m_VShaderkey.Clear();
	m_PShaderkey.Clear();
	m_GShaderkey.Clear();
	m_HShaderkey.Clear();
	m_DShaderkey.Clear();
	if (!GetVShader(VSResourceManager::GetShadowVertexShaderMap(), pMaterial->GetResourceName()))
	{
		return 0;
	}
	if (!GetHShader(VSResourceManager::GetInnerHullShaderMap(), VSUsedName::ms_cHullShader))
	{
		return 0;
	}
	if (!GetDShader(VSResourceManager::GetShadowDomainShaderMap(), pMaterial->GetResourceName()))
	{
		return 0;
	}
	if (!GetPShader(VSResourceManager::GetShadowPixelShaderMap(),pMaterial->GetResourceName()))
	{
		return 0;
	}

	pRenderer->SetMaterialVShaderConstant(MSPara,MSPara.pMaterialInstance->m_pCurVShader[GetPassType()]);
	if (MSPara.bUseTessellation)
	{
		pRenderer->SetMaterialHShaderConstant(MSPara, MSPara.pMaterialInstance->m_pCurHShader[GetPassType()]);
		pRenderer->SetMaterialDShaderConstant(MSPara, MSPara.pMaterialInstance->m_pCurDShader[GetPassType()]);
	}
	pRenderer->SetMaterialPShaderConstant(MSPara,MSPara.pMaterialInstance->m_pCurPShader[GetPassType()]);

	if(!pRenderer->DrawMesh(MSPara.pGeometry,&m_RenderState,
		MSPara.pMaterialInstance->m_pCurVShader[GetPassType()],
		MSPara.pMaterialInstance->m_pCurPShader[GetPassType()],
		MSPara.pMaterialInstance->m_pCurGShader[GetPassType()],
		MSPara.pMaterialInstance->m_pCurHShader[GetPassType()],
		MSPara.pMaterialInstance->m_pCurDShader[GetPassType()]))
	{
		return false;
	}
	return 1;
}



IMPLEMENT_RTTI(VSDualParaboloidShadowPass,VSPass)
BEGIN_ADD_PROPERTY(VSDualParaboloidShadowPass,VSPass)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSDualParaboloidShadowPass)
ADD_PRIORITY(VSBlendState)
ADD_PRIORITY(VSDepthStencilState)
ADD_PRIORITY(VSRasterizerState)
ADD_INITIAL_FUNCTION(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
DECLEAR_TIME_PROFILENODE(DualParaboloidRenderPassDraw, )
bool VSDualParaboloidShadowPass::InitialDefaultState()
{

	return 1;
}
bool VSDualParaboloidShadowPass::TerminalDefaultState()
{

	return 1;
}
VSDualParaboloidShadowPass::VSDualParaboloidShadowPass()
{



}
VSDualParaboloidShadowPass::~VSDualParaboloidShadowPass()
{



}


bool VSDualParaboloidShadowPass::Draw(VSRenderer * pRenderer)
{
	ADD_TIME_PROFILE(DualParaboloidRenderPassDraw)


	VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();


	m_VShaderkey.Clear();
	m_PShaderkey.Clear();
	m_GShaderkey.Clear();
	m_HShaderkey.Clear();
	m_DShaderkey.Clear();
	if (pMaterial && pMaterial->GetShaderMainFunction(MSPara.uiPassId)->HasTessellation())
	{
		MSPara.bUseTessellation = true;
	}
	else
	{
		MSPara.bUseTessellation = false;
	}
	if (!GetVShader(VSResourceManager::GetDualParaboloidShadowVertexShaderMap(), pMaterial->GetResourceName()))
	{
		return 0;
	}
	if (!GetHShader(VSResourceManager::GetInnerHullShaderMap(), VSUsedName::ms_cHullShader))
	{
		return 0;
	}
	if (!GetDShader(VSResourceManager::GetShadowDomainShaderMap(), pMaterial->GetResourceName()))
	{
		return 0;
	}
	if (!GetPShader(VSResourceManager::GetDualParaboloidShadowPixelShaderMap(),pMaterial->GetResourceName()))
	{
		return 0;
	}

	pRenderer->SetMaterialVShaderConstant(MSPara,MSPara.pMaterialInstance->m_pCurVShader[GetPassType()]);
	if (MSPara.bUseTessellation)
	{
		pRenderer->SetMaterialHShaderConstant(MSPara, MSPara.pMaterialInstance->m_pCurHShader[GetPassType()]);
		pRenderer->SetMaterialDShaderConstant(MSPara, MSPara.pMaterialInstance->m_pCurDShader[GetPassType()]);
	}
	pRenderer->SetMaterialPShaderConstant(MSPara,MSPara.pMaterialInstance->m_pCurPShader[GetPassType()]);

	if(!pRenderer->DrawMesh(MSPara.pGeometry,&m_RenderState,
		MSPara.pMaterialInstance->m_pCurVShader[GetPassType()],
		MSPara.pMaterialInstance->m_pCurPShader[GetPassType()],
		MSPara.pMaterialInstance->m_pCurGShader[GetPassType()],
		MSPara.pMaterialInstance->m_pCurHShader[GetPassType()],
		MSPara.pMaterialInstance->m_pCurDShader[GetPassType()]))
	{
		return false;
	}
	return 1;
}
