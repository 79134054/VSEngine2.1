#include "VSShaderMainFunction.h"
#include "VSShaderStringFactory.h"
#include "VSGraphicInclude.h"
#include "VSLight.h"
#include "VSPointLight.h"
#include "VSDirectionLight.h"
#include "VSNormalFunction.h"
#include "VSPosShaderFunction.h"
#include "VSNormalFunction.h"
#include "VSCameraShaderFunction.h"
#include "VSLightShaderFunction.h"
#include "VSSpotLight.h"
#include "VSColorBuffer.h"
using namespace VSEngine2;
IMPLEMENT_RTTI_NoCreateFun(VSShaderMainFunction,VSShaderFunction)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSShaderMainFunction)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(VSShaderMainFunction,VSShaderFunction)
REGISTER_PROPERTY(m_uiVESRGBWrite,VESRGBWrite,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_RenderState,RenderState,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY


VSShaderMainFunction::VSShaderMainFunction(const VSUsedName &ShowName,VSMaterial * pMaterial)
					:VSShaderFunction(ShowName,pMaterial)
{
	

	m_uiVESRGBWrite = false;
	m_fAlphaTestValue = 1.0f;
}

VSShaderMainFunction::~VSShaderMainFunction()
{

}
VSShaderMainFunction::VSShaderMainFunction()
{
	m_uiVESRGBWrite = false;
	m_fAlphaTestValue = 1.0f;
}
bool VSShaderMainFunction::GetVShaderTreeString(VSString &OutString, MaterialShaderPara &MSPara)
{
	unsigned int uiOutputStringType = MSPara.uiPassType;
	if (m_bIsVisited == 1)
		return 1;
	else
	{
		m_bIsVisited = 1;

		GetValueUseDeclareString(OutString, VUS_WORLD_POS | VUS_WORLD_NORMAL,MSPara);
		GetValueUseString(OutString, VUS_WORLD_POS | VUS_WORLD_NORMAL,MSPara);
		if (HasTessellation())
		{
			GetTessellationValueString(OutString,MSPara);
			GetTessellationValueInputValueString(OutString, MSPara);
			OutString += VSRenderer::ms_pRenderer->Float() + *VSShaderStringFactory::ms_TessellationValue + _T(" = ") + GetTessellationValueNode()->GetNodeName().GetString();
			OutString += _T(";\n");
		}
		return 1;
	}
}
bool VSShaderMainFunction::GetDShaderTreeString(VSString &OutString, MaterialShaderPara &MSPara)
{
	unsigned int uiOutputStringType = MSPara.uiPassType;
	if (m_bIsVisited == 1)
		return 1;
	else
	{
		m_bIsVisited = 1;
		GetValueUseDeclareString(OutString, VUS_WORLD_POS | VUS_WORLD_NORMAL, MSPara);
		GetValueUseString(OutString, VUS_WORLD_POS | VUS_WORLD_NORMAL, MSPara);		
		return 1;
	}
}
bool VSShaderMainFunction::GetPShaderTreeString(VSString &OutString,MaterialShaderPara &MSPara)
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
			GetValueUseString(OutString, VUS_ALL,MSPara);
			// second get other
			for(unsigned int i = 0 ; i < m_pInput.GetNum(); i++)
			{
				if(m_pInput[i]->GetOutputLink() == NULL)
					continue;
				else if (m_pInput[i] == GetNormalNode() || m_pInput[i] == GetWorldOffsetNode())
				{
					continue;
				}
				else
				{
					((VSShaderFunction *)m_pInput[i]->GetOutputLink()->GetOwner())->GetShaderTreeString(OutString,MSPara);

				}

			}
		}
		else if (uiOutputStringType == VSPass::PT_INDIRECT)
		{
			GetValueUseDeclareString(OutString, VUS_ALL,MSPara);
			GetNormalString(OutString,MSPara);
			GetValueUseString(OutString, VUS_ALL,MSPara);

			GetEmissiveString(OutString,MSPara);

			GetAlphaString(OutString,MSPara);
			GetDiffuseString(OutString,MSPara);


		}
		else if (uiOutputStringType == VSPass::PT_NORMALDEPTH)
		{
			GetValueUseDeclareString(OutString, VUS_ALL,MSPara);
			GetNormalString(OutString,MSPara);

			GetValueUseString(OutString, VUS_ALL,MSPara);
			GetAlphaString(OutString,MSPara);
			GetReflectMipString(OutString,MSPara);
			GetReflectPowString(OutString,MSPara);
		}
		else if (uiOutputStringType == VSPass::PT_POINT_CUBE_SHADOW || uiOutputStringType == VSPass::PT_DUAL_PARABOLOID_SHADOW)
		{
			GetValueUseDeclareString(OutString, VUS_WORLD_POS | VUS_WORLD_NORMAL,MSPara);
			GetNormalString(OutString,MSPara);

			GetValueUseString(OutString, VUS_WORLD_POS | VUS_WORLD_NORMAL,MSPara);
			GetAlphaString(OutString,MSPara);
		}
		else if (uiOutputStringType == VSPass::PT_SHADOW)
		{
			GetValueUseDeclareString(OutString, VUS_WORLD_POS | VUS_WORLD_NORMAL,MSPara);
			GetNormalString(OutString,MSPara);

			GetValueUseString(OutString, VUS_WORLD_POS | VUS_WORLD_NORMAL,MSPara);
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
void VSShaderMainFunction::GetAlphaTestString(VSString &OutString, MaterialShaderPara &MSPara)const
{
	if (m_fAlphaTestValue < 1.0f)
	{
		VSString AlphaName = GetAlphaNode()->GetNodeName().GetString();
		VSString AlphaTestValue = RealToString(m_fAlphaTestValue);

		VSRenderer::ms_pRenderer->Clip(AlphaName + _T(" - ") + AlphaTestValue, OutString);
		OutString += _T(";\n");

	}
}
void VSShaderMainFunction::GetSRGBWriteString(VSString &OutString, MaterialShaderPara &MSPara)const
{
	if (m_uiVESRGBWrite)
	{
		VSString NodeString = VSRenderer::GetValueElement(m_pOutput[OUT_COLOR], m_uiVESRGBWrite);

		OutString += NodeString + _T(" = ");
		VSRenderer::ms_pRenderer->GreaterZeroPow(NodeString, 1.0f / 2.2f, OutString);
		OutString += _T(";\n");
	}
}
bool VSShaderMainFunction::GetIndirectRenderString(VSString &OutString, MaterialShaderPara &MSPara)const
{
	
	GetAlphaTestString(OutString,MSPara);
	OutString +=  m_pOutput[OUT_COLOR]->GetNodeName().GetString() + _T(" = ") +
		GetEmissiveNode()->GetNodeName().GetString();

	VSRenderer::ms_pRenderer->SkyLight(*VSShaderStringFactory::ms_WorldNormal,*VSShaderStringFactory::ms_SkyLightUpColor,*VSShaderStringFactory::ms_SkyLightDownColor,GetDiffuseNode()->GetNodeName().GetString(),OutString);
	OutString += _T(";\n");

	GetSRGBWriteString(OutString,MSPara);

	VSString NodeStringA = VSRenderer::GetValueElement(m_pOutput[OUT_COLOR],VSRenderer::VE_A);
	OutString += NodeStringA + _T(" = ") + GetAlphaNode()->GetNodeName().GetString();
	OutString += _T(";\n");
	OutString +=  VSShaderStringFactory::ms_PSOutputColorValue + _T(" = ") + m_pOutput[OUT_COLOR]->GetNodeName().GetString() + _T(";\n");
	return true;
}
void VSShaderMainFunction::GetValueUseDeclareString(VSString &OutString, unsigned int uiValueUseString, MaterialShaderPara &MSPara)
{

	VSString DefaultValue = VSRenderer::ms_pRenderer->Float3Const(_T("0"), _T("0"), _T("0"));
	if ((uiValueUseString & VUS_WORLD_POS) == VUS_WORLD_POS)
		OutString += VSRenderer::ms_pRenderer->Float3() + *VSShaderStringFactory::ms_WorldPos + _T(" = ") + DefaultValue + _T(";\n");
	if ((uiValueUseString & VUS_VIEW_POS) == VUS_VIEW_POS)
		OutString += VSRenderer::ms_pRenderer->Float3() + *VSShaderStringFactory::ms_ViewPos + _T(" = ") + DefaultValue + _T(";\n");
	if ((uiValueUseString & VUS_PROJ_POS) == VUS_PROJ_POS)
		OutString += VSRenderer::ms_pRenderer->Float3() + *VSShaderStringFactory::ms_ProjectPos + _T(" = ") + DefaultValue + _T(";\n");

	DefaultValue = VSRenderer::ms_pRenderer->Float3Const(_T("0"), _T("1"), _T("0"));
	if ((uiValueUseString & VUS_WORLD_NORMAL) == VUS_WORLD_NORMAL)
	{
		OutString += VSRenderer::ms_pRenderer->Float3() + *VSShaderStringFactory::ms_WorldNormal + _T(" = ") + DefaultValue + _T(";\n");
		OutString += VSRenderer::ms_pRenderer->Float3() + *VSShaderStringFactory::ms_VertexWorldNormal + _T(" = ") + DefaultValue + _T(";\n");
	}
	if ((uiValueUseString & VUS_VIEW_NORMAL) == VUS_VIEW_NORMAL)
	{
		OutString += VSRenderer::ms_pRenderer->Float3() + *VSShaderStringFactory::ms_ViewNormal + _T(" = ") + DefaultValue + _T(";\n");
		OutString += VSRenderer::ms_pRenderer->Float3() + *VSShaderStringFactory::ms_VertexViewNormal + _T(" = ") + DefaultValue + _T(";\n");
	}


	if ((uiValueUseString & VUS_VIEW_WORLD_DIR) == VUS_VIEW_WORLD_DIR)
		OutString += VSRenderer::ms_pRenderer->Float3() + *VSShaderStringFactory::ms_ViewWorldDir + _T(" = ") + DefaultValue + _T(";\n");

}
bool VSShaderMainFunction::GetSDualParaboloidhadowString(VSString &OutString, MaterialShaderPara &MSPara)const
{
	GetAlphaTestString(OutString,MSPara);

	VSString NodeStringR = VSRenderer::GetValueElement(m_pOutput[OUT_COLOR],VSRenderer::VE_R);
	OutString += NodeStringR + _T(" = ") + VSShaderStringFactory::ms_InputProjectZ;
	OutString += _T(";\n");
	
	OutString +=  VSShaderStringFactory::ms_PSOutputColorValue + _T(" = ") + m_pOutput[OUT_COLOR]->GetNodeName().GetString() + _T(";\n");
	return true;	
}
bool VSShaderMainFunction::GetShadowString(VSString &OutString, MaterialShaderPara &MSPara)const
{

	GetAlphaTestString(OutString,MSPara);

	OutString += VSRenderer::ms_pRenderer->Float3() + _T("LightProj = ");

	VSRenderer::ms_pRenderer->TransProjPos(*VSShaderStringFactory::ms_WorldPos, *VSShaderStringFactory::ms_LightShadowMatrix, OutString);

	OutString += _T(";\n");
	VSString NodeStringR = VSRenderer::GetValueElement(m_pOutput[OUT_COLOR], VSRenderer::VE_R);
	OutString += NodeStringR + _T(" = ") + VSRenderer::GetValueElement(_T("LightProj"), VSRenderer::VE_B);
	OutString += _T(";\n");

	OutString +=  VSShaderStringFactory::ms_PSOutputColorValue + _T(" = ") + m_pOutput[OUT_COLOR]->GetNodeName().GetString() + _T(";\n");
	return true;
}
bool VSShaderMainFunction::GetCubShadowString(VSString &OutString, MaterialShaderPara &MSPara)const
{


	GetAlphaTestString(OutString,MSPara);

	OutString += VSRenderer::ms_pRenderer->Float() + _T("WorldCameraLength = ");
	VSRenderer::ms_pRenderer->ComputeLength(*VSShaderStringFactory::ms_CameraWorldPos,*VSShaderStringFactory::ms_WorldPos,OutString);
	OutString += _T(";\n");

	VSString NodeStringR = VSRenderer::GetValueElement(m_pOutput[OUT_COLOR],VSRenderer::VE_R);
	VSString SaturateString;
	VSRenderer::ms_pRenderer->Saturate(_T("WorldCameraLength /") + *VSShaderStringFactory::ms_PointLightRange,SaturateString);
	OutString += NodeStringR + _T(" = ") + SaturateString;
	OutString += _T(";\n");


	

	OutString +=  VSShaderStringFactory::ms_PSOutputColorValue + _T(" = ") + m_pOutput[OUT_COLOR]->GetNodeName().GetString() + _T(";\n");

	
	return true;
}
bool VSShaderMainFunction::GetNormalDepthString(VSString &OutString, MaterialShaderPara &MSPara)const
{
	GetAlphaTestString(OutString,MSPara);
	VSString NodeStringRG = VSRenderer::GetValueElement(m_pOutput[OUT_COLOR],VSRenderer::VE_R | VSRenderer::VE_G);
	OutString +=  NodeStringRG + _T(" = ");
	VSRenderer::ms_pRenderer->DecodeNormal3(*VSShaderStringFactory::ms_ViewNormal,OutString);
	OutString += _T(";\n");

	VSString NodeStringB = VSRenderer::GetValueElement(m_pOutput[OUT_COLOR],VSRenderer::VE_B);
	VSString StringViewZ = VSRenderer::GetValueElement(*VSShaderStringFactory::ms_ViewPos, VSRenderer::VE_B);
	VSString SaturateString;
	VSRenderer::ms_pRenderer->Saturate(StringViewZ + _T("/") + *VSShaderStringFactory::ms_FarZ, SaturateString);
	OutString += NodeStringB + _T(" = ") + SaturateString;
	OutString += _T(";\n");

	VSString NodeStringA = VSRenderer::GetValueElement(m_pOutput[OUT_COLOR],VSRenderer::VE_A);
	OutString +=  NodeStringA + _T(" = ");
	VSRenderer::ms_pRenderer->EncodeReflect(GetReflectMipNode()->GetNodeName().GetString(),GetReflectPowNode()->GetNodeName().GetString(),OutString);
	OutString += _T(";\n");

	OutString +=  VSShaderStringFactory::ms_PSOutputColorValue + _T(" = ") + m_pOutput[OUT_COLOR]->GetNodeName().GetString() + _T(";\n");

	return true;
}
bool VSShaderMainFunction::GetInputValueString(VSString &OutString, MaterialShaderPara &MSPara)const
{
	unsigned int uiOutputStringType = MSPara.uiPassType;
	if(!VSRenderer::ms_pRenderer)
		return 0;
	VSString Temp;
	if (uiOutputStringType == VSPass::PT_MATERIAL)
	{
		for(unsigned int i = 0 ; i < m_pInput.GetNum() ; i++)
		{
			if (m_pInput[i] == GetNormalNode() || m_pInput[i] == GetWorldOffsetNode())
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
void VSShaderMainFunction::GetLightShadow(MaterialShaderPara& MSPara,VSArray<VSString> ShadowStringArray[VSLight::LT_MAX])const
{
	VSArray<VSLight *> pLightArray = MSPara.LightArray;
	unsigned int uiShadowNum = 0;
	unsigned int uiLightIndex[VSLight::LT_MAX] = { 0 };	
	for (unsigned int i = 0 ; i< pLightArray.GetNum() ; i++)
	{
		VSString ShadowString;
		VSLocalLight * pLocalLight = DynamicCast<VSLocalLight>(pLightArray[i]);
		if (pLocalLight && pLocalLight->GetCastShadow())
		{
			if (pLocalLight->GetLightType() == VSLight::LT_POINT)
			{
				if( ((VSPointLight*)pLocalLight)->GetShadowType() == VSPointLight::ST_CUB )
				{
					ShadowString = _T(" * ");
					if (MSPara.pCamera->GetFov() <= AngleToRadian(90.0f))
					{
						VSRenderer::ms_pRenderer->PointLightCubeShadow(*VSShaderStringFactory::ms_LightName[VSLight::LT_POINT] + _T("[") + IntToString(uiLightIndex[VSLight::LT_POINT]) + _T("]"),
							*VSShaderStringFactory::ms_PSConstantShadowResource[uiShadowNum],*VSShaderStringFactory::ms_ViewMatrix,*VSShaderStringFactory::ms_WorldPos,ShadowString);
					}
					else
					{
						VSRenderer::ms_pRenderer->PointLightCubeShadow(*VSShaderStringFactory::ms_LightName[VSLight::LT_POINT] + _T("[") + IntToString(uiLightIndex[VSLight::LT_POINT]) + _T("]"),
							*VSShaderStringFactory::ms_PSConstantShadowResource[uiShadowNum],*VSShaderStringFactory::ms_WorldPos,ShadowString);
					}
					
					uiShadowNum++;
				}
				else if (((VSPointLight*)pLocalLight)->GetShadowType() == VSPointLight::ST_VOLUME)
				{
					ShadowString = _T(" * ");
					VSRenderer::ms_pRenderer->PointLightVolumeShadow(*VSShaderStringFactory::ms_LightName[VSLight::LT_POINT] + _T("[") + IntToString(uiLightIndex[VSLight::LT_POINT]) + _T("]"),
						*VSShaderStringFactory::ms_PSConstantShadowResource[uiShadowNum],*VSShaderStringFactory::ms_WorldPos,ShadowString);
					uiShadowNum++;
				}
				else if (((VSPointLight*)pLocalLight)->GetShadowType() == VSPointLight::ST_DUAL_PARABOLOID)
				{
					ShadowString = _T(" * ");
					unsigned int uiIndex = uiShadowNum;
					VSRenderer::ms_pRenderer->PointLightDualParaboloidShadow(*VSShaderStringFactory::ms_LightName[VSLight::LT_POINT] + _T("[") + IntToString(uiLightIndex[VSLight::LT_POINT]) + _T("]"),
						*VSShaderStringFactory::ms_PSConstantShadowResource[uiIndex],*VSShaderStringFactory::ms_WorldPos,ShadowString);
					uiShadowNum++;
				}
			}
			else if (pLocalLight->GetLightType() == VSLight::LT_DIRECTION)
			{	
				if (((VSDirectionLight*)pLocalLight)->GetShadowType() == VSDirectionLight::ST_VOLUME)
				{
					ShadowString = _T(" * ");
					VSRenderer::ms_pRenderer->DirectionLightVolumeShadow(*VSShaderStringFactory::ms_LightName[VSLight::LT_DIRECTION] + _T("[") + IntToString(uiLightIndex[VSLight::LT_DIRECTION]) + _T("]"),
						*VSShaderStringFactory::ms_PSConstantShadowResource[uiShadowNum],*VSShaderStringFactory::ms_WorldPos,ShadowString);
					uiShadowNum++;
				}
				else if (((VSDirectionLight*)pLocalLight)->GetShadowType() == VSDirectionLight::ST_CSM)
				{
					ShadowString = _T(" * ");
					unsigned int uiIndex = uiShadowNum;
					VSRenderer::ms_pRenderer->DirectionLightCSMShadow(*VSShaderStringFactory::ms_LightName[VSLight::LT_DIRECTION] + _T("[") + IntToString(uiLightIndex[VSLight::LT_DIRECTION]) + _T("]"),
						*VSShaderStringFactory::ms_PSConstantShadowResource[uiIndex],
						*VSShaderStringFactory::ms_WorldPos,*VSShaderStringFactory::ms_ViewPos,*VSShaderStringFactory::ms_FarZ,ShadowString);
					uiShadowNum++;
				}
				else if (((VSDirectionLight*)pLocalLight)->GetShadowType() == VSDirectionLight::ST_PROJECT)
				{
				}
				else
				{
					ShadowString = _T(" * ");
					VSRenderer::ms_pRenderer->DirectionLightShadow(*VSShaderStringFactory::ms_LightName[VSLight::LT_DIRECTION] + _T("[") + IntToString(uiLightIndex[VSLight::LT_DIRECTION]) + _T("]"),
						*VSShaderStringFactory::ms_PSConstantShadowResource[uiShadowNum],*VSShaderStringFactory::ms_WorldPos,ShadowString);
					uiShadowNum++;
				}

			}
			else if (pLocalLight->GetLightType() == VSLight::LT_SPOT)
			{	
				if (((VSSpotLight*)pLocalLight)->GetShadowType() == VSSpotLight::ST_NORMAL)
				{
					// use the same to dirlightshadow
					ShadowString = _T(" * ");
					VSRenderer::ms_pRenderer->SpotLightShadow(*VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + IntToString(uiLightIndex[VSLight::LT_SPOT]) + _T("]"),
						*VSShaderStringFactory::ms_PSConstantShadowResource[uiShadowNum],*VSShaderStringFactory::ms_WorldPos,ShadowString);
					uiShadowNum++;
				}
			}
			
		}

		ShadowStringArray[pLightArray[i]->GetLightType()].AddElement(ShadowString);

		uiLightIndex[pLightArray[i]->GetLightType()]++;
	}
}
bool VSShaderMainFunction::IsValidNodeToThis(VSShaderFunction * pShaderFunction, MaterialShaderPara &MSPara)
{
	

	//world offset node can't link view and project space node
	if (pShaderFunction->GetType().IsSameType(VSViewNormal::ms_Type) 
		|| pShaderFunction->GetType().IsSameType(VSViewPos::ms_Type)
		|| pShaderFunction->GetType().IsSameType(VSViewWorldDir::ms_Type)
		|| pShaderFunction->GetType().IsSameType(VSProjectPos::ms_Type))
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
		//normal node can't link normal node
		if (pShaderFunction->GetType().IsSameType(VSWorldNormal::ms_Type)
			|| pShaderFunction->GetType().IsSameType(VSViewNormal::ms_Type))
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
	

	
	// can't link custom light node
	if (pShaderFunction->GetType().IsSameType(VSLightDir::ms_Type) || pShaderFunction->GetType().IsSameType(VSLightColor::ms_Type) || pShaderFunction->GetType().IsSameType(VSLightSpecular::ms_Type))
	{
		return false;
	}
	//can't link GBuffer node
	if (pShaderFunction->GetType().IsSameType(VSColorBuffer::ms_Type))
	{
		return false;
	}
	return true;
}
void VSShaderMainFunction::GetValueUseString(VSString &OutString, unsigned int uiValueUseString, MaterialShaderPara &MSPara)
{
	if ((uiValueUseString & VUS_WORLD_NORMAL) == VUS_WORLD_NORMAL)
	{
		if (MSPara.uiCompileShaderType == VSEngineFlag::ST_VERTEX || 
			MSPara.uiCompileShaderType == VSEngineFlag::ST_DOMAIN)
		{
			if (VSShaderStringFactory::ms_InputLocalNormal.GetLength())
			{
				OutString += *VSShaderStringFactory::ms_VertexWorldNormal + _T(" = ");

				if (MSPara.pGeometry->IsSameType(VSInstanceGeometry::ms_Type))
				{
					VSRenderer::ms_pRenderer->BoneTranNormal(VSShaderStringFactory::ms_InputLocalNormal,
						VSShaderStringFactory::ms_InputInstancePos[0], VSShaderStringFactory::ms_InputInstancePos[1],
						VSShaderStringFactory::ms_InputInstancePos[2], OutString);
				}
				else
				{
					VSRenderer::ms_pRenderer->LocalToWorldNormal(VSShaderStringFactory::ms_InputLocalNormal, OutString);
				}

				OutString += _T(";\n");
			}
		}
		else if (MSPara.uiCompileShaderType == VSEngineFlag::ST_PIXEL)
		{
			if (VSShaderStringFactory::ms_InputLocalNormal.GetLength() > 0)
			{
				// add veretex normal
				OutString += *VSShaderStringFactory::ms_VertexWorldNormal + _T(" = ");
				if (MSPara.pGeometry->IsSameType(VSInstanceGeometry::ms_Type))
				{
					VSRenderer::ms_pRenderer->BoneTranNormal(VSShaderStringFactory::ms_InputLocalNormal,
						VSShaderStringFactory::ms_InputInstancePos[0], VSShaderStringFactory::ms_InputInstancePos[1],
						VSShaderStringFactory::ms_InputInstancePos[2], OutString);
				}
				else
				{
					VSRenderer::ms_pRenderer->LocalToWorldNormal(VSShaderStringFactory::ms_InputLocalNormal, OutString);
				}
				OutString += _T(";\n");

				OutString += *VSShaderStringFactory::ms_WorldNormal + _T(" = ") +
					*VSShaderStringFactory::ms_VertexWorldNormal + _T(";\n");

				// add pixel normal
				if (GetNormalNode()->GetOutputLink() && VSShaderStringFactory::ms_InputLocalTangent.GetLength() > 0)
				{
					GetNormalInputValueString(OutString, MSPara);
					OutString += VSRenderer::ms_pRenderer->Float3() + _T("LocalNormal = ");
					VSRenderer::ms_pRenderer->BumpNormal(GetNormalNode()->GetNodeName().GetString(), OutString);
					OutString += _T(";\n");

					OutString += *VSShaderStringFactory::ms_WorldNormal + _T(" = ");
					if (MSPara.pGeometry->IsSameType(VSInstanceGeometry::ms_Type))
					{
						VSRenderer::ms_pRenderer->BoneTranNormal(_T("LocalNormal"),
							VSShaderStringFactory::ms_InputInstancePos[0], VSShaderStringFactory::ms_InputInstancePos[1],
							VSShaderStringFactory::ms_InputInstancePos[2], OutString);
					}
					else
					{
						VSRenderer::ms_pRenderer->LocalToWorldNormal(_T("LocalNormal"), OutString);
					}
					OutString += _T(";\n");
				}
			}
		}
		
	}
	if ((uiValueUseString & VUS_VIEW_NORMAL) == VUS_VIEW_NORMAL)
	{

		OutString += *VSShaderStringFactory::ms_VertexViewNormal + _T(" = ");
		VSRenderer::ms_pRenderer->WorldToViewNormal(*VSShaderStringFactory::ms_VertexWorldNormal, OutString);
		OutString += _T(";\n");

		OutString += *VSShaderStringFactory::ms_ViewNormal + _T(" = ");
		VSRenderer::ms_pRenderer->WorldToViewNormal(*VSShaderStringFactory::ms_WorldNormal, OutString);
		OutString += _T(";\n");
	}


	if ((uiValueUseString & VUS_WORLD_POS) == VUS_WORLD_POS)
	{
		

		if (MSPara.uiCompileShaderType == VSEngineFlag::ST_VERTEX)
		{
			OutString += *VSShaderStringFactory::ms_WorldPos + _T(" = ");
			if (MSPara.pGeometry->IsSameType(VSInstanceGeometry::ms_Type))
			{
				VSRenderer::ms_pRenderer->BoneTranPos(VSShaderStringFactory::ms_InputLocalPos,
					VSShaderStringFactory::ms_InputInstancePos[0], VSShaderStringFactory::ms_InputInstancePos[1],
					VSShaderStringFactory::ms_InputInstancePos[2], OutString);
			}
			else
			{
				VSRenderer::ms_pRenderer->LocalToWorldPos(VSShaderStringFactory::ms_InputLocalPos, OutString);
			}

			OutString += _T(";\n");
			GetWorldOffsetString(OutString, MSPara);
			GetWorldOffsetInputValueString(OutString, MSPara);
			OutString += *VSShaderStringFactory::ms_WorldPos + _T(" += ") + GetWorldOffsetNode()->GetNodeName().GetString();
			OutString += _T(";\n");
		}
		else if (MSPara.uiCompileShaderType == VSEngineFlag::ST_PIXEL)
		{
			OutString += *VSShaderStringFactory::ms_WorldPos + _T(" = ") + VSShaderStringFactory::ms_InputWorldPos + _T(";\n");;
		}
		else if (MSPara.uiCompileShaderType == VSEngineFlag::ST_DOMAIN)
		{
			GetWorldDisplacementString(OutString, MSPara);
			GetWorldDisplacementInputValueString(OutString, MSPara);
			OutString += *VSShaderStringFactory::ms_WorldPos + _T(" = ") + VSShaderStringFactory::ms_InputWorldPos + _T(" + ") + GetWorldDisplacementNode()->GetNodeName().GetString();
			OutString += _T(";\n");
		}
	}

	if ((uiValueUseString & VUS_VIEW_POS) == VUS_VIEW_POS)
	{
		OutString += *VSShaderStringFactory::ms_ViewPos + _T(" = ");
		VSRenderer::ms_pRenderer->WorldToViewPos(*VSShaderStringFactory::ms_WorldPos, OutString);
		OutString += _T(";\n");
	}

	if ((uiValueUseString & VUS_PROJ_POS) == VUS_PROJ_POS)
	{
		OutString += *VSShaderStringFactory::ms_ProjectPos + _T(" = ");
		VSRenderer::ms_pRenderer->TransProjPos(*VSShaderStringFactory::ms_ViewPos, *VSShaderStringFactory::ms_ProjectMatrix, OutString);
		OutString += _T(";\n");
	}


	if ((uiValueUseString & VUS_VIEW_WORLD_DIR) == VUS_VIEW_WORLD_DIR)
	{
		VSString ViewWorldDir;
		VSRenderer::ms_pRenderer->GetWorldViewDir(ViewWorldDir);
		OutString += *VSShaderStringFactory::ms_ViewWorldDir + _T(" = ") + ViewWorldDir;
		OutString += _T(";\n");
	}

}
void VSShaderMainFunction::GetNormalString(VSString &OutString, MaterialShaderPara &MSPara)const
{
	if (GetNormalNode()->GetOutputLink())
	{
		((VSShaderFunction *)GetNormalNode()->GetOutputLink()->GetOwner())->GetShaderTreeString(OutString,MSPara);
	}
}
void VSShaderMainFunction::GetWorldOffsetString(VSString &OutString, MaterialShaderPara &MSPara)const
{
	if (GetWorldOffsetNode()->GetOutputLink())
	{
		((VSShaderFunction *)GetWorldOffsetNode()->GetOutputLink()->GetOwner())->GetShaderTreeString(OutString,MSPara);
	}
}
void VSShaderMainFunction::GetAlphaString(VSString &OutString, MaterialShaderPara &MSPara)const
{
	if (GetAlphaNode()->GetOutputLink())
	{
		((VSShaderFunction *)GetAlphaNode()->GetOutputLink()->GetOwner())->GetShaderTreeString(OutString,MSPara);
	}
}
void VSShaderMainFunction::GetEmissiveString(VSString &OutString, MaterialShaderPara &MSPara)const
{
	if (GetEmissiveNode()->GetOutputLink())
	{
		((VSShaderFunction *)GetEmissiveNode()->GetOutputLink()->GetOwner())->GetShaderTreeString(OutString,MSPara);
	}
}
void VSShaderMainFunction::GetDiffuseString(VSString &OutString, MaterialShaderPara &MSPara)const
{
	if (GetDiffuseNode()->GetOutputLink())
	{
		((VSShaderFunction *)GetDiffuseNode()->GetOutputLink()->GetOwner())->GetShaderTreeString(OutString,MSPara);
	}
}
void VSShaderMainFunction::GetReflectMipString(VSString &OutString, MaterialShaderPara &MSPara)const
{
	if (GetReflectMipNode()->GetOutputLink())
	{
		((VSShaderFunction *)GetReflectMipNode()->GetOutputLink()->GetOwner())->GetShaderTreeString(OutString,MSPara);
	}
}
void VSShaderMainFunction::GetReflectPowString(VSString &OutString, MaterialShaderPara &MSPara)const
{
	if (GetReflectPowNode()->GetOutputLink())
	{
		((VSShaderFunction *)GetReflectPowNode()->GetOutputLink()->GetOwner())->GetShaderTreeString(OutString,MSPara);
	}
}
void VSShaderMainFunction::GetTessellationValueString(VSString &OutString, MaterialShaderPara &MSPara)const
{
	if (GetTessellationValueNode()->GetOutputLink())
	{
		((VSShaderFunction *)GetTessellationValueNode()->GetOutputLink()->GetOwner())->GetShaderTreeString(OutString, MSPara);
	}
}
void VSShaderMainFunction::GetWorldDisplacementString(VSString &OutString, MaterialShaderPara &MSPara)const
{
	if (GetWorldDisplacementNode()->GetOutputLink())
	{
		((VSShaderFunction *)GetWorldDisplacementNode()->GetOutputLink()->GetOwner())->GetShaderTreeString(OutString, MSPara);
	}
}
bool VSShaderMainFunction::GetTessellationValueInputValueString(VSString &OutString, MaterialShaderPara &MSPara)const
{
	VSString Temp;
	unsigned int uiTessellationValueType = GetTessellationValueNode()->GetValueType();
	if (uiTessellationValueType == VSPutNode::VT_1)
	{
		OutString += VSRenderer::ms_pRenderer->Float() + _T(" ");/*_T("VSREAL ");*/
		Temp = VSRenderer::ms_pRenderer->FloatConst(_T("1"));
	}
	else
	{
		VSMAC_ASSERT(0);
		return 0;
	}
	if (!GetTessellationValueNode()->GetOutputLink())
	{
		OutString += GetTessellationValueNode()->GetNodeName().GetString() + _T(" = ") + Temp + _T(";\n");
	}
	else
	{
		OutString += GetValueEqualString(GetTessellationValueNode()->GetOutputLink(), GetTessellationValueNode());
	}
	return true;
}
bool VSShaderMainFunction::GetWorldDisplacementInputValueString(VSString &OutString, MaterialShaderPara &MSPara)const
{
	VSString Temp;
	unsigned int uiWorldDisplacementValueType = GetWorldDisplacementNode()->GetValueType();
	if (uiWorldDisplacementValueType == VSPutNode::VT_3)
	{
		OutString += VSRenderer::ms_pRenderer->Float3() + _T(" ");/*_T("float3 ");*/
		Temp = VSRenderer::ms_pRenderer->Float3Const(_T("0"), _T("0"), _T("0"));/*_T("float3(0,0,0)");*/
	}
	else
	{
		VSMAC_ASSERT(0);
		return false;
	}
	if (!GetWorldDisplacementNode()->GetOutputLink())
	{
		OutString += GetWorldDisplacementNode()->GetNodeName().GetString() + _T(" = ") + Temp + _T(";\n");
	}
	else
	{
		OutString += GetValueEqualString(GetWorldDisplacementNode()->GetOutputLink(), GetWorldDisplacementNode());
	}
	return true;
}
bool VSShaderMainFunction::GetWorldOffsetInputValueString(VSString &OutString, MaterialShaderPara &MSPara)const
{
	VSString Temp;
	unsigned int uiWorldOffsetValueType = GetWorldOffsetNode()->GetValueType();
	if (uiWorldOffsetValueType == VSPutNode::VT_3)
	{
		OutString += VSRenderer::ms_pRenderer->Float3() + _T(" ");/*_T("float3 ");*/
		Temp = VSRenderer::ms_pRenderer->Float3Const(_T("0"), _T("0"), _T("0"));/*_T("float3(0,0,0)");*/
	}
	else
	{
		VSMAC_ASSERT(0);
		return false;
	}
	if (!GetWorldOffsetNode()->GetOutputLink())
	{
		OutString += GetWorldOffsetNode()->GetNodeName().GetString() + _T(" = ") + Temp + _T(";\n");
	}
	else
	{
		OutString += GetValueEqualString(GetWorldOffsetNode()->GetOutputLink(), GetWorldOffsetNode());
	}
	return true;
}
bool VSShaderMainFunction::GetNormalInputValueString(VSString &OutString, MaterialShaderPara &MSPara)const
{
	VSString Temp;
	unsigned int uiNormalValueType = GetNormalNode()->GetValueType();
	if (uiNormalValueType == VSPutNode::VT_4)
	{
		OutString += VSRenderer::ms_pRenderer->Float4() + _T(" ");/*_T("float4 ");*/
		Temp = VSRenderer::ms_pRenderer->Float4Const(_T("0"), _T("0"), _T("0"), _T("1"));/*_T("float4(0,0,0,1)");*/
	}
	else
	{
		VSMAC_ASSERT(0);
		return false;
	}
	if (!GetNormalNode()->GetOutputLink())
	{
		OutString += GetNormalNode()->GetNodeName().GetString() + _T(" = ") + Temp + _T(";\n");
	}
	else
	{
		OutString += GetValueEqualString(GetNormalNode()->GetOutputLink(), GetNormalNode());
	}
	return true;
}
bool VSShaderMainFunction::GetAlphaInputValueString(VSString &OutString, MaterialShaderPara &MSPara)const
{
	VSString Temp;
	unsigned int uiAplhaValueType = GetAlphaNode()->GetValueType();
	if (uiAplhaValueType == VSPutNode::VT_1)
	{
		OutString += VSRenderer::ms_pRenderer->Float() + _T(" ");/*_T("VSREAL ");*/
		Temp = VSRenderer::ms_pRenderer->FloatConst(_T("1"));
	}
	else
	{
		VSMAC_ASSERT(0);
		return 0;
	}
	if (!GetAlphaNode()->GetOutputLink())
	{
		OutString += GetAlphaNode()->GetNodeName().GetString() + _T(" = ") + Temp + _T(";\n");
	}
	else
	{
		OutString += GetValueEqualString(GetAlphaNode()->GetOutputLink(), GetAlphaNode());
	}
	return true;
}
bool VSShaderMainFunction::GetEmissiveInputValueString(VSString &OutString, MaterialShaderPara &MSPara)const
{
	VSString Temp;
	unsigned int uiEmissiveType = GetEmissiveNode()->GetValueType();
	if (uiEmissiveType == VSPutNode::VT_4)
	{
		OutString += VSRenderer::ms_pRenderer->Float4() + _T(" ");/*_T("float4 ");*/
		Temp = VSRenderer::ms_pRenderer->Float4Const(_T("0"), _T("0"), _T("0"), _T("1"));/*_T("float4(0,0,0,1)");*/
	}
	else
	{
		VSMAC_ASSERT(0);
		return 0;
	}
	if (!GetEmissiveNode()->GetOutputLink())
	{
		OutString += GetEmissiveNode()->GetNodeName().GetString() + _T(" = ") + Temp + _T(";\n");
	}
	else
	{
		OutString += GetValueEqualString(GetEmissiveNode()->GetOutputLink(), GetEmissiveNode());
	}
	return true;
}
bool VSShaderMainFunction::GetDiffuseInputValueString(VSString &OutString, MaterialShaderPara &MSPara)const
{
	VSString Temp;
	unsigned int uiDiffuseType = GetDiffuseNode()->GetValueType();
	if (uiDiffuseType == VSPutNode::VT_4)
	{
		OutString += VSRenderer::ms_pRenderer->Float4() + _T(" ");/*_T("float4 ");*/
		Temp = VSRenderer::ms_pRenderer->Float4Const(_T("0"), _T("0"), _T("0"), _T("1"));/*_T("float4(0,0,0,1)");*/
	}
	else
	{
		VSMAC_ASSERT(0);
		return 0;
	}

	if (!GetDiffuseNode()->GetOutputLink())
	{
		OutString += GetDiffuseNode()->GetNodeName().GetString() + _T(" = ") + Temp + _T(";\n");
	}
	else
	{
		OutString += GetValueEqualString(GetDiffuseNode()->GetOutputLink(), GetDiffuseNode());
	}
	return true;
}
bool VSShaderMainFunction::GetReflectMipInputValueString(VSString &OutString, MaterialShaderPara &MSPara)const
{
	VSString Temp;
	unsigned int uiReflectMipValueType = GetReflectMipNode()->GetValueType();
	if (uiReflectMipValueType == VSPutNode::VT_1)
	{
		OutString += VSRenderer::ms_pRenderer->Float() + _T(" ");/*_T("VSREAL ");*/
		Temp = VSRenderer::ms_pRenderer->FloatConst(_T("0"));
	}
	else
	{
		VSMAC_ASSERT(0);
		return 0;
	}
	if (!GetReflectMipNode()->GetOutputLink())
	{
		OutString += GetReflectMipNode()->GetNodeName().GetString() + _T(" = ") + Temp + _T(";\n");
	}
	else
	{
		OutString += GetValueEqualString(GetReflectMipNode()->GetOutputLink(), GetReflectMipNode());
	}
	return true;
}
bool VSShaderMainFunction::GetReflectPowInputValueString(VSString &OutString, MaterialShaderPara &MSPara)const
{
	VSString Temp;
	unsigned int uiReflectPowValueType = GetReflectPowNode()->GetValueType();
	if (uiReflectPowValueType == VSPutNode::VT_1)
	{
		OutString += VSRenderer::ms_pRenderer->Float() + _T(" ");/*_T("VSREAL ");*/
		Temp = VSRenderer::ms_pRenderer->FloatConst(_T("0"));
	}
	else
	{
		VSMAC_ASSERT(0);
		return 0;
	}
	if (!GetReflectPowNode()->GetOutputLink())
	{
		OutString += GetReflectPowNode()->GetNodeName().GetString() + _T(" = ") + Temp + _T(";\n");
	}
	else
	{
		OutString += GetValueEqualString(GetReflectPowNode()->GetOutputLink(), GetReflectPowNode());
	}
	return true;
}
bool VSShaderMainFunction::HasTessellation()
{
	if (!VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_Tessellation))
	{
		return false;
	}
	if (!GetTessellationValueNode())
	{
		return false;
	}
	if (!GetTessellationValueNode()->GetOutputLink())
	{
		return false;
	}
	if (!GetWorldDisplacementNode())
	{
		return false;
	}
	if (!GetWorldDisplacementNode()->GetOutputLink())
	{
		return false;
	}
	return true;
}