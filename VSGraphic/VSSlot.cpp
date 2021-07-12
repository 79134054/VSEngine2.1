#include "VSSlot.h"
#include "VSGraphicInclude.h"
#include "VSRenderer.h"
using namespace VSEngine2;
VSSlot::VSSlot()
{
	m_bBindResourceUse = false;
	m_uiOutputType = VSOutputResource::OT_NONE;
}
VSSlot::~VSSlot()
{

}
void VSSlot::ClearAllSlot()
{
	while (m_VSTexSlot.GetNum() > 0)
	{
		VSRenderer::ms_pRenderer->SetVTexture(NULL, m_VSTexSlot[0]);
	}
	while (m_PSTexSlot.GetNum() > 0)
	{
		VSRenderer::ms_pRenderer->SetPTexture(NULL, m_PSTexSlot[0]);
	}
	while (m_GSTexSlot.GetNum() > 0)
	{
		VSRenderer::ms_pRenderer->SetGTexture(NULL, m_GSTexSlot[0]);
	}
	while (m_HSTexSlot.GetNum() > 0)
	{
		VSRenderer::ms_pRenderer->SetHTexture(NULL, m_HSTexSlot[0]);
	}
	while (m_DSTexSlot.GetNum() > 0)
	{
		VSRenderer::ms_pRenderer->SetDTexture(NULL, m_DSTexSlot[0]);
	}
	while (m_CSTexSlot.GetNum() > 0)
	{
		VSRenderer::ms_pRenderer->SetCTexture(NULL, m_CSTexSlot[0]);
	}

	m_VSTexSlot.Clear();
	m_PSTexSlot.Clear();
	m_GSTexSlot.Clear();
	m_DSTexSlot.Clear();
	m_HSTexSlot.Clear();
	m_CSTexSlot.Clear();
}
bool VSSlot::HasAnySlot()
{
	return m_VSTexSlot.GetNum() || m_PSTexSlot.GetNum()
		|| m_GSTexSlot.GetNum() || m_DSTexSlot.GetNum()
		|| m_HSTexSlot.GetNum() || m_CSTexSlot.GetNum();
}