#include "VSColorBuffer.h"
#include "VSShaderStringFactory.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSColorBuffer, VSShaderFunction)
BEGIN_ADD_PROPERTY(VSColorBuffer, VSShaderFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSColorBuffer)
IMPLEMENT_INITIAL_END
VSColorBuffer::VSColorBuffer(const VSUsedName & ShowName, VSMaterial * pMaterial)
:VSShaderFunction(ShowName, pMaterial)
{


	VSString InputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString InputName = _T("ColorBufferInput") + InputID;
	VSInputNode * pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::VT_2, InputName, this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);
	VSShaderStringFactory::ms_ShaderValueIndex++;


	VSString OutputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString OutputName = _T("ColorBufferOutput") + OutputID;
	VSOutputNode * pOutputNode = NULL;
	pOutputNode = VS_NEW VSOutputNode(VSPutNode::VT_4, OutputName, this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
	VSShaderStringFactory::ms_ShaderValueIndex++;

	VSString OutputNameR = VSRenderer::GetValueElement(GetOutputNode(VSOutputNode::ONI_COLOR), VSRenderer::VE_R);

	pOutputNode = VS_NEW VSOutputNode(VSPutNode::VT_1, OutputNameR, this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);

	VSString OutputNameG = VSRenderer::GetValueElement(GetOutputNode(VSOutputNode::ONI_COLOR), VSRenderer::VE_G);

	pOutputNode = VS_NEW VSOutputNode(VSPutNode::VT_1, OutputNameG, this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);

	VSString OutputNameB = VSRenderer::GetValueElement(GetOutputNode(VSOutputNode::ONI_COLOR), VSRenderer::VE_B);

	pOutputNode = VS_NEW VSOutputNode(VSPutNode::VT_1, OutputNameB, this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);

	VSString OutputNameA = VSRenderer::GetValueElement(GetOutputNode(VSOutputNode::ONI_COLOR), VSRenderer::VE_A);

	pOutputNode = VS_NEW VSOutputNode(VSPutNode::VT_1, OutputNameA, this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
}
VSColorBuffer::VSColorBuffer()
{

}
VSColorBuffer::~VSColorBuffer()
{

}
void VSColorBuffer::ResetInShaderName(MaterialShaderPara &MSPara)
{
	VSString InputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString InputName = _T("ColorBufferInput") + InputID;
	m_pInput[0]->SetNodeName(InputName);
	VSShaderStringFactory::ms_ShaderValueIndex++;

	VSString OutputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString OutputName = _T("ColorBufferOutput") + OutputID;
	m_pOutput[0]->SetNodeName(OutputName);
	VSShaderStringFactory::ms_ShaderValueIndex++;


	VSString OutputNameR = VSRenderer::GetValueElement(GetOutputNode(VSOutputNode::ONI_COLOR), VSRenderer::VE_R);
	m_pOutput[1]->SetNodeName(OutputNameR);

	VSString OutputNameG = VSRenderer::GetValueElement(GetOutputNode(VSOutputNode::ONI_COLOR), VSRenderer::VE_G);
	m_pOutput[2]->SetNodeName(OutputNameG);


	VSString OutputNameB = VSRenderer::GetValueElement(GetOutputNode(VSOutputNode::ONI_COLOR), VSRenderer::VE_B);
	m_pOutput[3]->SetNodeName(OutputNameB);

	VSString OutputNameA = VSRenderer::GetValueElement(GetOutputNode(VSOutputNode::ONI_COLOR), VSRenderer::VE_A);
	m_pOutput[4]->SetNodeName(OutputNameA);
}
bool VSColorBuffer::GetFunctionString(VSString &OutString, MaterialShaderPara &MSPara)const
{
	VSMAC_ASSERT(VSRenderer::ms_pRenderer);
	OutString += VSRenderer::ms_pRenderer->TexColorBuffer(this);
	return 1;
}
bool VSColorBuffer::GetInputValueString(VSString &OutString, MaterialShaderPara &MSPara)const
{
	VSMAC_ASSERT(VSRenderer::ms_pRenderer);
	VSString Temp;
	if (m_pInput[0]->GetValueType() == VSPutNode::VT_1)
	{
		OutString += VSRenderer::ms_pRenderer->Float() + _T(" ");/*_T("VSREAL ");*/
		Temp = VSRenderer::ms_pRenderer->FloatConst(_T("0"));
	}
	else if (m_pInput[0]->GetValueType() == VSPutNode::VT_2)
	{
		OutString += VSRenderer::ms_pRenderer->Float2() + _T(" ");/*_T("float2 ");*/
		Temp = VSRenderer::ms_pRenderer->Float2Const(_T("0"), _T("0"));/*_T("float2(0,0)");*/
	}
	else if (m_pInput[0]->GetValueType() == VSPutNode::VT_3)
	{
		OutString += VSRenderer::ms_pRenderer->Float3() + _T(" ");/*_T("float3 ");*/
		Temp = VSRenderer::ms_pRenderer->Float3Const(_T("0"), _T("0"), _T("0"));/*_T("float3(0,0,0)");*/
	}
	else if (m_pInput[0]->GetValueType() == VSPutNode::VT_4)
	{
		OutString += VSRenderer::ms_pRenderer->Float4() + _T(" ");/*_T("float4 ");*/
		Temp = VSRenderer::ms_pRenderer->Float4Const(_T("0"), _T("0"), _T("0"), _T("1"));/*_T("float4(0,0,0,1)");*/
	}
	else
		return 0;
	if (!m_pInput[0]->GetOutputLink())
	{
		OutString += m_pInput[0]->GetNodeName().GetString() + _T(" = ") + VSShaderStringFactory::ms_TextureInputCoordValue[0] + _T(";\n");
		return 1;
	}

	OutString += GetValueEqualString(m_pInput[0]->GetOutputLink(), m_pInput[0]);

	return 1;
}
bool VSColorBuffer::GetOutputValueString(VSString &OutString, MaterialShaderPara &MSPara)const
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
		return 0;

	OutString += m_pOutput[0]->GetNodeName().GetString() + _T(" = ") + Temp + _T(";\n");

	return 1;
}