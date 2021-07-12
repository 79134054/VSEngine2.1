#include "VSDx11Renderer.h"
#include "VSDx11Resource.h"
#include "VSTexture.h"
#include "VSCubeTexture.h"
#include "VSVertexBuffer.h"
#include "VSVShader.h"
#include "VSPShader.h"
#include "VSGShader.h"
#include "VSDShader.h"
#include "VSHShader.h"
#include "VSCShader.h"
#include "VSGeometry.h"
#include "VSSegment3.h"
#include "VS2DTexture.h"
#include "VS3DTexture.h"
#include "VSCubeTexture.h"
#include "VSDepthStencil.h"
#include "VSUserConstant.h"
#include "VSDx11ShaderInclude.h"
#include "VSShaderKey.h"
#include "VSRenderTarget.h"
#include "VSGraphicInclude.h"
#include "VSRenderThread.h"
#include "VSConfig.h"
#include "VSQuery.h"
#include "VSTimer.h"
#ifdef WINDOWS_PLATFORM
using namespace VSEngine2;
DECLEAR_TIME_PROFILENODE(DrawMesh, )
VSDx11ShaderInclude * VSDX11Renderer::ms_pDx11IncludeShader = NULL;
TCHAR VSDX11Renderer::ms_cVertexShaderProgramVersion[] = _T("vs_5_0");
TCHAR VSDX11Renderer::ms_cPixelShaderProgramVersion[] = _T("ps_5_0");
TCHAR VSDX11Renderer::ms_cGeometryShaderProgramVersion[] = _T("gs_5_0");
TCHAR VSDX11Renderer::ms_cDomainShaderProgramVersion[] = _T("ds_5_0");
TCHAR VSDX11Renderer::ms_cHullShaderProgramVersion[] = _T("hs_5_0");
TCHAR VSDX11Renderer::ms_cComputerShaderProgramVersion[] = _T("cs_5_0");
VSString VSDX11Renderer::ms_cTessellationPartitioning[] = {_T("fractional_even"),_T("fractional_odd") ,_T("integer"),_T("pow2")};
VSString VSDX11Renderer::ms_IncludeShader;
VSString VSDX11Renderer::ms_DynamicShader;
VSAdapterInfo	VSDX11Renderer::ms_AdapterInfo[5];
UINT VSDX11Renderer::ms_uiAdapterNum = 0;
VSREAL VSDX11Renderer::ms_fHSMaxTessFactor = 15.0f;
IDXGIFactory* VSDX11Renderer::ms_pDXGI = NULL;
DWORD VSDX11Renderer::ms_dwAlphaBlend[] =
{
	D3D11_BLEND_ZERO,
	D3D11_BLEND_ONE,
	D3D11_BLEND_SRC_COLOR,
	D3D11_BLEND_INV_SRC_COLOR,
	D3D11_BLEND_SRC_ALPHA,
	D3D11_BLEND_INV_SRC_ALPHA,
	D3D11_BLEND_DEST_ALPHA,
	D3D11_BLEND_INV_DEST_ALPHA,
	D3D11_BLEND_DEST_COLOR,
	D3D11_BLEND_INV_DEST_COLOR

};
DWORD VSDX11Renderer::ms_dwAlphaBlendOP[] =
{
	D3D11_BLEND_OP_ADD,
	D3D11_BLEND_OP_SUBTRACT,
	D3D11_BLEND_OP_REV_SUBTRACT,
	D3D11_BLEND_OP_MIN,
	D3D11_BLEND_OP_MAX
};
DWORD VSDX11Renderer::ms_dwCullType[] =
{
	D3D11_CULL_NONE,
	D3D11_CULL_FRONT,
	D3D11_CULL_BACK
};
DWORD VSDX11Renderer::ms_dwStencilOperation[] =
{
	D3D11_STENCIL_OP_KEEP,
	D3D11_STENCIL_OP_ZERO,
	D3D11_STENCIL_OP_REPLACE,
	D3D11_STENCIL_OP_INCR,
	D3D11_STENCIL_OP_DECR,
	D3D11_STENCIL_OP_INVERT,
	D3D11_STENCIL_OP_INCR_SAT,
	D3D11_STENCIL_OP_DECR_SAT
};
DWORD VSDX11Renderer::ms_dwCompare[] =
{
	D3D11_COMPARISON_NEVER,
	D3D11_COMPARISON_LESS,
	D3D11_COMPARISON_EQUAL,
	D3D11_COMPARISON_LESS_EQUAL,
	D3D11_COMPARISON_GREATER,
	D3D11_COMPARISON_NOT_EQUAL,
	D3D11_COMPARISON_GREATER_EQUAL,
	D3D11_COMPARISON_ALWAYS
};
DWORD VSDX11Renderer::ms_dwTextureFormatType[] =
{
	DXGI_FORMAT_B8G8R8A8_UNORM,
	DXGI_FORMAT_B8G8R8X8_UNORM,
	DXGI_FORMAT_D16_UNORM,
	DXGI_FORMAT_R32G32B32A32_FLOAT,
	DXGI_FORMAT_R16G16B16A16_FLOAT,
	DXGI_FORMAT_R16G16_FLOAT,
	DXGI_FORMAT_R16_FLOAT,
	DXGI_FORMAT_R32_FLOAT,
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_D24_UNORM_S8_UINT,
	DXGI_FORMAT_R32G32_FLOAT,
	DXGI_FORMAT_B5G6R5_UNORM,
	DXGI_FORMAT_A8_UNORM,
	DXGI_FORMAT_R16G16_UNORM,
	DXGI_FORMAT_R16G16B16A16_UNORM,
	DXGI_FORMAT_R8_UNORM,
	DXGI_FORMAT_R8G8B8A8_SNORM,
	DXGI_FORMAT_R16G16_SNORM,
	DXGI_FORMAT_R16_UNORM,
	DXGI_FORMAT_R16G16B16A16_SNORM,
	DXGI_FORMAT_BC1_UNORM,
	DXGI_FORMAT_BC2_UNORM,
	DXGI_FORMAT_BC3_UNORM,
	DXGI_FORMAT_BC4_UNORM,
	DXGI_FORMAT_BC5_UNORM
};
DWORD VSDX11Renderer::ms_dwSRGBTextureFormatType[] =
{
	DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,
	DXGI_FORMAT_B8G8R8X8_UNORM_SRGB,
	DXGI_FORMAT_D16_UNORM,
	DXGI_FORMAT_R32G32B32A32_FLOAT,
	DXGI_FORMAT_R16G16B16A16_FLOAT,
	DXGI_FORMAT_R16G16_FLOAT,
	DXGI_FORMAT_R16_FLOAT,
	DXGI_FORMAT_R32_FLOAT,
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_D24_UNORM_S8_UINT,
	DXGI_FORMAT_R32G32_FLOAT,
	DXGI_FORMAT_B5G6R5_UNORM,
	DXGI_FORMAT_A8_UNORM,
	DXGI_FORMAT_R16G16_UNORM,
	DXGI_FORMAT_R16G16B16A16_UNORM,
	DXGI_FORMAT_R8_UNORM,
	DXGI_FORMAT_R8G8B8A8_SNORM,
	DXGI_FORMAT_R16G16_SNORM,
	DXGI_FORMAT_R16_UNORM,
	DXGI_FORMAT_R16G16B16A16_SNORM,
	DXGI_FORMAT_BC1_UNORM_SRGB,
	DXGI_FORMAT_BC2_UNORM_SRGB,
	DXGI_FORMAT_BC3_UNORM_SRGB,
	DXGI_FORMAT_BC4_UNORM,
	DXGI_FORMAT_BC5_UNORM
};
UINT VSDX11Renderer::ms_dwMultisampleQualityLevel[VSRenderer::SFT_MAX][VSRenderer::MS_MAX] = { 0 };
DWORD VSDX11Renderer::ms_dwTextureFilterType[VSSamplerDesc::FM_MAX][VSSamplerDesc::FM_MAX][VSSamplerDesc::FM_MAX] =
{
	D3D11_FILTER_MIN_MAG_MIP_POINT,	// none none none
	D3D11_FILTER_MIN_MAG_MIP_POINT,	// none none point
	D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR, // none none line
	D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR, // none none anisotropic

	D3D11_FILTER_MIN_MAG_MIP_POINT,// none point none
	D3D11_FILTER_MIN_MAG_MIP_POINT,// none point point
	D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR, // none point line
	D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR, // none point anisotropic

	D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT,// none line none
	D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT,// none line point
	D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR, // none line line
	D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR, // none line anisotropic

	D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT, //none, anisotropic, none
	D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT,	//none, anisotropic, point
	D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR, // none anisotropic line
	D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR, // none anisotropic anisotropic



	D3D11_FILTER_MIN_MAG_MIP_POINT, //point, none, none
	D3D11_FILTER_MIN_MAG_MIP_POINT,	//point, none, point
	D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR, // point none line
	D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR, // point none anisotropic

	D3D11_FILTER_MIN_MAG_MIP_POINT, //point, point, none
	D3D11_FILTER_MIN_MAG_MIP_POINT,	//point, point, point
	D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR, // point point line
	D3D11_FILTER_MIN_MAG_MIP_POINT, // point point anisotropic

	D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT, //point, line, none
	D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT,	//point, line, point
	D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR, // point line line
	D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR, // point line anisotropic

	D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT, //point, anisotropic, none
	D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT,	//point, anisotropic, point
	D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR, // point anisotropic line
	D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR, // point anisotropic anisotropic



	D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT, //line, none, none
	D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT,	//line, none, point
	D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR, // line none line
	D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR, // line none anisotropic

	D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT, //line, point, none
	D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT,	//line, point, point
	D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR, // line point line
	D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR, // line point anisotropic

	D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT, //line, line, none
	D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT,	//line, line, point
	D3D11_FILTER_MIN_MAG_MIP_LINEAR, // line line line
	D3D11_FILTER_MIN_MAG_MIP_LINEAR, // line line anisotropic

	D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT, //line, anisotropic, none
	D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT,	//line, anisotropic, point
	D3D11_FILTER_MIN_MAG_MIP_LINEAR, // line anisotropic line
	D3D11_FILTER_MIN_MAG_MIP_LINEAR, // line anisotropic anisotropic



	D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT, //anisotropic, none, none
	D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT,	//anisotropic, none, point
	D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR, // anisotropic none line
	D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR, // anisotropic none anisotropic

	D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT, //anisotropic, point, none
	D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT,	//anisotropic, point, point
	D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR, // anisotropic point line
	D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR, // anisotropic point anisotropic

	D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT, //anisotropic, line, none
	D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT,	//anisotropic, line, point
	D3D11_FILTER_MIN_MAG_MIP_LINEAR, // anisotropic line line
	D3D11_FILTER_MIN_MAG_MIP_LINEAR, // anisotropic line anisotropic

	D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT, //anisotropic, anisotropic, none
	D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT,	//anisotropic, anisotropic, point
	D3D11_FILTER_MIN_MAG_MIP_LINEAR, // anisotropic anisotropic line
	D3D11_FILTER_ANISOTROPIC, // anisotropic anisotropic anisotropic
	 
};
DWORD VSDX11Renderer::ms_dwTextureAddrType[] =
{
	D3D11_TEXTURE_ADDRESS_CLAMP,
	D3D11_TEXTURE_ADDRESS_WRAP,
	D3D11_TEXTURE_ADDRESS_MIRROR,
	D3D11_TEXTURE_ADDRESS_BORDER,
};

DWORD VSDX11Renderer::ms_dwPrimitiveType[] =
{
	D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
	D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
};
DWORD VSDX11Renderer::ms_dwDevTypes[] = 
{ 
	D3D_DRIVER_TYPE_HARDWARE,
	D3D_DRIVER_TYPE_REFERENCE
};
DWORD VSDX11Renderer::ms_dwMultiSampleTypes[] =
{
	1 ,
	2 ,
	4 ,
	8 ,
	16 
};
DWORD VSDX11Renderer::ms_dwLockFlag[] = 
{
	D3D11_MAP_WRITE_NO_OVERWRITE,
	D3D11_MAP_WRITE_DISCARD,
	D3D11_MAP_READ
};
DWORD VSDX11Renderer::ms_dwVertexFormatDataType[] = 
{
	DXGI_FORMAT_R32_FLOAT,
	DXGI_FORMAT_R32G32_FLOAT,
	DXGI_FORMAT_R32G32B32_FLOAT,
	DXGI_FORMAT_R32G32B32A32_FLOAT,

	DXGI_FORMAT_R16_UINT,
	DXGI_FORMAT_R32_UINT,

	DXGI_FORMAT_R8G8B8A8_UINT,
	DXGI_FORMAT_R8G8B8A8_UNORM,

	DXGI_FORMAT_R16G16_SINT,
	DXGI_FORMAT_R16G16_SNORM,
	DXGI_FORMAT_R16G16_UNORM,

	DXGI_FORMAT_R16G16B16A16_SINT,
	DXGI_FORMAT_R16G16B16A16_SNORM,
	DXGI_FORMAT_R16G16B16A16_UNORM,

	DXGI_FORMAT_R16G16_FLOAT,
	DXGI_FORMAT_R16G16B16A16_FLOAT,

	DXGI_FORMAT_R8G8B8A8_UNORM,

	DXGI_FORMAT_R32_SINT,
	DXGI_FORMAT_R32G32_SINT,
	DXGI_FORMAT_R32G32B32_SINT,
	DXGI_FORMAT_R32G32B32A32_SINT,

	DXGI_FORMAT_R32G32_UINT,
	DXGI_FORMAT_R32G32B32_UINT,
	DXGI_FORMAT_R32G32B32A32_UINT,

	DXGI_FORMAT_R16_FLOAT,
};
DWORD VSDX11Renderer::ms_dwQueryType[] =
{
	D3D11_QUERY_OCCLUSION,
};
TCHAR* VSDX11Renderer::ms_dwVertexFormatClareType[] =
{
	"float ",
	"float2 ",
	"float3 ",
	"float4 ",

	"uint ",
	"uint ",

	"uint4 ",
	"float4 ",

	"int2 ",
	"float2 ",
	"float2 ",

	"int4 ",
	"float4 ",
	"float4 ",

	"float2 ",
	"float4 ",

	"float4 "
};
TCHAR* VSDX11Renderer::ms_dwVertexFormatSemantics[] =
{
	"POSITION",
	"TEXCOORD",
	"NORMAL",
	"TANGENT",
	"BINORMAL",
	"PSIZE",
	"COLOR",
	"FOG",
	"Depth",
	"BLENDWEIGHT",
	"BLENDINDICES",
	"MATERIAL_INSTANCE"
};
int VSDX11Renderer::GetRendererType() const
{

	return VSRenderer::RAT_DIRECTX11;
}
VSDX11Renderer::VSDX11Renderer(HWND hMainWindow, unsigned int uiScreenWidth, unsigned int uiScreenHeight, bool bIsWindowed,
							  unsigned int uiAnisotropy,unsigned int uiMultisample,
							  ChildWindowInfo * pChildWindow,int uiNumChildWindow)
{
	
	if (uiNumChildWindow > 0 && pChildWindow && bIsWindowed) 
	{
		m_pChildWindowInfo = VS_NEW ChildWindowInfo[uiNumChildWindow];
		VSMemcpy(m_pChildWindowInfo, pChildWindow, sizeof(ChildWindowInfo) * uiNumChildWindow);
		m_iNumChildWindow = uiNumChildWindow;
	}
	else 
	{
		uiNumChildWindow = 0;

	}

	m_hMainWindow  = hMainWindow;
	m_bWindowed = bIsWindowed;
	m_uiScreenHeight = uiScreenHeight;
	m_uiScreenWidth = uiScreenWidth;
	m_uiCurRTWidth = uiScreenWidth;
	m_uiCurRTHeight = uiScreenHeight;

	HRESULT hResult = NULL;
	m_uinAdapter = 0;
	hResult = ms_pDXGI->EnumAdapters(m_uinAdapter, &m_pAdapter);
	VSMAC_ASSERT(!FAILED(hResult));

	DXGI_ADAPTER_DESC AdapterDesc;
	hResult = m_pAdapter->GetDesc(&AdapterDesc);
	VSMAC_ASSERT(!FAILED(hResult));
	hResult = m_pAdapter->EnumOutputs(0, &m_pAdapterOutput);
	VSMAC_ASSERT(!FAILED(hResult));

	TCHAR TempName[128];
	VSWcsToMbs(TempName, 128, AdapterDesc.Description, 128);
	m_AdapterName = TempName;


	unsigned int DeviceFlags = 0; ;

	DeviceFlags |= D3D11_CREATE_DEVICE_SINGLETHREADED;
#if _DEBUG 
	DeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	UINT numDriverTypes = ARRAYSIZE(ms_dwDevTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);
	D3D_FEATURE_LEVEL OutFeatureLevel;

	
	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		hResult = D3D11CreateDevice(
			NULL,
			(D3D_DRIVER_TYPE)ms_dwDevTypes[driverTypeIndex],
			NULL,
			DeviceFlags,
			featureLevels,
			numFeatureLevels,
			D3D11_SDK_VERSION,
			&m_pDevice,
			&OutFeatureLevel,
			&m_pDeviceContext
			);
		if (SUCCEEDED(hResult))
		{
			m_uiDevType = driverTypeIndex;
			break;
		}
	}
	m_uiMaxMultisample = 0;
	unsigned int NumMultiSampleQualities = 0;
	for (int uiMultiSampleTypes = MS_MAX - 1; uiMultiSampleTypes >= MS_NONE; uiMultiSampleTypes--)
	{
		hResult = m_pDevice->CheckMultisampleQualityLevels((DXGI_FORMAT)ms_dwTextureFormatType[SFT_A8R8G8B8], ms_dwMultiSampleTypes[uiMultiSampleTypes], &NumMultiSampleQualities);
		if (SUCCEEDED(hResult) && NumMultiSampleQualities > 0)
		{
			m_uiMaxMultisample = uiMultiSampleTypes;
			break;
		}
	}
	m_SupportFeature[SF_MulBufferSwtich] = false;
	m_SupportFeature[SF_GS] = true;
	m_SupportFeature[SF_MaterialValueInstance] = true;
	m_SupportFeature[SF_Tessellation] = true;
	m_SupportFeature[SF_MSBufferRead] = true;
	m_SupportFeature[SF_DepthStensilBufferRead] = true;
	m_SupportFeature[SF_DepthStensilToTexture] = true;
	m_SupportFeature[SF_BufferResource] = true;
	m_SupportFeature[SF_UnorderAccess] = true;
	m_SupportFeature[SF_CS] = true;
	m_SupportFeature[SF_ClearRange] = false;
	m_SupportFeature[SF_VertexIDInShader] = true;
	m_SupportFeature[SF_ShareSampler] = true;
	m_SupportFeature[SF_VShaderSampler] = true;
	m_SupportFeature[SF_AdvanceInstance] = VSConfig::ms_EnableAdvanceInstance;
	m_SupportFeature[SF_NoPow2Texture] = true;
	

	m_pMainChain = NULL;
	m_uiBufferFormat = SFT_A8R8G8B8;
	m_uiDepthStencilFormat = SFT_D24S8;


	if (uiMultisample > m_uiMaxMultisample)
	{
		m_uiCurMultisample = m_uiMaxMultisample;
	}
	else
	{
		m_uiCurMultisample = uiMultisample;
	}

	m_uiCurRTMultisample = m_uiCurMultisample;
	hResult = m_pDevice->CheckMultisampleQualityLevels((DXGI_FORMAT)ms_dwTextureFormatType[SFT_A8R8G8B8], ms_dwMultiSampleTypes[m_uiCurMultisample], &NumMultiSampleQualities);
	VSMAC_ASSERT(!FAILED(hResult) && NumMultiSampleQualities > 0);
	m_dwMultisampleQuality = NumMultiSampleQualities;
	m_uiCurAnisotropy = D3D11_MAX_MAXANISOTROPY;

	m_uiMaxRTNum = D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT;

	m_uiMaxBindResource[VSEngineFlag::ST_PIXEL] = D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT;
	m_uiMaxBindResource[VSEngineFlag::ST_VERTEX] = D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT;
	m_uiMaxBindResource[VSEngineFlag::ST_GEOMETRY] = D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT;
	m_uiMaxBindResource[VSEngineFlag::ST_DOMAIN] = D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT;
	m_uiMaxBindResource[VSEngineFlag::ST_HULL] = D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT;
	m_uiMaxBindResource[VSEngineFlag::ST_COMPUTE] = D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT;

	m_uiMaxSampler[VSEngineFlag::ST_PIXEL] = D3D11_COMMONSHADER_SAMPLER_REGISTER_COUNT;
	m_uiMaxSampler[VSEngineFlag::ST_VERTEX] = D3D11_COMMONSHADER_SAMPLER_REGISTER_COUNT;
	m_uiMaxSampler[VSEngineFlag::ST_GEOMETRY] = D3D11_COMMONSHADER_SAMPLER_REGISTER_COUNT;
	m_uiMaxSampler[VSEngineFlag::ST_DOMAIN] = D3D11_COMMONSHADER_SAMPLER_REGISTER_COUNT;
	m_uiMaxSampler[VSEngineFlag::ST_HULL] = D3D11_COMMONSHADER_SAMPLER_REGISTER_COUNT;
	m_uiMaxSampler[VSEngineFlag::ST_COMPUTE] = D3D11_COMMONSHADER_SAMPLER_REGISTER_COUNT;

	m_uiMaxUAVNum = D3D11_PS_CS_UAV_REGISTER_COUNT;

	m_uiMaxTextureHeight = 8192;
	m_uiMaxTextureWidth = 8192;
	m_uiMaxUseClipPlane = 8;

	m_pMainRenderTargetView = NULL;
	m_pMainDepthStencilView = NULL;
	m_pMainDepthStencil = NULL;

	m_pChain = NULL;
	m_pChainRenderTargetView = NULL;
	m_pChainDepthStencil = NULL;
	m_pChainDepthStencilView = NULL;

	if (m_hMainWindow != NULL)
	{
		VSMemset(&m_SwapChainDesc, 0, sizeof(m_SwapChainDesc));
	
		m_SwapChainDesc.BufferCount = 2;
		m_SwapChainDesc.BufferDesc.Width = uiScreenWidth;
		m_SwapChainDesc.BufferDesc.Height = uiScreenHeight;
		m_SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		m_SwapChainDesc.OutputWindow = m_hMainWindow;
		m_SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		m_SwapChainDesc.Windowed = m_bWindowed;
		m_SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		m_SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;


		m_SwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		m_SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		m_SwapChainDesc.SampleDesc.Count = ms_dwMultiSampleTypes[m_uiCurMultisample];
		m_SwapChainDesc.SampleDesc.Quality = m_dwMultisampleQuality - 1;
		IDXGIDevice * pDXGIDevice;
		hResult = m_pDevice->QueryInterface(IID_IDXGIDevice, (void**)&pDXGIDevice);
		VSMAC_ASSERT(!FAILED(hResult));
		hResult = ms_pDXGI->CreateSwapChain(pDXGIDevice, &m_SwapChainDesc, &m_pMainChain);
		VSMAC_ASSERT(!FAILED(hResult));


		ID3D11Texture2D* pBackBuffer = NULL;
		hResult = m_pMainChain->GetBuffer(0, IID_ID3D11Texture2D, (LPVOID*)&pBackBuffer);
		VSMAC_ASSERT(!FAILED(hResult));

		hResult = m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pMainRenderTargetView);
		VSMAC_ASSERT(!FAILED(hResult));
		VSMAC_RELEASE(pBackBuffer);


		D3D11_TEXTURE2D_DESC descDepth;
		ZeroMemory(&descDepth, sizeof(descDepth));
		descDepth.Width = uiScreenWidth;
		descDepth.Height = uiScreenHeight;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDepth.SampleDesc.Count = ms_dwMultiSampleTypes[m_uiCurMultisample];
		descDepth.SampleDesc.Quality = m_dwMultisampleQuality - 1;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		hResult = m_pDevice->CreateTexture2D(&descDepth, NULL, &m_pMainDepthStencil);
		VSMAC_ASSERT(!FAILED(hResult));


		// Create the depth stencil view
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = descDepth.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		hResult = m_pDevice->CreateDepthStencilView(m_pMainDepthStencil, &descDSV, &m_pMainDepthStencilView);
		VSMAC_ASSERT(!FAILED(hResult));

		
		if ((m_iNumChildWindow > 0) && m_bWindowed)
		{
			m_pChain = VS_NEW IDXGISwapChain*[m_iNumChildWindow];
			m_pChainRenderTargetView = VS_NEW ID3D11RenderTargetView*[m_iNumChildWindow];
			m_pChainDepthStencil = VS_NEW ID3D11Texture2D*[m_iNumChildWindow];
			m_pChainDepthStencilView = VS_NEW ID3D11DepthStencilView*[m_iNumChildWindow];
			for (int i = 0; i < m_iNumChildWindow; i++)
			{
				m_pChain[i] = NULL;
				m_pChainRenderTargetView[i] = NULL;
				m_pChainDepthStencil[i] = NULL;
				m_pChainDepthStencilView[i] = NULL;
			}
			for (int i = 0; i < m_iNumChildWindow; i++)
			{
				m_SwapChainDesc.OutputWindow = m_pChildWindowInfo[i].m_hHwnd;
				m_SwapChainDesc.BufferDesc.Width = m_pChildWindowInfo[i].m_uiWidth;
				m_SwapChainDesc.BufferDesc.Height = m_pChildWindowInfo[i].m_uiHeight;
				hResult = ms_pDXGI->CreateSwapChain(pDXGIDevice, &m_SwapChainDesc, &m_pChain[i]);
				VSMAC_ASSERT(!FAILED(hResult));
				ID3D11Texture2D* pBackBuffer = NULL;
				hResult = m_pChain[i]->GetBuffer(0, IID_ID3D11Texture2D, (LPVOID*)&pBackBuffer);
				VSMAC_ASSERT(!FAILED(hResult));
				hResult = m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pChainRenderTargetView[i]);
				VSMAC_ASSERT(!FAILED(hResult));
				VSMAC_RELEASE(pBackBuffer);
				if (m_pChildWindowInfo[i].m_bDepth)
				{
					descDepth.Width = m_pChildWindowInfo[i].m_uiWidth;
					descDepth.Height = m_pChildWindowInfo[i].m_uiHeight;
					hResult = m_pDevice->CreateTexture2D(&descDepth, NULL, &m_pChainDepthStencil[i]);
					VSMAC_ASSERT(!FAILED(hResult));
					hResult = m_pDevice->CreateDepthStencilView(m_pChainDepthStencil[i], &descDSV, &m_pChainDepthStencilView[i]);
					VSMAC_ASSERT(!FAILED(hResult));
				}
			}
		}
		VSMAC_RELEASE(pDXGIDevice);
		m_pDeviceContext->OMSetRenderTargets(1, &m_pMainRenderTargetView, m_pMainDepthStencilView);
		SetViewPort();
	}

	SetDefaultValue();
	VSMemset(m_uiShaderBuffer, 0, MAX_SHADER_BUFFER);
	unsigned int uiFontNum = 200;
	D3D11_BUFFER_DESC VertexBufferDesc;
	VertexBufferDesc.ByteWidth = sizeof(SCREEN_QUAD_TYPE)* 4 * uiFontNum;
	VertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; 
	VertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	VertexBufferDesc.MiscFlags = 0;
	hResult = m_pDevice->CreateBuffer(&VertexBufferDesc, NULL, &m_pFontVertexBuffer);
	VSMAC_ASSERT(!FAILED(hResult));
	D3D11_BUFFER_DESC IndexBufferDesc;
	IndexBufferDesc.ByteWidth = sizeof(VSUSHORT_INDEX) * 6 * uiFontNum;
	IndexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	IndexBufferDesc.MiscFlags = 0;
	hResult = m_pDevice->CreateBuffer(&IndexBufferDesc, NULL, &m_pFontIndexBuffer);
	VSMAC_ASSERT(!FAILED(hResult));

	for (unsigned int i = 0 ; i < VSRenderer::SFT_MAX ; i++)
	{
		for (unsigned int j = 0 ; j < VSRenderer::VSRenderer::MS_MAX ; j++)
		{
			hResult = m_pDevice->CheckMultisampleQualityLevels((DXGI_FORMAT)ms_dwTextureFormatType[i], ms_dwMultiSampleTypes[j], &ms_dwMultisampleQualityLevel[i][j]);
			if (FAILED(hResult))
			{
				ms_dwMultisampleQualityLevel[i][j] = 1;
			}
		}
	}


	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = 4096;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	hResult = m_pDevice->CreateBuffer(&bd, NULL, &m_pVShaderBuffer);
	VSMAC_ASSERT(!FAILED(hResult));
	hResult = m_pDevice->CreateBuffer(&bd, NULL, &m_pPShaderBuffer);
	VSMAC_ASSERT(!FAILED(hResult));
	hResult = m_pDevice->CreateBuffer(&bd, NULL, &m_pHShaderBuffer);
	VSMAC_ASSERT(!FAILED(hResult));
	hResult = m_pDevice->CreateBuffer(&bd, NULL, &m_pDShaderBuffer);
	VSMAC_ASSERT(!FAILED(hResult));
	hResult = m_pDevice->CreateBuffer(&bd, NULL, &m_pGShaderBuffer);
	VSMAC_ASSERT(!FAILED(hResult));
	hResult = m_pDevice->CreateBuffer(&bd, NULL, &m_pCShaderBuffer);
	VSMAC_ASSERT(!FAILED(hResult));

	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pVShaderBuffer);
	m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pPShaderBuffer);
	m_pDeviceContext->HSSetConstantBuffers(0, 1, &m_pHShaderBuffer);
	m_pDeviceContext->DSSetConstantBuffers(0, 1, &m_pDShaderBuffer);
	m_pDeviceContext->GSSetConstantBuffers(0, 1, &m_pGShaderBuffer);
	m_pDeviceContext->CSSetConstantBuffers(0, 1, &m_pCShaderBuffer);
}
VSDX11Renderer::~VSDX11Renderer()
{
	
 	ReleaseDefaultValue();
	VSMAC_RELEASE(m_pVShaderBuffer);
	VSMAC_RELEASE(m_pPShaderBuffer);
	VSMAC_RELEASE(m_pHShaderBuffer);
	VSMAC_RELEASE(m_pDShaderBuffer);
	VSMAC_RELEASE(m_pGShaderBuffer);
	VSMAC_RELEASE(m_pCShaderBuffer);

	VSMAC_RELEASE(m_pFontVertexBuffer);
	VSMAC_RELEASE(m_pFontIndexBuffer);
	VSMAC_RELEASE(m_pMainDepthStencilView);
	VSMAC_RELEASE(m_pMainRenderTargetView);
 	for (int i = 0 ; i < m_iNumChildWindow ;i++)
 	{
		VSMAC_RELEASE(m_pChainDepthStencilView[i]);
		VSMAC_RELEASE(m_pChainDepthStencil[i]);	
 		VSMAC_RELEASE(m_pChainRenderTargetView[i]);
		VSMAC_RELEASE(m_pChain[i]);
 	}
	VSMAC_RELEASE(m_pMainDepthStencil);
	VSMAC_RELEASE(m_pMainChain);

	VSMAC_DELETEA(m_pChain);
	VSMAC_DELETEA(m_pChainRenderTargetView);
	VSMAC_DELETEA(m_pChainDepthStencil);
	VSMAC_DELETEA(m_pChainDepthStencilView);

	VSMAC_RELEASE(m_pAdapter);
	VSMAC_RELEASE(m_pAdapterOutput);
	VSMAC_RELEASE(m_pDeviceContext);
 	VSMAC_RELEASE(m_pDevice);
}
IMPLEMENT_PRIORITY(VSDX11Renderer)
IMPLEMENT_INITIAL_ONLY_BEGIN(VSDX11Renderer)
ADD_PRIORITY(VSResourceManager);
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState);
IMPLEMENT_INITIAL_ONLY_END
bool VSDX11Renderer::InitialDefaultState()
{
	IDXGIAdapter* pAdapter;
	IDXGIOutput* pAdapterOutput;
	VSMAC_RELEASE(ms_pDXGI);
	HRESULT hResult = NULL;
	hResult = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&ms_pDXGI);
	VSMAC_ASSERT(!FAILED(hResult));
	VSMAC_ASSERT(ms_pDXGI);
	ms_uiAdapterNum = 0;
	if (ms_pDXGI->EnumAdapters(ms_uiAdapterNum, &pAdapter) == DXGI_ERROR_NOT_FOUND)
	{
		return false;
	}
	if (pAdapter->EnumOutputs(0, &pAdapterOutput) == DXGI_ERROR_NOT_FOUND)
	{
		return false;
	}
	VSMAC_RELEASE(pAdapterOutput);
	VSMAC_RELEASE(pAdapter);
	ms_uiAdapterNum++;
	while (true)
	{
		if (ms_pDXGI->EnumAdapters(ms_uiAdapterNum, &pAdapter) != DXGI_ERROR_NOT_FOUND)
		{
			if (pAdapter->EnumOutputs(0, &pAdapterOutput) != DXGI_ERROR_NOT_FOUND)
			{
				VSMAC_RELEASE(pAdapterOutput);
				VSMAC_RELEASE(pAdapter);
				ms_uiAdapterNum++;
			}
			else
			{
				VSMAC_RELEASE(pAdapter);
				break;
			}
			
		}
		else
		{
			break;
		}
	}

	DXGI_ADAPTER_DESC AdapterDesc;
	for (unsigned int nAdapter = 0; nAdapter < ms_uiAdapterNum; nAdapter++)
	{
		hResult = ms_pDXGI->EnumAdapters(nAdapter, &pAdapter);
		VSMAC_ASSERT(!FAILED(hResult));
		hResult = pAdapter->GetDesc(&AdapterDesc);
		VSMAC_ASSERT(!FAILED(hResult));
		hResult = pAdapter->EnumOutputs(0, &pAdapterOutput);
		VSMAC_ASSERT(!FAILED(hResult));
		TCHAR TempName[128];
		VSWcsToMbs(TempName, 128, AdapterDesc.Description, 128);
		ms_AdapterInfo[nAdapter].AdapterName = TempName;

		unsigned int DisplayModeNumA8R8G8B8 = 0;
		hResult = pAdapterOutput->GetDisplayModeList((DXGI_FORMAT)ms_dwTextureFormatType[VSRenderer::SFT_A8R8G8B8], DXGI_ENUM_MODES_INTERLACED, &DisplayModeNumA8R8G8B8, NULL);
		VSMAC_ASSERT(!FAILED(hResult));
		ms_AdapterInfo[nAdapter].uiDisplayModeNum = DisplayModeNumA8R8G8B8;
		ms_AdapterInfo[nAdapter].DisplayMode.Clear();
		ms_AdapterInfo[nAdapter].DisplayMode.SetBufferNum(DisplayModeNumA8R8G8B8);
		DXGI_MODE_DESC* displayModes = VS_NEW DXGI_MODE_DESC[DisplayModeNumA8R8G8B8];
		hResult = pAdapterOutput->GetDisplayModeList((DXGI_FORMAT)ms_dwTextureFormatType[VSRenderer::SFT_A8R8G8B8], DXGI_ENUM_MODES_INTERLACED, &DisplayModeNumA8R8G8B8, displayModes);
		VSMAC_ASSERT(!FAILED(hResult));
		VSMAC_RELEASE(pAdapterOutput);
		VSMAC_RELEASE(pAdapter);
		for (UINT nMode = 0; nMode < DisplayModeNumA8R8G8B8; nMode++)
		{
			TCHAR temp[20];
			
			ms_AdapterInfo[nAdapter].DisplayMode[nMode].uiBitFormat = VSRenderer::SFT_A8R8G8B8;
			ms_AdapterInfo[nAdapter].DisplayMode[nMode].uiHeight = displayModes[nMode].Height;
			ms_AdapterInfo[nAdapter].DisplayMode[nMode].uiWidth = displayModes[nMode].Width;
			ms_AdapterInfo[nAdapter].DisplayMode[nMode].uiRefreshRate = displayModes[nMode].RefreshRate.Numerator / displayModes[nMode].RefreshRate.Denominator;
			VSSprintf(temp,20, "%d * %d * 32", displayModes[nMode].Width, displayModes[nMode].Height);
			ms_AdapterInfo[nAdapter].DisplayMode[nMode].StringExpress = temp;
		}
		VSMAC_DELETEA(displayModes);
	}

	VSString RenderAPIPre = VSRenderer::GetRenderTypeShaderPath(RAT_DIRECTX11);
	VSFile IncludeShaderFile;
	VSString IncludeShaderPath = VSShaderMapCache::ms_ResourcePath + RenderAPIPre + _T("Shader.txt");
	if (!IncludeShaderFile.Open(IncludeShaderPath.GetBuffer(),VSFile::OM_RB))
	{
		return false;
	}
	unsigned int uiIncludeSize = IncludeShaderFile.GetFileSize();
	ms_IncludeShader.SetTCHARBufferNum(uiIncludeSize);
	if (!IncludeShaderFile.Read(ms_IncludeShader.GetBuffer(),uiIncludeSize,1))
	{
		return false;
	}


	VSFile DynamicShaderFile;
	VSString DynamicShaderPath = VSShaderMapCache::ms_ResourcePath + RenderAPIPre + _T("DynamicShader.txt");
	if(!DynamicShaderFile.Open(DynamicShaderPath.GetBuffer(),VSFile::OM_RB))
	{
		return false;
	}
	unsigned int uiSize = DynamicShaderFile.GetFileSize();
	VSString VDynamicShaderString;
	VDynamicShaderString.SetTCHARBufferNum(uiSize);
	if(!DynamicShaderFile.Read(VDynamicShaderString.GetBuffer(),uiSize,1))
	{
		return false;
	}
	unsigned int MaxSimpleInstanceAnim = MAX_SIMPLE_INSTANCE_ANIM;
	ms_DynamicShader.Format(VDynamicShaderString.GetBuffer(), VSResourceManager::GetGpuSkinBoneNum() * 3, MaxSimpleInstanceAnim);

	ms_pDx11IncludeShader = VS_NEW VSDx11ShaderInclude();
	VSMAC_ASSERT(ms_pDx11IncludeShader)
	return 1;
}
bool VSDX11Renderer::TerminalDefaultState()
{
	VSMAC_DELETE(ms_pDx11IncludeShader);
	VSMAC_RELEASE(ms_pDXGI);
	return 1;
}
void VSDX11Renderer::ClearBuffers(unsigned int uiClearFlag)
{ 
	if (!uiClearFlag)
	{
		return;
	}
	DWORD Flag = 0;
	if (uiClearFlag & CF_COLOR)
	{
		ClearBackBuffer();
	}
	
	if (uiClearFlag & CF_DEPTH)
	{
		Flag |= D3D11_CLEAR_DEPTH;
	}
	if (uiClearFlag & CF_STENCIL)
	{
		Flag |= D3D11_CLEAR_STENCIL;
	}
	if (Flag)
	{
		ClearDepthStencilView(Flag, m_fClearDepth, m_uiClearStencil);
	}
}


void VSDX11Renderer::ClearZBuffer()
{
	ClearDepthStencilView(D3D11_CLEAR_DEPTH, m_fClearDepth, m_uiClearStencil);
	
}
void VSDX11Renderer::ClearStencilBuffer()
{ 
	ClearDepthStencilView(D3D11_CLEAR_STENCIL, m_fClearDepth, m_uiClearStencil);
	
}
void VSDX11Renderer::ClearBuffers()
{
	ClearBuffers(CF_COLOR | CF_DEPTH |CF_STENCIL);
}
unsigned int VSDX11Renderer::SetDepthStencilState(VSDepthStencilState * pDepthStencilState, bool bForceSet)
{
	unsigned int FRI = VSRenderer::SetDepthStencilState(pDepthStencilState, bForceSet);
	if (FRI == FRI_SAMERESOURCE)
		return 1;
	else if (FRI == FRI_FAIL)
	{
		OMSetDepthStencilState(NULL, 0);
		return 0;
	}
	if (!pDepthStencilState)
	{
		pDepthStencilState = (VSDepthStencilState *)VSDepthStencilState::GetDefault();
	}
	VSDepthStencilStateID *pDepthStencilStateID = (VSDepthStencilStateID *)pDepthStencilState->GetIdentifier();
	if (!pDepthStencilStateID)
		return 0;

	m_pDepthStencilState = pDepthStencilState;
	OMSetDepthStencilState(pDepthStencilStateID, pDepthStencilState->GetDepthStencilDesc().m_uiReference);
	return true;
}
unsigned int VSDX11Renderer::SetRasterizerState(VSRasterizerState * pRasterizerState, bool bForceSet)
{
	unsigned int FRI = VSRenderer::SetRasterizerState(pRasterizerState, bForceSet);
	if (FRI == FRI_SAMERESOURCE)
		return 1;
	else if (FRI == FRI_FAIL)
	{
		RSSetState(NULL);
		return 0;
	}
	if (!pRasterizerState)
	{
		pRasterizerState = (VSRasterizerState *)VSRasterizerState::GetDefault();
	}
	VSRasterStateID *pRasterStateID = (VSRasterStateID *)pRasterizerState->GetIdentifier();
	if (!pRasterStateID)
		return 0;

	m_pRasterizerState = pRasterizerState;
	RSSetState(pRasterStateID);
	return true;
}
void VSDX11Renderer::ClearBuffers(unsigned int uiClearFlag, int iXPos, int iYPos, int iWidth,
	int iHeight)
{
	VSCustomMaterial * pMaterial = NULL;
	if ((uiClearFlag & CF_USE_ALL))
	{
		VSDepthStencilDesc DepthStencilDesc;
		DepthStencilDesc.m_uiDepthCompareMethod = VSDepthStencilDesc::CM_ALWAYS;
		DepthStencilDesc.m_bStencilEnable = true;
		DepthStencilDesc.m_uiStencilCompareMethod = VSDepthStencilDesc::CM_ALWAYS;
		DepthStencilDesc.m_uiSPassZPassOP = VSDepthStencilDesc::OT_REPLACE;
		DepthStencilDesc.m_uiSPassZFailOP = VSDepthStencilDesc::OT_REPLACE;
		DepthStencilDesc.m_uiSFailZPassOP = VSDepthStencilDesc::OT_REPLACE;
		DepthStencilDesc.m_uiReference = m_uiClearStencil;
		VSDepthStencilState * pDepthStencilState = VSResourceManager::CreateDepthStencilState(DepthStencilDesc);
		pMaterial = VSCustomMaterial::GetClearAllRange();
		pMaterial->SetDepthStencilState(pDepthStencilState);
	}
	else if ((uiClearFlag & CF_COLOR_DEPTH))
	{
		pMaterial = VSCustomMaterial::GetClearColorDepthRange();
	}
	else if ((uiClearFlag & CF_COLOR_STENCIL))
	{
		VSDepthStencilDesc DepthStencilDesc;
		DepthStencilDesc.m_bDepthEnable = false;
		DepthStencilDesc.m_bStencilEnable = true;
		DepthStencilDesc.m_uiStencilCompareMethod = VSDepthStencilDesc::CM_ALWAYS;
		DepthStencilDesc.m_uiSPassZPassOP = VSDepthStencilDesc::OT_REPLACE;
		DepthStencilDesc.m_uiSPassZFailOP = VSDepthStencilDesc::OT_REPLACE;
		DepthStencilDesc.m_uiSFailZPassOP = VSDepthStencilDesc::OT_REPLACE;
		VSDepthStencilState * pDepthStencilState = VSResourceManager::CreateDepthStencilState(DepthStencilDesc);
		pMaterial = VSCustomMaterial::GetClearAllRange();
		pMaterial->SetDepthStencilState(pDepthStencilState);
	}
	else if ((uiClearFlag & CF_DEPTH_STENCIL))
	{
		VSDepthStencilDesc DepthStencilDesc;
		DepthStencilDesc.m_uiDepthCompareMethod = VSDepthStencilDesc::CM_ALWAYS;
		DepthStencilDesc.m_bStencilEnable = true;
		DepthStencilDesc.m_uiStencilCompareMethod = VSDepthStencilDesc::CM_ALWAYS;
		DepthStencilDesc.m_uiSPassZPassOP = VSDepthStencilDesc::OT_REPLACE;
		DepthStencilDesc.m_uiSPassZFailOP = VSDepthStencilDesc::OT_REPLACE;
		DepthStencilDesc.m_uiSFailZPassOP = VSDepthStencilDesc::OT_REPLACE;
		DepthStencilDesc.m_uiReference = m_uiClearStencil;
		VSDepthStencilState * pDepthStencilState = VSResourceManager::CreateDepthStencilState(DepthStencilDesc);
		pMaterial = VSCustomMaterial::GetClearDepthStencilRange();
		pMaterial->SetDepthStencilState(pDepthStencilState);
	}
	else if ((uiClearFlag & CF_COLOR))
	{
		pMaterial = VSCustomMaterial::GetClearColorRange();
	}
	else if ((uiClearFlag & CF_DEPTH))
	{
		pMaterial = VSCustomMaterial::GetClearDepthRange();
	}
	else if ((uiClearFlag & CF_STENCIL))
	{
		VSDepthStencilDesc DepthStencilDesc;
		DepthStencilDesc.m_bDepthEnable = false;
		DepthStencilDesc.m_bStencilEnable = true;
		DepthStencilDesc.m_uiStencilCompareMethod = VSDepthStencilDesc::CM_ALWAYS;
		DepthStencilDesc.m_uiSPassZPassOP = VSDepthStencilDesc::OT_REPLACE;
		DepthStencilDesc.m_uiSPassZFailOP = VSDepthStencilDesc::OT_REPLACE;
		DepthStencilDesc.m_uiSFailZPassOP = VSDepthStencilDesc::OT_REPLACE;
		DepthStencilDesc.m_uiReference = m_uiClearStencil;
		VSDepthStencilState * pDepthStencilState = VSResourceManager::CreateDepthStencilState(DepthStencilDesc);
		pMaterial = VSCustomMaterial::GetClearDepthStencilRange();
		pMaterial->SetDepthStencilState(pDepthStencilState);
	}
	if (pMaterial)
	{
		VSVShader * pVShader = pMaterial->GetCurVShader(*VSShaderKey::ms_NULLShaderKey);
		VSPShader * pPShader = pMaterial->GetCurPShader(*VSShaderKey::ms_NULLShaderKey);

		VSREAL ScaleX = (iWidth * 1.0f) / m_uiCurRTWidth;
		VSREAL ScaleY = (iHeight * 1.0f) / m_uiCurRTHeight;

		VSREAL StartX = (iXPos * 1.0f) / m_uiCurRTWidth;
		VSREAL StartY = (iYPos * 1.0f) / m_uiCurRTHeight;

		VSVector3W PositionInfoData(StartX, StartY, ScaleX, ScaleY);

		pVShader->SetParam(VSUsedName::ms_cPositionInfo, &PositionInfoData);
		pPShader->SetParam(VSUsedName::ms_cClearColor, &m_ClearColor);
		pPShader->SetParam(VSUsedName::ms_cDepth, &m_fClearDepth);
		pMaterial->UseMaterial();
		DrawScreen(NULL);
	}
	
}
void VSDX11Renderer::ClearBackBuffer(int iXPos, int iYPos, int iWidth,
	int iHeight)
{	
	ClearBuffers(CF_COLOR, iXPos, iYPos, iWidth, iHeight);
}
void VSDX11Renderer::ClearZBuffer(int iXPos, int iYPos, int iWidth,
	int iHeight)
{
	ClearBuffers(CF_DEPTH, iXPos, iYPos, iWidth, iHeight);
	
}
void VSDX11Renderer::ClearStencilBuffer(int iXPos, int iYPos, int iWidth,
	int iHeight)
{
	ClearBuffers(CF_STENCIL, iXPos, iYPos, iWidth, iHeight);
}
void VSDX11Renderer::ClearBuffers(int iXPos, int iYPos, int iWidth,
	int iHeight)
{
	ClearBuffers(CF_COLOR | CF_DEPTH | CF_STENCIL,iXPos,iYPos,iWidth,iHeight);
}
bool VSDX11Renderer::SetViewPort(VSViewPort *pViewPort)
{
	VSRenderer::SetViewPort(pViewPort);
	D3D11_VIEWPORT View;
	
	if (pViewPort)
	{
		VSViewPort ViewPort;
		ViewPort = *pViewPort;
		View.TopLeftX = ViewPort.XMin * m_uiCurRTWidth;
		View.TopLeftY = ViewPort.YMin * m_uiCurRTHeight;
		View.Width = ViewPort.XMax * m_uiCurRTWidth - View.TopLeftX;
		View.Height = ViewPort.YMax * m_uiCurRTHeight - View.TopLeftY;
		View.MinDepth = ViewPort.ZMin;
		View.MaxDepth = ViewPort.ZMax;
	}
	else
	{
		View.TopLeftX = 0;
		View.TopLeftY = 0;
		View.Width = (VSREAL)m_uiCurRTWidth;
		View.Height = (VSREAL)m_uiCurRTHeight;
		View.MinDepth = 0.0f;
		View.MaxDepth = 1.0f;

	}

	RSSetViewports(View);
	return true;
}
bool VSDX11Renderer::UseWindow(int uiWindowID)
{ 
	if (!VSRenderer::UseWindow(uiWindowID))
		return 0;
	UseWindowEx(uiWindowID);
	return true;
}

bool VSDX11Renderer::CooperativeLevel()
{ 
	return true; 
}
bool VSDX11Renderer::BeginRendering()
{ 
	VSRenderer::BeginRendering();
	return true;
}
bool VSDX11Renderer::EndRendering()
{ 
	VSRenderer::EndRendering(); 
	if (m_hMainWindow != NULL)
	{
		EndRender();
	}
	return true;
}
unsigned int VSDX11Renderer::SetPTexture(VSTexture* pTexture, unsigned int i)
{ 
	unsigned int FRI = VSRenderer::SetPTexture(pTexture, i);
	if (FRI == FRI_SAMERESOURCE)
		return 1;
	else if (FRI == FRI_FAIL)
	{
		PSSetShaderResources(i, NULL);
		return 0;
	}
	VSTextureID *pTextureID = (VSTextureID *)pTexture->GetIdentifier();
	if (!pTextureID)
		return 0;

	PSSetShaderResources(i, pTextureID);

	return 1;
}
ID3DBlob* VSDX11Renderer::CreateVertexFormatShader(const VSArray<VSVertexFormat::VERTEXFORMAT_TYPE>& Layouts)
{
	unsigned int SemanticNum = sizeof(ms_dwVertexFormatSemantics) / sizeof(TCHAR *);
	VSString ShaderString = _T("struct VS_INPUT{");
	for (unsigned int i = 0; i < Layouts.GetNum(); i++)
	{
		VSString DeclareString = ms_dwVertexFormatClareType[Layouts[i].DataType];
		VSString NameString = ms_dwVertexFormatSemantics[Layouts[i].Semantics];
		VSString IndexString = IntToString(Layouts[i].SemanticsIndex);
		ShaderString += DeclareString + NameString + IndexString + _T(" : ") + NameString + IndexString + _T(";");
	}
	ShaderString += _T("};float4 VS(VS_INPUT input) : SV_POSITION{return float4(0.0f,0.0f,0.0f,0.0f);}");

	HRESULT hResult = NULL;
	ID3DBlob* pCode = NULL;
	ID3DBlob* pErrors = NULL;
	DWORD Flags = D3DCOMPILE_WARNINGS_ARE_ERRORS | D3DCOMPILE_ENABLE_STRICTNESS;
#if _DEBUG 
	Flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	Flags = D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

	hResult = D3DCompile((LPCSTR)ShaderString.GetBuffer(), ShaderString.GetLength(),
		NULL, NULL, ms_pDx11IncludeShader, _T("VS"),
		ms_cVertexShaderProgramVersion, Flags, 0, &pCode, &pErrors);
#if _DEBUG 
	if (pErrors && pErrors->GetBufferPointer())
	{
		VSLog Error;
		VSString VSErrorString = VSConfig::ms_OutputLogPath + _T("VSError.txt");
		Error.Open(VSErrorString.GetBuffer());
		Error.WriteInfo("\n");
		Error.WriteInfo((const TCHAR *)pErrors->GetBufferPointer());
		Error.WriteInfo("\n");
		VSOutputDebugString((const TCHAR *)pErrors->GetBufferPointer());
	}
#endif
	VSMAC_ASSERT(!FAILED(hResult));
	VSMAC_ASSERT(pCode);
	VSMAC_RELEASE(pErrors);
	return pCode;

}
bool VSDX11Renderer::OnLoadVBufferFormat(VSVertexFormat * pVertexFormat, VSResourceIdentifier *&pID)
{ 
	if (!pVertexFormat->m_FormatArray.GetNum())
		return 0;
	VSVBufferFormatID * pVBufferFormatID = NULL;
	pVBufferFormatID = VS_NEW VSVBufferFormatID;
	if (!pVBufferFormatID)
		return 0;
	pID = pVBufferFormatID;

	
	CreateInputLayout(pVertexFormat->m_FormatArray, pVBufferFormatID);
	return true; 
}
void VSDX11Renderer::DeviceLost()
{ 
	m_pDeviceContext->OMSetRenderTargets(0, NULL, NULL);
	VSMAC_RELEASE(m_pMainRenderTargetView);
	VSMAC_RELEASE(m_pMainDepthStencilView);
	VSMAC_RELEASE(m_pMainDepthStencil);
	m_pDeviceContext->ClearState();
	m_pDeviceContext->Flush();
}
void VSDX11Renderer::ResetDevice()
{ 
	if (m_hMainWindow != NULL)
	{
		HRESULT hResult = NULL;
		hResult = m_pMainChain->ResizeBuffers(2, m_uiScreenWidth, m_uiScreenHeight, m_SwapChainDesc.BufferDesc.Format, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
		VSMAC_ASSERT(!FAILED(hResult));
		DXGI_MODE_DESC BufferDesc = m_SwapChainDesc.BufferDesc;
		hResult = m_pMainChain->ResizeTarget(&BufferDesc);
		VSMAC_ASSERT(!FAILED(hResult));
		hResult = m_pMainChain->SetFullscreenState(!m_bWindowed, NULL);
		VSMAC_ASSERT(!FAILED(hResult));
		ID3D11Texture2D* pBackBuffer = NULL;
		hResult = m_pMainChain->GetBuffer(0, IID_ID3D11Texture2D, (LPVOID*)&pBackBuffer);
		VSMAC_ASSERT(!FAILED(hResult));
		hResult = m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pMainRenderTargetView);
		VSMAC_ASSERT(!FAILED(hResult));
		VSMAC_RELEASE(pBackBuffer);

		D3D11_TEXTURE2D_DESC descDepth;
		ZeroMemory(&descDepth, sizeof(descDepth));
		descDepth.Width = m_uiScreenWidth;
		descDepth.Height = m_uiScreenHeight;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDepth.SampleDesc.Count = ms_dwMultiSampleTypes[m_uiCurMultisample];
		descDepth.SampleDesc.Quality = m_dwMultisampleQuality - 1;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		hResult = m_pDevice->CreateTexture2D(&descDepth, NULL, &m_pMainDepthStencil);
		VSMAC_ASSERT(!FAILED(hResult));


		// Create the depth stencil view
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = descDepth.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		hResult = m_pDevice->CreateDepthStencilView(m_pMainDepthStencil, &descDSV, &m_pMainDepthStencilView);
		VSMAC_ASSERT(!FAILED(hResult));
		m_pDeviceContext->OMSetRenderTargets(1, &m_pMainRenderTargetView, m_pMainDepthStencilView);
		SetViewPort();

		m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pVShaderBuffer);
		m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pPShaderBuffer);
		m_pDeviceContext->HSSetConstantBuffers(0, 1, &m_pHShaderBuffer);
		m_pDeviceContext->DSSetConstantBuffers(0, 1, &m_pDShaderBuffer);
		m_pDeviceContext->GSSetConstantBuffers(0, 1, &m_pGShaderBuffer);
		m_pDeviceContext->CSSetConstantBuffers(0, 1, &m_pCShaderBuffer);
	}
	

	RenderStateToDefault();
}
bool VSDX11Renderer::ChangeScreenSize(unsigned int uiWidth, unsigned int uiHeight, bool bWindow)
{
	if (!VSRenderer::ChangeScreenSize(uiWidth, uiHeight, bWindow))
	{
		return false;
	}
	m_SwapChainDesc.Windowed = bWindow;
	m_SwapChainDesc.BufferDesc.Height = m_uiScreenHeight;
	m_SwapChainDesc.BufferDesc.Width = m_uiScreenWidth;
	DeviceLost();
	ResetDevice();
	return true;
}
bool VSDX11Renderer::DrawMesh(VSGeometry * pGeometry, VSRenderState * pRenderState, VSVShader * pVShader, VSPShader * pPShader, 
	VSGShader * pGShader, VSHShader * pHShader, VSDShader * pDShader)
{
	ADD_TIME_PROFILE(DrawMesh);
	if (!VSRenderer::DrawMesh(pGeometry, pRenderState, pVShader, pPShader, pGShader, pHShader, pDShader))
		return 0;
	VSDynamicBufferGeometry * pDBGeometry = DynamicCast<VSDynamicBufferGeometry>(pGeometry);
	if (pDBGeometry)
	{
		DrawDynamicBufferMesh(pDBGeometry);
	}
	else
	{
		pGeometry->UpdateGeometryBeforeDrawCall();
		DrawMesh(pGeometry);
	}
	return true;
}
bool VSDX11Renderer::DrawScreen(SCREEN_QUAD_TYPE ScreenQuad[4])
{
	VSRenderer::DrawScreen(ScreenQuad);
	if (!ScreenQuad)
	{
		ScreenQuad = ms_FullScreen;
	}
	return DrawScreen(ScreenQuad, 4, ms_FullScreenI, 6);
}
bool VSDX11Renderer::DrawScreen(SCREEN_QUAD_TYPE * pScreenBuffer, unsigned int uiVertexNum,VSUSHORT_INDEX * pIndexBuffer, unsigned int uiIndexNum)
{
	if (!VSRenderer::DrawScreen(pScreenBuffer, uiVertexNum, pIndexBuffer, uiIndexNum))
	{
		return false;
	}
	DrawDynamicScreenData(pScreenBuffer, uiVertexNum, sizeof(SCREEN_QUAD_TYPE), pIndexBuffer, uiIndexNum);
	return true;
}
bool VSDX11Renderer::DrawScreenFont(SCREEN_FONT_TYPE * pScreenFontBuffer, unsigned int uiVertexNum,
	VSUSHORT_INDEX * pIndexBuffer, unsigned int uiIndexNum)
{
	if (!VSRenderer::DrawScreenFont(pScreenFontBuffer, uiVertexNum, pIndexBuffer, uiIndexNum))
	{
		return false;
	}
	DrawDynamicScreenData(pScreenFontBuffer, uiVertexNum, sizeof(SCREEN_FONT_TYPE), pIndexBuffer, uiIndexNum);
	return true;
}
unsigned int VSDX11Renderer::SetBlendState(VSBlendState * pBlendState, bool bForceSet)
{ 
	unsigned int FRI = VSRenderer::SetBlendState(pBlendState, bForceSet);
	if (FRI == FRI_SAMERESOURCE)
		return FRI_SUCCESS;
	VSMAC_ASSERT(FRI == FRI_SUCCESS);
	if (FRI == FRI_FAIL)
	{
		OMSetBlendState(NULL);
		return FRI_FAIL;
	}
	if (!pBlendState)
	{
		pBlendState = (VSBlendState *)VSBlendState::GetDefault();
	}
	

	m_pBlendState = pBlendState;

	VSBlendStateID* pBlendStateID = (VSBlendStateID *)m_pBlendState->GetIdentifier();
	if (!pBlendStateID)
		return 0;
	OMSetBlendState(pBlendStateID);
	
	return 1;
}
void VSDX11Renderer::SetClipPlane(VSArray<VSPlane3> &Plane, bool bEnable){ VSMAC_ASSERT(0); return; }
void VSDX11Renderer::SetScissorRect(VSArray<VSRect2> &Rect, bool bEnable)
{ 
	
	if (bEnable && Rect.GetNum())
	{
		D3D11_RECT Temp[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
		unsigned int i = 0;
		for (; i < Rect.GetNum(); i++)
		{
			if (i >= D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)
			{
				break;
			}

			Temp[i].left = (LONG)Rect[i].m_Min.x;
			Temp[i].top = (LONG)Rect[i].m_Min.y;
			Temp[i].right = (LONG)Rect[i].m_Max.x;
			Temp[i].bottom = (LONG)Rect[i].m_Max.y;			
		}
		RSSetScissorRects(i, Temp);
	}
	else
	{
		RSSetScissorRects(0,NULL);
	}
}
void VSDX11Renderer::SetVShaderConstant(VSVShader * pShader)
{
	
	if (!pShader->m_bCreatePara)
	{
		return;
	}
	if (!pShader->m_uiConstBufferSize)
	{
		return;
	}
	for (unsigned int i = 0; i < pShader->m_pUserConstant.GetNum(); i++)
	{
		VSUserConstant * pUserConstant = pShader->m_pUserConstant[i];
		unsigned int uiSize = pUserConstant->GetSize();
		
		unsigned int uiOffset = pUserConstant->GetRegisterIndex();
		VSMemcpy(&m_uiShaderBuffer[uiOffset], pUserConstant->GetData(), uiSize);
	}
	VSVProgramID * pVShaderID = (VSVProgramID *)pShader->GetIdentifier();
	if (!pVShaderID)
	{
		return;
	}
	UpdateSubresource((ID3D11Resource **)&m_pVShaderBuffer, m_uiShaderBuffer, pShader->m_uiConstBufferSize);
}
void VSDX11Renderer::SetPShaderConstant(VSPShader * pShader)
{

	if (!pShader->m_bCreatePara)
	{
		return;
	}
	if (!pShader->m_uiConstBufferSize)
	{
		return;
	}
	for (unsigned int i = 0; i < pShader->m_pUserConstant.GetNum(); i++)
	{
		VSUserConstant * pUserConstant = pShader->m_pUserConstant[i];
		unsigned int uiSize = pUserConstant->GetSize();	
		unsigned int uiOffset = pUserConstant->GetRegisterIndex();
		VSMemcpy(&m_uiShaderBuffer[uiOffset], pUserConstant->GetData(), uiSize);
	}
	VSPProgramID * pPShaderID = (VSPProgramID *)pShader->GetIdentifier();
	if (!pPShaderID)
	{
		return;
	}
	UpdateSubresource((ID3D11Resource **)&m_pPShaderBuffer, m_uiShaderBuffer, pShader->m_uiConstBufferSize);
}
void VSDX11Renderer::SetGShaderConstant(VSGShader * pShader)
{
	if (!pShader->m_bCreatePara)
	{
		return;
	}
	if (!pShader->m_uiConstBufferSize)
	{
		return;
	}
	for (unsigned int i = 0; i < pShader->m_pUserConstant.GetNum(); i++)
	{
		VSUserConstant * pUserConstant = pShader->m_pUserConstant[i];
		unsigned int uiSize = pUserConstant->GetSize();
		unsigned int uiOffset = pUserConstant->GetRegisterIndex();
		VSMemcpy(&m_uiShaderBuffer[uiOffset], pUserConstant->GetData(), uiSize);
	}
	VSGProgramID * pGShaderID = (VSGProgramID *)pShader->GetIdentifier();
	if (!pGShaderID)
	{
		return;
	}
	UpdateSubresource((ID3D11Resource **)&m_pGShaderBuffer, m_uiShaderBuffer, pShader->m_uiConstBufferSize);
}
void VSDX11Renderer::SetHShaderConstant(VSHShader * pShader)
{
	if (!pShader->m_bCreatePara)
	{
		return;
	}
	if (!pShader->m_uiConstBufferSize)
	{
		return;
	}
	for (unsigned int i = 0; i < pShader->m_pUserConstant.GetNum(); i++)
	{
		VSUserConstant * pUserConstant = pShader->m_pUserConstant[i];
		unsigned int uiSize = pUserConstant->GetSize();
		unsigned int uiOffset = pUserConstant->GetRegisterIndex();
		VSMemcpy(&m_uiShaderBuffer[uiOffset], pUserConstant->GetData(), uiSize);
	}
	VSHProgramID * pHShaderID = (VSHProgramID *)pShader->GetIdentifier();
	if (!pHShaderID)
	{
		return;
	}
	UpdateSubresource((ID3D11Resource **)&m_pHShaderBuffer, m_uiShaderBuffer, pShader->m_uiConstBufferSize);
}
void VSDX11Renderer::SetCShaderConstant(VSCShader * pShader)
{
	if (!pShader->m_bCreatePara)
	{
		return;
	}
	if (!pShader->m_uiConstBufferSize)
	{
		return;
	}
	for (unsigned int i = 0; i < pShader->m_pUserConstant.GetNum(); i++)
	{
		VSUserConstant * pUserConstant = pShader->m_pUserConstant[i];
		unsigned int uiSize = pUserConstant->GetSize();
		unsigned int uiOffset = pUserConstant->GetRegisterIndex();
		VSMemcpy(&m_uiShaderBuffer[uiOffset], pUserConstant->GetData(), uiSize);
	}
	VSCProgramID * pCShaderID = (VSCProgramID *)pShader->GetIdentifier();
	if (!pCShaderID)
	{
		return;
	}
	UpdateSubresource((ID3D11Resource **)&m_pCShaderBuffer, m_uiShaderBuffer, pShader->m_uiConstBufferSize);
}
void VSDX11Renderer::SetDShaderConstant(VSDShader * pShader)
{
	if (!pShader->m_bCreatePara)
	{
		return;
	}
	if (!pShader->m_uiConstBufferSize)
	{
		return;
	}
	for (unsigned int i = 0; i < pShader->m_pUserConstant.GetNum(); i++)
	{
		VSUserConstant * pUserConstant = pShader->m_pUserConstant[i];
		unsigned int uiSize = pUserConstant->GetSize();
		unsigned int uiOffset = pUserConstant->GetRegisterIndex();
		VSMemcpy(&m_uiShaderBuffer[uiOffset], pUserConstant->GetData(), uiSize);
	}
	VSDProgramID * pDShaderID = (VSDProgramID *)pShader->GetIdentifier();
	if (!pDShaderID)
	{
		return;
	}
	UpdateSubresource((ID3D11Resource **)&m_pDShaderBuffer, m_uiShaderBuffer, pShader->m_uiConstBufferSize);
}

unsigned int VSDX11Renderer::SetVBuffer(VSVertexBuffer * pVBuffer, unsigned int uiStream)
{
	unsigned int FRI = VSRenderer::SetVBuffer(pVBuffer, uiStream);
	if (FRI == FRI_SAMERESOURCE)
		return 1;
	else if (FRI == FRI_FAIL)
	{
		IASetVertexBuffers(uiStream,NULL, 0);
		return 0;
	}
	VSVBufferID* pVBufferID = (VSVBufferID *)pVBuffer->GetIdentifier();
	if (!pVBufferID)
		return 0;
	IASetVertexBuffers(uiStream,pVBufferID, pVBuffer->GetOneVertexSize());

	return 1;
}

unsigned int VSDX11Renderer::SetIBuffer(VSIndexBuffer* pIBuffer)
{
	unsigned int FRI = VSRenderer::SetIBuffer(pIBuffer);
	if (FRI == FRI_SAMERESOURCE)
		return 1;
	else if (FRI == FRI_FAIL)
	{
		IASetIndexBuffer(NULL, DXGI_FORMAT_UNKNOWN);	
		return 1;
	}
	VSIBufferID* pIBufferID = (VSIBufferID *)pIBuffer->GetIdentifier();
	if (!pIBufferID)
		return 0;

	IASetIndexBuffer(pIBufferID, (DXGI_FORMAT)ms_dwVertexFormatDataType[pIBuffer->GetDataType()]);
	return 1;
}
unsigned int VSDX11Renderer::BeginQuery(VSQuery * pQuery)
{
	if (!VSRenderer::BeginQuery(pQuery))
	{
		return 0;
	}
	VSQueryID * pQueryID = (VSQueryID *)pQuery->GetIdentifier();
	if (!pQueryID)
		return 0;
	BeginQuery(pQueryID);
	return 1;
}
unsigned int VSDX11Renderer::EndQuery(VSQuery * pQuery)
{
	if (!VSRenderer::EndQuery(pQuery))
	{
		if (m_pQuery)
		{
			pQuery = m_pQuery;
			m_pQuery = NULL;
		}
		else
		{
			return 0;
		}
	}
	VSQueryID * pQueryID = (VSQueryID *)pQuery->GetIdentifier();
	if (!pQueryID)
		return 0;
	EndQuery(pQueryID);
	return 1;
}
unsigned int VSDX11Renderer::SetVTexture(VSTexture* pTexture, unsigned int i)
{ 
	unsigned int FRI = VSRenderer::SetVTexture(pTexture, i);
	if (FRI == FRI_SAMERESOURCE)
		return 1;
	else if (FRI == FRI_FAIL)
	{
		VSSetShaderResources(i,NULL);
		return 0;
	}
	VSTextureID *pTextureID = (VSTextureID *)pTexture->GetIdentifier();
	if (!pTextureID)
		return 0;
	VSSetShaderResources(i,pTextureID);
	return true;
}
unsigned int VSDX11Renderer::SetGTexture(VSTexture* pTexture, unsigned int i)
{
	unsigned int FRI = VSRenderer::SetGTexture(pTexture, i);
	if (FRI == FRI_SAMERESOURCE)
		return 1;
	else if (FRI == FRI_FAIL)
	{
		GSSetShaderResources(i, NULL);
		return 0;
	}
	VSTextureID *pTextureID = (VSTextureID *)pTexture->GetIdentifier();
	if (!pTextureID)
		return 0;
	GSSetShaderResources(i, pTextureID);
	return true;
}
unsigned int VSDX11Renderer::SetDTexture(VSTexture* pTexture, unsigned int i)
{
	unsigned int FRI = VSRenderer::SetDTexture(pTexture, i);
	if (FRI == FRI_SAMERESOURCE)
		return 1;
	else if (FRI == FRI_FAIL)
	{
		DSSetShaderResources(i, NULL);
		return 0;
	}
	VSTextureID *pTextureID = (VSTextureID *)pTexture->GetIdentifier();
	if (!pTextureID)
		return 0;
	DSSetShaderResources(i, pTextureID);
	return true;
}
unsigned int VSDX11Renderer::SetHTexture(VSTexture* pTexture, unsigned int i)
{
	unsigned int FRI = VSRenderer::SetHTexture(pTexture, i);
	if (FRI == FRI_SAMERESOURCE)
		return 1;
	else if (FRI == FRI_FAIL)
	{
		HSSetShaderResources(i, NULL);
		return 0;
	}
	VSTextureID *pTextureID = (VSTextureID *)pTexture->GetIdentifier();
	if (!pTextureID)
		return 0;
	HSSetShaderResources(i, pTextureID);
	return true;
}
unsigned int VSDX11Renderer::SetCTexture(VSTexture* pTexture, unsigned int i)
{
	unsigned int FRI = VSRenderer::SetCTexture(pTexture, i);
	if (FRI == FRI_SAMERESOURCE)
		return 1;
	else if (FRI == FRI_FAIL)
	{
		CSSetShaderResources(i, NULL);
		return 0;
	}
	VSTextureID *pTextureID = (VSTextureID *)pTexture->GetIdentifier();
	if (!pTextureID)
		return 0;
	CSSetShaderResources(i, pTextureID);
	return true;
}
unsigned int VSDX11Renderer::SetCBufferResource(VSBufferResource* pBufferResource, unsigned int i)
{
	unsigned int FRI = VSRenderer::SetCBufferResource(pBufferResource, i);
	if (FRI == FRI_SAMERESOURCE)
		return 1;
	else if (FRI == FRI_FAIL)
	{
		CSSetBufferResources(i, NULL);
		return 0;
	}
	VSSBufferID *pBufferResourceID = (VSSBufferID *)pBufferResource->GetIdentifier();
	if (!pBufferResourceID)
		return 0;
	CSSetBufferResources(i, pBufferResourceID);
	return true;
}
unsigned int VSDX11Renderer::SetVBufferResource(VSBufferResource* pBufferResource, unsigned int i)
{
	unsigned int FRI = VSRenderer::SetVBufferResource(pBufferResource, i);
	if (FRI == FRI_SAMERESOURCE)
		return 1;
	else if (FRI == FRI_FAIL)
	{
		VSSetBufferResources(i, NULL);
		return 0;
	}
	VSSBufferID *pBufferResourceID = (VSSBufferID *)pBufferResource->GetIdentifier();
	if (!pBufferResourceID)
		return 0;
	VSSetBufferResources(i, pBufferResourceID);
	return true;
}
unsigned int VSDX11Renderer::SetPBufferResource(VSBufferResource* pBufferResource, unsigned int i)
{
	unsigned int FRI = VSRenderer::SetPBufferResource(pBufferResource, i);
	if (FRI == FRI_SAMERESOURCE)
		return 1;
	else if (FRI == FRI_FAIL)
	{
		PSSetBufferResources(i, NULL);
		return 0;
	}
	VSSBufferID *pBufferResourceID = (VSSBufferID *)pBufferResource->GetIdentifier();
	if (!pBufferResourceID)
		return 0;
	PSSetBufferResources(i, pBufferResourceID);
	return true;
}
unsigned int VSDX11Renderer::SetGBufferResource(VSBufferResource* pBufferResource, unsigned int i)
{
	unsigned int FRI = VSRenderer::SetGBufferResource(pBufferResource, i);
	if (FRI == FRI_SAMERESOURCE)
		return 1;
	else if (FRI == FRI_FAIL)
	{
		GSSetBufferResources(i, NULL);
		return 0;
	}
	VSSBufferID *pBufferResourceID = (VSSBufferID *)pBufferResource->GetIdentifier();
	if (!pBufferResourceID)
		return 0;
	GSSetBufferResources(i, pBufferResourceID);
	return true;
}
unsigned int VSDX11Renderer::SetHBufferResource(VSBufferResource* pBufferResource, unsigned int i)
{
	unsigned int FRI = VSRenderer::SetHBufferResource(pBufferResource, i);
	if (FRI == FRI_SAMERESOURCE)
		return 1;
	else if (FRI == FRI_FAIL)
	{
		HSSetBufferResources(i, NULL);
		return 0;
	}
	VSSBufferID *pBufferResourceID = (VSSBufferID *)pBufferResource->GetIdentifier();
	if (!pBufferResourceID)
		return 0;
	HSSetBufferResources(i, pBufferResourceID);
	return true;
}
unsigned int VSDX11Renderer::SetDBufferResource(VSBufferResource* pBufferResource, unsigned int i)
{
	unsigned int FRI = VSRenderer::SetDBufferResource(pBufferResource, i);
	if (FRI == FRI_SAMERESOURCE)
		return 1;
	else if (FRI == FRI_FAIL)
	{
		DSSetBufferResources(i, NULL);
		return 0;
	}
	VSSBufferID *pBufferResourceID = (VSSBufferID *)pBufferResource->GetIdentifier();
	if (!pBufferResourceID)
		return 0;
	DSSetBufferResources(i, pBufferResourceID);
	return true;
}
void VSDX11Renderer::Dispath(unsigned int uiThreadGroupCountX, unsigned int uiThreadGroupCountY,
	unsigned int uiThreadGroupCountZ)
{
	VSRenderer::Dispath(uiThreadGroupCountX, uiThreadGroupCountY, uiThreadGroupCountZ);
	OnDispath(uiThreadGroupCountX, uiThreadGroupCountY, uiThreadGroupCountZ);
}
unsigned int VSDX11Renderer::SetHSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet)
{
	unsigned int FRI = VSRenderer::SetHSamplerState(pSamplerState, i, bForceSet);
	if (FRI == FRI_SAMERESOURCE)
		return FRI_SUCCESS;
	VSMAC_ASSERT(FRI == FRI_SUCCESS);
	if (FRI == FRI_FAIL)
	{
		HSSetSamplers(i, NULL);
		return FRI_FAIL;
	}
	if (!pSamplerState)
	{
		pSamplerState = (VSSamplerState *)VSSamplerState::GetDefault();
	}


	m_pSamplerState[VSEngineFlag::ST_HULL][i] = pSamplerState;

	VSSamplerStateID* pSamplerStateID = (VSSamplerStateID *)m_pSamplerState[VSEngineFlag::ST_HULL][i]->GetIdentifier();
	if (!pSamplerStateID)
		return 0;
	HSSetSamplers(i, pSamplerStateID);
	return 1;
}
unsigned int VSDX11Renderer::SetCSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet)
{
	unsigned int FRI = VSRenderer::SetCSamplerState(pSamplerState, i, bForceSet);
	if (FRI == FRI_SAMERESOURCE)
		return FRI_SUCCESS;
	VSMAC_ASSERT(FRI == FRI_SUCCESS);
	if (FRI == FRI_FAIL)
	{
		CSSetSamplers(i, NULL);
		return FRI_FAIL;
	}
	if (!pSamplerState)
	{
		pSamplerState = (VSSamplerState *)VSSamplerState::GetDefault();
	}


	m_pSamplerState[VSEngineFlag::ST_HULL][i] = pSamplerState;

	VSSamplerStateID* pSamplerStateID = (VSSamplerStateID *)m_pSamplerState[VSEngineFlag::ST_HULL][i]->GetIdentifier();
	if (!pSamplerStateID)
		return 0;
	CSSetSamplers(i, pSamplerStateID);
	return 1;
}
unsigned int VSDX11Renderer::SetDSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet)
{
	unsigned int FRI = VSRenderer::SetDSamplerState(pSamplerState, i, bForceSet);
	if (FRI == FRI_SAMERESOURCE)
		return FRI_SUCCESS;
	VSMAC_ASSERT(FRI == FRI_SUCCESS);
	if (FRI == FRI_FAIL)
	{
		DSSetSamplers(i, NULL);
		return FRI_FAIL;
	}
	if (!pSamplerState)
	{
		pSamplerState = (VSSamplerState *)VSSamplerState::GetDefault();
	}


	m_pSamplerState[VSEngineFlag::ST_DOMAIN][i] = pSamplerState;

	VSSamplerStateID* pSamplerStateID = (VSSamplerStateID *)m_pSamplerState[VSEngineFlag::ST_DOMAIN][i]->GetIdentifier();
	if (!pSamplerStateID)
		return 0;
	DSSetSamplers(i, pSamplerStateID);
	return 1;
}
unsigned int VSDX11Renderer::SetGSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet)
{
	unsigned int FRI = VSRenderer::SetGSamplerState(pSamplerState, i, bForceSet);
	if (FRI == FRI_SAMERESOURCE)
		return FRI_SUCCESS;
	VSMAC_ASSERT(FRI == FRI_SUCCESS);
	if (FRI == FRI_FAIL)
	{
		GSSetSamplers(i, NULL);
		return FRI_FAIL;
	}
	if (!pSamplerState)
	{
		pSamplerState = (VSSamplerState *)VSSamplerState::GetDefault();
	}


	m_pSamplerState[VSEngineFlag::ST_GEOMETRY][i] = pSamplerState;

	VSSamplerStateID* pSamplerStateID = (VSSamplerStateID *)m_pSamplerState[VSEngineFlag::ST_GEOMETRY][i]->GetIdentifier();
	if (!pSamplerStateID)
		return 0;
	GSSetSamplers(i, pSamplerStateID);
	return 1;
}
unsigned int VSDX11Renderer::SetVSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet)
{
	unsigned int FRI = VSRenderer::SetVSamplerState(pSamplerState, i, bForceSet);
	if (FRI == FRI_SAMERESOURCE)
		return FRI_SUCCESS;
	VSMAC_ASSERT(FRI == FRI_SUCCESS);
	if (FRI == FRI_FAIL)
	{
		VSSetSamplers(i, NULL);
		return FRI_FAIL;
	}
	if (!pSamplerState)
	{
		pSamplerState = (VSSamplerState *)VSSamplerState::GetDefault();
	}


	m_pSamplerState[VSEngineFlag::ST_VERTEX][i] = pSamplerState;

	VSSamplerStateID* pSamplerStateID = (VSSamplerStateID *)m_pSamplerState[VSEngineFlag::ST_VERTEX][i]->GetIdentifier();
	if (!pSamplerStateID)
		return 0;
	VSSetSamplers(i, pSamplerStateID);
	return 1;
}
unsigned int VSDX11Renderer::SetPSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet)
{
	unsigned int FRI = VSRenderer::SetPSamplerState(pSamplerState, i, bForceSet);
	if (FRI == FRI_SAMERESOURCE)
		return FRI_SUCCESS;
	VSMAC_ASSERT(FRI == FRI_SUCCESS);
	if (FRI == FRI_FAIL)
	{
		PSSetSamplers(i, NULL);
		return FRI_FAIL;
	}
	if (!pSamplerState)
	{
		pSamplerState = (VSSamplerState *)VSSamplerState::GetDefault();
	}


	m_pSamplerState[VSEngineFlag::ST_PIXEL][i] = pSamplerState;

	VSSamplerStateID* pSamplerStateID = (VSSamplerStateID *)m_pSamplerState[VSEngineFlag::ST_PIXEL][i]->GetIdentifier();
	if (!pSamplerStateID)
		return 0;
	PSSetSamplers(i, pSamplerStateID);
	return 1;
}
bool VSDX11Renderer::SetCSUnorderAccesses(VSUnorderAccess * pUnorderAccesses[], unsigned int uiNum)
{
	if (!VSRenderer::SetCSUnorderAccesses(pUnorderAccesses, uiNum))
	{
		VSMAC_ASSERT(0);
		return false;
	}
	VSUnorderAccessID * UAVTemp[UNORDERACCESSS_LEVEL] = { NULL };
	unsigned int uiUAVNum = 0;
	for (unsigned int i = 0; i < UNORDERACCESSS_LEVEL; i++)
	{
		if (pUnorderAccesses[i])
		{
			UAVTemp[uiUAVNum] = (VSUnorderAccessID *)pUnorderAccesses[i]->GetIdentifier();
			uiUAVNum++;

		}
		else
		{
			break;
		}
	}
	SetCSUnorderAccessesEx(UAVTemp, uiUAVNum);

	return true;
}
bool VSDX11Renderer::EndCSUnorderAccesses(VSUnorderAccess * pUnorderAccesses[], unsigned int uiNum)
{
	if (!VSRenderer::EndCSUnorderAccesses(pUnorderAccesses, uiNum))
	{
		VSMAC_ASSERT(0);
		return false;
	}
	VSUnorderAccess *pUnorderAccessTemp[UNORDERACCESSS_LEVEL] = { NULL };
	VSUnorderAccessID * UAVTemp[UNORDERACCESSS_LEVEL] = { NULL };
	unsigned int uiUAVNum = 0;
	for (unsigned int i = 0; i < UNORDERACCESSS_LEVEL; i++)
	{
		if (pUnorderAccesses[i])
		{
			UAVTemp[uiUAVNum] = (VSUnorderAccessID *)pUnorderAccesses[i]->GetIdentifier();
			pUnorderAccessTemp[uiUAVNum] = pUnorderAccesses[i];
			uiUAVNum++;

		}
		else
		{
			break;
		}
	}
	EndCSUnorderAccessesEx(pUnorderAccessTemp, UAVTemp, uiUAVNum);


	return true;
}
bool VSDX11Renderer::SetRenderTargets(VSRenderTarget *pRenderTarget[], unsigned int uiNum)
{ 
	if (!VSRenderer::SetRenderTargets(pRenderTarget,uiNum))
	{
		VSMAC_ASSERT(0);
		return false;
	}	
	VSRenderTargetID * RTVTemp[RENDERTARGET_LEVEL] = { NULL };
	unsigned int uiTargetNum = 0;
	for (unsigned int i = 0; i < RENDERTARGET_LEVEL; i++)
	{
		if (pRenderTarget[i])
		{
			RTVTemp[uiTargetNum] = (VSRenderTargetID *)pRenderTarget[i]->GetIdentifier();
			uiTargetNum++;
			
		}
		else
		{
			break;
		}
	}
	SetRenderTargetsEx(RTVTemp, uiTargetNum, m_uiBackMacthType);

	return true;
}
bool VSDX11Renderer::EndRenderTargets(VSRenderTarget *pRenderTarget[], unsigned int uiNum)
{
	
	if (!VSRenderer::EndRenderTargets(pRenderTarget, uiNum))
	{
		VSMAC_ASSERT(0);
		return false;
	}
	VSRenderTarget *pRenderTargetTemp[RENDERTARGET_LEVEL] = { NULL };
	VSRenderTargetID * RTVTemp[RENDERTARGET_LEVEL] = { NULL };
	unsigned int uiTargetNum = 0;
	for (unsigned int i = 0; i < RENDERTARGET_LEVEL; i++)
	{
		if (pRenderTarget[i])
		{
			RTVTemp[uiTargetNum] = (VSRenderTargetID *)pRenderTarget[i]->GetIdentifier();
			pRenderTargetTemp[uiTargetNum] = pRenderTarget[i];
			uiTargetNum++;
			
		}
		else
		{
			break;
		}
	}
	EndRenderTargetsEx(pRenderTargetTemp,RTVTemp, uiTargetNum);


	return true;
}

bool VSDX11Renderer::SetDepthStencilBuffer(VSDepthStencil * pDepthStencilBuffer)
{ 
	

	if (!VSRenderer::SetDepthStencilBuffer(pDepthStencilBuffer))
	{
		VSMAC_ASSERT(0);
		return 0;
	}	
	VSDepthStencilID *pDepthStencilID = (VSDepthStencilID *)pDepthStencilBuffer->GetIdentifier();
	if (!pDepthStencilID)
	{
		return 0;
	}
	SetDepthStencilBufferEx(pDepthStencilID);
	return true;
}
bool VSDX11Renderer::EndDepthStencilBuffer(VSDepthStencil * pDepthStencilBuffer)
{ 
	
	if (!VSRenderer::EndDepthStencilBuffer(pDepthStencilBuffer))
	{	
		VSMAC_ASSERT(0);
		return 0;
	}
	VSDepthStencilID *pDepthStencilID = (VSDepthStencilID *)pDepthStencilBuffer->GetIdentifier();
	if (!pDepthStencilID)
		return 0;
	EndDepthStencilBufferEx(pDepthStencilBuffer,pDepthStencilID);
	return 1;
}


bool VSDX11Renderer::SetVShaderProgram(VSVShader*	pVShaderProgram)
{ 
	if (!VSRenderer::SetVShaderProgram(pVShaderProgram))
	{
		VSSetShader(NULL);
		return 0;
	}

	VSVProgramID *pVProgramID = (VSVProgramID *)pVShaderProgram->GetIdentifier();
	if (!pVProgramID)
		return FRI_FAIL;
	VSSetShader(pVProgramID);
	return 1;
}
bool VSDX11Renderer::SetGShaderProgram(VSGShader*pGShaderProgram)
{
	if (!VSRenderer::SetGShaderProgram(pGShaderProgram))
	{
		GSSetShader(NULL);
		return 0;
	}
	VSGProgramID *pGProgramID = (VSGProgramID *)pGShaderProgram->GetIdentifier();
	if (!pGProgramID)
		return FRI_FAIL;
	GSSetShader(pGProgramID);
	return 1;
}
bool VSDX11Renderer::SetDShaderProgram(VSDShader*pDShaderProgram)
{
	if (!VSRenderer::SetDShaderProgram(pDShaderProgram))
	{
		DSSetShader(NULL);
		return 0;
	}
	VSDProgramID *pDProgramID = (VSDProgramID *)pDShaderProgram->GetIdentifier();
	if (!pDProgramID)
		return FRI_FAIL;
	DSSetShader(pDProgramID);
	return 1;
}
bool VSDX11Renderer::SetCShaderProgram(VSCShader*	pCShaderProgram)
{
	if (!VSRenderer::SetCShaderProgram(pCShaderProgram))
	{
		CSSetShader(NULL);
		return 0;
	}
	VSCProgramID *pCProgramID = (VSCProgramID *)pCShaderProgram->GetIdentifier();
	if (!pCProgramID)
		return FRI_FAIL;
	CSSetShader(pCProgramID);
	return 1;
}
bool VSDX11Renderer::SetHShaderProgram(VSHShader*pHShaderProgram)
{
	if (!VSRenderer::SetHShaderProgram(pHShaderProgram))
	{
		HSSetShader(NULL);
		return 0;
	}
	VSHProgramID *pHProgramID = (VSHProgramID *)pHShaderProgram->GetIdentifier();
	if (!pHProgramID)
		return FRI_FAIL;
	HSSetShader(pHProgramID);
	return 1;
}
bool VSDX11Renderer::SetPShaderProgram(VSPShader*pPShaderProgram)
{
	if (!VSRenderer::SetPShaderProgram(pPShaderProgram))
	{
		PSSetShader(NULL);
		return 0;
	}
	VSPProgramID *pPProgramID = (VSPProgramID *)pPShaderProgram->GetIdentifier();
	if (!pPProgramID)
		return FRI_FAIL;
	PSSetShader(pPProgramID);
	return 1;
}
D3D_SHADER_MACRO * VSDX11Renderer::GetDefine(VSMap<VSString, VSString> & Define)
{
	if (Define.GetNum())
	{
		D3D_SHADER_MACRO * pMacro = VS_NEW D3D_SHADER_MACRO[Define.GetNum() + 1];
		for (unsigned int i = 0; i < Define.GetNum(); i++)
		{
			pMacro[i].Name = Define[i].Key.GetBuffer();
			pMacro[i].Definition = Define[i].Value.GetBuffer();
		}
		pMacro[Define.GetNum()].Name = NULL;
		pMacro[Define.GetNum()].Definition = NULL;
		return pMacro;
	}
	return NULL;
}
//Load Release
bool VSDX11Renderer::OnLoadVShaderProgram(VSVShader * pVShaderProgram, VSResourceIdentifier *&pID)
{ 
	VSVProgramID *pVProgramID = NULL;
	pVProgramID = VS_NEW VSVProgramID;
	if (!pVProgramID)
		return 0;
	pID = pVProgramID;

	if (!pVShaderProgram->GetCacheBuffer())
	{
		CreateVShaderFromString(pVShaderProgram, pVProgramID);
	}
	else
	{
		CreateVShaderFromCache(pVShaderProgram,pVProgramID);
	}

	
	return true;
}
bool VSDX11Renderer::OnLoadGShaderProgram(VSGShader * pGShaderProgram, VSResourceIdentifier *&pID)
{
	VSGProgramID *pGProgramID = NULL;
	pGProgramID = VS_NEW VSGProgramID;
	if (!pGProgramID)
		return 0;
	pID = pGProgramID;
	if (!pGShaderProgram->GetCacheBuffer())
	{
		CreateGShaderFromString(pGShaderProgram, pGProgramID);
	}
	else
	{
		CreateGShaderFromCache(pGShaderProgram, pGProgramID);
	}


	return true;
}
bool VSDX11Renderer::OnLoadDShaderProgram(VSDShader * pDShaderProgram, VSResourceIdentifier *&pID)
{
	VSDProgramID *pDProgramID = NULL;
	pDProgramID = VS_NEW VSDProgramID;
	if (!pDProgramID)
		return 0;
	pID = pDProgramID;
	if (!pDShaderProgram->GetCacheBuffer())
	{
		CreateDShaderFromString(pDShaderProgram, pDProgramID);
	}
	else
	{
		CreateDShaderFromCache(pDShaderProgram, pDProgramID);
	}


	return true;
}
bool VSDX11Renderer::OnLoadHShaderProgram(VSHShader * pHShaderProgram, VSResourceIdentifier *&pID)
{
	VSHProgramID *pHProgramID = NULL;
	pHProgramID = VS_NEW VSHProgramID;
	if (!pHProgramID)
		return 0;
	pID = pHProgramID;
	if (!pHShaderProgram->GetCacheBuffer())
	{
		CreateHShaderFromString(pHShaderProgram, pHProgramID);
	}
	else
	{
		CreateHShaderFromCache(pHShaderProgram, pHProgramID);
	}


	return true;
}
bool VSDX11Renderer::OnLoadPShaderProgram(VSPShader* pPShaderProgram, VSResourceIdentifier *&pID)
{ 
	VSPProgramID *pPProgramID = NULL;
	pPProgramID = VS_NEW VSPProgramID;
	if (!pPProgramID)
		return 0;
	pID = pPProgramID;
	if (!pPShaderProgram->GetCacheBuffer())
	{
		CreatePShaderFromString(pPShaderProgram, pPProgramID);
	}
	else
	{
		CreatePShaderFromCache(pPShaderProgram, pPProgramID);
	}

	
	return true;
}
bool VSDX11Renderer::OnLoadCShaderProgram(VSCShader * pCShaderProgram, VSResourceIdentifier *&pID)
{
	VSCProgramID *pCProgramID = NULL;
	pCProgramID = VS_NEW VSCProgramID;
	if (!pCProgramID)
		return 0;
	pID = pCProgramID;
	if (!pCShaderProgram->GetCacheBuffer())
	{
		CreateCShaderFromString(pCShaderProgram, pCProgramID);
	}
	else
	{
		CreateCShaderFromCache(pCShaderProgram, pCProgramID);
	}
	return true;
}
DXGI_FORMAT GetDepthStencilTextureFormat(DXGI_FORMAT depthFormat)
{
	if (depthFormat == DXGI_FORMAT_D16_UNORM)
	{
		return DXGI_FORMAT_R16_TYPELESS;
	}

	if (depthFormat == DXGI_FORMAT_D24_UNORM_S8_UINT)
	{
		return DXGI_FORMAT_R24G8_TYPELESS;
	}

	if (depthFormat == DXGI_FORMAT_D32_FLOAT)
	{
		return DXGI_FORMAT_R32_TYPELESS;
	}

	if (depthFormat == DXGI_FORMAT_D32_FLOAT_S8X24_UINT)
	{
		return DXGI_FORMAT_R32G8X24_TYPELESS;
	}

	VSMAC_ASSERT(0);
	return DXGI_FORMAT_UNKNOWN;
}

bool VSDX11Renderer::OnLoadTexture(VSTexture * pTexture, VSResourceIdentifier *&pID)
{ 
	VSTextureID * pTextureID = NULL;
	pTextureID = VS_NEW VSTextureID;
	if (!pTextureID)
		return 0;

	
	pID = pTextureID;
	
	unsigned int Width = pTexture->GetWidth(0);
	unsigned int Height = pTexture->GetHeight(0);
	unsigned int Depth = pTexture->GetLength(0);
	unsigned int MipLevels = pTexture->GetMipLevel();
	unsigned int ArraySize = pTexture->GetArraySize();
	DXGI_FORMAT Format = pTexture->GetSRGB() ? (DXGI_FORMAT)ms_dwSRGBTextureFormatType[pTexture->GetFormatType()] : (DXGI_FORMAT)ms_dwTextureFormatType[pTexture->GetFormatType()];
	unsigned int VSMSType = VSRenderer::MS_NONE;
	unsigned int Quality = 0;
	unsigned int BindFlags = D3D11_BIND_SHADER_RESOURCE;
	unsigned int MiscFlags = 0;// D3D11_RESOURCE_MISC_GENERATE_MIPS;
	D3D11_USAGE Usage = D3D11_USAGE_DEFAULT;
	unsigned int CPUAccessFlags = 0;
	if (pTexture->GetOutputType() == VSOutputResource::OT_RENDER_TARGET)
	{
		Usage = D3D11_USAGE_DEFAULT;	
		CPUAccessFlags = 0;	
		BindFlags |= D3D11_BIND_RENDER_TARGET;

	}
	else if (pTexture->GetOutputType() == VSOutputResource::OT_DEPTH_STENCIL)
	{
		Usage = D3D11_USAGE_DEFAULT;
		CPUAccessFlags = 0;
		BindFlags |= D3D11_BIND_DEPTH_STENCIL;
		Format = GetDepthStencilTextureFormat(Format);
	}
	else if (pTexture->GetOutputType() == VSOutputResource::OT_TEXTURE_UNORDER_ACCESS)
	{
		Usage = D3D11_USAGE_DEFAULT;
		CPUAccessFlags = 0;
		BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
	}
	else
	{
		if (!pTexture->IsStatic())
		{
			Usage = D3D11_USAGE_DYNAMIC;
			CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		
		}
		else
		{
			Usage = D3D11_USAGE_IMMUTABLE;
			CPUAccessFlags = 0;		
		}
	}
	




	if (pTexture->GetTexType() == VSTexture::TT_2D)
	{

		D3D11_TEXTURE2D_DESC desc;
		desc.Width = Width;
		desc.Height = Height;
		desc.MipLevels = MipLevels;
		desc.ArraySize = ArraySize;
		desc.Format = Format;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BindFlags = BindFlags;
		desc.MiscFlags = MiscFlags;
		desc.CPUAccessFlags = CPUAccessFlags;
		desc.Usage = Usage;

		CreateTexture2D((VS2DTexture*)pTexture, desc,pTextureID);		
	}
	else if (pTexture->GetTexType() == VSTexture::TT_3D)
	{
		D3D11_TEXTURE3D_DESC desc;
		desc.Width = Width;
		desc.Height = Height;
		desc.Depth = Depth;
		desc.MipLevels = MipLevels;
		desc.Format = Format;
		desc.BindFlags = BindFlags;
		desc.MiscFlags = MiscFlags;
		desc.Usage = Usage;
		desc.CPUAccessFlags = CPUAccessFlags;

		CreateTexture3D((VS3DTexture *)pTexture, desc, pTextureID);
	}
	else if (pTexture->GetTexType() == VSTexture::TT_CUBE || pTexture->GetTexType() == VSTexture::TT_2DARRAY)
	{
		D3D11_TEXTURE2D_DESC desc;
		desc.Width = Width;
		desc.Height = Height;
		desc.MipLevels = MipLevels;
		desc.ArraySize = ArraySize;
		desc.Format = Format;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BindFlags = BindFlags;
		if (pTexture->GetTexType() == VSTexture::TT_CUBE)
		{
			desc.MiscFlags = MiscFlags | D3D11_RESOURCE_MISC_TEXTURECUBE;
		}
		else
		{
			desc.MiscFlags = MiscFlags ;
		}
		
		desc.CPUAccessFlags = CPUAccessFlags;
		desc.Usage = Usage;

		CreateTexture2DArray((VS2DTextureArray *)pTexture, desc,pTextureID);
	}
	else if (pTexture->GetTexType() == VSTexture::TT_1D)
	{
		D3D11_TEXTURE1D_DESC desc;
		desc.Width = Width;
		desc.ArraySize = ArraySize;
		desc.MipLevels = MipLevels;
		desc.Format = Format;
		desc.BindFlags = BindFlags;
		desc.MiscFlags = MiscFlags;
		desc.Usage = Usage;
		desc.CPUAccessFlags = CPUAccessFlags;

		CreateTexture1D((VS1DTexture *)pTexture, desc, pTextureID);

	}
	else
	{
		VSMAC_ASSERT(0);
	}

	return 1;
}
bool VSDX11Renderer::OnLoadDepthStencil(VSDepthStencil * pDepthStencil, VSResourceIdentifier *&pID)
{ 
	VSDepthStencilID * pDepthStencilID = NULL;
	pDepthStencilID = VS_NEW VSDepthStencilID;
	if (!pDepthStencilID)
		return 0;	
	pID = pDepthStencilID;
	unsigned int VSMSType = GetSuitableMultisample(pDepthStencil->GetMulSample());
	VSTexture * pTexture = (VSTexture *)pDepthStencil->GetCreateBy();
	VSTextureID * pTextureID = NULL;
	unsigned int TextureType = 0;
	if (pTexture)
	{
		pTextureID = (VSTextureID *)pTexture->GetIdentifier();
		TextureType = pTexture->GetTexType();
	}
	OnLoadDepthStencilEx(pDepthStencil->GetWidth(), pDepthStencil->GetHeight(), pDepthStencil->GetFormatType(), VSMSType,
		pDepthStencil->GetLockFlag(), TextureType, pDepthStencil->GetLevel(), pDepthStencil->GetFirst(), pDepthStencil->GetArraySize(),
		pTextureID, pDepthStencilID);
	return true;
}
bool VSDX11Renderer::OnLoadRenderTarget(VSRenderTarget * pRenderTarget, VSResourceIdentifier *&pID)
{ 
	if (!pRenderTarget)
	{
		return false;
	}
	
	VSRenderTargetID *pRenderTargetID = VS_NEW VSRenderTargetID;
	pID = pRenderTargetID;
	
	unsigned int VSMSType = GetSuitableMultisample(pRenderTarget->GetMulSample());
	VSTexture * pTexture = (VSTexture *)pRenderTarget->GetCreateBy();
	VSTextureID * pTextureID = NULL;
	unsigned int TextureType = 0;
	if (pTexture)
	{
		pTextureID = (VSTextureID *)pTexture->GetIdentifier();
		TextureType = pTexture->GetTexType();
	}
	OnLoadRenderTargetEx(pRenderTarget->GetWidth(), pRenderTarget->GetHeight(), pRenderTarget->GetFormatType(), VSMSType,
		pRenderTarget->GetLockFlag(), TextureType, pRenderTarget->GetLevel(), pRenderTarget->GetFirst(),pRenderTarget->GetArraySize(),
		pTextureID, pRenderTargetID);

	return true;
}
bool VSDX11Renderer::OnLoadUnorderAccess(VSBufferUnorderAccess  * pUnorderAccess, VSResourceIdentifier *&pID)
{
	if (!pUnorderAccess)
	{
		return false;
	}

	VSUnorderAccessID *pUnorderAccessID = VS_NEW VSUnorderAccessID;
	pID = pUnorderAccessID;

	VSBufferResource * pBufferResource = (VSBufferResource *)pUnorderAccess->GetCreateBy();
	VSSBufferID * pSBufferID = NULL;

	if (pBufferResource)
	{
		pSBufferID = (VSSBufferID *)pBufferResource->GetIdentifier();
	}


	OnLoadUnOrderAccessEx(pUnorderAccess->GetNum(), pUnorderAccess->GetDataType(),
		pUnorderAccess->GetByteSize(), pUnorderAccess->GetStructStride(), pUnorderAccess->GetLockFlag(),
		pSBufferID, pUnorderAccessID);
	return true;
}
bool VSDX11Renderer::OnLoadUnorderAccess(VSTextureUnorderAccess  * pUnorderAccess, VSResourceIdentifier *&pID)
{
	if (!pUnorderAccess)
	{
		return false;
	}

	VSUnorderAccessID *pUnorderAccessID = VS_NEW VSUnorderAccessID;
	pID = pUnorderAccessID;

	unsigned int VSMSType = GetSuitableMultisample(pUnorderAccess->GetMulSample());
	VSTexture * pTexture = (VSTexture *)pUnorderAccess->GetCreateBy();
	VSTextureID * pTextureID = NULL;

	unsigned int TextureType = 0;
	if (pTexture)
	{
		pTextureID = (VSTextureID *)pTexture->GetIdentifier();
		TextureType = pTexture->GetTexType();
	}


	OnLoadTextureUnorderAccessEx(pUnorderAccess->GetWidth(), pUnorderAccess->GetHeight(), pUnorderAccess->GetFormatType(),
		pUnorderAccess->GetLockFlag(), TextureType, pUnorderAccess->GetLevel(), pUnorderAccess->GetFirst(), pUnorderAccess->GetArraySize(),
		pTextureID, pUnorderAccessID);

	return true;
}
bool VSDX11Renderer::OnLoadVBufferData(VSVertexBuffer * pVBuffer, VSResourceIdentifier *&pID)
{
	VSVBufferID * pVBufferID = NULL;
	pVBufferID = VS_NEW VSVBufferID;
	if (!pVBufferID)
		return 0;
	pID = pVBufferID;

	unsigned int uiOneVertexSize = pVBuffer->GetOneVertexSize();
	unsigned int uiTotalSize = uiOneVertexSize * pVBuffer->GetVertexNum();

	D3D11_USAGE Usage = D3D11_USAGE_DEFAULT;
	unsigned int CPUAccessFlags = 0;
	if (!pVBuffer->IsStatic())
	{
		Usage = D3D11_USAGE_DYNAMIC;
		CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		Usage = D3D11_USAGE_IMMUTABLE;
		CPUAccessFlags = 0;
	}
	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = uiTotalSize;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	desc.Usage = Usage;
	desc.CPUAccessFlags = CPUAccessFlags;

	OnLoadVertexBufferEx(desc, pVBuffer, pVBufferID);
	return true;
}
bool VSDX11Renderer::OnLoadSBuffer(VSBufferResource* pSBuffer, VSResourceIdentifier *&pID)
{
	VSSBufferID * pSBufferID = NULL;
	pSBufferID = VS_NEW VSSBufferID;
	if (!pSBufferID)
		return 0;
	pID = pSBufferID;

	unsigned int BindFlags = D3D11_BIND_SHADER_RESOURCE;
	unsigned int MiscFlags = 0;
	unsigned int StructureByteStride = 0;
	D3D11_USAGE Usage = D3D11_USAGE_DEFAULT;
	unsigned int CPUAccessFlags = 0;
	if (pSBuffer->GetOutputType() == VSOutputResource::OT_BUFFER_UNORDER_ACCESS)
	{
		Usage = D3D11_USAGE_DEFAULT;
		CPUAccessFlags = 0;
		BindFlags |= D3D11_BIND_UNORDERED_ACCESS;

	}
	else
	{
		if (!pSBuffer->IsStatic())
		{
			Usage = D3D11_USAGE_DYNAMIC;
			CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
		else
		{
			Usage = D3D11_USAGE_IMMUTABLE;
			CPUAccessFlags = 0;
		}
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	unsigned int uiNum = pSBuffer->GetNum();
	if (pSBuffer->GetDataType() == VSDataBuffer::DT_UBYTE)
	{
		MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;

		SRVDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
		SRVDesc.BufferEx.FirstElement = 0;
		SRVDesc.BufferEx.NumElements = uiNum / 4;
		SRVDesc.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;
	}
	else if (pSBuffer->GetDataType() == VSDataBuffer::DT_STRUCT)
	{
		MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		StructureByteStride = pSBuffer->GetStructStride();

		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		SRVDesc.Format = DXGI_FORMAT_UNKNOWN;
		SRVDesc.Buffer.FirstElement = 0;
		SRVDesc.Buffer.NumElements = uiNum;
	}
	else
	{
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		SRVDesc.Format = (DXGI_FORMAT)ms_dwVertexFormatDataType[pSBuffer->GetDataType()];
		SRVDesc.Buffer.FirstElement = 0;
		SRVDesc.Buffer.NumElements = uiNum;
	}

	void* pBuffer = NULL;
	if (pSBuffer->GetDataBuffer())
	{
		pBuffer = pSBuffer->GetDataBuffer()->GetData();
	}
	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = pSBuffer->GetByteSize();
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.MiscFlags = MiscFlags;
	desc.StructureByteStride = StructureByteStride;
	desc.Usage = Usage;
	desc.CPUAccessFlags = CPUAccessFlags;
	OnLoadSBufferEX(desc, SRVDesc,pBuffer, pSBufferID);
	return 1;
}
bool VSDX11Renderer::OnLoadIBuffer(VSIndexBuffer* pIBuffer, VSResourceIdentifier *&pID)
{ 
	VSIBufferID * pIBufferID = NULL;
	pIBufferID = VS_NEW VSIBufferID;
	if (!pIBufferID)
		return 0;
	pID = pIBufferID;

	void* pBuffer = NULL;
	if (pIBuffer->GetIndexData())
	{
		pBuffer = pIBuffer->GetIndexData()->GetData();
	}
	unsigned int ByteWidth = pIBuffer->GetByteSize();
	D3D11_USAGE Usage = D3D11_USAGE_DEFAULT;
	unsigned int CPUAccessFlags = 0;

	if (!pIBuffer->IsStatic())
	{
		Usage = D3D11_USAGE_DYNAMIC;
		CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		Usage = D3D11_USAGE_IMMUTABLE;
		CPUAccessFlags = 0;
	}
	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = ByteWidth;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	desc.Usage = Usage;
	desc.CPUAccessFlags = CPUAccessFlags;

	OnLoadIndexBufferEx(desc, pBuffer, pIBufferID);
	return true;
	
}
bool VSDX11Renderer::OnLoadBlendState(VSBlendState * pBlendState, VSResourceIdentifier *&pID)
{ 
	VSBlendStateID * pBlendStateID = NULL;
	pBlendStateID = VS_NEW VSBlendStateID;
	if (!pBlendStateID)
		return 0;
	pID = pBlendStateID;
	const VSBlendDesc& BD = pBlendState->GetBlendDesc();
	D3D11_BLEND_DESC desc;
	
	desc.AlphaToCoverageEnable = BD.bAlphaToCoverageEnable;
	desc.IndependentBlendEnable = BD.bIndependentBlendEnable;
	for (unsigned int i = 0; i < 8; ++i)
	{
		D3D11_RENDER_TARGET_BLEND_DESC& out = desc.RenderTarget[i];
		out.BlendEnable = BD.bBlendEnable[i];
		out.SrcBlend = (D3D11_BLEND)ms_dwAlphaBlend[BD.ucSrcBlend[i]];
		out.DestBlend = (D3D11_BLEND)ms_dwAlphaBlend[BD.ucDestBlend[i]];
		out.BlendOp = (D3D11_BLEND_OP)ms_dwAlphaBlendOP[BD.ucBlendOp[i]];
		out.SrcBlendAlpha = (D3D11_BLEND)ms_dwAlphaBlend[BD.ucSrcBlendAlpha[i]];
		out.DestBlendAlpha = (D3D11_BLEND)ms_dwAlphaBlend[BD.ucDestBlendAlpha[i]];
		out.BlendOpAlpha = (D3D11_BLEND_OP)ms_dwAlphaBlendOP[BD.ucBlendOpAlpha[i]];
		out.RenderTargetWriteMask = BD.ucWriteMask[i];
	}
	CreateBlendState(desc, pBlendStateID);
	return true;
}
bool VSDX11Renderer::OnLoadDepthStencilState(VSDepthStencilState * pDepthStencilState, VSResourceIdentifier *&pID)
{ 
	VSDepthStencilStateID * pDepthStenciStateID = NULL;
	pDepthStenciStateID = VS_NEW VSDepthStencilStateID;
	if (!pDepthStenciStateID)
		return 0;
	pID = pDepthStenciStateID;
	const VSDepthStencilDesc& DSD = pDepthStencilState->GetDepthStencilDesc();
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	
	depthStencilDesc.DepthEnable = DSD.m_bDepthEnable;
	depthStencilDesc.DepthWriteMask = DSD.m_bDepthWritable ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = (D3D11_COMPARISON_FUNC)ms_dwCompare[DSD.m_uiDepthCompareMethod];

	depthStencilDesc.StencilEnable = DSD.m_bStencilEnable;
	depthStencilDesc.StencilReadMask = DSD.m_uiMask;
	depthStencilDesc.StencilWriteMask = DSD.m_uiWriteMask;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = (D3D11_STENCIL_OP)ms_dwStencilOperation[DSD.m_uiSFailZPassOP];
	depthStencilDesc.FrontFace.StencilDepthFailOp = (D3D11_STENCIL_OP)ms_dwStencilOperation[DSD.m_uiSPassZFailOP];
	depthStencilDesc.FrontFace.StencilPassOp = (D3D11_STENCIL_OP)ms_dwStencilOperation[DSD.m_uiSPassZPassOP];
	depthStencilDesc.FrontFace.StencilFunc = (D3D11_COMPARISON_FUNC)ms_dwCompare[DSD.m_uiStencilCompareMethod];

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = (D3D11_STENCIL_OP)ms_dwStencilOperation[DSD.m_uiCCW_SFailZPassOP];
	depthStencilDesc.BackFace.StencilDepthFailOp = (D3D11_STENCIL_OP)ms_dwStencilOperation[DSD.m_uiCCW_SPassZFailOP];
	depthStencilDesc.BackFace.StencilPassOp = (D3D11_STENCIL_OP)ms_dwStencilOperation[DSD.m_uiCCW_SPassZPassOP];
	depthStencilDesc.BackFace.StencilFunc = (D3D11_COMPARISON_FUNC)ms_dwCompare[DSD.m_uiCCW_StencilCompareMethod];

	CreateDepthStencilState(depthStencilDesc, pDepthStenciStateID);

	return true;
}
bool VSDX11Renderer::OnLoadRasterizerState(VSRasterizerState * pRasterizerState, VSResourceIdentifier *&pID)
{ 
	VSRasterStateID * pRasterStateID = NULL;
	pRasterStateID = VS_NEW VSRasterStateID;
	if (!pRasterStateID)
		return 0;
	pID = pRasterStateID;
	const VSRasterizerDesc& RD = pRasterizerState->GetRasterizerDesc();
	D3D11_RASTERIZER_DESC rasterDesc;
	
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = (D3D11_CULL_MODE)ms_dwCullType[RD.m_uiCullType];
	rasterDesc.DepthBias = (int)RD.m_fDepthBias;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = (RD.m_bWireEnable) ? D3D11_FILL_WIREFRAME :D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = RD.m_bScissorTestEnable;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
	CreateRasterizerState(rasterDesc, pRasterStateID);
	return true; 
}
bool VSDX11Renderer::OnLoadQuery(VSQuery * pQuery, VSResourceIdentifier *&pID)
{
	VSQueryID * pQueryID = NULL;
	pQueryID = VS_NEW VSQueryID;
	if (!pQueryID)
		return 0;
	pID = pQueryID;
	CreateQuery((D3D11_QUERY)ms_dwQueryType[pQuery->GetQueryType()], pQueryID);
	return 1;
}
bool VSDX11Renderer::OnLoadSamplerState(VSSamplerState * pSamplerState, VSResourceIdentifier *&pID)
{ 
	VSSamplerStateID * pSamplerStateID = NULL;
	pSamplerStateID = VS_NEW VSSamplerStateID;
	if (!pSamplerStateID)
		return 0;
	pID = pSamplerStateID;
	const VSSamplerDesc& SD = pSamplerState->GetSamplerDesc();
	D3D11_SAMPLER_DESC sampDesc;
	
	sampDesc.Filter = (D3D11_FILTER)ms_dwTextureFilterType[SD.m_uiMin][SD.m_uiMag][SD.m_uiMip];
	sampDesc.AddressU = (D3D11_TEXTURE_ADDRESS_MODE)ms_dwTextureAddrType[SD.m_uiCoordU];
	sampDesc.AddressV = (D3D11_TEXTURE_ADDRESS_MODE)ms_dwTextureAddrType[SD.m_uiCoordV];
	sampDesc.AddressW = (D3D11_TEXTURE_ADDRESS_MODE)ms_dwTextureAddrType[SD.m_uiCoordW];
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = SD.m_uiMipLevel * 1.0f;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	sampDesc.BorderColor[0] = SD.m_BorderColor.r;
	sampDesc.BorderColor[1] = SD.m_BorderColor.g;
	sampDesc.BorderColor[2] = SD.m_BorderColor.b;
	sampDesc.BorderColor[3] = SD.m_BorderColor.a;
	if (SD.m_uiAniLevel <= m_uiCurAnisotropy)
	{
		sampDesc.MaxAnisotropy = SD.m_uiAniLevel;
	}
	else
	{
		sampDesc.MaxAnisotropy = m_uiCurAnisotropy;

	}
	sampDesc.MipLODBias = SD.m_MipLODBias;
	CreateSamplerState(sampDesc, pSamplerStateID);
	return true;
}
unsigned int VSDX11Renderer::SetVertexFormat(VSVertexFormat * pVertexFormat)
{ 
	unsigned int FRI = VSRenderer::SetVertexFormat(pVertexFormat);
	if (FRI == FRI_SAMERESOURCE)
		return 1;
	else if (FRI == FRI_FAIL)
	{
		IASetInputLayout(NULL);
		return 0;
	}

	VSVBufferFormatID * pVBufferFormatID = (VSVBufferFormatID *)pVertexFormat->GetIdentifier();
	if (!pVBufferFormatID)
		return 0;
	IASetInputLayout(pVBufferFormatID);
	return 1;
}

void * VSDX11Renderer::Lock(VSVertexBuffer * pVertexBuffer)
{ 
	if (!pVertexBuffer)
	{
		VSMAC_ASSERT(0);
		return NULL;
	}

	if (pVertexBuffer->IsStatic())
	{
		VSMAC_ASSERT(0);
		return NULL;
	}

	VSVBufferID* pVBufferID = (VSVBufferID *)pVertexBuffer->GetIdentifier();

	if (!pVBufferID->m_pVertexBuffer)
	{
		VSMAC_ASSERT(0);
		return NULL;
	}

	D3D11_MAPPED_SUBRESOURCE MappedResource;
	
	HRESULT hResult = NULL;
	hResult = m_pDeviceContext->Map(pVBufferID->m_pVertexBuffer, 0, (D3D11_MAP)ms_dwLockFlag[pVertexBuffer->GetLockFlag()], 0, &MappedResource);
	VSMAC_ASSERT(!FAILED(hResult));
	return MappedResource.pData;
}
void VSDX11Renderer::UnLock(VSIndexBuffer * pIndexBuffer)
{
	if (!pIndexBuffer)
		return;

	if (pIndexBuffer->IsStatic())
		return;
	VSIBufferID* pIBufferID = (VSIBufferID *)pIndexBuffer->GetIdentifier();
	if (!pIBufferID)
	{
		return;
	}
	if (!pIBufferID->m_IndexBuffer)
		return;
	m_pDeviceContext->Unmap(pIBufferID->m_IndexBuffer, 0);
	return;
}
void VSDX11Renderer::UnLock(VSVertexBuffer * pVertexBuffer)
{ 
	if (!pVertexBuffer)
		return;


	if (pVertexBuffer->IsStatic())
		return;

	VSVBufferID* pVBufferID = (VSVBufferID *)pVertexBuffer->GetIdentifier();
	if (!pVBufferID)
	{
		return;
	}
	if (!pVBufferID->m_pVertexBuffer)
	{
		return;
	}

	m_pDeviceContext->Unmap(pVBufferID->m_pVertexBuffer, 0);
	return;
}

void * VSDX11Renderer::Lock(VSIndexBuffer * pIndexBuffer)
{ 
	if (!pIndexBuffer)
	{
		VSMAC_ASSERT(0);
		return NULL;
	}


	if (pIndexBuffer->IsStatic())
	{
		VSMAC_ASSERT(0);
		return NULL;
	}

	VSIBufferID* pIBufferID = (VSIBufferID *)pIndexBuffer->GetIdentifier();

	if (!pIBufferID->m_IndexBuffer)
	{
		VSMAC_ASSERT(0);
		return NULL;
	}
	D3D11_MAPPED_SUBRESOURCE MappedResource;

	HRESULT hResult = NULL;
	hResult = m_pDeviceContext->Map(pIBufferID->m_IndexBuffer, 0, (D3D11_MAP)ms_dwLockFlag[pIndexBuffer->GetLockFlag()], 0, &MappedResource);
	VSMAC_ASSERT(!FAILED(hResult));
	return MappedResource.pData;
}

void * VSDX11Renderer::Lock(VSTexture * pTexture, unsigned int uiLevel, unsigned int uiFace)
{ 
	if (!pTexture)
	{
		VSMAC_ASSERT(0);
		return NULL;
	}
		
	if (pTexture->GetOutputType() != VSOutputResource::OT_NONE)
	{
		VSMAC_ASSERT(0);
		return NULL;
	}
	if (pTexture->IsStatic() || uiLevel >= pTexture->GetMipLevel())
	{
		VSMAC_ASSERT(0);
		return NULL;
	}

	VSTextureID * pTextureID = (VSTextureID *)pTexture->GetIdentifier();
	if (!pTextureID)
	{
		VSMAC_ASSERT(0);
		return NULL;
	}
	if (!pTextureID->m_pTexture)
	{
		VSMAC_ASSERT(0);
		return NULL;
	}
	HRESULT hResult = NULL;
	if (pTexture->GetTexType() == VSTexture::TT_2D)
	{
		D3D11_MAPPED_SUBRESOURCE MappedResource;
		const UINT Subresource = D3D11CalcSubresource(uiLevel, 0, pTexture->GetMipLevel());
		HRESULT hResult = NULL;
		hResult = m_pDeviceContext->Map(pTextureID->m_pTexture, Subresource, (D3D11_MAP)ms_dwLockFlag[pTexture->GetLockFlag()], 0, &MappedResource);
		VSMAC_ASSERT(!FAILED(hResult));
		return MappedResource.pData;
	}
	else if (pTexture->GetTexType() == VSTexture::TT_3D)
	{
		D3D11_MAPPED_SUBRESOURCE MappedResource;
		const UINT Subresource = D3D11CalcSubresource(uiLevel, 0, pTexture->GetMipLevel());
		HRESULT hResult = NULL;
		hResult = m_pDeviceContext->Map(pTextureID->m_pTexture, Subresource, (D3D11_MAP)ms_dwLockFlag[pTexture->GetLockFlag()], 0, &MappedResource);
		VSMAC_ASSERT(!FAILED(hResult));
		return MappedResource.pData;
	}
	else if (pTexture->GetTexType() == VSTexture::TT_CUBE)
	{
		if (uiFace > VSCubeTexture::F_MAX)
			return NULL;
		D3D11_MAPPED_SUBRESOURCE MappedResource;
		const UINT Subresource = D3D11CalcSubresource(uiLevel, uiFace, pTexture->GetMipLevel());
		HRESULT hResult = NULL;
		hResult = m_pDeviceContext->Map(pTextureID->m_pTexture, Subresource, (D3D11_MAP)ms_dwLockFlag[pTexture->GetLockFlag()], 0, &MappedResource);
		VSMAC_ASSERT(!FAILED(hResult));
		return MappedResource.pData;

	}
	else
	{
		VSMAC_ASSERT(0);
		return NULL;
	}
}
void VSDX11Renderer::UnLock(VSTexture * pTexture, unsigned int uiLevel, unsigned int uiFace)
{ 
	if (!pTexture)
		return;
	if (pTexture->GetOutputType() != VSOutputResource::OT_NONE)
	{
		return;
	}
	if (pTexture->IsStatic() || uiLevel >= pTexture->GetMipLevel())
		return;
	VSTextureID * pTextureID = (VSTextureID *)pTexture->GetIdentifier();
	if (!pTextureID)
		return;
	if (!pTextureID->m_pTexture)
		return;
	HRESULT hResult = NULL;
	if (pTexture->GetTexType() == VSTexture::TT_2D)
	{
		const UINT Subresource = D3D11CalcSubresource(uiLevel, 0, pTexture->GetMipLevel());
		m_pDeviceContext->Unmap(pTextureID->m_pTexture, Subresource);
		return;
	}
	else if (pTexture->GetTexType() == VSTexture::TT_3D)
	{
		const UINT Subresource = D3D11CalcSubresource(uiLevel, 0, pTexture->GetMipLevel());
		m_pDeviceContext->Unmap(pTextureID->m_pTexture, Subresource);
		return;
	}
	else if (pTexture->GetTexType() == VSTexture::TT_CUBE)
	{
		if (uiFace > VSCubeTexture::F_MAX)
			return;
		const UINT Subresource = D3D11CalcSubresource(uiLevel, uiFace, pTexture->GetMipLevel());
		m_pDeviceContext->Unmap(pTextureID->m_pTexture, Subresource);
		return;

	}
	else
	{
		VSMAC_ASSERT(0);
		return;
	}
	return;
}
bool VSDX11Renderer::CopyResourceBuffer(VS2DTexture * pSource, VSCubeTexture * pDest, unsigned int uiFace)
{ 
	if (!VSRenderer::CopyResourceBuffer(pSource, pDest, uiFace))
	{
		return false;
	}
	VSTextureID *pTextureID = (VSTextureID *)pSource->GetIdentifier();
	if (!pTextureID)
	{
		return false;
	}

	VSTextureID * pCubTextureID = (VSTextureID *)pDest->GetIdentifier();
	if (!pCubTextureID)
	{
		return false;
	}
	
	for (unsigned int i = 0; i < pDest->GetMipLevel(); i++)
	{
		const UINT DestSubresource = D3D11CalcSubresource(i, uiFace, pSource->GetMipLevel());
		const UINT SourceSubresource = D3D11CalcSubresource(i, 0, pSource->GetMipLevel());
		m_pDeviceContext->CopySubresourceRegion(pCubTextureID->m_pTexture, DestSubresource, 0, 0, 0, pTextureID->m_pTexture, SourceSubresource, NULL);
	}
	return true;
}
void * VSDX11Renderer::Lock(VSRenderTarget * pRenderTarget)
{
	if (!pRenderTarget)
	{
		VSMAC_ASSERT(0);
		return NULL;
	}
	if (pRenderTarget->GetLockFlag() != VSInheritBind::LF_READONLY)
	{
		VSMAC_ASSERT(0);
		return NULL;
	}

	VSRenderTargetID * pVSRenderTargetID = (VSRenderTargetID *)pRenderTarget->GetIdentifier();
	if (!pVSRenderTargetID)
	{
		VSMAC_ASSERT(0);
		return NULL;
	}

	ID3D11Resource* pDXStagTexture = pVSRenderTargetID->m_pStagTexture;
	if (!pDXStagTexture)
	{
		VSMAC_ASSERT(0);
		return  NULL;
	}
	D3D11_MAPPED_SUBRESOURCE MappedResource;

	HRESULT hResult = NULL;
	hResult = m_pDeviceContext->Map(pDXStagTexture, 0, (D3D11_MAP)ms_dwLockFlag[pRenderTarget->GetLockFlag()], 0, &MappedResource);
	VSMAC_ASSERT(!FAILED(hResult));
	return MappedResource.pData;
}
void VSDX11Renderer::UnLock(VSRenderTarget * pRenderTarget)
{
	if (!pRenderTarget)
	{
		return ;
	}
	if (pRenderTarget->GetLockFlag() != VSInheritBind::LF_READONLY)
	{
		return ;
	}

	VSRenderTargetID * pVSRenderTargetID = (VSRenderTargetID *)pRenderTarget->GetIdentifier();
	if (!pVSRenderTargetID)
		return ;

	ID3D11Resource* pDXStagTexture = pVSRenderTargetID->m_pStagTexture;
	if (!pDXStagTexture)
	{
		return  ;
	}
	m_pDeviceContext->Unmap(pDXStagTexture, 0);
}

void * VSDX11Renderer::Lock(VSBufferResource * pStructBuffer)
{
	if (!pStructBuffer)
	{
		VSMAC_ASSERT(0);
		return NULL;
	}
	if (pStructBuffer->GetOutputType() != VSOutputResource::OT_NONE)
	{
		VSMAC_ASSERT(0);
		return NULL;
	}
	if (pStructBuffer->IsStatic())
	{
		VSMAC_ASSERT(0);
		return NULL;
	}
	VSSBufferID * pSBufferID = (VSSBufferID *)pStructBuffer->GetIdentifier();
	if (!pSBufferID)
		return NULL;
	if (!pSBufferID->m_pSBuffer)
		return NULL;
	HRESULT hResult = NULL;
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	hResult = m_pDeviceContext->Map(pSBufferID->m_pSBuffer, 0, (D3D11_MAP)ms_dwLockFlag[pStructBuffer->GetLockFlag()], 0, &MappedResource);
	VSMAC_ASSERT(!FAILED(hResult));
	return MappedResource.pData;
}
void VSDX11Renderer::UnLock(VSBufferResource * pStructBuffer)
{
	if (!pStructBuffer)
		return;
	if (pStructBuffer->GetOutputType() != VSOutputResource::OT_NONE)
	{
		return;
	}
	if (pStructBuffer->IsStatic())
		return;
	VSSBufferID * pSBufferID = (VSSBufferID *)pStructBuffer->GetIdentifier();
	if (!pSBufferID)
		return;
	if (!pSBufferID->m_pSBuffer)
		return;
	HRESULT hResult = NULL;

	m_pDeviceContext->Unmap(pSBufferID->m_pSBuffer, 0);
	
	return;
}
void * VSDX11Renderer::Lock(VSDepthStencil * pDepthStencil)
{
	if (!pDepthStencil)
	{
		VSMAC_ASSERT(0);
		return NULL;
	}
	if (pDepthStencil->GetLockFlag() != VSInheritBind::LF_READONLY)
	{
		VSMAC_ASSERT(0);
		return NULL;
	}
	VSDepthStencilID * pVSDepthStenciltID = (VSDepthStencilID *)pDepthStencil->GetIdentifier();
	if (!pVSDepthStenciltID)
	{
		VSMAC_ASSERT(0);
		return NULL;
	}

	ID3D11Resource* pDXStagTexture = pVSDepthStenciltID->m_pStagTexture;
	if (!pDXStagTexture)
	{
		VSMAC_ASSERT(0);
		return  NULL;
	}
	D3D11_MAPPED_SUBRESOURCE MappedResource;

	HRESULT hResult = NULL;
	hResult = m_pDeviceContext->Map(pDXStagTexture, 0, (D3D11_MAP)ms_dwLockFlag[pDepthStencil->GetLockFlag()], 0, &MappedResource);
	VSMAC_ASSERT(!FAILED(hResult));
	return MappedResource.pData;
}
void VSDX11Renderer::UnLock(VSDepthStencil * pDepthStencil)
{
	if (!pDepthStencil)
	{
		return;
	}
	if (pDepthStencil->GetLockFlag() != VSInheritBind::LF_READONLY)
	{
		return;
	}

	VSDepthStencilID * pVSDepthStencilID = (VSDepthStencilID *)pDepthStencil->GetIdentifier();
	if (!pVSDepthStencilID)
		return;

	ID3D11Resource* pDXStagTexture = pVSDepthStencilID->m_pStagTexture;
	if (!pDXStagTexture)
	{
		return;
	}
	m_pDeviceContext->Unmap(pDXStagTexture, 0);
}
void * VSDX11Renderer::Lock(VSUnorderAccess  * pUnorderAccess)
{
	if (!pUnorderAccess)
	{
		VSMAC_ASSERT(0);
		return NULL;
	}
	if (pUnorderAccess->GetLockFlag() != VSInheritBind::LF_READONLY)
	{
		VSMAC_ASSERT(0);
		return NULL;
	}

	VSUnorderAccessID * pUnorderAccessID = (VSUnorderAccessID *)pUnorderAccess->GetIdentifier();
	if (!pUnorderAccessID)
	{
		VSMAC_ASSERT(0);
		return NULL;
	}

	ID3D11Resource* pDXtagBuffer = pUnorderAccessID->m_pStagBuffer;
	if (!pDXtagBuffer)
	{
		VSMAC_ASSERT(0);
		return  NULL;
	}
	D3D11_MAPPED_SUBRESOURCE MappedResource;

	HRESULT hResult = NULL;
	hResult = m_pDeviceContext->Map(pDXtagBuffer, 0, (D3D11_MAP)ms_dwLockFlag[pUnorderAccess->GetLockFlag()], 0, &MappedResource);
	VSMAC_ASSERT(!FAILED(hResult));
	return MappedResource.pData;
}
void VSDX11Renderer::UnLock(VSUnorderAccess  * pUnorderAccess)
{
	if (!pUnorderAccess)
	{
		return;
	}
	if (pUnorderAccess->GetLockFlag() != VSInheritBind::LF_READONLY)
	{
		return;
	}

	VSUnorderAccessID * pVSUnorderAccessID = (VSUnorderAccessID *)pUnorderAccess->GetIdentifier();
	if (!pVSUnorderAccessID)
		return;

	ID3D11Resource* pDXtagBuffer = pVSUnorderAccessID->m_pStagBuffer;
	if (!pDXtagBuffer)
	{
		return;
	}
	m_pDeviceContext->Unmap(pDXtagBuffer, 0);
}
bool VSDX11Renderer::GetData(VSQuery * pQuery, void * pData, unsigned int uiSize)
{
	unsigned char TempBuffer[8] = {0};
	unsigned int QuerySize = uiSize;
	if (!pQuery)
	{
		VSMAC_ASSERT(0);
		return FALSE;
	}
	VSQueryID *pQueryID = (VSQueryID *)pQuery->GetIdentifier();
	if (!pQueryID)
	{
		VSMAC_ASSERT(0);
		return FALSE;
	}

	if (pQuery->GetQueryType() == VSQuery::QT_OCCLUSION)
	{
		QuerySize = sizeof(UINT64);
	}
	ID3D11Query * pD3DQuery = pQueryID->m_Query;
	HRESULT Result = m_pDeviceContext->GetData(pD3DQuery, TempBuffer, QuerySize, 0);
	if (Result == S_FALSE)
	{
		do
		{
			Result = m_pDeviceContext->GetData(pD3DQuery, TempBuffer, QuerySize, 0);

		} while (Result == S_FALSE);
	}

	if (Result == S_OK)
	{
		VSMemcpy(pData, TempBuffer, uiSize);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
unsigned int VSDX11Renderer::GetTextureCompressLen(unsigned int uiLen)
{
	return Max((unsigned int)1, (unsigned int)(uiLen / 4));
}
void VSDX11Renderer::GetIncludeShader(VSString &OutString)
{
	OutString = ms_IncludeShader;
}

void VSDX11Renderer::GetDynamicShader(VSString &OutString)
{
	OutString = ms_DynamicShader;
}
const VSAdapterInfo* VSDX11Renderer::GetAdapterInfo(unsigned int & uiAdapterNum)
{
	uiAdapterNum = ms_uiAdapterNum;
	return ms_AdapterInfo;
}
#endif
