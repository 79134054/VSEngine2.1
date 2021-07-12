#include "VSDx9Renderer.h"
#include "VSDx9Resource.h"
#include "VSTexture.h"
#include "VSCubeTexture.h"
#include "VSVertexBuffer.h"
#include "VSVShader.h"
#include "VSPShader.h"
#include "VSGShader.h"
#include "VSGeometry.h"
#include "VSSegment3.h"
#include "VS2DTexture.h"
#include "VS3DTexture.h"
#include "VSCubeTexture.h"
#include "VSDepthStencil.h"
#include "VSUserConstant.h"
#include "VSDx9ShaderInclude.h"
#include "VSShaderKey.h"
#include "VSRenderTarget.h"
#include "VSGraphicInclude.h"
#include "VSRenderThread.h"
#include "VSConfig.h"
#include "VSQuery.h"
#ifdef WINDOWS_PLATFORM	
using namespace VSEngine2;
LPDIRECT3D9 VSDX9Renderer::ms_pMain = NULL;
VSDx9ShaderInclude * VSDX9Renderer::ms_pDx9IncludeShader = NULL;
TCHAR VSDX9Renderer::ms_cVertexShaderProgramVersion[] = _T("vs_3_0");
TCHAR VSDX9Renderer::ms_cPixelShaderProgramVersion[] = _T("ps_3_0");
VSString VSDX9Renderer::ms_IncludeShader;
VSString VSDX9Renderer::ms_DynamicShader;
VSAdapterInfo	VSDX9Renderer::ms_AdapterInfo[5];
UINT VSDX9Renderer::ms_uiAdapterNum = 0;
DWORD VSDX9Renderer::ms_dwAlphaBlend[] = 
{
	D3DBLEND_ZERO,
	D3DBLEND_ONE,
	D3DBLEND_SRCCOLOR,
	D3DBLEND_INVSRCCOLOR,
	D3DBLEND_SRCALPHA,
	D3DBLEND_INVSRCALPHA,
	D3DBLEND_DESTALPHA,
	D3DBLEND_INVDESTALPHA,
	D3DBLEND_DESTCOLOR,
	D3DBLEND_INVDESTCOLOR

};
DWORD VSDX9Renderer::ms_dwAlphaBlendOP[] = 
{
	D3DBLENDOP_ADD,
	D3DBLENDOP_SUBTRACT,
	D3DBLENDOP_REVSUBTRACT,
	D3DBLENDOP_MIN,
	D3DBLENDOP_MAX 
};
DWORD VSDX9Renderer::ms_dwCullType[] =
{
	D3DCULL_NONE,
	D3DCULL_CW,
	D3DCULL_CCW
};
DWORD VSDX9Renderer::ms_dwStencilOperation[] = 
{
	D3DSTENCILOP_KEEP,
	D3DSTENCILOP_ZERO,
	D3DSTENCILOP_REPLACE,
	D3DSTENCILOP_INCR,
	D3DSTENCILOP_DECR,
	D3DSTENCILOP_INVERT,
	D3DSTENCILOP_INCRSAT,
	D3DSTENCILOP_DECRSAT
};
DWORD VSDX9Renderer::ms_dwCompare[] =
{
	D3DCMP_NEVER,
	D3DCMP_LESS,
	D3DCMP_EQUAL,
	D3DCMP_LESSEQUAL,
	D3DCMP_GREATER,
	D3DCMP_NOTEQUAL,
	D3DCMP_GREATEREQUAL,
	D3DCMP_ALWAYS
};
DWORD VSDX9Renderer::ms_dwTextureFormatType[] = 
{
	D3DFMT_A8R8G8B8, 
	D3DFMT_X8R8G8B8,
	D3DFMT_D16,                         
	D3DFMT_A32B32G32R32F, 
	D3DFMT_A16B16G16R16F, 
	D3DFMT_G16R16F, 
	D3DFMT_R16F, 
	D3DFMT_R32F,
	D3DFMT_UNKNOWN,
	D3DFMT_D24S8,
	D3DFMT_G32R32F,
	D3DFMT_R5G6B5,
	D3DFMT_A8,
	D3DFMT_G16R16,
	D3DFMT_A16B16G16R16,
	D3DFMT_L8,
	D3DFMT_Q8W8V8U8,
	D3DFMT_V16U16,
	D3DFMT_L16,
	D3DFMT_Q16W16V16U16,
	D3DFMT_DXT1,
	D3DFMT_DXT3,
	D3DFMT_DXT5,
	MAKEFOURCC('A', 'T', 'I', '1'),
	MAKEFOURCC('A', 'T', 'I', '2')
};
DWORD VSDX9Renderer::ms_dwTextureFilterType[] =
{
	D3DTEXF_NONE,      
	D3DTEXF_POINT,       
	D3DTEXF_LINEAR,        
	D3DTEXF_ANISOTROPIC  
};
DWORD VSDX9Renderer::ms_dwTextureAddrType[] =
{
	D3DTADDRESS_CLAMP,      
	D3DTADDRESS_WRAP,       
	D3DTADDRESS_MIRROR,     
	D3DTADDRESS_BORDER,    
};
DWORD VSDX9Renderer::ms_dwPrimitiveType[] =
{
	D3DPT_POINTLIST,
	D3DPT_LINELIST,
	D3DPT_TRIANGLELIST
};
DWORD VSDX9Renderer::ms_dwDevTypes[] = 
{ 
	D3DDEVTYPE_HAL, 
	D3DDEVTYPE_REF
};
DWORD VSDX9Renderer::ms_dwMultiSampleTypes[] =
{
	D3DMULTISAMPLE_NONE      ,
	D3DMULTISAMPLE_2_SAMPLES ,
	D3DMULTISAMPLE_4_SAMPLES ,
	D3DMULTISAMPLE_8_SAMPLES ,
	D3DMULTISAMPLE_16_SAMPLES 
};
DWORD VSDX9Renderer::ms_dwCubeMapFace[] =
{
	D3DCUBEMAP_FACE_POSITIVE_X,
	D3DCUBEMAP_FACE_NEGATIVE_X,
	D3DCUBEMAP_FACE_POSITIVE_Y,
	D3DCUBEMAP_FACE_NEGATIVE_Y,
	D3DCUBEMAP_FACE_POSITIVE_Z,
	D3DCUBEMAP_FACE_NEGATIVE_Z
};
DWORD VSDX9Renderer::ms_dwLockFlag[] = 
{
	D3DLOCK_NOOVERWRITE,
	D3DLOCK_DISCARD,
	D3DLOCK_READONLY
};
DWORD VSDX9Renderer::ms_dwVertexFormatDataType[] = 
{
	D3DDECLTYPE_FLOAT1, 
	D3DDECLTYPE_FLOAT2,
	D3DDECLTYPE_FLOAT3,
	D3DDECLTYPE_FLOAT4,

	0,
	D3DDECLTYPE_D3DCOLOR,

	D3DDECLTYPE_UBYTE4,
	D3DDECLTYPE_UBYTE4N,

	D3DDECLTYPE_SHORT2,
	D3DDECLTYPE_SHORT2N,
	D3DDECLTYPE_USHORT2N,

	D3DDECLTYPE_SHORT4,
	D3DDECLTYPE_SHORT4N,
	D3DDECLTYPE_USHORT4N,

	D3DDECLTYPE_FLOAT16_2,
	D3DDECLTYPE_FLOAT16_4,

	D3DDECLTYPE_UBYTE4N,

	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
};
DWORD VSDX9Renderer::ms_dwVertexFormatSemantics[] = 
{
	D3DDECLUSAGE_POSITION,
	D3DDECLUSAGE_TEXCOORD,
	D3DDECLUSAGE_NORMAL,
	D3DDECLUSAGE_TANGENT,
	D3DDECLUSAGE_BINORMAL,
	D3DDECLUSAGE_PSIZE,
	D3DDECLUSAGE_COLOR,        
	D3DDECLUSAGE_FOG,           
	D3DDECLUSAGE_DEPTH,// Vertex ID 
	D3DDECLUSAGE_BLENDWEIGHT,
	D3DDECLUSAGE_BLENDINDICES,
	0//material instance data dx9 no use
};
DWORD VSDX9Renderer::ms_dwQueryType[] =
{
	D3DQUERYTYPE_OCCLUSION,
};
int VSDX9Renderer::GetRendererType() const
{
	return VSRenderer::RAT_DIRECTX9;
}
unsigned int VSDX9Renderer::SetGSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet)
{
	VSRenderer::SetGSamplerState(pSamplerState, i, bForceSet);

	return 1;
}
unsigned int VSDX9Renderer::SetDSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet)
{
	VSRenderer::SetDSamplerState(pSamplerState, i, bForceSet);

	return 1;
}
unsigned int VSDX9Renderer::SetHSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet)
{
	VSRenderer::SetHSamplerState(pSamplerState, i, bForceSet);

	return 1;
}
unsigned int VSDX9Renderer::SetCSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet)
{
	VSRenderer::SetCSamplerState(pSamplerState, i, bForceSet);

	return 1;
}
unsigned int VSDX9Renderer::SetVSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet)
{
	unsigned int FRI = VSRenderer::SetVSamplerState(pSamplerState, i, bForceSet);
	if (FRI == FRI_SAMERESOURCE)
		return FRI_SUCCESS;
	VSMAC_ASSERT(FRI == FRI_SUCCESS);
	if (FRI == FRI_FAIL)
	{
		return FRI_FAIL;
	}
	if (!pSamplerState)
	{
		pSamplerState = (VSSamplerState *)VSSamplerState::GetDefault();
	}

	m_pSamplerState[VSEngineFlag::ST_VERTEX][i] = pSamplerState;
	return 1;
}
unsigned int VSDX9Renderer::SetPSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet)
{
	unsigned int FRI = VSRenderer::SetPSamplerState(pSamplerState, i, bForceSet);
	if (FRI == FRI_SAMERESOURCE)
		return FRI_SUCCESS;
	VSMAC_ASSERT(FRI == FRI_SUCCESS);
	if (FRI == FRI_FAIL)
	{
		return FRI_FAIL;
	}
	if (!pSamplerState)
	{
		pSamplerState = (VSSamplerState *)VSSamplerState::GetDefault();
	}
	const VSSamplerDesc & BlendStateSrc = m_pSamplerState[VSEngineFlag::ST_PIXEL][i]->GetSamplerDesc();
	const VSSamplerDesc & BlendStateDest = pSamplerState->GetSamplerDesc();

	if (BlendStateSrc.m_uiCoordU != BlendStateDest.m_uiCoordU)
	{
		SetSamplerState(i, D3DSAMP_ADDRESSU, ms_dwTextureAddrType[BlendStateDest.m_uiCoordU]);
	}

	if (BlendStateSrc.m_uiCoordV != BlendStateDest.m_uiCoordV)
	{

		SetSamplerState(i, D3DSAMP_ADDRESSV, ms_dwTextureAddrType[BlendStateDest.m_uiCoordV]);
	}

	if (BlendStateSrc.m_uiCoordW != BlendStateDest.m_uiCoordW)
	{
		SetSamplerState(i, D3DSAMP_ADDRESSW, ms_dwTextureAddrType[BlendStateDest.m_uiCoordW]);
	}

	if (BlendStateSrc.m_BorderColor.GetDWARGB() != BlendStateDest.m_BorderColor.GetDWARGB())
	{

		SetSamplerState(i, D3DSAMP_BORDERCOLOR, BlendStateDest.m_BorderColor.GetDWARGB());
	}


	if (BlendStateSrc.m_uiMag != BlendStateDest.m_uiMag)
	{

		SetSamplerState(i, D3DSAMP_MAGFILTER, ms_dwTextureFilterType[BlendStateDest.m_uiMag]);
	}

	if (BlendStateSrc.m_uiMin != BlendStateDest.m_uiMin)
	{
		SetSamplerState(i, D3DSAMP_MINFILTER, ms_dwTextureFilterType[BlendStateDest.m_uiMin]);
	}

	if (BlendStateSrc.m_uiMip != BlendStateDest.m_uiMip)
	{
		SetSamplerState(i, D3DSAMP_MIPFILTER, ms_dwTextureFilterType[BlendStateDest.m_uiMip]);
	}

	if (BlendStateSrc.m_uiMipLevel != BlendStateDest.m_uiMipLevel)
	{
		SetSamplerState(i, D3DSAMP_MAXMIPLEVEL, BlendStateDest.m_uiMipLevel);
	}


	if (BlendStateDest.m_uiAniLevel <= m_uiCurAnisotropy)
	{
		if (BlendStateSrc.m_uiAniLevel != BlendStateDest.m_uiAniLevel)
		{
			SetSamplerState(i, D3DSAMP_MAXANISOTROPY, BlendStateDest.m_uiAniLevel);
		}
	}
	else
	{
		if (BlendStateSrc.m_uiAniLevel > m_uiCurAnisotropy)
		{
			SetSamplerState(i, D3DSAMP_MAXANISOTROPY, m_uiCurAnisotropy);
		}

	}
	m_pSamplerState[VSEngineFlag::ST_PIXEL][i] = pSamplerState;
	return 1;
}
unsigned int VSDX9Renderer::SetBlendState(VSBlendState * pBlendState,bool bForceSet)
{
	unsigned int FRI = VSRenderer::SetBlendState(pBlendState, bForceSet);
	if (FRI == FRI_SAMERESOURCE)
		return FRI_SUCCESS;
	VSMAC_ASSERT(FRI == FRI_SUCCESS);
	if (FRI == FRI_FAIL)
	{
		return FRI_FAIL;
	}
	if (!pBlendState)
	{
		pBlendState = (VSBlendState *)VSBlendState::GetDefault();
	}
	const VSBlendDesc & BlendStateSrc = m_pBlendState->GetBlendDesc();
	const VSBlendDesc & BlendStateDest = pBlendState->GetBlendDesc();
	if (BlendStateSrc.bBlendEnable[0] != BlendStateDest.bBlendEnable[0])
	{
		SetRenderState(D3DRS_ALPHABLENDENABLE, BlendStateDest.bBlendEnable[0]);
	}


	if (BlendStateSrc.ucSrcBlend[0] != BlendStateDest.ucSrcBlend[0])
	{
		SetRenderState(D3DRS_SRCBLEND,
			ms_dwAlphaBlend[BlendStateDest.ucSrcBlend[0]]);
	}

	if (BlendStateSrc.ucDestBlend[0] != BlendStateDest.ucDestBlend[0])
	{
		SetRenderState(D3DRS_DESTBLEND,
			ms_dwAlphaBlend[BlendStateDest.ucDestBlend[0]]);
	}

	if (BlendStateSrc.ucBlendOp[0] != BlendStateDest.ucBlendOp[0])
	{
		SetRenderState(D3DRS_BLENDOP,
			ms_dwAlphaBlendOP[BlendStateDest.ucBlendOp[0]]);
	}



	if (BlendStateSrc.bAlphaBlendEnable[0] != BlendStateDest.bAlphaBlendEnable[0])
	{
		SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, BlendStateDest.bAlphaBlendEnable[0]);
	}


	if (BlendStateSrc.ucSrcBlendAlpha[0] != BlendStateDest.ucSrcBlendAlpha[0])
	{
		SetRenderState(D3DRS_SRCBLENDALPHA,
			ms_dwAlphaBlend[BlendStateDest.ucSrcBlendAlpha[0]]);
	}


	if (BlendStateSrc.ucDestBlendAlpha[0] != BlendStateDest.ucDestBlendAlpha[0])
	{
		SetRenderState(D3DRS_DESTBLENDALPHA,
			ms_dwAlphaBlend[BlendStateDest.ucDestBlendAlpha[0]]);
	}


	if (BlendStateSrc.ucBlendOpAlpha[0] != BlendStateDest.ucBlendOpAlpha[0])
	{
		SetRenderState(D3DRS_BLENDOPALPHA,
			ms_dwAlphaBlendOP[BlendStateDest.ucBlendOpAlpha[0]]);
	}


	for (unsigned int i = 0; i < 4; i++)
	{

		if (BlendStateSrc.ucWriteMask[i] != BlendStateDest.ucWriteMask[i])
		{
			DWORD dwMask = 0;

			if (BlendStateDest.ucWriteMask[i] & VSBlendDesc::WM_RED)
			{
				dwMask |= D3DCOLORWRITEENABLE_RED;
			}

			if (BlendStateDest.ucWriteMask[i] & VSBlendDesc::WM_Green)
			{
				dwMask |= D3DCOLORWRITEENABLE_GREEN;
			}

			if (BlendStateDest.ucWriteMask[i] & VSBlendDesc::WM_BLUE)
			{
				dwMask |= D3DCOLORWRITEENABLE_BLUE;
			}

			if (BlendStateDest.ucWriteMask[i] & VSBlendDesc::WM_ALPHA)
			{
				dwMask |= D3DCOLORWRITEENABLE_ALPHA;
			}

			if (i == 0)
			{
				SetRenderState(D3DRS_COLORWRITEENABLE, dwMask);
			}
			else
			{
				SetRenderState(D3DRENDERSTATETYPE(D3DRS_COLORWRITEENABLE1 + i - 1), dwMask);
			}
		}

	}

	m_pBlendState = pBlendState;
	return 1;
}
unsigned int VSDX9Renderer::SetDepthStencilState(VSDepthStencilState * pDepthStencilState, bool bForceSet)
{
	unsigned int FRI = VSRenderer::SetDepthStencilState(pDepthStencilState, bForceSet);
	if (FRI == FRI_SAMERESOURCE)
		return FRI_SUCCESS;
	VSMAC_ASSERT(FRI == FRI_SUCCESS);
	if (FRI == FRI_FAIL)
	{
		return FRI_FAIL;
	}
	if (!pDepthStencilState)
	{
		pDepthStencilState = (VSDepthStencilState *)VSDepthStencilState::GetDefault();
	}
	const VSDepthStencilDesc & DepthStencilDescDest = pDepthStencilState->GetDepthStencilDesc();
	const VSDepthStencilDesc & DepthStencilDescSrc = m_pDepthStencilState->GetDepthStencilDesc();

	if (DepthStencilDescSrc.m_bDepthEnable != DepthStencilDescDest.m_bDepthEnable)
	{
		if (DepthStencilDescDest.m_bDepthEnable)
		{
			SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		}
		else
		{
			SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		}
	}



	if (DepthStencilDescSrc.m_uiDepthCompareMethod != DepthStencilDescDest.m_uiDepthCompareMethod)
	{
		SetRenderState(D3DRS_ZFUNC,
			ms_dwCompare[DepthStencilDescDest.m_uiDepthCompareMethod]);
	}

	if (DepthStencilDescSrc.m_bDepthWritable != DepthStencilDescDest.m_bDepthWritable)
	{

		SetRenderState(D3DRS_ZWRITEENABLE, DepthStencilDescDest.m_bDepthWritable);
	}


	if (DepthStencilDescSrc.m_bStencilEnable != DepthStencilDescDest.m_bStencilEnable)
	{

		SetRenderState(D3DRS_STENCILENABLE, DepthStencilDescDest.m_bStencilEnable);
	}


	if (DepthStencilDescSrc.m_uiStencilCompareMethod != DepthStencilDescDest.m_uiStencilCompareMethod)
	{

		SetRenderState(D3DRS_STENCILFUNC,
			ms_dwCompare[DepthStencilDescDest.m_uiStencilCompareMethod]);
	}


	if (DepthStencilDescSrc.m_uiCCW_StencilCompareMethod != DepthStencilDescDest.m_uiCCW_StencilCompareMethod)
	{

		SetRenderState(D3DRS_CCW_STENCILFUNC,
			ms_dwCompare[DepthStencilDescDest.m_uiCCW_StencilCompareMethod]);
	}

	if (DepthStencilDescSrc.m_uiReference != DepthStencilDescDest.m_uiReference)
	{

		SetRenderState(D3DRS_STENCILREF,
			DepthStencilDescDest.m_uiReference);
	}

	if (DepthStencilDescSrc.m_uiMask != DepthStencilDescDest.m_uiMask)
	{
		SetRenderState(D3DRS_STENCILMASK,
			DepthStencilDescDest.m_uiMask);
	}

	if (DepthStencilDescSrc.m_uiWriteMask != DepthStencilDescDest.m_uiWriteMask)
	{
		SetRenderState(D3DRS_STENCILWRITEMASK,
			DepthStencilDescDest.m_uiWriteMask);
	}

	if (DepthStencilDescSrc.m_uiSFailZPassOP != DepthStencilDescDest.m_uiSFailZPassOP)
	{
		SetRenderState(D3DRS_STENCILFAIL,
			ms_dwStencilOperation[DepthStencilDescDest.m_uiSFailZPassOP]);
	}

	if (DepthStencilDescSrc.m_uiSPassZFailOP != DepthStencilDescDest.m_uiSPassZFailOP)
	{
		SetRenderState(D3DRS_STENCILZFAIL,
			ms_dwStencilOperation[DepthStencilDescDest.m_uiSPassZFailOP]);
	}

	if (DepthStencilDescSrc.m_uiSPassZPassOP != DepthStencilDescDest.m_uiSPassZPassOP)
	{
		SetRenderState(D3DRS_STENCILPASS,
			ms_dwStencilOperation[DepthStencilDescDest.m_uiSPassZPassOP]);
	}

	if (DepthStencilDescSrc.m_uiCCW_SFailZPassOP != DepthStencilDescDest.m_uiCCW_SFailZPassOP)
	{
		SetRenderState(D3DRS_CCW_STENCILFAIL,
			ms_dwStencilOperation[DepthStencilDescDest.m_uiCCW_SFailZPassOP]);
	}

	if (DepthStencilDescSrc.m_uiCCW_SPassZFailOP != DepthStencilDescDest.m_uiCCW_SPassZFailOP)
	{
		SetRenderState(D3DRS_CCW_STENCILZFAIL,
			ms_dwStencilOperation[DepthStencilDescDest.m_uiCCW_SPassZFailOP]);
	}

	if (DepthStencilDescSrc.m_uiCCW_SPassZPassOP != DepthStencilDescDest.m_uiCCW_SPassZPassOP)
	{
		SetRenderState(D3DRS_CCW_STENCILPASS,
			ms_dwStencilOperation[DepthStencilDescDest.m_uiCCW_SPassZPassOP]);
	}
	if (DepthStencilDescSrc.m_bTwoSideStencilMode != DepthStencilDescDest.m_bTwoSideStencilMode)
	{
		SetRenderState(D3DRS_TWOSIDEDSTENCILMODE,
			DepthStencilDescDest.m_bTwoSideStencilMode);
	}

	m_pDepthStencilState = pDepthStencilState;
	return 1;
}
unsigned int VSDX9Renderer::SetRasterizerState(VSRasterizerState * pRasterizerState, bool bForceSet)
{
	unsigned int FRI = VSRenderer::SetRasterizerState(pRasterizerState, bForceSet);
	if (FRI == FRI_SAMERESOURCE)
		return FRI_SUCCESS;
	VSMAC_ASSERT(FRI == FRI_SUCCESS);
	if (FRI == FRI_FAIL)
	{
		return FRI_FAIL;
	}
	if (!pRasterizerState)
	{
		pRasterizerState = (VSRasterizerState *)VSRasterizerState::GetDefault();
	}
	const VSRasterizerDesc & RasterizerStateDest = pRasterizerState->GetRasterizerDesc();
	const VSRasterizerDesc & RasterizerStateScr = m_pRasterizerState->GetRasterizerDesc();
	if (RasterizerStateScr.m_fDepthBias != RasterizerStateDest.m_fDepthBias)
	{
		SetRenderState(D3DRS_DEPTHBIAS, *(DWORD *)&RasterizerStateDest.m_fDepthBias);
	}
	if (RasterizerStateScr.m_bClipPlaneEnable != RasterizerStateDest.m_bClipPlaneEnable)
	{
		if (!RasterizerStateDest.m_bClipPlaneEnable)
		{
			SetRenderState(D3DRS_CLIPPLANEENABLE, 0);
		}
	}
	if (RasterizerStateScr.m_bScissorTestEnable != RasterizerStateDest.m_bScissorTestEnable)
	{
		SetRenderState(D3DRS_SCISSORTESTENABLE, RasterizerStateDest.m_bScissorTestEnable);
	}
	if (RasterizerStateScr.m_bWireEnable != RasterizerStateDest.m_bWireEnable)
	{
		if (RasterizerStateDest.m_bWireEnable)
		{
			SetRenderState(D3DRS_FILLMODE,
				D3DFILL_WIREFRAME);
		}
		else
		{
			SetRenderState(D3DRS_FILLMODE,
				D3DFILL_SOLID);
		}
	}
	if (RasterizerStateScr.m_uiCullType != RasterizerStateDest.m_uiCullType)
	{
		SetRenderState(D3DRS_CULLMODE,
			ms_dwCullType[RasterizerStateDest.m_uiCullType]);
	}
	m_pRasterizerState = pRasterizerState;
	return 1;
}
void VSDX9Renderer::SetClipPlane(VSArray<VSPlane3> &Plane, bool bEnable)
{
	if (bEnable)
	{
		for (unsigned int i = 0; i < Plane.GetNum(); i++)
		{
			SetRenderState(D3DRS_CLIPPLANEENABLE, 1 << i);
			VSPlane3 PlaneTemp;
			Plane[i].GetReverse(PlaneTemp);
			SetClipPlane(i, (VSREAL *)&PlaneTemp);
		}
	}
	else
	{
		SetRenderState(D3DRS_CLIPPLANEENABLE, 0);
	}

}
void VSDX9Renderer::SetScissorRect(VSArray<VSRect2> &Rect, bool bEnable)
{
	if (bEnable)
	{
		RECT Temp;
		Temp.left = (LONG)Rect[0].m_Min.x;
		Temp.top = (LONG)Rect[0].m_Min.y;
		Temp.right = (LONG)Rect[0].m_Max.x;
		Temp.bottom = (LONG)Rect[0].m_Max.y;
		SetScissorRect(&Temp);
	}
	else
	{
		SetScissorRect(NULL);
	}
}
void VSDX9Renderer::ClearBackBuffer()
{

	Clear(0, 0, D3DCLEAR_TARGET, m_ClearColor.GetDWARGB(), 1.0f, 0);
}
void VSDX9Renderer::ClearZBuffer()
{
	Clear(0, 0, D3DCLEAR_ZBUFFER, 0, m_fClearDepth, 0);
}
void VSDX9Renderer::ClearStencilBuffer()
{
	Clear(0, 0, D3DCLEAR_STENCIL, 0, 1.0f, (DWORD)m_uiClearStencil);
}
void VSDX9Renderer::ClearBuffers(unsigned int uiClearFlag)
{
	if (!uiClearFlag)
	{
		return;
	}
	DWORD Flag = 0;
	if (uiClearFlag & CF_COLOR)
	{
		Flag |= D3DCLEAR_TARGET;
	}
	if (uiClearFlag & CF_DEPTH)
	{
		Flag |= D3DCLEAR_ZBUFFER;
	}
	if (uiClearFlag & CF_STENCIL)
	{
		Flag |= D3DCLEAR_STENCIL;
	}
	Clear(0, 0,
		Flag,
		m_ClearColor.GetDWARGB(), m_fClearDepth, (DWORD)m_uiClearStencil);

}
void VSDX9Renderer::ClearBuffers()
{
	Clear(0, 0,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		m_ClearColor.GetDWARGB(), m_fClearDepth, (DWORD)m_uiClearStencil);
}
void VSDX9Renderer::ClearBackBuffer(int iXPos, int iYPos, int iWidth, int iHeight)
{
	D3DRECT kRect;
	kRect.x1 = (long)iXPos;
	kRect.y1 = (long)iYPos;
	kRect.x2 = (long)(iXPos + iWidth - 1);
	kRect.y2 = (long)(iYPos + iHeight - 1);


	Clear(1, &kRect, D3DCLEAR_TARGET, m_ClearColor.GetDWARGB(),
		1.0f, 0);

}
void VSDX9Renderer::ClearZBuffer(int iXPos, int iYPos, int iWidth, int iHeight)
{
	D3DRECT kRect;
	kRect.x1 = (long)iXPos;
	kRect.y1 = (long)iYPos;
	kRect.x2 = (long)(iXPos + iWidth - 1);
	kRect.y2 = (long)(iYPos + iHeight - 1);

	Clear(1, &kRect, D3DCLEAR_ZBUFFER, 0,
		m_fClearDepth, 0);

}
void VSDX9Renderer::ClearStencilBuffer(int iXPos, int iYPos, int iWidth, int iHeight)
{
	D3DRECT kRect;
	kRect.x1 = (long)iXPos;
	kRect.y1 = (long)iYPos;
	kRect.x2 = (long)(iXPos + iWidth - 1);
	kRect.y2 = (long)(iYPos + iHeight - 1);

	Clear(1, &kRect, D3DCLEAR_STENCIL, 0, 1.0f,
		(DWORD)m_uiClearStencil);
}
void VSDX9Renderer::ClearBuffers(unsigned int uiClearFlag, int iXPos, int iYPos, int iWidth,
	int iHeight)
{
	if (!uiClearFlag)
	{
		return;
	}
	DWORD Flag = 0;
	if (uiClearFlag & CF_COLOR)
	{
		Flag |= D3DCLEAR_TARGET;
	}
	if (uiClearFlag & CF_DEPTH)
	{
		Flag |= D3DCLEAR_ZBUFFER;
	}
	if (uiClearFlag & CF_STENCIL)
	{
		Flag |= D3DCLEAR_STENCIL;
	}
	D3DRECT kRect;
	kRect.x1 = (long)iXPos;
	kRect.y1 = (long)iYPos;
	kRect.x2 = (long)(iXPos + iWidth - 1);
	kRect.y2 = (long)(iYPos + iHeight - 1);


	Clear(1, &kRect,
		Flag,
		m_ClearColor.GetDWARGB(), m_fClearDepth, (DWORD)m_uiClearStencil);
}
void VSDX9Renderer::ClearBuffers(int iXPos, int iYPos, int iWidth, int iHeight)
{
	D3DRECT kRect;
	kRect.x1 = (long)iXPos;
	kRect.y1 = (long)iYPos;
	kRect.x2 = (long)(iXPos + iWidth - 1);
	kRect.y2 = (long)(iYPos + iHeight - 1);


	Clear(1, &kRect,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		m_ClearColor.GetDWARGB(), m_fClearDepth, (DWORD)m_uiClearStencil);

}
void VSDX9Renderer::GetShaderInstruction(ID3DXBuffer * pShader, unsigned int &uiArithmeticInstructionSlots, unsigned int &uiTextureInstructionSlots)
{
	HRESULT hResult = NULL;
	ID3DXBuffer* DisassemblyBuffer;
	hResult = D3DXDisassembleShader((const DWORD*)pShader->GetBufferPointer(), FALSE, NULL, &DisassemblyBuffer);
	VSMAC_ASSERT(!FAILED(hResult));

	VSString TempCache = (TCHAR*)DisassemblyBuffer->GetBufferPointer();

	VSMAC_RELEASE(DisassemblyBuffer);

	TCHAR TempBuffer[VSString::BUFFER_SIZE];

	int iIndex = TempCache.GetSubStringIndex(_T("approximately"), -1);

	VSString Useful = &TempCache[iIndex];

	iIndex = Useful.GetSubStringIndex(_T("instruction"), 1);

	int iInstructionStringNum = iIndex - VSStrLen(_T("approximately"));

	VSMemcpy(TempBuffer, Useful.GetBuffer() + VSStrLen(_T("approximately")), iInstructionStringNum);

	TempBuffer[iInstructionStringNum] = _T('\0');

	VSString InstructionString = TempBuffer;

	uiArithmeticInstructionSlots = StringToInt(InstructionString);
	unsigned int TextureIndex = Useful.GetSubStringIndex(_T("texture"), 1);

	if (TextureIndex != -1)
	{
		iIndex = Useful.GetSubStringIndex(_T("("), 1);

		VSMemcpy(TempBuffer, Useful.GetBuffer() + iIndex + 1, TextureIndex - iIndex - 1);
		TempBuffer[TextureIndex - iIndex - 1] = _T('\0');
		VSString TextureString = TempBuffer;
		uiTextureInstructionSlots = StringToInt(TextureString);
		uiArithmeticInstructionSlots -= uiTextureInstructionSlots;

	}

}
void VSDX9Renderer::OnLoadVShaderFromString(VSVShader * pVShaderProgram, VSVProgramID *pID)
{
	HRESULT hResult = NULL;
	LPD3DXBUFFER pCode = NULL;
	LPD3DXBUFFER pErrors = NULL;
	DWORD Flags = NULL;
#if _DEBUG 
	Flags = D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION;
#endif
	VSMap<VSString, VSString> Define;
	pVShaderProgram->m_ShaderKey.GetDefine(Define);
	D3DXMACRO * pMacro = GetDefine(Define);
	LPD3DXCONSTANTTABLE          pConstantTable = NULL; //常量表对象
	if (pVShaderProgram->GetBuffer().GetLength())
	{
		hResult = D3DXCompileShader((LPCSTR)pVShaderProgram->GetBuffer().GetBuffer(), pVShaderProgram->GetBuffer().GetLength(), pMacro, ms_pDx9IncludeShader, pVShaderProgram->GetMainFunName().GetBuffer(),
			ms_cVertexShaderProgramVersion, Flags, &pCode, &pErrors, &pConstantTable);

		pVShaderProgram->ClearBuffer();
	}
	else if (pVShaderProgram->GetFileName().GetLength())
	{
		VSString RenderAPIPre = VSRenderer::GetRenderTypeShaderPath(RAT_DIRECTX9);
		VSString Path = VSShaderMapCache::ms_ResourcePath + RenderAPIPre + pVShaderProgram->GetFileName().GetString();
		hResult = D3DXCompileShaderFromFile((LPCSTR)Path.GetBuffer(), pMacro, ms_pDx9IncludeShader, pVShaderProgram->GetMainFunName().GetBuffer(),
			ms_cVertexShaderProgramVersion, Flags, &pCode, &pErrors, &pConstantTable);
	}
	else
		VSMAC_ASSERT(0);

	VSMAC_DELETEA(pMacro);
#if _DEBUG 
	if (pErrors && pErrors->GetBufferPointer())
	{
		VSLog Error;
		VSString VSErrorString = VSConfig::ms_OutputLogPath + _T("VSError.txt");
		Error.Open(VSErrorString.GetBuffer());
		Error.WriteInfo(pVShaderProgram->GetFileName().GetBuffer());
		Error.WriteInfo("\n");
		Error.WriteInfo((const TCHAR *)pErrors->GetBufferPointer());
		VSOutputDebugString(pVShaderProgram->GetFileName().GetBuffer());
		Error.WriteInfo("\n");
		VSOutputDebugString((const TCHAR *)pErrors->GetBufferPointer());
	}
#endif
	VSMAC_ASSERT(!FAILED(hResult));
	VSMAC_ASSERT(pCode);
	VSMAC_ASSERT(pConstantTable);

	if (!pVShaderProgram->HasBindResource())
	{
		D3DXCONSTANTTABLE_DESC ConstantTableDesc;
		hResult = pConstantTable->GetDesc(&ConstantTableDesc);
		VSMAC_ASSERT(!FAILED(hResult));

		for (unsigned int uiConstantIndex = 0; uiConstantIndex < ConstantTableDesc.Constants; uiConstantIndex++)
		{
			// Read the constant description.
			D3DXHANDLE ConstantHandle = pConstantTable->GetConstant(NULL, uiConstantIndex);
			D3DXCONSTANT_DESC ConstantDesc;
			unsigned int NumConstants = 1;
			pConstantTable->GetConstantDesc(ConstantHandle, &ConstantDesc, &NumConstants);
			if (ConstantDesc.RegisterSet != D3DXRS_SAMPLER)
			{

				unsigned int uiValueType = 0;
				if (ConstantDesc.RegisterSet == D3DXRS_BOOL)
				{
					uiValueType = VSUserConstant::VT_BOOL;
				}
				else if (ConstantDesc.RegisterSet == D3DXRS_INT4)
				{
					uiValueType = VSUserConstant::VT_INT;
				}
				else if (ConstantDesc.RegisterSet == D3DXRS_FLOAT4)
				{
					uiValueType = VSUserConstant::VT_FLOAT;
				}
				else
				{
					VSMAC_ASSERT(0);
				}
				VSUserConstant * pUserConstant =
					VS_NEW VSUserConstant(ConstantDesc.Name,ConstantDesc.Bytes, ConstantDesc.RegisterIndex, ConstantDesc.RegisterCount, uiValueType);
				pVShaderProgram->m_pUserConstant.AddElement(pUserConstant);

			}
			else
			{
				unsigned int uiType = 0;
				if (ConstantDesc.Type == D3DXPT_SAMPLER1D)
				{
					uiType = VSTexture::TT_1D;
				}
				else if (ConstantDesc.Type == D3DXPT_SAMPLER2D)
				{
					uiType = VSTexture::TT_2D;
				}
				else if (ConstantDesc.Type == D3DXPT_SAMPLER3D)
				{
					uiType = VSTexture::TT_3D;
				}
				else if (ConstantDesc.Type == D3DXPT_SAMPLERCUBE)
				{
					uiType = VSTexture::TT_CUBE;
				}
				else
				{
					VSMAC_ASSERT(0);
				}
				VSUserSampler * pUerSampler = VS_NEW VSUserSampler(ConstantDesc.Name, uiType, ConstantDesc.RegisterIndex, ConstantDesc.RegisterCount);
				pVShaderProgram->m_pUserSampler.AddElement(pUerSampler);
			}
		}
		VSMAC_RELEASE(pConstantTable);
		pVShaderProgram->m_bCreatePara = true;
	}
	hResult = m_pDevice->CreateVertexShader((DWORD*)pCode->GetBufferPointer(), &pID->m_pVertexShader);
	VSMAC_ASSERT(!FAILED(hResult));
	pVShaderProgram->SetCacheBuffer(pCode->GetBufferPointer(), pCode->GetBufferSize());
	GetShaderInstruction(pCode, pVShaderProgram->m_uiArithmeticInstructionSlots, pVShaderProgram->m_uiTextureInstructionSlots);
	VSMAC_RELEASE(pCode);

	VSMAC_RELEASE(pErrors);
}
bool VSDX9Renderer::OnLoadVShaderProgram(VSVShader * pVShaderProgram, VSResourceIdentifier *&pID)
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
		// 		HRESULT hResult = NULL;
		// 		hResult = m_pDevice->CreateVertexShader((DWORD*)pVShaderProgram->GetCacheBuffer(),&pVProgramID->m_pVertexShader );
		// 		VSMAC_ASSERT(!FAILED(hResult));
		CreateVShaderFromCache(pVShaderProgram, pVProgramID);
	}

	return 1;
}
void VSDX9Renderer::OnLoadPShaderFromString(VSPShader* pPShaderProgram, VSPProgramID *pID)
{
	HRESULT hResult = NULL;
	LPD3DXBUFFER pCode = NULL;
	LPD3DXBUFFER pErrors = NULL;
	DWORD Flags = NULL;
#if _DEBUG 
	Flags = D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION;
#endif
	LPD3DXCONSTANTTABLE      pConstantTable; //常量表对象
	VSMap<VSString, VSString> Define;
	pPShaderProgram->m_ShaderKey.GetDefine(Define);
	D3DXMACRO * pMacro = GetDefine(Define);
	if (pPShaderProgram->GetBuffer().GetLength())
	{
		hResult = D3DXCompileShader((LPCSTR)pPShaderProgram->GetBuffer().GetBuffer(), pPShaderProgram->GetBuffer().GetLength(), pMacro, ms_pDx9IncludeShader, pPShaderProgram->GetMainFunName().GetBuffer(),
			ms_cPixelShaderProgramVersion, Flags, &pCode, &pErrors, &pConstantTable);

		pPShaderProgram->ClearBuffer();
	}
	else if (pPShaderProgram->GetFileName().GetLength())
	{
		VSString RenderAPIPre = VSRenderer::GetRenderTypeShaderPath(RAT_DIRECTX9);
		VSString Path = VSShaderMapCache::ms_ResourcePath + RenderAPIPre + pPShaderProgram->GetFileName().GetString();
		hResult = D3DXCompileShaderFromFile((LPCSTR)Path.GetBuffer(), pMacro, ms_pDx9IncludeShader, pPShaderProgram->GetMainFunName().GetBuffer(),
			ms_cPixelShaderProgramVersion, Flags, &pCode, &pErrors, &pConstantTable);
	}
	else
	{
		VSMAC_ASSERT(0);
	}
	VSMAC_DELETEA(pMacro);
#if _DEBUG 
	if (pErrors && pErrors->GetBufferPointer())
	{
		VSLog Error;
		VSString PSErrorString = VSConfig::ms_OutputLogPath + _T("PSError.txt");
		Error.Open(PSErrorString.GetBuffer());
		Error.WriteInfo(pPShaderProgram->GetFileName().GetBuffer());
		Error.WriteInfo("\n");
		Error.WriteInfo((const TCHAR *)pErrors->GetBufferPointer());
		VSOutputDebugString(pPShaderProgram->GetFileName().GetBuffer());
		Error.WriteInfo("\n");
		VSOutputDebugString((const TCHAR *)pErrors->GetBufferPointer());
	}
#endif
	VSMAC_ASSERT(!FAILED(hResult));
	VSMAC_ASSERT(pCode);
	VSMAC_ASSERT(pConstantTable);
	if (!pPShaderProgram->HasBindResource())
	{
		D3DXCONSTANTTABLE_DESC ConstantTableDesc;
		hResult = pConstantTable->GetDesc(&ConstantTableDesc);
		VSMAC_ASSERT(!FAILED(hResult));
		for (unsigned int uiConstantIndex = 0; uiConstantIndex < ConstantTableDesc.Constants; uiConstantIndex++)
		{
			// Read the constant description.
			D3DXHANDLE ConstantHandle = pConstantTable->GetConstant(NULL, uiConstantIndex);
			D3DXCONSTANT_DESC ConstantDesc;
			unsigned int NumConstants = 1;
			pConstantTable->GetConstantDesc(ConstantHandle, &ConstantDesc, &NumConstants);


			if (ConstantDesc.RegisterSet != D3DXRS_SAMPLER)
			{

				unsigned int uiValueType = 0;
				if (ConstantDesc.RegisterSet == D3DXRS_BOOL)
				{
					uiValueType = VSUserConstant::VT_BOOL;
				}
				else if (ConstantDesc.RegisterSet == D3DXRS_INT4)
				{
					uiValueType = VSUserConstant::VT_INT;
				}
				else if (ConstantDesc.RegisterSet == D3DXRS_FLOAT4)
				{
					uiValueType = VSUserConstant::VT_FLOAT;
				}
				else
				{
					VSMAC_ASSERT(0);
				}
				VSUserConstant * pUserConstant =
					VS_NEW VSUserConstant(ConstantDesc.Name,ConstantDesc.Bytes, ConstantDesc.RegisterIndex, ConstantDesc.RegisterCount, uiValueType);
				pPShaderProgram->m_pUserConstant.AddElement(pUserConstant);

			}
			else
			{
				unsigned int uiType = 0;
				if (ConstantDesc.Type == D3DXPT_SAMPLER1D)
				{
					uiType = VSTexture::TT_1D;
				}
				else if (ConstantDesc.Type == D3DXPT_SAMPLER2D)
				{
					uiType = VSTexture::TT_2D;
				}
				else if (ConstantDesc.Type == D3DXPT_SAMPLER3D)
				{
					uiType = VSTexture::TT_3D;
				}
				else if (ConstantDesc.Type == D3DXPT_SAMPLERCUBE)
				{
					uiType = VSTexture::TT_CUBE;
				}
				else
				{
					VSMAC_ASSERT(0);
				}
				VSUserSampler * pUerSampler = VS_NEW VSUserSampler(ConstantDesc.Name, uiType, ConstantDesc.RegisterIndex, ConstantDesc.RegisterCount);
				pPShaderProgram->m_pUserSampler.AddElement(pUerSampler);
			}
		}
		VSMAC_RELEASE(pConstantTable)
			pPShaderProgram->m_bCreatePara = true;
	}

	hResult = m_pDevice->CreatePixelShader((DWORD*)pCode->GetBufferPointer(), &pID->m_pPixelShader);
	pPShaderProgram->SetCacheBuffer(pCode->GetBufferPointer(), pCode->GetBufferSize());
	VSMAC_ASSERT(!FAILED(hResult));
	GetShaderInstruction(pCode, pPShaderProgram->m_uiArithmeticInstructionSlots, pPShaderProgram->m_uiTextureInstructionSlots);
	VSMAC_RELEASE(pCode);
	VSMAC_RELEASE(pErrors);
}
bool VSDX9Renderer::OnLoadPShaderProgram(VSPShader* pPShaderProgram, VSResourceIdentifier *&pID)
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
		// 		HRESULT hResult = NULL;
		// 		hResult = m_pDevice->CreatePixelShader((DWORD*)pPShaderProgram->GetCacheBuffer(),&pPProgramID->m_pPixelShader);
		// 		VSMAC_ASSERT(!FAILED(hResult));
		CreatePShaderFromCache(pPShaderProgram, pPProgramID);
	}

	return 1;

}
bool VSDX9Renderer::OnLoadGShaderProgram(VSGShader * pGShaderProgram, VSResourceIdentifier *&pID)
{
	return 1;
}
bool VSDX9Renderer::OnLoadDShaderProgram(VSDShader * pDShaderProgram, VSResourceIdentifier *&pID)
{
	return 1;
}
bool VSDX9Renderer::OnLoadHShaderProgram(VSHShader * pHShaderProgram, VSResourceIdentifier *&pID)
{
	return 1;
}
bool VSDX9Renderer::OnLoadCShaderProgram(VSCShader * pCShaderProgram, VSResourceIdentifier *&pID)
{
	return 1;
}
bool VSDX9Renderer::OnLoadTexture(VSTexture * pTexture, VSResourceIdentifier *&pID)
{

	VSTextureID * pTextureID = NULL;
	pTextureID = VS_NEW VSTextureID;
	if (!pTextureID)
		return 0;
	pID = pTextureID;
	DWORD dwUsage = 0;
	D3DPOOL Pool;
	DWORD LockFlag;
	if (pTexture->GetOutputType() == VSOutputResource::OT_RENDER_TARGET)
	{
		Pool = D3DPOOL_DEFAULT;
		dwUsage |= D3DUSAGE_RENDERTARGET;
		LockFlag = D3DLOCK_DISCARD;
	}
	else if (pTexture->GetOutputType() == VSOutputResource::OT_DEPTH_STENCIL)
	{
		VSMAC_ASSERT(0);
	}
	else
	{
		if (!pTexture->IsStatic())
		{
			dwUsage |= D3DUSAGE_DYNAMIC;
			Pool = D3DPOOL_DEFAULT;
			LockFlag = D3DLOCK_DISCARD;
			//dwUsage |= D3DUSAGE_WRITEONLY;
		}
		else
		{
			Pool = D3DPOOL_MANAGED;
			LockFlag = 0;

		}
	}



	if (pTexture->GetTexType() == VSTexture::TT_2D)
	{
		//		D3DLOCKED_RECT LockRect;
		// 		LPDIRECT3DTEXTURE9 pDXTexture;
		// 
		// 		
		// 		hResult = D3DXCreateTexture(m_pDevice,pTexture->GetWidth(0),
		// 			pTexture->GetHeight(0),pTexture->GetMipLevel(),dwUsage,(D3DFORMAT)ms_dwTextureFormatType[pTexture->GetFormatType()],Pool,&pDXTexture);
		// 		VSMAC_ASSERT(!FAILED(hResult));
		//		pTextureID->m_pTexture =  pDXTexture;


		// 		if(!((VS2DTexture *)pTexture)->IsRenderTarget())
		// 		{
		// 			for (unsigned int i = 0 ; i < pTexture->GetMipLevel() ; i++)
		// 			{
		// 				if (!pTexture->GetBuffer(i))
		// 				{
		// 					continue;
		// 				}
		// 				hResult = pDXTexture->LockRect(i,&LockRect,0,LockFlag);
		// 				VSMAC_ASSERT(!FAILED(hResult));
		// 				VSMemcpy(LockRect.pBits,pTexture->GetBuffer(i),pTexture->GetByteSize(i));
		// 				hResult = pDXTexture->UnlockRect(i);
		// 				VSMAC_ASSERT(!FAILED(hResult));
		// 			}		
		// 			
		// 
		// 		}
		Create2DTexture(pTexture, dwUsage, (D3DFORMAT)ms_dwTextureFormatType[pTexture->GetFormatType()], Pool, LockFlag, &pTextureID->m_pTexture);

	}
	else if (pTexture->GetTexType() == VSTexture::TT_3D)
	{
		// 		LPDIRECT3DVOLUMETEXTURE9 pDXTexture;
		// 		
		// 			
		// 		hResult = D3DXCreateVolumeTexture(m_pDevice,pTexture->GetWidth(0),
		// 						pTexture->GetHeight(0),pTexture->GetLength(0),pTexture->GetMipLevel(),dwUsage,
		// 						(D3DFORMAT)ms_dwTextureFormatType[pTexture->GetFormatType()],
		// 						Pool,&pDXTexture);
		// 		VSMAC_ASSERT(!FAILED(hResult));
		// 		pTextureID->m_pTexture =  pDXTexture;	


		// 		for (unsigned int i = 0 ; i < pTexture->GetMipLevel() ; i++)
		// 		{
		// 			if (!pTexture->GetBuffer(i))
		// 			{
		// 				continue;
		// 			}
		// 			D3DLOCKED_BOX kLockBox;
		// 			hResult = pDXTexture->LockBox(i,&kLockBox,0,LockFlag);
		// 			VSMAC_ASSERT(!FAILED(hResult));
		// 			VSMemcpy(kLockBox.pBits,pTexture->GetBuffer(i),pTexture->GetByteSize(i));
		// 			hResult = pDXTexture->UnlockBox(i);
		// 			VSMAC_ASSERT(!FAILED(hResult));
		// 		}



		CreateVolumeTexture(pTexture, dwUsage, (D3DFORMAT)ms_dwTextureFormatType[pTexture->GetFormatType()], Pool, LockFlag, &pTextureID->m_pTexture);

	}
	else if (pTexture->GetTexType() == VSTexture::TT_CUBE)
	{
		//		D3DLOCKED_RECT LockRect;
		// 		LPDIRECT3DCUBETEXTURE9 pDXTexture;
		// 
		// 			
		// 		hResult = D3DXCreateCubeTexture(m_pDevice,pTexture->GetWidth(0),
		// 			pTexture->GetMipLevel(),dwUsage,(D3DFORMAT)ms_dwTextureFormatType[pTexture->GetFormatType()],Pool,&pDXTexture);
		// 		VSMAC_ASSERT(!FAILED(hResult));
		// 		pTextureID->m_pTexture =  pDXTexture;


		// 		VSCubeTexture * pCubeTexture =DynamicCast<VSCubeTexture>(pTexture);
		// 		if(!pCubeTexture->IsRenderTarget())
		// 		{
		// 			for (unsigned int j = 0 ; j < pTexture->GetMipLevel() ; j++)
		// 			{
		// 				for(unsigned int i = 0 ; i < 6 ; i++)
		// 				{
		// 					if(!pCubeTexture->GetBuffer(j,i))
		// 						continue;
		// 					hResult = pDXTexture->LockRect((D3DCUBEMAP_FACES)ms_dwCubeMapFace[i],j,&LockRect,NULL,LockFlag);
		// 					VSMAC_ASSERT(!FAILED(hResult));
		// 					VSMemcpy(LockRect.pBits,pCubeTexture->GetBuffer(j,i),pCubeTexture->GetByteSize(j));
		// 					hResult = pDXTexture->UnlockRect((D3DCUBEMAP_FACES)ms_dwCubeMapFace[i],j);
		// 					VSMAC_ASSERT(!FAILED(hResult));
		// 				}
		// 			}
		// 		}
		CreateCubeTexture(pTexture, dwUsage, (D3DFORMAT)ms_dwTextureFormatType[pTexture->GetFormatType()], Pool, LockFlag, &pTextureID->m_pTexture);


	}
	else if (pTexture->GetTexType() == VSTexture::TT_1D)
	{
		Create1DTexture(pTexture, dwUsage, (D3DFORMAT)ms_dwTextureFormatType[pTexture->GetFormatType()], Pool, LockFlag, &pTextureID->m_pTexture);
	}
	else
	{
		VSMAC_ASSERT(0);
	}

	return 1;
}
bool VSDX9Renderer::OnLoadRenderTarget(VSRenderTarget * pRenderTarget, VSResourceIdentifier *&pID)
{
	if (!pRenderTarget)
	{
		return false;
	}

	VSRenderTargetID *pRenderTargetID = VS_NEW VSRenderTargetID;
	pID = pRenderTargetID;
	VSTexture * pTexture = (VSTexture *)pRenderTarget->GetCreateBy();
	bool b1 = false;
	bool b2 = false;
	if (pTexture && pRenderTarget->GetMulSample() == VSRenderer::MS_NONE)
	{
		b1 = true;

	}
	else if (pTexture)
	{
		b1 = true;
		b2 = true;

	}
	else
	{
		b2 = true;
	}
	if (b1)
	{
		VSTextureID *pTextureID = (VSTextureID *)pTexture->GetIdentifier();
		if (!pTextureID)
		{
			return 0;
		}
		if (pTexture->GetTexType() == VSTexture::TT_2D)
		{
			GetSurfaceLevel((LPDIRECT3DTEXTURE9 *)&pTextureID->m_pTexture, pRenderTarget->GetLevel(), &pRenderTargetID->m_pTextureSurface);
			// 			HRESULT hResult = NULL;
			// 			hResult = ((LPDIRECT3DTEXTURE9)pTextureID->m_pTexture)->GetSurfaceLevel(pRenderTarget->GetLevel(),&pRenderTargetID->m_pTextureSurface);
			// 			VSMAC_ASSERT(!FAILED(hResult));
		}
		else if (pTexture->GetTexType() == VSTexture::TT_CUBE)
		{
			GetCubeMapSurface((LPDIRECT3DCUBETEXTURE9*)&pTextureID->m_pTexture, (D3DCUBEMAP_FACES)ms_dwCubeMapFace[pRenderTarget->GetFirst()], pRenderTarget->GetLevel(), &pRenderTargetID->m_pTextureSurface);
			// 			HRESULT hResult = NULL;
			// 			hResult = ((LPDIRECT3DCUBETEXTURE9)pTextureID->m_pTexture)->GetCubeMapSurface((D3DCUBEMAP_FACES)ms_dwCubeMapFace[pRenderTarget->GetParam()],pRenderTarget->GetLevel(),&pRenderTargetID->m_pTextureSurface);
			// 			VSMAC_ASSERT(!FAILED(hResult));
		}
		else
		{
			return false;
		}
		
	}
	if (b2)
	{
		CreateRenderTarget((D3DFORMAT)ms_dwTextureFormatType[pRenderTarget->GetFormatType()], (D3DMULTISAMPLE_TYPE)ms_dwMultiSampleTypes[GetSuitableMultisample(pRenderTarget->GetMulSample())],
			pRenderTarget->GetWidth(), pRenderTarget->GetHeight(),&pRenderTargetID->m_pRenderTarget);
		// 		HRESULT hResult = NULL;
		// 		DWORD uiMulSample;
		// 		hResult = ms_pMain->CheckDeviceMultiSampleType(m_uinAdapter,(D3DDEVTYPE)ms_dwDevTypes[m_uiDevType], 
		// 			(D3DFORMAT)ms_dwTextureFormatType[pRenderTarget->GetFormatType()],m_bWindowed,(D3DMULTISAMPLE_TYPE)ms_dwMultiSampleTypes[pRenderTarget->GetMulSample()], &uiMulSample);
		// 		if(FAILED(hResult))			
		// 		{
		// 			VSMAC_ASSERT(0);
		// 		}
		// 		hResult = m_pDevice->CreateRenderTarget(pRenderTarget->GetWidth(),pRenderTarget->GetHeight(),
		// 			(D3DFORMAT)ms_dwTextureFormatType[pRenderTarget->GetFormatType()],(D3DMULTISAMPLE_TYPE)ms_dwMultiSampleTypes[pRenderTarget->GetMulSample()],
		// 			uiMulSample - 1,false,&pRenderTargetID->m_pRenderTarget,NULL);
		// 		VSMAC_ASSERT(!FAILED(hResult));
		if (pRenderTarget->GetLockFlag() == VSInheritBind::LF_READONLY)
		{
			VSMAC_ASSERT(0);
		}

	}
	if (pTexture || pRenderTarget->GetMulSample() == VSRenderer::MS_NONE)
	{
		if (pRenderTarget->GetLockFlag() == VSInheritBind::LF_READONLY)
		{
			Create2DStagTexture(pRenderTarget->GetWidth(), pRenderTarget->GetHeight(), (D3DFORMAT)ms_dwTextureFormatType[pRenderTarget->GetFormatType()], &pRenderTargetID->m_pStagTexture);
			GetSurfaceLevel(&pRenderTargetID->m_pStagTexture, 0, &pRenderTargetID->m_pStagSurface);
		}
	}

	return true;
}
bool VSDX9Renderer::OnLoadDepthStencil(VSDepthStencil * pDepthStencil, VSResourceIdentifier *&pID)
{
	VSDepthStencilID  * pDepthStencilID = NULL;
	pDepthStencilID = VS_NEW VSDepthStencilID;
	if (!pDepthStencilID)
		return 0;
	pID = pDepthStencilID;
	CreateDepthStencilSurface((D3DFORMAT)ms_dwTextureFormatType[pDepthStencil->GetFormatType()], (D3DMULTISAMPLE_TYPE)ms_dwMultiSampleTypes[GetSuitableMultisample(pDepthStencil->GetMulSample())],
		pDepthStencil->GetWidth(), pDepthStencil->GetHeight(), &pDepthStencilID->m_pDepthStencilBuffer);
	return 1;

}
bool VSDX9Renderer::OnLoadVBufferData(VSVertexBuffer * pVBuffer, VSResourceIdentifier *&pID)
{

	VSVBufferID * pVBufferID = NULL;
	pVBufferID = VS_NEW VSVBufferID;
	if (!pVBufferID)
		return 0;
	pID = pVBufferID;


	unsigned int uiOneVertexSize = pVBuffer->GetOneVertexSize();

	unsigned int uiTotalSize = uiOneVertexSize * pVBuffer->GetVertexNum();

	DWORD dwUsage = 0;
	D3DPOOL Pool = D3DPOOL_DEFAULT;
	DWORD LockFlag = 0;
	if (!pVBuffer->IsStatic())
	{
		dwUsage |= D3DUSAGE_DYNAMIC;
		dwUsage |= D3DUSAGE_WRITEONLY;
		LockFlag = D3DLOCK_DISCARD;
	}
	else
	{
		dwUsage |= D3DUSAGE_WRITEONLY;
		Pool = D3DPOOL_MANAGED;
	}

	CreateVertexBuffer(uiTotalSize, dwUsage, Pool, &pVBufferID->m_pVertexBuffer, LockFlag, pVBuffer, uiOneVertexSize);

	return 1;

}
bool VSDX9Renderer::OnLoadVBufferFormat(VSVertexFormat * pVertexFormat, VSResourceIdentifier *&pID)
{
	if (!pVertexFormat->m_FormatArray.GetNum())
		return 0;
	VSVBufferFormatID * pVBufferFormatID = NULL;
	pVBufferFormatID = VS_NEW VSVBufferFormatID;
	if (!pVBufferFormatID)
		return 0;
	pID = pVBufferFormatID;

	VSArray<D3DVERTEXELEMENT9> Elements;
	D3DVERTEXELEMENT9 Element;
	
	Element.Method = D3DDECLMETHOD_DEFAULT;
	for (unsigned int i = 0; i < pVertexFormat->m_FormatArray.GetNum(); i++)
	{
		VSVertexFormat::VERTEXFORMAT_TYPE &Format = pVertexFormat->m_FormatArray[i];
		Element.Stream = Format.Stream;
		Element.Offset = Format.Offset;
		Element.Type = (BYTE)ms_dwVertexFormatDataType[Format.DataType];
		Element.Usage = (BYTE)ms_dwVertexFormatSemantics[Format.Semantics];
		Element.UsageIndex = Format.SemanticsIndex;
		Elements.AddElement(Element);
	}

	D3DVERTEXELEMENT9 kSentinel = D3DDECL_END();
	Elements.AddElement(kSentinel);
	CreateVertexDeclaration(Elements, &pVBufferFormatID->m_pDeclaration);
	return 1;
}
bool VSDX9Renderer::OnLoadUnorderAccess(VSTextureUnorderAccess  * pUnorderAccess, VSResourceIdentifier *&pID)
{
	return 1;
}
bool VSDX9Renderer::OnLoadUnorderAccess(VSBufferUnorderAccess  * pUnorderAccess, VSResourceIdentifier *&pID)
{
	return 1;
}
bool VSDX9Renderer::OnLoadSBuffer(VSBufferResource* pSBuffer, VSResourceIdentifier *&pID)
{
	return 1;
}
bool VSDX9Renderer::OnLoadIBuffer(VSIndexBuffer* pIBuffer, VSResourceIdentifier *&pID)
{
	VSIBufferID * pIBufferID = NULL;
	pIBufferID = VS_NEW VSIBufferID;
	if (!pIBufferID)
		return 0;
	pID = pIBufferID;
	HRESULT hResult = NULL;
	D3DFORMAT DataFormat;
	if (pIBuffer->GetDataType() == VSDataBuffer::DT_USHORT)
		DataFormat = D3DFMT_INDEX16;
	else if (pIBuffer->GetDataType() == VSDataBuffer::DT_UINT)
		DataFormat = D3DFMT_INDEX32;
	else
		return 0;
	DWORD dwUsage = 0;
	D3DPOOL Pool = D3DPOOL_DEFAULT;
	DWORD LockFlag = 0;

	if (!pIBuffer->IsStatic())
	{
		dwUsage |= D3DUSAGE_DYNAMIC;
		dwUsage |= D3DUSAGE_WRITEONLY;
		LockFlag = D3DLOCK_DISCARD;
	}
	else
	{
		dwUsage |= D3DUSAGE_WRITEONLY;
		Pool = D3DPOOL_MANAGED;
	}
	void* pBuffer = NULL;
	if (pIBuffer->GetIndexData())
	{
		pBuffer = pIBuffer->GetIndexData()->GetData();
	}
	CreateIndexBuffer(pIBuffer->GetByteSize(), dwUsage, DataFormat, Pool, &pIBufferID->m_IndexBuffer, pBuffer, LockFlag);
	return 1;
}
bool VSDX9Renderer::OnLoadBlendState(VSBlendState * pBlendState, VSResourceIdentifier *&pID)
{
	return 1;
}
bool VSDX9Renderer::OnLoadDepthStencilState(VSDepthStencilState * pDepthStencilState, VSResourceIdentifier *&pID)
{
	return 1;
}
bool VSDX9Renderer::OnLoadRasterizerState(VSRasterizerState * pRasterizerState, VSResourceIdentifier *&pID)
{

	return 1;
}
bool VSDX9Renderer::GetData(VSQuery * pQuery, void * pData, unsigned int uiSize)
{
	if (!pQuery)
	{
		return FALSE;
	}
	if (!pQuery->IsBindResource())
	{
		return FALSE;
	}
	VSQueryID *pQueryID = (VSQueryID *)pQuery->GetIdentifier();
	if (!pQueryID)
		return FALSE;
	LPDIRECT3DQUERY9 pD3DQuery = pQueryID->m_Query;
	HRESULT Result = pD3DQuery->GetData(pData, uiSize, D3DGETDATA_FLUSH);

	if (Result == S_FALSE)
	{
		do
		{
			Result = pD3DQuery->GetData(pData, uiSize, D3DGETDATA_FLUSH);

		} while (Result == S_FALSE);
	}

	if (Result == S_OK)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
bool VSDX9Renderer::OnLoadQuery(VSQuery * pQuery, VSResourceIdentifier *&pID)
{
	VSQueryID * pQueryID = NULL;
	pQueryID = VS_NEW VSQueryID;
	if (!pQueryID)
		return 0;
	pID = pQueryID;
	CreateQuery((D3DQUERYTYPE)ms_dwQueryType[pQuery->GetQueryType()], pQueryID);
	return 1;
}
bool VSDX9Renderer::OnLoadSamplerState(VSSamplerState * pSamplerState, VSResourceIdentifier *&pID)
{
	return 1;
}
bool VSDX9Renderer::SetVShaderProgram(VSVShader*	pVShaderProgram)
{
	if (!VSRenderer::SetVShaderProgram(pVShaderProgram))
	{
		SetVertexShader(NULL);
		return 0;
	}

	VSVProgramID *pVProgramID = (VSVProgramID *)pVShaderProgram->GetIdentifier();
	if (!pVProgramID)
		return FRI_FAIL;
	SetVertexShader(&pVProgramID->m_pVertexShader);
	return 1;
}
bool VSDX9Renderer::SetGShaderProgram(VSGShader*pGShaderProgram)
{
	return 1;
}
bool VSDX9Renderer::SetDShaderProgram(VSDShader*	pDShaderProgram)
{
	return 1;
}
bool VSDX9Renderer::SetHShaderProgram(VSHShader*	pHShaderProgram)
{
	return 1;
}
bool VSDX9Renderer::SetCShaderProgram(VSCShader*	pCShaderProgram)
{
	return 1;
}
bool VSDX9Renderer::SetPShaderProgram(VSPShader*pPShaderProgram)
{
	if (!VSRenderer::SetPShaderProgram(pPShaderProgram))
	{
		SetPixelShader(NULL);
		return 0;
	}
	VSPProgramID *pPProgramID = (VSPProgramID *)pPShaderProgram->GetIdentifier();
	if (!pPProgramID)
		return FRI_FAIL;
	SetPixelShader(&pPProgramID->m_pPixelShader);
	return 1;
}
unsigned int VSDX9Renderer::SetGTexture(VSTexture* pTexture, unsigned int i)
{
	VSRenderer::SetGTexture(pTexture, i);
	return 1;
}
unsigned int VSDX9Renderer::SetDTexture(VSTexture* pTexture, unsigned int i)
{
	VSRenderer::SetDTexture(pTexture, i);
	return 1;
}
unsigned int VSDX9Renderer::SetHTexture(VSTexture* pTexture, unsigned int i)
{
	VSRenderer::SetHTexture(pTexture, i);
	return 1;
}
unsigned int VSDX9Renderer::SetCTexture(VSTexture* pTexture, unsigned int i)
{
	VSRenderer::SetCTexture(pTexture, i);
	return 1;
}
unsigned int VSDX9Renderer::SetCBufferResource(VSBufferResource* pBufferResource, unsigned int i)
{
	VSRenderer::SetCBufferResource(pBufferResource, i);
	return 1;
}
unsigned int VSDX9Renderer::SetVBufferResource(VSBufferResource* pBufferResource, unsigned int i)
{
	VSRenderer::SetVBufferResource(pBufferResource, i);
	return 1;
}
unsigned int VSDX9Renderer::SetPBufferResource(VSBufferResource* pBufferResource, unsigned int i)
{
	VSRenderer::SetPBufferResource(pBufferResource, i);
	return 1;
}
unsigned int VSDX9Renderer::SetGBufferResource(VSBufferResource* pBufferResource, unsigned int i)
{
	VSRenderer::SetGBufferResource(pBufferResource, i);
	return 1;
}
unsigned int VSDX9Renderer::SetHBufferResource(VSBufferResource* pBufferResource, unsigned int i)
{
	VSRenderer::SetHBufferResource(pBufferResource, i);
	return 1;
}
unsigned int VSDX9Renderer::SetDBufferResource(VSBufferResource* pBufferResource, unsigned int i)
{
	VSRenderer::SetDBufferResource(pBufferResource, i);
	return 1;
}
void VSDX9Renderer::Dispath(unsigned int uiThreadGroupCountX, unsigned int uiThreadGroupCountY,
	unsigned int uiThreadGroupCountZ) 
{
	VSRenderer::Dispath(uiThreadGroupCountX,uiThreadGroupCountY,uiThreadGroupCountZ);
	return;
}
void VSDX9Renderer::EnablenInstanceRender(unsigned int uiInstanceNum)
{

	m_bInstanceRenderMode = true;
	SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | uiInstanceNum);
	SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1ul);
}
void VSDX9Renderer::DisableInstanceRender()
{
	if (!m_bInstanceRenderMode)
	{
		return;
	}
	m_bInstanceRenderMode = false;
	SetStreamSourceFreq(0, 1);
	SetStreamSourceFreq(1, 1);
}
unsigned int VSDX9Renderer::SetNormalMesh(VSGeometry *pGeometry)
{
	if (VSRenderer::SetNormalMesh(pGeometry) == FRI_FAIL)
	{
		DisableInstanceRender();
		return FRI_FAIL;
	}
	else
	{
		DisableInstanceRender();
		return FRI_SUCCESS;
	}
}
unsigned int VSDX9Renderer::SetInstanceMesh(VSInstanceGeometry *pInstanceGeometry)
{
	if (VSRenderer::SetInstanceMesh(pInstanceGeometry) == FRI_FAIL)
	{
		DisableInstanceRender();
		return FRI_FAIL;
	}
	else
	{
		EnablenInstanceRender(pInstanceGeometry->GetInstanceNum());
		return FRI_SUCCESS;
	}
}
unsigned int VSDX9Renderer::SetVTexture(VSTexture* pTexture, unsigned int i)
{
	unsigned int FRI = VSRenderer::SetVTexture(pTexture, i);
	if (FRI == FRI_SAMERESOURCE)
		return 1;
	else if (FRI == FRI_FAIL)
	{
		SetTexture(D3DDMAPSAMPLER + 1 + i, NULL);
		return 0;
	}
	VSTextureID *pTextureID = (VSTextureID *)pTexture->GetIdentifier();
	if (!pTextureID)
		return 0;


	SetTexture(D3DDMAPSAMPLER + 1 + i, &pTextureID->m_pTexture);
	return 1;

}
unsigned int VSDX9Renderer::SetPTexture(VSTexture* pTexture, unsigned int i)
{
	unsigned int FRI = VSRenderer::SetPTexture(pTexture, i);
	if (FRI == FRI_SAMERESOURCE)
		return 1;
	else if (FRI == FRI_FAIL)
	{
		SetTexture(i, NULL);
		return 0;
	}
	VSTextureID *pTextureID = (VSTextureID *)pTexture->GetIdentifier();
	if (!pTextureID)
		return 0;


	SetTexture(i, &pTextureID->m_pTexture);

	return 1;

}
void VSDX9Renderer::SetGShaderConstant(VSGShader * pShader)
{

}
void VSDX9Renderer::SetDShaderConstant(VSDShader * pShader)
{

}
void VSDX9Renderer::SetHShaderConstant(VSHShader * pShader)
{

}
void VSDX9Renderer::SetCShaderConstant(VSCShader * pShader)
{

}
void VSDX9Renderer::SetVShaderConstant(VSVShader * pShader)
{
	unsigned int uiFloatRegisterID = 0;
	unsigned int uiBoolRegisterID = 0;
	unsigned int uiIntRegisterID = 0;
	if (!pShader->m_bCreatePara)
	{
		return;
	}
	for (unsigned int i = 0; i < pShader->m_pUserConstant.GetNum(); i++)
	{
		VSUserConstant * pUserConstant = pShader->m_pUserConstant[i];

		if (pUserConstant->GetValueType() == VSUserConstant::VT_FLOAT)
		{
			VSMemcpy(&m_fFloatShaderBuffer[pUserConstant->GetRegisterIndex() << 2],
				pUserConstant->GetData(), sizeof(VSREAL)* (pUserConstant->GetRegisterNum() << 2));
			uiFloatRegisterID = uiFloatRegisterID + pUserConstant->GetRegisterNum();
		}
		if (pUserConstant->GetValueType() == VSUserConstant::VT_INT)
		{
			VSMemcpy(&m_iIntShaderBuffer[pUserConstant->GetRegisterIndex() << 2],
				pUserConstant->GetData(), sizeof(int)* (pUserConstant->GetRegisterNum() << 2));
			uiIntRegisterID = uiIntRegisterID + pUserConstant->GetRegisterNum();
		}
		if (pUserConstant->GetValueType() == VSUserConstant::VT_BOOL)
		{
			VSMemcpy(&m_bBoolShaderBuffer[pUserConstant->GetRegisterIndex() << 2],
				pUserConstant->GetData(), sizeof(bool)* (pUserConstant->GetRegisterNum() << 2));
			uiBoolRegisterID = uiBoolRegisterID + pUserConstant->GetRegisterNum();
		}

	}
	if (uiFloatRegisterID)
	{
		SetVProgramConstant(0, m_fFloatShaderBuffer,
			uiFloatRegisterID, VSUserConstant::VT_FLOAT);
	}
	if (uiIntRegisterID)
	{
		SetVProgramConstant(0, m_iIntShaderBuffer,
			uiIntRegisterID, VSUserConstant::VT_INT);
	}
	if (uiBoolRegisterID)
	{
		SetVProgramConstant(0, m_bBoolShaderBuffer,
			uiBoolRegisterID, VSUserConstant::VT_BOOL);
	}
}
void VSDX9Renderer::SetPShaderConstant(VSPShader * pShader)
{
	unsigned int uiFloatRegisterID = 0;
	unsigned int uiBoolRegisterID = 0;
	unsigned int uiIntRegisterID = 0;
	if (!pShader->m_bCreatePara)
	{
		return;
	}
	for (unsigned int i = 0; i < pShader->m_pUserConstant.GetNum(); i++)
	{
		VSUserConstant * pUserConstant = pShader->m_pUserConstant[i];

		if (pUserConstant->GetValueType() == VSUserConstant::VT_FLOAT)
		{
			VSMemcpy(&m_fFloatShaderBuffer[pUserConstant->GetRegisterIndex() << 2],
				pUserConstant->GetData(), sizeof(VSREAL)* (pUserConstant->GetRegisterNum() << 2));
			uiFloatRegisterID = uiFloatRegisterID + pUserConstant->GetRegisterNum();
		}
		if (pUserConstant->GetValueType() == VSUserConstant::VT_INT)
		{
			VSMemcpy(&m_iIntShaderBuffer[pUserConstant->GetRegisterIndex() << 2],
				pUserConstant->GetData(), sizeof(int)* (pUserConstant->GetRegisterNum() << 2));
			uiIntRegisterID = uiIntRegisterID + pUserConstant->GetRegisterNum();
		}
		if (pUserConstant->GetValueType() == VSUserConstant::VT_BOOL)
		{
			VSMemcpy(&m_bBoolShaderBuffer[pUserConstant->GetRegisterIndex() << 2],
				pUserConstant->GetData(), sizeof(bool)* (pUserConstant->GetRegisterNum() << 2));
			uiBoolRegisterID = uiBoolRegisterID + pUserConstant->GetRegisterNum();
		}

	}
	if (uiFloatRegisterID)
	{
		SetPProgramConstant(0, m_fFloatShaderBuffer,
			uiFloatRegisterID, VSUserConstant::VT_FLOAT);
	}
	if (uiIntRegisterID)
	{
		SetPProgramConstant(0, m_iIntShaderBuffer,
			uiIntRegisterID, VSUserConstant::VT_INT);
	}
	if (uiBoolRegisterID)
	{
		SetPProgramConstant(0, m_bBoolShaderBuffer,
			uiBoolRegisterID, VSUserConstant::VT_BOOL);
	}
}
void VSDX9Renderer::SetPTexAllState(VSTexAllState * pTexAllState, unsigned int i)
{
	VSRenderer::SetPTexAllState(pTexAllState, i);
	if (pTexAllState)
	{
		if (m_bSRGB[i] != pTexAllState->GetSRGB())
		{
			SetSamplerState(i, D3DSAMP_SRGBTEXTURE, pTexAllState->GetSRGB());
			m_bSRGB[i] = pTexAllState->GetSRGB();
		}

	}
	else
	{
		if (m_bSRGB[i])
		{
			m_bSRGB[i] = false;
			SetSamplerState(i, D3DSAMP_SRGBTEXTURE, 0);
		}

	}
}
D3DXMACRO * VSDX9Renderer::GetDefine(VSMap<VSString, VSString> & Define)
{
	if (Define.GetNum())
	{
		D3DXMACRO * pMacro = VS_NEW D3DXMACRO[Define.GetNum() + 1];
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
unsigned int VSDX9Renderer::SetVBuffer(VSVertexBuffer * pVBuffer, unsigned int uiStream)
{
	unsigned int FRI = VSRenderer::SetVBuffer(pVBuffer, uiStream);
	if (FRI == FRI_SAMERESOURCE)
		return 1;
	else if (FRI == FRI_FAIL)
	{
		SetStreamSource(uiStream, NULL, 0, 0);
		return 0;
	}
	VSVBufferID* pVBufferID = (VSVBufferID *)pVBuffer->GetIdentifier();
	if (!pVBufferID)
		return 0;

	SetStreamSource(uiStream, &pVBufferID->m_pVertexBuffer, 0,
		pVBuffer->GetOneVertexSize());
	return 1;
}
unsigned int VSDX9Renderer::SetVertexFormat(VSVertexFormat * pVertexFormat)
{
	unsigned int FRI = VSRenderer::SetVertexFormat(pVertexFormat);
	if (FRI == FRI_SAMERESOURCE)
		return 1;
	else if (FRI == FRI_FAIL)
	{
		SetVertexDeclaration(NULL);
		return 0;
	}

	VSVBufferFormatID * pVBufferFormatID = (VSVBufferFormatID *)pVertexFormat->GetIdentifier();
	if (!pVBufferFormatID)
		return 0;

	SetVertexDeclaration(&pVBufferFormatID->m_pDeclaration);

	return 1;
}
unsigned int VSDX9Renderer::BeginQuery(VSQuery * pQuery)
{
	if (!VSRenderer::BeginQuery(pQuery))
	{
		return 0;
	}
	VSQueryID * pQueryID = (VSQueryID *)pQuery->GetIdentifier();
	if (!pQueryID)
		return 0;
	BeginQuery(&pQueryID->m_Query);
	return 1;
}
unsigned int VSDX9Renderer::EndQuery(VSQuery * pQuery)
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
	EndQuery(&pQueryID->m_Query);
	return 1;
}
unsigned int VSDX9Renderer::SetIBuffer(VSIndexBuffer* pIBuffer)
{
	unsigned int FRI = VSRenderer::SetIBuffer(pIBuffer);
	if (FRI == FRI_SAMERESOURCE)
		return 1;
	else if (FRI == FRI_FAIL)
	{
		SetIndices(NULL);	//可以设置索引为空 modify 2010.10.16
		return 1;
	}
	VSIBufferID* pIBufferID = (VSIBufferID *)pIBuffer->GetIdentifier();
	if (!pIBufferID)
		return 0;
	SetIndices(&pIBufferID->m_IndexBuffer);
	return 1;
}
bool VSDX9Renderer::CopyResourceBuffer(VS2DTexture * pSource, VSCubeTexture * pDest, unsigned int uiFace)
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
		RECT Rect = { 0, 0, (LONG)(pSource->GetWidth(i) - 1), (LONG)(pSource->GetHeight(i) - 1) };

		CopyResourceBuffer(i, ((LPDIRECT3DTEXTURE9 *)&pTextureID->m_pTexture), ((LPDIRECT3DCUBETEXTURE9 *)&pCubTextureID->m_pTexture), &Rect, (D3DTEXTUREFILTERTYPE)ms_dwTextureFilterType[VSSamplerDesc::FM_LINE], (D3DCUBEMAP_FACES)ms_dwCubeMapFace[uiFace]);

	}
	return true;
}
bool VSDX9Renderer::SetRenderTarget(VSRenderTarget *pRenderTarget, unsigned int i)
{

	if (!VSRenderer::SetRenderTarget(pRenderTarget, i))
	{
		if (!pRenderTarget && i > 0 && i < m_uiMaxRTNum)
		{
			SetRenderTarget(i, NULL);
		}
		return 0;
	}
	VSRenderTargetID *pRenderTargetID = (VSRenderTargetID *)pRenderTarget->GetIdentifier();
	if (!pRenderTargetID)
	{
		return 0;
	}

	if (pRenderTarget->GetMulSample() == MS_NONE && pRenderTarget->GetCreateBy())
	{
		SetRenderTarget(i, &pRenderTargetID->m_pTextureSurface, &pRenderTargetID->m_pSaveRenderTarget);
	}
	else
	{
		SetRenderTarget(i, &pRenderTargetID->m_pRenderTarget, &pRenderTargetID->m_pSaveRenderTarget);
	}

	return 1;
}
bool VSDX9Renderer::EndRenderTarget(VSRenderTarget *pRenderTarget, unsigned int i)
{
	if (!VSRenderer::EndRenderTarget(pRenderTarget, i))
		return 0;
	VSRenderTargetID *pRenderTargetID = (VSRenderTargetID *)pRenderTarget->GetIdentifier();
	if (!pRenderTargetID)
		return 0;

	VSTexture * pTexture = (VSTexture *)pRenderTarget->GetCreateBy();
	if (pRenderTarget->GetMulSample() != MS_NONE && pTexture)
	{

		RECT SourceRect = { 0, 0, (LONG)(pRenderTarget->GetWidth() - 1), (LONG)(pRenderTarget->GetHeight() - 1) };
		RECT DestRect = { 0, 0, (LONG)(pTexture->GetWidth(pRenderTarget->GetLevel()) - 1), (LONG)(pTexture->GetHeight(pRenderTarget->GetLevel()) - 1) };
		StretchRect(&pRenderTargetID->m_pRenderTarget, &SourceRect,
			&pRenderTargetID->m_pTextureSurface, &DestRect, (D3DTEXTUREFILTERTYPE)ms_dwTextureFilterType[VSSamplerDesc::FM_LINE]);

	}
	
	if (pRenderTarget->GetLockFlag() == VSInheritBind::LF_READONLY && pRenderTargetID->m_pStagTexture)
	{
		if (pTexture)
		{
			GetRenderTargetData(&pRenderTargetID->m_pTextureSurface, &pRenderTargetID->m_pStagSurface);
		}
		else if (pRenderTarget->GetMulSample() == MS_NONE)
		{
			GetRenderTargetData(&pRenderTargetID->m_pRenderTarget, &pRenderTargetID->m_pStagSurface);
		}
	}

	EndRenderTarget(i, &pRenderTargetID->m_pSaveRenderTarget);
	// 	hResult = m_pDevice->SetRenderTarget(i,pRenderTargetID->m_pSaveRenderTarget);
	// 	VSMAC_ASSERT(!FAILED(hResult));
	// 	VSMAC_RELEASE(pRenderTargetID->m_pSaveRenderTarget);

	return 1;
}
bool VSDX9Renderer::SetDepthStencilBuffer(VSDepthStencil * pDepthStencilBuffer)
{
	if (!VSRenderer::SetDepthStencilBuffer(pDepthStencilBuffer))
	{
		SetDepthStencilSurface(NULL);
		return 0;
	}
	VSDepthStencilID *pDepthStencilID = (VSDepthStencilID *)pDepthStencilBuffer->GetIdentifier();
	if (!pDepthStencilID)
	{
		return 0;
	}
	SetDepthStencilSurface(&pDepthStencilID->m_pDepthStencilBuffer, &pDepthStencilID->m_pSaveDepthStencilBuffer);
	// 	VSMAC_RELEASE(pDepthStencilID->m_pSaveDepthStencilBuffer)
	// 	hResult = m_pDevice->GetDepthStencilSurface(&pDepthStencilID->m_pSaveDepthStencilBuffer);
	// 	VSMAC_ASSERT(!FAILED(hResult));
	// 	hResult = m_pDevice->SetDepthStencilSurface(pDepthStencilID->m_pDepthStencilBuffer);
	// 	VSMAC_ASSERT(!FAILED(hResult));
	return 1;

}
bool VSDX9Renderer::EndDepthStencilBuffer(VSDepthStencil * pDepthStencilBuffer)
{
	if (!VSRenderer::EndDepthStencilBuffer(pDepthStencilBuffer))
		return 0;
	VSDepthStencilID *pDepthStencilID = (VSDepthStencilID *)pDepthStencilBuffer->GetIdentifier();
	if (!pDepthStencilID)
		return 0;
	// 	if(!pDepthStencilID->m_pSaveDepthStencilBuffer)
	// 		return 0;
	EndDepthStencilSurface(&pDepthStencilID->m_pSaveDepthStencilBuffer);
	// 	hResult = m_pDevice->SetDepthStencilSurface(pDepthStencilID->m_pSaveDepthStencilBuffer);
	// 	VSMAC_ASSERT(!FAILED(hResult));
	// 	VSMAC_RELEASE(pDepthStencilID->m_pSaveDepthStencilBuffer)
	return 1;
}
unsigned int VSDX9Renderer::GetTextureCompressLen(unsigned int uiLen)
{
	return Max((unsigned int)1, (unsigned int)(uiLen / 4));
}
bool VSDX9Renderer::SetVProgramConstant(unsigned int uiStartRegister, void * pData,
	unsigned int RegisterNum, unsigned int uiType)
{
	SetVertexShaderConstant(uiStartRegister, pData, RegisterNum, uiType);
	// 	if(uiType == VSUserConstant::VT_BOOL)
	// 	{
	// 		hResult = m_pDevice->SetVertexShaderConstantB(uiStartRegister,(const BOOL *)pData,RegisterNum);
	// 		VSMAC_ASSERT(!FAILED(hResult));
	// 	}
	// 	else if(uiType == VSUserConstant::VT_FLOAT)
	// 	{
	// 		hResult = m_pDevice->SetVertexShaderConstantF(uiStartRegister,(const VSREAL *)pData,RegisterNum);
	// 		VSMAC_ASSERT(!FAILED(hResult));
	// 	}
	// 	else if(uiType == VSUserConstant::VT_INT)
	// 	{
	// 		hResult = m_pDevice->SetVertexShaderConstantI(uiStartRegister,(const int *)pData,RegisterNum);
	// 		VSMAC_ASSERT(!FAILED(hResult));
	// 	}
	// 	else 
	// 	{
	// 		VSMAC_ASSERT(0);
	// 	}
	return 1;
}
bool VSDX9Renderer::SetPProgramConstant(unsigned int uiStartRegister, void * pData,
	unsigned int RegisterNum, unsigned int uiType)
{
	SetPixelShaderConstant(uiStartRegister, pData, RegisterNum, uiType);
	// 	if(uiType == VSUserConstant::VT_BOOL)
	// 	{
	// 		hResult = m_pDevice->SetPixelShaderConstantB(uiStartRegister,(const BOOL *)pData,RegisterNum);
	// 		VSMAC_ASSERT(!FAILED(hResult));
	// 	}
	// 	else if(uiType == VSUserConstant::VT_FLOAT)
	// 	{
	// 		hResult = m_pDevice->SetPixelShaderConstantF(uiStartRegister,(const VSREAL *)pData,RegisterNum);
	// 		VSMAC_ASSERT(!FAILED(hResult));
	// 	}
	// 	else if(uiType == VSUserConstant::VT_INT)
	// 	{
	// 		hResult = m_pDevice->SetPixelShaderConstantI(uiStartRegister,(const int *)pData,RegisterNum);
	// 		VSMAC_ASSERT(!FAILED(hResult));
	// 	}
	// 	else 
	// 	{
	// 		VSMAC_ASSERT(0);
	// 	}
	return 1;
}
bool VSDX9Renderer::DrawMesh(VSGeometry * pGeometry, VSRenderState * pRenderState, VSVShader * pVShader, 
	VSPShader * pPShader, VSGShader * pGShader, VSHShader * pHShader, VSDShader * pDShader)
{

	ADD_TIME_PROFILE(DrawMesh);
	if (!VSRenderer::DrawMesh(pGeometry, pRenderState, pVShader, pPShader, pGShader, pHShader,pDShader))
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

	return 1;
}
bool VSDX9Renderer::DrawScreen(SCREEN_QUAD_TYPE ScreenQuad[4])
{
	if (!VSRenderer::DrawScreen(ScreenQuad))
		return 0;
	DisableInstanceRender();
	if (!ScreenQuad)
	{
		ScreenQuad = ms_FullScreen;
	}
	DrawScreenEX1(ScreenQuad);
	return 1;
}
bool VSDX9Renderer::DrawScreenFont(SCREEN_FONT_TYPE * pScreenFontBuffer, unsigned int uiVertexNum,
	VSUSHORT_INDEX * pIndexBuffer, unsigned int uiIndexNum)
{
	if (!VSRenderer::DrawScreenFont(pScreenFontBuffer, uiVertexNum, pIndexBuffer, uiIndexNum))
	{
		return false;
	}
	DisableInstanceRender();
	unsigned int uiNumTri = uiIndexNum / 3;
	VSMAC_ASSERT(uiNumTri);
	if (!uiNumTri)
	{
		return false;
	}
	DrawScreenFontEX(pScreenFontBuffer, uiVertexNum, pIndexBuffer, uiNumTri);
	return true;
}
bool VSDX9Renderer::DrawScreen(SCREEN_QUAD_TYPE * pScreenBuffer, unsigned int uiVertexNum,
	VSUSHORT_INDEX * pIndexBuffer, unsigned int uiIndexNum)
{
	if (!VSRenderer::DrawScreen(pScreenBuffer, uiVertexNum, pIndexBuffer, uiIndexNum))
	{
		return false;
	}
	unsigned int uiNumTri = uiIndexNum / 3;
	VSMAC_ASSERT(uiNumTri);
	if (!uiNumTri)
	{
		return false;
	}
	DrawScreenEX2(pScreenBuffer, uiVertexNum, pIndexBuffer, uiNumTri);
	// 	D3DFORMAT DataFormat;
	// 
	// 	DataFormat =  D3DFMT_INDEX16;
	// 
	// 	hResult = 
	// 		m_pDevice->DrawIndexedPrimitiveUP((D3DPRIMITIVETYPE)ms_dwPrimitiveType[VSMeshData::MDT_TRIANGLE],
	// 		0,uiVertexNum,uiNumTri,pIndexBuffer,DataFormat,pScreenBuffer,sizeof(SCREEN_QUAD_TYPE));
	// 	VSMAC_ASSERT(!FAILED(hResult));
	return true;
}
bool VSDX9Renderer::SetViewPort(VSViewPort *pViewPort)
{
	VSRenderer::SetViewPort(pViewPort);
	D3DVIEWPORT9 View;
	if (pViewPort)
	{
		VSViewPort ViewPort;
		ViewPort = *pViewPort;
		View.X = Rounding(ViewPort.XMin * m_uiCurRTWidth);
		View.Y = Rounding(ViewPort.YMin * m_uiCurRTHeight);
		View.Width = Rounding(ViewPort.XMax * m_uiCurRTWidth) - View.X;
		View.Height = Rounding(ViewPort.YMax * m_uiCurRTHeight) - View.Y;
		View.MinZ = ViewPort.ZMin;
		View.MaxZ = ViewPort.ZMax;
	}
	else
	{
		View.X = 0;
		View.Y = 0;
		View.Width = m_uiCurRTWidth;
		View.Height = m_uiCurRTHeight;
		View.MinZ = 0.0f;
		View.MaxZ = 1.0f;

	}



	SetViewport(&View);
	return true;
}
VSDX9Renderer::VSDX9Renderer(HWND hMainWindow, unsigned int uiScreenWidth, unsigned int uiScreenHeight, bool bIsWindowed,
	unsigned int uiAnisotropy, unsigned int uiMultisample,
	ChildWindowInfo * pChildWindow, int uiNumChildWindow)
{
	//是否有子窗口
	if (uiNumChildWindow > 0 && pChildWindow && bIsWindowed)
	{
		m_pChildWindowInfo = VS_NEW ChildWindowInfo[uiNumChildWindow];
		VSMemcpy(m_pChildWindowInfo, pChildWindow, sizeof(ChildWindowInfo)* uiNumChildWindow);
		m_iNumChildWindow = uiNumChildWindow;
	}
	else
	{
		uiNumChildWindow = 0;

	}

	m_hMainWindow = hMainWindow;
	m_bWindowed = bIsWindowed;
	m_uiScreenHeight = uiScreenHeight;
	m_uiScreenWidth = uiScreenWidth;
	m_uiCurRTWidth = uiScreenWidth;
	m_uiCurRTHeight = uiScreenHeight;
	VSMemset(&m_Present, 0, sizeof(m_Present));
	if (bIsWindowed)
	{
		m_Present.FullScreen_RefreshRateInHz = 0;
	}
	else
	{
		m_Present.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	}

	m_SupportFeature[SF_MulBufferSwtich] = true;
	m_SupportFeature[SF_GS] = false;
	m_SupportFeature[SF_MaterialValueInstance] = false;
	m_SupportFeature[SF_Tessellation] = false;
	m_SupportFeature[SF_MSBufferRead] = false;
	m_SupportFeature[SF_DepthStensilBufferRead] = false;
	m_SupportFeature[SF_DepthStensilToTexture] = false;
	m_SupportFeature[SF_BufferResource] = false;
	m_SupportFeature[SF_UnorderAccess] = false;
	m_SupportFeature[SF_CS] = false;
	m_SupportFeature[SF_ClearRange] = true;
	m_SupportFeature[SF_VertexIDInShader] = false;
	m_SupportFeature[SF_ShareSampler] = false;
	m_SupportFeature[SF_VShaderSampler] = false;
	m_SupportFeature[SF_AdvanceInstance] = false;
	m_SupportFeature[SF_NoPow2Texture] = false;

	m_uinAdapter = 0;
	m_uiDevType = 0;
	//取得显示模式
	D3DDISPLAYMODE d3dDisplayMode;
	HRESULT hResult = NULL;
	hResult = ms_pMain->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3dDisplayMode);
	VSMAC_ASSERT(!FAILED(hResult));

	D3DADAPTER_IDENTIFIER9 d3dAdapterIdentifier;
	hResult = ms_pMain->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0, &d3dAdapterIdentifier);
	VSMAC_ASSERT(!FAILED(hResult));
	m_AdapterName = d3dAdapterIdentifier.DeviceName;

	for (unsigned int i = 0; i < sizeof(ms_dwTextureFormatType) / sizeof(DWORD); i++)
	{
		if (d3dDisplayMode.Format == ms_dwTextureFormatType[i])
		{
			m_uiBufferFormat = i;
			break;
		}
	}
	m_uiDepthStencilFormat = SFT_D24S8;

	m_Present.Windowed = m_bWindowed;
	m_Present.BackBufferCount = 1;
	m_Present.BackBufferFormat = d3dDisplayMode.Format;
	m_Present.EnableAutoDepthStencil = TRUE;
	m_Present.AutoDepthStencilFormat = D3DFMT_D24S8;
	m_Present.SwapEffect = D3DSWAPEFFECT_DISCARD;


	m_Present.hDeviceWindow = m_hMainWindow;

	m_Present.BackBufferWidth = uiScreenWidth;
	m_Present.BackBufferHeight = uiScreenHeight;
	m_Present.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	m_Present.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;


	for (int uiMultiSampleTypes = MS_MAX - 1; uiMultiSampleTypes >= MS_NONE; uiMultiSampleTypes--)
	{
		hResult = ms_pMain->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
			D3DFMT_A8R8G8B8, bIsWindowed, (D3DMULTISAMPLE_TYPE)ms_dwMultiSampleTypes[uiMultiSampleTypes], NULL);
		if (SUCCEEDED(hResult))
		{
			m_uiMaxMultisample = uiMultiSampleTypes;
			break;
		}
	}

	if (uiMultisample > m_uiMaxMultisample)
	{
		m_Present.MultiSampleType = (D3DMULTISAMPLE_TYPE)ms_dwMultiSampleTypes[m_uiMaxMultisample];
		m_uiCurMultisample = m_uiMaxMultisample;
	}
	else
	{
		m_uiCurMultisample = uiMultisample;
		m_Present.MultiSampleType = (D3DMULTISAMPLE_TYPE)ms_dwMultiSampleTypes[m_uiCurMultisample];
	}

	m_uiCurRTMultisample = m_uiCurMultisample;
	m_dwMultisampleQuality = 0;
	hResult = ms_pMain->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		D3DFMT_A8R8G8B8, bIsWindowed, (D3DMULTISAMPLE_TYPE)ms_dwMultiSampleTypes[m_uiCurMultisample], &m_dwMultisampleQuality);
	VSMAC_ASSERT(!FAILED(hResult));

	m_Present.MultiSampleQuality = m_dwMultisampleQuality - 1;

	struct DeviceType
	{
		D3DDEVTYPE DevType;
		DWORD dwBehavior;
	};
	DeviceType device_type[] =
	{
		{ D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING },
		{ D3DDEVTYPE_REF, D3DCREATE_SOFTWARE_VERTEXPROCESSING }
	};
	unsigned int type;
	for (type = 0; type < DT_MAX; type++)
	{
		if (VSResourceManager::ms_bRenderThread)
		{
			device_type[type].dwBehavior |= D3DCREATE_MULTITHREADED;
		}
		hResult = ms_pMain->CreateDevice(D3DADAPTER_DEFAULT, device_type[type].DevType,
			m_hMainWindow, device_type[type].dwBehavior, &m_Present, &m_pDevice);
		if (SUCCEEDED(hResult))
		{
			m_uiDevType = type;
			break;
		}
	}
	VSMAC_ASSERT(type == 0);
	m_pMainChain = NULL;
	hResult = m_pDevice->GetSwapChain(0, &m_pMainChain);
	VSMAC_ASSERT(m_pMainChain);
	VSMAC_ASSERT(!FAILED(hResult));
	hResult = m_pDevice->GetDepthStencilSurface(&m_pMainDepthStencilBuffer);
	VSMAC_ASSERT(m_pMainDepthStencilBuffer);
	VSMAC_ASSERT(!FAILED(hResult));
	D3DCAPS9 d3dCaps;
	hResult = ms_pMain->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dCaps);
	VSMAC_ASSERT(d3dCaps.MaxSimultaneousTextures > 1);
	VSMAC_ASSERT(!FAILED(hResult));
	m_uiMaxAnisotropy = d3dCaps.MaxAnisotropy;
	if (uiAnisotropy > m_uiMaxAnisotropy)
		m_uiCurAnisotropy = m_uiMaxAnisotropy;
	else
	{
		m_uiCurAnisotropy = uiAnisotropy;
	}


	m_uiMaxBindResource[VSEngineFlag::ST_PIXEL] = 16;	
	m_uiMaxBindResource[VSEngineFlag::ST_VERTEX] = 4;
	m_uiMaxBindResource[VSEngineFlag::ST_GEOMETRY] = 0;
	m_uiMaxBindResource[VSEngineFlag::ST_DOMAIN] = 0;
	m_uiMaxBindResource[VSEngineFlag::ST_HULL] = 0;
	m_uiMaxBindResource[VSEngineFlag::ST_COMPUTE] = 0;

	m_uiMaxSampler[VSEngineFlag::ST_PIXEL] = 16;
	m_uiMaxSampler[VSEngineFlag::ST_VERTEX] = 4;
	m_uiMaxSampler[VSEngineFlag::ST_GEOMETRY] = 0;
	m_uiMaxSampler[VSEngineFlag::ST_DOMAIN] = 0;
	m_uiMaxSampler[VSEngineFlag::ST_HULL] = 0;
	m_uiMaxSampler[VSEngineFlag::ST_COMPUTE] = 0;

	m_uiMaxRTNum = d3dCaps.NumSimultaneousRTs;
	m_uiMaxTextureHeight = d3dCaps.MaxTextureHeight;
	m_uiMaxTextureWidth = d3dCaps.MaxTextureWidth;
	m_uiMaxUseClipPlane = d3dCaps.MaxUserClipPlanes;

	m_uiMaxUAVNum = 0;

	m_pChain = NULL;
	m_pChainnDepthStencilBuffer = NULL;
	if ((m_iNumChildWindow > 0) && m_bWindowed)
	{
		m_pChain = VS_NEW LPDIRECT3DSWAPCHAIN9[m_iNumChildWindow];
		m_pChainnDepthStencilBuffer = VS_NEW LPDIRECT3DSURFACE9[m_iNumChildWindow];
		for (int i = 0; i < m_iNumChildWindow; i++)
		{
			m_pChain[i] = NULL;
			m_pChainnDepthStencilBuffer[i] = NULL;
		}
		for (int i = 0; i < m_iNumChildWindow; i++)
		{
			m_Present.hDeviceWindow = m_pChildWindowInfo[i].m_hHwnd;
			m_Present.BackBufferWidth = m_pChildWindowInfo[i].m_uiWidth;
			m_Present.BackBufferHeight = m_pChildWindowInfo[i].m_uiHeight;
			hResult = m_pDevice->CreateAdditionalSwapChain(&m_Present, &m_pChain[i]);
			VSMAC_ASSERT(!FAILED(hResult));
			if (m_pChildWindowInfo[i].m_bDepth)
			{
				hResult = m_pDevice->CreateDepthStencilSurface(m_pChildWindowInfo[i].m_uiWidth, m_pChildWindowInfo[i].m_uiHeight, D3DFMT_D24S8,
					m_Present.MultiSampleType, m_Present.MultiSampleQuality, FALSE, &m_pChainnDepthStencilBuffer[i], NULL);
				VSMAC_ASSERT(!FAILED(hResult));
			}
		}
	}
	SetDefaultValue();
	m_bSRGB.SetBufferNum(m_uiMaxBindResource[VSEngineFlag::ST_PIXEL]);
	for (unsigned int i = 0; i < m_uiMaxBindResource[VSEngineFlag::ST_PIXEL]; i++)
	{
		m_bSRGB[i] = false;
	}
	m_bInstanceRenderMode = false;
	VSMemset(m_fFloatShaderBuffer, 0, sizeof(VSREAL)* MAX_SHADER_BUFFER);
	VSMemset(m_iIntShaderBuffer, 0, sizeof(int)* MAX_SHADER_BUFFER);
	VSMemset(m_bBoolShaderBuffer, 0, sizeof(bool)* MAX_SHADER_BUFFER);
}
VSDX9Renderer::~VSDX9Renderer()
{

	ReleaseDefaultValue();
	VSMAC_RELEASE(m_pMainDepthStencilBuffer);
	VSMAC_RELEASE(m_pMainChain);
	for (int i = 0; i < m_iNumChildWindow; i++)
	{
		VSMAC_RELEASE(m_pChainnDepthStencilBuffer[i]);
		VSMAC_RELEASE(m_pChain[i]);
	}
	VSMAC_DELETEA(m_pChain);
	VSMAC_DELETEA(m_pChainnDepthStencilBuffer);

	VSMAC_RELEASE(m_pDevice);
}
IMPLEMENT_PRIORITY(VSDX9Renderer)
IMPLEMENT_INITIAL_ONLY_BEGIN(VSDX9Renderer)
ADD_PRIORITY(VSResourceManager);
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState);
IMPLEMENT_INITIAL_ONLY_END
bool VSDX9Renderer::InitialDefaultState()
{
	VSMAC_RELEASE(ms_pMain);
	ms_pMain = Direct3DCreate9(D3D_SDK_VERSION);
	VSMAC_ASSERT(ms_pMain);
	ms_uiAdapterNum = ms_pMain->GetAdapterCount();
	D3DADAPTER_IDENTIFIER9 d3dAdapterIdentifier;
	D3DDISPLAYMODE d3ddspmd;
	D3DCAPS9 d3dCaps;
	HRESULT hResult = NULL;
	for (unsigned int nAdapter = 0; nAdapter < ms_uiAdapterNum; nAdapter++)
	{
		hResult = ms_pMain->GetAdapterIdentifier(nAdapter, 0, &d3dAdapterIdentifier);
		VSMAC_ASSERT(!FAILED(hResult));
		ms_AdapterInfo[nAdapter].AdapterName = d3dAdapterIdentifier.DeviceName;
		unsigned int DisplayModeNumA8R8G8B8 = 0;
		DisplayModeNumA8R8G8B8 = ms_pMain->GetAdapterModeCount(nAdapter, (D3DFORMAT)ms_dwTextureFormatType[VSRenderer::SFT_A8R8G8B8]);
		ms_AdapterInfo[nAdapter].uiDisplayModeNum = DisplayModeNumA8R8G8B8;
		ms_AdapterInfo[nAdapter].DisplayMode.Clear();
		ms_AdapterInfo[nAdapter].DisplayMode.SetBufferNum(DisplayModeNumA8R8G8B8);
		for (UINT nMode = 0; nMode < DisplayModeNumA8R8G8B8; nMode++)
		{
			TCHAR temp[20];
			hResult = ms_pMain->EnumAdapterModes(nAdapter, (D3DFORMAT)ms_dwTextureFormatType[VSRenderer::SFT_A8R8G8B8], nMode, &d3ddspmd);
			VSMAC_ASSERT(!FAILED(hResult));
			ms_AdapterInfo[nAdapter].DisplayMode[nMode].uiBitFormat = VSRenderer::SFT_A8R8G8B8;
			ms_AdapterInfo[nAdapter].DisplayMode[nMode].uiHeight = d3ddspmd.Height;
			ms_AdapterInfo[nAdapter].DisplayMode[nMode].uiWidth = d3ddspmd.Width;
			ms_AdapterInfo[nAdapter].DisplayMode[nMode].uiRefreshRate = d3ddspmd.RefreshRate;
			sprintf_s(temp, "%d * %d * 32", d3ddspmd.Width, d3ddspmd.Height);
			ms_AdapterInfo[nAdapter].DisplayMode[nMode].StringExpress = temp;
		}
		for (unsigned int DevType = 0; DevType < DT_MAX; DevType++)
		{
			hResult = ms_pMain->GetDeviceCaps(nAdapter, (D3DDEVTYPE)ms_dwDevTypes[DevType], &d3dCaps);
			VSMAC_ASSERT(!FAILED(hResult));
			ms_AdapterInfo[nAdapter].DeviceInfo[DevType].fMaxAnisotropy = d3dCaps.MaxAnisotropy;
			ms_AdapterInfo[nAdapter].DeviceInfo[DevType].PixelShaderVersion = d3dCaps.PixelShaderVersion;
			ms_AdapterInfo[nAdapter].DeviceInfo[DevType].uiMaxTexture = d3dCaps.MaxSimultaneousTextures;
			for (unsigned int uiMultiSampleTypes = MS_MAX - 1; uiMultiSampleTypes >= MS_NONE; uiMultiSampleTypes--)
			{
				hResult = ms_pMain->CheckDeviceMultiSampleType(nAdapter, (D3DDEVTYPE)ms_dwDevTypes[DevType],
					D3DFMT_A8R8G8B8, false, (D3DMULTISAMPLE_TYPE)ms_dwMultiSampleTypes[uiMultiSampleTypes], NULL);
				if (SUCCEEDED(hResult))
				{
					ms_AdapterInfo[nAdapter].DeviceInfo[DevType].uiMaxMultisample = uiMultiSampleTypes;
					break;
				}
			}
			ms_AdapterInfo[nAdapter].DeviceInfo[DevType].uiMaxTextureHeight = d3dCaps.MaxTextureHeight;
			ms_AdapterInfo[nAdapter].DeviceInfo[DevType].uiMaxTextureWidth = d3dCaps.MaxTextureWidth;
			ms_AdapterInfo[nAdapter].DeviceInfo[DevType].uiMaxUseClipPlane = d3dCaps.MaxUserClipPlanes;
			ms_AdapterInfo[nAdapter].DeviceInfo[DevType].VertexShaderVersion = d3dCaps.VertexShaderVersion;
			ms_AdapterInfo[nAdapter].DeviceInfo[DevType].uiMaxRTNum = d3dCaps.NumSimultaneousRTs;
		}
	}

	VSString RenderAPIPre = VSRenderer::GetRenderTypeShaderPath(RAT_DIRECTX9);
	VSFile IncludeShaderFile;
	VSString IncludeShaderPath = VSShaderMapCache::ms_ResourcePath + RenderAPIPre + _T("Shader.txt");
	if (!IncludeShaderFile.Open(IncludeShaderPath.GetBuffer(), VSFile::OM_RB))
	{
		return false;
	}
	unsigned int uiIncludeSize = IncludeShaderFile.GetFileSize();
	ms_IncludeShader.SetTCHARBufferNum(uiIncludeSize);
	if (!IncludeShaderFile.Read(ms_IncludeShader.GetBuffer(), uiIncludeSize, 1))
	{
		return false;
	}


	VSFile DynamicShaderFile;
	VSString DynamicShaderPath = VSShaderMapCache::ms_ResourcePath + RenderAPIPre + _T("DynamicShader.txt");
	if (!DynamicShaderFile.Open(DynamicShaderPath.GetBuffer(), VSFile::OM_RB))
	{
		return false;
	}
	unsigned int uiSize = DynamicShaderFile.GetFileSize();
	VSString VDynamicShaderString;
	VDynamicShaderString.SetTCHARBufferNum(uiSize);
	if (!DynamicShaderFile.Read(VDynamicShaderString.GetBuffer(), uiSize, 1))
	{
		return false;
	}
	unsigned int MaxSimpleInstanceAnim = MAX_SIMPLE_INSTANCE_ANIM;
	ms_DynamicShader.Format(VDynamicShaderString.GetBuffer(), VSResourceManager::GetGpuSkinBoneNum() * 3, MaxSimpleInstanceAnim);

	ms_pDx9IncludeShader = VS_NEW VSDx9ShaderInclude();
	VSMAC_ASSERT(ms_pDx9IncludeShader)
		return 1;
}
bool VSDX9Renderer::TerminalDefaultState()
{
	VSMAC_DELETE(ms_pDx9IncludeShader);
	VSMAC_RELEASE(ms_pMain);
	return 1;
}
bool VSDX9Renderer::UseWindow(int uiWindowID)
{
	if (!VSRenderer::UseWindow(uiWindowID))
		return 0;
	UseWindowEx(uiWindowID);
	// 	LPDIRECT3DSURFACE9 pBack=NULL;
	// 	if (uiWindowID == - 1)
	// 	{
	// 		hResult = m_pMainChain->GetBackBuffer(0,D3DBACKBUFFER_TYPE_MONO,&pBack);
	// 	}
	// 	else
	// 	{
	// 		hResult = m_pChain[uiWindowID]->GetBackBuffer(0,D3DBACKBUFFER_TYPE_MONO,&pBack);
	// 	}
	// 	
	// 	VSMAC_ASSERT(!FAILED(hResult));
	// 	m_pDevice->SetRenderTarget(0, pBack);
	// 	VSMAC_RELEASE(pBack);
	return 1;
}
bool VSDX9Renderer::CooperativeLevel()
{
	HRESULT hResult = NULL;
	hResult = m_pDevice->TestCooperativeLevel();

	if (hResult == D3DERR_DEVICELOST)
	{
		DeviceLost();
		return false;
	}

	if (hResult == D3DERR_DEVICENOTRESET)
	{

		ResetDevice();
	}
	return true;
}
bool VSDX9Renderer::BeginRendering()
{
	VSRenderer::BeginRendering();

	BeginRender();
	// 	hResult = m_pDevice->BeginScene();
	// 	VSMAC_ASSERT(!FAILED(hResult));
	return true;
}
bool VSDX9Renderer::EndRendering()
{
	VSRenderer::EndRendering();
	EndRender();
	// 	m_pDevice->EndScene();
	// 	if (m_bWindowed && (m_iNumChildWindow > 0)) 
	// 	{
	// 		hResult = m_pChain[m_iCurWindowID]->Present(NULL, NULL, NULL, NULL, 0);
	// 		VSMAC_ASSERT(!FAILED(hResult));	
	// 	}
	// 	else 
	// 	{
	// 		hResult = m_pMainChain->Present(NULL, NULL, NULL, NULL,0);
	// 		VSMAC_ASSERT(!FAILED(hResult));	
	// 	}
	return true;
}
void VSDX9Renderer::DeviceLost()
{
	VSMAC_RELEASE(m_pMainChain);
	VSMAC_RELEASE(m_pMainDepthStencilBuffer);
	for (int i = 0; i < m_iNumChildWindow; i++)
	{
		VSMAC_RELEASE(m_pChain[i]);
		VSMAC_RELEASE(m_pChainnDepthStencilBuffer[i]);
	}
	for (unsigned int i = 0; i < VSBind::ms_BindArray.GetNum(); i++)
	{
		VSTexture * pTexture = DynamicCast<VSTexture>(VSBind::ms_BindArray[i]);
		if (pTexture)
		{
			if (pTexture->GetOutputType()!= VSOutputResource::OT_NONE || !pTexture->IsStatic())
			{
				if (!pTexture->ReleaseResource())
				{
					VSMAC_ASSERT(0);
				}
			}
		}

		VSDepthStencil * pDepthStencilBuffer = DynamicCast<VSDepthStencil>(VSBind::ms_BindArray[i]);
		if (pDepthStencilBuffer)
		{
			if (!pDepthStencilBuffer->ReleaseResource())
			{
				VSMAC_ASSERT(0);
			}
		}
		VSRenderTarget * pRenderTarget = DynamicCast<VSRenderTarget>(VSBind::ms_BindArray[i]);
		if (pRenderTarget)
		{
			if (!pRenderTarget->ReleaseResource())
			{
				VSMAC_ASSERT(0);
			}
		}
		VSQuery * pQuery = DynamicCast<VSQuery>(VSBind::ms_BindArray[i]);
		if (pQuery)
		{
			if (!pQuery->ReleaseResource())
			{
				VSMAC_ASSERT(0);
			}
		}
	}

}
void VSDX9Renderer::ResetDevice()
{

	HRESULT hResult = NULL;
	m_Present.BackBufferWidth = m_uiScreenWidth;
	m_Present.BackBufferHeight = m_uiScreenHeight;
	hResult = m_pDevice->Reset(&m_Present);
	VSMAC_ASSERT(SUCCEEDED(hResult));

	hResult = m_pDevice->GetSwapChain(0, &m_pMainChain);
	VSMAC_ASSERT(m_pMainChain);
	VSMAC_ASSERT(!FAILED(hResult));

	hResult = m_pDevice->GetDepthStencilSurface(&m_pMainDepthStencilBuffer);
	VSMAC_ASSERT(m_pMainDepthStencilBuffer);
	VSMAC_ASSERT(!FAILED(hResult));

	if (m_bWindowed && m_iNumChildWindow > 0)
	{
		for (int i = 0; i < m_iNumChildWindow; i++)
		{
			m_Present.hDeviceWindow = m_pChildWindowInfo[i].m_hHwnd;
			m_Present.BackBufferWidth = m_pChildWindowInfo[i].m_uiWidth;
			m_Present.BackBufferHeight = m_pChildWindowInfo[i].m_uiHeight;
			hResult = m_pDevice->CreateAdditionalSwapChain(&m_Present, &m_pChain[i]);
			VSMAC_ASSERT(!FAILED(hResult));

			if (m_pChildWindowInfo[i].m_bDepth)
			{
				hResult = m_pDevice->CreateDepthStencilSurface(m_pChildWindowInfo[i].m_uiWidth, m_pChildWindowInfo[i].m_uiHeight, D3DFMT_D24S8,
					m_Present.MultiSampleType, m_Present.MultiSampleQuality, FALSE, &m_pChainnDepthStencilBuffer[i], NULL);
				VSMAC_ASSERT(!FAILED(hResult));
			}
		}
	}
	RenderStateToDefault();
	m_bSRGB.SetBufferNum(m_uiMaxBindResource[VSEngineFlag::ST_PIXEL]);
	for (unsigned int i = 0; i < m_uiMaxBindResource[VSEngineFlag::ST_PIXEL]; i++)
	{
		if (m_bSRGB[i])
		{
			m_bSRGB[i] = false;
			SetSamplerState(i, D3DSAMP_SRGBTEXTURE, 0);
		}
	}
	m_bInstanceRenderMode = false;

	for (unsigned int i = 0; i < VSBind::ms_BindArray.GetNum(); i++)
	{
		VSTexture * pTexture = DynamicCast<VSTexture>(VSBind::ms_BindArray[i]);
		if (pTexture)
		{
			if (pTexture->GetOutputType() != VSOutputResource::OT_NONE || !pTexture->IsStatic())
			{
				if (!pTexture->LoadResource(this))
				{
					VSMAC_ASSERT(0);
				}
			}
		}

		VSDepthStencil * pDepthStencilBuffer = DynamicCast<VSDepthStencil>(VSBind::ms_BindArray[i]);
		if (pDepthStencilBuffer)
		{
			if (!pDepthStencilBuffer->LoadResource(this))
			{
				VSMAC_ASSERT(0);
			}
		}
		VSRenderTarget * pRenderTarget = DynamicCast<VSRenderTarget>(VSBind::ms_BindArray[i]);
		if (pRenderTarget)
		{
			if (!pRenderTarget->LoadResource(this))
			{
				VSMAC_ASSERT(0);
			}
		}
		VSQuery * pQuery = DynamicCast<VSQuery>(VSBind::ms_BindArray[i]);
		if (pQuery)
		{
			if (!pQuery->LoadResource(this))
			{
				VSMAC_ASSERT(0);
			}
		}
	}
}
bool VSDX9Renderer::ChangeScreenSize(unsigned int uiWidth, unsigned int uiHeight, bool bWindow)
{
	if (!VSRenderer::ChangeScreenSize(uiWidth, uiHeight, bWindow))
	{
		return false;
	}
	m_Present.Windowed = bWindow;
	if (m_Present.Windowed)
	{
		m_Present.FullScreen_RefreshRateInHz = 0;

	}
	else
	{

		m_Present.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	}

	DeviceLost();
	ResetDevice();
	return true;
}
void * VSDX9Renderer::Lock(VSVertexBuffer * pVertexBuffer)
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

	if (!pVertexBuffer->IsBindResource())
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

	void *pResource = NULL;
	HRESULT hResult = NULL;
	hResult = pVBufferID->m_pVertexBuffer->Lock(0,
		pVertexBuffer->GetOneVertexSize() * pVertexBuffer->GetVertexNum(), (void**)(&pResource), ms_dwLockFlag[pVertexBuffer->GetLockFlag()]);
	VSMAC_ASSERT(!FAILED(hResult));
	return pResource;
}
void VSDX9Renderer::UnLock(VSVertexBuffer * pVertexBuffer)
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
	HRESULT hResult = NULL;
	hResult = pVBufferID->m_pVertexBuffer->Unlock();
	VSMAC_ASSERT(!FAILED(hResult));
	return;
}

void * VSDX9Renderer::Lock(VSIndexBuffer * pIndexBuffer)
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
		return NULL;
	}
	void *pResource = NULL;
	HRESULT hResult = NULL;
	hResult = pIBufferID->m_IndexBuffer->Lock(0,
		pIndexBuffer->GetByteSize(), (void**)(&pResource), ms_dwLockFlag[pIndexBuffer->GetLockFlag()]);
	VSMAC_ASSERT(!FAILED(hResult));
	return pResource;
}
void VSDX9Renderer::UnLock(VSIndexBuffer * pIndexBuffer)
{
	if (!pIndexBuffer)
		return;
	// 	if(!pIndexBuffer->GetData())
	// 		return ;
	if (pIndexBuffer->IsStatic())
		return;
	VSIBufferID* pIBufferID = (VSIBufferID *)pIndexBuffer->GetIdentifier();
	if (!pIBufferID)
	{
		return;
	}
	if (!pIBufferID->m_IndexBuffer)
		return;
	HRESULT hResult = NULL;
	hResult = pIBufferID->m_IndexBuffer->Unlock();
	VSMAC_ASSERT(!FAILED(hResult));
	return;
}
void * VSDX9Renderer::Lock(VSTexture * pTexture, unsigned int uiLevel, unsigned int uiFace)
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
		D3DLOCKED_RECT LockRect;
		LPDIRECT3DTEXTURE9 pDXTexture = (LPDIRECT3DTEXTURE9)pTextureID->m_pTexture;
		hResult = pDXTexture->LockRect(uiLevel, &LockRect, 0, ms_dwLockFlag[pTexture->GetLockFlag()]);
		VSMAC_ASSERT(!FAILED(hResult));
		return LockRect.pBits;
	}
	else if (pTexture->GetTexType() == VSTexture::TT_3D)
	{
		LPDIRECT3DVOLUMETEXTURE9 pDXTexture = (LPDIRECT3DVOLUMETEXTURE9)pTextureID->m_pTexture;

		D3DLOCKED_BOX kLockBox;
		hResult = pDXTexture->LockBox(uiLevel, &kLockBox, 0, ms_dwLockFlag[pTexture->GetLockFlag()]);
		VSMAC_ASSERT(!FAILED(hResult));
		return kLockBox.pBits;
	}
	else if (pTexture->GetTexType() == VSTexture::TT_CUBE)
	{
		if (uiFace > VSCubeTexture::F_MAX)
			return NULL;
		D3DLOCKED_RECT LockRect;
		LPDIRECT3DCUBETEXTURE9 pDXTexture = (LPDIRECT3DCUBETEXTURE9)pTextureID->m_pTexture;
		hResult = pDXTexture->LockRect((D3DCUBEMAP_FACES)ms_dwCubeMapFace[uiFace], uiLevel, &LockRect, NULL, ms_dwLockFlag[pTexture->GetLockFlag()]);
		VSMAC_ASSERT(!FAILED(hResult));
		return LockRect.pBits;

	}
	else
	{
		VSMAC_ASSERT(0);
		return NULL;
	}

}
void VSDX9Renderer::UnLock(VSTexture * pTexture, unsigned int uiLevel, unsigned int uiFace)
{
	if (!pTexture)
		return;
	if (pTexture->IsStatic() || uiLevel >= pTexture->GetMipLevel())
		return;
	if (pTexture->GetOutputType() != VSOutputResource::OT_NONE)
	{
		return;
	}
	VSTextureID * pTextureID = (VSTextureID *)pTexture->GetIdentifier();
	if (!pTextureID)
		return;
	if (!pTextureID->m_pTexture)
		return;
	HRESULT hResult = NULL;
	if (pTexture->GetTexType() == VSTexture::TT_2D)
	{
		LPDIRECT3DTEXTURE9 pDXTexture = (LPDIRECT3DTEXTURE9)pTextureID->m_pTexture;

		hResult = pDXTexture->UnlockRect(uiLevel);
		VSMAC_ASSERT(!FAILED(hResult));
		return;
	}
	else if (pTexture->GetTexType() == VSTexture::TT_3D)
	{
		LPDIRECT3DVOLUMETEXTURE9 pDXTexture = (LPDIRECT3DVOLUMETEXTURE9)pTextureID->m_pTexture;
		hResult = pDXTexture->UnlockBox(uiLevel);
		VSMAC_ASSERT(!FAILED(hResult));
		return;
	}
	else if (pTexture->GetTexType() == VSTexture::TT_CUBE)
	{
		if (uiFace > VSCubeTexture::F_MAX)
			return;
		LPDIRECT3DCUBETEXTURE9 pDXTexture = (LPDIRECT3DCUBETEXTURE9)pTextureID->m_pTexture;

		hResult = pDXTexture->UnlockRect((D3DCUBEMAP_FACES)ms_dwCubeMapFace[uiFace], uiLevel);
		VSMAC_ASSERT(!FAILED(hResult));
		return;

	}
	else
	{
		VSMAC_ASSERT(0);
		return;
	}
	return;
}
void * VSDX9Renderer::Lock(VSBufferResource * pStructBuffer)
{
	return NULL;
}
void VSDX9Renderer::UnLock(VSBufferResource * pStructBuffer)
{

}
void * VSDX9Renderer::Lock(VSUnorderAccess  * pUnorderAccess)
{
	return NULL;
}
void VSDX9Renderer::UnLock(VSUnorderAccess  * pUnorderAccess)
{

}
void * VSDX9Renderer::Lock(VSRenderTarget * pRenderTarget)
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

	LPDIRECT3DTEXTURE9 pDXStagTexture = (LPDIRECT3DTEXTURE9)pVSRenderTargetID->m_pStagTexture;
	if (pDXStagTexture)
	{
		return  NULL;
	}
	HRESULT hResult = NULL;
	D3DLOCKED_RECT LockRect;
	hResult = pDXStagTexture->LockRect(0,&LockRect, 0, ms_dwLockFlag[pRenderTarget->GetLockFlag()]);
	VSMAC_ASSERT(!FAILED(hResult));
	return LockRect.pBits;
}
void VSDX9Renderer::UnLock(VSRenderTarget * pRenderTarget)
{
	if (!pRenderTarget)
	{
		return ;
	}
	if (pRenderTarget->GetLockFlag() != VSInheritBind::LF_READONLY)
	{
		return;
	}
	VSRenderTargetID * pVSRenderTargetID = (VSRenderTargetID *)pRenderTarget->GetIdentifier();
	if (!pVSRenderTargetID)
		return ;
	LPDIRECT3DTEXTURE9 pDXStagTexture = (LPDIRECT3DTEXTURE9)pVSRenderTargetID->m_pStagTexture;
	if (pDXStagTexture)
	{
		return;
	}
	HRESULT hResult = NULL;
	hResult = pDXStagTexture->UnlockRect(0);
	VSMAC_ASSERT(!FAILED(hResult));
	return;
}

void * VSDX9Renderer::Lock(VSDepthStencil * pDepthStencil)
{
	return NULL;
}
void VSDX9Renderer::UnLock(VSDepthStencil * pDepthStencil)
{

}
void VSDX9Renderer::GetIncludeShader(VSString &OutString)
{
	OutString = ms_IncludeShader;
}

void VSDX9Renderer::GetDynamicShader(VSString &OutString)
{
	OutString = ms_DynamicShader;
}
const VSAdapterInfo* VSDX9Renderer::GetAdapterInfo(unsigned int & uiAdapterNum)
{
	uiAdapterNum = ms_uiAdapterNum;
	return ms_AdapterInfo;
}
#endif
