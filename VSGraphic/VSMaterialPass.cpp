#include "VSMaterialPass.h"
#include "VSSpatial.h"
#include "VSBoneNode.h"
#include "VSShaderStringFactory.h"
#include "VSConstValue.h"
#include "VSGeometry.h"
#include "VSBoneNode.h"
#include "VSResourceManager.h"
#include "VSGraphicInclude.h"
#include "VSMaterial.h"
#include "VSSortLight.h"
#include "VSProfiler.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSMaterialPass,VSPass)
BEGIN_ADD_PROPERTY(VSMaterialPass,VSPass)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSMaterialPass)
IMPLEMENT_INITIAL_END
DECLEAR_TIME_PROFILENODE(MaterialRenderPassDraw, )
VSMaterialPass::VSMaterialPass()
{

	

}
VSMaterialPass::~VSMaterialPass()
{


}
bool VSMaterialPass::Draw(VSRenderer * pRenderer)
{
	ADD_TIME_PROFILE(MaterialRenderPassDraw)


	VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();



	MSPara.LightArray = m_Light;

	if (pMaterial && pMaterial->GetShaderMainFunction(MSPara.uiPassId)->HasTessellation())
	{
		MSPara.bUseTessellation = true;
	}
	else
	{
		MSPara.bUseTessellation = false;
	}

	VSArray<VSLight*> LightTemp;

	
	m_RenderSceondPassUsed = pMaterial->GetRenderState(MSPara.uiPassId);

	VSBlendDesc BlendDesc = m_RenderSceondPassUsed.GetBlendState()->GetBlendDesc();
	if (BlendDesc.bBlendEnable[0] == true)
	{
		BlendDesc.ucDestBlend[0] = VSBlendDesc::BP_ONE;
	}
	else
	{
		BlendDesc.ucSrcBlend[0] = VSBlendDesc::BP_ONE;
		BlendDesc.ucDestBlend[0] = VSBlendDesc::BP_ONE;
	}
	BlendDesc.bBlendEnable[0] = true;		
	BlendDesc.bAlphaBlendEnable[0] = false;

	VSBlendState * pBlendState = VSResourceManager::CreateBlendState(BlendDesc);
	m_RenderSceondPassUsed.SetBlendState(pBlendState);




	for (unsigned int i = 0 ; i < m_Light.GetNum() ; i++)
	{
		LightTemp.AddElement(m_Light[i]);
		if (LightTemp.GetNum() >= MAX_DYNAMIC_LIGHT || i == m_Light.GetNum() - 1)
		{
			MSPara.LightArray = LightTemp;

			m_VShaderkey.Clear();
			m_PShaderkey.Clear();
			m_GShaderkey.Clear();
			m_HShaderkey.Clear();
			m_DShaderkey.Clear();
			if (!GetVShader(VSResourceManager::GetMaterialVertexShaderMap(), pMaterial->GetResourceName()))
			{
				m_Light.Clear();
				return 0;
			}
			if (!GetHShader(VSResourceManager::GetInnerHullShaderMap(), VSUsedName::ms_cHullShader))
			{
				m_Light.Clear();
				return 0;
			}
			if (!GetDShader(VSResourceManager::GetMaterialDomainShaderMap(), pMaterial->GetResourceName()))
			{
				m_Light.Clear();
				return 0;
			}
			if (!GetPShader(VSResourceManager::GetMaterialPixelShaderMap(),pMaterial->GetResourceName()))
			{
				m_Light.Clear();
				return 0;
			}


			pRenderer->SetMaterialVShaderConstant(MSPara,MSPara.pMaterialInstance->m_pCurVShader[GetPassType()]);
			if (MSPara.bUseTessellation)
			{
				pRenderer->SetMaterialHShaderConstant(MSPara, MSPara.pMaterialInstance->m_pCurHShader[GetPassType()]);
				pRenderer->SetMaterialDShaderConstant(MSPara, MSPara.pMaterialInstance->m_pCurDShader[GetPassType()]);
			}			
			pRenderer->SetMaterialPShaderConstant(MSPara,MSPara.pMaterialInstance->m_pCurPShader[GetPassType()]);


			pMaterial->SetGlobleValue(MSPara,
				MSPara.pMaterialInstance->m_pCurVShader[GetPassType()],
				MSPara.pMaterialInstance->m_pCurPShader[GetPassType()],
				MSPara.pMaterialInstance->m_pCurGShader[GetPassType()],
				MSPara.pMaterialInstance->m_pCurHShader[GetPassType()],
				MSPara.pMaterialInstance->m_pCurDShader[GetPassType()]);


			if(!pRenderer->DrawMesh(MSPara.pGeometry,&m_RenderSceondPassUsed,
				MSPara.pMaterialInstance->m_pCurVShader[GetPassType()],
				MSPara.pMaterialInstance->m_pCurPShader[GetPassType()],
				MSPara.pMaterialInstance->m_pCurGShader[GetPassType()],
				MSPara.pMaterialInstance->m_pCurHShader[GetPassType()],
				MSPara.pMaterialInstance->m_pCurDShader[GetPassType()]))
			{
				return false;
			}



			LightTemp.Clear();
		}

	}
	

	
	m_Light.Clear();
	return 1;
}

