#include "VSTexAllState.h"
#include "VS2DTexture.h"
#include "VSResourceManager.h"
#include "VSImage.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
#include "VSConfig.h"
#include "VSRenderThread.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSTexAllState,VSObject)
BEGIN_ADD_PROPERTY(VSTexAllState,VSObject)
REGISTER_PROPERTY(m_pTex,Tex,VSProperty::F_CLONE)
REGISTER_PROPERTY(m_SamplerDesc,SamplerDesc,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_bSRGB,bSRGB,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiFormatType, FormatType, VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiWidth, Width, VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiHeight, Height, VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiLength, Length, VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiArraySize, ArraySize, VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_SourceData, SourceData, VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_bNormal, bNormal, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_bMip, bMip, VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSTexAllState)
ADD_PRIORITY(VS2DTexture)
ADD_PRIORITY(VSSamplerState)
ADD_PRIORITY(VSConfig)
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION_WITH_PRIORITY(TerminalDefaultState)
IMPLEMENT_INITIAL_END
IMPLEMENT_RESOURCE(VSTexAllState)
VSPointer<VSTexAllState> VSTexAllState::ms_pOrenNayarLookUpTable;
//#define DEFAULT_16FLOAT_TEXTURE
bool VSTexAllState::InitialDefaultState()
{


	VSSamplerStatePtr pSamplerState = (VSSamplerState *)VSSamplerState::GetTriLine();

	VSFileName FileName = VSTexAllState::ms_ResourcePath;
	FileName += _T("Default.bmp");
	ms_Default = VSResourceManager::Load2DTexture(FileName.GetBuffer(),pSamplerState,VSResourceManager::CompressType::CT_NONE,false,true,true);
	if(!ms_Default)
		return 0;

	//
#ifdef DEFAULT_16FLOAT_TEXTURE
	unsigned int uiTextureFormat = VSRenderer::SFT_R16F;
#else
	unsigned int uiTextureFormat = VSRenderer::SFT_R32F;
#endif

	unsigned int uiOrenNayarTexSize = 128;
#ifdef DEFAULT_16FLOAT_TEXTURE
	unsigned short * pBuffer = VS_NEW unsigned short[uiOrenNayarTexSize * uiOrenNayarTexSize];	
#else
	VSREAL * pBuffer = VS_NEW VSREAL[uiOrenNayarTexSize * uiOrenNayarTexSize];
#endif		
	for (unsigned int i = 0 ; i < uiOrenNayarTexSize ;i++)
	{
		VSREAL VdotN = (i * 1.0f / (uiOrenNayarTexSize - 1)) * 2.0f - 1.0f;
		VSREAL AngleViewNormal = ACOS(VdotN);
		for (unsigned int j = 0 ; j < uiOrenNayarTexSize ; j++)
		{			
			VSREAL LdotN = (j * 1.0f / (uiOrenNayarTexSize - 1)) * 2.0f - 1.0f;
			VSREAL AngleLightNormal = ACOS(LdotN);
			
			VSREAL Alpha = Max(AngleViewNormal,AngleLightNormal);
			VSREAL Beta = Min(AngleViewNormal,AngleLightNormal);
			VSREAL fResult = ABS(SIN(Alpha) * TAN(Beta));
#ifdef DEFAULT_16FLOAT_TEXTURE
			pBuffer[i * uiOrenNayarTexSize + j] = FloatToHalf(fResult);	
#else
			pBuffer[i * uiOrenNayarTexSize + j] = fResult;	
#endif
		}
	}
	ms_pOrenNayarLookUpTable = VSResourceManager::Create2DTexture(uiOrenNayarTexSize,uiOrenNayarTexSize,uiTextureFormat,1,pBuffer);
	VSMAC_DELETEA(pBuffer);

	
	ms_pOrenNayarLookUpTable->SetSamplerState(pSamplerState);
	if (!ms_pOrenNayarLookUpTable)
	{
		return false;
	}

	ms_DefaultResource = VSTexAllStateR::Create(ms_Default);
	if (!ms_DefaultResource)
	{
		return false;
	}
	return 1;
}
bool VSTexAllState::TerminalDefaultState()
{
	ms_DefaultResource = NULL;
	ms_Default = NULL;
	ms_pOrenNayarLookUpTable = NULL;
	return 1;
}
void VSTexAllState::SetTexture(VSTexture * pTexture)
{
	m_pTex = pTexture;
	if (m_pTex)
	{
		m_bSRGB = m_pTex->GetSRGB();
		m_uiArraySize = m_pTex->GetArraySize();
		m_uiFormatType = m_pTex->GetFormatType();
		m_uiWidth = m_pTex->GetWidth(0);
		m_uiHeight = m_pTex->GetHeight(0);
		m_uiLength = m_pTex->GetLength(0);
		m_bMip = m_pTex->GetMipLevel() > 1;
	}
}
VSTexAllState::VSTexAllState()
{
	m_pTex			= NULL;
	m_pSamplerState = NULL;
	m_bSRGB = false;
	m_bNormal = false;
	m_uiArraySize = 1;
	m_uiFormatType = VSRenderer::SFT_A8R8G8B8;
	m_uiWidth = 1;
	m_uiHeight = 1;
	m_uiLength = 1;
	m_bMip = false;
}
VSTexAllState::~VSTexAllState()
{
	m_pTex			= NULL;

	m_pSamplerState = NULL;
}
bool VSTexAllState::BeforeSave(VSStream* pStream)
{
	if (m_SourceData.GetNum() > 0 && m_uiLength == 1 && m_uiArraySize == 1)
	{
		VS2DTexture * pNewTexture = VSResourceManager::CreateTextureCache(m_SourceData.GetBuffer(), m_uiWidth, m_uiHeight,
			m_uiFormatType, m_bNormal, m_bSRGB, m_bMip);
		VSString CacheFilePath = GetCacheFilePath();
		VSTextureCache TextureCache(pNewTexture);
		VSResourceManager::SaveResouce(&TextureCache, CacheFilePath.GetBuffer());
	}
	return true;
}
bool VSTexAllState::PostLoad(VSStream* pStream)
{
	m_pSamplerState = VSResourceManager::CreateSamplerState(m_SamplerDesc);

	return true;
}
bool VSTexAllState::PostClone(VSObject * pObjectSrc)
{
	m_pSamplerState = VSResourceManager::CreateSamplerState(m_SamplerDesc);

	return true;
}
void VSTexAllState::SetSRGBEable(bool bEnable)
{
	m_bSRGB = bEnable;

}
unsigned int VSTexAllState::GetCurStreamLevel()const
{
	return m_pTex->GetMipLevel();
}
VSObject* VSTexAllState::CreateToStreamObject(unsigned int uiWantSteamLevel, const VSCacheResource* pCacheResouce)const
{
	const VSTextureCache* pTextureCache = DynamicCast<VSTextureCache>(pCacheResouce);
	const VSTexture* pTexture = pTextureCache->GetTexture();
	unsigned int MipDelta =  pTexture->GetMipLevel() - uiWantSteamLevel;
	VSMAC_ASSERT(MipDelta >= 0);
	unsigned char* pData = NULL;
	VSTexture* pNewTexture = NULL;

	if (pTexture->GetTexType() == VSTexture::TT_1D)
	{
		pNewTexture = VS_NEW VS1DTexture(pTexture->GetWidth(MipDelta), m_uiFormatType, uiWantSteamLevel, true, m_bSRGB);
	}
	else if (pTexture->GetTexType() == VSTexture::TT_2D)
	{
		pNewTexture = VS_NEW VS2DTexture(pTexture->GetWidth(MipDelta), pTexture->GetHeight(MipDelta),m_uiFormatType, uiWantSteamLevel, true, m_bSRGB);
	}
	else if (pTexture->GetTexType() == VSTexture::TT_3D)
	{
		pNewTexture = VS_NEW VS3DTexture(pTexture->GetWidth(MipDelta), pTexture->GetHeight(MipDelta), pTexture->GetLength(MipDelta), m_uiFormatType, uiWantSteamLevel, true, m_bSRGB);
	}
	else if (pTexture->GetTexType() == VSTexture::TT_CUBE)
	{
		pNewTexture = VS_NEW VSCubeTexture(pTexture->GetWidth(MipDelta), m_uiFormatType, uiWantSteamLevel, true, m_bSRGB);
	}

	pNewTexture->CopyRAMData(pTexture);

	return pNewTexture;
}
void VSTexAllState::StreamEnd(VSObject* pStreamResource)
{
	if (m_pTex)
	{
		m_pTex->ClearAllSlot();
	}
	SetTexture((VSTexture*)pStreamResource);
}