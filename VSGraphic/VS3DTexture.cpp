#include "VS3DTexture.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VS3DTexture,VSTexture)
BEGIN_ADD_PROPERTY(VS3DTexture,VSTexture)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VS3DTexture)
IMPLEMENT_INITIAL_END
VS3DTexture::VS3DTexture(unsigned int uiWidth, unsigned int uiHeight,unsigned int uiLength,
						 unsigned int uiFormatType,unsigned int uiMipLevel,bool bIsStatic, bool bSRGB)
						 :VSTexture(uiFormatType,uiWidth,uiHeight,uiLength,1,uiMipLevel,bIsStatic, bSRGB)
{

	SetMipLevel();

}
VS3DTexture::~VS3DTexture()
{

}
VS3DTexture::VS3DTexture()
{

}
void VS3DTexture::SetMipLevel()
{
	VSMAC_ASSERT(m_uiWidth && m_uiHeight && m_uiLength);
	VSMAC_ASSERT(IsTwoPower(m_uiWidth) && IsTwoPower(m_uiHeight) && IsTwoPower(m_uiLength));
	unsigned int uiWidthLevel = FastLog2(m_uiWidth);
	unsigned int uHeightLevel = FastLog2(m_uiHeight);
	unsigned int uiLengthLevel = FastLog2(m_uiLength);;
	unsigned int uiCurMipLevel = Max(Max(uiWidthLevel,uHeightLevel),uiLengthLevel) + 1;
	if (!m_uiMipLevel || uiCurMipLevel < m_uiMipLevel)
	{
		m_uiMipLevel = uiCurMipLevel;
	}

}
bool VS3DTexture::SetOutput(class VSOutputResource *pOutputResource)
{
	VSMAC_ASSERT(pOutputResource&& pOutputResource->GetOutputType() != VSOutputResource::OT_BUFFER_UNORDER_ACCESS);
	if (IsBindResource())
	{
		return false;
	}
	if (m_uiOutputType == VSOutputResource::OT_NONE)
	{
		m_uiOutputType = ((VSOutputResource *)pOutputResource)->GetOutputType();
		m_pOutputResource.AddElement(pOutputResource);
	}
	else if (m_uiOutputType != ((VSOutputResource *)pOutputResource)->GetOutputType())
	{
		return false;
	}
	else
	{
		bool IsOk = true;
		
		for (unsigned int i = 0; i < m_pOutputResource.GetNum(); i++)
		{
			VSTextureOutputInfo * pTextureOutputInfo_i = GetTextureOutputInfo(m_pOutputResource[i]);
			VSTextureOutputInfo * pTextureOutputInfo = GetTextureOutputInfo(pOutputResource);
			if (pTextureOutputInfo_i->GetLevel() == pTextureOutputInfo->GetLevel())
			{
				unsigned int uiMin1 = pTextureOutputInfo_i->GetFirst();
				unsigned int uiMax1 = pTextureOutputInfo_i->GetFirst() + pTextureOutputInfo_i->GetArraySize() - 1;

				unsigned int uiMin2 = pTextureOutputInfo->GetFirst();
				unsigned int uiMax2 = pTextureOutputInfo->GetFirst() + pTextureOutputInfo->GetArraySize() - 1;

				if (uiMin1 <= uiMax2 && uiMin2 <= uiMax1)
				{
					IsOk = false;
					break;
				}
			}
		}

		if (IsOk)
		{
			m_pOutputResource.AddElement(pOutputResource);
		}
		else
		{
			VSMAC_ASSERT(0);
			return false;
		}
	}
	return true;
}
