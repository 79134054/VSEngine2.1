#include "VSPass.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
IMPLEMENT_RTTI_NoCreateFun(VSPass,VSObject)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSPass)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(VSPass,VSObject)
END_ADD_PROPERTY
VSPass::~VSPass()
{
	m_pVShaderSet = NULL;
	m_pPShaderSet = NULL;
	m_pGShaderSet = NULL;
	m_pHShaderSet = NULL;
	m_pDShaderSet = NULL;
}
VSPass::VSPass()
{
	m_pVShaderSet = NULL;
	m_pPShaderSet = NULL;
	m_pGShaderSet = NULL;
	m_pHShaderSet = NULL;
	m_pDShaderSet = NULL;
}
bool VSPass::GetHShader(VSShaderMap & ShaderMap, const VSUsedName &Name)
{
	VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
	if (!(pMaterial && pMaterial->GetShaderMainFunction(MSPara.uiPassId)->HasTessellation()))
	{
		MSPara.pMaterialInstance->m_pCurHShader[GetPassType()] = NULL;
		return 1;
	}
	VSHShader * pHullShader = NULL;

	unsigned int uiHShaderNum = 0;


	m_pHShaderSet = ShaderMap.GetShaderSet(Name);

	

	if (m_pHShaderSet)
	{

		uiHShaderNum = m_pHShaderSet->GetNum();
	}

	VSShaderKey::SetMaterialHShaderKey(&m_HShaderkey, MSPara);
	if (MSPara.pMaterialInstance->m_pCurHShader[GetPassType()] == NULL)
	{
		if (m_pHShaderSet)
		{
			unsigned int uiIndex = m_pHShaderSet->Find(m_HShaderkey);

			if (uiIndex != m_pHShaderSet->GetNum())
			{
				VSShader * pTemp = (*m_pHShaderSet)[uiIndex].Value;
				pHullShader = (VSHShader *)(pTemp);
			}

		}
		if (pHullShader == NULL)
		{
			pHullShader = VSResourceManager::CreateHShader(MSPara, uiHShaderNum);
			if (!pHullShader)
			{
				return 0;
			}
			if (m_pHShaderSet)
			{
				m_pHShaderSet->AddElement(m_HShaderkey, pHullShader);
			}
		}
		MSPara.pMaterialInstance->m_pCurHShader[GetPassType()] = pHullShader;
	}
	else
	{
		if (MSPara.pMaterialInstance->m_pCurHShader[GetPassType()]->m_ShaderKey == m_HShaderkey)
		{

		}
		else
		{

			if (m_pHShaderSet)
			{
				unsigned int uiIndex = m_pHShaderSet->Find(m_HShaderkey);

				if (uiIndex != m_pHShaderSet->GetNum())
				{
					VSShader * pTemp = (*m_pHShaderSet)[uiIndex].Value;
					pHullShader = (VSHShader *)(pTemp);
				}
			}
			if (pHullShader == NULL)
			{
				pHullShader = VSResourceManager::CreateHShader(MSPara, uiHShaderNum);
				if (!pHullShader)
				{
					return 0;
				}
				if (m_pHShaderSet)
				{
					m_pHShaderSet->AddElement(m_HShaderkey, pHullShader);
				}
			}
			MSPara.pMaterialInstance->m_pCurHShader[GetPassType()] = pHullShader;
		}
	}

	if (!m_pHShaderSet)
	{
		ShaderMap.SetShader(Name, m_HShaderkey, pHullShader);
		m_pHShaderSet = ShaderMap.GetShaderSet(Name);
	}
	return 1;
}
bool VSPass::GetDShader(VSShaderMap & ShaderMap, const VSUsedName &Name)
{
	VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
	if (!(pMaterial && pMaterial->GetShaderMainFunction(MSPara.uiPassId)->HasTessellation()))
	{
		MSPara.pMaterialInstance->m_pCurDShader[GetPassType()] = NULL;
		return 1;
	}
	VSDShader * pDomainShader = NULL;

	unsigned int uiDShaderNum = 0;


	m_pDShaderSet = ShaderMap.GetShaderSet(Name);

	

	if (m_pDShaderSet)
	{

		uiDShaderNum = m_pDShaderSet->GetNum();
	}

	VSShaderKey::SetMaterialDShaderKey(&m_DShaderkey, MSPara);
	if (MSPara.pMaterialInstance->m_pCurDShader[GetPassType()] == NULL)
	{
		if (m_pDShaderSet)
		{
			unsigned int uiIndex = m_pDShaderSet->Find(m_DShaderkey);

			if (uiIndex != m_pDShaderSet->GetNum())
			{
				VSShader * pTemp = (*m_pDShaderSet)[uiIndex].Value;
				pDomainShader = (VSDShader *)(pTemp);
			}

		}
		if (pDomainShader == NULL)
		{
			pDomainShader = VSResourceManager::CreateDShader(MSPara, uiDShaderNum);
			if (!pDomainShader)
			{
				return 0;
			}
			if (m_pDShaderSet)
			{
				m_pDShaderSet->AddElement(m_DShaderkey, pDomainShader);
			}
		}
		MSPara.pMaterialInstance->m_pCurDShader[GetPassType()] = pDomainShader;
	}
	else
	{
		if (MSPara.pMaterialInstance->m_pCurDShader[GetPassType()]->m_ShaderKey == m_DShaderkey)
		{

		}
		else
		{

			if (m_pDShaderSet)
			{
				unsigned int uiIndex = m_pDShaderSet->Find(m_DShaderkey);

				if (uiIndex != m_pDShaderSet->GetNum())
				{
					VSShader * pTemp = (*m_pDShaderSet)[uiIndex].Value;
					pDomainShader = (VSDShader *)(pTemp);
				}
			}
			if (pDomainShader == NULL)
			{
				pDomainShader = VSResourceManager::CreateDShader(MSPara, uiDShaderNum);
				if (!pDomainShader)
				{
					return 0;
				}
				if (m_pDShaderSet)
				{
					m_pDShaderSet->AddElement(m_DShaderkey, pDomainShader);
				}
			}
			MSPara.pMaterialInstance->m_pCurDShader[GetPassType()] = pDomainShader;
		}
	}

	if (!m_pDShaderSet)
	{
		ShaderMap.SetShader(Name, m_DShaderkey, pDomainShader);
		m_pDShaderSet = ShaderMap.GetShaderSet(Name);
	}
	return 1;
}
bool VSPass::GetGShader(VSShaderMap & ShaderMap, const VSUsedName &Name)
{
	if (!VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_GS))
	{
		MSPara.pMaterialInstance->m_pCurGShader[GetPassType()] = NULL;
		return true;
	}
	VSGShader * pGeometryShader = NULL;

	unsigned int uiGShaderNum = 0;


	m_pGShaderSet = ShaderMap.GetShaderSet(Name);

	

	if (m_pGShaderSet)
	{

		uiGShaderNum = m_pGShaderSet->GetNum();
	}

	VSShaderKey::SetMaterialGShaderKey(&m_GShaderkey, MSPara);
	if (MSPara.pMaterialInstance->m_pCurGShader[GetPassType()] == NULL)
	{
		if (m_pGShaderSet)
		{
			unsigned int uiIndex = m_pGShaderSet->Find(m_GShaderkey);

			if (uiIndex != m_pGShaderSet->GetNum())
			{
				VSShader * pTemp = (*m_pGShaderSet)[uiIndex].Value;
				pGeometryShader = (VSGShader *)(pTemp);
			}

		}
		if (pGeometryShader == NULL)
		{
			pGeometryShader = VSResourceManager::CreateGShader(MSPara,uiGShaderNum);
			if (!pGeometryShader)
			{
				return 0;
			}
			if (m_pGShaderSet)
			{
				m_pGShaderSet->AddElement(m_GShaderkey, pGeometryShader);
			}
		}
		MSPara.pMaterialInstance->m_pCurGShader[GetPassType()] = pGeometryShader;
	}
	else
	{
		if (MSPara.pMaterialInstance->m_pCurGShader[GetPassType()]->m_ShaderKey == m_GShaderkey)
		{

		}
		else
		{

			if (m_pGShaderSet)
			{
				unsigned int uiIndex = m_pGShaderSet->Find(m_GShaderkey);

				if (uiIndex != m_pGShaderSet->GetNum())
				{
					VSShader * pTemp = (*m_pGShaderSet)[uiIndex].Value;
					pGeometryShader = (VSGShader *)(pTemp);
				}
			}
			if (pGeometryShader == NULL)
			{
				pGeometryShader = VSResourceManager::CreateGShader(MSPara,uiGShaderNum);
				if (!pGeometryShader)
				{
					return 0;
				}
				if (m_pGShaderSet)
				{
					m_pGShaderSet->AddElement(m_GShaderkey, pGeometryShader);
				}
			}
			MSPara.pMaterialInstance->m_pCurGShader[GetPassType()] = pGeometryShader;
		}
	}

	if (!m_pGShaderSet)
	{
		ShaderMap.SetShader(Name, m_GShaderkey, pGeometryShader);
		m_pGShaderSet = ShaderMap.GetShaderSet(Name);
	}
	return 1;
}
bool VSPass::GetVShader(VSShaderMap & ShaderMap,const VSUsedName &Name)
{

	VSVShader * pVertexShader = NULL;

	unsigned int uiVShaderNum = 0;


	m_pVShaderSet = ShaderMap.GetShaderSet(Name);

	
	
	if (m_pVShaderSet)
	{

		uiVShaderNum = m_pVShaderSet->GetNum();
	}

	VSShaderKey::SetMaterialVShaderKey(&m_VShaderkey,MSPara);
	if (MSPara.pMaterialInstance->m_pCurVShader[GetPassType()] == NULL)
	{
		if (m_pVShaderSet)
		{
			unsigned int uiIndex = m_pVShaderSet->Find(m_VShaderkey);

			if (uiIndex != m_pVShaderSet->GetNum())
			{
				VSShader * pTemp = (*m_pVShaderSet)[uiIndex].Value;
				pVertexShader = (VSVShader *)(pTemp);
			}

		}
		if (pVertexShader == NULL)
		{
			pVertexShader = VSResourceManager::CreateVShader(MSPara,uiVShaderNum);
			if (!pVertexShader)
			{
				return 0;
			}
			if (m_pVShaderSet)
			{
				m_pVShaderSet->AddElement(m_VShaderkey,pVertexShader);
			}
		}
		MSPara.pMaterialInstance->m_pCurVShader[GetPassType()] = pVertexShader;
	}
	else
	{
		if (MSPara.pMaterialInstance->m_pCurVShader[GetPassType()]->m_ShaderKey == m_VShaderkey)
		{

		}
		else
		{

			if (m_pVShaderSet)
			{
				unsigned int uiIndex = m_pVShaderSet->Find(m_VShaderkey);

				if (uiIndex != m_pVShaderSet->GetNum())
				{
					VSShader * pTemp = (*m_pVShaderSet)[uiIndex].Value;
					pVertexShader = (VSVShader *)(pTemp);
				}
			}
			if (pVertexShader == NULL)
			{
				pVertexShader = VSResourceManager::CreateVShader(MSPara,uiVShaderNum);
				if (!pVertexShader)
				{
					return 0;
				}
				if (m_pVShaderSet)
				{
					m_pVShaderSet->AddElement(m_VShaderkey,pVertexShader);
				}
			}
			MSPara.pMaterialInstance->m_pCurVShader[GetPassType()] = pVertexShader;
		}
	}

	if (!m_pVShaderSet)
	{
		ShaderMap.SetShader(Name,m_VShaderkey,pVertexShader);
		m_pVShaderSet = ShaderMap.GetShaderSet(Name);
	}
	return 1;
}
bool VSPass::GetPShader(VSShaderMap & ShaderMap,const VSUsedName &Name)
{

	unsigned int uiPShaderNum = 0;
	VSPShader * pPixelShader = NULL;


	m_pPShaderSet = ShaderMap.GetShaderSet(Name);

	
	
	if (m_pPShaderSet)
	{
		uiPShaderNum = m_pPShaderSet->GetNum();
	}

	VSShaderKey::SetMaterialPShaderKey(&m_PShaderkey,MSPara);

	if (MSPara.pMaterialInstance->m_pCurPShader[GetPassType()] == NULL)
	{
		if (m_pPShaderSet)
		{
			unsigned int uiIndex = m_pPShaderSet->Find(m_PShaderkey);

			if (uiIndex != m_pPShaderSet->GetNum())
			{
				VSShader * pTemp = (*m_pPShaderSet)[uiIndex].Value;
				pPixelShader = (VSPShader *)(pTemp);
			}
		}
		if (pPixelShader == NULL)
		{
			pPixelShader = VSResourceManager::CreatePShader(MSPara,uiPShaderNum);
			if (!pPixelShader)
			{
				return 0;
			}
			if (m_pPShaderSet)
			{
				m_pPShaderSet->AddElement(m_PShaderkey,pPixelShader);
			}
		}
		MSPara.pMaterialInstance->m_pCurPShader[GetPassType()] = pPixelShader;
	}
	else
	{
		if (MSPara.pMaterialInstance->m_pCurPShader[GetPassType()]->m_ShaderKey == m_PShaderkey)
		{

		}
		else
		{
			if (m_pPShaderSet)
			{
				unsigned int uiIndex = m_pPShaderSet->Find(m_PShaderkey);

				if (uiIndex != m_pPShaderSet->GetNum())
				{
					VSShader * pTemp = (*m_pPShaderSet)[uiIndex].Value;
					pPixelShader = (VSPShader *)(pTemp);
				}
			}
			if (pPixelShader == NULL)
			{
				pPixelShader = VSResourceManager::CreatePShader(MSPara,uiPShaderNum);
				if (!pPixelShader)
				{
					return 0;
				}
				if (m_pPShaderSet)
				{
					m_pPShaderSet->AddElement(m_PShaderkey,pPixelShader);
				}
			}
			MSPara.pMaterialInstance->m_pCurPShader[GetPassType()] = pPixelShader;
		}
	}

	if (!m_pPShaderSet)
	{
		ShaderMap.SetShader(Name,m_PShaderkey,pPixelShader);
		m_pPShaderSet = ShaderMap.GetShaderSet(Name);
	}
	return 1;
}
