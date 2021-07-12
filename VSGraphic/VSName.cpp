#include "VSName.h"
#include "VSResourceManager.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
VSName::~VSName()
{

	m_uiID = 0;
	m_String.Clear();
	
}
VSName::VSName(const TCHAR * pChar,unsigned int uiID)
{
	VSMAC_ASSERT(pChar);
	m_uiID = uiID;
	m_String.Clear();
	m_String = pChar;
}
VSName::VSName(const VSString & String,unsigned int uiID)
{
	m_uiID = uiID;
	m_String.Clear();
	m_String = String;

}

VSUsedName::~VSUsedName()
{
	m_pName = NULL;
}
VSUsedName::VSUsedName()
{
	m_pName = NULL;
}
VSUsedName::VSUsedName(const TCHAR * pChar)
{
	m_pName = NULL;

	m_pName = VSResourceManager::CreateName(pChar);
}
VSUsedName::VSUsedName(const VSString & String)
{
	m_pName = NULL;
	m_pName = VSResourceManager::CreateName(String);
}
void VSUsedName::operator =(const VSString &String)
{
	m_pName = NULL;
	m_pName = VSResourceManager::CreateName(String);
		
}
void VSUsedName::operator =(const TCHAR *pChar)
{
	m_pName = NULL;

	m_pName = VSResourceManager::CreateName(pChar);
}
void VSUsedName::operator =(const VSUsedName & Name)
{
	m_pName = Name.m_pName;
}
VSUsedName VSUsedName::ms_cPreZBeUsedBone;
VSUsedName VSUsedName::ms_cUseInstance;
VSUsedName VSUsedName::ms_cUseTessellation;
VSUsedName VSUsedName::ms_cGPUMorphTarget;
VSUsedName VSUsedName::ms_cMaterialVertexFormat;
VSUsedName VSUsedName::ms_cVolumeVertexFormat;
VSUsedName VSUsedName::ms_cLightFunKey;
VSUsedName VSUsedName::ms_cMaterialLightKey;
VSUsedName VSUsedName::ms_cLighted;
VSUsedName VSUsedName::ms_cPointLightShadowNum;
VSUsedName VSUsedName::ms_cPointLightVolumeShadowNum;
VSUsedName VSUsedName::ms_cPointLightParaboloidShadowNum;
VSUsedName VSUsedName::ms_cDirectionLightShadowNum;
VSUsedName VSUsedName::ms_cDirectionLightVolumeShadowNum;
VSUsedName VSUsedName::ms_cDirectionLightCSMShadowNum;
VSUsedName VSUsedName::ms_cBlinnPhong;
VSUsedName VSUsedName::ms_cSimpleAnimInstanceLerp;
VSUsedName VSUsedName::ms_cOrenNayarLookUpTable;
VSUsedName VSUsedName::ms_cPreZShader;
VSUsedName VSUsedName::ms_cVolumeShadowShader;
VSUsedName VSUsedName::ms_cPostGray;
VSUsedName VSUsedName::ms_cGammaCorrect;
VSUsedName VSUsedName::ms_cPostScreenQuad;
VSUsedName VSUsedName::ms_cPostSaturation;
VSUsedName VSUsedName::ms_cOldPhoto;
VSUsedName VSUsedName::ms_cGuassBlurH;
VSUsedName VSUsedName::ms_cGuassBlurV;
VSUsedName VSUsedName::ms_cFilter3X3;
VSUsedName VSUsedName::ms_cBright;
VSUsedName VSUsedName::ms_cTexAdd;
VSUsedName VSUsedName::ms_cPostVolumeShadowMap;
VSUsedName VSUsedName::ms_cPostInputTexture[POSTEFFECT_MAX_INPUT];
VSUsedName VSUsedName::ms_cTexOffsetAndWeight;
VSUsedName VSUsedName::ms_cPostInv_Width;
VSUsedName VSUsedName::ms_cPostInv_Height;
VSUsedName VSUsedName::ms_cPassID;
VSUsedName VSUsedName::ms_cMainScene;
VSUsedName VSUsedName::ms_cCubShadowFov;
VSUsedName VSUsedName::ms_cSpotLightShadowNum;
VSUsedName VSUsedName::ms_cLightFunShader;
VSUsedName VSUsedName::ms_cPostEffectShader;
VSUsedName VSUsedName::ms_cHullShader;
VSUsedName VSUsedName::ms_cPositionInfo;
VSUsedName VSUsedName::ms_cClearColor;
VSUsedName VSUsedName::ms_cDepth;
VSUsedName VSUsedName::ms_cNULL;
VSUsedName VSUsedName::ms_cWorldViewProjectMatrix;
VSUsedName VSUsedName::ms_cWorldMatrix;
VSUsedName VSUsedName::ms_cProjectShadowMatrix;
VSUsedName VSUsedName::ms_cViewProjectMatrix;
IMPLEMENT_PRIORITY(VSUsedName)
IMPLEMENT_INITIAL_ONLY_BEGIN(VSUsedName)
ADD_PRIORITY(VSResourceManager)
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState);
IMPLEMENT_INITIAL_ONLY_END
bool VSUsedName::InitialDefaultState()
{
	ms_cPreZBeUsedBone = _T("PreZBeUsedBone");
	ms_cUseInstance = _T("UseInstance");
	ms_cUseTessellation = _T("UseTessellation");
	ms_cMaterialVertexFormat = _T("MaterialVertexFormat");
	ms_cLightFunKey= _T("LightFunKey");
	ms_cMaterialLightKey = _T("MaterialLightKey");
	ms_cGPUMorphTarget = _T("GPUMorphTarget");
	ms_cVolumeVertexFormat = _T("VolumeVertexFormat");

	ms_cLightFunShader = _T("LightFunShader");
	ms_cPostEffectShader = _T("PostEffectShader");
	ms_cVolumeShadowShader = _T("VolumeShadowShader");
	ms_cHullShader = _T("HullShader");

	ms_cPreZShader = _T("PreZShader");
	ms_cPostGray =_T("PostGray");
	ms_cGammaCorrect = _T("GammaCorrect");
	ms_cPostScreenQuad = _T("PostScreenQuad");
	ms_cPostSaturation = _T("PostSaturation");
	ms_cOldPhoto = _T("OldPhoto");
	ms_cGuassBlurH = _T("GuassBlurH");
	ms_cGuassBlurV = _T("GuassBlurV");
	ms_cFilter3X3 = _T("Filter3X3");
	ms_cBright = _T("Bright");
	ms_cTexAdd = _T("TexAdd");

	ms_cPostVolumeShadowMap = _T("PostVolumeShadowMap");
	ms_cPostInputTexture[0] = _T("PostInputTexture");
	ms_cPostInputTexture[1] = _T("PostInputTexture1");
	ms_cPostInputTexture[2] = _T("PostInputTexture2");
	ms_cPostInputTexture[3] = _T("PostInputTexture3");
	ms_cPostInv_Width = _T("PostInv_Width");
	ms_cPostInv_Height = _T("PostInv_Height");
	ms_cBlinnPhong = _T("BlinnPhone");
	ms_cSimpleAnimInstanceLerp = _T("SimpleAnimInstanceLerp");
	ms_cOrenNayarLookUpTable = _T("OrenNayarLookUpTable");
	ms_cTexOffsetAndWeight = _T("TexOffsetAndWeight");
	ms_cWorldViewProjectMatrix = _T("WorldViewProjectMatrix");
	ms_cViewProjectMatrix = _T("ViewProjectMatrix");
	ms_cProjectShadowMatrix = _T("ProjectShadowMatrix");
	ms_cWorldMatrix = _T("WorldMatrix");

	ms_cPassID = _T("PassID");
	ms_cMainScene = _T("Main");
	ms_cLighted = _T("bLighted");
	ms_cPointLightShadowNum = _T("PointLightShadowNum");
	ms_cPointLightVolumeShadowNum = _T("PointLightVolumeShadowNum");
	ms_cPointLightParaboloidShadowNum = _T("PointLightParaboloidShadowNum");
	ms_cDirectionLightShadowNum = _T("DirectionLightShadowNum");
	ms_cDirectionLightVolumeShadowNum = _T("DirectionLightVolumeShadowNum");
	ms_cDirectionLightCSMShadowNum = _T("DirectionLightCSMShadowNum");
	ms_cSpotLightShadowNum = _T("SpotLightShadowNum");

	ms_cCubShadowFov = _T("CubShadowFov");

	ms_cPositionInfo = _T("PositionInfo");
	ms_cClearColor = _T("ClearColor");
	ms_cDepth = _T("Depth");
	ms_cNULL = _T("");
	return 1;
}
bool VSUsedName::TerminalDefaultState()
{
	return 1;
}
void VSUsedName::Archive(VSStream & Stream)
{
	if (Stream.GetStreamFlag() == VSStream::AT_LOAD)
	{
		VSString String;
		Stream.ReadString(String);
		if (String == VSString::ms_StringNULL)
		{
			m_pName = NULL;
		}
		else
		{
			m_pName = VSResourceManager::CreateName(String);
		}
	}
	else if (Stream.GetStreamFlag() == VSStream::AT_SAVE)
	{
		if (m_pName)
		{
			Stream.WriteString(m_pName->GetString());
		}
		else
		{
			Stream.WriteString(VSString::ms_StringNULL);
		}
	}
	else if (Stream.GetStreamFlag() == VSStream::AT_SIZE)
	{
		if (m_pName)
		{

			Stream.AddBufferSize(VSStream::GetStrDistUse(m_pName->GetString()));
		}
		else
		{

			Stream.AddBufferSize(VSStream::GetStrDistUse(VSString::ms_StringNULL));
		}
	}
	
}
void VSUsedName::CopyFrom(VSCustomArchiveObject * pObject,VSMap<VSObject *,VSObject*>& CloneMap)
{
	VSUsedName * pUsedName = (VSUsedName *)pObject;
	*this = *pUsedName;
}
namespace VSEngine2
{
bool operator ==(const VSUsedName &Name1,const VSUsedName &Name2)
{
	if (!Name1.m_pName && !Name2.m_pName)
	{
		return 1;
	}
	else if (!Name1.m_pName || !Name2.m_pName)
	{
		return 0;
	}
	return Name1.m_pName->GetID() == Name2.m_pName->GetID();
}


bool operator !=(const VSUsedName &Name1,const VSUsedName &Name2)
{
	if (!Name1.m_pName && !Name2.m_pName)
	{
		return 0;
	}
	else if (!Name1.m_pName || !Name2.m_pName)
	{
		return 1;
	}
	return Name1.m_pName->GetID() != Name2.m_pName->GetID();
}

bool operator ==(const VSUsedName &Name,const VSString & String)
{
	if (!Name.m_pName)
	{
		return 0;
	}
	return Name.m_pName->GetString() == String;
}


bool operator !=(const VSUsedName &Name,const VSString & String)
{
	if (!Name.m_pName)
	{
		return 1;
	}
	return Name.m_pName->GetString() != String;
}

bool operator ==(const VSUsedName &Name,const TCHAR * pChar)
{
	if (!Name.m_pName && !pChar)
	{
		return 1;
	}
	else if (!Name.m_pName || !pChar)
	{
		return 0;
	}
	return Name.m_pName->GetString() == pChar;
}


bool operator !=(const VSUsedName &Name,const TCHAR * pChar)
{
	if (!Name.m_pName && !pChar)
	{
		return 0;
	}
	else if (!Name.m_pName || !pChar)
	{
		return 1;
	}
	return Name.m_pName->GetString() != pChar;
}

bool operator >(const VSUsedName &Name1,const VSUsedName &Name2)
{
	if (!Name1.m_pName && !Name2.m_pName)
	{
		return 0;
	}
	else if (Name1.m_pName && !Name2.m_pName)
	{
		return 1;
	}
	else if (!Name1.m_pName && Name2.m_pName)
	{
		return 0;
	}
	return Name1.m_pName->GetID() > Name2.m_pName->GetID();
}

bool operator <(const VSUsedName &Name1,const VSUsedName &Name2)
{
	if (!Name1.m_pName && !Name2.m_pName)
	{
		return 0;
	}
	else if (Name1.m_pName && !Name2.m_pName)
	{
		return 0;
	}
	else if (!Name1.m_pName && Name2.m_pName)
	{
		return 1;
	}
	return Name1.m_pName->GetID() < Name2.m_pName->GetID();
}
}