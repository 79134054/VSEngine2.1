#include "VS1DTexture.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VS1DTexture,VSTexture)
BEGIN_ADD_PROPERTY(VS1DTexture,VSTexture)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VS1DTexture)
IMPLEMENT_INITIAL_END
VS1DTexture::VS1DTexture(unsigned int uiWidth,unsigned int uiFormatType,
						 unsigned int uiMipLevel,bool bIsStatic, bool bSRGB)
						 :VSTexture(uiFormatType,uiWidth,1,1,1,uiMipLevel,bIsStatic, bSRGB)
{

	SetMipLevel();
	m_pOutputResource.SetBufferNum(m_uiMipLevel);

	for (unsigned int i = 0; i < m_uiMipLevel; i++)
	{
		m_pOutputResource[i] = NULL;

	}
	
}
VS1DTexture::~VS1DTexture()
{

}
VS1DTexture::VS1DTexture()
{

}
void VS1DTexture::SetMipLevel()
{
	VSMAC_ASSERT(m_uiWidth && m_uiHeight);
	VSMAC_ASSERT(IsTwoPower(m_uiWidth));
	unsigned int uiWidthLevel = FastLog2(m_uiWidth);
	unsigned int uiCurMipLevel = uiWidthLevel + 1;
	if (!m_uiMipLevel || m_uiMipLevel > uiCurMipLevel)
	{
		m_uiMipLevel = uiCurMipLevel;
	}

}