#include "VSSamplerState.h"
#include "VSShader.h"
#include "VSMaterial.h"
#include "VSGraphicInclude.h"
#include "VSResourceManager.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSSamplerState,VSBind)
VSPointer<VSSamplerState> VSSamplerState::ms_Default;
VSPointer<VSSamplerState> VSSamplerState::ms_DoubleLine;
VSPointer<VSSamplerState> VSSamplerState::ms_TriLine;
VSPointer<VSSamplerState> VSSamplerState::ms_TwoLineBorderOne;
VSPointer<VSSamplerState> VSSamplerState::ms_TwoLineBorderZero;
VSPointer<VSSamplerState> VSSamplerState::ms_TriLineClamp;
VSPointer<VSSamplerState> VSSamplerState::ms_DoubleLineClamp;
VSPointer<VSSamplerState> VSSamplerState::ms_Clamp;
VSPointer<VSSamplerState> VSSamplerState::ms_ShareSampler[VSEngineFlag::ms_uiShaderSamplerNum];
BEGIN_ADD_PROPERTY(VSSamplerState,VSBind)
REGISTER_PROPERTY(m_SamplerDesc,SamplerDesc,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSSamplerState)
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
bool VSSamplerState::InitialDefaultState()
{
	VSSamplerDesc SamplerDesc;
	ms_Default = VSResourceManager::CreateSamplerState(SamplerDesc);
	if(!ms_Default)
		return 0;


	SamplerDesc.m_uiMag = VSSamplerDesc::FM_LINE;
	SamplerDesc.m_uiMin = VSSamplerDesc::FM_LINE;
	ms_DoubleLine = VSResourceManager::CreateSamplerState(SamplerDesc);
	if(!ms_DoubleLine)
		return 0;


	SamplerDesc.m_uiMag = VSSamplerDesc::FM_LINE;
	SamplerDesc.m_uiMin = VSSamplerDesc::FM_LINE;
	SamplerDesc.m_uiMip = VSSamplerDesc::FM_LINE;
	ms_TriLine = VSResourceManager::CreateSamplerState(SamplerDesc);
	if(!ms_TriLine)
		return 0;


	VSSamplerDesc TwoLineBorderOneDesc;
	TwoLineBorderOneDesc.m_uiMag = VSSamplerDesc::FM_LINE;
	TwoLineBorderOneDesc.m_uiMin = VSSamplerDesc::FM_LINE;
	TwoLineBorderOneDesc.m_uiCoordU = VSSamplerDesc::CM_BORDER;
	TwoLineBorderOneDesc.m_uiCoordV = VSSamplerDesc::CM_BORDER;
	TwoLineBorderOneDesc.m_BorderColor = VSColorRGBA(1.0f,1.0f,1.0f,1.0f);
	ms_TwoLineBorderOne = VSResourceManager::CreateSamplerState(TwoLineBorderOneDesc);
	if (!ms_TwoLineBorderOne)
		return 0;

	VSSamplerDesc TwoLineBorderZeroDesc;
	TwoLineBorderZeroDesc.m_uiMag = VSSamplerDesc::FM_LINE;
	TwoLineBorderZeroDesc.m_uiMin = VSSamplerDesc::FM_LINE;
	TwoLineBorderZeroDesc.m_uiCoordU = VSSamplerDesc::CM_BORDER;
	TwoLineBorderZeroDesc.m_uiCoordV = VSSamplerDesc::CM_BORDER;
	ms_TwoLineBorderZero = VSResourceManager::CreateSamplerState(TwoLineBorderZeroDesc);
	if (!ms_TwoLineBorderZero)
		return 0;


	VSSamplerDesc ShareDesc;

	ShareDesc.m_uiCoordU = VSSamplerDesc::CM_CLAMP;
	ShareDesc.m_uiCoordV = VSSamplerDesc::CM_CLAMP;
	ShareDesc.m_uiCoordW = VSSamplerDesc::CM_CLAMP;
	ms_Clamp = VSResourceManager::CreateSamplerState(ShareDesc);

	
	ShareDesc.m_uiMin = VSSamplerDesc::FM_LINE;	
	ShareDesc.m_uiMag = VSSamplerDesc::FM_LINE;
	ms_DoubleLineClamp = VSResourceManager::CreateSamplerState(ShareDesc);


	ShareDesc.m_uiMip = VSSamplerDesc::FM_LINE;
	ms_TriLineClamp = VSResourceManager::CreateSamplerState(ShareDesc);


	ms_ShareSampler[0] = ms_TriLineClamp;
	ms_ShareSampler[1] = ms_TriLine;
 	ms_ShareSampler[2] = ms_TwoLineBorderOne;
 	ms_ShareSampler[3] = ms_TwoLineBorderZero;
	return 1;
}
bool VSSamplerState::TerminalDefaultState()
{
	ms_Default = NULL;
	ms_DoubleLine = NULL;
	ms_TriLine = NULL;
	ms_TwoLineBorderOne = NULL;
	ms_TwoLineBorderZero = NULL;
	ms_DoubleLineClamp = NULL;
	ms_TriLineClamp = NULL;
	ms_Clamp = NULL;
	for (unsigned int i = 0 ; i < VSEngineFlag::ms_uiShaderSamplerNum ; i++)
	{
		ms_ShareSampler[i] = NULL;
	}
	return 1;
}
bool VSSamplerState::OnLoadResource(VSResourceIdentifier *&pID)
{
	if(!m_pUser)
		return 0;
	if(!m_pUser->OnLoadSamplerState(this,pID))
		return 0;
	return 1;
}
VSSamplerState::VSSamplerState()
{

}
VSSamplerState::~VSSamplerState()
{
	ReleaseResource();
}

IMPLEMENT_RTTI(VSSamplerDesc,VSObject)
BEGIN_ADD_PROPERTY(VSSamplerDesc,VSObject)
REGISTER_PROPERTY(m_uiMag,Mag,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiMin,Min,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiMip,Mip,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiMipLevel,MipLevel,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiAniLevel,AniLevel,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiCoordU,CoordU,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiCoordV,CoordV,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiCoordW,CoordW,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_BorderColor,BorderColor,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_MipLODBias, MipLODBias, VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSSamplerDesc)
IMPLEMENT_INITIAL_END
