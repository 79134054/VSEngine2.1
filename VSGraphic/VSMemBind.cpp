#include "VSMemBind.h"
#include "VSResourceManager.h"
#include "VSGraphicInclude.h"
#include "VSRenderThread.h"
using namespace VSEngine2;
IMPLEMENT_RTTI_NoCreateFun(VSMemBind, VSInheritBind)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSMemBind)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(VSMemBind, VSInheritBind)
REGISTER_PROPERTY(m_uiSwapChainNum, SwapChainNum, VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiMemType, MemType, VSProperty::F_SAVE_LOAD_CLONE);
END_ADD_PROPERTY
VSMemBind::VSMemBind(unsigned int uiMemType)
{
	m_uiSwapChainNum = 1;
	m_uiCurID = 0;
	
	m_uiMemType = uiMemType;
}
VSMemBind::~VSMemBind()
{
	if (!m_bIsStatic && m_uiSwapChainNum == 2)
	{
		if (VSResourceManager::ms_bRenderThread)
		{
			VSResourceManager::GetDelayUpdateNoTimeObject().RemoveMethod<VSMemBind, &VSMemBind::ExChange>(this);
		}
	}
}
VSResourceIdentifier* VSMemBind::GetIdentifier()
{
	if (!m_InfoArray.GetNum())
	{
		return NULL;
	}
	VSResourceIdentifier * pID = NULL;
	pID = m_InfoArray[m_uiCurID].ID;
	return pID;
}
void VSMemBind::SetStatic(bool bIsStatic, bool bMulThreadUse)
{
	if (!m_bIsStatic && m_uiSwapChainNum == 2)
	{
		if (VSResourceManager::ms_bRenderThread)
		{
			VSResourceManager::GetDelayUpdateNoTimeObject().RemoveMethod<VSMemBind, &VSMemBind::ExChange>(this);
		}
	}
	m_bIsStatic = bIsStatic;

	if (m_bIsStatic)
	{
		m_uiSwapChainNum = 1;
	}
	else
	{
		if (VSResourceManager::ms_bRenderThread && bMulThreadUse && VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_MulBufferSwtich))
		{

			m_uiSwapChainNum = 2;
			VSResourceManager::GetDelayUpdateNoTimeObject().AddMethod<VSMemBind, &VSMemBind::ExChange>(this);
		}
		else
		{
			m_uiSwapChainNum = 1;
		}

	}

	m_uiCurID = 0;

}
void VSMemBind::ExChange()
{
	if (!m_bIsStatic && m_uiSwapChainNum == 2)
	{
		m_uiCurID = (m_uiCurID + 1) % m_uiSwapChainNum;
	}
}
bool VSMemBind::LoadResource(VSRenderer * pRender)
{
	if (!pRender)
		return 0;
	if (m_uiMemType == MT_RAM)
	{
		return 1;
	}
	if (m_uiSwapChainNum == m_InfoArray.GetNum())
		return 1;
	else
	{
		m_pUser = pRender;
		for (unsigned int i = 0; i < m_uiSwapChainNum; i++)
		{
			VSResourceIdentifier *pID = NULL;
			if (!OnLoadResource(pID))
				return 0;
			if (!pID)
				return 0;
			Bind(pID);
		}
		ENQUEUE_LAMBDA_RENDER_COMMAND(ClearInfoCommand)
		([this]()
		{
			ClearInfo();
		});
		return 1;
	}

}