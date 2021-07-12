#include "VSTextureUnorderAccess.h"
#include "VSVertexFormat.h"
#include "VSGraphicInclude.h"
#include "VS2DTexture.h"
#include "VS2DTextureArray.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSTextureUnorderAccess, VSUnorderAccess)
BEGIN_ADD_PROPERTY(VSTextureUnorderAccess, VSUnorderAccess)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSTextureUnorderAccess)
IMPLEMENT_INITIAL_END
VSTextureUnorderAccess::VSTextureUnorderAccess()
{
	m_uiWidth = 512;
	m_uiHeight = 512;
	m_uiFormatType = VSRenderer::SFT_R32F;
	m_uiMulSample = VSRenderer::MS_NONE;
	m_pCreateBy = NULL;
	m_bUsed = false;
	m_pLockData = NULL;
	m_uiFirst = 0;
	m_uiLevel = 0;
	m_uiArraySize = 1;
}
VSTextureUnorderAccess::VSTextureUnorderAccess(unsigned int uiWidth, unsigned int uiHeight,
	unsigned int uiFormatType, bool CPURead)
{
	m_uiWidth = uiWidth;
	m_uiHeight = uiHeight;
	m_uiFormatType = uiFormatType;
	m_uiMulSample = VSRenderer::MS_NONE;
	m_pCreateBy = NULL;
	m_uiLevel = 0;
	m_uiFirst = 0;
	m_bUsed = false;
	m_bIsStatic = !CPURead;
	m_uiLockFlag = CPURead ? VSInheritBind::LF_READONLY : VSInheritBind::LF_NOOVERWRITE;
	m_pLockData = NULL;
	m_uiArraySize = 1;
}
VSTextureUnorderAccess::VSTextureUnorderAccess(VS2DTexture * pCreateBy,unsigned int uiLevel, bool CPURead)
{


	m_uiFirst = 0;
	m_uiFormatType = pCreateBy->GetFormatType();
	m_uiMulSample = VSRenderer::MS_NONE;
	m_pCreateBy = pCreateBy;
	m_uiLevel = uiLevel;
	m_uiWidth = pCreateBy->GetWidth(m_uiLevel);
	m_uiHeight = pCreateBy->GetHeight(m_uiLevel);
	m_bUsed = false;
	m_bIsStatic = !CPURead;
	m_uiLockFlag = CPURead ? VSInheritBind::LF_READONLY : VSInheritBind::LF_NOOVERWRITE;
	m_pLockData = NULL;
	m_uiArraySize = 1;
	if (!pCreateBy->SetOutput(this))
	{
		VSMAC_ASSERT(0);
	}
}
VSTextureUnorderAccess::VSTextureUnorderAccess(VS2DTextureArray * pCreateBy,unsigned int uiLevel,
	unsigned int First, unsigned int uiArraySize, bool CPURead)
{
	if (pCreateBy->GetTexType() == VSTexture::TT_CUBE && uiArraySize > VSCubeTexture::F_MAX)
	{
		m_uiArraySize = VSCubeTexture::F_MAX;
	}

	m_uiFirst = First;

	m_uiFormatType = pCreateBy->GetFormatType();
	m_uiMulSample = VSRenderer::MS_NONE;
	m_pCreateBy = pCreateBy;
	m_uiLevel = uiLevel;
	m_uiWidth = pCreateBy->GetWidth(m_uiLevel);
	m_uiHeight = pCreateBy->GetHeight(m_uiLevel);
	m_bUsed = false;
	m_bIsStatic = !CPURead;
	m_uiLockFlag = CPURead ? VSInheritBind::LF_READONLY : VSInheritBind::LF_NOOVERWRITE;
	m_pLockData = NULL;
	m_uiArraySize = uiArraySize;
	if (!pCreateBy->SetOutput(this))
	{
		VSMAC_ASSERT(0);
	}
}
VSTextureUnorderAccess::VSTextureUnorderAccess(VS3DTexture * pCreateBy, unsigned int uiLevel,
	unsigned int First, unsigned int uiArraySize, bool CPURead)
{
	m_uiFirst = First;
	m_uiFormatType = pCreateBy->GetFormatType();
	m_uiMulSample = VSRenderer::MS_NONE;
	m_pCreateBy = pCreateBy;
	m_uiLevel = uiLevel;
	m_uiWidth = pCreateBy->GetWidth(m_uiLevel);
	m_uiHeight = pCreateBy->GetHeight(m_uiLevel);
	m_bUsed = false;
	m_bIsStatic = !CPURead;
	m_uiLockFlag = CPURead ? VSInheritBind::LF_READONLY : VSInheritBind::LF_NOOVERWRITE;
	m_pLockData = NULL;
	m_uiArraySize = uiArraySize;
	if (!pCreateBy->SetOutput(this))
	{
		VSMAC_ASSERT(0);
	}
}
VSTextureUnorderAccess::~VSTextureUnorderAccess()
{
	ReleaseResource();
	m_pCreateBy = NULL;
}
bool VSTextureUnorderAccess::OnLoadResource(VSResourceIdentifier *&pID)
{

	if (!m_pUser->OnLoadUnorderAccess(this, pID))
		return 0;
	return 1;
}
void *VSTextureUnorderAccess::Lock()
{
	if (m_pLockData)
	{
		return NULL;
	}

	m_pLockData = m_pUser->Lock(this);


	return m_pLockData;
}
void VSTextureUnorderAccess::UnLock()
{
	if (!m_pLockData)
	{
		return;
	}

	m_pUser->UnLock(this);

	m_pLockData = NULL;
}