#include "VSDx11Renderer.h"
#include "VS2DTexSampler.h"
#include "VS1DTexSampler.h"
#include "VSGeometry.h"
#include "VSString.h"
#include "VSResourceManager.h"
#include "VSShaderStringFactory.h"
#include "VSGraphicInclude.h"
#include "VSOrenNayarShaderFunction.h"
#include "VSPointLight.h"
#include "VSDirectionLight.h"
#include "VSArithmeticShaderFunction.h"
#include "VSSpotLight.h"
#include "VSColorBuffer.h"
#ifdef WINDOWS_PLATFORM
using namespace VSEngine2;
VSString VSDX11Renderer::TexColorBuffer(const VSColorBuffer * pColorBuffer) const
{
	if (!pColorBuffer)
		return VSString();
	return pColorBuffer->GetOutputNode(VSOutputNode::ONI_COLOR)->GetNodeName().GetString()
		+ _T(" = ") + *VSShaderStringFactory::ms_PSColorBufferResource + _T("Texture") + _T(".Sample(") + *VSShaderStringFactory::ms_PSColorBufferResource + _T("Sampler") + _T(", ") +
		pColorBuffer->GetInputNode(VSInputNode::INI_TEXCOORD)->GetNodeName().GetString() + _T(");\n");
}
VSString VSDX11Renderer::Tex2D(const VS2DTexSampler * p2DTexSampler, unsigned int uiShaderType) const
{
	VSMAC_ASSERT(p2DTexSampler);
	if (!p2DTexSampler)
		return VSString();
	if (uiShaderType == VSEngineFlag::ST_PIXEL)
	{
		if (p2DTexSampler->GetSamplerSource() == VSEngineFlag::SS_FROM_TEXTURE)
		{
			return p2DTexSampler->GetOutputNode(VSOutputNode::ONI_COLOR)->GetNodeName().GetString()
				+ _T(" = ") + p2DTexSampler->GetShowName().GetString() + _T("Texture") +
				_T(".Sample(") + p2DTexSampler->GetShowName().GetString() + _T("Sampler") + _T(", ") +
				p2DTexSampler->GetInputNode(VSInputNode::INI_TEXCOORD)->GetNodeName().GetString() + _T(");\n");
		}
		else
		{
			return p2DTexSampler->GetOutputNode(VSOutputNode::ONI_COLOR)->GetNodeName().GetString()
				+ _T(" = ") + p2DTexSampler->GetShowName().GetString() + _T("Texture") +
				_T(".Sample(") + *VSShaderStringFactory::ms_ShareSampler[p2DTexSampler->GetSamplerSource() - 1] + _T(", ") +
				p2DTexSampler->GetInputNode(VSInputNode::INI_TEXCOORD)->GetNodeName().GetString() + _T(");\n");
		}
	}
	else
	{
		if (p2DTexSampler->GetSamplerSource() == VSEngineFlag::SS_FROM_TEXTURE)
		{
			return p2DTexSampler->GetOutputNode(VSOutputNode::ONI_COLOR)->GetNodeName().GetString()
				+ _T(" = ") + p2DTexSampler->GetShowName().GetString() + _T("Texture") +
				_T(".SampleLevel(") + p2DTexSampler->GetShowName().GetString() + _T("Sampler") + _T(", ") +
				p2DTexSampler->GetInputNode(VSInputNode::INI_TEXCOORD)->GetNodeName().GetString() + _T(",0);\n");
		}
		else
		{
			return p2DTexSampler->GetOutputNode(VSOutputNode::ONI_COLOR)->GetNodeName().GetString()
				+ _T(" = ") + p2DTexSampler->GetShowName().GetString() + _T("Texture") +
				_T(".SampleLevel(") + *VSShaderStringFactory::ms_ShareSampler[p2DTexSampler->GetSamplerSource() - 1] + _T(", ") +
				p2DTexSampler->GetInputNode(VSInputNode::INI_TEXCOORD)->GetNodeName().GetString() + _T(",0);\n");
		}
	}
	
}
VSString VSDX11Renderer::Tex1D(const VS1DTexSampler * p1DTexSampler, unsigned int uiShaderType) const
{
	VSMAC_ASSERT(p1DTexSampler);
	if (!p1DTexSampler)
		return VSString();
	if (uiShaderType == VSEngineFlag::ST_PIXEL)
	{
		if (p1DTexSampler->GetSamplerSource() == VSEngineFlag::SS_FROM_TEXTURE)
		{
			return p1DTexSampler->GetOutputNode(VSOutputNode::ONI_COLOR)->GetNodeName().GetString()
				+ _T(" = ") + p1DTexSampler->GetShowName().GetString() + _T("Texture") +
				_T(".Sample(") + p1DTexSampler->GetShowName().GetString() + _T("Sampler") + _T(", ") +
				p1DTexSampler->GetInputNode(VSInputNode::INI_TEXCOORD)->GetNodeName().GetString() + _T(");\n");
		}
		else
		{
			return p1DTexSampler->GetOutputNode(VSOutputNode::ONI_COLOR)->GetNodeName().GetString()
				+ _T(" = ") + p1DTexSampler->GetShowName().GetString() + _T("Texture") +
				_T(".Sample(") + *VSShaderStringFactory::ms_ShareSampler[p1DTexSampler->GetSamplerSource() - 1] + _T(", ") +
				p1DTexSampler->GetInputNode(VSInputNode::INI_TEXCOORD)->GetNodeName().GetString() + _T(");\n");
		}
	}
	else
	{
		if (p1DTexSampler->GetSamplerSource() == VSEngineFlag::SS_FROM_TEXTURE)
		{
			return p1DTexSampler->GetOutputNode(VSOutputNode::ONI_COLOR)->GetNodeName().GetString()
				+ _T(" = ") + p1DTexSampler->GetShowName().GetString() + _T("Texture") +
				_T(".SampleLevel(") + p1DTexSampler->GetShowName().GetString() + _T("Sampler") + _T(", ") +
				p1DTexSampler->GetInputNode(VSInputNode::INI_TEXCOORD)->GetNodeName().GetString() + _T(",0);\n");
		}
		else
		{
			return p1DTexSampler->GetOutputNode(VSOutputNode::ONI_COLOR)->GetNodeName().GetString()
				+ _T(" = ") + p1DTexSampler->GetShowName().GetString() + _T("Texture") +
				_T(".SampleLevel(") + *VSShaderStringFactory::ms_ShareSampler[p1DTexSampler->GetSamplerSource() - 1] + _T(", ") +
				p1DTexSampler->GetInputNode(VSInputNode::INI_TEXCOORD)->GetNodeName().GetString() + _T(",0);\n");
		}
	}
}
VSString VSDX11Renderer::Mul(const VSMul * pMul) const
{
	return pMul->GetOutputNode(VSOutputNode::ONI_VALUE)->GetNodeName().GetString()
		+ _T(" = ") + pMul->GetInputNode(VSInputNode::INI_A)->GetNodeName().GetString() + _T(" * ") + 
		pMul->GetInputNode(VSInputNode::INI_B)->GetNodeName().GetString() + _T(";\n");
}
VSString VSDX11Renderer::Saturate(const VSSaturate * pSaturate) const
{
	return pSaturate->GetOutputNode(VSOutputNode::ONI_VALUE)->GetNodeName().GetString()
		+ _T(" = saturate(") + pSaturate->GetInputNode(VSInputNode::INI_VALUE)->GetNodeName().GetString() + _T(");\n");
}
VSString VSDX11Renderer::Div(const VSDiv * pDiv) const
{
	return pDiv->GetOutputNode(VSOutputNode::ONI_VALUE)->GetNodeName().GetString()
		+ _T(" = ") + pDiv->GetInputNode(VSInputNode::INI_A)->GetNodeName().GetString() + _T(" / ") +
		pDiv->GetInputNode(VSInputNode::INI_B)->GetNodeName().GetString() + _T(";\n");
}
VSString VSDX11Renderer::Sub(const VSSub * pSub) const
{
	return pSub->GetOutputNode(VSOutputNode::ONI_VALUE)->GetNodeName().GetString()
		+ _T(" = ") + pSub->GetInputNode(VSInputNode::INI_A)->GetNodeName().GetString() + _T(" - ") + 
		pSub->GetInputNode(VSInputNode::INI_B)->GetNodeName().GetString() + _T(";\n");
}
VSString VSDX11Renderer::Add(const VSAdd * pAdd) const
{
	return pAdd->GetOutputNode(VSOutputNode::ONI_VALUE)->GetNodeName().GetString()
		+ _T(" = ") + pAdd->GetInputNode(VSInputNode::INI_A)->GetNodeName().GetString() + _T(" + ") +
		pAdd->GetInputNode(VSInputNode::INI_B)->GetNodeName().GetString() + _T(";\n");
}
VSString VSDX11Renderer::Dot(const VSDot * pDot) const
{
	return pDot->GetOutputNode(VSOutputNode::ONI_VALUE)->GetNodeName().GetString()
		+ _T(" = dot(") + pDot->GetInputNode(VSInputNode::INI_A)->GetNodeName().GetString() + _T(",") + 
		pDot->GetInputNode(VSInputNode::INI_B)->GetNodeName().GetString() + _T(");\n");
}
VSString VSDX11Renderer::Length(const VSLength * pLength) const
{
	return pLength->GetOutputNode(VSOutputNode::ONI_VALUE)->GetNodeName().GetString()
		+ _T(" = length(") + pLength->GetInputNode(VSInputNode::INI_VALUE)->GetNodeName().GetString() + _T(");\n");
}
VSString VSDX11Renderer::MakeValue(const VSMakeValue * pMakeValue)const
{
	VSString Temp;
	if (pMakeValue->GetOutputNode(VSOutputNode::ONI_VALUE)->GetValueType() == VSPutNode::VT_1)
	{
		Temp = _T("float(");
	}
	else if (pMakeValue->GetOutputNode(VSOutputNode::ONI_VALUE)->GetValueType() == VSPutNode::VT_2)
	{
		Temp = _T("float2(");		
	}
	else if (pMakeValue->GetOutputNode(VSOutputNode::ONI_VALUE)->GetValueType() == VSPutNode::VT_3)
	{
		Temp = _T("float3(");
	}
	else if (pMakeValue->GetOutputNode(VSOutputNode::ONI_VALUE)->GetValueType() == VSPutNode::VT_4)
	{
		Temp = _T("float4(");
	}
	for (unsigned int i = 0; i < pMakeValue->GetInputNodeNum(); i++)
	{
		Temp += pMakeValue->GetInputNode(i)->GetNodeName().GetString();
		if (i == pMakeValue->GetInputNodeNum() - 1)
		{
			Temp += _T(")");
		}
		else
		{
			Temp += _T(",");
		}
	}


	return pMakeValue->GetOutputNode(VSOutputNode::ONI_VALUE)->GetNodeName().GetString() + _T(" = ") + Temp +  _T(";\n");
}
VSString VSDX11Renderer::Sampler(unsigned int uiType)const
{
	if (uiType == VSTexture::TT_1D)
	{
		return VSString(_T("Texture1D "));
	}
	else if (uiType == VSTexture::TT_2D)
	{
		return VSString(_T("Texture2D "));
	}
	else if (uiType == VSTexture::TT_3D)
	{
		return VSString(_T("Texture3D "));
	}
	else if (uiType == VSTexture::TT_CUBE)
	{
		return VSString(_T("TextureCube "));
	}
	return VSString(_T("Texture2D "));
}
VSString VSDX11Renderer::FloatByChannelNum(unsigned int ChannelNum)const
{
	VSMAC_ASSERT(ChannelNum > 0 && ChannelNum < 5)
	if (ChannelNum == 1)
	{
		return VSString(_T("float "));
	}
	else if (ChannelNum == 2)
	{
		return VSString(_T("float2 "));
	}
	else if (ChannelNum == 3)
	{
		return VSString(_T("float3 "));
	}
	else if (ChannelNum == 4)
	{
		return VSString(_T("float4 "));
	}
	return VSString(_T("float "));
}
VSString VSDX11Renderer::Float()const
{
	return VSString(_T("float "));
}
VSString VSDX11Renderer::Float2()const
{
	return VSString(_T("float2 "));
}
VSString VSDX11Renderer::Float3()const
{
	return VSString(_T("float3 "));
}
VSString VSDX11Renderer::Float4()const
{
	return VSString(_T("float4 "));
}
VSString VSDX11Renderer::Return()const
{
	return VSString(_T("return "));
}
VSString VSDX11Renderer::FloatConst(const VSString & Value1)const
{
	return Value1;
}
VSString VSDX11Renderer::Float2Const(const VSString &Value1,const VSString &Value2)const
{
	return VSString(_T("float2")) + _T("(") + Value1 + _T(",") + Value2 + _T(")");
}
VSString VSDX11Renderer::Float3Const(const VSString & Value1,
							 const VSString& Value2,const VSString &Value3)const
{
	return VSString(_T("float3")) + _T("(") + Value1 + _T(",") + Value2 + _T(",") + Value3 + _T(")");
}
VSString VSDX11Renderer::Float4Const(const VSString &Value1,const VSString &Value2,
							 const VSString &Value3,const VSString &Value4)const
{
	return VSString(_T("float4")) + _T("(") + Value1 + _T(",") + Value2 + _T(",") + Value3 + _T(",") + Value4 + _T(")");
}
void VSDX11Renderer::CustomPointLightUseString(VSString & PointLightUseString ,VSString & PointAttenuationDivString)
{
	PointLightUseString = _T("float3 ") + *VSShaderStringFactory::ms_LightDir + _T(" = WorldPos - LightWorldPos;\n")
		+ _T("float  fDistance = length(") + *VSShaderStringFactory::ms_LightDir + _T(");\n")
		+ *VSShaderStringFactory::ms_LightDir + _T(" = ") + *VSShaderStringFactory::ms_LightDir + _T(" / fDistance;\n") 
		+ _T("float fLightAttenuationDiv = saturate(1.0f - fDistance / Range);\n");
	PointAttenuationDivString = _T("* fLightAttenuationDiv");
}
void VSDX11Renderer::CustomSpotLightUseString(VSString & SpotLightUseString ,VSString & SpotAttenuationDivString)
{
	SpotLightUseString = _T("float3 ") + *VSShaderStringFactory::ms_LightDir + _T(" = WorldPos - LightWorldPos;\n")
		+ _T("float  fDistance = length(") + *VSShaderStringFactory::ms_LightDir + _T(");\n")
		+ *VSShaderStringFactory::ms_LightDir + _T(" = ") + *VSShaderStringFactory::ms_LightDir + _T(" / fDistance;\n")  
		+ _T("float fLightAttenuationDiv = saturate(1.0f - fDistance / Range);\n")
		+ _T("float fSpotLightCos = dot(") + *VSShaderStringFactory::ms_LightDir + _T(",LightWorldDirection);\n")
		+ _T("float fLightIf = saturate((fSpotLightCos - cos(Phi/ 2)) / (cos(Theta/ 2) - cos(Phi / 2)));\n")
		+ _T("float fLightEffect = pow(fLightIf,Falloff);\n");

	SpotAttenuationDivString = _T(" * fLightAttenuationDiv  * fLightEffect");
}
VSString VSDX11Renderer::GetDeclareValue(const VSString &ShowName, unsigned int uiValueType, unsigned int uiRegisterIndex)const
{
	VSString OutString;
	if (uiValueType == VSPutNode::VT_1)
	{
		OutString += _T("float ");
	}
	else if (uiValueType == VSPutNode::VT_2)
	{
		OutString += _T("float2 ");
	}
	else if (uiValueType == VSPutNode::VT_3)
	{
		OutString += _T("float3 ");
	}
	else if (uiValueType == VSPutNode::VT_4)
	{
		OutString += _T("float4 ");
	}
	OutString += ShowName + _T(";\n");
	return OutString;
}
void VSDX11Renderer::GetDeclareShareSampler(unsigned int SamplerSource, unsigned int uiRegisterIndex, VSString & OutString)const
{
	OutString +=  _T("SamplerState ");
	OutString += *VSShaderStringFactory::ms_ShareSampler[SamplerSource] + SetRegister(RT_S, uiRegisterIndex) + _T(";\n");
}
VSString VSDX11Renderer::GetDeclareSampler(const VSString &ShowName, unsigned int SamplerType, unsigned int uiRegisterIndex, unsigned int SamplerSource)const
{
	VSString OutString;
	if (SamplerSource == VSEngineFlag::SS_FROM_TEXTURE)
	{
		OutString += _T("SamplerState ");
		OutString += ShowName + _T("Sampler")
			+ SetRegister(RT_S, uiRegisterIndex) + _T(";\n");
	}
	OutString += Sampler(SamplerType) + _T(" ") + ShowName + _T("Texture")
		+ SetRegister(RT_T, uiRegisterIndex) + _T(";\n");
	return OutString;
}
VSString VSDX11Renderer::SetRegister(unsigned int uiRegisterType,unsigned int uiRegisterIndex)const
{

	VSString RegisterID = IntToString(uiRegisterIndex);
	if(uiRegisterType == RT_S)
		return _T(" : register(s") + RegisterID + _T(")");
	else if (uiRegisterType == RT_T)
		return _T(" : register(t") + RegisterID + _T(")");
	else
		VSMAC_ASSERT(1);
	return VSString();
}
void VSDX11Renderer::CreateVInputDeclareMaterialInstance(MaterialShaderPara &MSPara,
	VSString & OutString)
{
	VSVertexBuffer *pInstanceVertex = ((VSInstanceGeometry *)MSPara.pGeometry)->GetInstanceVertexBuffer();
	for (unsigned int i = 0; i < pInstanceVertex->GetMaterialInstanceLevel(); i++)
	{
		if (pInstanceVertex->HaveMaterialInstanceInfo(i))
		{
			VSString MaterialInstanceID = IntToString(i);
			OutString += FloatByChannelNum(pInstanceVertex->MaterialInstanceChannel(i)) + _T("MaterialInstance") + MaterialInstanceID + _T(":MATERIAL_INSTANCE") + MaterialInstanceID + _T(";\n");
			VSString InputString = _T("Input.MaterialInstance") + MaterialInstanceID;
			VSShaderStringFactory::ms_InputMaterialInstance.AddElement(InputString);
		}
	}
}
void VSDX11Renderer::CreateVInputDeclareInstancePosition(MaterialShaderPara &MSPara,
	VSString & OutString)
{
	VSVertexBuffer * pVBuffer = MSPara.pGeometry->GetMeshData()->GetVertexBuffer();
	VSVertexBuffer * pInstanceVertex = ((VSInstanceGeometry *)MSPara.pGeometry)->GetInstanceVertexBuffer();
	unsigned int j = pVBuffer->GetPositionLevel();

	VSString Index[4] = { _T("[0]"),_T("[1]"),_T("[2]"),_T("[3]") };
	unsigned int uiAnimSize = sizeof(ANIM_INSTANCE_SHADER_DATA) / sizeof(VSREAL) * MAX_SIMPLE_INSTANCE_ANIM;
	unsigned int uiAnimSizeIndex = 0;
	unsigned int uiLastID = 0;
	VSString InputPos = _T("Input.Position");
	for (unsigned int i = 0; i < pInstanceVertex->GetPositionLevel(); i++)
	{
		if (pInstanceVertex->HavePositionInfo(i))
		{
			uiLastID = i + j;
			VSString VertexID = IntToString(i + j);
			OutString += FloatByChannelNum(pInstanceVertex->PositionChannel(j)) + _T("Position") + VertexID + _T(":POSITION") + VertexID + _T(";\n");

			VSString InputPosID = InputPos + VertexID;
			if (i < 3)
			{
				VSShaderStringFactory::ms_InputInstancePos[i] = InputPosID;
			}
			else
			{
				for (unsigned int j = 0; j < 4; j++)
				{
					if (uiAnimSizeIndex < uiAnimSize - 1)
					{
						VSShaderStringFactory::ms_InputInstanceAnimData += InputPosID + Index[j] + _T(",");
						uiAnimSizeIndex++;
					}
					else if (uiAnimSizeIndex < uiAnimSize)
					{
						VSShaderStringFactory::ms_InputInstanceAnimData += InputPosID + Index[j];
						uiAnimSizeIndex++;
					}
				}
			}
		}

	}
	VSShaderStringFactory::ms_InputInstanceAnimNum = InputPos + IntToString(uiLastID) + Index[3];
}
void VSDX11Renderer::CreateVInputDeclarePosition(MaterialShaderPara &MSPara,
								 VSString & OutString)
{
	VSVertexBuffer * pVBuffer = MSPara.pGeometry->GetMeshData()->GetVertexBuffer();
	for(unsigned int i = 0 ; i < pVBuffer->GetPositionLevel(); i++)
	{	
		if(pVBuffer->HavePositionInfo(i))
		{
			VSString VertexID = IntToString(i);		
			OutString += FloatByChannelNum(pVBuffer->PositionChannel(i)) + _T("Position") + VertexID + _T(":POSITION") + VertexID + _T(";\n");
		}

	}
}
void VSDX11Renderer::CreateVInputDeclareTexCoord(MaterialShaderPara &MSPara,
								 VSString & OutString)
{
	VSVertexBuffer * pVBuffer = MSPara.pGeometry->GetMeshData()->GetVertexBuffer();
	for(unsigned int i = 0 ; i < pVBuffer->GetTexCoordLevel(); i++)
	{
		if(pVBuffer->HaveTexCoordInfo(i))
		{
			VSString TextureID = IntToString(i);
			OutString += FloatByChannelNum(pVBuffer->TexCoordChannel(i)) + _T("TexCoord") + TextureID + _T(":TEXCOORD") + TextureID + _T(";\n");
		}
	}
}
void VSDX11Renderer::CreateVInputDeclareNormal(MaterialShaderPara &MSPara,
							   VSString & OutString)
{
	VSVertexBuffer * pVBuffer = MSPara.pGeometry->GetMeshData()->GetVertexBuffer();
	for(unsigned int i = 0 ; i < pVBuffer->GetNormalLevel(); i++)
	{
		if(pVBuffer->HaveNormalInfo(i))
		{
			VSString NomalID = IntToString(i);
			OutString += FloatByChannelNum(pVBuffer->NormalChannel(i)) + _T("Normal") + NomalID + _T(":NORMAL") + NomalID + _T(";\n");
		}
	}

	if(pVBuffer->HaveTangentInfo())
	{
		OutString += _T("float4 Tangent : TANGENT;\n");
		if (pVBuffer->HaveBinormalInfo())
		{
			OutString += _T("float3 Binormal : BINORMAL;\n");
		}
	}
}
void VSDX11Renderer::CreateVInputDeclareColor(MaterialShaderPara &MSPara,
							  VSString & OutString)
{
	VSVertexBuffer * pVBuffer = MSPara.pGeometry->GetMeshData()->GetVertexBuffer();
	for(unsigned int i = 0 ; i < pVBuffer->GetColorLevel(); i++)
	{

		if(pVBuffer->HaveColorInfo(i))
		{
			VSString ColorID = IntToString(i);
			OutString += FloatByChannelNum(pVBuffer->ColorChannel(i)) + _T("Color") + ColorID + _T(":COLOR") + ColorID + _T(";\n");
			VSShaderStringFactory::ms_InputColor[i] = _T("Input.Color") + ColorID;
		}
	}
}
void VSDX11Renderer::CreateVInputDeclareSkin(MaterialShaderPara &MSPara,
							 VSString & OutString)
{
	VSVertexBuffer * pVBuffer = MSPara.pGeometry->GetMeshData()->GetVertexBuffer();
	if(pVBuffer->HaveBlendWeightInfo())
	{
		OutString += _T("float4 BlendWeight :BLENDWEIGHT;\n");
	}

	if(pVBuffer->HaveBlendIndicesInfo(0))
	{
		OutString += _T("uint4 BlendIndices :BLENDINDICES0;\n");
	}
	if (pVBuffer->HaveBlendIndicesInfo(1))
	{
		OutString += _T("uint4 BlendIndicesInstance :BLENDINDICES1;\n");
	}
}
void VSDX11Renderer::CreateVInputDeclare(MaterialShaderPara &MSPara,
										VSString & OutString)
{

	VSString TempDeclare;
	
	CreateVInputDeclarePosition(MSPara,TempDeclare);
	CreateVInputDeclareTexCoord(MSPara,TempDeclare);
	CreateVInputDeclareNormal(MSPara,TempDeclare);
	CreateVInputDeclareColor(MSPara,TempDeclare);
	CreateVInputDeclareSkin(MSPara,TempDeclare);
	if (MSPara.pGeometry->IsSameType(VSInstanceGeometry::ms_Type))
	{
		CreateVInputDeclareInstancePosition(MSPara,TempDeclare);
		CreateVInputDeclareMaterialInstance(MSPara, TempDeclare);
	}
	OutString += _T("struct VS_INPUT \n{ \n") + TempDeclare + _T("};\n");
	
}
void VSDX11Renderer::CreateDeclarePosition(MaterialShaderPara &MSPara,
								  VSString & OutString, bool bInput)
{
	OutString += _T("float4 Position:SV_POSITION;\n");
}
void VSDX11Renderer::CreateDeclareTexCoord(MaterialShaderPara &MSPara, unsigned int& ID,
								  VSString & OutString,bool bInput)
{
	VSVertexBuffer * pVBuffer = MSPara.pGeometry->GetMeshData()->GetVertexBuffer();
	for(unsigned int i = 0 ; i < pVBuffer->GetTexCoordLevel(); i++)
	{

		if(pVBuffer->HaveTexCoordInfo(i))
		{
			VSString TextureID = IntToString(ID);
			VSString TextureNameID = IntToString(i);
			OutString += _T("float2 TexCoord") + TextureNameID + _T(":TEXCOORD") + TextureID + _T(";\n");
			ID++;
			if (bInput)
			{
				if (MSPara.uiCompileShaderType == VSEngineFlag::ST_PIXEL)
				{
					VSShaderStringFactory::ms_TextureInputCoordValue[i] = _T("ps_Input.TexCoord") + TextureNameID;
				}
				else if (MSPara.uiCompileShaderType == VSEngineFlag::ST_DOMAIN)
				{
					VSShaderStringFactory::ms_TextureInputCoordValue[i] = _T("DSTexCoord") + TextureNameID;
				}
			}
		}
	}
}
void VSDX11Renderer::CreateDeclareNormal(MaterialShaderPara &MSPara, unsigned int& ID,
								VSString & OutString, bool bInput)
{
	VSVertexBuffer * pVBuffer = MSPara.pGeometry->GetMeshData()->GetVertexBuffer();
	if(pVBuffer->HaveNormalInfo(0))
	{		
		VSString TextureID = IntToString(ID);

		OutString += _T("float3 Normal:TEXCOORD") + TextureID + _T(";\n");
		ID++;
		if (bInput)
		{
			if (MSPara.uiCompileShaderType == VSEngineFlag::ST_PIXEL)
			{
				VSShaderStringFactory::ms_InputLocalNormal = _T("ps_Input.Normal");
			}
			else if (MSPara.uiCompileShaderType == VSEngineFlag::ST_DOMAIN)
			{
				VSShaderStringFactory::ms_InputLocalNormal = _T("DSNormal");
			}
		}
	}




	if(pVBuffer->HaveTangentInfo())
	{
		{
			VSString TextureID = IntToString(ID);

			OutString += _T("float3 Tangent:TEXCOORD") + TextureID + _T(";\n");
			ID++;
			if (bInput)
			{
				if (MSPara.uiCompileShaderType == VSEngineFlag::ST_PIXEL)
				{
					VSShaderStringFactory::ms_InputLocalTangent = _T("ps_Input.Tangent");
				}
				else if (MSPara.uiCompileShaderType == VSEngineFlag::ST_DOMAIN)
				{
					VSShaderStringFactory::ms_InputLocalTangent = _T("DSTangent");
				}
			}
		}
		{
			VSString TextureID = IntToString(ID);

			OutString += _T("float3 Binormal:TEXCOORD") + TextureID + _T(";\n");
			ID++;
			if (bInput)
			{
				if (MSPara.uiCompileShaderType == VSEngineFlag::ST_PIXEL)
				{
					VSShaderStringFactory::ms_InputLocalBinormal = _T("ps_Input.Binormal");
				}
				else if (MSPara.uiCompileShaderType == VSEngineFlag::ST_DOMAIN)
				{
					VSShaderStringFactory::ms_InputLocalBinormal = _T("DSBinormal");
				}
			}
		}
	}
}
void VSDX11Renderer::CreateDeclareColor(MaterialShaderPara &MSPara,
							   VSString & OutString, bool bInput)
{
	VSVertexBuffer * pVBuffer = MSPara.pGeometry->GetMeshData()->GetVertexBuffer();
	for(unsigned int i = 0 ; i < pVBuffer->GetColorLevel(); i++)
	{
		if(pVBuffer->HaveColorInfo(i))
		{
			VSString ColorID = IntToString(i);
			OutString += _T("float4 Color") + ColorID + _T(":COLOR") + ColorID + _T(";\n");
			if (bInput)
			{
				if (MSPara.uiCompileShaderType == VSEngineFlag::ST_PIXEL)
				{
					VSShaderStringFactory::ms_InputColor[i] = _T("ps_Input.Color") + ColorID;
				}
				else if (MSPara.uiCompileShaderType == VSEngineFlag::ST_DOMAIN)
				{
					VSShaderStringFactory::ms_InputColor[i] = _T("DSColor") + ColorID;
				}
			}
		}
	}
}
void VSDX11Renderer::CreateDeclareTessellationValue(MaterialShaderPara &MSPara, VSString & OutString, bool bInput)
{
	OutString += _T("float TessellationValue:TESSELLATION_VALUE;\n");
}
void VSDX11Renderer::CreateDeclareWorldPos(MaterialShaderPara &MSPara, unsigned int& ID, VSString & OutString, bool bInput)
{
	VSString TextureID = IntToString(ID);
	OutString += _T("float3 WorldPos:TEXCOORD") + TextureID + _T(";\n");
	ID++;
	if (bInput)
	{
		if (MSPara.uiCompileShaderType == VSEngineFlag::ST_PIXEL)
		{
			VSShaderStringFactory::ms_InputWorldPos = _T("ps_Input.WorldPos");
		}
		else if (MSPara.uiCompileShaderType == VSEngineFlag::ST_DOMAIN)
		{
			VSShaderStringFactory::ms_InputWorldPos = _T("DSWorldPos");
		}
	}
}
void VSDX11Renderer::CreateDeclareProjectZ(MaterialShaderPara &MSPara, unsigned int& ID,
								  VSString & OutString, bool bInput)
{
	VSString TextureID = IntToString(ID);
	OutString += _T("float ProjectZ:TEXCOORD") + TextureID + _T(";\n");
	ID++;
	if (bInput)
	{
		if (MSPara.uiCompileShaderType == VSEngineFlag::ST_PIXEL)
		{
			VSShaderStringFactory::ms_InputProjectZ = _T("ps_Input.ProjectZ");
		}
	}
}
void VSDX11Renderer::CreateVOutputDeclare(MaterialShaderPara &MSPara,
										 VSString & OutString)
{
	VSString TempDeclare;
	
	unsigned int ID = 0;
	unsigned int uiPassType = MSPara.uiPassType;
	VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();

	if (uiPassType != VSPass::PT_POINT_VOLUME_SHADOW && uiPassType != VSPass::PT_DIRECT_VOLUME_SHADOW)
	{
		if (!MSPara.bUseTessellation)
		{
			CreateDeclarePosition(MSPara, TempDeclare,false);
		}
		//must first uv coord , Volume Shadow use file gshader.
		if (uiPassType == VSPass::PT_MATERIAL
			|| uiPassType == VSPass::PT_INDIRECT
			|| uiPassType == VSPass::PT_POINT_CUBE_SHADOW
			|| uiPassType == VSPass::PT_SHADOW
			|| uiPassType == VSPass::PT_NORMALDEPTH
			|| uiPassType == VSPass::PT_DUAL_PARABOLOID_SHADOW
			|| uiPassType == VSPass::PT_PREZ)
		{
			CreateDeclareWorldPos(MSPara, ID, TempDeclare,false);
		}
		CreateDeclareTexCoord(MSPara, ID, TempDeclare, false);
		CreateDeclareNormal(MSPara, ID, TempDeclare, false);
		CreateDeclareColor(MSPara,  TempDeclare, false);



		if (uiPassType == VSPass::PT_DUAL_PARABOLOID_SHADOW)
		{
			if (!MSPara.bUseTessellation)
			{
				CreateDeclareProjectZ(MSPara, ID, TempDeclare, false);
			}
		}
		if (uiPassType == VSPass::PT_MATERIAL
			|| uiPassType == VSPass::PT_INDIRECT
			|| uiPassType == VSPass::PT_POINT_CUBE_SHADOW
			|| uiPassType == VSPass::PT_SHADOW
			|| uiPassType == VSPass::PT_NORMALDEPTH
			|| uiPassType == VSPass::PT_DUAL_PARABOLOID_SHADOW
			|| uiPassType == VSPass::PT_PREZ)
		{
			
			if (MSPara.pGeometry->IsSameType(VSInstanceGeometry::ms_Type))
			{
				CreateDeclareInstancePos(MSPara, ID, TempDeclare, false);
				CreateDeclareMaterialInstance(MSPara, TempDeclare, false);
			}
			if (MSPara.bUseTessellation)
			{
				CreateDeclareTessellationValue(MSPara, TempDeclare, false);
			}
		}
	}
	else
	{
		CreateDeclareWorldPos(MSPara, ID, TempDeclare, false);
	}
	OutString += _T("struct VS_OUTPUT \n{\n") + TempDeclare + _T("};\n");
}
void VSDX11Renderer::CreateDeclareInstancePos(MaterialShaderPara &MSPara, unsigned int& ID,
	VSString & OutString, bool bInput)
{
	VSString TextureID = IntToString(ID);
	OutString += _T("float4 Position1:TEXCOORD") + TextureID + _T(";\n");
	ID++;

	TextureID = IntToString(ID);
	OutString += _T("float4 Position2:TEXCOORD") + TextureID + _T(";\n");
	ID++;

	TextureID = IntToString(ID);
	OutString += _T("float4 Position3:TEXCOORD") + TextureID + _T(";\n");
	ID++;

	if (bInput)
	{
		if (MSPara.uiCompileShaderType == VSEngineFlag::ST_PIXEL)
		{
			VSShaderStringFactory::ms_InputInstancePos[0] = _T("ps_Input.Position1");
			VSShaderStringFactory::ms_InputInstancePos[1] = _T("ps_Input.Position2");
			VSShaderStringFactory::ms_InputInstancePos[2] = _T("ps_Input.Position3");
		}
		else if (MSPara.uiCompileShaderType == VSEngineFlag::ST_DOMAIN)
		{
			VSShaderStringFactory::ms_InputInstancePos[0] = _T("DSPosition1");
			VSShaderStringFactory::ms_InputInstancePos[1] = _T("DSPosition2");
			VSShaderStringFactory::ms_InputInstancePos[2] = _T("DSPosition3");
		}
	}
}
void VSDX11Renderer::CreateDeclareMaterialInstance(MaterialShaderPara &MSPara,
	VSString & OutString, bool bInput)
{
	VSVertexBuffer * pInstanceVertex = ((VSInstanceGeometry *)MSPara.pGeometry)->GetInstanceVertexBuffer();
	for (unsigned int i = 0; i < pInstanceVertex->GetMaterialInstanceLevel(); i++)
	{
		if (pInstanceVertex->HaveMaterialInstanceInfo(i))
		{
			VSString MaterialInstanceID = IntToString(i);
			OutString += FloatByChannelNum(pInstanceVertex->MaterialInstanceChannel(i)) + _T("MaterialInstance") + MaterialInstanceID + _T(":MATERIAL_INSTANCE") + MaterialInstanceID + _T(";\n");
			if (bInput)
			{
				if (MSPara.uiCompileShaderType == VSEngineFlag::ST_PIXEL)
				{
					VSString InputString = _T("ps_Input.MaterialInstance") + MaterialInstanceID;
					VSShaderStringFactory::ms_InputMaterialInstance.AddElement(InputString);
				}
				else if (MSPara.uiCompileShaderType == VSEngineFlag::ST_DOMAIN)
				{
					VSString InputString = _T("DSMaterialInstance") + MaterialInstanceID;
					VSShaderStringFactory::ms_InputMaterialInstance.AddElement(InputString);
				}
			}
		}
	}
}
void VSDX11Renderer::CreateUserConstantLightShadowMatrix(VSShader * pShader,unsigned int& ID,
															 VSString & OutString)
{
	OutString += _T("row_major float4x4 ") + *VSShaderStringFactory::ms_LightShadowMatrix + _T(";\n");

	VSUserConstant * pUserConstant = VS_NEW VSUserConstant(*VSShaderStringFactory::ms_LightShadowMatrix,sizeof(VSREAL)* 16, 0, 4);
	pShader->m_pUserConstant.AddElement(pUserConstant);

}

void VSDX11Renderer::CreateUserConstantLightWorldPos(VSShader * pShader,unsigned int& ID,
									 VSString & OutString)
{
	OutString += _T("float3 ") + *VSShaderStringFactory::ms_LightWorldPos + _T(";\n");

	VSUserConstant * pUserConstant = VS_NEW VSUserConstant(*VSShaderStringFactory::ms_LightWorldPos,sizeof(VSREAL) * 3,0,1);
	pShader->m_pUserConstant.AddElement(pUserConstant);

}
void VSDX11Renderer::CreateUserConstantLightDirection(VSShader * pShader,unsigned int& ID,
									  VSString & OutString)
{
	OutString += _T("float3 ") + *VSShaderStringFactory::ms_LightWorldDirection + _T(";\n");

	VSUserConstant * pUserConstant = VS_NEW VSUserConstant(*VSShaderStringFactory::ms_LightWorldDirection,sizeof(VSREAL) * 3,0,1);
	pShader->m_pUserConstant.AddElement(pUserConstant);

}
void VSDX11Renderer::CreateUserConstantViewMatrix(VSShader * pShader,unsigned int& ID,
												 VSString & OutString)
{
	OutString += _T("row_major float4x4 ") + *VSShaderStringFactory::ms_ViewMatrix +_T(";\n");

	VSUserConstant * pUserConstant = VS_NEW VSUserConstant(*VSShaderStringFactory::ms_ViewMatrix,sizeof(VSREAL) * 16,0,4);
	pShader->m_pUserConstant.AddElement(pUserConstant);
}
void VSDX11Renderer::CreateUserConstantProjectMatrix(VSShader * pShader, unsigned int& ID,
	VSString & OutString)
{

	OutString += _T("row_major float4x4 ") + *VSShaderStringFactory::ms_ProjectMatrix + _T(";\n");

	VSUserConstant * pUserConstant = VS_NEW VSUserConstant(*VSShaderStringFactory::ms_ProjectMatrix,sizeof(VSREAL)* 16, 0, 4);
	pShader->m_pUserConstant.AddElement(pUserConstant);

}
void VSDX11Renderer::CreateUserConstantViewProjectMatrix(VSShader * pShader,unsigned int& ID,
															 VSString & OutString)
{

	OutString += _T("row_major float4x4 ") + *VSShaderStringFactory::ms_ViewProjectMatrix + _T(";\n");

	VSUserConstant * pUserConstant = VS_NEW VSUserConstant(VSShaderStringFactory::ms_ViewProjectMatrix,sizeof(VSREAL) * 16,0,4);
	pShader->m_pUserConstant.AddElement(pUserConstant);

}
void VSDX11Renderer::CreateUserConstantMorph(VSGeometry * pGeometry, VSShader * pShader, unsigned int& ID,
	VSString & OutString)
{
	if (pGeometry->HasMorphTarget())
	{
		OutString += _T("uint4 ") + *VSShaderStringFactory::ms_MorphData + _T(";\n");
		VSUserConstant * pUserConstant = VS_NEW VSUserConstant(*VSShaderStringFactory::ms_MorphData, sizeof(unsigned int) * 4, 0, 1,VSUserConstant::VT_INT);
		pShader->m_pUserConstant.AddElement(pUserConstant);

		unsigned int uiRegisterNum = (VSResourceManager::GetGpuMorphTargetNum() >> 2);
		VSString MorphTargetNum = IntToString(uiRegisterNum);
		OutString += _T("float4 ") + *VSShaderStringFactory::ms_MorphValue + _T("[") + MorphTargetNum + _T("];\n");
		pUserConstant = VS_NEW VSUserConstant(*VSShaderStringFactory::ms_MorphValue, sizeof(VSREAL) * 4 * uiRegisterNum, 0, uiRegisterNum, VSUserConstant::VT_FLOAT);
		pShader->m_pUserConstant.AddElement(pUserConstant);
	}
}
void VSDX11Renderer::CreateUserConstantSimpleSkinInstanceTexture(VSGeometry * pGeometry, VSShader * pShader, unsigned int& uiTexRegisterID,
	VSString & OutString)
{
	VSInstanceGeometry * pInstanceGeometry = DynamicCast<VSInstanceGeometry>(pGeometry);
	if (!pInstanceGeometry)
	{
		return;
	}
	if (!pInstanceGeometry->HaveSkinInstance())
	{
		return;
	}

	OutString += Sampler(VSTexture::TT_2D) + _T(" ");
	OutString += *VSShaderStringFactory::ms_VSAnimDataResource + _T("Texture")
		+ SetRegister(RT_S, uiTexRegisterID) + _T(";\n");

	VSUserSampler * pSampler = VS_NEW VSUserSampler(*VSShaderStringFactory::ms_VSAnimDataResource, VSTexture::TT_2D, uiTexRegisterID, 1);
	pShader->m_pUserSampler.AddElement(pSampler);
	uiTexRegisterID++;
}
void VSDX11Renderer::CreateUserConstantMorphTexture(VSGeometry * pGeometry, VSShader * pShader, unsigned int& uiTexRegisterID,
	VSString & OutString)
{
	if (!pGeometry->HasMorphTarget())
	{
		return;
	}

	OutString += Sampler(VSTexture::TT_2D) + _T(" ");
	OutString += *VSShaderStringFactory::ms_VSMorphVDataResource + _T("Texture")
		+ SetRegister(RT_T, uiTexRegisterID) + _T(";\n");

	VSUserSampler * pSampler = VS_NEW VSUserSampler(*VSShaderStringFactory::ms_VSMorphVDataResource, VSTexture::TT_2D, uiTexRegisterID, 1);
	pShader->m_pUserSampler.AddElement(pSampler);
	uiTexRegisterID++;

	VSVertexBuffer * pVBuffer = pGeometry->GetMeshData()->GetVertexBuffer();
	if (pVBuffer->HaveNormalInfo(0))
	{

		OutString += Sampler(VSTexture::TT_2D) + _T(" ");
		OutString += *VSShaderStringFactory::ms_VSMorphNDataResource + _T("Texture")
			+ SetRegister(RT_T, uiTexRegisterID) + _T(";\n");

		VSUserSampler * pSampler = VS_NEW VSUserSampler(*VSShaderStringFactory::ms_VSMorphNDataResource, VSTexture::TT_2D, uiTexRegisterID, 1);
		pShader->m_pUserSampler.AddElement(pSampler);
		uiTexRegisterID++;
	}


	if (pVBuffer->HaveTangentInfo())
	{

		OutString += Sampler(VSTexture::TT_2D) + _T(" ");
		OutString += *VSShaderStringFactory::ms_VSMorphTDataResource + _T("Texture")
			+ SetRegister(RT_T, uiTexRegisterID) + _T(";\n");

		VSUserSampler * pSampler = VS_NEW VSUserSampler(*VSShaderStringFactory::ms_VSMorphTDataResource, VSTexture::TT_2D, uiTexRegisterID, 1);
		pShader->m_pUserSampler.AddElement(pSampler);
		uiTexRegisterID++;
		if (pVBuffer->HaveBinormalInfo())
		{

			OutString += Sampler(VSTexture::TT_2D) + _T(" ");
			OutString += *VSShaderStringFactory::ms_VSMorphBDataResource + _T("Texture")
				+ SetRegister(RT_T, uiTexRegisterID) + _T(";\n");

			VSUserSampler * pSampler = VS_NEW VSUserSampler(*VSShaderStringFactory::ms_VSMorphBDataResource, VSTexture::TT_2D, uiTexRegisterID, 1);
			pShader->m_pUserSampler.AddElement(pSampler);
			uiTexRegisterID++;
		}
	}
}
void VSDX11Renderer::CreateUserConstantSkin(VSGeometry * pGeometry,VSShader * pShader,unsigned int& ID,
							VSString & OutString)
{
	VSVertexBuffer * pVBuffer = pGeometry->GetMeshData()->GetVertexBuffer();
	VSInstanceGeometry * pInstanceGeometry = DynamicCast<VSInstanceGeometry>(pGeometry);
	if (pInstanceGeometry && pVBuffer->HaveBlendIndicesInfo(1))
	{
		OutString += _T("uint4 ") + *VSShaderStringFactory::ms_AnimAllInfo + _T(";\n");

		VSUserConstant * pUserConstant = VS_NEW VSUserConstant(*VSShaderStringFactory::ms_AnimAllInfo, sizeof(unsigned int) * 4, 0, 1, VSUserConstant::VT_INT);
		pShader->m_pUserConstant.AddElement(pUserConstant);
	}
	else if (pVBuffer->HaveBlendIndicesInfo(0))
	{

		unsigned int uiBoneNum = VSResourceManager::GetGpuSkinBoneNum();
		VSString BoneNum = IntToString(uiBoneNum * 3);
		OutString += _T("float4 ") + *VSShaderStringFactory::ms_BoneMatrix + _T("[") + BoneNum + _T("]")+ _T(";\n");

		VSUserConstant * pUserConstant = VS_NEW VSUserConstant(*VSShaderStringFactory::ms_BoneMatrix,sizeof(VSREAL) * 4 * uiBoneNum * 3,0,uiBoneNum * 3);
		pShader->m_pUserConstant.AddElement(pUserConstant);
	}
}
void VSDX11Renderer::CreateVUserConstant(VSVShader * pVShader,MaterialShaderPara &MSPara,
										VSString & OutString)
{
	OutString += _T("cbuffer CB : register( b0 ) \n{\n");
	unsigned int uiRegisterID = 0;
	unsigned int uiTexRegisterID = 0;
	unsigned int uiPassType = MSPara.uiPassType;
	if (uiPassType == VSPass::PT_MATERIAL 
		|| uiPassType == VSPass::PT_PREZ 
		|| uiPassType == VSPass::PT_NORMALDEPTH 
		|| uiPassType == VSPass::PT_INDIRECT
		|| uiPassType == VSPass::PT_POINT_CUBE_SHADOW)
	{
		CreateUserConstantWorldMatrix(pVShader, uiRegisterID, OutString);
		CreateUserConstantViewProjectMatrix(pVShader,uiRegisterID,OutString);
		CreateUserConstantCameraWorldPos(pVShader, uiRegisterID, OutString);
		CreateUserConstantSkin(MSPara.pGeometry,pVShader,uiRegisterID,OutString);
		CreateUserConstantMorph(MSPara.pGeometry, pVShader, uiRegisterID, OutString);
	}
	else if (uiPassType == VSPass::PT_POINT_VOLUME_SHADOW || uiPassType == VSPass::PT_DIRECT_VOLUME_SHADOW)
	{
		CreateUserConstantWorldMatrix(pVShader,uiRegisterID,OutString);
		CreateUserConstantCameraWorldPos(pVShader, uiRegisterID, OutString);
		CreateUserConstantSkin(MSPara.pGeometry,pVShader,uiRegisterID,OutString);
	}
	else if (uiPassType == VSPass::PT_SHADOW)
	{
		CreateUserConstantWorldMatrix(pVShader, uiRegisterID, OutString);
		CreateUserConstantLightShadowMatrix(pVShader,uiRegisterID,OutString);
		CreateUserConstantCameraWorldPos(pVShader, uiRegisterID, OutString);
		CreateUserConstantSkin(MSPara.pGeometry,pVShader,uiRegisterID,OutString);
		CreateUserConstantMorph(MSPara.pGeometry, pVShader, uiRegisterID, OutString);
	}
	else if (uiPassType == VSPass::PT_DUAL_PARABOLOID_SHADOW)
	{
		CreateUserConstantWorldMatrix(pVShader, uiRegisterID, OutString);
		CreateUserConstantViewProjectMatrix(pVShader, uiRegisterID, OutString);
		CreateUserConstantCameraWorldPos(pVShader, uiRegisterID, OutString);
		CreateUserConstantPointLightRange(pVShader,uiRegisterID,OutString);
		CreateUserConstantSkin(MSPara.pGeometry,pVShader,uiRegisterID,OutString);
		CreateUserConstantMorph(MSPara.pGeometry, pVShader, uiRegisterID, OutString);
	}
	else if (uiPassType == VSPass::PT_LIGHT_FUNCTION)
	{
		//CreateUserConstantInvRTWidth(pVShader,uiRegisterID,OutString);
	}
	if (uiPassType == VSPass::PT_MATERIAL
		|| uiPassType == VSPass::PT_PREZ
		|| uiPassType == VSPass::PT_NORMALDEPTH
		|| uiPassType == VSPass::PT_INDIRECT
		|| uiPassType == VSPass::PT_POINT_CUBE_SHADOW
		|| uiPassType == VSPass::PT_POINT_VOLUME_SHADOW
		|| uiPassType == VSPass::PT_DIRECT_VOLUME_SHADOW
		|| uiPassType == VSPass::PT_SHADOW
		|| uiPassType == VSPass::PT_DUAL_PARABOLOID_SHADOW
		)
	{
		VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		pMaterial->CreateConstValueDeclare(OutString, uiRegisterID, MSPara);
		pMaterial->CreateCustomValue(pVShader);
		OutString += _T("} \n");
		unsigned uiTexRegisterID = 0;
		pMaterial->CreateTextureDeclare(OutString, uiTexRegisterID, MSPara);
		pMaterial->CreateCustomTexture(pVShader);
	}
	else
	{
		OutString += _T("} \n");
	}
	if (uiPassType == VSPass::PT_MATERIAL
		|| uiPassType == VSPass::PT_PREZ
		|| uiPassType == VSPass::PT_NORMALDEPTH
		|| uiPassType == VSPass::PT_INDIRECT
		|| uiPassType == VSPass::PT_POINT_CUBE_SHADOW
		||uiPassType == VSPass::PT_SHADOW
		||uiPassType == VSPass::PT_DUAL_PARABOLOID_SHADOW)
	{
		CreateUserConstantMorphTexture(MSPara.pGeometry, pVShader, uiTexRegisterID, OutString);
		CreateUserConstantSimpleSkinInstanceTexture(MSPara.pGeometry, pVShader, uiTexRegisterID, OutString);
	}

}
void VSDX11Renderer::CreateVFunctionDualParaboloidPosition(MaterialShaderPara &MSPara,VSString & FunctionBody)
{
	CreateVFunctionPosition(MSPara,FunctionBody);
	//no project matrix , in the view space
	FunctionBody += _T("float fLength = length(Out.Position.xyz);\n");
	FunctionBody += _T("Out.Position.xyz /= fLength;\n");
 	FunctionBody += _T("Out.Position.x /= Out.Position.z + 1.01f;\n");
 	FunctionBody += _T("Out.Position.y /= Out.Position.z + 1.01f;\n");
	FunctionBody += _T("Out.Position.z = saturate(fLength / ") + *VSShaderStringFactory::ms_PointLightRange + _T(");\n");
	

}
void VSDX11Renderer::CreateVFunctionSkinInfo(MaterialShaderPara &MSPara, VSString & FunctionBody)
{
	VSVertexBuffer * pVBuffer = MSPara.pGeometry->GetMeshData()->GetVertexBuffer();
	if (pVBuffer->HaveBlendIndicesInfo(0))
	{
		FunctionBody += _T("float4 U = 0;\n");
		FunctionBody += _T("float4 V = 0;\n");
		FunctionBody += _T("float4 N = 0;\n");
		VSInstanceGeometry * pInstanceGeometry = DynamicCast<VSInstanceGeometry>(MSPara.pGeometry);
		if (pInstanceGeometry && pVBuffer->HaveBlendIndicesInfo(1))
		{
			FunctionBody += _T("ANIM_INSTANCE_SHADER_DATA AnimInsData[") + IntToString(MAX_SIMPLE_INSTANCE_ANIM) + _T("]={");
			FunctionBody += VSShaderStringFactory::ms_InputInstanceAnimData + _T("};\n");

			VSRenderer::ms_pRenderer->ComputeBoneVector(_T("Input.BlendIndicesInstance"), _T("Input.BlendWeight"),
				*VSShaderStringFactory::ms_VSAnimDataResource + _T("Texture"), *VSShaderStringFactory::ms_AnimAllInfo, _T("AnimInsData"), VSShaderStringFactory::ms_InputInstanceAnimNum,
				"U", "V", "N", FunctionBody);
			FunctionBody += _T(";\n");
		}
		else
		{
			VSRenderer::ms_pRenderer->ComputeBoneVector(_T("Input.BlendIndices"), _T("Input.BlendWeight"), *VSShaderStringFactory::ms_BoneMatrix
				, "U", "V", "N", FunctionBody);
			FunctionBody += _T(";\n");
		}

	}
}
void VSDX11Renderer::CreateVFunctionVolumeShadowNormal(MaterialShaderPara &MSPara, VSString & FunctionBody)
{
	VSVertexBuffer * pVBuffer = MSPara.pGeometry->GetMeshData()->GetVertexBuffer();
	if (!pVBuffer->HaveNormalInfo(0))
	{
		return;
	}
	FunctionBody += _T("float3 Normal = Input.Normal0.xyz;\n");

	if (pVBuffer->NormalDataType(0) == VSDataBuffer::DT_UBYTE4N)
	{
		FunctionBody += _T("Normal = ");
		VSRenderer::ms_pRenderer->DecodeNormal1("Normal", FunctionBody);
		FunctionBody += _T(";\n");
	}

	if (pVBuffer->HaveBlendIndicesInfo(0))
	{
		FunctionBody += _T("Normal = ");
		VSRenderer::ms_pRenderer->BoneTranNormal(_T("Normal"), _T("U"), _T("V"), _T("N"), FunctionBody);
		FunctionBody += _T(";\n");
	}
	VSShaderStringFactory::ms_InputLocalNormal = _T("Normal");
}
void VSDX11Renderer::CreateVFunctionMorphData(MaterialShaderPara &MSPara, VSString & FunctionBody)
{
	if (!MSPara.pGeometry->HasMorphTarget())
	{
		return ;
	}
	VSVertexBuffer * pVBuffer = MSPara.pGeometry->GetMeshData()->GetVertexBuffer();
	FunctionBody += _T("float3 MorphVData = float3(0.0f,0.0f,0.0f);\n");
	if (pVBuffer->HaveNormalInfo(0))
	{
		FunctionBody += _T("float3 MorphNData = float3(0.0f,0.0f,0.0f);\n");
	}
	if (pVBuffer->HaveTangentInfo())
	{
		FunctionBody += _T("float4 MorphTData = float4(0.0f,0.0f,0.0f,0.0f);\n");		
	}
	if (pVBuffer->HaveBinormalInfo())
	{
		FunctionBody += _T("float3 MorphBData = float3(0.0f,0.0f,0.0f);\n");
	}

	VSString TextureSize = *VSShaderStringFactory::ms_MorphData + _T(".z");
	VSString MorphValue = *VSShaderStringFactory::ms_MorphValue + _T("[m][n]");
	VSString MorphLay = *VSShaderStringFactory::ms_MorphData + _T(".x");
	VSString MorphNum = *VSShaderStringFactory::ms_MorphData + _T(".y");

	FunctionBody += _T("for (uint i = 0 ; i < ") + MorphNum + _T(" ; i++){\n");
	FunctionBody += _T("uint y = VertexID / ") + TextureSize + _T(" + i * ") + MorphLay + _T(";\n");
	FunctionBody += _T("uint x = VertexID % ") + TextureSize + _T(" ;\n");
	FunctionBody += _T("uint m = i >> 2; uint n = i % 4;\n");
	FunctionBody += _T("float3 MorphVDataTemp = ") + *VSShaderStringFactory::ms_VSMorphVDataResource + _T("Texture[uint2(x,y)].xyz - Input.Position0;\n");
	FunctionBody += _T("MorphVData += MorphVDataTemp * ") + MorphValue + _T(";\n");
	if (pVBuffer->HaveNormalInfo(0))
	{
		FunctionBody += _T("float3 MorphNDataTemp = ") + *VSShaderStringFactory::ms_VSMorphNDataResource + _T("Texture[uint2(x,y)].xyz - Input.Normal0.xyz;\n");
		FunctionBody += _T("MorphNData += MorphNDataTemp * ") + MorphValue + _T(";\n");
	}
	if (pVBuffer->HaveTangentInfo())
	{
		FunctionBody += _T("float4 MorphTDataTemp = ") + *VSShaderStringFactory::ms_VSMorphTDataResource + _T("Texture[uint2(x,y)] - Input.Tangent;\n");
		FunctionBody += _T("MorphTData += MorphTDataTemp * ") + MorphValue + _T(";\n");
	}
	if (pVBuffer->HaveBinormalInfo())
	{
		FunctionBody += _T("float3 MorphBDataTemp = ") + *VSShaderStringFactory::ms_VSMorphBDataResource + _T("Texture[uint2(x,y)].xyz - Input.Binormal;\n");
		FunctionBody += _T("MorphBData += MorphBDataTemp * ") + MorphValue + _T(" ;\n");
	}
	FunctionBody += _T("}\n");
}
void VSDX11Renderer::CreateVFunctionNormal(MaterialShaderPara &MSPara, VSString & FunctionBody)
{
	VSVertexBuffer * pVBuffer = MSPara.pGeometry->GetMeshData()->GetVertexBuffer();
	bool HasMorph = MSPara.pGeometry->HasMorphTarget();
	if (!pVBuffer->HaveNormalInfo(0))
	{		
		return;
	}
	FunctionBody += _T("float3 Normal = Input.Normal0.xyz;\n");
	if (HasMorph)
	{
		FunctionBody += _T("Normal += MorphNData;\n");
	}
	if (pVBuffer->HaveTangentInfo())
	{
		FunctionBody += _T("float4 Tangent = Input.Tangent;\n");
		if (HasMorph)
		{
			FunctionBody += _T("Tangent += MorphTData;\n");
		}
		if (pVBuffer->HaveBinormalInfo())
		{
			
			FunctionBody += _T("float3 Binormal = Input.Binormal;\n");
			if (HasMorph)
			{
				FunctionBody += _T("Binormal += MorphBData;\n");
			}

		}
		else
		{
			FunctionBody += _T("float3 Binormal;\n");
		}		
	}
	if (pVBuffer->NormalDataType(0) == VSDataBuffer::DT_UBYTE4N)
	{
		FunctionBody += _T("Normal = ");
		VSRenderer::ms_pRenderer->DecodeNormal1("Normal", FunctionBody);
		FunctionBody += _T(";\n");
	}

	if (pVBuffer->HaveTangentInfo())
	{
		if (pVBuffer->TangentDataType() == VSDataBuffer::DT_UBYTE4N)
		{
			FunctionBody += _T("Tangent = ");
			VSRenderer::ms_pRenderer->DecodeNormal1("Tangent", FunctionBody);
			FunctionBody += _T(";\n");
		}
		if (pVBuffer->HaveBinormalInfo())
		{
			FunctionBody += _T("Binormal = Input.Binormal;\n");
		}
		else
		{
			FunctionBody += _T("Binormal = ");
			VSRenderer::ms_pRenderer->DecodeNormal2(_T("Normal"), _T("Tangent"), FunctionBody);
			FunctionBody += _T(";\n");
		}
	}

	if (pVBuffer->HaveBlendIndicesInfo(0))
	{				
		FunctionBody += _T("Normal = ");
		VSRenderer::ms_pRenderer->BoneTranNormal(_T("Normal"), _T("U"), _T("V"), _T("N"), FunctionBody);
		FunctionBody += _T(";\n");

		if (pVBuffer->HaveTangentInfo())
		{
			FunctionBody += _T("Tangent.xyz = ");
			VSRenderer::ms_pRenderer->BoneTranNormal(_T("Tangent.xyz"), _T("U"), _T("V"), _T("N"), FunctionBody);
			FunctionBody += _T(";\n");

			FunctionBody += _T("Binormal = ");
			VSRenderer::ms_pRenderer->BoneTranNormal(_T("Binormal"), _T("U"), _T("V"), _T("N"), FunctionBody);
			FunctionBody += _T(";\n");
		}

	}



	FunctionBody += _T("Out.Normal = Normal;\n");
	VSShaderStringFactory::ms_InputLocalNormal = _T("Normal");

	if (pVBuffer->HaveTangentInfo())
	{
		FunctionBody += _T("Out.Tangent = Tangent.xyz;\n");
		FunctionBody += _T("Out.Binormal = Binormal;\n");
	}
}
void VSDX11Renderer::CreateVFunctionPosition(MaterialShaderPara &MSPara,VSString & FunctionBody)
{
	VSVertexBuffer * pVBuffer = MSPara.pGeometry->GetMeshData()->GetVertexBuffer();
	FunctionBody +=_T("float3 Position = Input.Position0;\n");
	bool HasMorph = MSPara.pGeometry->HasMorphTarget();
	if (HasMorph)
	{
		FunctionBody += _T("Position += MorphVData;\n");
	}
	if (pVBuffer->HaveBlendIndicesInfo(0))
	{

		FunctionBody += _T("Position = ");
		VSRenderer::ms_pRenderer->BoneTranPos(_T("Position"), _T("U"), _T("V"), _T("N"), FunctionBody);
		FunctionBody +=_T(";\n");
	}

	VSShaderStringFactory::ms_InputLocalPos = _T("Position");

	VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
	pMaterial->GetVShaderTreeString(FunctionBody, MSPara);
	if (MSPara.pGeometry->IsSameType(VSInstanceGeometry::ms_Type))
	{
		FunctionBody += _T("Out.Position1 = Input.Position1;\nOut.Position2 = Input.Position2;\nOut.Position3 = Input.Position3;\n");
	}
	if (MSPara.bUseTessellation)
	{
		FunctionBody += _T("Out.TessellationValue = ") + *VSShaderStringFactory::ms_TessellationValue + _T("; \n");
		// 0-1 map to 1 - MaxTessFactor 
		VSString MaxTessFactor = RealToString(ms_fHSMaxTessFactor - 1);
		FunctionBody += _T("Out.TessellationValue = clamp(Out.TessellationValue ,0.0f,1.0f) * ") + MaxTessFactor + _T(" + 1.0f;\n");
	}
	FunctionBody += _T("Out.WorldPos = ") + *VSShaderStringFactory::ms_WorldPos + _T("; \n");
	if (!MSPara.bUseTessellation)
	{
		FunctionBody += _T("Out.Position = mul(float4(") + *VSShaderStringFactory::ms_WorldPos + _T(",1), ") +
			*VSShaderStringFactory::ms_ViewProjectMatrix + _T(");\n");
	}
}
void VSDX11Renderer::CreateVFunctionMaterialInstance(MaterialShaderPara &MSPara, VSString & FunctionBody)
{
	VSVertexBuffer * pInstanceVertex = ((VSInstanceGeometry *)MSPara.pGeometry)->GetInstanceVertexBuffer();
	for (unsigned int i = 0; i < pInstanceVertex->GetMaterialInstanceLevel(); i++)
	{
		if (pInstanceVertex->HaveMaterialInstanceInfo(i))
		{
			VSString MaterialInstanceID = IntToString(i);
			FunctionBody += _T("Out.MaterialInstance") + MaterialInstanceID + _T(" = ") +_T("Input.MaterialInstance") + MaterialInstanceID + _T(";\n");
		}
	}
}
void VSDX11Renderer::CreateVFunctionVolumeShadowPosition(MaterialShaderPara &MSPara, VSString & FunctionBody)
{
	VSVertexBuffer * pVBuffer = MSPara.pGeometry->GetMeshData()->GetVertexBuffer();
	FunctionBody +=_T("float3 Position;\n");

	if(pVBuffer->HaveBlendIndicesInfo(0))
	{
		FunctionBody += _T("Position = ");
		VSRenderer::ms_pRenderer->BoneTranPos(_T("Input.Position0"), _T("U"), _T("V"), _T("N"), FunctionBody);
		FunctionBody +=_T(";\n");

	}
	else
	{	
		FunctionBody +=_T("Position = Input.Position0;\n");
	}
	VSShaderStringFactory::ms_InputLocalPos = _T("Position");

	VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
	pMaterial->GetVShaderTreeString(FunctionBody, MSPara);

	FunctionBody += _T("Out.WorldPos = ") + *VSShaderStringFactory::ms_WorldPos + _T("; \n");
}
void VSDX11Renderer::CreateVFunctionColor(MaterialShaderPara &MSPara,VSString & FunctionBody)
{
	VSVertexBuffer * pVBuffer = MSPara.pGeometry->GetMeshData()->GetVertexBuffer();
	for(unsigned int i = 0 ; i < pVBuffer->GetColorLevel(); i++)
	{

		if(pVBuffer->HaveColorInfo(i))
		{
			VSString ColorID = IntToString(i);
			FunctionBody += _T("Out.Color") + ColorID + _T(" = Input.Color") + ColorID + _T(";\n");
		}
	}
}
void VSDX11Renderer::CreateVFunctionTexCoord(MaterialShaderPara &MSPara,VSString & FunctionBody)
{
	VSVertexBuffer * pVBuffer = MSPara.pGeometry->GetMeshData()->GetVertexBuffer();
	for(unsigned int i = 0 ; i < pVBuffer->GetTexCoordLevel(); i++)
	{
		if(pVBuffer->HaveTexCoordInfo(0))
		{
			VSString TextureID = IntToString(i);
			FunctionBody += _T("Out.TexCoord") + TextureID + _T(" = Input.TexCoord") + TextureID + _T(";\n");
		}
	}
}
void VSDX11Renderer::CreateVFunctionProjectZ(MaterialShaderPara &MSPara,VSString & FunctionBody)
{
	FunctionBody += _T("Out.ProjectZ = Out.Position.z / Out.Position.w;\n");
}
void VSDX11Renderer::CreateVFunction(MaterialShaderPara &MSPara,
								   VSString & OutString)
{
	VSString FunctionBody;
	unsigned int uiPassType = MSPara.uiPassType;
	if (uiPassType == VSPass::PT_MATERIAL 
		|| uiPassType == VSPass::PT_INDIRECT
		|| uiPassType == VSPass::PT_POINT_CUBE_SHADOW
		|| uiPassType == VSPass::PT_SHADOW
		|| uiPassType == VSPass::PT_NORMALDEPTH)
	{
		CreateVFunctionMorphData(MSPara, FunctionBody);
		CreateVFunctionSkinInfo(MSPara, FunctionBody);
		CreateVFunctionNormal(MSPara, FunctionBody);
		CreateVFunctionPosition(MSPara, FunctionBody);
		CreateVFunctionColor(MSPara,FunctionBody);
		CreateVFunctionTexCoord(MSPara,FunctionBody);
		if (MSPara.pGeometry->IsSameType(VSInstanceGeometry::ms_Type))
		{
			CreateVFunctionMaterialInstance(MSPara, FunctionBody);
		}
	}
	else if (uiPassType == VSPass::PT_POINT_VOLUME_SHADOW || uiPassType == VSPass::PT_DIRECT_VOLUME_SHADOW)
	{
		CreateVFunctionSkinInfo(MSPara, FunctionBody);
		CreateVFunctionVolumeShadowNormal(MSPara, FunctionBody);
		CreateVFunctionVolumeShadowPosition(MSPara, FunctionBody);
	}
	else if (uiPassType == VSPass::PT_DUAL_PARABOLOID_SHADOW)
	{
		CreateVFunctionMorphData(MSPara, FunctionBody);
		CreateVFunctionSkinInfo(MSPara, FunctionBody);
		CreateVFunctionNormal(MSPara, FunctionBody);
		CreateVFunctionDualParaboloidPosition(MSPara,FunctionBody);
		CreateVFunctionColor(MSPara,FunctionBody);
		CreateVFunctionTexCoord(MSPara,FunctionBody);
		if (!MSPara.bUseTessellation)
		{
			CreateVFunctionProjectZ(MSPara, FunctionBody);
		}	
		if (MSPara.pGeometry->IsSameType(VSInstanceGeometry::ms_Type))
		{
			CreateVFunctionMaterialInstance(MSPara, FunctionBody);
		}
	}
	else if (uiPassType == VSPass::PT_PREZ)
	{	
		CreateVFunctionPosition(MSPara,FunctionBody);
		if (MSPara.pGeometry->IsSameType(VSInstanceGeometry::ms_Type))
		{
			CreateVFunctionMaterialInstance(MSPara, FunctionBody);
		}
	}
	else if (uiPassType == VSPass::PT_LIGHT_FUNCTION)
	{
		CreateVFunctionPost(MSPara,FunctionBody);
	}
	OutString += _T("VS_OUTPUT ") + ms_ShaderProgramMain[VSEngineFlag::ST_VERTEX] + _T("( VS_INPUT Input,uint VertexID : SV_VertexID )\n{\nVS_OUTPUT Out = (VS_OUTPUT) 0; \n") + FunctionBody + _T("return Out;\n};\n");	
}
void VSDX11Renderer::CreateVFunctionPost(MaterialShaderPara &MSPara,VSString & FunctionBody)
{
	FunctionBody += _T("Out.Position = float4(Input.Position0.xy, 0, 1);\n");
	FunctionBody += _T("Out.TexCoord0.xy = 0.5 * (1 + Input.Position0.xy * float2(1,-1));\n");
}
void VSDX11Renderer::CreatePInputDeclare(MaterialShaderPara &MSPara,
	VSString & OutString)
{
	VSString TempDeclare;
	
	unsigned int j = 0;
	
	unsigned int uiPassType = MSPara.uiPassType;
	CreateDeclarePosition(MSPara, TempDeclare,true);
	
	if (uiPassType != VSPass::PT_POINT_VOLUME_SHADOW && uiPassType != VSPass::PT_DIRECT_VOLUME_SHADOW)
	{
		if (uiPassType == VSPass::PT_MATERIAL
			|| uiPassType == VSPass::PT_INDIRECT
			|| uiPassType == VSPass::PT_POINT_CUBE_SHADOW
			|| uiPassType == VSPass::PT_SHADOW
			|| uiPassType == VSPass::PT_NORMALDEPTH
			|| uiPassType == VSPass::PT_DUAL_PARABOLOID_SHADOW
			|| uiPassType == VSPass::PT_PREZ)
		{
			CreateDeclareWorldPos(MSPara, j, TempDeclare,true);
		}
		CreateDeclareTexCoord(MSPara, j, TempDeclare, true);
		CreateDeclareNormal(MSPara, j, TempDeclare, true);
		CreateDeclareColor(MSPara, TempDeclare, true);
		if (uiPassType == VSPass::PT_DUAL_PARABOLOID_SHADOW)
		{
			CreateDeclareProjectZ(MSPara, j, TempDeclare, true);
		}
		if (uiPassType == VSPass::PT_MATERIAL
			|| uiPassType == VSPass::PT_INDIRECT
			|| uiPassType == VSPass::PT_POINT_CUBE_SHADOW
			|| uiPassType == VSPass::PT_SHADOW
			|| uiPassType == VSPass::PT_NORMALDEPTH
			|| uiPassType == VSPass::PT_DUAL_PARABOLOID_SHADOW
			|| uiPassType == VSPass::PT_PREZ)
		{				
			if (MSPara.pGeometry->IsSameType(VSInstanceGeometry::ms_Type))
			{
				CreateDeclareInstancePos(MSPara, j, TempDeclare, true);
				CreateDeclareMaterialInstance(MSPara, TempDeclare, true);
			}
		}
	}

	OutString += _T("struct PS_INPUT \n{ \n") + TempDeclare + _T("};\n");
}

void VSDX11Renderer::CreatePOutputDeclare(MaterialShaderPara &MSPara,
										 VSString & OutString)
{

	VSString TempDeclare;
	TempDeclare += _T("float4 Color0:SV_Target0;");
	VSShaderStringFactory::ms_PSOutputColorValue = _T("Out.Color0");
	OutString += _T("struct PS_OUTPUT \n{ \n") + TempDeclare + _T("\n};\n");

}
void VSDX11Renderer::CreateUserConstantWorldMatrix(VSShader * pShader,unsigned int& ID,
												  VSString & OutString)
{

	OutString += _T("row_major float4x4 ") + *VSShaderStringFactory::ms_WorldMatrix +  _T(";\n");
	VSUserConstant * pUserConstant = VS_NEW VSUserConstant(*VSShaderStringFactory::ms_WorldMatrix,sizeof(VSREAL) * 16,0,4);
	pShader->m_pUserConstant.AddElement(pUserConstant);

	
}
void VSDX11Renderer::CreateUserConstantCameraWorldPos(VSShader * pShader,unsigned int& ID,
													 VSString & OutString)
{
	OutString += _T("float3 ") + *VSShaderStringFactory::ms_CameraWorldPos + _T(";\n");		 
	VSUserConstant * pUserConstant = VS_NEW VSUserConstant(*VSShaderStringFactory::ms_CameraWorldPos,sizeof(VSREAL) * 3,0,1);
	pShader->m_pUserConstant.AddElement(pUserConstant);

}
void VSDX11Renderer::CreateUserConstantDirectionLight(VSShader * pShader,unsigned int& ID,unsigned int uiLightNum,
													 VSString & OutString)
{
	if (uiLightNum > 0)
	{
		VSString TypeString;
		GetLightType(VSLight::LT_DIRECTION,TypeString);

		OutString += TypeString + _T(" ") + *VSShaderStringFactory::ms_LightName[VSLight::LT_DIRECTION] + _T("[") + IntToString(uiLightNum) + _T("];\n");

		VSUserConstant * pUserConstant = VS_NEW VSUserConstant(*VSShaderStringFactory::ms_LightName[VSLight::LT_DIRECTION],sizeof(VSREAL) * 84 * uiLightNum,0,21 * uiLightNum);
		pShader->m_pUserConstant.AddElement(pUserConstant);

	}
	

}
void VSDX11Renderer::CreateUserConstantPointLight(VSShader * pShader,unsigned int& ID,unsigned int uiLightNum,
												 VSString & OutString)
{
	if (uiLightNum > 0)
	{
		VSString TypeString;
		GetLightType(VSLight::LT_POINT,TypeString);

		OutString += TypeString + _T(" ") + *VSShaderStringFactory::ms_LightName[VSLight::LT_POINT] + _T("[") + IntToString(uiLightNum) + _T("];\n");

		VSUserConstant * pUserConstant = VS_NEW VSUserConstant(*VSShaderStringFactory::ms_LightName[VSLight::LT_POINT],sizeof(VSREAL) * 52 * uiLightNum,0,13 * uiLightNum);
		pShader->m_pUserConstant.AddElement(pUserConstant);

	}
}
void VSDX11Renderer::CreateUserConstantSpotLight(VSShader * pShader,unsigned int& ID,unsigned int uiLightNum,
												VSString & OutString)
{
	if (uiLightNum > 0)
	{
		VSString TypeString;
		GetLightType(VSLight::LT_SPOT,TypeString);
		VSString RegisterID = IntToString(ID);
		OutString += TypeString + _T(" ") + *VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + IntToString(uiLightNum) + _T("];\n");

		VSUserConstant * pUserConstant = VS_NEW VSUserConstant(*VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT],sizeof(VSREAL) * 56 * uiLightNum,0,14 * uiLightNum);
		pShader->m_pUserConstant.AddElement(pUserConstant);
		ID += pUserConstant->GetRegisterNum();
	}
}
void VSDX11Renderer::CreateUserConstantLightFunction(VSPShader* pPShader,VSArray<VSLight*> & LightArray,unsigned int &ID,VSString & OutString)
{
	unsigned int uiLightFunction = 0;
	bool bAddShareSampler = false;
	for (unsigned int i = 0 ; i < LightArray.GetNum() ; i++)
	{
		VSLocalLight * pLocalLight = DynamicCast<VSLocalLight>(LightArray[i]);
		if (pLocalLight && pLocalLight->HaveLightFun())
		{
			if (!bAddShareSampler)
			{
				VSShaderStringFactory::AddShaderSamplerRegisterID(pPShader, ID, VSShaderStringFactory::SS_LightFun);
				bAddShareSampler = true;
			}
			OutString += Sampler(VSTexture::TT_2D) + _T(" ");
			OutString += *VSShaderStringFactory::ms_PSConstantLightFunResource[uiLightFunction] + _T("Texture")
				+ SetRegister(RT_T, ID) + _T(";\n");

			VSUserSampler * pSampler = VS_NEW VSUserSampler(*VSShaderStringFactory::ms_PSConstantLightFunResource[uiLightFunction],VSTexture::TT_2D,ID,1);
			pPShader->m_pUserSampler.AddElement(pSampler);
			uiLightFunction++;
			ID++;
		}
	}
}
void VSDX11Renderer::CreateUserConstantShadow(VSPShader* pPShader,VSArray<VSLight*> & LightArray,unsigned int &ID,VSString & OutString)
{

	unsigned int uiShadowNum = 0;
	bool bAddShareSampler = false;
	for (unsigned int i = 0 ; i < LightArray.GetNum() ; i++)
	{
		VSLocalLight * pLocalLight = DynamicCast<VSLocalLight>(LightArray[i]);
		if (pLocalLight && pLocalLight->GetCastShadow())
		{
			if (!bAddShareSampler)
			{
				VSShaderStringFactory::AddShaderSamplerRegisterID(pPShader, ID, VSShaderStringFactory::SS_Shadow);
				bAddShareSampler = true;
			}
			if (pLocalLight->GetLightType() == VSLight::LT_POINT)
			{
				if(((VSPointLight*)pLocalLight)->GetShadowType() == VSPointLight::ST_CUB)
				{
					OutString += Sampler(VSTexture::TT_CUBE) + _T(" ");
					OutString += *VSShaderStringFactory::ms_PSConstantShadowResource[uiShadowNum] + _T("Texture")
						+ SetRegister(RT_T, ID) + _T(";\n");

					VSUserSampler * pSampler = VS_NEW VSUserSampler(*VSShaderStringFactory::ms_PSConstantShadowResource[uiShadowNum],VSTexture::TT_CUBE,ID,1);
					pPShader->m_pUserSampler.AddElement(pSampler);
					uiShadowNum++;
					ID++;
					
				}
				if(((VSPointLight*)pLocalLight)->GetShadowType() == VSPointLight::ST_VOLUME)
				{

					OutString += Sampler(VSTexture::TT_2D) + _T(" ");
					OutString += *VSShaderStringFactory::ms_PSConstantShadowResource[uiShadowNum] + _T("Texture")
						+ SetRegister(RT_T, ID) + _T(";\n");

					VSUserSampler * pSampler = VS_NEW VSUserSampler(*VSShaderStringFactory::ms_PSConstantShadowResource[uiShadowNum],VSTexture::TT_2D,ID,1);
					pPShader->m_pUserSampler.AddElement(pSampler);
					uiShadowNum++;
					ID++;

				}
				if(((VSPointLight*)pLocalLight)->GetShadowType() == VSPointLight::ST_DUAL_PARABOLOID)
				{

					OutString += Sampler(VSTexture::TT_2D) + _T(" ");
					OutString += *VSShaderStringFactory::ms_PSConstantShadowResource[uiShadowNum]  + _T("Texture")+ _T("[2]")
						+ SetRegister(RT_T, ID) + _T(";\n");

					VSUserSampler * pSampler = VS_NEW VSUserSampler(*VSShaderStringFactory::ms_PSConstantShadowResource[uiShadowNum],VSTexture::TT_2D,ID,2);
					pPShader->m_pUserSampler.AddElement(pSampler);
					uiShadowNum++;
					ID++;
					ID++;

				}
			}
			else if (pLocalLight->GetLightType() == VSLight::LT_DIRECTION)
			{
				if(((VSDirectionLight*)pLocalLight)->GetShadowType() == VSDirectionLight::ST_CSM)
				{

					OutString += Sampler(VSTexture::TT_2D) + _T(" ");
					OutString += *VSShaderStringFactory::ms_PSConstantShadowResource[uiShadowNum] + _T("Texture") + _T("[3]")
						+ SetRegister(RT_T, ID) + _T(";\n");

					VSUserSampler * pSampler = VS_NEW VSUserSampler(*VSShaderStringFactory::ms_PSConstantShadowResource[uiShadowNum],VSTexture::TT_2D,ID,3);
					pPShader->m_pUserSampler.AddElement(pSampler);
					uiShadowNum++;
					ID++;
					ID++;
					ID++;
				}
				else if (((VSDirectionLight*)pLocalLight)->GetShadowType() == VSDirectionLight::ST_PROJECT)
				{

				}
				else
				{

					OutString += Sampler(VSTexture::TT_2D) + _T(" ");
					OutString += *VSShaderStringFactory::ms_PSConstantShadowResource[uiShadowNum] + _T("Texture")
						+ SetRegister(RT_T, ID) + _T(";\n");

					VSUserSampler * pSampler = VS_NEW VSUserSampler(*VSShaderStringFactory::ms_PSConstantShadowResource[uiShadowNum],VSTexture::TT_2D,ID,1);
					pPShader->m_pUserSampler.AddElement(pSampler);
					uiShadowNum++;
					ID++;
				}
				
			}
			else if (pLocalLight->GetLightType() == VSLight::LT_SPOT)
			{
				if (((VSSpotLight*)pLocalLight)->GetShadowType() == VSSpotLight::ST_NORMAL)
				{

					OutString += Sampler(VSTexture::TT_2D) + _T(" ");
					OutString += *VSShaderStringFactory::ms_PSConstantShadowResource[uiShadowNum] + _T("Texture")
						+ SetRegister(RT_T, ID) + _T(";\n");

					VSUserSampler * pSampler = VS_NEW VSUserSampler(*VSShaderStringFactory::ms_PSConstantShadowResource[uiShadowNum],VSTexture::TT_2D,ID,1);
					pPShader->m_pUserSampler.AddElement(pSampler);
					uiShadowNum++;
					ID++;
				}
			}
			
		}
	}

}
void VSDX11Renderer::CreateGBufferSampler(VSPShader* pPShader, MaterialShaderPara &MSPara, unsigned int &uiTexRegisterID, VSString & OutString)
{
	VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
	if (pMaterial->GetPostEffectShaderFunction())
	{
		OutString += _T("SamplerState ");
		OutString += *VSShaderStringFactory::ms_PSColorBufferResource + _T("Sampler")
			+ SetRegister(RT_S, uiTexRegisterID) + _T(";\n");

		OutString += Sampler(VSTexture::TT_2D) + _T(" ");
		OutString += *VSShaderStringFactory::ms_PSColorBufferResource + _T("Texture")
			+ SetRegister(RT_T, uiTexRegisterID) + _T(";\n");

		VSUserSampler * pSampler = VS_NEW VSUserSampler(*VSShaderStringFactory::ms_PSColorBufferResource, VSTexture::TT_2D, uiTexRegisterID, 1);
		pPShader->m_pUserSampler.AddElement(pSampler);
		uiTexRegisterID++;

		OutString += _T("SamplerState ");
		OutString += *VSShaderStringFactory::ms_PSDepthNormalBufferResource + _T("Sampler")
			+ SetRegister(RT_S, uiTexRegisterID) + _T(";\n");

		OutString += Sampler(VSTexture::TT_2D) + _T(" ");
		OutString += *VSShaderStringFactory::ms_PSDepthNormalBufferResource + _T("Texture")
			+ SetRegister(RT_T, uiTexRegisterID) + _T(";\n");

		pSampler = VS_NEW VSUserSampler(*VSShaderStringFactory::ms_PSDepthNormalBufferResource, VSTexture::TT_2D, uiTexRegisterID, 1);
		pPShader->m_pUserSampler.AddElement(pSampler);
		uiTexRegisterID++;
	}
}
void VSDX11Renderer::CreateLightModleLookUpTable(VSPShader* pPShader,MaterialShaderPara &MSPara, unsigned int &uiTexRegisterID,VSString & OutString)
{
	VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
	if (pMaterial->GetShaderMainFunction(MSPara.uiPassId)->GetSMType() == VSShaderMainFunction::SM_OREN_NAYAR)
	{
		VSOrenNayarShaderFunction * pOrenNayarShaderFunction = (VSOrenNayarShaderFunction *)pMaterial->GetShaderMainFunction(MSPara.uiPassId);
		if (pOrenNayarShaderFunction->UseLookUpTable())
		{
			VSShaderStringFactory::AddShaderSamplerRegisterID(pPShader, uiTexRegisterID, VSShaderStringFactory::SS_OrenNayarLookUpTable);

			OutString += Sampler(VSTexture::TT_2D) + _T(" ");
			OutString += *VSShaderStringFactory::ms_PSOrenNayarLookUpTableResource + _T("Texture")
				+ SetRegister(RT_T, uiTexRegisterID) + _T(";\n");

			VSUserSampler * pSampler = VS_NEW VSUserSampler(*VSShaderStringFactory::ms_PSOrenNayarLookUpTableResource,VSTexture::TT_2D,uiTexRegisterID,1);
			pPShader->m_pUserSampler.AddElement(pSampler);
			uiTexRegisterID++;
		}
	}
}
void VSDX11Renderer::CreateUserConstantFarZ(VSShader * pShader,unsigned int& uiRegisterID,
							VSString & OutString)
{

	OutString += _T("float ") + *VSShaderStringFactory::ms_FarZ +_T(";\n");

	VSUserConstant * pUserConstant = VS_NEW VSUserConstant(*VSShaderStringFactory::ms_FarZ,sizeof(VSREAL) * 1,0,1);
	pShader->m_pUserConstant.AddElement(pUserConstant);

}
void VSDX11Renderer::CreateUserConstantSkyLightUpColor(VSShader * pShader,unsigned int& uiRegisterID,
									   VSString & OutString)
{
	VSString RegisterID = IntToString(uiRegisterID);
	OutString += _T("float4 ") + *VSShaderStringFactory::ms_SkyLightUpColor + _T(";\n");

	VSUserConstant * pUserConstant = VS_NEW VSUserConstant(*VSShaderStringFactory::ms_SkyLightUpColor,sizeof(VSREAL) * 4,0,1);
	pShader->m_pUserConstant.AddElement(pUserConstant);

}
void VSDX11Renderer::CreateUserConstantSkyLightDownColor(VSShader * pShader,unsigned int& uiRegisterID,
										 VSString & OutString)
{

	OutString += _T("float4 ") + *VSShaderStringFactory::ms_SkyLightDownColor + _T(";\n");

	VSUserConstant * pUserConstant = VS_NEW VSUserConstant(*VSShaderStringFactory::ms_SkyLightDownColor,sizeof(VSREAL) * 4,0,1);
	pShader->m_pUserConstant.AddElement(pUserConstant);

}
void VSDX11Renderer::CreateUserConstantPointLightRange(VSShader * pShader,unsigned int& uiRegisterID,
									   VSString & OutString)
{

	OutString += _T("float ") + *VSShaderStringFactory::ms_PointLightRange + _T(";\n");

	VSUserConstant * pUserConstant = VS_NEW VSUserConstant(*VSShaderStringFactory::ms_PointLightRange,sizeof(VSREAL) * 1,0,1);
	pShader->m_pUserConstant.AddElement(pUserConstant);

}
void VSDX11Renderer::CreateUserConstantInvRTWidth(VSShader * pShader,unsigned int& uiRegisterID,
							   VSString & OutString)
{

	OutString += _T("float ") + *VSShaderStringFactory::ms_InvRTWidth + _T(";\n");

	VSUserConstant * pUserConstant = VS_NEW VSUserConstant(*VSShaderStringFactory::ms_InvRTWidth,sizeof(VSREAL) * 1,0,1);
	pShader->m_pUserConstant.AddElement(pUserConstant);

}
void VSDX11Renderer::CreateUserConstantLight(VSShader * pShader,MaterialShaderPara &MSPara,unsigned int& uiRegisterID,
							 VSString & OutString)
{
	VSArray<VSLight*> & LightArray = MSPara.LightArray;

	int iLightNum[VSLight::LT_MAX] = { 0 };	
	for (unsigned int i = 0 ; i < LightArray.GetNum() ; i++)
	{
		if (LightArray[i])
		{
			for (unsigned int j = 0 ; j < VSLight::LT_MAX ; j++)
			{
				if (LightArray[i]->GetLightType() == j)
				{

					iLightNum[j]++;
				}
			}
		}
	}
	
	CreateUserConstantPointLight(pShader,uiRegisterID,iLightNum[VSLight::LT_POINT],OutString);
	CreateUserConstantSpotLight(pShader,uiRegisterID,iLightNum[VSLight::LT_SPOT],OutString);
	CreateUserConstantDirectionLight(pShader,uiRegisterID,iLightNum[VSLight::LT_DIRECTION],OutString);
}
void VSDX11Renderer::CreatePUserConstant(VSPShader* pPShader,MaterialShaderPara &MSPara,
										VSString & OutString)
{
	unsigned int uiPassType = MSPara.uiPassType;
	if (uiPassType == VSPass::PT_MATERIAL)
	{
		unsigned int uiRegisterID = 0;
		unsigned int uiLightNum = MSPara.LightArray.GetNum();
		VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		OutString += _T("cbuffer CB : register( b0 ) \n{\n");
		CreateUserConstantWorldMatrix(pPShader,uiRegisterID,OutString);
		CreateUserConstantViewMatrix(pPShader,uiRegisterID,OutString);
		CreateUserConstantProjectMatrix(pPShader, uiRegisterID, OutString);
		CreateUserConstantCameraWorldPos(pPShader,uiRegisterID,OutString);
		CreateUserConstantFarZ(pPShader,uiRegisterID,OutString);
		if (uiLightNum)
		{			
			CreateUserConstantLight(pPShader,MSPara,uiRegisterID,OutString);	
		}
		pMaterial->CreateConstValueDeclare(OutString,uiRegisterID,MSPara);
		pMaterial->CreateCustomValue(pPShader);
		OutString += _T("} \n");
		unsigned uiTexRegisterID = 0;
		pMaterial->CreateTextureDeclare(OutString, uiTexRegisterID, MSPara);
		pMaterial->CreateCustomTexture(pPShader);
		CreateUserConstantShadow(pPShader, MSPara.LightArray, uiTexRegisterID, OutString);
		CreateUserConstantLightFunction(pPShader, MSPara.LightArray, uiTexRegisterID, OutString);
		CreateLightModleLookUpTable(pPShader, MSPara, uiTexRegisterID, OutString);
	}
	else if (uiPassType == VSPass::PT_INDIRECT)
	{
		unsigned int uiRegisterID = 0;
		VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		OutString += _T("cbuffer CB : register( b0 ) \n{\n");
		CreateUserConstantWorldMatrix(pPShader,uiRegisterID,OutString);
		CreateUserConstantViewMatrix(pPShader,uiRegisterID,OutString);
		CreateUserConstantProjectMatrix(pPShader, uiRegisterID, OutString);
		CreateUserConstantCameraWorldPos(pPShader,uiRegisterID,OutString);
		CreateUserConstantSkyLightUpColor(pPShader,uiRegisterID,OutString);
		CreateUserConstantSkyLightDownColor(pPShader,uiRegisterID,OutString);

		pMaterial->CreateConstValueDeclare(OutString, uiRegisterID, MSPara);
		pMaterial->CreateCustomValue(pPShader);
		OutString += _T("} \n");
		unsigned uiTexRegisterID = 0;
		pMaterial->CreateTextureDeclare(OutString, uiTexRegisterID, MSPara);
		pMaterial->CreateCustomTexture(pPShader);
	}
	else if (uiPassType == VSPass::PT_NORMALDEPTH)
	{
		VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		unsigned int uiRegisterID = 0;
		OutString += _T("cbuffer CB : register( b0 ) \n{\n");
		CreateUserConstantWorldMatrix(pPShader, uiRegisterID, OutString);
		CreateUserConstantViewMatrix(pPShader, uiRegisterID, OutString);
		CreateUserConstantProjectMatrix(pPShader, uiRegisterID, OutString);
		CreateUserConstantCameraWorldPos(pPShader, uiRegisterID, OutString);
		CreateUserConstantFarZ(pPShader,uiRegisterID,OutString);
			
		pMaterial->CreateConstValueDeclare(OutString, uiRegisterID, MSPara);
		pMaterial->CreateCustomValue(pPShader);
		OutString += _T("} \n");
		unsigned uiTexRegisterID = 0;
		pMaterial->CreateTextureDeclare(OutString, uiTexRegisterID, MSPara);
		pMaterial->CreateCustomTexture(pPShader);
		
	}
	else if (uiPassType == VSPass::PT_POINT_CUBE_SHADOW)
	{

		VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		unsigned int uiRegisterID = 0;
		OutString += _T("cbuffer CB : register( b0 ) \n{\n");
		CreateUserConstantWorldMatrix(pPShader,uiRegisterID,OutString);
		CreateUserConstantCameraWorldPos(pPShader,uiRegisterID,OutString);
		CreateUserConstantPointLightRange(pPShader,uiRegisterID,OutString);
		

		pMaterial->CreateConstValueDeclare(OutString, uiRegisterID, MSPara);
		pMaterial->CreateCustomValue(pPShader);
		OutString += _T("} \n");
		unsigned uiTexRegisterID = 0;
		pMaterial->CreateTextureDeclare(OutString, uiTexRegisterID, MSPara);
		pMaterial->CreateCustomTexture(pPShader);
		
	}
	else if (uiPassType == VSPass::PT_DUAL_PARABOLOID_SHADOW)
	{
		VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		unsigned int uiRegisterID = 0;
		OutString += _T("cbuffer CB : register( b0 ) \n{\n");
		CreateUserConstantWorldMatrix(pPShader, uiRegisterID, OutString);
		CreateUserConstantCameraWorldPos(pPShader, uiRegisterID, OutString);


		pMaterial->CreateConstValueDeclare(OutString, uiRegisterID, MSPara);
		pMaterial->CreateCustomValue(pPShader);
		OutString += _T("} \n");
		unsigned uiTexRegisterID = 0;
		pMaterial->CreateTextureDeclare(OutString, uiTexRegisterID, MSPara);
		pMaterial->CreateCustomTexture(pPShader);
	}
	else if (uiPassType == VSPass::PT_LIGHT_FUNCTION
		|| uiPassType == VSPass::PT_POINT_VOLUME_SHADOW)
	{
		unsigned int uiRegisterID = 0;

		VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		OutString += _T("cbuffer CB : register( b0 ) \n{\n");
		pMaterial->CreateConstValueDeclare(OutString, uiRegisterID, MSPara);
		pMaterial->CreateCustomValue(pPShader);
		OutString += _T("} \n");
		unsigned uiTexRegisterID = 0;

		pMaterial->CreateTextureDeclare(OutString, uiTexRegisterID, MSPara);
		pMaterial->CreateCustomTexture(pPShader);
	}
	else if (uiPassType == VSPass::PT_SHADOW)
	{
		unsigned int uiRegisterID = 0;

		VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		OutString += _T("cbuffer CB : register( b0 ) \n{\n");
		CreateUserConstantLightShadowMatrix(pPShader, uiRegisterID, OutString);
		CreateUserConstantWorldMatrix(pPShader, uiRegisterID, OutString);
		CreateUserConstantCameraWorldPos(pPShader, uiRegisterID, OutString);

		pMaterial->CreateConstValueDeclare(OutString, uiRegisterID, MSPara);
		pMaterial->CreateCustomValue(pPShader);
		OutString += _T("} \n");
		unsigned uiTexRegisterID = 0;

		pMaterial->CreateTextureDeclare(OutString, uiTexRegisterID, MSPara);
		pMaterial->CreateCustomTexture(pPShader);
	}
	else if (uiPassType == VSPass::PT_POSTEFFECT)
	{
		unsigned int uiRegisterID = 0;

		VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		OutString += _T("cbuffer CB : register( b0 ) \n{\n");
		pMaterial->CreateConstValueDeclare(OutString, uiRegisterID, MSPara);
		pMaterial->CreateCustomValue(pPShader);
		OutString += _T("} \n");
		unsigned uiTexRegisterID = 0;
		CreateGBufferSampler(pPShader, MSPara, uiTexRegisterID, OutString);
		pMaterial->CreateTextureDeclare(OutString, uiTexRegisterID, MSPara);
		pMaterial->CreateCustomTexture(pPShader);
	}
	
}
void VSDX11Renderer::CreatePFunction(MaterialShaderPara &MSPara,
								   VSString & OutString)
{
	unsigned int uiPassType = MSPara.uiPassType;
	if (uiPassType == VSPass::PT_PREZ || uiPassType == VSPass::PT_POINT_VOLUME_SHADOW || uiPassType == VSPass::PT_DIRECT_VOLUME_SHADOW)
	{
		OutString = _T("PS_OUTPUT ") + ms_ShaderProgramMain[VSEngineFlag::ST_PIXEL] + _T("(PS_INPUT ps_Input)\n{\n PS_OUTPUT Out = (PS_OUTPUT) 0;\nOut.Color0 = float4(0.0f,0.0f,0.0f,1.0f);\nreturn Out;\n};");
	}
	else
	{
		VSString FunctionBody;
		VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		pMaterial->GetPShaderTreeString(FunctionBody, MSPara);
		if (uiPassType == VSPass::PT_MATERIAL)
		{
			VSString VSCustomDeclareString;
			pMaterial->GetCustomDeclareString(VSCustomDeclareString, MSPara);
			OutString += VSCustomDeclareString;
		}
		OutString += _T("PS_OUTPUT ") + ms_ShaderProgramMain[VSEngineFlag::ST_PIXEL] + _T("(PS_INPUT ps_Input)\n{\nPS_OUTPUT Out = (PS_OUTPUT) 0;\n") + FunctionBody + _T("return Out;\n};\n");
	}
}
void VSDX11Renderer::SkyLight(const VSString &WorldNormal,const VSString &UpColor,const VSString & DownColor,const VSString & DiffuseColor,VSString & OutString)
{
	OutString += _T(" + SkyLight(") + WorldNormal + _T(",") + UpColor + _T(",") + DownColor + _T(") * ") + DiffuseColor;
}
void VSDX11Renderer::CustomDirectionalLight(int iLightNum,const VSString &CustomString,VSArray<VSString> ShadowString,VSString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		VSString ID = IntToString(i);
		//OutString += _T("+ float4(1.0f,1.0f,1.0f,1.0f)") + ShadowString[i];
		OutString += _T(" + CustomDirectionLightFun(") + CustomString
			+ *VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightDiffuse,") 
			+ *VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
			+ *VSShaderStringFactory::ms_LightName[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i];
	}
}
void VSDX11Renderer::DirectionalLight(int iLightNum,const VSString &Diffuse,const VSString &Specular ,const VSString &SpecularPow,
							  const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,VSString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		VSString ID = IntToString(i);
		OutString += _T(" + DirectionalLightFun(") + Diffuse + _T(",") + Specular + _T(",") + SpecularPow + _T(",") 
					+ WorldNormal + _T(",") + WorldCameraDir + _T(",") 
					+ *VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightDiffuse,") 
					+ *VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
					+ *VSShaderStringFactory::ms_LightName[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i];
	}
}
void VSDX11Renderer::OrenNayarDirectionalLight(int iLightNum,const VSString &Diffuse,const VSString &RoughnessSquared,
									 const VSString &WorldNormal,const VSString &WorldCameraDir,bool bLookUpTable,VSArray<VSString> ShadowString,VSString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		VSString ID = IntToString(i);
		if (bLookUpTable == false)
		{
			OutString += _T(" + OrenNayarDirectionalLightFun(") + Diffuse + _T(",") + RoughnessSquared + _T(",") 
				+ WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ *VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightDiffuse,")  
				+ *VSShaderStringFactory::ms_LightName[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i];
		}
		else
		{
			OutString += _T(" + OrenNayarDirectionalLightFun(") + Diffuse + _T(",") + RoughnessSquared + _T(",") 
				+ WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ *VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightDiffuse,") 
				+ *VSShaderStringFactory::ms_LightName[VSLight::LT_DIRECTION] + _T("[") + ID + _T("].LightWorldDirection.xyz,")
				+ *VSShaderStringFactory::ms_ShareSampler[VSShaderStringFactory::SS_OrenNayarLookUpTable] + _T(",") + *VSShaderStringFactory::ms_PSOrenNayarLookUpTableResource + _T("Texture)") + ShadowString[i];
		}
		
	}
}

void VSDX11Renderer::PointLight(int iLightNum,const VSString &Diffuse,const VSString &Specular ,const VSString &SpecularPow,
							const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
							VSString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		VSString ID = IntToString(i);
		OutString += _T(" + PointLightFun(") + Diffuse + _T(",") + Specular + _T(",") + SpecularPow + _T(",") 
			+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
			+ *VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_POINT] + _T("[") + ID + _T("].LightDiffuse,") 
			+ *VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_POINT] + _T("[") + ID + _T("].LightSpecular,") 
			+ *VSShaderStringFactory::ms_LightName[VSLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.w,") 				 
			+ *VSShaderStringFactory::ms_LightName[VSLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.xyz)")  + ShadowString[i];
	}
	
}
void VSDX11Renderer::CustomPointLight(int iLightNum, const VSString &CustomString,const VSString & WorldPos,VSArray<VSString> ShadowString,VSString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		VSString ID = IntToString(i);
		OutString += _T(" + CustomPointLightFun(") + CustomString
			+ WorldPos + _T(",")
			+ *VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_POINT] + _T("[") + ID + _T("].LightDiffuse,") 
			+ *VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_POINT] + _T("[") + ID + _T("].LightSpecular,") 
			+ *VSShaderStringFactory::ms_LightName[VSLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.w,") 				 
			+ *VSShaderStringFactory::ms_LightName[VSLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.xyz)")  + ShadowString[i];
	}
}
void VSDX11Renderer::OrenNayarPointLight(int iLightNum,const VSString &Diffuse,const VSString &RoughnessSquared,
										 const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,bool bLookUpTable,VSArray<VSString> ShadowString,
										 VSString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		VSString ID = IntToString(i);
		if (bLookUpTable == false)
		{
			OutString += _T(" + OrenNayarPointLightFun(") + Diffuse + _T(",") + RoughnessSquared + _T(",") 
				+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ *VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_POINT] + _T("[") + ID + _T("].LightDiffuse,") 
				+ *VSShaderStringFactory::ms_LightName[VSLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.w,") 	 
				+ *VSShaderStringFactory::ms_LightName[VSLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.xyz)") + ShadowString[i];
		}
		else
		{
			OutString += _T(" + OrenNayarPointLightFun(") + Diffuse + _T(",") + RoughnessSquared + _T(",") 
				+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ *VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_POINT] + _T("[") + ID + _T("].LightDiffuse,") 
				+ *VSShaderStringFactory::ms_LightName[VSLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.w,") 			 
				+ *VSShaderStringFactory::ms_LightName[VSLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.xyz,")
				+ *VSShaderStringFactory::ms_ShareSampler[VSShaderStringFactory::SS_OrenNayarLookUpTable] +_T(",") + *VSShaderStringFactory::ms_PSOrenNayarLookUpTableResource + _T("Texture)") + ShadowString[i];
		}
	}
}

void VSDX11Renderer::SpotLight(int iLightNum,const VSString &Diffuse,const VSString &Specular ,const VSString &SpecularPow,
					   const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
					   VSString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		VSString ID = IntToString(i);
		OutString += _T(" + SpotLightFun(") + Diffuse + _T(",") + Specular + _T(",") + SpecularPow + _T(",") 
			+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
			+ *VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightDiffuse,") 
			+ *VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightSpecular,") 
			+ *VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.w,") 		 
			+ *VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.x,") 
			+ *VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.y,") 
			+ *VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.z,") 
			+ *VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.xyz,") 
			+ *VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i] ;

	}
}
void VSDX11Renderer::CustomSpotLight(int iLightNum,const VSString &CustomString,const VSString & WorldPos,VSArray<VSString> ShadowString,VSString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		VSString ID = IntToString(i);
		OutString += _T(" + CustomSpotLightFun(") + CustomString
			+ WorldPos + _T(",")
			+ *VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightDiffuse,") 
			+ *VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightSpecular,") 
			+ *VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.w,") 		 
			+ *VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.x,") 
			+ *VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.y,") 
			+ *VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.z,") 
			+ *VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.xyz,") 
			+ *VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i] ;

	}
}
void VSDX11Renderer::OrenNayarSpotLight(int iLightNum,const VSString &Diffuse,const VSString &RoughnessSquared,
								const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,bool bLookUpTable,VSArray<VSString> ShadowString,
								VSString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		if (bLookUpTable == false)
		{
			VSString ID = IntToString(i);
			OutString += _T(" + OrenNayarSpotLightFun(") + Diffuse + _T(",") + RoughnessSquared + _T(",") 
				+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ *VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightDiffuse,") 
				+ *VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.w,") 		 
				+ *VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.x,") 
				+ *VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.y,") 
				+ *VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.z,") 
				+ *VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.xyz,") 
				+ *VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i] ;
		}
		else
		{
			VSString ID = IntToString(i);
			OutString += _T(" + OrenNayarSpotLightFun(") + Diffuse + _T(",") + RoughnessSquared + _T(",") 
				+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ *VSShaderStringFactory::ms_LightNameTemp[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightDiffuse,") 
				+ *VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.w,") 		 
				+ *VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.x,") 
				+ *VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.y,") 
				+ *VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.z,") 
				+ *VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.xyz,") 
				+ *VSShaderStringFactory::ms_LightName[VSLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldDirection.xyz,")
				+ *VSShaderStringFactory::ms_ShareSampler[VSShaderStringFactory::SS_OrenNayarLookUpTable] +_T(",") + *VSShaderStringFactory::ms_PSOrenNayarLookUpTableResource + _T("Texture)") + ShadowString[i];
		}

	}
}
void VSDX11Renderer::LocalToWorldPos(const VSString & LocalPos,VSString &OutString)const
{
	OutString += _T("TransPos(") + LocalPos + _T(",") + 
					*VSShaderStringFactory::ms_WorldMatrix + _T(")");
}
void VSDX11Renderer::WorldToViewPos(const VSString & WorldPos,VSString &OutString)const
{
	OutString += _T("TransPos(") + WorldPos + _T(",") + 
		*VSShaderStringFactory::ms_ViewMatrix + _T(")");
}
void VSDX11Renderer::TransProjPos(const VSString & Pos,const VSString & Matrix,VSString &OutString)const
{
	OutString += _T("TransProjPos(") + Pos + _T(",") + Matrix + _T(")");
}
void VSDX11Renderer::LocalToWorldNormal(const VSString & LocalNormal,VSString &OutString)const
{
	OutString += _T("TransNormal(") + LocalNormal + _T(",") + 
		*VSShaderStringFactory::ms_WorldMatrix + _T(")");
}
void VSDX11Renderer::GetWorldViewDir(VSString &OutString)const
{
	OutString += _T("GetZDir(") + *VSShaderStringFactory::ms_ViewMatrix + _T(")");
	
}
void VSDX11Renderer::WorldToViewNormal(const VSString & WorldNormal,VSString &OutString)const
{
	OutString += _T("TransNormal(") + WorldNormal + _T(",") + 
		*VSShaderStringFactory::ms_ViewMatrix + _T(")");
}
void VSDX11Renderer::BumpNormal(const VSString &TexNormal,VSString &OutString)const
{
	OutString += _T("BumpNormal(") + VSShaderStringFactory::ms_InputLocalTangent + _T(",") 
				+ VSShaderStringFactory::ms_InputLocalBinormal + _T(",")
				+ VSShaderStringFactory::ms_InputLocalNormal + _T(",")
				+ TexNormal + _T(")");
}
void VSDX11Renderer::ComputeDir(const VSString & Orgin,const VSString & End,VSString &OutString)const
{
	OutString += _T("ComputeDir(") + Orgin + _T(",")
				+ End + _T(")");
}
void VSDX11Renderer::ComputeLength(const VSString & Orgin,const VSString & End,VSString &OutString)const
{
	OutString += _T("ComputeLength(") + Orgin + _T(",")
		+ End + _T(")");
}
void VSDX11Renderer::GetLightType(unsigned int i,VSString &OutString)const
{
	if (i == VSLight::LT_DIRECTION)
	{ 
		OutString = _T("DirLightType");
	}
	else if (i == VSLight::LT_POINT)
	{
		OutString = _T("PointLightType");
	}
	else if (i == VSLight::LT_SPOT)
	{
		OutString = _T("SpotLightType");
	}
}
void VSDX11Renderer::GetLightTypeTemp(VSString &OutString)const
{

	OutString = _T("LightTypeTemp");

}
void VSDX11Renderer::ComputeBoneVector(const VSString& BoneIndex, const VSString & BoneWeight,const VSString &BoneVector,
							  const  VSString & U,const VSString & V,const VSString &N,VSString &OutString)const
{
	OutString += _T("ComputeBoneVector(") + BoneIndex + _T(",")
		+ BoneWeight + _T(",") + BoneVector + _T(",") +  U + _T(",") + V + _T(",") + N  + _T(")");
}
void VSDX11Renderer::ComputeBoneVector(const VSString& BoneIndex, const VSString & BoneWeight,
	const VSString &AnimDataResource, const VSString & AnimAllInfo,
	const VSString &AnimInstanceInfo, const VSString &AnimNum,
	const  VSString & U, const VSString & V, const VSString &N, VSString &OutString)const
{
	OutString += _T("ComputeBoneVector(") + BoneIndex + _T(",")
		+ BoneWeight + _T(",") + AnimDataResource + _T(",") + AnimAllInfo + _T(",")
		+ AnimInstanceInfo + _T(",") + AnimNum + _T(",")
		+ U + _T(",") + V + _T(",") + N + _T(")");
}
void VSDX11Renderer::BoneTranPos( const VSString & LocalPos,const  VSString & U,const VSString & V,const VSString &N,VSString &OutString)const
{
	OutString += _T("TransPos(") + LocalPos + _T(",") +  U + _T(",") + V + _T(",") + N  + _T(")");
}
 void VSDX11Renderer::BoneTranNormal( const VSString & LocalNormal,const  VSString & U,const VSString & V,const VSString &N,VSString &OutString)const
{
	OutString += _T("TransNormal(") + LocalNormal + _T(",") +  U + _T(",") + V + _T(",") + N  + _T(")");
}
void VSDX11Renderer::DecodeNormal1(const VSString & Normal,VSString &OutString) const
{
	OutString += _T("DecodeNormal1(") + Normal + _T(")");
}
void VSDX11Renderer::RecodeNormal1(const VSString & Normal,VSString &OutString) const
{
	OutString += _T("RecodeNormal1(") + Normal + _T(")");
}
void VSDX11Renderer::DecodeNormal2(const VSString & Normal,const VSString & Tengent,VSString &OutString) const
{
	OutString += _T("DecodeNormal2(") + Normal + _T(",") + Tengent + _T(")");
}
void VSDX11Renderer::DecodeNormal3(const VSString & Normal,VSString &OutString) const
{
	OutString += _T("DecodeNormal3(") + Normal + _T(")");
}
void VSDX11Renderer::RecodeNormal3(const VSString & Normal,VSString &OutString) const
{
	OutString += _T("RecodeNormal3(") + Normal + _T(")");
}
void VSDX11Renderer::EncodeReflect(const VSString & Mip , const VSString & Pow , VSString& OutString)
{
	OutString += _T("EncodeReflect(") + Mip + _T(",") + Pow + _T(")");
}
void VSDX11Renderer::DecodeReflect(const VSString & Mip , const VSString & Pow , const VSString & Value,VSString& OutString)
{
	OutString += _T("DecodeReflect(") + Mip + _T(",") + Pow + _T(",") + Value + _T(")");
}
void VSDX11Renderer::GreaterZeroPow(const VSString & Value,VSREAL Exp,VSString &OutString) const
{
	VSString SExp = RealToString(Exp);
	OutString += _T("GreaterZeroPow(") + Value + _T(",") + SExp + _T(")");
}
void VSDX11Renderer::Reciprocal(const VSString & Value, VSString &OutString)const
{
	OutString += _T("1.0 / (") + Value + _T(")");
}
void VSDX11Renderer::Clip(const VSString & ClipValue,VSString &OutString)const
{
	OutString += _T("clip(") + ClipValue + _T(")");
}
void VSDX11Renderer::SaturatePow(const VSString & Value,VSREAL Exp,VSString &OutString) const
{
	VSString SExp = RealToString(Exp);
	OutString += _T("SaturatePow(") + Value + _T(",") + SExp + _T(")");
}
void VSDX11Renderer::Saturate(const VSString & Value,VSString &OutString)const
{
	OutString += _T("saturate(") + Value + _T(")");
}
void VSDX11Renderer::PointLightCubeShadow(const VSString & PointLightName , const VSString & CubeShadowSampler , const VSString & ViewMatrix,
										 const VSString & WorldPos, VSString &OutString)const
{

	OutString += _T("PointLightCubShadow(") + PointLightName + _T(",") + WorldPos + _T(",") + ViewMatrix + _T(",") + 
		*VSShaderStringFactory::ms_ShareSampler[VSShaderStringFactory::SS_Shadow] + _T(",") + CubeShadowSampler + _T("Texture)");
	
}
void VSDX11Renderer::PointLightCubeShadow(const VSString & PointLightName , const VSString & CubeShadowSampler ,
										 const VSString & WorldPos, VSString &OutString)const
{

	OutString += _T("PointLightCubShadow(") + PointLightName + _T(",") + WorldPos + _T(",") + 
		*VSShaderStringFactory::ms_ShareSampler[VSShaderStringFactory::SS_Shadow] + _T(",") + CubeShadowSampler + _T("Texture)");

}
void VSDX11Renderer::PointLightVolumeShadow(const VSString & PointLightName , const VSString & VolumeShadowSampler , 
									const VSString & WorldPos, VSString& OutString)const
{
	OutString += _T("PointLightVolumeShadow(") + PointLightName + _T(",") + WorldPos + _T(",") + 
		*VSShaderStringFactory::ms_ShareSampler[VSShaderStringFactory::SS_Shadow] + _T(",") + VolumeShadowSampler + _T("Texture)");
}
void VSDX11Renderer::PointLightDualParaboloidShadow(const VSString & PointLightName , const VSString & ShadowSampler ,  
											const VSString & WorldPos, VSString& OutString)const
{
	OutString += _T("PointLightDualParaboloidShadow(") + PointLightName + _T(",") + WorldPos + _T(",") + 
		*VSShaderStringFactory::ms_ShareSampler[VSShaderStringFactory::SS_Shadow] + _T(",") + ShadowSampler + _T("Texture)");
}
void VSDX11Renderer::DirectionLightVolumeShadow(const VSString & DirLightName , const VSString & VolumeShadowSampler , 
										const VSString & WorldPos, VSString& OutString)const
{
	OutString += _T("DirectionLightVolumeShadow(") + DirLightName + _T(",") + WorldPos + _T(",") + 
		*VSShaderStringFactory::ms_ShareSampler[VSShaderStringFactory::SS_Shadow] + _T(",") + VolumeShadowSampler + _T("Texture)");
}
void VSDX11Renderer::DirectionLightCSMShadow(const VSString & DirLightName , const VSString & ShadowSampler , 
									 const VSString & WorldPos, const VSString & ViewPos,const VSString &FarZ,VSString& OutString)const
{
	OutString += _T("DirLightCSMShadow(") + DirLightName + _T(",") + WorldPos + _T(",") + ViewPos + _T(",") + FarZ + _T(",") + 
		*VSShaderStringFactory::ms_ShareSampler[VSShaderStringFactory::SS_Shadow] + _T(",") + ShadowSampler + _T("Texture)");
}
void VSDX11Renderer::DirectionLightShadow(const VSString & DirLightName , const VSString & ShadowSampler , 
								  const VSString & WorldPos, VSString& OutString)const
{
	OutString += _T("DirLightShadow(") + DirLightName + _T(",") + WorldPos + _T(",") + 
		*VSShaderStringFactory::ms_ShareSampler[VSShaderStringFactory::SS_Shadow] + _T(",") + ShadowSampler + _T("Texture)");
}
void VSDX11Renderer::SpotLightShadow(const VSString & SpotLightName , const VSString & ShadowSampler , 
							 const VSString & WorldPos, VSString& OutString)const
{
	OutString += _T("SpotLightShadow(") + SpotLightName + _T(",") + WorldPos + _T(",") + 
		*VSShaderStringFactory::ms_ShareSampler[VSShaderStringFactory::SS_Shadow] + _T(",") + ShadowSampler + _T("Texture)");
}
void VSDX11Renderer::GetLightFunction(VSArray<VSLight *> LightArray,const VSString & WorldPos,VSString & OutString)const
{
	unsigned int iLightNum[VSLight::LT_MAX] = { 0 };
	unsigned int uiLightFunSampler = 0;
	for (unsigned int i = 0 ; i < LightArray.GetNum() ; i++)
	{

		VSLocalLight * pLocalLight = DynamicCast<VSLocalLight>(LightArray[i]);
		if (pLocalLight)
		{
			for (unsigned int j = 0 ; j < VSLight::LT_MAX ; j++)
			{
				if (LightArray[i]->GetLightType() == j)
				{
					if (pLocalLight->HaveLightFun())
					{
						OutString += _T("float4 FunResult = float4(0.0f,0.0f,0.0f,1.0f);\n");
						
						if (j == VSLight::LT_POINT)
						{
							OutString += _T("FunResult = PointLightFunction(")
								+ *VSShaderStringFactory::ms_LightName[j] + _T("[") + IntToString(iLightNum[j]) + _T("].LightWorldPos.xyz,")
								+ WorldPos + _T(",")
								+ *VSShaderStringFactory::ms_LightName[j] + _T("[") + IntToString(iLightNum[j]) + _T("].LightFunParam,")
								+ *VSShaderStringFactory::ms_ShareSampler[VSShaderStringFactory::SS_LightFun] + _T(",") 
								+ *VSShaderStringFactory::ms_PSConstantLightFunResource[uiLightFunSampler] + _T("Texture") + _T(");\n");
						}
						else
						{
							OutString += _T("FunResult = LightFunction(")
								+ *VSShaderStringFactory::ms_LightName[j] + _T("[") + IntToString(iLightNum[j]) + _T("].WVP,")
								+ WorldPos + _T(",")
								+ *VSShaderStringFactory::ms_LightName[j] + _T("[") + IntToString(iLightNum[j]) + _T("].LightFunParam,")
								+ *VSShaderStringFactory::ms_ShareSampler[VSShaderStringFactory::SS_LightFun] + _T(",")
								+ *VSShaderStringFactory::ms_PSConstantLightFunResource[uiLightFunSampler] + _T("Texture") + _T(");\n");
						}
						
						OutString += *VSShaderStringFactory::ms_LightNameTemp[j] + _T("[") + IntToString(iLightNum[j]) + _T("].LightDiffuse *= FunResult;\n");
						OutString += *VSShaderStringFactory::ms_LightNameTemp[j] + _T("[") + IntToString(iLightNum[j]) + _T("].LightSpecular *= dot(FunResult,float4(0.299,0.587,0.184,0));\n");
						uiLightFunSampler++;
					}
					
					iLightNum[j]++;
				}
			}
			
		}
	}
}
void VSDX11Renderer::TranLightToTemp(VSArray<VSLight *> LightArray,VSString & OutString)const
{
	unsigned int iLightNum[VSLight::LT_MAX] = { 0 };	
	for (unsigned int i = 0 ; i < LightArray.GetNum() ; i++)
	{
		if (LightArray[i])
		{
			for (unsigned int j = 0 ; j < VSLight::LT_MAX ; j++)
			{
				if (LightArray[i]->GetLightType() == j)
				{

					iLightNum[j]++;

				}
			}
		}
	}
	VSString LightTypeTempString;
	GetLightTypeTemp(LightTypeTempString);
	for (unsigned int j = 0 ; j < VSLight::LT_MAX ; j++)
	{
		if (iLightNum[j] > 0)
		{
			OutString += LightTypeTempString + _T(" ") + *VSShaderStringFactory::ms_LightNameTemp[j] + _T("[") + IntToString(iLightNum[j]) + _T("];\n"); 
		}
		
	}
	for (unsigned int j = 0 ; j < VSLight::LT_MAX ; j++)
	{

		for (unsigned int k = 0 ; k < iLightNum[j] ; k++)
		{
			OutString += *VSShaderStringFactory::ms_LightNameTemp[j] + _T("[") + IntToString(k) + _T("].LightDiffuse = ") + 
				*VSShaderStringFactory::ms_LightName[j] + _T("[") + IntToString(k) + _T("].LightDiffuse;\n");  

			OutString += *VSShaderStringFactory::ms_LightNameTemp[j] + _T("[") + IntToString(k) + _T("].LightSpecular = ") + 
				*VSShaderStringFactory::ms_LightName[j] + _T("[") + IntToString(k) + _T("].LightSpecular;\n");  
		}

		
	}
}
void VSDX11Renderer::CreateGInputDeclare(MaterialShaderPara &MSPara,
	VSString & OutString){}
void VSDX11Renderer::CreateGOutputDeclare(MaterialShaderPara &MSPara,
	VSString & OutString){}
void VSDX11Renderer::CreateGUserConstant(VSGShader * pGShader, MaterialShaderPara &MSPara,
	VSString & OutString){}
void VSDX11Renderer::CreateGFunction(MaterialShaderPara &MSPara,
	VSString & OutString){}
void VSDX11Renderer::SetMaterialGShaderConstant(MaterialShaderPara &MSPara,VSGShader *pGShader){}
void VSDX11Renderer::CreateHInputDeclare(MaterialShaderPara &MSPara,
	VSString & OutString) 
{
	VSMAC_ASSERT(MSPara.bUseTessellation);
	VSString TempDeclare;
	unsigned int ID = 0;
	unsigned int uiPassType = MSPara.uiPassType;
	CreateDeclareWorldPos(MSPara,ID, TempDeclare, true);
	CreateDeclareTexCoord(MSPara, ID, TempDeclare, true);
	CreateDeclareNormal(MSPara, ID, TempDeclare, true);
	CreateDeclareColor(MSPara,TempDeclare, true);
	if (MSPara.pGeometry->IsSameType(VSInstanceGeometry::ms_Type))
	{
		CreateDeclareInstancePos(MSPara,ID, TempDeclare, true);
		CreateDeclareMaterialInstance(MSPara, TempDeclare, true);
	}
	CreateDeclareTessellationValue(MSPara,TempDeclare, true);
	OutString += _T("struct HS_INPUT \n{ \n") + TempDeclare + _T("};\n");
}
void VSDX11Renderer::CreateHOutputDeclare(MaterialShaderPara &MSPara,
	VSString & OutString)
{
	VSMAC_ASSERT(MSPara.bUseTessellation);
	VSString TempDeclare;
	unsigned int ID = 0;
	unsigned int uiPassType = MSPara.uiPassType;
	CreateDeclareWorldPos(MSPara, ID, TempDeclare, false);
	CreateDeclareTexCoord(MSPara, ID, TempDeclare, false);
	CreateDeclareNormal(MSPara, ID, TempDeclare, false);
	CreateDeclareColor(MSPara, TempDeclare, false);
	if (MSPara.pGeometry->IsSameType(VSInstanceGeometry::ms_Type))
	{
		CreateDeclareInstancePos(MSPara, ID, TempDeclare, false);
		CreateDeclareMaterialInstance(MSPara, TempDeclare, false);
	}
	OutString += _T("struct HS_OUTPUT \n{ \n") + TempDeclare + _T("};\n");
}
void VSDX11Renderer::CreateHUserConstant(VSHShader * pHShader, MaterialShaderPara &MSPara,
	VSString & OutString) {}
void VSDX11Renderer::CreateHFunctionWorldPos(MaterialShaderPara &MSPara, VSString & FunctionBody)
{
	FunctionBody += _T("Out.WorldPos = hs_Input[i].WorldPos;\n");
}
void VSDX11Renderer::CreateHFunctionTexCoord(MaterialShaderPara &MSPara, VSString & FunctionBody)
{
	VSVertexBuffer * pVBuffer = MSPara.pGeometry->GetMeshData()->GetVertexBuffer();
	for (unsigned int i = 0; i < pVBuffer->GetTexCoordLevel(); i++)
	{
		if (pVBuffer->HaveTexCoordInfo(i))
		{
			VSString TextureID = IntToString(i);
			FunctionBody += _T("Out.TexCoord") + TextureID + _T(" = hs_Input[i].TexCoord") + TextureID + _T(";\n");
		}
	}
}
void VSDX11Renderer::CreateHFunctionNormal(MaterialShaderPara &MSPara, VSString & FunctionBody)
{
	VSVertexBuffer * pVBuffer = MSPara.pGeometry->GetMeshData()->GetVertexBuffer();
	if (pVBuffer->HaveNormalInfo(0))
	{
		FunctionBody += _T("Out.Normal = hs_Input[i].Normal;\n");
	}

	if (pVBuffer->HaveTangentInfo())
	{
		{
			FunctionBody += _T("Out.Tangent = hs_Input[i].Tangent;\n");
		}
		{
			FunctionBody += _T("Out.Binormal = hs_Input[i].Binormal;\n");
		}
	}
}
void VSDX11Renderer::CreateHFunctionColor(MaterialShaderPara &MSPara, VSString & FunctionBody)
{
	VSVertexBuffer * pVBuffer = MSPara.pGeometry->GetMeshData()->GetVertexBuffer();
	for (unsigned int i = 0; i < pVBuffer->GetColorLevel(); i++)
	{
		if (pVBuffer->HaveColorInfo(i))
		{
			VSString ColorID = IntToString(i);
			FunctionBody += _T("Out.Color") + ColorID + _T(" = hs_Input[i].Color") + ColorID + _T(";\n");
		}
	}
}
void VSDX11Renderer::CreateHFunctionInstancePos(MaterialShaderPara &MSPara, VSString & FunctionBody)
{
	FunctionBody += _T("Out.Position1 = hs_Input[i].Position1;\n");
	FunctionBody += _T("Out.Position2 = hs_Input[i].Position2;\n");
	FunctionBody += _T("Out.Position3 = hs_Input[i].Position3;\n");
}
void VSDX11Renderer::CreateHFunctionMaterialInstance(MaterialShaderPara &MSPara, VSString & FunctionBody)
{
	VSVertexBuffer * pInstanceVertex = ((VSInstanceGeometry *)MSPara.pGeometry)->GetInstanceVertexBuffer();
	for (unsigned int i = 0; i < pInstanceVertex->GetMaterialInstanceLevel(); i++)
	{
		if (pInstanceVertex->HaveMaterialInstanceInfo(i))
		{
			VSString MaterialInstanceID = IntToString(i);
			FunctionBody += _T("Out.MaterialInstance") + MaterialInstanceID + _T(" = hs_Input[i].MaterialInstance") + MaterialInstanceID + _T(";\n");
		}
	}
}
void VSDX11Renderer::CreateHFunction(MaterialShaderPara &MSPara,
	VSString & OutString) 
{
	VSString FunctionBody;
	VSMAC_ASSERT(MSPara.bUseTessellation);
	CreateHFunctionWorldPos(MSPara, FunctionBody);
	CreateHFunctionTexCoord(MSPara, FunctionBody);
	CreateHFunctionNormal(MSPara,FunctionBody);
	CreateHFunctionColor(MSPara,FunctionBody);
	if (MSPara.pGeometry->IsSameType(VSInstanceGeometry::ms_Type))
	{
		CreateHFunctionInstancePos(MSPara, FunctionBody);
		CreateHFunctionMaterialInstance(MSPara, FunctionBody);
	}
	VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
	OutString += _T("PatchTess ConstantHS(InputPatch<HS_INPUT, 3> hs_Input){return FlatConstantHS(hs_Input[0].TessellationValue,hs_Input[1].TessellationValue,hs_Input[2].TessellationValue);}\n");
	OutString += _T("[domain(\"tri\")]\n");
	OutString += _T("[outputtopology(\"triangle_cw\")]\n");
	OutString += _T("[outputcontrolpoints(3)]\n");
	OutString += _T("[patchconstantfunc(\"ConstantHS\")]\n");
	VSString MaxTessFactor = RealToString(ms_fHSMaxTessFactor);
	OutString += _T("[maxtessfactor(") + MaxTessFactor + _T(")]\n");
	OutString += _T("[partitioning(\"") + ms_cTessellationPartitioning[pMaterial->GetTessellationPartitioning()] + _T("\")]\n");
	OutString += _T("HS_OUTPUT ") + ms_ShaderProgramMain[VSEngineFlag::ST_HULL] + _T("( InputPatch<HS_INPUT, 3> hs_Input, uint i : SV_OutputControlPointID)\n{\nHS_OUTPUT Out = (HS_OUTPUT) 0; \n") + FunctionBody + _T("return Out;\n};\n");
}


void VSDX11Renderer::CreateDInputDeclare(MaterialShaderPara &MSPara,
	VSString & OutString) 
{
	VSMAC_ASSERT(MSPara.bUseTessellation);
	VSString TempDeclare;
	unsigned int ID = 0;
	unsigned int uiPassType = MSPara.uiPassType;
	CreateDeclareWorldPos(MSPara, ID, TempDeclare, true);
	CreateDeclareTexCoord(MSPara, ID, TempDeclare, true);
	CreateDeclareNormal(MSPara, ID, TempDeclare, true);
	CreateDeclareColor(MSPara, TempDeclare, true);
	if (MSPara.pGeometry->IsSameType(VSInstanceGeometry::ms_Type))
	{
		CreateDeclareInstancePos(MSPara, ID, TempDeclare, true);
		CreateDeclareMaterialInstance(MSPara, TempDeclare, true);
	}
	OutString += _T("struct DS_INPUT \n{ \n") + TempDeclare + _T("};\n");

}
void VSDX11Renderer::CreateDOutputDeclare(MaterialShaderPara &MSPara,
	VSString & OutString) 
{
	VSMAC_ASSERT(MSPara.bUseTessellation);
	VSString TempDeclare;
	unsigned int ID = 0;
	unsigned int uiPassType = MSPara.uiPassType;
	CreateDeclarePosition(MSPara, TempDeclare, false);
	CreateDeclareWorldPos(MSPara, ID, TempDeclare, false);
	CreateDeclareTexCoord(MSPara, ID, TempDeclare, false);
	CreateDeclareNormal(MSPara, ID, TempDeclare, false);
	CreateDeclareColor(MSPara, TempDeclare, false);
	if (uiPassType == VSPass::PT_DUAL_PARABOLOID_SHADOW)
	{
		CreateDeclareProjectZ(MSPara, ID, TempDeclare, false);
	}
	if (MSPara.pGeometry->IsSameType(VSInstanceGeometry::ms_Type))
	{
		CreateDeclareInstancePos(MSPara, ID, TempDeclare, false);
		CreateDeclareMaterialInstance(MSPara, TempDeclare, false);
	}
	OutString += _T("struct DS_OUTPUT \n{ \n") + TempDeclare + _T("};\n");
}
void VSDX11Renderer::CreateDUserConstant(VSDShader * pDShader, MaterialShaderPara &MSPara,
	VSString & OutString) 
{
	
	VSMAC_ASSERT(MSPara.bUseTessellation);
	
	OutString += _T("cbuffer CB : register( b0 ) \n{\n");
	unsigned int uiRegisterID = 0;
	unsigned int uiPassType = MSPara.uiPassType;
	if (uiPassType == VSPass::PT_MATERIAL
		|| uiPassType == VSPass::PT_PREZ
		|| uiPassType == VSPass::PT_NORMALDEPTH
		|| uiPassType == VSPass::PT_INDIRECT
		|| uiPassType == VSPass::PT_POINT_CUBE_SHADOW)
	{
		CreateUserConstantWorldMatrix(pDShader, uiRegisterID, OutString);
		CreateUserConstantViewProjectMatrix(pDShader, uiRegisterID, OutString);
		CreateUserConstantCameraWorldPos(pDShader, uiRegisterID, OutString);
	}
	else if (uiPassType == VSPass::PT_SHADOW)
	{
		CreateUserConstantWorldMatrix(pDShader, uiRegisterID, OutString);
		CreateUserConstantLightShadowMatrix(pDShader, uiRegisterID, OutString);
		CreateUserConstantCameraWorldPos(pDShader, uiRegisterID, OutString);
	}
	else if (uiPassType == VSPass::PT_DUAL_PARABOLOID_SHADOW)
	{
		CreateUserConstantWorldMatrix(pDShader, uiRegisterID, OutString);
		CreateUserConstantViewProjectMatrix(pDShader, uiRegisterID, OutString);
		CreateUserConstantCameraWorldPos(pDShader, uiRegisterID, OutString);
		CreateUserConstantPointLightRange(pDShader, uiRegisterID, OutString);
	}

	VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
	pMaterial->CreateConstValueDeclare(OutString, uiRegisterID, MSPara);
	pMaterial->CreateCustomValue(pDShader);
	OutString += _T("} \n");
	unsigned uiTexRegisterID = 0;
	pMaterial->CreateTextureDeclare(OutString, uiTexRegisterID, MSPara);
	pMaterial->CreateCustomTexture(pDShader);

	
}
void VSDX11Renderer::CreateDFunction(MaterialShaderPara &MSPara,
	VSString & OutString) 
{
	VSMAC_ASSERT(MSPara.bUseTessellation);
	VSString FunctionBody;
	unsigned int uiPassType = MSPara.uiPassType;
	CreateDFunctionWorldPos(MSPara, FunctionBody);
	CreateDFunctionTexCoord(MSPara, FunctionBody);
	CreateDFunctionNormal(MSPara, FunctionBody);
	CreateDFunctionColor(MSPara, FunctionBody);
	if (MSPara.pGeometry->IsSameType(VSInstanceGeometry::ms_Type))
	{
		CreateDFunctionInstancePos(MSPara, FunctionBody);
		CreateDFunctionMaterialInstance(MSPara, FunctionBody);
	}
	VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
	pMaterial->GetDShaderTreeString(FunctionBody, MSPara);
	FunctionBody += _T("Out.WorldPos = ") + *VSShaderStringFactory::ms_WorldPos + _T("; \n");
	FunctionBody += _T("Out.Position = mul(float4(") + *VSShaderStringFactory::ms_WorldPos + _T(",1), ") +
		*VSShaderStringFactory::ms_ViewProjectMatrix + _T(");\n");
	if (uiPassType == VSPass::PT_DUAL_PARABOLOID_SHADOW)
	{
		CreateVFunctionProjectZ(MSPara, FunctionBody);
	}
	OutString += _T("[domain(\"tri\")]\n");
	OutString += _T("DS_OUTPUT ") + ms_ShaderProgramMain[VSEngineFlag::ST_DOMAIN] + _T("( PatchTess PTInput,float3 Coords: SV_DomainLocation, const OutputPatch<DS_INPUT, 3>  ds_Input)\n{\nDS_OUTPUT Out = (DS_OUTPUT) 0; \n") + FunctionBody + _T("return Out;\n};\n");
}
void VSDX11Renderer::CreateDFunctionWorldPos(MaterialShaderPara &MSPara, VSString & FunctionBody)
{
	FunctionBody += _T("float3 ") + VSShaderStringFactory::ms_InputWorldPos + _T(" = DSLerp(ds_Input[0].WorldPos,ds_Input[1].WorldPos,ds_Input[2].WorldPos,Coords);\n");
}
void VSDX11Renderer::CreateDFunctionTexCoord(MaterialShaderPara &MSPara, VSString & FunctionBody)
{
	VSVertexBuffer * pVBuffer = MSPara.pGeometry->GetMeshData()->GetVertexBuffer();
	for (unsigned int i = 0; i < pVBuffer->GetTexCoordLevel(); i++)
	{
		if (pVBuffer->HaveTexCoordInfo(i))
		{
			VSString TextureName = _T("TexCoord") + IntToString(i);

			FunctionBody += _T("float2 ") + VSShaderStringFactory::ms_TextureInputCoordValue[i] +
				_T(" = DSLerp(ds_Input[0].") + TextureName  + _T(",ds_Input[1].") + 
				TextureName + (",ds_Input[2].") + TextureName + _T(",Coords);\n");
			FunctionBody += _T("Out.") + TextureName + _T(" = ") + VSShaderStringFactory::ms_TextureInputCoordValue[i] + _T(";\n");
		}
	}
}
void VSDX11Renderer::CreateDFunctionNormal(MaterialShaderPara &MSPara, VSString & FunctionBody)
{
	VSVertexBuffer * pVBuffer = MSPara.pGeometry->GetMeshData()->GetVertexBuffer();
	if (pVBuffer->HaveNormalInfo(0))
	{
		FunctionBody += _T("float3 ") + VSShaderStringFactory::ms_InputLocalNormal + _T(" = DSLerp(ds_Input[0].Normal,ds_Input[1].Normal,ds_Input[2].Normal,Coords);\n");
		FunctionBody += _T("Out.Normal = ") + VSShaderStringFactory::ms_InputLocalNormal +_T(";\n");
	}

	if (pVBuffer->HaveTangentInfo())
	{
		{
			FunctionBody += _T("float3 ") + VSShaderStringFactory::ms_InputLocalTangent + _T(" = DSLerp(ds_Input[0].Tangent,ds_Input[1].Tangent,ds_Input[2].Tangent,Coords);\n");
			FunctionBody += _T("Out.Tangent = ") + VSShaderStringFactory::ms_InputLocalTangent + _T(";\n");
		}
		{
			FunctionBody += _T("float3 ") + VSShaderStringFactory::ms_InputLocalBinormal + _T(" = DSLerp(ds_Input[0].Binormal,ds_Input[1].Binormal,ds_Input[2].Binormal, Coords);\n");
			FunctionBody += _T("Out.Binormal = ") + VSShaderStringFactory::ms_InputLocalBinormal + _T(";\n");
		}
	}
}
void VSDX11Renderer::CreateDFunctionColor(MaterialShaderPara &MSPara, VSString & FunctionBody)
{
	VSVertexBuffer * pVBuffer = MSPara.pGeometry->GetMeshData()->GetVertexBuffer();
	for (unsigned int i = 0; i < pVBuffer->GetColorLevel(); i++)
	{
		if (pVBuffer->HaveColorInfo(i))
		{
			VSString ColorName = _T("Color") + IntToString(i);
			FunctionBody += _T("float4 ") + VSShaderStringFactory::ms_InputColor[i] +
				_T(" = DSLerp(ds_Input[0].") + ColorName + _T(",ds_Input[1].") +
				ColorName + (",ds_Input[2].") + ColorName + _T(",Coords);\n");
			FunctionBody += _T("Out.") + ColorName + _T(" = ") + VSShaderStringFactory::ms_InputColor[i] + _T(";\n");
		}
	}
}
void VSDX11Renderer::CreateDFunctionInstancePos(MaterialShaderPara &MSPara, VSString & FunctionBody)
{
	FunctionBody += _T("float4 ") + VSShaderStringFactory::ms_InputInstancePos[0] + _T(" = DSLerp(ds_Input[0].Position1,ds_Input[1].Position1,ds_Input[2].Position1,Coords);\n");
	FunctionBody += _T("Out.Position1 = ") + VSShaderStringFactory::ms_InputInstancePos[0] + _T(";\n");

	FunctionBody += _T("float4 ") + VSShaderStringFactory::ms_InputInstancePos[1] + _T(" = DSLerp(ds_Input[0].Position2,ds_Input[1].Position2,ds_Input[2].Position2,Coords);\n");
	FunctionBody += _T("Out.Position2 = ") + VSShaderStringFactory::ms_InputInstancePos[1] + _T(";\n");

	FunctionBody += _T("float4 ") + VSShaderStringFactory::ms_InputInstancePos[2] + _T(" = DSLerp(ds_Input[0].Position3,ds_Input[1].Position3,ds_Input[2].Position3,Coords);\n");
	FunctionBody += _T("Out.Position3 = ") + VSShaderStringFactory::ms_InputInstancePos[2] + _T(";\n");
}
void VSDX11Renderer::CreateDFunctionMaterialInstance(MaterialShaderPara &MSPara, VSString & FunctionBody)
{
	VSVertexBuffer * pInstanceVertex = ((VSInstanceGeometry *)MSPara.pGeometry)->GetInstanceVertexBuffer();
	for (unsigned int i = 0; i < pInstanceVertex->GetMaterialInstanceLevel(); i++)
	{
		if (pInstanceVertex->HaveMaterialInstanceInfo(i))
		{
			VSString MaterialInstanceName = _T("MaterialInstance") + IntToString(i);
			FunctionBody += FloatByChannelNum(pInstanceVertex->MaterialInstanceChannel(i)) + VSShaderStringFactory::ms_InputMaterialInstance[i] +
				_T(" = DSLerp(ds_Input[0].") + MaterialInstanceName + _T(",ds_Input[1].") +
				MaterialInstanceName + (",ds_Input[2].") + MaterialInstanceName + _T(",Coords);\n");
			FunctionBody += _T("Out.") + MaterialInstanceName + _T(" = ") + VSShaderStringFactory::ms_InputMaterialInstance[i] + _T(";\n");
		}
	}
}
#endif