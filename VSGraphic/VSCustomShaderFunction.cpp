#include "VSCustomShaderFunction.h"
#include "VSShaderStringFactory.h"
#include "VSGraphicInclude.h"
#include "VSNormalFunction.h"
#include "VSLightShaderFunction.h"
#include "VSPosShaderFunction.h"
#include "VSCameraShaderFunction.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSCustomShaderFunction,VSShaderMainFunction)
BEGIN_ADD_PROPERTY(VSCustomShaderFunction,VSShaderMainFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSCustomShaderFunction)
IMPLEMENT_INITIAL_END
VSCustomShaderFunction::VSCustomShaderFunction(const VSUsedName &ShowName,VSMaterial * pMaterial)
:VSShaderMainFunction(ShowName,pMaterial)
{

	VSString InputName = _T("EmissiveColor");
	VSInputNode * pInputNode = NULL;

	pInputNode = VS_NEW VSInputNode(VSPutNode::VT_4,InputName,this);
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

	InputName = _T("Custom");
	pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::VT_4,InputName,this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);

	InputName = _T("Diffuse");
	pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::VT_4,InputName,this);
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

}
bool VSCustomShaderFunction::GetFunctionString(VSString &OutString, MaterialShaderPara &MSPara)const
{

	GetAlphaTestString(OutString,MSPara);
	if(!m_pInput[IN_CUSTOM]->GetOutputLink())
	{
		OutString +=  m_pOutput[OUT_COLOR]->GetNodeName().GetString() + _T(" = ") + VSRenderer::ms_pRenderer->Float4Const(_T("0"),_T("0"),_T("0"),_T("1"));

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


				VSRenderer::ms_pRenderer->CustomDirectionalLight(iLightNum[i],m_CustomDefine,ShadowStringArray[i],OutString);


			}
			else if(i == VSLight::LT_POINT)
			{


				VSRenderer::ms_pRenderer->CustomPointLight(iLightNum[i],m_CustomDefine,*VSShaderStringFactory::ms_WorldPos,ShadowStringArray[i],OutString);

			}
			else if(i == VSLight::LT_SPOT)
			{

				VSRenderer::ms_pRenderer->CustomSpotLight(iLightNum[i],m_CustomDefine,*VSShaderStringFactory::ms_WorldPos,ShadowStringArray[i],OutString);

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
VSCustomShaderFunction::~VSCustomShaderFunction()
{

}
VSCustomShaderFunction::VSCustomShaderFunction()
{
	
}
bool VSCustomShaderFunction::IsValidNodeToThis(VSShaderFunction * pShaderFunction, MaterialShaderPara &MSPara)
{

	//world offset node can't link view and project space node
	if (pShaderFunction->GetType().IsSameType(VSViewNormal::ms_Type)
		|| pShaderFunction->GetType().IsSameType(VSViewPos::ms_Type)
		|| pShaderFunction->GetType().IsSameType(VSViewWorldDir::ms_Type)
		|| pShaderFunction->GetType().IsSameType(VSProjectPos::ms_Type)
		|| pShaderFunction->GetType().IsSameType(VSLightDir::ms_Type) 
		|| pShaderFunction->GetType().IsSameType(VSLightColor::ms_Type) 
		|| pShaderFunction->GetType().IsSameType(VSLightSpecular::ms_Type))
	{
		if (GetWorldOffsetNode()->GetOutputLink())
		{
			VSShaderFunction * pOwner = (VSShaderFunction *)GetWorldOffsetNode()->GetOutputLink()->GetOwner();
			if (pOwner == pShaderFunction)
			{
				return false;
			}
			if (pOwner->HaveThisChild(pShaderFunction, MSPara) == true)
			{
				return false;
			}
		}
	}

	if (MSPara.uiCompileShaderType == VSEngineFlag::ST_PIXEL)
	{
		if (pShaderFunction->GetType().IsSameType(VSWorldNormal::ms_Type) || pShaderFunction->GetType().IsSameType(VSViewNormal::ms_Type))
		{
			if (GetNormalNode()->GetOutputLink())
			{
				VSShaderFunction * pOwner = (VSShaderFunction *)GetNormalNode()->GetOutputLink()->GetOwner();
				if (pOwner == pShaderFunction)
				{
					return false;
				}
				if (pOwner->HaveThisChild(pShaderFunction, MSPara) == true)
				{
					return false;
				}
			}
		}
	}
	return true;
}
void VSCustomShaderFunction::CreatLightFunctionString(MaterialShaderPara &MSPara)
{
	unsigned int uiOutputStringType = MSPara.uiPassType;
	if (uiOutputStringType != VSPass::PT_MATERIAL)
	{
		return ;
	}
	VSString CustomFunctionString;
	VSArray<VSShaderFunction *>  NoLightFunctionParentNodeArray;
	m_LightFunctionString.Clear();
	m_CustomContentString.Clear();
	m_CustomDefine.Clear();
	if (m_pInput[IN_CUSTOM]->GetOutputLink())
	{
		
		((VSShaderFunction *)m_pInput[IN_CUSTOM]->GetOutputLink()->GetOwner())->GetNoLightFunctionParentNode(NoLightFunctionParentNodeArray,MSPara);

		for (unsigned int i = 0 ; i < NoLightFunctionParentNodeArray.GetNum() ; i++)
		{
			NoLightFunctionParentNodeArray[i]->GetShaderTreeString(m_CustomContentString,MSPara);
		}
		

		((VSShaderFunction *)m_pInput[IN_CUSTOM]->GetOutputLink()->GetOwner())->GetShaderTreeString(CustomFunctionString,MSPara);
	}
	
	unsigned int uiCustomValueType = m_pInput[IN_CUSTOM]->GetValueType();
	VSString Temp;
	if(uiCustomValueType == VSPutNode::VT_4)
	{
		CustomFunctionString +=VSRenderer::ms_pRenderer->Float4() + _T(" ");/*_T("float4 ");*/
		Temp = VSRenderer::ms_pRenderer->Float4Const(_T("0"),_T("0"),_T("0"),_T("1"));/*_T("float4(0,0,0,1)");*/
	}
	else
	{
		VSMAC_ASSERT(0);
	}
	if(!m_pInput[IN_CUSTOM]->GetOutputLink())
	{
		CustomFunctionString += m_pInput[IN_CUSTOM]->GetNodeName().GetString() + _T(" = ") + Temp + _T(";\n");
	}
	else
	{
		CustomFunctionString += GetValueEqualString(m_pInput[IN_CUSTOM]->GetOutputLink(),m_pInput[IN_CUSTOM]);
	}

	VSString CustomDeclareString;
	for (unsigned int i = 0 ; i < NoLightFunctionParentNodeArray.GetNum() ; i++)
	{
		VSString NodeName = NoLightFunctionParentNodeArray[i]->GetOutputNode(0)->GetNodeName().GetString();
		unsigned int VTType = NoLightFunctionParentNodeArray[i]->GetOutputNode(0)->GetValueType();

		VSString TypeString;
		if(VTType == VSPutNode::VT_1)
		{
			TypeString +=VSRenderer::ms_pRenderer->Float() + _T(" ");/*_T("VSREAL ");*/
		}
		else if(VTType == VSPutNode::VT_2)
		{
			TypeString +=VSRenderer::ms_pRenderer->Float2() + _T(" ");/*_T("float2 ")*/;

		}
		else if(VTType == VSPutNode::VT_3)
		{
			TypeString +=VSRenderer::ms_pRenderer->Float3() + _T(" ");/*_T("float3 ");*/

		}
		else if(VTType == VSPutNode::VT_4)
		{
			TypeString +=VSRenderer::ms_pRenderer->Float4() + _T(" ");/*_T("float4 ");*/

		}
		CustomDeclareString += TypeString + NodeName + _T(","); 
		m_CustomDefine += NodeName + _T(","); 

	}
	VSString DirectionLightString;
	DirectionLightString = VSRenderer::ms_pRenderer->Float4() + _T(" CustomDirectionLightFun(") + CustomDeclareString + 
							VSRenderer::ms_pRenderer->Float4() + *VSShaderStringFactory::ms_LightColor + _T(",") +
							VSRenderer::ms_pRenderer->Float4() + *VSShaderStringFactory::ms_LightSpecular + _T(",") +
							VSRenderer::ms_pRenderer->Float3() + *VSShaderStringFactory::ms_LightDir + _T(")");
	DirectionLightString += _T("\n{\n") + CustomFunctionString + VSRenderer::ms_pRenderer->Return() + m_pInput[IN_CUSTOM]->GetNodeName().GetString() + _T(";}\n");

	VSString PointLightString;
	PointLightString = VSRenderer::ms_pRenderer->Float4() + _T(" CustomPointLightFun(") + CustomDeclareString + 
							VSRenderer::ms_pRenderer->Float3() + *VSShaderStringFactory::ms_WorldPos + _T(",") +
							VSRenderer::ms_pRenderer->Float4() + *VSShaderStringFactory::ms_LightColor + _T(",") +
							VSRenderer::ms_pRenderer->Float4() + *VSShaderStringFactory::ms_LightSpecular + _T(",") +
							VSRenderer::ms_pRenderer->Float() +_T("Range") + _T(",") +
							VSRenderer::ms_pRenderer->Float3() + _T("LightWorldPos") + _T(")");

	VSString  PointLightUseString ;
	VSString  PointAttenuationDivString ;
	VSRenderer::ms_pRenderer->CustomPointLightUseString(PointLightUseString,PointAttenuationDivString);
	PointLightString += _T("\n{\n") + PointLightUseString + CustomFunctionString + VSRenderer::ms_pRenderer->Return() + m_pInput[IN_CUSTOM]->GetNodeName().GetString() + PointAttenuationDivString + _T(";}\n");



	VSString SpotLightString;
	SpotLightString = VSRenderer::ms_pRenderer->Float4() + _T(" CustomSpotLightFun(") + CustomDeclareString + 
						VSRenderer::ms_pRenderer->Float3() + *VSShaderStringFactory::ms_WorldPos + _T(",") +
						VSRenderer::ms_pRenderer->Float4() + *VSShaderStringFactory::ms_LightColor + _T(",") +
						VSRenderer::ms_pRenderer->Float4() + *VSShaderStringFactory::ms_LightSpecular + _T(",") +
						VSRenderer::ms_pRenderer->Float() +_T("Range") + _T(",") +
						VSRenderer::ms_pRenderer->Float() +_T("Phi") + _T(",") +
						VSRenderer::ms_pRenderer->Float() +_T("Theta") + _T(",") +
						VSRenderer::ms_pRenderer->Float() +_T("Falloff") + _T(",") +
						VSRenderer::ms_pRenderer->Float3() +_T("LightWorldPos") + _T(",") +
						VSRenderer::ms_pRenderer->Float3() + _T("LightWorldDirection") + _T(")");
	VSString  SpotLightUseString ;
	VSString  SpotAttenuationDivString ;
	VSRenderer::ms_pRenderer->CustomSpotLightUseString(SpotLightUseString,SpotAttenuationDivString);
	SpotLightString += _T("\n{\n") + SpotLightUseString + CustomFunctionString + VSRenderer::ms_pRenderer->Return() + m_pInput[IN_CUSTOM]->GetNodeName().GetString() + SpotAttenuationDivString + _T(";}\n");

	m_LightFunctionString = DirectionLightString + PointLightString + SpotLightString;
	
}
bool VSCustomShaderFunction::GetInputValueString(VSString &OutString, MaterialShaderPara &MSPara)const
{
	VSMAC_ASSERT(VSRenderer::ms_pRenderer);
	VSString Temp;
	unsigned int uiOutputStringType = MSPara.uiPassType;
	if (uiOutputStringType == VSPass::PT_MATERIAL)
	{
		for(unsigned int i = 0 ; i < m_pInput.GetNum() ; i++)
		{

			if (m_pInput[i] == GetNormalNode() || i == IN_CUSTOM || m_pInput[i] == GetWorldOffsetNode())
			{
				continue;
			}
			if(m_pInput[i]->GetValueType() == VSPutNode::VT_1)
			{
				OutString +=VSRenderer::ms_pRenderer->Float() + _T(" ");/*_T("VSREAL ");*/
				if (m_pInput[i] == GetAlphaNode())
				{
					Temp = VSRenderer::ms_pRenderer->FloatConst(_T("1"));
				}
				else
				{
					Temp = VSRenderer::ms_pRenderer->FloatConst(_T("0"));
				}

			}
			else if(m_pInput[i]->GetValueType() == VSPutNode::VT_2)
			{
				OutString +=VSRenderer::ms_pRenderer->Float2() + _T(" ");/*_T("float2 ");*/
				Temp = VSRenderer::ms_pRenderer->Float2Const(_T("0"),_T("0"));/*_T("float2(0,0)");*/
			}
			else if(m_pInput[i]->GetValueType() == VSPutNode::VT_3)
			{
				OutString +=VSRenderer::ms_pRenderer->Float3() + _T(" ");/*_T("float3 ");*/
				Temp = VSRenderer::ms_pRenderer->Float3Const(_T("0"),_T("0"),_T("0"));/*_T("float3(0,0,0)");*/
			}
			else if(m_pInput[i]->GetValueType() == VSPutNode::VT_4)
			{
				OutString +=VSRenderer::ms_pRenderer->Float4() + _T(" ");/*_T("float4 ");*/
				Temp = VSRenderer::ms_pRenderer->Float4Const(_T("0"),_T("0"),_T("0"),_T("1"));/*_T("float4(0,0,0,1)");*/
			}
			else
				return 0;
			if(!m_pInput[i]->GetOutputLink())
			{
				OutString += m_pInput[i]->GetNodeName().GetString() + _T(" = ") + Temp + _T(";\n");
				continue;
			}
			// 		if(m_pInput[i]->GetValueType() != m_pInput[i]->GetOutputLink()->GetValueType())
			// 			return 0;
			OutString += GetValueEqualString(m_pInput[i]->GetOutputLink(),m_pInput[i]);/*m_pInput[i]->GetNodeName() + _T(" = ") + m_pInput[i]->GetOutputLink()->GetNodeName() + _T(";\n");*/
		}
	}
	else if (uiOutputStringType == VSPass::PT_INDIRECT)
	{

		GetEmissiveInputValueString(OutString,MSPara);
		GetAlphaInputValueString(OutString,MSPara);
		GetDiffuseInputValueString(OutString,MSPara);
	}
	else if (uiOutputStringType == VSPass::PT_NORMALDEPTH)
	{
		GetAlphaInputValueString(OutString,MSPara);
		GetReflectMipInputValueString(OutString,MSPara);
		GetReflectPowInputValueString(OutString,MSPara);
	}
	else if (uiOutputStringType == VSPass::PT_POINT_CUBE_SHADOW || uiOutputStringType == VSPass::PT_SHADOW || uiOutputStringType == VSPass::PT_DUAL_PARABOLOID_SHADOW)
	{
		GetAlphaInputValueString(OutString,MSPara);
	}
	return 1;
}
bool VSCustomShaderFunction::GetPShaderTreeString(VSString &OutString, MaterialShaderPara &MSPara)
{
	unsigned int uiOutputStringType = MSPara.uiPassType;
	if(m_bIsVisited == 1)
		return 1;
	else
	{
		m_bIsVisited = 1;

		
		if (uiOutputStringType == VSPass::PT_MATERIAL)
		{
			GetValueUseDeclareString(OutString, VUS_ALL,MSPara);
			// first get normal string
			GetNormalString(OutString,MSPara);
			GetWorldOffsetString(OutString, MSPara);
			GetValueUseString(OutString, VUS_ALL,MSPara);

			GetAlphaString(OutString,MSPara);
			
			GetEmissiveString(OutString,MSPara);
			if (m_pInput[IN_CUSTOM]->GetOutputLink())
			{
				OutString += m_CustomContentString;
			}
		}
		else if (uiOutputStringType == VSPass::PT_INDIRECT)
		{
			GetValueUseDeclareString(OutString, VUS_ALL,MSPara);
			// first get normal string
			GetNormalString(OutString,MSPara);
			GetWorldOffsetString(OutString, MSPara);
			GetValueUseString(OutString, VUS_ALL,MSPara);

			GetAlphaString(OutString,MSPara);

			GetEmissiveString(OutString,MSPara);
			GetDiffuseString(OutString,MSPara);
		}
		else if (uiOutputStringType == VSPass::PT_NORMALDEPTH)
		{

			GetValueUseDeclareString(OutString, VUS_ALL,MSPara);
			GetNormalString(OutString,MSPara);
			GetWorldOffsetString(OutString, MSPara);
			GetValueUseString(OutString, VUS_ALL,MSPara);
			GetAlphaString(OutString,MSPara);
			GetReflectMipString(OutString,MSPara);
			GetReflectPowString(OutString,MSPara);
		}
		else if (uiOutputStringType == VSPass::PT_POINT_CUBE_SHADOW || uiOutputStringType == VSPass::PT_SHADOW || uiOutputStringType == VSPass::PT_DUAL_PARABOLOID_SHADOW)
		{
			GetValueUseDeclareString(OutString, VUS_WORLD_POS,MSPara);
			GetWorldOffsetString(OutString, MSPara);
			GetValueUseString(OutString, VUS_WORLD_POS, MSPara);
			GetAlphaString(OutString,MSPara);
		}

		if(!GetInputValueString(OutString,MSPara))
			return 0;
		if(!GetOutputValueString(OutString,MSPara))
			return 0;
		if (uiOutputStringType == VSPass::PT_MATERIAL)
		{
			if(!GetFunctionString(OutString,MSPara))
				return 0;
		}
		else if (uiOutputStringType == VSPass::PT_INDIRECT)
		{
			if (!GetIndirectRenderString(OutString,MSPara))
			{
				return 0;
			}
		}
		else if (uiOutputStringType == VSPass::PT_NORMALDEPTH)
		{
			if (!GetNormalDepthString(OutString,MSPara))
			{
				return 0;
			}
		}
		else if (uiOutputStringType == VSPass::PT_POINT_CUBE_SHADOW)
		{
			if (!GetCubShadowString(OutString,MSPara))
			{
				return 0;
			}
		}
		else if (uiOutputStringType == VSPass::PT_SHADOW)
		{
			if (!GetShadowString(OutString,MSPara))
			{
				return 0;
			}
		}
		else if (uiOutputStringType == VSPass::PT_DUAL_PARABOLOID_SHADOW)
		{
			if (!GetSDualParaboloidhadowString(OutString,MSPara))
			{
				return 0;
			}
		}
		return 1;
	}
}
bool VSCustomShaderFunction::PostLoad(VSStream* pStream)
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