#pragma once
#include "VSBind.h"
#ifdef WINDOWS_PLATFORM
#ifdef VSDX11RENDERER_EXPORTS
#define  VSDX11RENDERER_API __declspec(dllexport) 
#else
#define  VSDX11RENDERER_API __declspec(dllimport)
#endif
#include <d3d11.h>
#endif
namespace VSEngine2
{
#ifdef WINDOWS_PLATFORM
	class  VSDX11RENDERER_API VSSamplerStateID : public VSResourceIdentifier
	{
	public:
		VSSamplerStateID()
		{
			m_pSamplerState = NULL;
		}

		virtual ~VSSamplerStateID()
		{
			VSMAC_RELEASE(m_pSamplerState)
		}
		ID3D11SamplerState* m_pSamplerState;

	};
	class  VSDX11RENDERER_API VSBlendStateID : public VSResourceIdentifier
	{
	public:
		VSBlendStateID()
		{
			m_pBlendState = NULL;
		}

		virtual ~VSBlendStateID()
		{
				VSMAC_RELEASE(m_pBlendState)
		}
		ID3D11BlendState* m_pBlendState;

	};
	class  VSDX11RENDERER_API VSRasterStateID : public VSResourceIdentifier
	{
	public:
		VSRasterStateID()
		{
			m_pRasterState = NULL;
		}
		virtual ~VSRasterStateID()
		{
			VSMAC_RELEASE(m_pRasterState)

		}
		ID3D11RasterizerState* m_pRasterState;  //∂•µ„‰÷»æ∆˜

	};
	class  VSDX11RENDERER_API VSDepthStencilStateID : public VSResourceIdentifier
	{
	public:
		VSDepthStencilStateID()
		{
			m_pDepthStencilState = NULL;
		}
		virtual ~VSDepthStencilStateID()
		{
			VSMAC_RELEASE(m_pDepthStencilState)

		}
		ID3D11DepthStencilState* m_pDepthStencilState;  //∂•µ„‰÷»æ∆˜

	};
	class  VSDX11RENDERER_API VSVProgramID : public VSResourceIdentifier
	{
	public:
		VSVProgramID()
		{
			m_pVertexShader = NULL;
		}
		virtual ~VSVProgramID()
		{
			VSMAC_RELEASE(m_pVertexShader)
		}
		ID3D11VertexShader*      m_pVertexShader;  //∂•µ„‰÷»æ∆˜
	};
	class  VSDX11RENDERER_API VSPProgramID : public VSResourceIdentifier
	{
	public:
		VSPProgramID()
		{
			m_pPixelShader = NULL;
		}
		virtual ~VSPProgramID()
		{
			VSMAC_RELEASE(m_pPixelShader)
		}
		ID3D11PixelShader*      m_pPixelShader;  //œÒÀÿ‰÷»æ∆˜
	};
	class  VSDX11RENDERER_API VSGProgramID : public VSResourceIdentifier
	{
	public:
		VSGProgramID()
		{
			m_pGeometryShader = NULL;
		}
		virtual ~VSGProgramID()
		{
			VSMAC_RELEASE(m_pGeometryShader)
		}
		ID3D11GeometryShader*      m_pGeometryShader;  
	};
	class  VSDX11RENDERER_API VSDProgramID : public VSResourceIdentifier
	{
	public:
		VSDProgramID()
		{
			m_pDomainShader = NULL;
		}
		virtual ~VSDProgramID()
		{
			VSMAC_RELEASE(m_pDomainShader)
		}
		ID3D11DomainShader*      m_pDomainShader;  
	};
	class  VSDX11RENDERER_API VSHProgramID : public VSResourceIdentifier
	{
	public:
		VSHProgramID()
		{
			m_pHullShader = NULL;
		}
		virtual ~VSHProgramID()
		{
			VSMAC_RELEASE(m_pHullShader)
		}
		ID3D11HullShader*      m_pHullShader;  
	};
	class  VSDX11RENDERER_API VSCProgramID : public VSResourceIdentifier
	{
	public:
		VSCProgramID()
		{
			m_pComputeShader = NULL;
		}
		virtual ~VSCProgramID()
		{
			VSMAC_RELEASE(m_pComputeShader)
		}
		ID3D11ComputeShader*      m_pComputeShader;
	};
// 
	class  VSDX11RENDERER_API VSRenderTargetID : public VSResourceIdentifier
	{
	public:
		VSRenderTargetID()
		{
			m_pSaveRenderTarget = NULL;
			m_pSaveDepthStencilBuffer = NULL;
			m_pRenderTarget = NULL;
			m_pTextureSurface = NULL;
			m_pOldTexture = NULL;
			m_pStagTexture = NULL;
		}
		~VSRenderTargetID()
		{
			VSMAC_RELEASE(m_pRenderTarget);
			VSMAC_RELEASE(m_pTextureSurface);
			VSMAC_RELEASE(m_pStagTexture);
			m_pOldTexture = NULL;
			m_pSaveRenderTarget = NULL;
			m_pSaveDepthStencilBuffer = NULL;
		}
		ID3D11Resource*				m_pOldTexture;
		ID3D11Resource*				m_pTextureSurface;
		ID3D11RenderTargetView *	m_pRenderTarget;
		ID3D11RenderTargetView *	m_pSaveRenderTarget;
		ID3D11DepthStencilView *	m_pSaveDepthStencilBuffer;
		ID3D11Resource*				m_pStagTexture;
	};
	class  VSDX11RENDERER_API VSTextureID : public VSResourceIdentifier
	{
	public:
		VSTextureID()
		{
			
			m_pTexture = NULL;
			m_pShaderResourceView = NULL;
		}
		virtual ~VSTextureID()
		{
			VSMAC_RELEASE(m_pShaderResourceView);
			VSMAC_RELEASE(m_pTexture);		
		}

		ID3D11Resource*      m_pTexture;
		ID3D11ShaderResourceView* m_pShaderResourceView;
	};
	class VSDX11RENDERER_API VSDepthStencilID : public VSResourceIdentifier
	{
	public:
		VSDepthStencilID()
		{
			m_pTextureSurface = NULL;
			m_pDepthStencilBuffer = NULL;
			m_pSaveDepthStencilBuffer = NULL;
			m_pOldTexture = NULL;
			m_pStagTexture = NULL;
		}
		virtual ~VSDepthStencilID()
		{
			VSMAC_RELEASE(m_pDepthStencilBuffer);
			VSMAC_RELEASE(m_pTextureSurface);
			VSMAC_RELEASE(m_pStagTexture);
			m_pSaveDepthStencilBuffer = NULL;
			m_pOldTexture = NULL;
		}
		ID3D11Resource*				m_pOldTexture;
		ID3D11Resource*				m_pTextureSurface;
		ID3D11DepthStencilView *	m_pDepthStencilBuffer;
		ID3D11DepthStencilView *	m_pSaveDepthStencilBuffer;
		ID3D11Resource*				m_pStagTexture;
	};
	class VSDX11RENDERER_API VSUnorderAccessID : public VSResourceIdentifier
	{
	public:
		VSUnorderAccessID()
		{
			m_pOldBuffer = NULL;
			m_pBuffer = NULL;
			m_pUnorderedAccess = NULL;
			m_pStagBuffer = NULL;
		}
		~VSUnorderAccessID()
		{
			VSMAC_RELEASE(m_pUnorderedAccess);
			VSMAC_RELEASE(m_pBuffer);
			VSMAC_RELEASE(m_pStagBuffer);

			m_pOldBuffer = NULL;
		}
		ID3D11Resource*				m_pOldBuffer;
		ID3D11Resource*				m_pBuffer;
		ID3D11UnorderedAccessView *	m_pUnorderedAccess;
		ID3D11Resource*				m_pStagBuffer;
	};
	class VSDX11RENDERER_API VSVBufferID : public VSResourceIdentifier
	{
	public:
		VSVBufferID()
		{
			m_pVertexBuffer = NULL;
		}
		virtual ~VSVBufferID()
		{
			VSMAC_RELEASE(m_pVertexBuffer);
		}
		ID3D11Buffer* m_pVertexBuffer;
	
	};
	class VSDX11RENDERER_API VSSBufferID : public VSResourceIdentifier
	{
	public:
		VSSBufferID()
		{
			m_pSBuffer = NULL;
			m_pShaderResourceView = NULL;
		}
		virtual ~VSSBufferID()
		{
			VSMAC_RELEASE(m_pShaderResourceView);
			VSMAC_RELEASE(m_pSBuffer);
		}
		ID3D11Buffer* m_pSBuffer;
		ID3D11ShaderResourceView* m_pShaderResourceView;

	};
	class VSDX11RENDERER_API VSVBufferFormatID : public VSResourceIdentifier
	{
	public:
		VSVBufferFormatID()
		{

			m_pDeclaration = NULL;
		}
		virtual ~VSVBufferFormatID()
		{
			VSMAC_RELEASE(m_pDeclaration);
		}
		ID3D11InputLayout* m_pDeclaration;
	
	};
	class VSDX11RENDERER_API VSIBufferID : public VSResourceIdentifier
	{
	public:
		VSIBufferID()
		{
			m_IndexBuffer = NULL;
		}
		virtual ~VSIBufferID()
		{
			VSMAC_RELEASE(m_IndexBuffer);

		}
		ID3D11Buffer* m_IndexBuffer;
	};
	class VSDX11RENDERER_API VSQueryID : public VSResourceIdentifier
	{
	public:
		VSQueryID()
		{
			m_Query = NULL;
		}
		virtual ~VSQueryID()
		{
			VSMAC_RELEASE(m_Query);
		}
		ID3D11Query * m_Query;
	};
#endif
}
