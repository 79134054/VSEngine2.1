#include "VSNormalFunction.h"
#include "VSShaderStringFactory.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSWorldNormal,VSShaderFunction)
BEGIN_ADD_PROPERTY(VSWorldNormal,VSShaderFunction)
REGISTER_ENUM_PROPERTY(m_uiNormalType, WorldNormalType, WorldNormalType, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
BEGIN_ADD_ENUM
ADD_ENUM(WorldNormalType, WNT_PIXEL)
ADD_ENUM(WorldNormalType, WNT_VERTEX)
END_ADD_ENUM
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSWorldNormal)
IMPLEMENT_INITIAL_END
VSWorldNormal::VSWorldNormal(const VSUsedName & ShowName,VSMaterial * pMaterial)
:VSShaderFunction(ShowName,pMaterial)
{


	VSString OutputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString OutputName = _T("WorldNormalOutput") + OutputID;
	VSOutputNode * pOutputNode = NULL;
	pOutputNode = VS_NEW VSOutputNode(VSPutNode::VT_3,OutputName,this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
	VSShaderStringFactory::ms_ShaderValueIndex++;

	VSString OutputNameR = VSRenderer::GetValueElement(GetOutputNode(VSOutputNode::ONI_VALUE), VSRenderer::VE_R);

	pOutputNode = VS_NEW VSOutputNode(VSPutNode::VT_1, OutputNameR, this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);

	VSString OutputNameG = VSRenderer::GetValueElement(GetOutputNode(VSOutputNode::ONI_VALUE), VSRenderer::VE_G);

	pOutputNode = VS_NEW VSOutputNode(VSPutNode::VT_1, OutputNameG, this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);

	VSString OutputNameB = VSRenderer::GetValueElement(GetOutputNode(VSOutputNode::ONI_VALUE), VSRenderer::VE_B);

	pOutputNode = VS_NEW VSOutputNode(VSPutNode::VT_1, OutputNameB, this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);

	m_uiNormalType = WNT_PIXEL;
}
VSWorldNormal::VSWorldNormal()
{
	m_uiNormalType = WNT_PIXEL;
}
VSWorldNormal::~VSWorldNormal()
{

}
void VSWorldNormal::ResetInShaderName(MaterialShaderPara &MSPara)
{
	VSString OutputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString OutputName = _T("WorldNormalOutput") + OutputID;
	m_pOutput[0]->SetNodeName(OutputName);
	VSShaderStringFactory::ms_ShaderValueIndex++;

	VSString OutputNameR = VSRenderer::GetValueElement(GetOutputNode(VSOutputNode::ONI_VALUE), VSRenderer::VE_R);
	m_pOutput[1]->SetNodeName(OutputNameR);

	VSString OutputNameG = VSRenderer::GetValueElement(GetOutputNode(VSOutputNode::ONI_VALUE), VSRenderer::VE_G);
	m_pOutput[2]->SetNodeName(OutputNameG);


	VSString OutputNameB = VSRenderer::GetValueElement(GetOutputNode(VSOutputNode::ONI_VALUE), VSRenderer::VE_B);
	m_pOutput[3]->SetNodeName(OutputNameB);
}
bool VSWorldNormal::GetFunctionString(VSString &OutString, MaterialShaderPara &MSPara)const
{
	if (MSPara.uiCompileShaderType == VSEngineFlag::ST_VERTEX || MSPara.uiCompileShaderType == VSEngineFlag::ST_DOMAIN)
	{
		OutString += GetOutputNode(VSOutputNode::ONI_VALUE)->GetNodeName().GetString() + _T(" = ") + *VSShaderStringFactory::ms_VertexWorldNormal + _T(";\n");
	}
	else
	{
		if (m_uiNormalType == WNT_PIXEL)
		{
			OutString += GetOutputNode(VSOutputNode::ONI_VALUE)->GetNodeName().GetString() + _T(" = ") + *VSShaderStringFactory::ms_WorldNormal + _T(";\n");
		}
		else
		{
			OutString += GetOutputNode(VSOutputNode::ONI_VALUE)->GetNodeName().GetString() + _T(" = ") + *VSShaderStringFactory::ms_VertexWorldNormal + _T(";\n");
		}
	}

	return 1;
}
bool VSWorldNormal::GetOutputValueString(VSString &OutString, MaterialShaderPara &MSPara)const
{
	VSString Temp;
	if (m_pOutput[0]->GetValueType() == VSPutNode::VT_1)
	{
		OutString += VSRenderer::ms_pRenderer->Float() + _T(" ");/*_T("VSREAL ");*/
		Temp = VSRenderer::ms_pRenderer->FloatConst(_T("0"));
	}
	else if (m_pOutput[0]->GetValueType() == VSPutNode::VT_2)
	{
		OutString += VSRenderer::ms_pRenderer->Float2() + _T(" ");/*_T("float2 ")*/;
		Temp = VSRenderer::ms_pRenderer->Float2Const(_T("0"), _T("0"));/*_T("float2(0,0)");*/
	}
	else if (m_pOutput[0]->GetValueType() == VSPutNode::VT_3)
	{
		OutString += VSRenderer::ms_pRenderer->Float3() + _T(" ");/*_T("float3 ");*/
		Temp = VSRenderer::ms_pRenderer->Float3Const(_T("0"), _T("0"), _T("0"));/*_T("float3(0,0,0)");*/
	}
	else if (m_pOutput[0]->GetValueType() == VSPutNode::VT_4)
	{
		OutString += VSRenderer::ms_pRenderer->Float4() + _T(" ");/*_T("float4 ");*/
		Temp = VSRenderer::ms_pRenderer->Float4Const(_T("0"), _T("0"), _T("0"), _T("1"));/*_T("float4(0,0,0,1)");*/
	}
	else
		VSMAC_ASSERT(0);

	OutString += m_pOutput[0]->GetNodeName().GetString() + _T(" = ") + Temp + _T(";\n");

	return 1;
}

IMPLEMENT_RTTI(VSViewNormal,VSShaderFunction)
BEGIN_ADD_PROPERTY(VSViewNormal,VSShaderFunction)
REGISTER_ENUM_PROPERTY(m_uiNormalType, ViewNormalType, ViewNormalType, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
BEGIN_ADD_ENUM
ADD_ENUM(ViewNormalType, VNT_PIXEL)
ADD_ENUM(ViewNormalType, VNT_VERTEX)
END_ADD_ENUM
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSViewNormal)
IMPLEMENT_INITIAL_END
VSViewNormal::VSViewNormal(const VSUsedName & ShowName,VSMaterial * pMaterial)
:VSShaderFunction(ShowName,pMaterial)
{


	VSString OutputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString OutputName = _T("ViewNormalOutput") + OutputID;
	VSOutputNode * pOutputNode = NULL;
	pOutputNode = VS_NEW VSOutputNode(VSPutNode::VT_4,OutputName,this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
	VSShaderStringFactory::ms_ShaderValueIndex++;

	VSString OutputNameR = VSRenderer::GetValueElement(GetOutputNode(VSOutputNode::ONI_VALUE), VSRenderer::VE_R);

	pOutputNode = VS_NEW VSOutputNode(VSPutNode::VT_1, OutputNameR, this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);

	VSString OutputNameG = VSRenderer::GetValueElement(GetOutputNode(VSOutputNode::ONI_VALUE), VSRenderer::VE_G);

	pOutputNode = VS_NEW VSOutputNode(VSPutNode::VT_1, OutputNameG, this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);

	VSString OutputNameB = VSRenderer::GetValueElement(GetOutputNode(VSOutputNode::ONI_VALUE), VSRenderer::VE_B);

	pOutputNode = VS_NEW VSOutputNode(VSPutNode::VT_1, OutputNameB, this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
	m_uiNormalType = VNT_PIXEL;
}
VSViewNormal::VSViewNormal()
{
	m_uiNormalType = VNT_PIXEL;
}
VSViewNormal::~VSViewNormal()
{

}
void VSViewNormal::ResetInShaderName(MaterialShaderPara &MSPara)
{
	VSString OutputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString OutputName = _T("ViewNormalOutput") + OutputID;
	m_pOutput[0]->SetNodeName(OutputName);
	VSShaderStringFactory::ms_ShaderValueIndex++;

	VSString OutputNameR = VSRenderer::GetValueElement(GetOutputNode(VSOutputNode::ONI_VALUE), VSRenderer::VE_R);
	m_pOutput[1]->SetNodeName(OutputNameR);

	VSString OutputNameG = VSRenderer::GetValueElement(GetOutputNode(VSOutputNode::ONI_VALUE), VSRenderer::VE_G);
	m_pOutput[2]->SetNodeName(OutputNameG);


	VSString OutputNameB = VSRenderer::GetValueElement(GetOutputNode(VSOutputNode::ONI_VALUE), VSRenderer::VE_B);
	m_pOutput[3]->SetNodeName(OutputNameB);
}
bool VSViewNormal::GetFunctionString(VSString &OutString, MaterialShaderPara &MSPara)const
{

	if (MSPara.uiCompileShaderType == VSEngineFlag::ST_VERTEX)
	{
		OutString += GetOutputNode(VSOutputNode::ONI_VALUE)->GetNodeName().GetString() + _T(" = ") + *VSShaderStringFactory::ms_VertexViewNormal + _T(";\n");
	}
	else
	{
		if (m_uiNormalType == VNT_PIXEL)
		{
			OutString += GetOutputNode(VSOutputNode::ONI_VALUE)->GetNodeName().GetString() + _T(" = ") + *VSShaderStringFactory::ms_ViewNormal + _T(";\n");
		}
		else
		{
			OutString += GetOutputNode(VSOutputNode::ONI_VALUE)->GetNodeName().GetString() + _T(" = ") + *VSShaderStringFactory::ms_VertexViewNormal + _T(";\n");
		}
	}

	

	return 1;
}
bool VSViewNormal::GetOutputValueString(VSString &OutString, MaterialShaderPara &MSPara)const
{
	VSString Temp;
	if (m_pOutput[0]->GetValueType() == VSPutNode::VT_1)
	{
		OutString += VSRenderer::ms_pRenderer->Float() + _T(" ");/*_T("VSREAL ");*/
		Temp = VSRenderer::ms_pRenderer->FloatConst(_T("0"));
	}
	else if (m_pOutput[0]->GetValueType() == VSPutNode::VT_2)
	{
		OutString += VSRenderer::ms_pRenderer->Float2() + _T(" ");/*_T("float2 ")*/;
		Temp = VSRenderer::ms_pRenderer->Float2Const(_T("0"), _T("0"));/*_T("float2(0,0)");*/
	}
	else if (m_pOutput[0]->GetValueType() == VSPutNode::VT_3)
	{
		OutString += VSRenderer::ms_pRenderer->Float3() + _T(" ");/*_T("float3 ");*/
		Temp = VSRenderer::ms_pRenderer->Float3Const(_T("0"), _T("0"), _T("0"));/*_T("float3(0,0,0)");*/
	}
	else if (m_pOutput[0]->GetValueType() == VSPutNode::VT_4)
	{
		OutString += VSRenderer::ms_pRenderer->Float4() + _T(" ");/*_T("float4 ");*/
		Temp = VSRenderer::ms_pRenderer->Float4Const(_T("0"), _T("0"), _T("0"), _T("1"));/*_T("float4(0,0,0,1)");*/
	}
	else
		VSMAC_ASSERT(0);

	OutString += m_pOutput[0]->GetNodeName().GetString() + _T(" = ") + Temp + _T(";\n");

	return 1;
}