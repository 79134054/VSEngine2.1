#include "VSShader.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI_NoCreateFun(VSShader,VSBind)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSShader)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(VSShader,VSBind)
REGISTER_PROPERTY(m_pUserConstant,UserConstant,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_pUserSampler,UserSampler,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_pUserBuffer, UserBuffer, VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_ShaderKey,ShaderKey,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_bCreatePara,bCreatePara,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY_DATA(m_pCacheBuffer,m_uiCacheBufferSize,CacheBuffer);
REGISTER_PROPERTY(m_uiArithmeticInstructionSlots,ArithmeticInstructionSlots,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiTextureInstructionSlots,TextureInstructionSlots,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiConstBufferSize, ConstBufferSize, VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiShareSamplerRegister, ShareSamplerRegister, VSProperty::F_SAVE_LOAD_CLONE);
END_ADD_PROPERTY
VSShader::VSShader(const TCHAR * pBuffer,const VSString & MainFunName,bool IsFromFile)
:VSBind()
{
	m_pUserConstant.Clear();
	VSMAC_ASSERT(pBuffer);
	if (IsFromFile)
	{
		m_FileName = pBuffer;
	}
	else
	{
		m_Buffer = pBuffer;
	}

	m_pCacheBuffer = NULL;
	m_uiCacheBufferSize = 0;
	m_MainFunName = MainFunName;
	m_bCreatePara = false;
	m_uiArithmeticInstructionSlots = 0;
	m_uiTextureInstructionSlots = 0;
	m_uiConstBufferSize = 0;
}
VSShader::VSShader()
:VSBind()
{
	m_pCacheBuffer = NULL;
	m_uiCacheBufferSize = 0;
	m_bCreatePara = false;
	m_uiArithmeticInstructionSlots = 0;
	m_uiTextureInstructionSlots = 0;
	m_uiConstBufferSize = 0;
}
VSShader::VSShader(const VSString &Buffer,const VSString & MainFunName,bool IsFromFile)
:VSBind()
{
	m_pUserConstant.Clear();
	if (IsFromFile)
	{
		m_FileName = Buffer;
	}
	else
	{
		m_Buffer = Buffer;
	}

	m_pCacheBuffer = NULL;
	m_uiCacheBufferSize = 0;
	m_MainFunName = MainFunName;
	m_bCreatePara = false;
	m_uiArithmeticInstructionSlots = 0;
	m_uiTextureInstructionSlots = 0;
}
VSShader::~VSShader()
{
	VSMAC_DELETEA(m_pCacheBuffer);
	m_uiCacheBufferSize = 0;
	ReleaseResource();
}
void VSShader::SetShaderString(const TCHAR * pBuffer,const VSString & MainFunName,bool IsFromFile)
{
	VSMAC_DELETEA(m_pCacheBuffer);
	m_uiCacheBufferSize = 0;
	ReleaseResource();

	VSMAC_ASSERT(pBuffer);
	if (IsFromFile)
	{
		m_FileName = pBuffer;
	}
	else
	{
		m_Buffer = pBuffer;
	}

	m_pCacheBuffer = NULL;
	m_uiCacheBufferSize = 0;
	m_MainFunName = MainFunName;
}
void VSShader::SetShaderString(const VSString &Buffer,const VSString & MainFunName,bool IsFromFile)
{
	VSMAC_DELETEA(m_pCacheBuffer);
	m_uiCacheBufferSize = 0;
	ReleaseResource();

	if (IsFromFile)
	{
		m_FileName = Buffer;
	}
	else
	{
		m_Buffer = Buffer;
	}

	m_pCacheBuffer = NULL;
	m_uiCacheBufferSize = 0;
	m_MainFunName = MainFunName;
}
bool VSShader::SetParam(const VSUsedName &Name,VSTexAllState * pTexture,unsigned int uiIndex)
{
	if (!pTexture || !m_bCreatePara)
	{
		return false;
	}
	for (unsigned int i = 0 ; i < m_pUserSampler.GetNum() ;i++)
	{
		if (Name == m_pUserSampler[i]->GetName()
			&& pTexture->GetTexture()->GetTexType() == m_pUserSampler[i]->GetTexType())
		{
			m_pUserSampler[i]->SetTex(pTexture,uiIndex);
			break;
		}	
	}
	return true;
}
bool VSShader::HasBindResource()
{
	return m_pUserConstant.GetNum() || m_pUserSampler.GetNum() || m_pUserBuffer.GetNum();
}
bool VSShader::SetParam(const VSUsedName &Name, VSBufferResource * pBufferResource, unsigned int uiIndex)
{
	if (!pBufferResource || !m_bCreatePara)
	{
		return false;
	}
	for (unsigned int i = 0; i < m_pUserBuffer.GetNum(); i++)
	{
		if (Name == m_pUserBuffer[i]->GetName()
			&& pBufferResource->GetDataType() == m_pUserBuffer[i]->GetDataType())
		{
			m_pUserBuffer[i]->SetBufferResource(pBufferResource, uiIndex);
			break;
		}
	}
	return true;
}
bool VSShader::SetParam(const VSUsedName &Name,void * pData)
{
	if (!pData || !m_bCreatePara)
	{
		return false;
	}
	for (unsigned int i = 0 ; i < m_pUserConstant.GetNum() ;i++)
	{
		if (Name == m_pUserConstant[i]->GetShowName())
		{
			void * pConstantData = m_pUserConstant[i]->GetData();
			if (!pConstantData)
			{
				return false;
			}
			VSMemcpy(pConstantData,pData,m_pUserConstant[i]->GetSize());
			break;
		}	
	}
	return true;
}
bool VSShader::SetCacheBuffer(void * pBuffer,unsigned int uiSize)
{
	if (!pBuffer || !uiSize)
	{
		return false;
	}
	VSMAC_DELETEA(m_pCacheBuffer);
	m_uiCacheBufferSize = uiSize;
	m_pCacheBuffer = VS_NEW UCHAR[uiSize];
	VSMemcpy(m_pCacheBuffer,pBuffer,uiSize);
	return 1;
}
void VSShader::AddShareSampler(unsigned int ShareSamplerRegister, unsigned int ShareSamplerType)
{
	if (ShareSamplerRegister >= VSRenderer::ms_pRenderer->GetMaxSampler(GetShaderType()))
	{
		VSMAC_ASSERT(0);
		return;
	}
	if (ShareSamplerType >= VSEngineFlag::SS_MAX)
	{
		VSMAC_ASSERT(0);
		return;
	}
	if (m_uiShareSamplerRegister.GetNum() == VSEngineFlag::SS_MAX )
	{
		return;
	}
	for (unsigned int i = 0 ; i < m_uiShareSamplerRegister.GetNum() ; i++)
	{
		if (m_uiShareSamplerRegister[i].Value == ShareSamplerType)
		{
			return;
		}
	}
	m_uiShareSamplerRegister.AddElement(ShareSamplerRegister, ShareSamplerType);
}