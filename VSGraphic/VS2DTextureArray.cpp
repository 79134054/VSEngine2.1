#include "VS2DTextureArray.h"
#include "VSGraphicInclude.h"
#include "VSRenderTarget.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VS2DTextureArray, VSTexture)
BEGIN_ADD_PROPERTY(VS2DTextureArray, VSTexture)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VS2DTextureArray)
IMPLEMENT_INITIAL_END
VS2DTextureArray::VS2DTextureArray(unsigned int uiWidth, unsigned int uiHeight, unsigned int uiFormatType,
	unsigned int uiMipLevel, unsigned int uiArraySize, bool bIsStatic, bool bSRGB)
	:VSTexture(uiFormatType, uiWidth, uiHeight, 1, uiArraySize,uiMipLevel, bIsStatic, bSRGB)
{

	SetMipLevel();
	m_uiArraySize = uiArraySize;
}
bool VS2DTextureArray::SetOutput(class VSOutputResource *pOutputResource)
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
		for (unsigned int i = 0; i < m_pOutputResource.GetNum() ; i++)
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

VS2DTextureArray::VS2DTextureArray(VSArray<VS2DTexture *> &pTextureArray)
{
	bool bFind = false;
	unsigned int uiWidth = 0;
	unsigned int uiHeight = 0;
	unsigned int uiFormat = 0;
	unsigned int uiMipLevel = 0;
	unsigned int uiOutputType = VSOutputResource::OT_NONE;
	bool bStatic = true;
	bool bSRGB = false;
	for (unsigned int i = 0; i < pTextureArray.GetNum(); i++)
	{
		if (pTextureArray[i])
		{
			if (!bFind)
			{
				uiWidth = pTextureArray[i]->GetWidth(0);
				uiHeight = pTextureArray[i]->GetHeight(0);
				VSMAC_ASSERT(uiWidth == uiHeight);
				uiFormat = pTextureArray[i]->GetFormatType();
				uiOutputType = pTextureArray[i]->GetOutputType();
				uiMipLevel = pTextureArray[i]->GetMipLevel();
				bStatic = pTextureArray[i]->IsStatic();
				bSRGB = pTextureArray[i]->GetSRGB();
				bFind = true;


			}
			else
			{
				if (uiWidth != pTextureArray[i]->GetWidth(0)
					|| uiFormat != pTextureArray[i]->GetFormatType()
					|| uiOutputType != pTextureArray[i]->GetOutputType()
					|| uiMipLevel != pTextureArray[i]->GetMipLevel()
					|| bStatic != pTextureArray[i]->IsStatic()
					|| bSRGB != pTextureArray[i]->GetSRGB())
				{
					VSMAC_ASSERT(0);
				}
			}

		}
	}



	VSMAC_ASSERT(uiWidth);
	VSMAC_ASSERT(uiFormat < VSRenderer::SFT_MAX);
	m_uiFormatType = uiFormat;
	m_uiHeight = uiWidth;
	m_uiWidth = uiWidth;
	m_uiLength = 1;


	m_bIsStatic = true;
	m_uiMipLevel = uiMipLevel;
	m_uiArraySize = pTextureArray.GetNum();
	m_bSRGB = bSRGB;
	if (uiOutputType == VSOutputResource::OT_NONE)
	{
		SetMipLevel();

		CreateRAMData();
		for (unsigned int i = 0; i < m_uiArraySize; i++)
		{
			if (pTextureArray[i])
			{
				for (unsigned int j = 0; j < m_uiMipLevel; j++)
				{
					VSMemcpy(GetBuffer(j, i), pTextureArray[i]->GetBuffer(j), pTextureArray[i]->GetByteSize(j));
				}

			}
			else
			{
				for (unsigned int j = 0; j < m_uiMipLevel; j++)
				{
					VSMemset(GetBuffer(j, i), 0, GetByteSize(j));
				}
			}
		}


	}
	else
	{
		VSMAC_ASSERT(0);
	}
}
VS2DTextureArray::~VS2DTextureArray()
{

}
VS2DTextureArray::VS2DTextureArray()
{

}
void VS2DTextureArray::SetMipLevel()
{
	VSMAC_ASSERT(m_uiWidth && m_uiHeight);
	VSMAC_ASSERT(IsTwoPower(m_uiWidth));
	unsigned int uiWidthLevel = FastLog2(m_uiWidth);
	unsigned int uiCurMipLevel = uiWidthLevel + 1;
	if (!m_uiMipLevel || uiCurMipLevel < m_uiMipLevel)
	{
		m_uiMipLevel = uiCurMipLevel;
	}

}