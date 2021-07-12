#include "VSPhoneShaderFunction.h"
#include "VSShaderStringFactory.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSPhoneShaderFunction,VSShaderMainFunction)
BEGIN_ADD_PROPERTY(VSPhoneShaderFunction,VSShaderMainFunction)
REGISTER_ENUM_PROPERTY(m_uiSpecularType, SpecularType, PhoneShaderFunctionSpecularType, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
BEGIN_ADD_ENUM
ADD_ENUM(PhoneShaderFunctionSpecularType,ST_BlinnPhong)
ADD_ENUM(PhoneShaderFunctionSpecularType, ST_Phong)
END_ADD_ENUM
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSPhoneShaderFunction)
IMPLEMENT_INITIAL_END
VSPhoneShaderFunction::VSPhoneShaderFunction(const VSUsedName &ShowName,VSMaterial * pMaterial)
:VSShaderMainFunction(ShowName,pMaterial)
{

	VSString InputName = _T("DiffuseColor");
	VSInputNode * pInputNode = NULL;

	pInputNode = VS_NEW VSInputNode(VSPutNode::VT_4,InputName,this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);

	InputName = _T("EmissiveColor");
	pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::VT_4,InputName,this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);

	InputName = _T("SpecularColor");
	pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::VT_4,InputName,this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);

	InputName = _T("SpecularPow");
	pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::VT_1,InputName,this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);

	InputName = _T("Normal");
	pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::VT_4,InputName,this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);


	InputName = _T("Alpha");
	pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::VT_1,InputName,this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);

	InputName = _T("ReflectMip");
	pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::VT_1,InputName,this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);

	InputName = _T("ReflectPow");
	pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::VT_1,InputName,this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);

	InputName = _T("WorldOffset");
	pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::VT_3, InputName, this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);

	InputName = _T("TessellationValue");
	pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::VT_1, InputName, this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);

	InputName = _T("WorldDisplacement");
	pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::VT_3, InputName, this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);

	VSString OutputName = _T("OutputColor");
	VSOutputNode * pOutputNode = NULL;
	pOutputNode = VS_NEW VSOutputNode(VSPutNode::VT_4,OutputName,this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);

	
	m_uiSpecularType = ST_BlinnPhong;
}

bool VSPhoneShaderFunction::GetFunctionString(VSString &OutString, MaterialShaderPara &MSPara)const
{
	GetAlphaTestString(OutString,MSPara);
	if(!m_pInput[IN_DIFFUSE_COLOR]->GetOutputLink() && !m_pInput[IN_SPECULAR_COLOR]->GetOutputLink())
	{
		OutString +=  m_pOutput[OUT_COLOR]->GetNodeName().GetString() + _T(" = ") + VSRenderer::ms_pRenderer->Float4Const(_T("0"),_T("0"),_T("0"),_T("1"));
		OutString += _T(";\n");
	}
	else
	{

		int iLightNum[VSLight::LT_MAX] = { 0 };	
		for (unsigned int i = 0 ; i < MSPara.LightArray.GetNum() ; i++)
		{
			if (MSPara.LightArray[i])
			{
				for (unsigned int j = 0 ; j < VSLight::LT_MAX ; j++)
				{
					if (MSPara.LightArray[i]->GetLightType() == j)
					{

						iLightNum[j]++;
					}
				}
			}
		}


		OutString += VSRenderer::ms_pRenderer->Float3() + _T("WorldCameraDir = ");
		VSRenderer::ms_pRenderer->ComputeDir(*VSShaderStringFactory::ms_CameraWorldPos,*VSShaderStringFactory::ms_WorldPos,OutString);
		OutString += _T(";\n");


		
		VSRenderer::ms_pRenderer->TranLightToTemp(MSPara.LightArray,OutString);
		VSRenderer::ms_pRenderer->GetLightFunction(MSPara.LightArray,*VSShaderStringFactory::ms_WorldPos,OutString);
		OutString +=  m_pOutput[OUT_COLOR]->GetNodeName().GetString() + _T(" = ") +
			VSRenderer::ms_pRenderer->Float4Const(_T("0"),_T("0"),_T("0"),_T("0")); 

		
		VSArray<VSString> ShadowStringArray[VSLight::LT_MAX];
		GetLightShadow(MSPara,ShadowStringArray);
		for (unsigned int i = 0 ; i < VSLight::LT_MAX ; i++)
		{
			if (!iLightNum[i])
			{
				continue;
			}
			if(i == VSLight::LT_DIRECTION)
			{			


				VSRenderer::ms_pRenderer->DirectionalLight(iLightNum[i],m_pInput[IN_DIFFUSE_COLOR]->GetNodeName().GetString()
					,m_pInput[IN_SPECULAR_COLOR]->GetNodeName().GetString(),m_pInput[IN_SPECULAR_POW]->GetNodeName().GetString(),*VSShaderStringFactory::ms_WorldNormal, _T("WorldCameraDir"),ShadowStringArray[i],OutString);


			}
			else if(i == VSLight::LT_POINT)
			{


				VSRenderer::ms_pRenderer->PointLight(iLightNum[i],m_pInput[IN_DIFFUSE_COLOR]->GetNodeName().GetString()
					,m_pInput[IN_SPECULAR_COLOR]->GetNodeName().GetString(),m_pInput[IN_SPECULAR_POW]->GetNodeName().GetString(),*VSShaderStringFactory::ms_WorldPos,*VSShaderStringFactory::ms_WorldNormal, _T("WorldCameraDir"),ShadowStringArray[i],OutString);

			}
			else if(i == VSLight::LT_SPOT)
			{

				VSRenderer::ms_pRenderer->SpotLight(iLightNum[i],m_pInput[IN_DIFFUSE_COLOR]->GetNodeName().GetString()
					,m_pInput[IN_SPECULAR_COLOR]->GetNodeName().GetString(),m_pInput[IN_SPECULAR_POW]->GetNodeName().GetString(),*VSShaderStringFactory::ms_WorldPos,*VSShaderStringFactory::ms_WorldNormal, _T("WorldCameraDir"),ShadowStringArray[i],OutString);

			}

		}
		OutString += _T(";\n");
	}

	GetSRGBWriteString(OutString,MSPara);

	VSString NodeStringA = VSRenderer::GetValueElement(m_pOutput[OUT_COLOR],VSRenderer::VE_A);
	OutString += NodeStringA + _T(" = ") + m_pInput[IN_ALPHA]->GetNodeName().GetString();
	OutString += _T(";\n");
	OutString +=  VSShaderStringFactory::ms_PSOutputColorValue + _T(" = ") + m_pOutput[OUT_COLOR]->GetNodeName().GetString() + _T(";\n");
	return 1;

}
VSPhoneShaderFunction::~VSPhoneShaderFunction()
{

}
VSPhoneShaderFunction::VSPhoneShaderFunction()
{
	m_uiSpecularType = ST_BlinnPhong;
}
bool VSPhoneShaderFunction::PostLoad(VSStream* pStream)
{
	if (!GetWorldOffsetNode())
	{
		VSString InputName = _T("WorldOffset");
		VSInputNode * pInputNode = NULL;
		pInputNode = VS_NEW VSInputNode(VSPutNode::VT_3, InputName, this);
		VSMAC_ASSERT(pInputNode);
		m_pInput.AddElement(pInputNode);
	}
	if (!GetTessellationValueNode())
	{
		VSString InputName = _T("TessellationValue");
		VSInputNode * pInputNode = NULL;
		pInputNode = VS_NEW VSInputNode(VSPutNode::VT_1, InputName, this);
		VSMAC_ASSERT(pInputNode);
		m_pInput.AddElement(pInputNode);
	}
	if (!GetWorldDisplacementNode())
	{
		VSString InputName = _T("WorldDisplacement");
		VSInputNode * pInputNode = NULL;
		pInputNode = VS_NEW VSInputNode(VSPutNode::VT_3, InputName, this);
		VSMAC_ASSERT(pInputNode);
		m_pInput.AddElement(pInputNode);
	}
	return true;
}