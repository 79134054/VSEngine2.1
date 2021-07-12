#include "VSPreZPass.h"
#include "VSSpatial.h"
#include "VSBoneNode.h"
#include "VSShaderStringFactory.h"
#include "VSConstValue.h"
#include "VSGeometry.h"
#include "VSBoneNode.h"
#include "VSResourceManager.h"
#include "VSGraphicInclude.h"
#include "VSMaterial.h"
#include "VSProfiler.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSPreZPass,VSPass)
VSPointer<VSPreZPass> VSPreZPass::ms_Default;
BEGIN_ADD_PROPERTY(VSPreZPass,VSPass)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSPreZPass)
ADD_PRIORITY(VSBlendState)
ADD_PRIORITY(VSDepthStencilState)
ADD_PRIORITY(VSRasterizerState)
ADD_INITIAL_FUNCTION(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
DECLEAR_TIME_PROFILENODE(PreZRenderPassDraw, )
bool VSPreZPass::InitialDefaultState()
{
	ms_Default = VS_NEW VSPreZPass();
	if(!ms_Default)
		return 0;
	return 1;
}
bool VSPreZPass::TerminalDefaultState()
{
	ms_Default = NULL;
	return 1;
}
VSPreZPass::VSPreZPass()
{


	VSBlendDesc BlendDesc;
	BlendDesc.ucWriteMask[0] = VSBlendDesc::WM_NONE;
	VSBlendState * pBlendState = VSResourceManager::CreateBlendState(BlendDesc);
	m_RenderState.SetBlendState(pBlendState);

}
VSPreZPass::~VSPreZPass()
{


}


bool VSPreZPass::Draw(VSRenderer * pRenderer)
{
	ADD_TIME_PROFILE(PreZRenderPassDraw)



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
	if (!GetVShader(VSResourceManager::GetPreZVertexShaderMap(), pMaterial->GetResourceName()))
	{
		return 0;
	}
	if (!GetHShader(VSResourceManager::GetInnerHullShaderMap(), VSUsedName::ms_cHullShader))
	{
		return 0;
	}
	if (!GetDShader( VSResourceManager::GetPreZDomainShaderMap(), pMaterial->GetResourceName()))
	{
		return 0;
	}
	if (!GetPShader(VSResourceManager::GetInnerPixelShaderMap(),VSUsedName::ms_cPreZShader))
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

