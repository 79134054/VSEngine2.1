#include "VSDx11Renderer.h"
#include "VSDx11Resource.h"
#include "VSGraphicInclude.h"
#include "VSRenderThread.h"
#include "VSConfig.h"
#ifdef WINDOWS_PLATFORM
using namespace VSEngine2;
void VSDX11Renderer::ClearBackBuffer()
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11ClearBackBufferCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		VSColorRGBA, m_ClearColor, m_ClearColor,
	{
		ID3D11RenderTargetView * RTVTemp[RENDERTARGET_LEVEL] = { NULL };
		m_pDeviceContext->OMGetRenderTargets(RENDERTARGET_LEVEL, RTVTemp, NULL);
		for (unsigned int i = 0; i < RENDERTARGET_LEVEL; i++)
		{
			if (RTVTemp[i])
			{
				m_pDeviceContext->ClearRenderTargetView(RTVTemp[i], m_ClearColor.m);
			}
			else
			{
				break;
			}
		}
		for (unsigned int i = 0; i < RENDERTARGET_LEVEL; i++)
		{
			VSMAC_RELEASE(RTVTemp[i]);
		}
	})
}
void VSDX11Renderer::ClearDepthStencilView(UINT ClearFlags, VSREAL Depth, unsigned char Stencil)
{
	struct  VSClearDepthStencilViewPara
	{
		UINT ClearFlags;
		VSREAL Depth;
		unsigned char Stencil;
	};
	VSClearDepthStencilViewPara ClearDepthStencilViewPara;
	ClearDepthStencilViewPara.ClearFlags = ClearFlags;
	ClearDepthStencilViewPara.Depth = Depth;
	ClearDepthStencilViewPara.Stencil = Stencil;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11ClearDepthStencilViewCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		VSClearDepthStencilViewPara, ClearDepthStencilViewPara, ClearDepthStencilViewPara,
	{
		ID3D11DepthStencilView * DSV = NULL;
		m_pDeviceContext->OMGetRenderTargets(0, NULL, &DSV);
		if (DSV)
		{
			m_pDeviceContext->ClearDepthStencilView(DSV, ClearDepthStencilViewPara.ClearFlags, ClearDepthStencilViewPara.Depth, ClearDepthStencilViewPara.Stencil);
		}
		VSMAC_RELEASE(DSV);
	})
}
void VSDX11Renderer::OMSetDepthStencilState(VSDepthStencilStateID *pDepthStencilStateID, UINT StencilRef)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11OMSetDepthStencilStateCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		VSDepthStencilStateID *, pDepthStencilStateID, pDepthStencilStateID, UINT, StencilRef, StencilRef,
	{
		if (pDepthStencilStateID)
		{
			m_pDeviceContext->OMSetDepthStencilState(pDepthStencilStateID->m_pDepthStencilState, StencilRef);
		}
		else
		{
			m_pDeviceContext->OMSetDepthStencilState(NULL, 0);
		}
	})
}
void VSDX11Renderer::RSSetState(VSRasterStateID *pRasterStateID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11RSSetStateCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		VSRasterStateID *, pRasterStateID, pRasterStateID,
	{
		if (pRasterStateID)
		{
			m_pDeviceContext->RSSetState(pRasterStateID->m_pRasterState);
		}
		else
		{
			m_pDeviceContext->RSSetState(NULL);
		}
	})
}
void VSDX11Renderer::RSSetViewports(D3D11_VIEWPORT& View)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11RSSetStateCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		D3D11_VIEWPORT, View, View,
	{
		m_pDeviceContext->RSSetViewports(1, &View);
	})
}
void VSDX11Renderer::UseWindowEx(int uiWindowID)
{
	ID3D11RenderTargetView* pRenderTargetView = NULL;
	ID3D11DepthStencilView* pDepthStencilView = NULL;
	if (uiWindowID == -1)
	{
		pRenderTargetView = m_pMainRenderTargetView;
		pDepthStencilView = m_pMainDepthStencilView;
		
	}
	else
	{
		pRenderTargetView = m_pChainRenderTargetView[m_iCurWindowID];
		pDepthStencilView = m_pChainDepthStencilView[uiWindowID];
	}
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11UseWindowExCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		ID3D11RenderTargetView*, pRenderTargetView, pRenderTargetView, ID3D11DepthStencilView*, pDepthStencilView, pDepthStencilView,
	{
		m_pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);
	})
}
bool VSDX11Renderer::EndRender()
{
	IDXGISwapChain* pChain = NULL;
	if (m_iCurWindowID == -1)
	{
		pChain = m_pMainChain;
	}
	else
	{
		pChain = m_pChain[m_iCurWindowID];
	}

	ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(VSDx11EndRenderCommand, IDXGISwapChain*, pChain, pChain,
	{
		HRESULT hResult = NULL;
		hResult = pChain->Present(0, 0);
		VSMAC_ASSERT(!FAILED(hResult));
	})
	return true;
}
void VSDX11Renderer::PSSetShaderResources(UINT StartSlot, VSTextureID *pTextureID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11PSSetShaderResourcesCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		UINT, StartSlot, StartSlot, VSTextureID *, pTextureID, pTextureID,
	{
		if (pTextureID)
		{
			ID3D11ShaderResourceView* Temp[1] = { pTextureID->m_pShaderResourceView };
			m_pDeviceContext->PSSetShaderResources(StartSlot, 1, Temp);
		}
		else
		{
			ID3D11ShaderResourceView* Temp[1] = { NULL };
			m_pDeviceContext->PSSetShaderResources(StartSlot, 1, Temp);
		}
	})	
}
bool VSDX11Renderer::CreateInputLayout(const VSArray<VSVertexFormat::VERTEXFORMAT_TYPE>& FormatArray, VSVBufferFormatID * pVBufferFormatID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11CreateInputLayoutCommand, ID3D11Device*, m_pDevice, m_pDevice,
		VSArray<VSVertexFormat::VERTEXFORMAT_TYPE>, FormatArray, FormatArray, 
		VSVBufferFormatID *, pVBufferFormatID, pVBufferFormatID,
	{
		HRESULT hResult = NULL;
		VSArray<D3D11_INPUT_ELEMENT_DESC> Elements;
		D3D11_INPUT_ELEMENT_DESC Element;	
		for (unsigned int i = 0; i < FormatArray.GetNum(); i++)
		{
			VSVertexFormat::VERTEXFORMAT_TYPE &Format = FormatArray[i];
			Element.InputSlot = Format.Stream;
			Element.SemanticName = ms_dwVertexFormatSemantics[Format.Semantics];
			Element.AlignedByteOffset = Format.Offset;
			Element.Format = (DXGI_FORMAT)ms_dwVertexFormatDataType[Format.DataType];
			Element.SemanticIndex = Format.SemanticsIndex;
			if (Format.InstanceDataStepRate == VSVertexFormat::VERTEXFORMAT_TYPE::IDSR_PER_VERTEX_DATA)
			{
				Element.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				Element.InstanceDataStepRate = 0;
			}
			else
			{
				Element.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
				Element.InstanceDataStepRate = 1;
			}
			Elements.AddElement(Element);
		}
		ID3DBlob* pCode = VSDX11Renderer::CreateVertexFormatShader(FormatArray);
		if (!pCode)
		{
			return false;
		}
		// Create the vertex input layout.
		hResult = m_pDevice->CreateInputLayout(Elements.GetBuffer(), Elements.GetNum(), pCode->GetBufferPointer(),
			pCode->GetBufferSize(), &pVBufferFormatID->m_pDeclaration);
		VSMAC_RELEASE(pCode);
		VSMAC_ASSERT(!FAILED(hResult));
	})

	return true;
}
void VSDX11Renderer::DrawDynamicScreenData(void * pScreenBuffer, unsigned int uiVertexNum, unsigned int uiVertexStride, VSUSHORT_INDEX * pIndexBuffer, unsigned int uiIndexNum)
{
	struct ScreenData
	{
		void* pScreenBuffer;
		VSUSHORT_INDEX * pIndexBuffer;
		unsigned int uiVertexNum;
		unsigned int uiIndexNum;
		unsigned int uiVertexStride;
		ID3D11Buffer** pFontVertexBuffer;
		ID3D11Buffer** pFontIndexBuffer;
	};
	HRESULT hResult = NULL;
	ScreenData Screen;
	Screen.uiVertexNum = uiVertexNum;
	Screen.uiIndexNum = uiIndexNum;
	Screen.uiVertexStride = uiVertexStride;
	if (VSResourceManager::ms_bRenderThread)
	{
		unsigned int uiVertexLen = uiVertexStride * uiVertexNum;
		Screen.pScreenBuffer = (void *)VSRenderThreadSys::ms_pRenderThreadSys->Assign(uiVertexLen);
		VSMemcpy(Screen.pScreenBuffer, pScreenBuffer, uiVertexLen);

		unsigned int uiIndexLen = sizeof(VSUSHORT_INDEX)* uiIndexNum;
		Screen.pIndexBuffer = (VSUSHORT_INDEX *)VSRenderThreadSys::ms_pRenderThreadSys->Assign(uiIndexLen);
		VSMemcpy(Screen.pIndexBuffer, pIndexBuffer, uiIndexLen);

	}
	else
	{
		Screen.pScreenBuffer = pScreenBuffer;
		Screen.pIndexBuffer = pIndexBuffer;
	}
	Screen.pFontVertexBuffer = &m_pFontVertexBuffer;
	Screen.pFontIndexBuffer = &m_pFontIndexBuffer;
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11DrawScreenEXCommand,
		ScreenData, Screen, Screen, ID3D11Device*, m_pDevice, m_pDevice, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		{
		unsigned int VertexBytes = Screen.uiVertexNum * Screen.uiVertexStride;
		unsigned int IndexBytes = Screen.uiIndexNum * sizeof(VSUSHORT_INDEX);
		D3D11_BUFFER_DESC VertexBufferDesc;
		(*Screen.pFontVertexBuffer)->GetDesc(&VertexBufferDesc);
		if (VertexBufferDesc.ByteWidth < VertexBytes)
		{
			VertexBufferDesc.ByteWidth = VertexBytes;
			VSMAC_RELEASE((*Screen.pFontVertexBuffer));
			m_pDevice->CreateBuffer(&VertexBufferDesc, NULL, Screen.pFontVertexBuffer);
		}
		D3D11_BUFFER_DESC IndexBufferDesc;
		(*Screen.pFontIndexBuffer)->GetDesc(&IndexBufferDesc);
		if (IndexBufferDesc.ByteWidth < IndexBytes)
		{
			IndexBufferDesc.ByteWidth = IndexBytes;
			VSMAC_RELEASE((*Screen.pFontIndexBuffer));
			m_pDevice->CreateBuffer(&IndexBufferDesc, NULL, Screen.pFontIndexBuffer);
		}
		HRESULT hResult = NULL;
		D3D11_MAPPED_SUBRESOURCE MappedResource;
		hResult = m_pDeviceContext->Map((*Screen.pFontVertexBuffer), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
		VSMAC_ASSERT(!FAILED(hResult));
		VSMemcpy(MappedResource.pData, (void*)Screen.pScreenBuffer, VertexBytes);
		m_pDeviceContext->Unmap((*Screen.pFontVertexBuffer), 0);


		hResult = m_pDeviceContext->Map((*Screen.pFontIndexBuffer), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
		VSMAC_ASSERT(!FAILED(hResult));
		VSMemcpy(MappedResource.pData, (void*)Screen.pIndexBuffer, IndexBytes);
		m_pDeviceContext->Unmap((*Screen.pFontIndexBuffer), 0);

		unsigned int Stride = Screen.uiVertexStride;
		unsigned int Offset = 0;
		m_pDeviceContext->IASetIndexBuffer((*Screen.pFontIndexBuffer), DXGI_FORMAT_R16_UINT, 0);
		m_pDeviceContext->IASetVertexBuffers(0, 1, Screen.pFontVertexBuffer, &Stride, &Offset);
		m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_pDeviceContext->DrawIndexed(Screen.uiIndexNum, 0, 0);
	})
}
void VSDX11Renderer::OMSetBlendState(VSBlendStateID* pBlendStateID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11OMSetBlendStateCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		VSBlendStateID*, pBlendStateID, pBlendStateID,
	{
		VSREAL fBlendFactor[4] = { 0.0f };
		if (pBlendStateID)
		{

			m_pDeviceContext->OMSetBlendState(pBlendStateID->m_pBlendState, fBlendFactor, 0xffffffff);
		}
		else
		{
			m_pDeviceContext->OMSetBlendState(NULL, fBlendFactor, 0xffffffff);
		}
		//m_pDeviceContext->OMSetBlendState(pBlendStateID->m_pBlendState, m_pBlendState->GetBlendDesc().fBlendFactor, m_pBlendState->GetBlendDesc().ucSampleMask);
	})
}
void VSDX11Renderer::RSSetScissorRects(UINT NumRects, D3D11_RECT *pRects)
{
	D3D11_RECT *pData = pRects;
	if (VSResourceManager::ms_bRenderThread)
	{
		unsigned int uiDataSize = NumRects * sizeof(D3D11_RECT);
		pData = (D3D11_RECT *)VSRenderThreadSys::ms_pRenderThreadSys->Assign(uiDataSize);
		VSMemcpy(pData, pRects, uiDataSize);
	}
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11RSSetScissorRectsCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		D3D11_RECT *, pData, pData, UINT, NumRects, NumRects,
	{
		m_pDeviceContext->RSSetScissorRects(NumRects, pData);
	})
}
void VSDX11Renderer::UpdateSubresource(ID3D11Resource **pDstResource, void *pSrcData, unsigned int uiDataSize)
{
	void *pData = pSrcData;
	if (VSResourceManager::ms_bRenderThread)
	{
		pData = (void *)VSRenderThreadSys::ms_pRenderThreadSys->Assign(uiDataSize);
		VSMemcpy(pData, pSrcData, uiDataSize);
	}
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11UpdateSubresourceCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		void *, pData, pData, ID3D11Resource **, pDstResource, pDstResource,
	{
		m_pDeviceContext->UpdateSubresource(*pDstResource, 0, NULL, pData, 0, 0);
	})
}
void VSDX11Renderer::IASetVertexBuffers(unsigned int uiStream,VSVBufferID* pVBufferID, unsigned int uiStrides)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_FOURPARAMETER(VSDx11IASetVertexBuffersCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		VSVBufferID*, pVBufferID, pVBufferID, unsigned int, uiStrides, uiStrides, unsigned int, uiStream, uiStream,
	{
		if (pVBufferID && uiStrides)
		{
			UINT strides[1] = { uiStrides };
			UINT offset[1] = { 0 };
			ID3D11Buffer* VertexBuffer[1] = { pVBufferID->m_pVertexBuffer };
			m_pDeviceContext->IASetVertexBuffers(uiStream, 1, VertexBuffer, strides, offset);
		}
		else
		{
			m_pDeviceContext->IASetVertexBuffers(uiStream, 0, NULL, NULL, NULL);
		}
	})
}
void VSDX11Renderer::IASetIndexBuffer(VSIBufferID* pIBufferID, DXGI_FORMAT Format)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11IASetIndexBufferCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		VSIBufferID*, pIBufferID, pIBufferID, DXGI_FORMAT, Format, Format,
	{
		if (pIBufferID)
		{
			m_pDeviceContext->IASetIndexBuffer(pIBufferID->m_IndexBuffer, Format, 0);
		}
		else
		{
			m_pDeviceContext->IASetIndexBuffer(NULL, DXGI_FORMAT_UNKNOWN, 0);
		}
	})
}
void VSDX11Renderer::GSSetShaderResources(UINT StartSlot, VSTextureID *pTextureID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11GSSetShaderResourcesCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		UINT, StartSlot, StartSlot, VSTextureID *, pTextureID, pTextureID,
		{
		if (pTextureID)
		{
			ID3D11ShaderResourceView* Temp[1] = { pTextureID->m_pShaderResourceView };
			m_pDeviceContext->GSSetShaderResources(StartSlot, 1, Temp);
		}
		else
		{
			ID3D11ShaderResourceView* Temp[1] = { NULL };
			m_pDeviceContext->GSSetShaderResources(StartSlot, 1, Temp);
		}
	})
}
void VSDX11Renderer::DSSetShaderResources(UINT StartSlot, VSTextureID *pTextureID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11DSSetShaderResourcesCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		UINT, StartSlot, StartSlot, VSTextureID *, pTextureID, pTextureID,
		{
		if (pTextureID)
		{
			ID3D11ShaderResourceView* Temp[1] = { pTextureID->m_pShaderResourceView };
			m_pDeviceContext->DSSetShaderResources(StartSlot, 1, Temp);
		}
		else
		{
			ID3D11ShaderResourceView* Temp[1] = { NULL };
			m_pDeviceContext->DSSetShaderResources(StartSlot, 1, Temp);
		}
	})
}
void VSDX11Renderer::CSSetBufferResources(UINT StartSlot, VSSBufferID *pBufferResourceID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11CSSetBufferResourcesCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		UINT, StartSlot, StartSlot, VSSBufferID *, pBufferResourceID, pBufferResourceID,
		{
		if (pBufferResourceID)
		{
			ID3D11ShaderResourceView* Temp[1] = { pBufferResourceID->m_pShaderResourceView };
			m_pDeviceContext->CSSetShaderResources(StartSlot, 1, Temp);
		}
		else
		{
			ID3D11ShaderResourceView* Temp[1] = { NULL };
			m_pDeviceContext->CSSetShaderResources(StartSlot, 1, Temp);
		}
		})
}
void VSDX11Renderer::VSSetBufferResources(UINT StartSlot, VSSBufferID *pBufferResourceID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11CSSetBufferResourcesCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		UINT, StartSlot, StartSlot, VSSBufferID *, pBufferResourceID, pBufferResourceID,
		{
		if (pBufferResourceID)
		{
			ID3D11ShaderResourceView* Temp[1] = { pBufferResourceID->m_pShaderResourceView };
			m_pDeviceContext->VSSetShaderResources(StartSlot, 1, Temp);
		}
		else
		{
			ID3D11ShaderResourceView* Temp[1] = { NULL };
			m_pDeviceContext->VSSetShaderResources(StartSlot, 1, Temp);
		}
		})
}
void VSDX11Renderer::PSSetBufferResources(UINT StartSlot, VSSBufferID *pBufferResourceID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11CSSetBufferResourcesCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		UINT, StartSlot, StartSlot, VSSBufferID *, pBufferResourceID, pBufferResourceID,
		{
		if (pBufferResourceID)
		{
			ID3D11ShaderResourceView* Temp[1] = { pBufferResourceID->m_pShaderResourceView };
			m_pDeviceContext->PSSetShaderResources(StartSlot, 1, Temp);
		}
		else
		{
			ID3D11ShaderResourceView* Temp[1] = { NULL };
			m_pDeviceContext->PSSetShaderResources(StartSlot, 1, Temp);
		}
		})
}
void VSDX11Renderer::GSSetBufferResources(UINT StartSlot, VSSBufferID *pBufferResourceID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11CSSetBufferResourcesCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		UINT, StartSlot, StartSlot, VSSBufferID *, pBufferResourceID, pBufferResourceID,
		{
		if (pBufferResourceID)
		{
			ID3D11ShaderResourceView* Temp[1] = { pBufferResourceID->m_pShaderResourceView };
			m_pDeviceContext->GSSetShaderResources(StartSlot, 1, Temp);
		}
		else
		{
			ID3D11ShaderResourceView* Temp[1] = { NULL };
			m_pDeviceContext->GSSetShaderResources(StartSlot, 1, Temp);
		}
		})
}
void VSDX11Renderer::HSSetBufferResources(UINT StartSlot, VSSBufferID *pBufferResourceID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11CSSetBufferResourcesCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		UINT, StartSlot, StartSlot, VSSBufferID *, pBufferResourceID, pBufferResourceID,
		{
		if (pBufferResourceID)
		{
			ID3D11ShaderResourceView* Temp[1] = { pBufferResourceID->m_pShaderResourceView };
			m_pDeviceContext->HSSetShaderResources(StartSlot, 1, Temp);
		}
		else
		{
			ID3D11ShaderResourceView* Temp[1] = { NULL };
			m_pDeviceContext->HSSetShaderResources(StartSlot, 1, Temp);
		}
		})
}
void VSDX11Renderer::DSSetBufferResources(UINT StartSlot, VSSBufferID *pBufferResourceID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11CSSetBufferResourcesCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		UINT, StartSlot, StartSlot, VSSBufferID *, pBufferResourceID, pBufferResourceID,
		{
		if (pBufferResourceID)
		{
			ID3D11ShaderResourceView* Temp[1] = { pBufferResourceID->m_pShaderResourceView };
			m_pDeviceContext->DSSetShaderResources(StartSlot, 1, Temp);
		}
		else
		{
			ID3D11ShaderResourceView* Temp[1] = { NULL };
			m_pDeviceContext->DSSetShaderResources(StartSlot, 1, Temp);
		}
		})
}
void VSDX11Renderer::CSSetShaderResources(UINT StartSlot, VSTextureID *pTextureID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11CSSetShaderResourcesCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		UINT, StartSlot, StartSlot, VSTextureID *, pTextureID, pTextureID,
		{
		if (pTextureID)
		{
			ID3D11ShaderResourceView* Temp[1] = { pTextureID->m_pShaderResourceView };
			m_pDeviceContext->CSSetShaderResources(StartSlot, 1, Temp);
		}
		else
		{
			ID3D11ShaderResourceView* Temp[1] = { NULL };
			m_pDeviceContext->CSSetShaderResources(StartSlot, 1, Temp);
		}
		})
}
void VSDX11Renderer::HSSetShaderResources(UINT StartSlot, VSTextureID *pTextureID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11HSSetShaderResourcesCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		UINT, StartSlot, StartSlot, VSTextureID *, pTextureID, pTextureID,
		{
		if (pTextureID)
		{
			ID3D11ShaderResourceView* Temp[1] = { pTextureID->m_pShaderResourceView };
			m_pDeviceContext->HSSetShaderResources(StartSlot, 1, Temp);
		}
		else
		{
			ID3D11ShaderResourceView* Temp[1] = { NULL };
			m_pDeviceContext->HSSetShaderResources(StartSlot, 1, Temp);
		}
	})
}
void VSDX11Renderer::VSSetShaderResources(UINT StartSlot, VSTextureID *pTextureID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11VSSetShaderResourcesCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		UINT, StartSlot, StartSlot, VSTextureID *, pTextureID, pTextureID,
	{
		if (pTextureID)
		{
			ID3D11ShaderResourceView* Temp[1] = { pTextureID->m_pShaderResourceView };
			m_pDeviceContext->VSSetShaderResources(StartSlot, 1, Temp);
		}
		else
		{
			ID3D11ShaderResourceView* Temp[1] = { NULL };
			m_pDeviceContext->VSSetShaderResources(StartSlot, 1, Temp);
		}
	})
}
void VSDX11Renderer::VSSetSamplers(UINT StartSlot, VSSamplerStateID* pSamplerStateID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11VSSetSamplersCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		UINT, StartSlot, StartSlot, VSSamplerStateID*, pSamplerStateID, pSamplerStateID,
	{
		if (!pSamplerStateID)
		{
			ID3D11SamplerState* states[1] = { NULL };
			m_pDeviceContext->VSSetSamplers(StartSlot, 1, states);
		}
		else
		{
			ID3D11SamplerState* states[1] = { pSamplerStateID->m_pSamplerState };
			m_pDeviceContext->VSSetSamplers(StartSlot, 1, states);
		}
	})
}
void VSDX11Renderer::DSSetSamplers(UINT StartSlot, VSSamplerStateID* pSamplerStateID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11DSSetSamplersCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		UINT, StartSlot, StartSlot, VSSamplerStateID*, pSamplerStateID, pSamplerStateID,
		{
		if (!pSamplerStateID)
		{
			ID3D11SamplerState* states[1] = { NULL };
			m_pDeviceContext->DSSetSamplers(StartSlot, 1, states);
		}
		else
		{
			ID3D11SamplerState* states[1] = { pSamplerStateID->m_pSamplerState };
			m_pDeviceContext->DSSetSamplers(StartSlot, 1, states);
		}
	})
}
void VSDX11Renderer::HSSetSamplers(UINT StartSlot, VSSamplerStateID* pSamplerStateID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11HSSetSamplersCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		UINT, StartSlot, StartSlot, VSSamplerStateID*, pSamplerStateID, pSamplerStateID,
		{
		if (!pSamplerStateID)
		{
			ID3D11SamplerState* states[1] = { NULL };
			m_pDeviceContext->HSSetSamplers(StartSlot, 1, states);
		}
		else
		{
			ID3D11SamplerState* states[1] = { pSamplerStateID->m_pSamplerState };
			m_pDeviceContext->HSSetSamplers(StartSlot, 1, states);
		}
	})
}
void VSDX11Renderer::CSSetSamplers(UINT StartSlot, VSSamplerStateID* pSamplerStateID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11CSSetSamplersCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		UINT, StartSlot, StartSlot, VSSamplerStateID*, pSamplerStateID, pSamplerStateID,
		{
		if (!pSamplerStateID)
		{
			ID3D11SamplerState* states[1] = { NULL };
			m_pDeviceContext->CSSetSamplers(StartSlot, 1, states);
		}
		else
		{
			ID3D11SamplerState* states[1] = { pSamplerStateID->m_pSamplerState };
			m_pDeviceContext->CSSetSamplers(StartSlot, 1, states);
		}
		})
}
void VSDX11Renderer::GSSetSamplers(UINT StartSlot, VSSamplerStateID* pSamplerStateID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11GSSetSamplersCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		UINT, StartSlot, StartSlot, VSSamplerStateID*, pSamplerStateID, pSamplerStateID,
		{
		if (!pSamplerStateID)
		{
			ID3D11SamplerState* states[1] = { NULL };
			m_pDeviceContext->GSSetSamplers(StartSlot, 1, states);
		}
		else
		{
			ID3D11SamplerState* states[1] = { pSamplerStateID->m_pSamplerState };
			m_pDeviceContext->GSSetSamplers(StartSlot, 1, states);
		}
	})
}
void VSDX11Renderer::PSSetSamplers(UINT StartSlot, VSSamplerStateID* pSamplerStateID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11PSSetSamplersCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		UINT, StartSlot, StartSlot, VSSamplerStateID*, pSamplerStateID, pSamplerStateID,
		{
		if (!pSamplerStateID)
		{
			ID3D11SamplerState* states[1] = { NULL };
			m_pDeviceContext->PSSetSamplers(StartSlot, 1, states);
		}
		else
		{
			ID3D11SamplerState* states[1] = { pSamplerStateID->m_pSamplerState };
			m_pDeviceContext->PSSetSamplers(StartSlot, 1, states);
		}
	})
}
void VSDX11Renderer::SetCSUnorderAccessesEx(VSUnorderAccessID *pUnorderAccess[], unsigned int uiNum)
{
	struct VSSetCSUnorderAccessesEx
	{
		VSUnorderAccessID *pUnorderAccess[RENDERTARGET_LEVEL];
		unsigned int uiNum;
	};
	VSSetCSUnorderAccessesEx SetCSUnorderAccessesExPara;
	for (unsigned int i = 0; i < uiNum; i++)
	{
		SetCSUnorderAccessesExPara.pUnorderAccess[i] = pUnorderAccess[i];
	}
	SetCSUnorderAccessesExPara.uiNum = uiNum;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11SetCSUnorderAccessesExCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		VSSetCSUnorderAccessesEx, SetCSUnorderAccessesExPara, SetCSUnorderAccessesExPara,
		{
			ID3D11UnorderedAccessView * UAVTemp[RENDERTARGET_LEVEL] = { NULL };		
			for (unsigned int i = 0; i < SetCSUnorderAccessesExPara.uiNum; i++)
			{
				VSUnorderAccessID * pUnorderAccessID = SetCSUnorderAccessesExPara.pUnorderAccess[i];
				UAVTemp[i] = pUnorderAccessID->m_pUnorderedAccess;
			}
			m_pDeviceContext->CSSetUnorderedAccessViews(0, SetCSUnorderAccessesExPara.uiNum, UAVTemp, NULL);
		})
}
void VSDX11Renderer::OnDispath(unsigned int uiThreadGroupCountX, unsigned int uiThreadGroupCountY,
	unsigned int uiThreadGroupCountZ)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_FOURPARAMETER(VSDx11SetCSUnorderAccessesExCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		unsigned int, uiThreadGroupCountX, uiThreadGroupCountX, unsigned int, uiThreadGroupCountY, uiThreadGroupCountY,
		unsigned int, uiThreadGroupCountZ, uiThreadGroupCountZ,
		{
			m_pDeviceContext->Dispatch(uiThreadGroupCountX, uiThreadGroupCountY, uiThreadGroupCountZ);
		})
}
void VSDX11Renderer::EndCSUnorderAccessesEx(VSUnorderAccess *pUnorderAccess[], VSUnorderAccessID *pUnorderAccessID[], unsigned int uiNum)
{
	struct VSEndCSUnorderAccessesExPara
	{
		VSUnorderAccessID *pUnorderAccessID[RENDERTARGET_LEVEL];
		VSUnorderAccess *pUnorderAccess[RENDERTARGET_LEVEL];
		unsigned int uiNum;
	};
	VSEndCSUnorderAccessesExPara EndCSUnorderAccessesExPara;
	for (unsigned int i = 0; i < uiNum; i++)
	{
		EndCSUnorderAccessesExPara.pUnorderAccessID[i] = pUnorderAccessID[i];
		EndCSUnorderAccessesExPara.pUnorderAccess[i] = pUnorderAccess[i];
	}
	EndCSUnorderAccessesExPara.uiNum = uiNum;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11EndCSUnorderAccessesExCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		VSEndCSUnorderAccessesExPara, EndCSUnorderAccessesExPara, EndCSUnorderAccessesExPara,
		{
			for (unsigned int i = 0; i < EndCSUnorderAccessesExPara.uiNum; i++)
			{
				VSUnorderAccessID * pUnorderAccessID = EndCSUnorderAccessesExPara.pUnorderAccessID[i];
				VSUnorderAccess * pUnorderAccess = EndCSUnorderAccessesExPara.pUnorderAccess[i];
				VSMAC_ASSERT(pUnorderAccessID && pUnorderAccessID->m_pUnorderedAccess);
				if (pUnorderAccess->GetLockFlag() == VSInheritBind::LF_READONLY)
				{
					VSMAC_ASSERT(pUnorderAccessID->m_pStagBuffer);
					if (pUnorderAccessID->m_pOldBuffer)
					{
						m_pDeviceContext->CopyResource(pUnorderAccessID->m_pStagBuffer, pUnorderAccessID->m_pOldBuffer);
					}			
					else if (pUnorderAccessID->m_pBuffer)
					{
						m_pDeviceContext->CopyResource(pUnorderAccessID->m_pStagBuffer, pUnorderAccessID->m_pBuffer);
					}
					else
					{
						VSMAC_ASSERT(0);
					}
				}

			}
			ID3D11UnorderedAccessView * UAVTemp[UNORDERACCESSS_LEVEL] = { NULL };
			m_pDeviceContext->CSSetUnorderedAccessViews(0, EndCSUnorderAccessesExPara.uiNum, UAVTemp, 0);
		})
}
void VSDX11Renderer::SetRenderTargetsEx(VSRenderTargetID *pRenderTarget[], unsigned int uiNum, unsigned int uiBackMacthType)
{
	struct VSSetRenderTargetsExPara
	{
		VSRenderTargetID *pRenderTarget[RENDERTARGET_LEVEL];
		unsigned int uiNum;
		unsigned int uiBackMacthType;
	};
	VSSetRenderTargetsExPara SetRenderTargetsExPara;
	for (unsigned int i = 0; i < uiNum; i++)
	{
		SetRenderTargetsExPara.pRenderTarget[i] = pRenderTarget[i];
	}
	SetRenderTargetsExPara.uiNum = uiNum;
	SetRenderTargetsExPara.uiBackMacthType = uiBackMacthType;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11SetRenderTargetsExCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		VSSetRenderTargetsExPara, SetRenderTargetsExPara, SetRenderTargetsExPara,
	{
		ID3D11DepthStencilView * DSV = NULL;
		ID3D11RenderTargetView * RTVTemp[RENDERTARGET_LEVEL] = { NULL };
		m_pDeviceContext->OMGetRenderTargets(RENDERTARGET_LEVEL, RTVTemp, &DSV);
		for (unsigned int i = 0; i < SetRenderTargetsExPara.uiNum; i++)
		{
			VSRenderTargetID * pRenderTargetID = SetRenderTargetsExPara.pRenderTarget[i];
			pRenderTargetID->m_pSaveRenderTarget = RTVTemp[i];
			RTVTemp[i] = pRenderTargetID->m_pRenderTarget;

		}

		if (SetRenderTargetsExPara.uiBackMacthType == BMT_RENDER_TARGET)
		{
			VSRenderTargetID * pRenderTargetID = SetRenderTargetsExPara.pRenderTarget[0];
			pRenderTargetID->m_pSaveDepthStencilBuffer = DSV;
			m_pDeviceContext->OMSetRenderTargets(SetRenderTargetsExPara.uiNum, RTVTemp, NULL);
		}
		else
		{
			m_pDeviceContext->OMSetRenderTargets(SetRenderTargetsExPara.uiNum, RTVTemp, DSV);
		}

		VSMAC_RELEASE(DSV);
	})
}
void VSDX11Renderer::EndRenderTargetsEx(VSRenderTarget *pRenderTarget[],VSRenderTargetID *pRenderTargetID[], unsigned int uiNum)
{
	struct VSEndRenderTargetsExPara
	{
		VSRenderTargetID *pRenderTargetID[RENDERTARGET_LEVEL];
		VSRenderTarget *pRenderTarget[RENDERTARGET_LEVEL];
		unsigned int uiNum;
	};
	VSEndRenderTargetsExPara EndRenderTargetsExPara;
	for (unsigned int i = 0; i < uiNum; i++)
	{
		EndRenderTargetsExPara.pRenderTargetID[i] = pRenderTargetID[i];
		EndRenderTargetsExPara.pRenderTarget[i] = pRenderTarget[i];
	}
	EndRenderTargetsExPara.uiNum = uiNum;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11EndRenderTargetsExCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		VSEndRenderTargetsExPara, EndRenderTargetsExPara, EndRenderTargetsExPara,
	{
		ID3D11RenderTargetView * RTVTemp[RENDERTARGET_LEVEL] = { NULL };
		ID3D11DepthStencilView * DSV = NULL;
		m_pDeviceContext->OMGetRenderTargets(0, NULL, &DSV);

		for (unsigned int i = 0; i < EndRenderTargetsExPara.uiNum; i++)
		{
			VSRenderTargetID * pRenderTargetID = EndRenderTargetsExPara.pRenderTargetID[i];
			VSRenderTarget * pRenderTarget = EndRenderTargetsExPara.pRenderTarget[i];
			VSMAC_ASSERT(pRenderTargetID && pRenderTargetID->m_pRenderTarget);
			RTVTemp[i] = pRenderTargetID->m_pSaveRenderTarget;
			pRenderTargetID->m_pSaveRenderTarget = NULL;
			VSTexture * pCreateBy = (VSTexture *)pRenderTarget->GetCreateBy();
			UINT NumLevels = pCreateBy ? pCreateBy->GetMipLevel() : 1;
			UINT SrcSubresource = 0;
			UINT DestSubresource = D3D11CalcSubresource(pRenderTarget->GetLevel(),
				pRenderTarget->GetFirst(), NumLevels);
			if (pCreateBy && pRenderTarget->GetMulSample() != VSRenderer::MS_NONE)
			{
				VSMAC_ASSERT(pRenderTargetID->m_pOldTexture && pRenderTargetID->m_pTextureSurface);				
				m_pDeviceContext->ResolveSubresource(pRenderTargetID->m_pOldTexture, DestSubresource,
					pRenderTargetID->m_pTextureSurface, SrcSubresource, (DXGI_FORMAT)ms_dwTextureFormatType[pRenderTarget->GetFormatType()]);
					
			}
			if (pRenderTarget->GetLockFlag() == VSInheritBind::LF_READONLY)
			{
				if (pCreateBy)
				{
					VSMAC_ASSERT(pRenderTargetID->m_pStagTexture && pRenderTargetID->m_pOldTexture);
					m_pDeviceContext->CopySubresourceRegion(pRenderTargetID->m_pStagTexture, 0, 0, 0, 0,
						pRenderTargetID->m_pOldTexture, DestSubresource, NULL);
				}
				else if (pRenderTarget->GetMulSample() != VSRenderer::MS_NONE)
				{
					VSMAC_ASSERT(pRenderTargetID->m_pStagTexture && pRenderTargetID->m_pTextureSurface);
					m_pDeviceContext->ResolveSubresource(pRenderTargetID->m_pStagTexture, 0,
						pRenderTargetID->m_pTextureSurface, 0, (DXGI_FORMAT)ms_dwTextureFormatType[pRenderTarget->GetFormatType()]);
				}
				else
				{
					VSMAC_ASSERT(pRenderTargetID->m_pStagTexture && pRenderTargetID->m_pTextureSurface);
					m_pDeviceContext->CopyResource(pRenderTargetID->m_pStagTexture, pRenderTargetID->m_pTextureSurface);
				}
			}
			
		}
		VSRenderTargetID * pRenderTargetID = EndRenderTargetsExPara.pRenderTargetID[0];
		if (pRenderTargetID->m_pSaveDepthStencilBuffer)
		{
			m_pDeviceContext->OMSetRenderTargets(EndRenderTargetsExPara.uiNum, RTVTemp, pRenderTargetID->m_pSaveDepthStencilBuffer);
			pRenderTargetID->m_pSaveDepthStencilBuffer = NULL;
		}
		else
		{
			m_pDeviceContext->OMSetRenderTargets(EndRenderTargetsExPara.uiNum, RTVTemp, DSV);
		}


		VSMAC_RELEASE(DSV);
		for (unsigned int i = 0; i < RENDERTARGET_LEVEL; i++)
		{
			VSMAC_RELEASE(RTVTemp[i]);
		}
	})
}
void VSDX11Renderer::SetDepthStencilBufferEx(VSDepthStencilID *pDepthStencilID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11SetDepthStencilBufferExCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
	VSDepthStencilID *, pDepthStencilID, pDepthStencilID, 
	{
		ID3D11DepthStencilView * DSV = NULL;
		ID3D11RenderTargetView * RTVTemp[RENDERTARGET_LEVEL] = { NULL };
		m_pDeviceContext->OMGetRenderTargets(RENDERTARGET_LEVEL, RTVTemp, &DSV);
		pDepthStencilID->m_pSaveDepthStencilBuffer = DSV;
		m_pDeviceContext->OMSetRenderTargets(RENDERTARGET_LEVEL, RTVTemp, pDepthStencilID->m_pDepthStencilBuffer);
		for (unsigned int i = 0; i < RENDERTARGET_LEVEL; i++)
		{
			VSMAC_RELEASE(RTVTemp[i]);
		}
	})
}
void VSDX11Renderer::EndDepthStencilBufferEx(VSDepthStencil * pDepthStencil, VSDepthStencilID *pDepthStencilID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11EndDepthStencilBufferExCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		VSDepthStencilID *, pDepthStencilID, pDepthStencilID, VSDepthStencil *, pDepthStencil, pDepthStencil,
	{
		ID3D11RenderTargetView * RTVTemp[RENDERTARGET_LEVEL] = { NULL };
		m_pDeviceContext->OMGetRenderTargets(RENDERTARGET_LEVEL, RTVTemp, NULL);
		m_pDeviceContext->OMSetRenderTargets(RENDERTARGET_LEVEL, RTVTemp, pDepthStencilID->m_pSaveDepthStencilBuffer);
		VSMAC_RELEASE(pDepthStencilID->m_pSaveDepthStencilBuffer);
		for (unsigned int i = 0; i < RENDERTARGET_LEVEL; i++)
		{
			VSMAC_RELEASE(RTVTemp[i]);
		}
		VSTexture * pCreateBy = (VSTexture *)pDepthStencil->GetCreateBy();
		UINT SrcSubresource = 0;
		UINT NumLevels = pCreateBy ? pCreateBy->GetMipLevel() : 1;
		UINT DestSubresource = D3D11CalcSubresource(pDepthStencil->GetLevel(),
			pDepthStencil->GetFirst(), NumLevels);
		if (pCreateBy && pDepthStencil->GetMulSample() != VSRenderer::MS_NONE)
		{
			VSMAC_ASSERT(pDepthStencilID->m_pOldTexture && pDepthStencilID->m_pTextureSurface);
			m_pDeviceContext->ResolveSubresource(pDepthStencilID->m_pOldTexture, DestSubresource,
				pDepthStencilID->m_pTextureSurface, SrcSubresource, (DXGI_FORMAT)ms_dwTextureFormatType[pDepthStencil->GetFormatType()]);

		}
		if (pDepthStencil->GetLockFlag() == VSInheritBind::LF_READONLY)
		{
			if (pCreateBy)
			{
				VSMAC_ASSERT(pDepthStencilID->m_pStagTexture && pDepthStencilID->m_pOldTexture);
				m_pDeviceContext->CopySubresourceRegion(pDepthStencilID->m_pStagTexture, 0, 0, 0, 0,
					pDepthStencilID->m_pOldTexture, DestSubresource, NULL);
			}
			else if (pDepthStencil->GetMulSample() != VSRenderer::MS_NONE)
			{
				VSMAC_ASSERT(pDepthStencilID->m_pStagTexture && pDepthStencilID->m_pTextureSurface);
				m_pDeviceContext->ResolveSubresource(pDepthStencilID->m_pStagTexture, 0,
					pDepthStencilID->m_pTextureSurface, 0, (DXGI_FORMAT)ms_dwTextureFormatType[pDepthStencil->GetFormatType()]);
			}
			else
			{
				VSMAC_ASSERT(pDepthStencilID->m_pStagTexture && pDepthStencilID->m_pTextureSurface);
				m_pDeviceContext->CopyResource(pDepthStencilID->m_pStagTexture, pDepthStencilID->m_pTextureSurface);
			}
		}
	})
}
void VSDX11Renderer::VSSetShader(VSVProgramID *pVProgramID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11VSSetShaderCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		VSVProgramID *,pVProgramID ,pVProgramID,
	{
		if (!pVProgramID)
		{
			m_pDeviceContext->VSSetShader(NULL, NULL, 0);
		}
		else
		{
			m_pDeviceContext->VSSetShader(pVProgramID->m_pVertexShader, NULL, 0);
		}
	})
}
void VSDX11Renderer::GSSetShader(VSGProgramID *pGProgramID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11GSSetShaderCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		VSGProgramID *, pGProgramID, pGProgramID,
		{
		if (!pGProgramID)
		{
			m_pDeviceContext->GSSetShader(NULL, NULL, 0);
		}
		else
		{
			m_pDeviceContext->GSSetShader(pGProgramID->m_pGeometryShader, NULL, 0);
		}
	})
}
void VSDX11Renderer::CSSetShader(VSCProgramID *pCProgramID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11CSSetShaderCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		VSCProgramID *, pCProgramID, pCProgramID,
		{
		if (!pCProgramID)
		{
			m_pDeviceContext->CSSetShader(NULL, NULL, 0);
		}
		else
		{
			m_pDeviceContext->CSSetShader(pCProgramID->m_pComputeShader, NULL, 0);
		}
		})
}
void VSDX11Renderer::HSSetShader(VSHProgramID *pHProgramID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11HSSetShaderCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		VSHProgramID *, pHProgramID, pHProgramID,
		{
		if (!pHProgramID)
		{
			m_pDeviceContext->HSSetShader(NULL, NULL, 0);
		}
		else
		{
			m_pDeviceContext->HSSetShader(pHProgramID->m_pHullShader, NULL, 0);
		}
	})
}
void VSDX11Renderer::DSSetShader(VSDProgramID *pDProgramID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11DSSetShaderCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		VSDProgramID *, pDProgramID, pDProgramID,
		{
		if (!pDProgramID)
		{
			m_pDeviceContext->DSSetShader(NULL, NULL, 0);
		}
		else
		{
			m_pDeviceContext->DSSetShader(pDProgramID->m_pDomainShader, NULL, 0);
		}
	})
}
void VSDX11Renderer::PSSetShader(VSPProgramID *pPProgramID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11PSSetShaderCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		VSPProgramID *, pPProgramID, pPProgramID,
	{
		if (!pPProgramID)
		{
			m_pDeviceContext->PSSetShader(NULL, NULL, 0);
		}
		else
		{
			m_pDeviceContext->PSSetShader(pPProgramID->m_pPixelShader, NULL, 0);
		}
	})
}
void VSDX11Renderer::OnLoadVShaderFromString(VSVShader * pVShaderProgram, VSVProgramID *pVProgramID)
{
	HRESULT hResult = NULL;
	ID3DBlob* pCode = NULL;
	ID3DBlob* pErrors = NULL;
	DWORD Flags = D3DCOMPILE_WARNINGS_ARE_ERRORS | D3DCOMPILE_ENABLE_STRICTNESS;
 #if _DEBUG 
 	Flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
 #else
	Flags = D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif
	VSMap<VSString, VSString> Define;
	pVShaderProgram->m_ShaderKey.GetDefine(Define);
	D3D_SHADER_MACRO * pMacro = GetDefine(Define);
	if (pVShaderProgram->GetBuffer().GetLength())
	{
		hResult = D3DCompile((LPCSTR)pVShaderProgram->GetBuffer().GetBuffer(), pVShaderProgram->GetBuffer().GetLength(),
			NULL, pMacro, ms_pDx11IncludeShader, pVShaderProgram->GetMainFunName().GetBuffer(),
			ms_cVertexShaderProgramVersion, Flags, 0, &pCode, &pErrors);

		pVShaderProgram->ClearBuffer();
	}
	else if (pVShaderProgram->GetFileName().GetLength())
	{
		VSString RenderAPIPre = VSRenderer::GetRenderTypeShaderPath(RAT_DIRECTX11);
		VSString Path = VSShaderMapCache::ms_ResourcePath + RenderAPIPre + pVShaderProgram->GetFileName().GetString();
		hResult = D3DX11CompileFromFile((LPCSTR)Path.GetBuffer(), pMacro, ms_pDx11IncludeShader, pVShaderProgram->GetMainFunName().GetBuffer(),
			ms_cVertexShaderProgramVersion, Flags, 0, NULL, &pCode, &pErrors, NULL);
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

	ID3D11ShaderReflection* pReflector = NULL;
	hResult = D3DReflect(pCode->GetBufferPointer(), pCode->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflector);
	VSMAC_ASSERT(!FAILED(hResult));
	D3D11_SHADER_DESC Desc;
	hResult = pReflector->GetDesc(&Desc);
	VSMAC_ASSERT(!FAILED(hResult));
	if (!pVShaderProgram->HasBindResource())
	{
		for (unsigned int uiSamplerIndex = 0; uiSamplerIndex < Desc.BoundResources; uiSamplerIndex++)
		{
			D3D11_SHADER_INPUT_BIND_DESC resDesc;
			HRESULT hr = pReflector->GetResourceBindingDesc(uiSamplerIndex, &resDesc);
			VSMAC_ASSERT(!FAILED(hResult));
			VSString RealName(resDesc.Name);
			VSString TempName;
			if (TempName.GetString(RealName, '[', 1) == true)
			{
				VSOutputDebugString(_T("Can't use [] %s"), resDesc.Name);
				VSMAC_ASSERT(0);
			}
			if (resDesc.Type == D3D_SIT_TEXTURE)
			{				
				if (resDesc.Dimension == D3D_SRV_DIMENSION_BUFFER)
				{
					VSUserBuffer * pUserBuffer = VS_NEW VSUserBuffer(resDesc.Name, VSDataBuffer::DT_MAXNUM,resDesc.BindPoint,resDesc.BindCount);
					pVShaderProgram->m_pUserBuffer.AddElement(pUserBuffer);
				}
				else
				{
					unsigned int uiType = 0;
					if (resDesc.Dimension == D3D_SRV_DIMENSION_TEXTURE1D)
					{
						uiType = VSTexture::TT_1D;
					}
					else if (resDesc.Dimension == D3D_SRV_DIMENSION_TEXTURE2D)
					{
						uiType = VSTexture::TT_2D;
					}
					else if (resDesc.Dimension == D3D_SRV_DIMENSION_TEXTURE3D)
					{
						uiType = VSTexture::TT_3D;
					}
					else if (resDesc.Dimension == D3D_SRV_DIMENSION_TEXTURECUBE)
					{
						uiType = VSTexture::TT_CUBE;
					}
					else
					{
						VSMAC_ASSERT(0);
					}
					VSUserSampler * pUerSampler = VS_NEW VSUserSampler(resDesc.Name, uiType, resDesc.BindPoint, resDesc.BindCount);
					pVShaderProgram->m_pUserSampler.AddElement(pUerSampler);
				}
				
			}
			else if (resDesc.Type == D3D_SIT_SAMPLER)
			{

			}
			else if (resDesc.Type == D3D_SIT_BYTEADDRESS)
			{
				VSUserBuffer * pUserBuffer = VS_NEW VSUserBuffer(resDesc.Name, VSDataBuffer::DT_UBYTE, resDesc.BindPoint, resDesc.BindCount);
				pVShaderProgram->m_pUserBuffer.AddElement(pUserBuffer);
			}
			else if (resDesc.Type == D3D_SIT_STRUCTURED)
			{
				VSUserBuffer * pUserBuffer = VS_NEW VSUserBuffer(resDesc.Name, VSDataBuffer::DT_STRUCT,resDesc.BindPoint, resDesc.BindCount);
				pVShaderProgram->m_pUserBuffer.AddElement(pUserBuffer);
			}
			else if (resDesc.Type == D3D_SIT_CBUFFER)
			{
				ID3D11ShaderReflectionConstantBuffer* pRCB = pReflector->GetConstantBufferByName(resDesc.Name);
				D3D11_SHADER_BUFFER_DESC cbDesc;
				pRCB->GetDesc(&cbDesc);
				VSMAC_ASSERT(resDesc.BindPoint == 0 && resDesc.BindCount == 1);
				pVShaderProgram->m_uiConstBufferSize = cbDesc.Size;
				for (unsigned int ValueID = 0; ValueID < cbDesc.Variables; ValueID++)
				{
					ID3D11ShaderReflectionVariable* pVar = pRCB->GetVariableByIndex(ValueID);
					VSMAC_ASSERT(pVar);
					ID3D11ShaderReflectionType* pVarType = pVar->GetType();
					VSMAC_ASSERT(pVarType);

					D3D11_SHADER_VARIABLE_DESC varDesc;
					hResult = pVar->GetDesc(&varDesc);
					VSMAC_ASSERT(!FAILED(hResult));

					D3D11_SHADER_TYPE_DESC varTypeDesc;
					hResult = pVarType->GetDesc(&varTypeDesc);
					VSMAC_ASSERT(!FAILED(hResult));

					unsigned int uiValueType = 0;
					if (varTypeDesc.Type == D3D_SVT_BOOL)
					{
						uiValueType = VSUserConstant::VT_BOOL;
					}
					else if (varTypeDesc.Type == D3D_SVT_INT)
					{
						uiValueType = VSUserConstant::VT_INT;
					}
					else if (varTypeDesc.Type == D3D_SVT_FLOAT)
					{
						uiValueType = VSUserConstant::VT_FLOAT;
					}
					else
					{
						VSMAC_ASSERT(0);
					}
					unsigned int RegisterNum = CalcAlign(varDesc.Size, 16) / 16;
					VSUserConstant * pUserConstant =
						VS_NEW VSUserConstant(varDesc.Name,varDesc.Size, varDesc.StartOffset, RegisterNum, uiValueType);
					pVShaderProgram->m_pUserConstant.AddElement(pUserConstant);
				}
			}
		}

		pVShaderProgram->m_bCreatePara = true;
	}
	else if (pVShaderProgram->m_bCreatePara == false)
	{
		for (unsigned int uiSamplerIndex = 0; uiSamplerIndex < Desc.BoundResources; uiSamplerIndex++)
		{
			D3D11_SHADER_INPUT_BIND_DESC resDesc;
			HRESULT hr = pReflector->GetResourceBindingDesc(uiSamplerIndex, &resDesc);
			VSMAC_ASSERT(!FAILED(hResult));
			if (resDesc.Type == D3D_SIT_CBUFFER)
			{
				ID3D11ShaderReflectionConstantBuffer* pRCB = pReflector->GetConstantBufferByName(resDesc.Name);
				D3D11_SHADER_BUFFER_DESC cbDesc;
				pRCB->GetDesc(&cbDesc);
				VSMAC_ASSERT(resDesc.BindPoint == 0 && resDesc.BindCount == 1);
				pVShaderProgram->m_uiConstBufferSize = cbDesc.Size;
				for (unsigned int ValueID = 0; ValueID < cbDesc.Variables; ValueID++)
				{
					ID3D11ShaderReflectionVariable* pVar = pRCB->GetVariableByIndex(ValueID);
					VSMAC_ASSERT(pVar);

					D3D11_SHADER_VARIABLE_DESC varDesc;
					hResult = pVar->GetDesc(&varDesc);
					VSMAC_ASSERT(!FAILED(hResult));


					for (unsigned int i = 0; i < pVShaderProgram->m_pUserConstant.GetNum(); i++)
					{
						VSUserConstant * pUserConstant = pVShaderProgram->m_pUserConstant[i];
						if (pUserConstant && pUserConstant->GetNameInShader() == varDesc.Name)
						{
							VSMAC_ASSERT(pUserConstant->m_uiSize == varDesc.Size);
							pUserConstant->m_uiRegisterIndex = varDesc.StartOffset;
							break;
						}
					}
				}
			}
		}
		pVShaderProgram->m_bCreatePara = true;
	}
	hResult = m_pDevice->CreateVertexShader((DWORD*)pCode->GetBufferPointer(), pCode->GetBufferSize(), NULL, &pVProgramID->m_pVertexShader);
	VSMAC_ASSERT(!FAILED(hResult));
	pVShaderProgram->SetCacheBuffer(pCode->GetBufferPointer(),(unsigned int)pCode->GetBufferSize());
	pVShaderProgram->m_uiArithmeticInstructionSlots = Desc.InstructionCount;
	pVShaderProgram->m_uiTextureInstructionSlots = Desc.TextureBiasInstructions +
		Desc.TextureNormalInstructions + Desc.TextureLoadInstructions + Desc.TextureCompInstructions + Desc.TextureGradientInstructions;
	VSMAC_RELEASE(pCode);

	VSMAC_RELEASE(pErrors);
}
void VSDX11Renderer::CreateVShaderFromString(VSVShader * pVShaderProgram, VSVProgramID *pVProgramID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11CreateVShaderFromStringCommand, VSDX11Renderer *, pRenderer, this,
		VSVShader *, pVShaderProgram, pVShaderProgram, VSVProgramID *, pVProgramID, pVProgramID,
	{
		pRenderer->OnLoadVShaderFromString(pVShaderProgram, pVProgramID);
	})
}
void VSDX11Renderer::CreateVShaderFromCache(VSVShader * pVShaderProgram, VSVProgramID *pVProgramID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11CreateVShaderFromCacheCommand, ID3D11Device*, m_pDevice, m_pDevice,
		VSVShader *, pVShaderProgram, pVShaderProgram, VSVProgramID *, pVProgramID, pVProgramID,
	{
		HRESULT hResult = m_pDevice->CreateVertexShader(pVShaderProgram->GetCacheBuffer(), pVShaderProgram->GetCacheBufferSize(), NULL, &pVProgramID->m_pVertexShader);
		VSMAC_ASSERT(!FAILED(hResult));
	})
}
void VSDX11Renderer::OnLoadGShaderFromString(VSGShader* pGShaderProgram, VSGProgramID *pGProgramID)
{
	HRESULT hResult = NULL;
	ID3DBlob* pCode = NULL;
	ID3DBlob* pErrors = NULL;
	DWORD Flags = D3DCOMPILE_WARNINGS_ARE_ERRORS | D3DCOMPILE_ENABLE_STRICTNESS;
#if _DEBUG 
	Flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	Flags = D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif
	VSMap<VSString, VSString> Define;
	pGShaderProgram->m_ShaderKey.GetDefine(Define);
	D3D_SHADER_MACRO * pMacro = GetDefine(Define);
	if (pGShaderProgram->GetBuffer().GetLength())
	{
		hResult = D3DCompile((LPCSTR)pGShaderProgram->GetBuffer().GetBuffer(), pGShaderProgram->GetBuffer().GetLength(),
			NULL, pMacro, ms_pDx11IncludeShader, pGShaderProgram->GetMainFunName().GetBuffer(),
			ms_cGeometryShaderProgramVersion, Flags, 0, &pCode, &pErrors);

		pGShaderProgram->ClearBuffer();
	}
	else if (pGShaderProgram->GetFileName().GetLength())
	{
		VSString RenderAPIPre = VSRenderer::GetRenderTypeShaderPath(RAT_DIRECTX11);
		VSString Path = VSShaderMapCache::ms_ResourcePath + RenderAPIPre + pGShaderProgram->GetFileName().GetString();
		hResult = D3DX11CompileFromFile((LPCSTR)Path.GetBuffer(), pMacro, ms_pDx11IncludeShader, pGShaderProgram->GetMainFunName().GetBuffer(),
			ms_cGeometryShaderProgramVersion, Flags, 0, NULL, &pCode, &pErrors, NULL);
	}
	else
		VSMAC_ASSERT(0);

	VSMAC_DELETEA(pMacro);
#if _DEBUG 
	if (pErrors && pErrors->GetBufferPointer())
	{
		VSLog Error;
		VSString VSErrorString = VSConfig::ms_OutputLogPath + _T("PSError.txt");
		Error.Open(VSErrorString.GetBuffer());
		Error.WriteInfo(pGShaderProgram->GetFileName().GetBuffer());
		Error.WriteInfo("\n");
		Error.WriteInfo((const TCHAR *)pErrors->GetBufferPointer());
		VSOutputDebugString(pGShaderProgram->GetFileName().GetBuffer());
		Error.WriteInfo("\n");
		VSOutputDebugString((const TCHAR *)pErrors->GetBufferPointer());
	}
#endif
	VSMAC_ASSERT(!FAILED(hResult));
	VSMAC_ASSERT(pCode);

	ID3D11ShaderReflection* pReflector = NULL;
	hResult = D3DReflect(pCode->GetBufferPointer(), pCode->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflector);
	VSMAC_ASSERT(!FAILED(hResult));
	D3D11_SHADER_DESC Desc;
	hResult = pReflector->GetDesc(&Desc);
	VSMAC_ASSERT(!FAILED(hResult));
	if (!pGShaderProgram->HasBindResource())
	{
		for (unsigned int uiSamplerIndex = 0; uiSamplerIndex < Desc.BoundResources; uiSamplerIndex++)
		{
			D3D11_SHADER_INPUT_BIND_DESC resDesc;
			HRESULT hr = pReflector->GetResourceBindingDesc(uiSamplerIndex, &resDesc);
			VSMAC_ASSERT(!FAILED(hResult));
			VSString RealName(resDesc.Name);
			VSString TempName;
			if (TempName.GetString(RealName, '[', 1) == true)
			{
				VSOutputDebugString(_T("Can't use [] %s"), resDesc.Name);
				VSMAC_ASSERT(0);
			}
			if (resDesc.Type == D3D_SIT_TEXTURE)
			{
				if (resDesc.Dimension == D3D_SRV_DIMENSION_BUFFER)
				{
					VSUserBuffer * pUserBuffer = VS_NEW VSUserBuffer(resDesc.Name, VSDataBuffer::DT_MAXNUM, resDesc.BindPoint, resDesc.BindCount);
					pGShaderProgram->m_pUserBuffer.AddElement(pUserBuffer);
				}
				else
				{
					unsigned int uiType = 0;
					if (resDesc.Dimension == D3D_SRV_DIMENSION_TEXTURE1D)
					{
						uiType = VSTexture::TT_1D;
					}
					else if (resDesc.Dimension == D3D_SRV_DIMENSION_TEXTURE2D)
					{
						uiType = VSTexture::TT_2D;
					}
					else if (resDesc.Dimension == D3D_SRV_DIMENSION_TEXTURE3D)
					{
						uiType = VSTexture::TT_3D;
					}
					else if (resDesc.Dimension == D3D_SRV_DIMENSION_TEXTURECUBE)
					{
						uiType = VSTexture::TT_CUBE;
					}
					else
					{
						VSMAC_ASSERT(0);
					}
					VSUserSampler * pUerSampler = VS_NEW VSUserSampler(resDesc.Name, uiType, resDesc.BindPoint, resDesc.BindCount);
					pGShaderProgram->m_pUserSampler.AddElement(pUerSampler);
				}
			}
			else if (resDesc.Type == D3D_SIT_SAMPLER)
			{

			}
			else if (resDesc.Type == D3D_SIT_BYTEADDRESS)
			{
				VSUserBuffer * pUserBuffer = VS_NEW VSUserBuffer(resDesc.Name, VSDataBuffer::DT_UBYTE, resDesc.BindPoint, resDesc.BindCount);
				pGShaderProgram->m_pUserBuffer.AddElement(pUserBuffer);
			}
			else if (resDesc.Type == D3D_SIT_STRUCTURED)
			{
				VSUserBuffer * pUserBuffer = VS_NEW VSUserBuffer(resDesc.Name, VSDataBuffer::DT_STRUCT, resDesc.BindPoint, resDesc.BindCount);
				pGShaderProgram->m_pUserBuffer.AddElement(pUserBuffer);
			}
			else if (resDesc.Type == D3D_CT_CBUFFER)
			{
				ID3D11ShaderReflectionConstantBuffer* pRCB = pReflector->GetConstantBufferByName(resDesc.Name);
				D3D11_SHADER_BUFFER_DESC cbDesc;
				pRCB->GetDesc(&cbDesc);
				VSMAC_ASSERT(resDesc.BindPoint == 0 && resDesc.BindCount == 1);
				pGShaderProgram->m_uiConstBufferSize = cbDesc.Size;
				for (unsigned int ValueID = 0; ValueID < cbDesc.Variables; ValueID++)
				{
					ID3D11ShaderReflectionVariable* pVar = pRCB->GetVariableByIndex(ValueID);
					VSMAC_ASSERT(pVar);
					ID3D11ShaderReflectionType* pVarType = pVar->GetType();
					VSMAC_ASSERT(pVarType);

					D3D11_SHADER_VARIABLE_DESC varDesc;
					hResult = pVar->GetDesc(&varDesc);
					VSMAC_ASSERT(!FAILED(hResult));

					D3D11_SHADER_TYPE_DESC varTypeDesc;
					hResult = pVarType->GetDesc(&varTypeDesc);
					VSMAC_ASSERT(!FAILED(hResult));

					unsigned int uiValueType = 0;
					if (varTypeDesc.Type == D3D_SVT_BOOL)
					{
						uiValueType = VSUserConstant::VT_BOOL;
					}
					else if (varTypeDesc.Type == D3D_SVT_INT)
					{
						uiValueType = VSUserConstant::VT_INT;
					}
					else if (varTypeDesc.Type == D3D_SVT_FLOAT)
					{
						uiValueType = VSUserConstant::VT_FLOAT;
					}
					else
					{
						VSMAC_ASSERT(0);
					}
					unsigned int RegisterNum = CalcAlign(varDesc.Size, 16) / 16;
					VSUserConstant * pUserConstant =
						VS_NEW VSUserConstant(varDesc.Name, varDesc.Size, varDesc.StartOffset, RegisterNum, uiValueType);
					pGShaderProgram->m_pUserConstant.AddElement(pUserConstant);
				}
			}
		}

		pGShaderProgram->m_bCreatePara = true;
	}
	else if (pGShaderProgram->m_bCreatePara == false)
	{
		for (unsigned int uiSamplerIndex = 0; uiSamplerIndex < Desc.BoundResources; uiSamplerIndex++)
		{
			D3D11_SHADER_INPUT_BIND_DESC resDesc;
			HRESULT hr = pReflector->GetResourceBindingDesc(uiSamplerIndex, &resDesc);
			VSMAC_ASSERT(!FAILED(hResult));
			if (resDesc.Type == D3D_CT_CBUFFER)
			{
				ID3D11ShaderReflectionConstantBuffer* pRCB = pReflector->GetConstantBufferByName(resDesc.Name);
				D3D11_SHADER_BUFFER_DESC cbDesc;
				pRCB->GetDesc(&cbDesc);
				VSMAC_ASSERT(resDesc.BindPoint == 0 && resDesc.BindCount == 1);
				pGShaderProgram->m_uiConstBufferSize = cbDesc.Size;
				for (unsigned int ValueID = 0; ValueID < cbDesc.Variables; ValueID++)
				{
					ID3D11ShaderReflectionVariable* pVar = pRCB->GetVariableByIndex(ValueID);
					VSMAC_ASSERT(pVar);

					D3D11_SHADER_VARIABLE_DESC varDesc;
					hResult = pVar->GetDesc(&varDesc);
					VSMAC_ASSERT(!FAILED(hResult));


					for (unsigned int i = 0; i < pGShaderProgram->m_pUserConstant.GetNum(); i++)
					{
						VSUserConstant * pUserConstant = pGShaderProgram->m_pUserConstant[i];
						if (pUserConstant && pUserConstant->GetNameInShader() == varDesc.Name)
						{
							VSMAC_ASSERT(pUserConstant->m_uiSize == varDesc.Size);
							pUserConstant->m_uiRegisterIndex = varDesc.StartOffset;
							break;
						}
					}
				}
			}
		}
		pGShaderProgram->m_bCreatePara = true;
	}
	hResult = m_pDevice->CreateGeometryShader((DWORD*)pCode->GetBufferPointer(), pCode->GetBufferSize(), NULL, &pGProgramID->m_pGeometryShader);
	VSMAC_ASSERT(!FAILED(hResult));
	pGShaderProgram->SetCacheBuffer(pCode->GetBufferPointer(), (unsigned int)pCode->GetBufferSize());
	pGShaderProgram->m_uiArithmeticInstructionSlots = Desc.InstructionCount;
	pGShaderProgram->m_uiTextureInstructionSlots = Desc.TextureBiasInstructions +
		Desc.TextureNormalInstructions + Desc.TextureLoadInstructions + Desc.TextureCompInstructions + Desc.TextureGradientInstructions;
	VSMAC_RELEASE(pCode);

	VSMAC_RELEASE(pErrors);
}
void VSDX11Renderer::OnLoadHShaderFromString(VSHShader* pHShaderProgram, VSHProgramID *pHProgramID)
{
	HRESULT hResult = NULL;
	ID3DBlob* pCode = NULL;
	ID3DBlob* pErrors = NULL;
	DWORD Flags = D3DCOMPILE_WARNINGS_ARE_ERRORS | D3DCOMPILE_ENABLE_STRICTNESS;
#if _DEBUG 
	Flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	Flags = D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif
	VSMap<VSString, VSString> Define;
	pHShaderProgram->m_ShaderKey.GetDefine(Define);
	D3D_SHADER_MACRO * pMacro = GetDefine(Define);
	if (pHShaderProgram->GetBuffer().GetLength())
	{
		hResult = D3DCompile((LPCSTR)pHShaderProgram->GetBuffer().GetBuffer(), pHShaderProgram->GetBuffer().GetLength(),
			NULL, pMacro, ms_pDx11IncludeShader, pHShaderProgram->GetMainFunName().GetBuffer(),
			ms_cHullShaderProgramVersion, Flags, 0, &pCode, &pErrors);

		pHShaderProgram->ClearBuffer();
	}
	else if (pHShaderProgram->GetFileName().GetLength())
	{
		VSString RenderAPIPre = VSRenderer::GetRenderTypeShaderPath(RAT_DIRECTX11);
		VSString Path = VSShaderMapCache::ms_ResourcePath + RenderAPIPre + pHShaderProgram->GetFileName().GetString();
		hResult = D3DX11CompileFromFile((LPCSTR)Path.GetBuffer(), pMacro, ms_pDx11IncludeShader, pHShaderProgram->GetMainFunName().GetBuffer(),
			ms_cHullShaderProgramVersion, Flags, 0, NULL, &pCode, &pErrors, NULL);
	}
	else
		VSMAC_ASSERT(0);

	VSMAC_DELETEA(pMacro);
#if _DEBUG 
	if (pErrors && pErrors->GetBufferPointer())
	{
		VSLog Error;
		VSString VSErrorString = VSConfig::ms_OutputLogPath + _T("PSError.txt");
		Error.Open(VSErrorString.GetBuffer());
		Error.WriteInfo(pHShaderProgram->GetFileName().GetBuffer());
		Error.WriteInfo("\n");
		Error.WriteInfo((const TCHAR *)pErrors->GetBufferPointer());
		VSOutputDebugString(pHShaderProgram->GetFileName().GetBuffer());
		Error.WriteInfo("\n");
		VSOutputDebugString((const TCHAR *)pErrors->GetBufferPointer());
	}
#endif
	VSMAC_ASSERT(!FAILED(hResult));
	VSMAC_ASSERT(pCode);

	ID3D11ShaderReflection* pReflector = NULL;
	hResult = D3DReflect(pCode->GetBufferPointer(), pCode->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflector);
	VSMAC_ASSERT(!FAILED(hResult));
	D3D11_SHADER_DESC Desc;
	hResult = pReflector->GetDesc(&Desc);
	VSMAC_ASSERT(!FAILED(hResult));
	if (!pHShaderProgram->HasBindResource())
	{
		for (unsigned int uiSamplerIndex = 0; uiSamplerIndex < Desc.BoundResources; uiSamplerIndex++)
		{
			D3D11_SHADER_INPUT_BIND_DESC resDesc;
			HRESULT hr = pReflector->GetResourceBindingDesc(uiSamplerIndex, &resDesc);
			VSMAC_ASSERT(!FAILED(hResult));
			VSString RealName(resDesc.Name);
			VSString TempName;
			if (TempName.GetString(RealName, '[', 1) == true)
			{
				VSOutputDebugString(_T("Can't use [] %s"), resDesc.Name);
				VSMAC_ASSERT(0);
			}
			if (resDesc.Type == D3D_SIT_TEXTURE)
			{
				if (resDesc.Dimension == D3D_SRV_DIMENSION_BUFFER)
				{
					VSUserBuffer * pUserBuffer = VS_NEW VSUserBuffer(resDesc.Name, VSDataBuffer::DT_MAXNUM, resDesc.BindPoint, resDesc.BindCount);
					pHShaderProgram->m_pUserBuffer.AddElement(pUserBuffer);
				}
				else
				{
					unsigned int uiType = 0;
					if (resDesc.Dimension == D3D_SRV_DIMENSION_TEXTURE1D)
					{
						uiType = VSTexture::TT_1D;
					}
					else if (resDesc.Dimension == D3D_SRV_DIMENSION_TEXTURE2D)
					{
						uiType = VSTexture::TT_2D;
					}
					else if (resDesc.Dimension == D3D_SRV_DIMENSION_TEXTURE3D)
					{
						uiType = VSTexture::TT_3D;
					}
					else if (resDesc.Dimension == D3D_SRV_DIMENSION_TEXTURECUBE)
					{
						uiType = VSTexture::TT_CUBE;
					}
					else
					{
						VSMAC_ASSERT(0);
					}
					VSUserSampler * pUerSampler = VS_NEW VSUserSampler(resDesc.Name, uiType, resDesc.BindPoint, resDesc.BindCount);
					pHShaderProgram->m_pUserSampler.AddElement(pUerSampler);
				}
			}
			else if (resDesc.Type == D3D_SIT_SAMPLER)
			{

			}
			else if (resDesc.Type == D3D_SIT_BYTEADDRESS)
			{
				VSUserBuffer * pUserBuffer = VS_NEW VSUserBuffer(resDesc.Name, VSDataBuffer::DT_UBYTE, resDesc.BindPoint, resDesc.BindCount);
				pHShaderProgram->m_pUserBuffer.AddElement(pUserBuffer);
			}
			else if (resDesc.Type == D3D_SIT_STRUCTURED)
			{
				VSUserBuffer * pUserBuffer = VS_NEW VSUserBuffer(resDesc.Name, VSDataBuffer::DT_STRUCT, resDesc.BindPoint, resDesc.BindCount);
				pHShaderProgram->m_pUserBuffer.AddElement(pUserBuffer);
			}
			else if (resDesc.Type == D3D_CT_CBUFFER)
			{
				ID3D11ShaderReflectionConstantBuffer* pRCB = pReflector->GetConstantBufferByName(resDesc.Name);
				D3D11_SHADER_BUFFER_DESC cbDesc;
				pRCB->GetDesc(&cbDesc);
				VSMAC_ASSERT(resDesc.BindPoint == 0 && resDesc.BindCount == 1);
				pHShaderProgram->m_uiConstBufferSize = cbDesc.Size;
				for (unsigned int ValueID = 0; ValueID < cbDesc.Variables; ValueID++)
				{
					ID3D11ShaderReflectionVariable* pVar = pRCB->GetVariableByIndex(ValueID);
					VSMAC_ASSERT(pVar);
					ID3D11ShaderReflectionType* pVarType = pVar->GetType();
					VSMAC_ASSERT(pVarType);

					D3D11_SHADER_VARIABLE_DESC varDesc;
					hResult = pVar->GetDesc(&varDesc);
					VSMAC_ASSERT(!FAILED(hResult));

					D3D11_SHADER_TYPE_DESC varTypeDesc;
					hResult = pVarType->GetDesc(&varTypeDesc);
					VSMAC_ASSERT(!FAILED(hResult));

					unsigned int uiValueType = 0;
					if (varTypeDesc.Type == D3D_SVT_BOOL)
					{
						uiValueType = VSUserConstant::VT_BOOL;
					}
					else if (varTypeDesc.Type == D3D_SVT_INT)
					{
						uiValueType = VSUserConstant::VT_INT;
					}
					else if (varTypeDesc.Type == D3D_SVT_FLOAT)
					{
						uiValueType = VSUserConstant::VT_FLOAT;
					}
					else
					{
						VSMAC_ASSERT(0);
					}
					unsigned int RegisterNum = CalcAlign(varDesc.Size, 16) / 16;
					VSUserConstant * pUserConstant =
						VS_NEW VSUserConstant(varDesc.Name,varDesc.Size, varDesc.StartOffset, RegisterNum, uiValueType);
					pHShaderProgram->m_pUserConstant.AddElement(pUserConstant);
				}
			}
		}

		pHShaderProgram->m_bCreatePara = true;
	}
	else if (pHShaderProgram->m_bCreatePara == false)
	{
		for (unsigned int uiSamplerIndex = 0; uiSamplerIndex < Desc.BoundResources; uiSamplerIndex++)
		{
			D3D11_SHADER_INPUT_BIND_DESC resDesc;
			HRESULT hr = pReflector->GetResourceBindingDesc(uiSamplerIndex, &resDesc);
			VSMAC_ASSERT(!FAILED(hResult));
			if (resDesc.Type == D3D_CT_CBUFFER)
			{
				ID3D11ShaderReflectionConstantBuffer* pRCB = pReflector->GetConstantBufferByName(resDesc.Name);
				D3D11_SHADER_BUFFER_DESC cbDesc;
				pRCB->GetDesc(&cbDesc);
				VSMAC_ASSERT(resDesc.BindPoint == 0 && resDesc.BindCount == 1);
				pHShaderProgram->m_uiConstBufferSize = cbDesc.Size;
				for (unsigned int ValueID = 0; ValueID < cbDesc.Variables; ValueID++)
				{
					ID3D11ShaderReflectionVariable* pVar = pRCB->GetVariableByIndex(ValueID);
					VSMAC_ASSERT(pVar);

					D3D11_SHADER_VARIABLE_DESC varDesc;
					hResult = pVar->GetDesc(&varDesc);
					VSMAC_ASSERT(!FAILED(hResult));


					for (unsigned int i = 0; i < pHShaderProgram->m_pUserConstant.GetNum(); i++)
					{
						VSUserConstant * pUserConstant = pHShaderProgram->m_pUserConstant[i];
						if (pUserConstant && pUserConstant->GetNameInShader() == varDesc.Name)
						{
							VSMAC_ASSERT(pUserConstant->m_uiSize == varDesc.Size);
							pUserConstant->m_uiRegisterIndex = varDesc.StartOffset;
							break;
						}
					}
				}
			}
		}
		pHShaderProgram->m_bCreatePara = true;
	}
	hResult = m_pDevice->CreateHullShader((DWORD*)pCode->GetBufferPointer(), pCode->GetBufferSize(), NULL, &pHProgramID->m_pHullShader);
	VSMAC_ASSERT(!FAILED(hResult));
	pHShaderProgram->SetCacheBuffer(pCode->GetBufferPointer(), (unsigned int)pCode->GetBufferSize());
	pHShaderProgram->m_uiArithmeticInstructionSlots = Desc.InstructionCount;
	pHShaderProgram->m_uiTextureInstructionSlots = Desc.TextureBiasInstructions +
		Desc.TextureNormalInstructions + Desc.TextureLoadInstructions + Desc.TextureCompInstructions + Desc.TextureGradientInstructions;
	VSMAC_RELEASE(pCode);

	VSMAC_RELEASE(pErrors);
}
void VSDX11Renderer::OnLoadDShaderFromString(VSDShader* pDShaderProgram, VSDProgramID *pDProgramID)
{
	HRESULT hResult = NULL;
	ID3DBlob* pCode = NULL;
	ID3DBlob* pErrors = NULL;
	DWORD Flags = D3DCOMPILE_WARNINGS_ARE_ERRORS | D3DCOMPILE_ENABLE_STRICTNESS;
#if _DEBUG 
	Flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	Flags = D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif
	VSMap<VSString, VSString> Define;
	pDShaderProgram->m_ShaderKey.GetDefine(Define);
	D3D_SHADER_MACRO * pMacro = GetDefine(Define);
	if (pDShaderProgram->GetBuffer().GetLength())
	{
		hResult = D3DCompile((LPCSTR)pDShaderProgram->GetBuffer().GetBuffer(), pDShaderProgram->GetBuffer().GetLength(),
			NULL, pMacro, ms_pDx11IncludeShader, pDShaderProgram->GetMainFunName().GetBuffer(),
			ms_cDomainShaderProgramVersion, Flags, 0, &pCode, &pErrors);
		pDShaderProgram->ClearBuffer();

	}
	else if (pDShaderProgram->GetFileName().GetLength())
	{
		VSString RenderAPIPre = VSRenderer::GetRenderTypeShaderPath(RAT_DIRECTX11);
		VSString Path = VSShaderMapCache::ms_ResourcePath + RenderAPIPre + pDShaderProgram->GetFileName().GetString();
		hResult = D3DX11CompileFromFile((LPCSTR)Path.GetBuffer(), pMacro, ms_pDx11IncludeShader, pDShaderProgram->GetMainFunName().GetBuffer(),
			ms_cDomainShaderProgramVersion, Flags, 0, NULL, &pCode, &pErrors, NULL);
	}
	else
		VSMAC_ASSERT(0);

	VSMAC_DELETEA(pMacro);
#if _DEBUG 
	if (pErrors && pErrors->GetBufferPointer())
	{
		VSLog Error;
		VSString VSErrorString = VSConfig::ms_OutputLogPath + _T("PSError.txt");
		Error.Open(VSErrorString.GetBuffer());
		Error.WriteInfo(pDShaderProgram->GetFileName().GetBuffer());
		Error.WriteInfo("\n");
		Error.WriteInfo((const TCHAR *)pErrors->GetBufferPointer());
		VSOutputDebugString(pDShaderProgram->GetFileName().GetBuffer());
		Error.WriteInfo("\n");
		VSOutputDebugString((const TCHAR *)pErrors->GetBufferPointer());
	}
#endif
	VSMAC_ASSERT(!FAILED(hResult));
	VSMAC_ASSERT(pCode);

	ID3D11ShaderReflection* pReflector = NULL;
	hResult = D3DReflect(pCode->GetBufferPointer(), pCode->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflector);
	VSMAC_ASSERT(!FAILED(hResult));
	D3D11_SHADER_DESC Desc;
	hResult = pReflector->GetDesc(&Desc);
	VSMAC_ASSERT(!FAILED(hResult));
	if (!pDShaderProgram->HasBindResource())
	{
		for (unsigned int uiSamplerIndex = 0; uiSamplerIndex < Desc.BoundResources; uiSamplerIndex++)
		{
			D3D11_SHADER_INPUT_BIND_DESC resDesc;
			HRESULT hr = pReflector->GetResourceBindingDesc(uiSamplerIndex, &resDesc);
			VSMAC_ASSERT(!FAILED(hResult));
			VSString RealName(resDesc.Name);
			VSString TempName;
			if (TempName.GetString(RealName, '[', 1) == true)
			{
				VSOutputDebugString(_T("Can't use [] %s"), resDesc.Name);
				VSMAC_ASSERT(0);
			}
			if (resDesc.Type == D3D_SIT_TEXTURE)
			{
				if (resDesc.Dimension == D3D_SRV_DIMENSION_BUFFER)
				{
					VSUserBuffer * pUserBuffer = VS_NEW VSUserBuffer(resDesc.Name, VSDataBuffer::DT_MAXNUM, resDesc.BindPoint, resDesc.BindCount);
					pDShaderProgram->m_pUserBuffer.AddElement(pUserBuffer);
				}
				else
				{
					unsigned int uiType = 0;
					if (resDesc.Dimension == D3D_SRV_DIMENSION_TEXTURE1D)
					{
						uiType = VSTexture::TT_1D;
					}
					else if (resDesc.Dimension == D3D_SRV_DIMENSION_TEXTURE2D)
					{
						uiType = VSTexture::TT_2D;
					}
					else if (resDesc.Dimension == D3D_SRV_DIMENSION_TEXTURE3D)
					{
						uiType = VSTexture::TT_3D;
					}
					else if (resDesc.Dimension == D3D_SRV_DIMENSION_TEXTURECUBE)
					{
						uiType = VSTexture::TT_CUBE;
					}
					else
					{
						VSMAC_ASSERT(0);
					}
					VSUserSampler * pUerSampler = VS_NEW VSUserSampler(resDesc.Name, uiType, resDesc.BindPoint, resDesc.BindCount);
					pDShaderProgram->m_pUserSampler.AddElement(pUerSampler);
				}
			}
			else if (resDesc.Type == D3D_SIT_SAMPLER)
			{

			}
			else if (resDesc.Type == D3D_SIT_BYTEADDRESS)
			{
				VSUserBuffer * pUserBuffer = VS_NEW VSUserBuffer(resDesc.Name, VSDataBuffer::DT_UBYTE, resDesc.BindPoint, resDesc.BindCount);
				pDShaderProgram->m_pUserBuffer.AddElement(pUserBuffer);
			}
			else if (resDesc.Type == D3D_SIT_STRUCTURED)
			{
				VSUserBuffer * pUserBuffer = VS_NEW VSUserBuffer(resDesc.Name, VSDataBuffer::DT_STRUCT, resDesc.BindPoint, resDesc.BindCount);
				pDShaderProgram->m_pUserBuffer.AddElement(pUserBuffer);
			}
			else if (resDesc.Type == D3D_CT_CBUFFER)
			{
				ID3D11ShaderReflectionConstantBuffer* pRCB = pReflector->GetConstantBufferByName(resDesc.Name);
				D3D11_SHADER_BUFFER_DESC cbDesc;
				pRCB->GetDesc(&cbDesc);
				VSMAC_ASSERT(resDesc.BindPoint == 0 && resDesc.BindCount == 1);
				pDShaderProgram->m_uiConstBufferSize = cbDesc.Size;
				for (unsigned int ValueID = 0; ValueID < cbDesc.Variables; ValueID++)
				{
					ID3D11ShaderReflectionVariable* pVar = pRCB->GetVariableByIndex(ValueID);
					VSMAC_ASSERT(pVar);
					ID3D11ShaderReflectionType* pVarType = pVar->GetType();
					VSMAC_ASSERT(pVarType);

					D3D11_SHADER_VARIABLE_DESC varDesc;
					hResult = pVar->GetDesc(&varDesc);
					VSMAC_ASSERT(!FAILED(hResult));

					D3D11_SHADER_TYPE_DESC varTypeDesc;
					hResult = pVarType->GetDesc(&varTypeDesc);
					VSMAC_ASSERT(!FAILED(hResult));

					unsigned int uiValueType = 0;
					if (varTypeDesc.Type == D3D_SVT_BOOL)
					{
						uiValueType = VSUserConstant::VT_BOOL;
					}
					else if (varTypeDesc.Type == D3D_SVT_INT)
					{
						uiValueType = VSUserConstant::VT_INT;
					}
					else if (varTypeDesc.Type == D3D_SVT_FLOAT)
					{
						uiValueType = VSUserConstant::VT_FLOAT;
					}
					else
					{
						VSMAC_ASSERT(0);
					}
					unsigned int RegisterNum = CalcAlign(varDesc.Size, 16) / 16;
					VSUserConstant * pUserConstant =
						VS_NEW VSUserConstant(varDesc.Name,varDesc.Size, varDesc.StartOffset, RegisterNum, uiValueType);
					pDShaderProgram->m_pUserConstant.AddElement(pUserConstant);
				}
			}
		}

		pDShaderProgram->m_bCreatePara = true;
	}
	else if (pDShaderProgram->m_bCreatePara == false)
	{
		for (unsigned int uiSamplerIndex = 0; uiSamplerIndex < Desc.BoundResources; uiSamplerIndex++)
		{
			D3D11_SHADER_INPUT_BIND_DESC resDesc;
			HRESULT hr = pReflector->GetResourceBindingDesc(uiSamplerIndex, &resDesc);
			VSMAC_ASSERT(!FAILED(hResult));
			if (resDesc.Type == D3D_CT_CBUFFER)
			{
				ID3D11ShaderReflectionConstantBuffer* pRCB = pReflector->GetConstantBufferByName(resDesc.Name);
				D3D11_SHADER_BUFFER_DESC cbDesc;
				pRCB->GetDesc(&cbDesc);
				VSMAC_ASSERT(resDesc.BindPoint == 0 && resDesc.BindCount == 1);
				pDShaderProgram->m_uiConstBufferSize = cbDesc.Size;
				for (unsigned int ValueID = 0; ValueID < cbDesc.Variables; ValueID++)
				{
					ID3D11ShaderReflectionVariable* pVar = pRCB->GetVariableByIndex(ValueID);
					VSMAC_ASSERT(pVar);

					D3D11_SHADER_VARIABLE_DESC varDesc;
					hResult = pVar->GetDesc(&varDesc);
					VSMAC_ASSERT(!FAILED(hResult));


					for (unsigned int i = 0; i < pDShaderProgram->m_pUserConstant.GetNum(); i++)
					{
						VSUserConstant * pUserConstant = pDShaderProgram->m_pUserConstant[i];
						if (pUserConstant && pUserConstant->GetNameInShader() == varDesc.Name)
						{
							VSMAC_ASSERT(pUserConstant->m_uiSize == varDesc.Size);
							pUserConstant->m_uiRegisterIndex = varDesc.StartOffset;
							break;
						}
					}
				}
			}
		}
		pDShaderProgram->m_bCreatePara = true;
	}
	hResult = m_pDevice->CreateDomainShader((DWORD*)pCode->GetBufferPointer(), pCode->GetBufferSize(), NULL, &pDProgramID->m_pDomainShader);
	VSMAC_ASSERT(!FAILED(hResult));
	pDShaderProgram->SetCacheBuffer(pCode->GetBufferPointer(), (unsigned int)pCode->GetBufferSize());
	pDShaderProgram->m_uiArithmeticInstructionSlots = Desc.InstructionCount;
	pDShaderProgram->m_uiTextureInstructionSlots = Desc.TextureBiasInstructions +
		Desc.TextureNormalInstructions + Desc.TextureLoadInstructions + Desc.TextureCompInstructions + Desc.TextureGradientInstructions;
	VSMAC_RELEASE(pCode);

	VSMAC_RELEASE(pErrors);
}
void VSDX11Renderer::OnLoadCShaderFromString(VSCShader* pCShaderProgram, VSCProgramID *pCProgramID)
{
	HRESULT hResult = NULL;
	ID3DBlob* pCode = NULL;
	ID3DBlob* pErrors = NULL;
	DWORD Flags = D3DCOMPILE_WARNINGS_ARE_ERRORS | D3DCOMPILE_ENABLE_STRICTNESS;
#if _DEBUG 
	Flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	Flags = D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif
	VSMap<VSString, VSString> Define;
	pCShaderProgram->m_ShaderKey.GetDefine(Define);
	D3D_SHADER_MACRO * pMacro = GetDefine(Define);
	if (pCShaderProgram->GetBuffer().GetLength())
	{
		hResult = D3DCompile((LPCSTR)pCShaderProgram->GetBuffer().GetBuffer(), pCShaderProgram->GetBuffer().GetLength(),
			NULL, pMacro, ms_pDx11IncludeShader, pCShaderProgram->GetMainFunName().GetBuffer(),
			ms_cComputerShaderProgramVersion, Flags, 0, &pCode, &pErrors);

		pCShaderProgram->ClearBuffer();
	}
	else if (pCShaderProgram->GetFileName().GetLength())
	{
		VSString RenderAPIPre = VSRenderer::GetRenderTypeShaderPath(RAT_DIRECTX11);
		VSString Path = VSShaderMapCache::ms_ResourcePath + RenderAPIPre + pCShaderProgram->GetFileName().GetString();
		hResult = D3DX11CompileFromFile((LPCSTR)Path.GetBuffer(), pMacro, ms_pDx11IncludeShader, pCShaderProgram->GetMainFunName().GetBuffer(),
			ms_cComputerShaderProgramVersion, Flags, 0, NULL, &pCode, &pErrors, NULL);
	}
	else
		VSMAC_ASSERT(0);

	VSMAC_DELETEA(pMacro);
#if _DEBUG 
	if (pErrors && pErrors->GetBufferPointer())
	{
		VSLog Error;
		VSString VSErrorString = VSConfig::ms_OutputLogPath + _T("PSError.txt");
		Error.Open(VSErrorString.GetBuffer());
		Error.WriteInfo(pCShaderProgram->GetFileName().GetBuffer());
		Error.WriteInfo("\n");
		Error.WriteInfo((const TCHAR *)pErrors->GetBufferPointer());
		VSOutputDebugString(pCShaderProgram->GetFileName().GetBuffer());
		Error.WriteInfo("\n");
		VSOutputDebugString((const TCHAR *)pErrors->GetBufferPointer());
	}
#endif
	VSMAC_ASSERT(!FAILED(hResult));
	VSMAC_ASSERT(pCode);

	ID3D11ShaderReflection* pReflector = NULL;
	hResult = D3DReflect(pCode->GetBufferPointer(), pCode->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflector);
	VSMAC_ASSERT(!FAILED(hResult));
	D3D11_SHADER_DESC Desc;
	hResult = pReflector->GetDesc(&Desc);
	VSMAC_ASSERT(!FAILED(hResult));
	if (!pCShaderProgram->HasBindResource())
	{
		for (unsigned int uiSamplerIndex = 0; uiSamplerIndex < Desc.BoundResources; uiSamplerIndex++)
		{
			D3D11_SHADER_INPUT_BIND_DESC resDesc;
			HRESULT hr = pReflector->GetResourceBindingDesc(uiSamplerIndex, &resDesc);
			VSMAC_ASSERT(!FAILED(hResult));
			VSString RealName(resDesc.Name);
			VSString TempName;
			if (TempName.GetString(RealName, '[', 1) == true)
			{
				VSOutputDebugString(_T("Can't use [] %s"), resDesc.Name);
				VSMAC_ASSERT(0);
			}
			if (resDesc.Type == D3D_SIT_TEXTURE)
			{
				if (resDesc.Dimension == D3D_SRV_DIMENSION_BUFFER)
				{
					VSUserBuffer * pUserBuffer = VS_NEW VSUserBuffer(resDesc.Name, VSDataBuffer::DT_MAXNUM, resDesc.BindPoint, resDesc.BindCount);
					pCShaderProgram->m_pUserBuffer.AddElement(pUserBuffer);
				}
				else
				{
					unsigned int uiType = 0;
					if (resDesc.Dimension == D3D_SRV_DIMENSION_TEXTURE1D)
					{
						uiType = VSTexture::TT_1D;
					}
					else if (resDesc.Dimension == D3D_SRV_DIMENSION_TEXTURE2D)
					{
						uiType = VSTexture::TT_2D;
					}
					else if (resDesc.Dimension == D3D_SRV_DIMENSION_TEXTURE3D)
					{
						uiType = VSTexture::TT_3D;
					}
					else if (resDesc.Dimension == D3D_SRV_DIMENSION_TEXTURECUBE)
					{
						uiType = VSTexture::TT_CUBE;
					}
					else
					{
						VSMAC_ASSERT(0);
					}
					VSUserSampler * pUerSampler = VS_NEW VSUserSampler(resDesc.Name, uiType, resDesc.BindPoint, resDesc.BindCount);
					pCShaderProgram->m_pUserSampler.AddElement(pUerSampler);
				}
			}
			else if (resDesc.Type == D3D_SIT_SAMPLER)
			{

			}
			else if (resDesc.Type == D3D_SIT_BYTEADDRESS)
			{
				VSUserBuffer * pUserBuffer = VS_NEW VSUserBuffer(resDesc.Name, VSDataBuffer::DT_UBYTE, resDesc.BindPoint, resDesc.BindCount);
				pCShaderProgram->m_pUserBuffer.AddElement(pUserBuffer);
			}
			else if (resDesc.Type == D3D_SIT_STRUCTURED)
			{
				VSUserBuffer * pUserBuffer = VS_NEW VSUserBuffer(resDesc.Name, VSDataBuffer::DT_STRUCT, resDesc.BindPoint, resDesc.BindCount);
				pCShaderProgram->m_pUserBuffer.AddElement(pUserBuffer);
			}
			else if (resDesc.Type == D3D_CT_CBUFFER)
			{
				ID3D11ShaderReflectionConstantBuffer* pRCB = pReflector->GetConstantBufferByName(resDesc.Name);
				D3D11_SHADER_BUFFER_DESC cbDesc;
				pRCB->GetDesc(&cbDesc);
				VSMAC_ASSERT(resDesc.BindPoint == 0 && resDesc.BindCount == 1);
				pCShaderProgram->m_uiConstBufferSize = cbDesc.Size;
				for (unsigned int ValueID = 0; ValueID < cbDesc.Variables; ValueID++)
				{
					ID3D11ShaderReflectionVariable* pVar = pRCB->GetVariableByIndex(ValueID);
					VSMAC_ASSERT(pVar);
					ID3D11ShaderReflectionType* pVarType = pVar->GetType();
					VSMAC_ASSERT(pVarType);

					D3D11_SHADER_VARIABLE_DESC varDesc;
					hResult = pVar->GetDesc(&varDesc);
					VSMAC_ASSERT(!FAILED(hResult));

					D3D11_SHADER_TYPE_DESC varTypeDesc;
					hResult = pVarType->GetDesc(&varTypeDesc);
					VSMAC_ASSERT(!FAILED(hResult));

					unsigned int uiValueType = 0;
					if (varTypeDesc.Type == D3D_SVT_BOOL)
					{
						uiValueType = VSUserConstant::VT_BOOL;
					}
					else if (varTypeDesc.Type == D3D_SVT_INT)
					{
						uiValueType = VSUserConstant::VT_INT;
					}
					else if (varTypeDesc.Type == D3D_SVT_FLOAT)
					{
						uiValueType = VSUserConstant::VT_FLOAT;
					}
					else
					{
						VSMAC_ASSERT(0);
					}
					unsigned int RegisterNum = CalcAlign(varDesc.Size, 16) / 16;
					VSUserConstant * pUserConstant =
						VS_NEW VSUserConstant(varDesc.Name,varDesc.Size, varDesc.StartOffset, RegisterNum, uiValueType);
					pCShaderProgram->m_pUserConstant.AddElement(pUserConstant);
				}
			}
		}

		pCShaderProgram->m_bCreatePara = true;
	}
	else if (pCShaderProgram->m_bCreatePara == false)
	{
		for (unsigned int uiSamplerIndex = 0; uiSamplerIndex < Desc.BoundResources; uiSamplerIndex++)
		{
			D3D11_SHADER_INPUT_BIND_DESC resDesc;
			HRESULT hr = pReflector->GetResourceBindingDesc(uiSamplerIndex, &resDesc);
			VSMAC_ASSERT(!FAILED(hResult));
			if (resDesc.Type == D3D_CT_CBUFFER)
			{
				ID3D11ShaderReflectionConstantBuffer* pRCB = pReflector->GetConstantBufferByName(resDesc.Name);
				D3D11_SHADER_BUFFER_DESC cbDesc;
				pRCB->GetDesc(&cbDesc);
				VSMAC_ASSERT(resDesc.BindPoint == 0 && resDesc.BindCount == 1);
				pCShaderProgram->m_uiConstBufferSize = cbDesc.Size;
				for (unsigned int ValueID = 0; ValueID < cbDesc.Variables; ValueID++)
				{
					ID3D11ShaderReflectionVariable* pVar = pRCB->GetVariableByIndex(ValueID);
					VSMAC_ASSERT(pVar);

					D3D11_SHADER_VARIABLE_DESC varDesc;
					hResult = pVar->GetDesc(&varDesc);
					VSMAC_ASSERT(!FAILED(hResult));


					for (unsigned int i = 0; i < pCShaderProgram->m_pUserConstant.GetNum(); i++)
					{
						VSUserConstant * pUserConstant = pCShaderProgram->m_pUserConstant[i];
						if (pUserConstant && pUserConstant->GetNameInShader() == varDesc.Name)
						{
							VSMAC_ASSERT(pUserConstant->m_uiSize == varDesc.Size);
							pUserConstant->m_uiRegisterIndex = varDesc.StartOffset;
							break;
						}
					}
				}
			}
		}
		pCShaderProgram->m_bCreatePara = true;
	}
	hResult = m_pDevice->CreateComputeShader((DWORD*)pCode->GetBufferPointer(), pCode->GetBufferSize(), NULL, &pCProgramID->m_pComputeShader);
	VSMAC_ASSERT(!FAILED(hResult));
	pCShaderProgram->SetCacheBuffer(pCode->GetBufferPointer(), (unsigned int)pCode->GetBufferSize());
	pCShaderProgram->m_uiArithmeticInstructionSlots = Desc.InstructionCount;
	pCShaderProgram->m_uiTextureInstructionSlots = Desc.TextureBiasInstructions +
		Desc.TextureNormalInstructions + Desc.TextureLoadInstructions + Desc.TextureCompInstructions + Desc.TextureGradientInstructions;
	VSMAC_RELEASE(pCode);

	VSMAC_RELEASE(pErrors);
}
void VSDX11Renderer::OnLoadPShaderFromString(VSPShader* pPShaderProgram, VSPProgramID *pPProgramID)
{
	HRESULT hResult = NULL;
	ID3DBlob* pCode = NULL;
	ID3DBlob* pErrors = NULL;
	DWORD Flags = D3DCOMPILE_WARNINGS_ARE_ERRORS | D3DCOMPILE_ENABLE_STRICTNESS;
#if _DEBUG 
	Flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	Flags = D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif
	VSMap<VSString, VSString> Define;
	pPShaderProgram->m_ShaderKey.GetDefine(Define);
	D3D_SHADER_MACRO * pMacro = GetDefine(Define);
	if (pPShaderProgram->GetBuffer().GetLength())
	{
		hResult = D3DCompile((LPCSTR)pPShaderProgram->GetBuffer().GetBuffer(), pPShaderProgram->GetBuffer().GetLength(),
			NULL, pMacro, ms_pDx11IncludeShader, pPShaderProgram->GetMainFunName().GetBuffer(),
			ms_cPixelShaderProgramVersion, Flags, 0, &pCode, &pErrors);

		pPShaderProgram->ClearBuffer();
	}
	else if (pPShaderProgram->GetFileName().GetLength())
	{
		VSString RenderAPIPre = VSRenderer::GetRenderTypeShaderPath(RAT_DIRECTX11);
		VSString Path = VSShaderMapCache::ms_ResourcePath + RenderAPIPre + pPShaderProgram->GetFileName().GetString();
		hResult = D3DX11CompileFromFile((LPCSTR)Path.GetBuffer(), pMacro, ms_pDx11IncludeShader, pPShaderProgram->GetMainFunName().GetBuffer(),
			ms_cPixelShaderProgramVersion, Flags, 0, NULL, &pCode, &pErrors, NULL);
	}
	else
		VSMAC_ASSERT(0);

	VSMAC_DELETEA(pMacro);
#if _DEBUG 
	if (pErrors && pErrors->GetBufferPointer())
	{
		VSLog Error;
		VSString VSErrorString = VSConfig::ms_OutputLogPath + _T("PSError.txt");
		Error.Open(VSErrorString.GetBuffer());
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

	ID3D11ShaderReflection* pReflector = NULL;
	hResult = D3DReflect(pCode->GetBufferPointer(), pCode->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflector);
	VSMAC_ASSERT(!FAILED(hResult));
	D3D11_SHADER_DESC Desc;
	hResult = pReflector->GetDesc(&Desc);
	VSMAC_ASSERT(!FAILED(hResult));
	if (!pPShaderProgram->HasBindResource())
	{
		for (unsigned int uiSamplerIndex = 0; uiSamplerIndex < Desc.BoundResources; uiSamplerIndex++)
		{
			D3D11_SHADER_INPUT_BIND_DESC resDesc;
			HRESULT hr = pReflector->GetResourceBindingDesc(uiSamplerIndex, &resDesc);
			VSMAC_ASSERT(!FAILED(hResult));
			VSString RealName(resDesc.Name);
			VSString TempName;
			if (TempName.GetString(RealName, '[', 1) == true)
			{
				VSOutputDebugString(_T("Can't use [] %s"), resDesc.Name);
				VSMAC_ASSERT(0);
			}
			if (resDesc.Type == D3D_SIT_TEXTURE)
			{
				if (resDesc.Dimension == D3D_SRV_DIMENSION_BUFFER)
				{
					VSUserBuffer * pUserBuffer = VS_NEW VSUserBuffer(resDesc.Name, VSDataBuffer::DT_MAXNUM, resDesc.BindPoint, resDesc.BindCount);
					pPShaderProgram->m_pUserBuffer.AddElement(pUserBuffer);
				}
				else
				{
					unsigned int uiType = 0;
					if (resDesc.Dimension == D3D_SRV_DIMENSION_TEXTURE1D)
					{
						uiType = VSTexture::TT_1D;
					}
					else if (resDesc.Dimension == D3D_SRV_DIMENSION_TEXTURE2D)
					{
						uiType = VSTexture::TT_2D;
					}
					else if (resDesc.Dimension == D3D_SRV_DIMENSION_TEXTURE3D)
					{
						uiType = VSTexture::TT_3D;
					}
					else if (resDesc.Dimension == D3D_SRV_DIMENSION_TEXTURECUBE)
					{
						uiType = VSTexture::TT_CUBE;
					}
					else
					{
						VSMAC_ASSERT(0);
					}
					VSUserSampler * pUerSampler = VS_NEW VSUserSampler(resDesc.Name, uiType, resDesc.BindPoint, resDesc.BindCount);
					pPShaderProgram->m_pUserSampler.AddElement(pUerSampler);
				}
			}
			else if (resDesc.Type == D3D_SIT_SAMPLER)
			{

			}
			else if (resDesc.Type == D3D_SIT_BYTEADDRESS)
			{
				VSUserBuffer * pUserBuffer = VS_NEW VSUserBuffer(resDesc.Name, VSDataBuffer::DT_UBYTE, resDesc.BindPoint, resDesc.BindCount);
				pPShaderProgram->m_pUserBuffer.AddElement(pUserBuffer);
			}
			else if (resDesc.Type == D3D_SIT_STRUCTURED)
			{
				VSUserBuffer * pUserBuffer = VS_NEW VSUserBuffer(resDesc.Name, VSDataBuffer::DT_STRUCT, resDesc.BindPoint, resDesc.BindCount);
				pPShaderProgram->m_pUserBuffer.AddElement(pUserBuffer);
			}
			else if (resDesc.Type == D3D_CT_CBUFFER)
			{
				ID3D11ShaderReflectionConstantBuffer* pRCB = pReflector->GetConstantBufferByName(resDesc.Name);
				D3D11_SHADER_BUFFER_DESC cbDesc;
				pRCB->GetDesc(&cbDesc);
				VSMAC_ASSERT(resDesc.BindPoint == 0 && resDesc.BindCount == 1);
				pPShaderProgram->m_uiConstBufferSize = cbDesc.Size;
				for (unsigned int ValueID = 0; ValueID < cbDesc.Variables; ValueID++)
				{
					ID3D11ShaderReflectionVariable* pVar = pRCB->GetVariableByIndex(ValueID);
					VSMAC_ASSERT(pVar);
					ID3D11ShaderReflectionType* pVarType = pVar->GetType();
					VSMAC_ASSERT(pVarType);

					D3D11_SHADER_VARIABLE_DESC varDesc;
					hResult = pVar->GetDesc(&varDesc);
					VSMAC_ASSERT(!FAILED(hResult));

					D3D11_SHADER_TYPE_DESC varTypeDesc;
					hResult = pVarType->GetDesc(&varTypeDesc);
					VSMAC_ASSERT(!FAILED(hResult));

					unsigned int uiValueType = 0;
					if (varTypeDesc.Type == D3D_SVT_BOOL)
					{
						uiValueType = VSUserConstant::VT_BOOL;
					}
					else if (varTypeDesc.Type == D3D_SVT_INT)
					{
						uiValueType = VSUserConstant::VT_INT;
					}
					else if (varTypeDesc.Type == D3D_SVT_FLOAT)
					{
						uiValueType = VSUserConstant::VT_FLOAT;
					}
					else
					{
						VSMAC_ASSERT(0);
					}
					unsigned int RegisterNum = CalcAlign(varDesc.Size, 16) / 16;
					VSUserConstant * pUserConstant =
						VS_NEW VSUserConstant(varDesc.Name,varDesc.Size, varDesc.StartOffset, RegisterNum, uiValueType);
					pPShaderProgram->m_pUserConstant.AddElement(pUserConstant);
				}
			}
		}

		pPShaderProgram->m_bCreatePara = true;
	}
	else if (pPShaderProgram->m_bCreatePara == false)
	{
		for (unsigned int uiSamplerIndex = 0; uiSamplerIndex < Desc.BoundResources; uiSamplerIndex++)
		{
			D3D11_SHADER_INPUT_BIND_DESC resDesc;
			HRESULT hr = pReflector->GetResourceBindingDesc(uiSamplerIndex, &resDesc);
			VSMAC_ASSERT(!FAILED(hResult));
			if (resDesc.Type == D3D_CT_CBUFFER)
			{
				ID3D11ShaderReflectionConstantBuffer* pRCB = pReflector->GetConstantBufferByName(resDesc.Name);
				D3D11_SHADER_BUFFER_DESC cbDesc;
				pRCB->GetDesc(&cbDesc);
				VSMAC_ASSERT(resDesc.BindPoint == 0 && resDesc.BindCount == 1);
				pPShaderProgram->m_uiConstBufferSize = cbDesc.Size;
				for (unsigned int ValueID = 0; ValueID < cbDesc.Variables; ValueID++)
				{
					ID3D11ShaderReflectionVariable* pVar = pRCB->GetVariableByIndex(ValueID);
					VSMAC_ASSERT(pVar);

					D3D11_SHADER_VARIABLE_DESC varDesc;
					hResult = pVar->GetDesc(&varDesc);
					VSMAC_ASSERT(!FAILED(hResult));


					for (unsigned int i = 0; i < pPShaderProgram->m_pUserConstant.GetNum(); i++)
					{
						VSUserConstant * pUserConstant = pPShaderProgram->m_pUserConstant[i];
						if (pUserConstant && pUserConstant->GetNameInShader() == varDesc.Name)
						{
							VSMAC_ASSERT(pUserConstant->m_uiSize == varDesc.Size);
							pUserConstant->m_uiRegisterIndex = varDesc.StartOffset;
							break;
						}
					}
				}
			}
		}
		pPShaderProgram->m_bCreatePara = true;
	}
	hResult = m_pDevice->CreatePixelShader((DWORD*)pCode->GetBufferPointer(), pCode->GetBufferSize(), NULL, &pPProgramID->m_pPixelShader);
	VSMAC_ASSERT(!FAILED(hResult));
	pPShaderProgram->SetCacheBuffer(pCode->GetBufferPointer(), (unsigned int)pCode->GetBufferSize());
	pPShaderProgram->m_uiArithmeticInstructionSlots = Desc.InstructionCount;
	pPShaderProgram->m_uiTextureInstructionSlots = Desc.TextureBiasInstructions +
		Desc.TextureNormalInstructions + Desc.TextureLoadInstructions + Desc.TextureCompInstructions + Desc.TextureGradientInstructions;
	VSMAC_RELEASE(pCode);

	VSMAC_RELEASE(pErrors);
}
void VSDX11Renderer::CreatePShaderFromString(VSPShader* pPShaderProgram, VSPProgramID *pPProgramID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11CreatePShaderFromStringCommand, VSDX11Renderer *, pRenderer, this,
		VSPShader *, pPShaderProgram, pPShaderProgram, VSPProgramID *, pPProgramID, pPProgramID,
	{
		pRenderer->OnLoadPShaderFromString(pPShaderProgram, pPProgramID);
	})
}
void VSDX11Renderer::CreateGShaderFromString(VSGShader* pGShaderProgram, VSGProgramID *pGProgramID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11CreateGShaderFromStringCommand, VSDX11Renderer *, pRenderer, this,
		VSGShader *, pGShaderProgram, pGShaderProgram, VSGProgramID *, pGProgramID, pGProgramID,
		{
		pRenderer->OnLoadGShaderFromString(pGShaderProgram, pGProgramID);
	})
}
void VSDX11Renderer::CreateDShaderFromString(VSDShader* pDShaderProgram, VSDProgramID *pDProgramID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11CreateDShaderFromStringCommand, VSDX11Renderer *, pRenderer, this,
		VSDShader *, pDShaderProgram, pDShaderProgram, VSDProgramID *, pDProgramID, pDProgramID,
		{
		pRenderer->OnLoadDShaderFromString(pDShaderProgram, pDProgramID);
	})
}
void VSDX11Renderer::CreateHShaderFromString(VSHShader* pHShaderProgram, VSHProgramID *pHProgramID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11CreateHShaderFromStringCommand, VSDX11Renderer *, pRenderer, this,
		VSHShader *, pHShaderProgram, pHShaderProgram, VSHProgramID *, pHProgramID, pHProgramID,
		{
		pRenderer->OnLoadHShaderFromString(pHShaderProgram, pHProgramID);
	})
}
void VSDX11Renderer::CreateCShaderFromString(VSCShader* pCShaderProgram, VSCProgramID *pCProgramID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11CreateCShaderFromStringCommand, VSDX11Renderer *, pRenderer, this,
		VSCShader *, pCShaderProgram, pCShaderProgram, VSCProgramID *, pCProgramID, pCProgramID,
		{
		pRenderer->OnLoadCShaderFromString(pCShaderProgram, pCProgramID);
		})
}

void VSDX11Renderer::CreateGShaderFromCache(VSGShader* pGShaderProgram, VSGProgramID *pGProgramID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11CreateGShaderFromCacheCommand, ID3D11Device*, m_pDevice, m_pDevice,
		VSGShader *, pGShaderProgram, pGShaderProgram, VSGProgramID *, pGProgramID, pGProgramID,
	{
		HRESULT hResult = m_pDevice->CreateGeometryShader(pGShaderProgram->GetCacheBuffer(), pGShaderProgram->GetCacheBufferSize(), NULL, &pGProgramID->m_pGeometryShader);
		VSMAC_ASSERT(!FAILED(hResult));

	})
}
void VSDX11Renderer::CreateCShaderFromCache(VSCShader* pCShaderProgram, VSCProgramID *pCProgramID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11CreateCShaderFromCacheCommand, ID3D11Device*, m_pDevice, m_pDevice,
		VSCShader *, pCShaderProgram, pCShaderProgram, VSCProgramID *, pCProgramID, pCProgramID,
	{
		HRESULT hResult = m_pDevice->CreateComputeShader(pCShaderProgram->GetCacheBuffer(), pCShaderProgram->GetCacheBufferSize(), NULL, &pCProgramID->m_pComputeShader);
		VSMAC_ASSERT(!FAILED(hResult));
	})
}
void VSDX11Renderer::CreateHShaderFromCache(VSHShader* pHShaderProgram, VSHProgramID *pHProgramID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11CreateHShaderFromCacheCommand, ID3D11Device*, m_pDevice, m_pDevice,
		VSHShader *, pHShaderProgram, pHShaderProgram, VSHProgramID *, pHProgramID, pHProgramID,
	{
		HRESULT hResult = m_pDevice->CreateHullShader(pHShaderProgram->GetCacheBuffer(), pHShaderProgram->GetCacheBufferSize(), NULL, &pHProgramID->m_pHullShader);
		VSMAC_ASSERT(!FAILED(hResult));
	})
}
void VSDX11Renderer::CreateDShaderFromCache(VSDShader* pDShaderProgram, VSDProgramID *pDProgramID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11CreateDShaderFromCacheCommand, ID3D11Device*, m_pDevice, m_pDevice,
		VSDShader *, pDShaderProgram, pDShaderProgram, VSDProgramID *, pDProgramID, pDProgramID,
	{
		HRESULT hResult = m_pDevice->CreateDomainShader(pDShaderProgram->GetCacheBuffer(), pDShaderProgram->GetCacheBufferSize(), NULL, &pDProgramID->m_pDomainShader);
		VSMAC_ASSERT(!FAILED(hResult));
	})
}
void VSDX11Renderer::CreatePShaderFromCache(VSPShader* pPShaderProgram, VSPProgramID *pPProgramID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11CreatePShaderFromCacheCommand, ID3D11Device*, m_pDevice, m_pDevice,
		VSPShader *, pPShaderProgram, pPShaderProgram, VSPProgramID *, pPProgramID, pPProgramID,
	{
		HRESULT hResult = m_pDevice->CreatePixelShader(pPShaderProgram->GetCacheBuffer(), pPShaderProgram->GetCacheBufferSize(), NULL, &pPProgramID->m_pPixelShader);
		VSMAC_ASSERT(!FAILED(hResult));
	})	
}
void VSDX11Renderer::CreateTexture1D(VS1DTexture *pTexture, D3D11_TEXTURE1D_DESC &desc, VSTextureID * pTextureID)
{
	struct VSCreateTexture1DPara
	{
		VS1DTexture *pTexture;
		D3D11_TEXTURE1D_DESC desc;
		VSTextureID * pTextureID;
	};
	VSCreateTexture1DPara CreateTexture1DPara;
	CreateTexture1DPara.desc = desc;
	CreateTexture1DPara.pTexture = pTexture;
	CreateTexture1DPara.pTextureID = pTextureID;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11CreateTexture1DCommand, ID3D11Device*, m_pDevice, m_pDevice,
		VSCreateTexture1DPara, CreateTexture1DPara, CreateTexture1DPara,
	{
		VS1DTexture *pTexture = CreateTexture1DPara.pTexture;
		D3D11_TEXTURE1D_DESC desc = CreateTexture1DPara.desc;
		VSTextureID * pTextureID = CreateTexture1DPara.pTextureID;

		HRESULT hResult = NULL;
		VSArray<D3D11_SUBRESOURCE_DATA> data;
		data.SetBufferNum(pTexture->GetMipLevel());
		bool HasData = false;
		for (unsigned int i = 0; i < pTexture->GetMipLevel(); i++)
		{
			if (!pTexture->GetBuffer(i))
			{
				continue;
			}
			HasData = true;
			data[i].pSysMem = pTexture->GetBuffer(i);
			data[i].SysMemPitch = 0;
			data[i].SysMemSlicePitch = 0;
		}

		if (HasData == true)
		{
			VSMAC_ASSERT(desc.Usage == D3D11_USAGE_IMMUTABLE);
			hResult = m_pDevice->CreateTexture1D(&desc, data.GetBuffer(), (ID3D11Texture1D **)&pTextureID->m_pTexture);
			VSMAC_ASSERT(!FAILED(hResult));
		}
		else
		{
			hResult = m_pDevice->CreateTexture1D(&desc, NULL, (ID3D11Texture1D **)&pTextureID->m_pTexture);
			VSMAC_ASSERT(!FAILED(hResult));
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC RSVdesc;
		RSVdesc.Format = desc.Format;
		RSVdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
		RSVdesc.Texture1D.MostDetailedMip = 0;
		RSVdesc.Texture1D.MipLevels = desc.MipLevels;

		HRESULT hr = m_pDevice->CreateShaderResourceView(pTextureID->m_pTexture, &RSVdesc, &pTextureID->m_pShaderResourceView);
		VSMAC_ASSERT(!FAILED(hResult));
	})
}
void VSDX11Renderer::CreateTexture2DArray(VS2DTextureArray *pTexture, D3D11_TEXTURE2D_DESC &desc, VSTextureID * pTextureID)
{
	struct VSCreateTextureCubePara
	{
		VS2DTextureArray *pTexture;
		D3D11_TEXTURE2D_DESC desc;
		VSTextureID * pTextureID;
	};
	VSCreateTextureCubePara CreateTextureCubePara;
	CreateTextureCubePara.desc = desc;
	CreateTextureCubePara.pTexture = pTexture;
	CreateTextureCubePara.pTextureID = pTextureID;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11CreateTextureCubeCommand, ID3D11Device*, m_pDevice, m_pDevice,
		VSCreateTextureCubePara, CreateTextureCubePara, CreateTextureCubePara,
	{
		VS2DTextureArray *pTexture = CreateTextureCubePara.pTexture;
		D3D11_TEXTURE2D_DESC desc = CreateTextureCubePara.desc;
		VSTextureID * pTextureID = CreateTextureCubePara.pTextureID;
		HRESULT hResult = NULL;
		VSArray<D3D11_SUBRESOURCE_DATA> data;
		data.SetBufferNum(pTexture->GetMipLevel() * pTexture->GetArraySize());
		bool HasData = false;
		for (unsigned int j = 0; j < pTexture->GetArraySize() ; j++)
		{		
			for (unsigned int i = 0; i < pTexture->GetMipLevel(); i++)
			{
				if (!pTexture->GetBuffer(i,j))
				{
					continue;
				}
				HasData = true;
				data[i].pSysMem = pTexture->GetBuffer(i,j);
				data[i].SysMemPitch = pTexture->GetMemPitch(i);
				data[i].SysMemSlicePitch = 0;
			}
		}
		if (HasData == true)
		{
			VSMAC_ASSERT(desc.Usage == D3D11_USAGE_IMMUTABLE);
			hResult = m_pDevice->CreateTexture2D(&desc, data.GetBuffer(), (ID3D11Texture2D **)&pTextureID->m_pTexture);
			VSMAC_ASSERT(!FAILED(hResult));
		}
		else
		{
			hResult = m_pDevice->CreateTexture2D(&desc, NULL, (ID3D11Texture2D **)&pTextureID->m_pTexture);
			VSMAC_ASSERT(!FAILED(hResult));
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC RSVdesc;
		RSVdesc.Format = desc.Format;
		if (pTexture->GetTexType() == VSTexture::TT_CUBE)
		{
			RSVdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
			RSVdesc.TextureCube.MostDetailedMip = 0;
			RSVdesc.TextureCube.MipLevels = desc.MipLevels;
		}
		else
		{
			RSVdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
			RSVdesc.Texture2DArray.MostDetailedMip = 0;
			RSVdesc.Texture2DArray.MipLevels = desc.MipLevels;
			RSVdesc.Texture2DArray.FirstArraySlice = 0;
			RSVdesc.Texture2DArray.ArraySize = desc.ArraySize;
		}

		HRESULT hr = m_pDevice->CreateShaderResourceView(pTextureID->m_pTexture, &RSVdesc, &pTextureID->m_pShaderResourceView);
		VSMAC_ASSERT(!FAILED(hResult));
	})
}
void VSDX11Renderer::CreateTexture3D(VS3DTexture *pTexture, D3D11_TEXTURE3D_DESC &desc, VSTextureID * pTextureID)
{
	struct VSCreateTexture3DPara
	{
		VS3DTexture *pTexture;
		D3D11_TEXTURE3D_DESC desc;
		VSTextureID * pTextureID;
	};
	VSCreateTexture3DPara CreateTexture3DPara;
	CreateTexture3DPara.desc = desc;
	CreateTexture3DPara.pTexture = pTexture;
	CreateTexture3DPara.pTextureID = pTextureID;

	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11CreateTexture3DCommand, ID3D11Device*, m_pDevice, m_pDevice,
		VSCreateTexture3DPara, CreateTexture3DPara, CreateTexture3DPara,
		{
		VS3DTexture *pTexture = CreateTexture3DPara.pTexture;
		D3D11_TEXTURE3D_DESC desc = CreateTexture3DPara.desc;
		VSTextureID * pTextureID = CreateTexture3DPara.pTextureID;
		HRESULT hResult = NULL;
		VSArray<D3D11_SUBRESOURCE_DATA> data;
		data.SetBufferNum(pTexture->GetMipLevel());
		bool HasData = false;
		for (unsigned int i = 0; i < pTexture->GetMipLevel(); i++)
		{
			if (!pTexture->GetBuffer(i))
			{
				continue;
			}
			HasData = true;
			data[i].pSysMem = pTexture->GetBuffer(i);
			data[i].SysMemPitch = pTexture->GetMemPitch(i);
			data[i].SysMemSlicePitch = pTexture->GetMemSlicePitch(i);
		}

		if (HasData == true)
		{
			VSMAC_ASSERT(desc.Usage == D3D11_USAGE_IMMUTABLE);
			hResult = m_pDevice->CreateTexture3D(&desc, data.GetBuffer(), (ID3D11Texture3D **)&pTextureID->m_pTexture);
			VSMAC_ASSERT(!FAILED(hResult));
		}
		else
		{
			hResult = m_pDevice->CreateTexture3D(&desc, NULL, (ID3D11Texture3D **)&pTextureID->m_pTexture);
			VSMAC_ASSERT(!FAILED(hResult));
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC RSVdesc;
		RSVdesc.Format = desc.Format;
		RSVdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
		RSVdesc.Texture3D.MostDetailedMip = 0;
		RSVdesc.Texture3D.MipLevels = desc.MipLevels;

		HRESULT hr = m_pDevice->CreateShaderResourceView(pTextureID->m_pTexture, &RSVdesc, &pTextureID->m_pShaderResourceView);
		VSMAC_ASSERT(!FAILED(hResult));
	})
}
void VSDX11Renderer::CreateTexture2D(VS2DTexture *pTexture, D3D11_TEXTURE2D_DESC &desc, VSTextureID * pTextureID)
{
	struct VSCreateTexture2DPara
	{
		VS2DTexture *pTexture;
		D3D11_TEXTURE2D_DESC desc;
		VSTextureID * pTextureID;
	};
	VSCreateTexture2DPara CreateTexture2DPara;
	CreateTexture2DPara.desc = desc;
	CreateTexture2DPara.pTexture = pTexture;
	CreateTexture2DPara.pTextureID = pTextureID;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11CreateTexture2DCommand, ID3D11Device*, m_pDevice, m_pDevice,
		VSCreateTexture2DPara, CreateTexture2DPara, CreateTexture2DPara,
		{
			VS2DTexture *pTexture = CreateTexture2DPara.pTexture;
			D3D11_TEXTURE2D_DESC desc = CreateTexture2DPara.desc;
			VSTextureID * pTextureID = CreateTexture2DPara.pTextureID;
			HRESULT hResult = NULL;
			VSArray<D3D11_SUBRESOURCE_DATA> data;
			data.SetBufferNum(pTexture->GetMipLevel());
			bool HasData = false;
			for (unsigned int i = 0; i < pTexture->GetMipLevel(); i++)
			{
				if (!pTexture->GetBuffer(i))
				{
					continue;
				}
				HasData = true;
				data[i].pSysMem = pTexture->GetBuffer(i);
				data[i].SysMemPitch = pTexture->GetMemPitch(i);
				data[i].SysMemSlicePitch = 0;
			}

			if (HasData == true)
			{
				VSMAC_ASSERT(desc.Usage == D3D11_USAGE_IMMUTABLE);
				hResult = m_pDevice->CreateTexture2D(&desc, data.GetBuffer(), (ID3D11Texture2D **)&pTextureID->m_pTexture);
				VSMAC_ASSERT(!FAILED(hResult));
			}
			else
			{
				hResult = m_pDevice->CreateTexture2D(&desc, NULL, (ID3D11Texture2D **)&pTextureID->m_pTexture);
				VSMAC_ASSERT(!FAILED(hResult));
			}

			D3D11_SHADER_RESOURCE_VIEW_DESC RSVdesc;
			RSVdesc.Format = desc.Format;
			RSVdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			RSVdesc.Texture2D.MostDetailedMip = 0;
			RSVdesc.Texture2D.MipLevels = desc.MipLevels;

			HRESULT hr = m_pDevice->CreateShaderResourceView(pTextureID->m_pTexture, &RSVdesc, &pTextureID->m_pShaderResourceView);
			VSMAC_ASSERT(!FAILED(hResult));
		})
}
void VSDX11Renderer::OnLoadDepthStencilEx(unsigned int Width, unsigned int Height, unsigned int Format, unsigned int MSType,
	unsigned int LockFlag, unsigned int uiTextureType, unsigned int uiDepthStencilLevel, unsigned int uiFirst,
	unsigned int uiArraySize, VSTextureID * pTextureID, VSDepthStencilID *pDepthStencilID)
{
	struct VSOnLoadDepthStencilExPara
	{
		unsigned int Width;
		unsigned int Height;
		unsigned int Format;
		unsigned int MSType;
		unsigned int LockFlag;
		unsigned int uiTextureType;
		unsigned int uiDepthStencilLevel;
		unsigned int uiFirst;
		unsigned int uiArraySize;
		VSTextureID * pTextureID;
		VSDepthStencilID *pDepthStencilID;
	};
	VSOnLoadDepthStencilExPara OnLoadDepthStencilExPara;
	OnLoadDepthStencilExPara.Width = Width;
	OnLoadDepthStencilExPara.Height = Height;
	OnLoadDepthStencilExPara.Format = Format;
	OnLoadDepthStencilExPara.MSType = MSType;
	OnLoadDepthStencilExPara.LockFlag = LockFlag;
	OnLoadDepthStencilExPara.uiTextureType = uiTextureType;
	OnLoadDepthStencilExPara.uiDepthStencilLevel = uiDepthStencilLevel;
	OnLoadDepthStencilExPara.uiFirst = uiFirst;
	OnLoadDepthStencilExPara.uiArraySize = uiArraySize;
	OnLoadDepthStencilExPara.pTextureID = pTextureID;
	OnLoadDepthStencilExPara.pDepthStencilID = pDepthStencilID;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11OnLoadDepthStencilExCommand, ID3D11Device*, m_pDevice, m_pDevice,
		VSOnLoadDepthStencilExPara, OnLoadDepthStencilExPara, OnLoadDepthStencilExPara,
		{
			unsigned int Width = OnLoadDepthStencilExPara.Width;
			unsigned int Height = OnLoadDepthStencilExPara.Height;
			DXGI_FORMAT DXGIFormat = (DXGI_FORMAT)ms_dwTextureFormatType[OnLoadDepthStencilExPara.Format];
			unsigned int Count = ms_dwMultiSampleTypes[OnLoadDepthStencilExPara.MSType];
			unsigned int LockFlag = OnLoadDepthStencilExPara.LockFlag;
			unsigned int uiTextureType = OnLoadDepthStencilExPara.uiTextureType;
			unsigned int uiDepthStencilLevel = OnLoadDepthStencilExPara.uiDepthStencilLevel;
			unsigned int uiFirst = OnLoadDepthStencilExPara.uiFirst;
			unsigned int uiArraySize = OnLoadDepthStencilExPara.uiArraySize;
			VSTextureID * pTextureID = OnLoadDepthStencilExPara.pTextureID;
			VSDepthStencilID *pDepthStencilID = OnLoadDepthStencilExPara.pDepthStencilID;
			HRESULT hResult = NULL;
			unsigned int Quality = ms_dwMultisampleQualityLevel[OnLoadDepthStencilExPara.Format][OnLoadDepthStencilExPara.MSType] -1;
			bool b1 = false;
			bool b2 = false;
			ID3D11Resource * pTempTexture = NULL;
			if (pTextureID)
			{
				pTempTexture = pTextureID->m_pTexture;
				if (OnLoadDepthStencilExPara.MSType == VSRenderer::MS_NONE)
				{
					b1 = true;
				}
				else
				{
					b2 = true;
				}
			}
			else
			{
				b2 = true;
			}

			if (LockFlag == VSInheritBind::LF_READONLY)
			{
				D3D11_TEXTURE2D_DESC desc;
				desc.Width = Width;
				desc.Height = Height;
				desc.MipLevels = 1;
				desc.ArraySize = uiArraySize;
				desc.Format = DXGIFormat;
				desc.SampleDesc.Count = 1;
				desc.SampleDesc.Quality = 0;
				desc.BindFlags = 0;
				desc.MiscFlags = 0;
				desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
				desc.Usage = D3D11_USAGE_STAGING;
				hResult = m_pDevice->CreateTexture2D(&desc, NULL, (ID3D11Texture2D **)&pDepthStencilID->m_pStagTexture);
				VSMAC_ASSERT(!FAILED(hResult));
			}

			pDepthStencilID->m_pOldTexture = pTempTexture;
			if (b1)
			{
				D3D11_DEPTH_STENCIL_VIEW_DESC desc;
				desc.Format = DXGIFormat;
				desc.Flags = 0;
				if (uiTextureType == VSTexture::TT_2D)
				{
					desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
					desc.Texture2D.MipSlice = uiDepthStencilLevel;
				}
				else if (uiTextureType == VSTexture::TT_CUBE || uiTextureType == VSTexture::TT_2DARRAY)
				{
					desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
					desc.Texture2DArray.MipSlice = uiDepthStencilLevel;
					desc.Texture2DArray.ArraySize = uiArraySize;
					desc.Texture2DArray.FirstArraySlice = uiFirst;
				}

				hResult = m_pDevice->CreateDepthStencilView(pTempTexture,&desc, &pDepthStencilID->m_pDepthStencilBuffer);
				VSMAC_ASSERT(!FAILED(hResult));
			}
			if (b2)
			{
				D3D11_TEXTURE2D_DESC desc;
				desc.Width = Width;
				desc.Height = Height;
				desc.MipLevels = 1;
				desc.ArraySize = uiArraySize;
				desc.Format = DXGIFormat;
				desc.SampleDesc.Count = Count;
				desc.SampleDesc.Quality = Quality;
				desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
				desc.MiscFlags = 0;
				desc.CPUAccessFlags = 0;
				desc.Usage = D3D11_USAGE_DEFAULT;



				hResult = m_pDevice->CreateTexture2D(&desc, NULL, (ID3D11Texture2D **)&pDepthStencilID->m_pTextureSurface);
				VSMAC_ASSERT(!FAILED(hResult));

				D3D11_DEPTH_STENCIL_VIEW_DESC DSdesc;
				DSdesc.Format = DXGIFormat;
				DSdesc.Flags = 0;
				if (Count > 1)
				{
					if (uiArraySize > 1)
					{
						DSdesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY;
						DSdesc.Texture2DMSArray.FirstArraySlice = 0;
						DSdesc.Texture2DMSArray.ArraySize = uiArraySize;
					}
					else
					{
						DSdesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
						DSdesc.Texture2DMS.UnusedField_NothingToDefine = 0;
					}
					
				}
				else
				{
					if (uiArraySize > 1)
					{
						DSdesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
						DSdesc.Texture2DArray.MipSlice = 0;
						DSdesc.Texture2DArray.FirstArraySlice = 0;
						DSdesc.Texture2DArray.ArraySize = uiArraySize;
					}
					else
					{
						DSdesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
						DSdesc.Texture2D.MipSlice = 0;
					}
				}
				
				

				hResult = m_pDevice->CreateDepthStencilView(pDepthStencilID->m_pTextureSurface, &DSdesc, &pDepthStencilID->m_pDepthStencilBuffer);
				VSMAC_ASSERT(!FAILED(hResult));
			}
		})
}
void VSDX11Renderer::OnLoadUnOrderAccessEx(unsigned int uiNum, unsigned int uiDataType, unsigned int uiByteWidth,
	unsigned int uiStructureByteStride, unsigned int LockFlag, VSSBufferID * pSBufferID, VSUnorderAccessID *pUnorderAccessID)
{
	struct VSOnLoadUnOrderAccessExPara
	{
		unsigned int uiNum;
		unsigned int uiDataType;
		unsigned int uiByteWidth;
		unsigned int uiStructureByteStride; 
		unsigned int LockFlag;
		VSSBufferID * pSBufferID; 
		VSUnorderAccessID *pUnorderAccessID;
	};
	VSOnLoadUnOrderAccessExPara OnLoadUnOrderAccessExPara;
	OnLoadUnOrderAccessExPara.uiNum = uiNum;
	OnLoadUnOrderAccessExPara.uiDataType = uiDataType;
	OnLoadUnOrderAccessExPara.uiByteWidth = uiByteWidth;
	OnLoadUnOrderAccessExPara.uiStructureByteStride = uiStructureByteStride;
	OnLoadUnOrderAccessExPara.LockFlag = LockFlag;
	OnLoadUnOrderAccessExPara.pSBufferID = pSBufferID;
	OnLoadUnOrderAccessExPara.pUnorderAccessID = pUnorderAccessID;

	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11OnLoadUnOrderAccessExCommand, ID3D11Device*, m_pDevice, m_pDevice,
	VSOnLoadUnOrderAccessExPara, OnLoadUnOrderAccessExPara, OnLoadUnOrderAccessExPara,
	{
		HRESULT hResult = NULL;
		D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc;
		unsigned int MiscFlags = 0;
		if (OnLoadUnOrderAccessExPara.uiDataType == VSDataBuffer::DT_UBYTE)
		{
			MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
			UAVDesc.Format = DXGI_FORMAT_R32_TYPELESS;
			UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
			UAVDesc.Buffer.FirstElement = 0;
			UAVDesc.Buffer.NumElements = OnLoadUnOrderAccessExPara.uiNum;
			UAVDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
		}
		else if (OnLoadUnOrderAccessExPara.uiDataType == VSDataBuffer::DT_STRUCT)
		{
			MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

			UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
			UAVDesc.Format = DXGI_FORMAT_UNKNOWN;
			UAVDesc.Buffer.FirstElement = 0;
			UAVDesc.Buffer.NumElements = OnLoadUnOrderAccessExPara.uiNum;
			UAVDesc.Buffer.Flags = 0;
		}
		else
		{
			UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
			UAVDesc.Format = (DXGI_FORMAT)ms_dwVertexFormatDataType[OnLoadUnOrderAccessExPara.uiDataType];
			UAVDesc.Buffer.FirstElement = 0;
			UAVDesc.Buffer.NumElements = OnLoadUnOrderAccessExPara.uiNum;
			UAVDesc.Buffer.Flags = 0;
		}

		if (OnLoadUnOrderAccessExPara.pSBufferID)
		{
			OnLoadUnOrderAccessExPara.pUnorderAccessID->m_pOldBuffer =
				OnLoadUnOrderAccessExPara.pSBufferID->m_pSBuffer;
			hResult = m_pDevice->CreateUnorderedAccessView(OnLoadUnOrderAccessExPara.pSBufferID->m_pSBuffer,
				&UAVDesc, &OnLoadUnOrderAccessExPara.pUnorderAccessID->m_pUnorderedAccess);
			VSMAC_ASSERT(!FAILED(hResult));
		}
		else
		{
			D3D11_BUFFER_DESC desc;
			desc.ByteWidth = OnLoadUnOrderAccessExPara.uiByteWidth;
			desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
			desc.MiscFlags = MiscFlags;
			desc.StructureByteStride = OnLoadUnOrderAccessExPara.uiStructureByteStride;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.CPUAccessFlags = 0;

			hResult = m_pDevice->CreateBuffer(&desc, nullptr,
				(ID3D11Buffer **)&OnLoadUnOrderAccessExPara.pUnorderAccessID->m_pBuffer);
			VSMAC_ASSERT(!FAILED(hResult));
			hResult = m_pDevice->CreateUnorderedAccessView(OnLoadUnOrderAccessExPara.pUnorderAccessID->m_pBuffer,
				&UAVDesc, &OnLoadUnOrderAccessExPara.pUnorderAccessID->m_pUnorderedAccess);
			VSMAC_ASSERT(!FAILED(hResult));
		}
		if (OnLoadUnOrderAccessExPara.LockFlag == VSInheritBind::LF_READONLY)
		{
			D3D11_BUFFER_DESC desc;
			desc.ByteWidth = OnLoadUnOrderAccessExPara.uiByteWidth;
			desc.BindFlags = 0;
			desc.MiscFlags = 0;
			desc.StructureByteStride = OnLoadUnOrderAccessExPara.uiStructureByteStride;
			desc.Usage = D3D11_USAGE_STAGING;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			hResult = m_pDevice->CreateBuffer(&desc, nullptr,
				(ID3D11Buffer **)&OnLoadUnOrderAccessExPara.pUnorderAccessID->m_pStagBuffer);
			VSMAC_ASSERT(!FAILED(hResult));
		}
	})
}
void VSDX11Renderer::OnLoadTextureUnorderAccessEx(unsigned int Width, unsigned int Height, unsigned int Format,
	unsigned int LockFlag, unsigned int uiTextureType, unsigned int uiUnorderAccessLevel, unsigned int uiFirst,
	unsigned int uiArraySize, VSTextureID * pTextureID, VSUnorderAccessID *pUnorderAccessID)
{
	struct VSOnLoadTextureUnorderAccessExPara
	{
		unsigned int Width;
		unsigned int Height;
		unsigned int Format;
		unsigned int LockFlag;
		unsigned int uiTextureType;
		unsigned int uiUnorderAccessLevel;
		unsigned int uiFirst;
		unsigned int uiArraySize;
		VSTextureID * pTextureID;
		VSUnorderAccessID *pUnorderAccessID;
	};
	VSOnLoadTextureUnorderAccessExPara OnLoadTextureUnorderAccessExPara;
	OnLoadTextureUnorderAccessExPara.Width = Width;
	OnLoadTextureUnorderAccessExPara.Height = Height;
	OnLoadTextureUnorderAccessExPara.Format = Format;
	OnLoadTextureUnorderAccessExPara.LockFlag = LockFlag;
	OnLoadTextureUnorderAccessExPara.uiTextureType = uiTextureType;
	OnLoadTextureUnorderAccessExPara.uiUnorderAccessLevel = uiUnorderAccessLevel;
	OnLoadTextureUnorderAccessExPara.uiFirst = uiFirst;
	OnLoadTextureUnorderAccessExPara.uiArraySize = uiArraySize;
	OnLoadTextureUnorderAccessExPara.pTextureID = pTextureID;
	OnLoadTextureUnorderAccessExPara.pUnorderAccessID = pUnorderAccessID;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11OnLoadTextureUnorderAccessExCommand, ID3D11Device*, m_pDevice, m_pDevice,
		VSOnLoadTextureUnorderAccessExPara, OnLoadTextureUnorderAccessExPara, OnLoadTextureUnorderAccessExPara,
		{
		unsigned int Width = OnLoadTextureUnorderAccessExPara.Width;
		unsigned int Height = OnLoadTextureUnorderAccessExPara.Height;
		DXGI_FORMAT DXGIFormat = (DXGI_FORMAT)ms_dwTextureFormatType[OnLoadTextureUnorderAccessExPara.Format];
		unsigned int LockFlag = OnLoadTextureUnorderAccessExPara.LockFlag;
		unsigned int uiTextureType = OnLoadTextureUnorderAccessExPara.uiTextureType;
		unsigned int uiUnorderAccessLevel = OnLoadTextureUnorderAccessExPara.uiUnorderAccessLevel;
		unsigned int uiFirst = OnLoadTextureUnorderAccessExPara.uiFirst;
		unsigned int uiArraySize = OnLoadTextureUnorderAccessExPara.uiArraySize;
		VSTextureID * pTextureID = OnLoadTextureUnorderAccessExPara.pTextureID;
		VSUnorderAccessID *pUnorderAccessID = OnLoadTextureUnorderAccessExPara.pUnorderAccessID;
		HRESULT hResult = NULL;

		bool b1 = false;
		bool b2 = false;
		ID3D11Resource * pTempTexture = NULL;
		if (pTextureID)
		{
			pTempTexture = pTextureID->m_pTexture;
			b1 = true;
		}
		else
		{
			b2 = true;
		}
		if (LockFlag == VSInheritBind::LF_READONLY)
		{
			D3D11_TEXTURE2D_DESC desc;
			desc.Width = Width;
			desc.Height = Height;
			desc.MipLevels = 1;
			desc.ArraySize = uiArraySize;
			desc.Format = DXGIFormat;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.BindFlags = 0;
			desc.MiscFlags = 0;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			desc.Usage = D3D11_USAGE_STAGING;
			hResult = m_pDevice->CreateTexture2D(&desc, NULL, (ID3D11Texture2D **)&pUnorderAccessID->m_pStagBuffer);
			VSMAC_ASSERT(!FAILED(hResult));
		}
		pUnorderAccessID->m_pOldBuffer = pTempTexture;
		if (b1)
		{
			D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
			desc.Format = DXGIFormat;
			if (uiTextureType == VSTexture::TT_2D)
			{
				desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
				desc.Texture2D.MipSlice = uiUnorderAccessLevel;
			}
			else if (uiTextureType == VSTexture::TT_CUBE || uiTextureType == VSTexture::TT_2DARRAY)
			{
				desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
				desc.Texture2DArray.MipSlice = uiUnorderAccessLevel;
				desc.Texture2DArray.ArraySize = uiArraySize;
				desc.Texture2DArray.FirstArraySlice = uiFirst;
			}
			else if (uiTextureType == VSTexture::TT_3D)
			{
				desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE3D;
				desc.Texture3D.MipSlice = uiUnorderAccessLevel;
				desc.Texture3D.WSize = uiArraySize;
				desc.Texture3D.FirstWSlice = uiFirst;
			}
			hResult = m_pDevice->CreateUnorderedAccessView(pTempTexture, &desc, &pUnorderAccessID->m_pUnorderedAccess);
			VSMAC_ASSERT(!FAILED(hResult));
		}
		if (b2)
		{
			D3D11_TEXTURE2D_DESC desc;
			desc.Width = Width;
			desc.Height = Height;
			desc.MipLevels = 1;
			desc.ArraySize = uiArraySize;
			desc.Format = DXGIFormat;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
			desc.MiscFlags = 0;
			desc.CPUAccessFlags = 0;
			desc.Usage = D3D11_USAGE_DEFAULT;



			hResult = m_pDevice->CreateTexture2D(&desc, NULL, (ID3D11Texture2D **)&pUnorderAccessID->m_pBuffer);
			VSMAC_ASSERT(!FAILED(hResult));

			D3D11_UNORDERED_ACCESS_VIEW_DESC UAVdesc;
			UAVdesc.Format = DXGIFormat;

			if (uiArraySize > 1)
			{
				UAVdesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
				UAVdesc.Texture2DArray.MipSlice = 0;
				UAVdesc.Texture2DArray.ArraySize = uiArraySize;
				UAVdesc.Texture2DArray.FirstArraySlice = 0;
			}
			else
			{
				UAVdesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
				UAVdesc.Texture2D.MipSlice = 0;
			}

			hResult = m_pDevice->CreateUnorderedAccessView(pUnorderAccessID->m_pBuffer, &UAVdesc, &pUnorderAccessID->m_pUnorderedAccess);
			VSMAC_ASSERT(!FAILED(hResult));
		}
	})
}
void VSDX11Renderer::OnLoadRenderTargetEx(unsigned int Width, unsigned int Height, unsigned int Format, unsigned int MSType,
	unsigned int LockFlag, unsigned int uiTextureType, unsigned int uiRenderTargetLevel, unsigned int uiFirst,
	unsigned int uiArraySize, VSTextureID * pTextureID, VSRenderTargetID *pRenderTargetID)
{
	struct VSOnLoadRenderTargetExPara
	{
		unsigned int Width;
		unsigned int Height;
		unsigned int Format;
		unsigned int MSType;
		unsigned int LockFlag;
		unsigned int uiTextureType;
		unsigned int uiRenderTargetLevel;
		unsigned int uiFirst;
		unsigned int uiArraySize;
		VSTextureID * pTextureID;
		VSRenderTargetID *pRenderTargetID;
	};
	VSOnLoadRenderTargetExPara OnLoadRenderTargetExPara;
	OnLoadRenderTargetExPara.Width = Width;
	OnLoadRenderTargetExPara.Height = Height;
	OnLoadRenderTargetExPara.Format = Format;
	OnLoadRenderTargetExPara.MSType = MSType;
	OnLoadRenderTargetExPara.LockFlag = LockFlag;
	OnLoadRenderTargetExPara.uiTextureType = uiTextureType;
	OnLoadRenderTargetExPara.uiRenderTargetLevel = uiRenderTargetLevel;
	OnLoadRenderTargetExPara.uiFirst = uiFirst;
	OnLoadRenderTargetExPara.uiArraySize = uiArraySize;
	OnLoadRenderTargetExPara.pTextureID = pTextureID;
	OnLoadRenderTargetExPara.pRenderTargetID = pRenderTargetID;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11OnLoadRenderTargetExCommand, ID3D11Device*, m_pDevice, m_pDevice,
		VSOnLoadRenderTargetExPara, OnLoadRenderTargetExPara, OnLoadRenderTargetExPara,
		{
		unsigned int Width = OnLoadRenderTargetExPara.Width;
		unsigned int Height = OnLoadRenderTargetExPara.Height;
		DXGI_FORMAT DXGIFormat = (DXGI_FORMAT)ms_dwTextureFormatType[OnLoadRenderTargetExPara.Format];
		unsigned int Count = ms_dwMultiSampleTypes[OnLoadRenderTargetExPara.MSType];
		unsigned int LockFlag = OnLoadRenderTargetExPara.LockFlag;
		unsigned int uiTextureType = OnLoadRenderTargetExPara.uiTextureType;
		unsigned int uiRenderTargetLevel = OnLoadRenderTargetExPara.uiRenderTargetLevel;
		unsigned int uiFirst = OnLoadRenderTargetExPara.uiFirst;
		unsigned int uiArraySize = OnLoadRenderTargetExPara.uiArraySize;
		VSTextureID * pTextureID = OnLoadRenderTargetExPara.pTextureID;
		VSRenderTargetID *pRenderTargetID = OnLoadRenderTargetExPara.pRenderTargetID;

		HRESULT hResult = NULL;
		unsigned int Quality = ms_dwMultisampleQualityLevel[OnLoadRenderTargetExPara.Format][OnLoadRenderTargetExPara.MSType] - 1;
		bool b1 = false;
		bool b2 = false;
		ID3D11Resource * pTempTexture = NULL;
		if (pTextureID)
		{
			pTempTexture = pTextureID->m_pTexture;
			if (OnLoadRenderTargetExPara.MSType == VSRenderer::MS_NONE)
			{
				b1 = true;
			}
			else
			{
				b2 = true;
			}
		}
		else
		{
			b2 = true;
		}

		if (LockFlag == VSInheritBind::LF_READONLY)
		{
			D3D11_TEXTURE2D_DESC desc;
			desc.Width = Width;
			desc.Height = Height;
			desc.MipLevels = 1;
			desc.ArraySize = uiArraySize;
			desc.Format = DXGIFormat;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.BindFlags = 0;
			desc.MiscFlags = 0;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			desc.Usage = D3D11_USAGE_STAGING;
			hResult = m_pDevice->CreateTexture2D(&desc, NULL, (ID3D11Texture2D **)&pRenderTargetID->m_pStagTexture);
			VSMAC_ASSERT(!FAILED(hResult));
		}

		pRenderTargetID->m_pOldTexture = pTempTexture;
		if (b1)
		{
			D3D11_RENDER_TARGET_VIEW_DESC desc;
			desc.Format = DXGIFormat;
			if (uiTextureType == VSTexture::TT_2D)
			{
				desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
				desc.Texture2D.MipSlice = uiRenderTargetLevel;
			}
			else if (uiTextureType == VSTexture::TT_CUBE || uiTextureType == VSTexture::TT_2DARRAY)
			{
				desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
				desc.Texture2DArray.MipSlice = uiRenderTargetLevel;
				desc.Texture2DArray.ArraySize = uiArraySize;
				desc.Texture2DArray.FirstArraySlice = uiFirst;
			}
			else if (uiTextureType == VSTexture::TT_3D)
			{
				desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE3D;
				desc.Texture3D.MipSlice = uiRenderTargetLevel;
				desc.Texture3D.WSize = uiArraySize;
				desc.Texture3D.FirstWSlice = uiFirst;
			}
			hResult = m_pDevice->CreateRenderTargetView(pTempTexture, &desc, &pRenderTargetID->m_pRenderTarget);
			VSMAC_ASSERT(!FAILED(hResult));
		}
		if (b2)
		{
			D3D11_TEXTURE2D_DESC desc;
			desc.Width = Width;
			desc.Height = Height;
			desc.MipLevels = 1;
			desc.ArraySize = uiArraySize;
			desc.Format = DXGIFormat;
			desc.SampleDesc.Count = Count;
			desc.SampleDesc.Quality = Quality;
			desc.BindFlags = D3D11_BIND_RENDER_TARGET;
			desc.MiscFlags = 0;
			desc.CPUAccessFlags = 0;
			desc.Usage = D3D11_USAGE_DEFAULT;



			hResult = m_pDevice->CreateTexture2D(&desc, NULL, (ID3D11Texture2D **)&pRenderTargetID->m_pTextureSurface);
			VSMAC_ASSERT(!FAILED(hResult));

			D3D11_RENDER_TARGET_VIEW_DESC RTVdesc;
			RTVdesc.Format = DXGIFormat;
			if (Count > 1)
			{
				if (uiArraySize > 1)
				{
					RTVdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY;
					RTVdesc.Texture2DMSArray.FirstArraySlice = 0;
					RTVdesc.Texture2DMSArray.ArraySize = uiArraySize;
				}
				else
				{
					RTVdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
					RTVdesc.Texture2DMS.UnusedField_NothingToDefine = 0;
				}
				
			}
			else
			{
				if (uiArraySize > 1)
				{
					RTVdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
					RTVdesc.Texture2DArray.MipSlice = 0;
					RTVdesc.Texture2DArray.ArraySize = uiArraySize;
					RTVdesc.Texture2DArray.FirstArraySlice = 0;
				}
				else
				{
					RTVdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
					RTVdesc.Texture2D.MipSlice = 0;
				}
				
			}
			

			hResult = m_pDevice->CreateRenderTargetView(pRenderTargetID->m_pTextureSurface, &RTVdesc, &pRenderTargetID->m_pRenderTarget);
			VSMAC_ASSERT(!FAILED(hResult));
		}
	})
}
void VSDX11Renderer::OnLoadSBufferEX(D3D11_BUFFER_DESC &Desc, D3D11_SHADER_RESOURCE_VIEW_DESC& SRVDesc,void * pBuffer, VSSBufferID * pSBufferID)
{
	struct VSCreateBufferEXPara
	{
		D3D11_BUFFER_DESC Desc;
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		void * pBuffer;
		VSSBufferID * pSBufferID;
	};

	VSCreateBufferEXPara CreateBufferEXPara;
	CreateBufferEXPara.Desc = Desc;
	CreateBufferEXPara.SRVDesc = SRVDesc;
	CreateBufferEXPara.pBuffer = pBuffer;
	CreateBufferEXPara.pSBufferID = pSBufferID;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11OnLoadSBufferEXCommand, ID3D11Device*, m_pDevice, m_pDevice,
		VSCreateBufferEXPara, CreateBufferEXPara, CreateBufferEXPara,
	{
		HRESULT hResult = NULL;
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = CreateBufferEXPara.pBuffer;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;
		if (CreateBufferEXPara.pBuffer)
		{
			hResult = m_pDevice->CreateBuffer(&CreateBufferEXPara.Desc, &InitData, &CreateBufferEXPara.pSBufferID->m_pSBuffer);
		}
		else
		{
			hResult = m_pDevice->CreateBuffer(&CreateBufferEXPara.Desc, NULL, &CreateBufferEXPara.pSBufferID->m_pSBuffer);
		}
		VSMAC_ASSERT(!FAILED(hResult));	
		HRESULT hr = m_pDevice->CreateShaderResourceView(CreateBufferEXPara.pSBufferID->m_pSBuffer, &CreateBufferEXPara.SRVDesc, &CreateBufferEXPara.pSBufferID->m_pShaderResourceView);
		VSMAC_ASSERT(!FAILED(hResult));
	})
}
void VSDX11Renderer::OnLoadVertexBufferEx(D3D11_BUFFER_DESC& desc,VSVertexBuffer * pVBuffer, VSVBufferID * pVBufferID)
{
	struct VSOnLoadVertexBufferExPara
	{
		D3D11_BUFFER_DESC desc;
		VSVertexBuffer * pVBuffer;
		VSVBufferID * pVBufferID;
	};
	VSOnLoadVertexBufferExPara OnLoadVertexBufferExPara;
	OnLoadVertexBufferExPara.desc = desc;
	OnLoadVertexBufferExPara.pVBuffer = pVBuffer;
	OnLoadVertexBufferExPara.pVBufferID = pVBufferID;


	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11OnLoadVertexBufferExCommand, ID3D11Device*, m_pDevice, m_pDevice,
		VSOnLoadVertexBufferExPara, OnLoadVertexBufferExPara, OnLoadVertexBufferExPara,
	{
		unsigned int uiOneVertexSize = OnLoadVertexBufferExPara.pVBuffer->GetOneVertexSize();
		unsigned int uiTotalSize = OnLoadVertexBufferExPara.desc.ByteWidth;


		VSVertexBuffer * pVBuffer = OnLoadVertexBufferExPara.pVBuffer;
		unsigned char * pTempBufferUsed = VS_NEW unsigned char[uiTotalSize];
		VSDataBuffer * pData;
		unsigned int iVertexSize = 0;
		for (unsigned int i = 0; i < pVBuffer->GetPositionLevel(); i++)
		{
			pData = pVBuffer->GetPositionData(i);
			if (pData)
			{
				for (unsigned int j = 0; j < pData->GetNum(); j++)
				{
					VSMemcpy(pTempBufferUsed + uiOneVertexSize * j + iVertexSize,
						(unsigned char *)pData->GetData() + j * pData->GetStride(), pData->GetStride());

				}
				iVertexSize += pData->GetStride();
			}

		}

		for (unsigned int i = 0; i < pVBuffer->GetTexCoordLevel(); i++)
		{
			pData = pVBuffer->GetTexCoordData(i);
			if (pData)
			{
				for (unsigned int j = 0; j < pData->GetNum(); j++)
				{
					VSMemcpy(pTempBufferUsed + uiOneVertexSize * j + iVertexSize,
						(unsigned char *)pData->GetData() + j * pData->GetStride(), pData->GetStride());

				}
				iVertexSize += pData->GetStride();
			}
		}

		for (unsigned int i = 0; i < pVBuffer->GetNormalLevel(); i++)
		{
			pData = pVBuffer->GetNormalData(i);
			if (pData)
			{
				for (unsigned int j = 0; j < pData->GetNum(); j++)
				{
					VSMemcpy(pTempBufferUsed + uiOneVertexSize * j + iVertexSize,
						(unsigned char *)pData->GetData() + j * pData->GetStride(), pData->GetStride());

				}
				iVertexSize += pData->GetStride();
			}
		}
		pData = pVBuffer->GetTangentData();
		if (pData)
		{
			for (unsigned int j = 0; j < pData->GetNum(); j++)
			{
				VSMemcpy(pTempBufferUsed + uiOneVertexSize * j + iVertexSize,
					(unsigned char *)pData->GetData() + j * pData->GetStride(), pData->GetStride());

			}
			iVertexSize += pData->GetStride();
		}


		pData = pVBuffer->GetBinormalData();
		if (pData)
		{
			for (unsigned int j = 0; j < pData->GetNum(); j++)
			{
				VSMemcpy(pTempBufferUsed + uiOneVertexSize * j + iVertexSize,
					(unsigned char *)pData->GetData() + j * pData->GetStride(), pData->GetStride());

			}
			iVertexSize += pData->GetStride();
		}
		pData = pVBuffer->GetPSizeData();
		if (pData)
		{
			for (unsigned int j = 0; j < pData->GetNum(); j++)
			{
				VSMemcpy(pTempBufferUsed + uiOneVertexSize * j + iVertexSize,
					(unsigned char *)pData->GetData() + j * pData->GetStride(), pData->GetStride());

			}
			iVertexSize += pData->GetStride();
		}

		for (unsigned int i = 0; i < pVBuffer->GetColorLevel(); i++)
		{

			pData = pVBuffer->GetColorData(i);
			if (pData)
			{
				for (unsigned int j = 0; j < pData->GetNum(); j++)
				{
					VSMemcpy(pTempBufferUsed + uiOneVertexSize * j + iVertexSize,
						(unsigned char *)pData->GetData() + j * pData->GetStride(), pData->GetStride());

				}
				iVertexSize += pData->GetStride();
			}
		}

		pData = pVBuffer->GetFogData();
		if (pData)
		{
			for (unsigned int j = 0; j < pData->GetNum(); j++)
			{
				VSMemcpy(pTempBufferUsed + uiOneVertexSize * j + iVertexSize,
					(unsigned char *)pData->GetData() + j * pData->GetStride(), pData->GetStride());

			}
			iVertexSize += pData->GetStride();
		}

		pData = pVBuffer->GetVertexIDData();
		if (pData)
		{
			for (unsigned int j = 0; j < pData->GetNum(); j++)
			{
				VSMemcpy(pTempBufferUsed + uiOneVertexSize * j + iVertexSize,
					(unsigned char *)pData->GetData() + j * pData->GetStride(), pData->GetStride());

			}
			iVertexSize += pData->GetStride();
		}

		pData = pVBuffer->GetBlendWeightData();
		if (pData)
		{
			for (unsigned int j = 0; j < pData->GetNum(); j++)
			{
				VSMemcpy(pTempBufferUsed + uiOneVertexSize * j + iVertexSize,
					(unsigned char *)pData->GetData() + j * pData->GetStride(), pData->GetStride());

			}
			iVertexSize += pData->GetStride();
		}

		for (unsigned int i = 0; i < pVBuffer->GetBlendIndicesLevel(); i++)
		{
			pData = pVBuffer->GetBlendIndicesData(i);
			if (pData)
			{
				for (unsigned int j = 0; j < pData->GetNum(); j++)
				{
					VSMemcpy(pTempBufferUsed + uiOneVertexSize * j + iVertexSize,
						(unsigned char *)pData->GetData() + j * pData->GetStride(), pData->GetStride());

				}
				iVertexSize += pData->GetStride();
			}
		}
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = pTempBufferUsed;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;
		HRESULT hResult = NULL;
		if (iVertexSize > 0)
		{
			hResult = m_pDevice->CreateBuffer(&OnLoadVertexBufferExPara.desc, &InitData, &OnLoadVertexBufferExPara.pVBufferID->m_pVertexBuffer);
		}
		else
		{
			hResult = m_pDevice->CreateBuffer(&OnLoadVertexBufferExPara.desc, NULL, &OnLoadVertexBufferExPara.pVBufferID->m_pVertexBuffer);
		}
		VSMAC_DELETEA(pTempBufferUsed);
		VSMAC_ASSERT(!FAILED(hResult));
	})
}
void VSDX11Renderer::OnLoadIndexBufferEx(D3D11_BUFFER_DESC& desc, void* pBuffer, VSIBufferID * pIBufferID)
{
	struct VSOnLoadIndexBufferExPara
	{
		D3D11_BUFFER_DESC desc;
		void* pBuffer;
		VSIBufferID * pIBufferID;
	};
	VSOnLoadIndexBufferExPara OnLoadIndexBufferExPara;
	OnLoadIndexBufferExPara.desc = desc;
	OnLoadIndexBufferExPara.pBuffer = pBuffer;
	OnLoadIndexBufferExPara.pIBufferID = pIBufferID;


	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11OnLoadIndexBufferExCommand, ID3D11Device*, m_pDevice, m_pDevice,
		VSOnLoadIndexBufferExPara, OnLoadIndexBufferExPara, OnLoadIndexBufferExPara,
	{
		HRESULT hResult = NULL;
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = OnLoadIndexBufferExPara.pBuffer;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;

		if (OnLoadIndexBufferExPara.pBuffer)
		{
			hResult = m_pDevice->CreateBuffer(&OnLoadIndexBufferExPara.desc, &InitData, &OnLoadIndexBufferExPara.pIBufferID->m_IndexBuffer);
		}
		else
		{
			hResult = m_pDevice->CreateBuffer(&OnLoadIndexBufferExPara.desc, NULL, &OnLoadIndexBufferExPara.pIBufferID->m_IndexBuffer);
		}
		VSMAC_ASSERT(!FAILED(hResult));
	})
}
bool VSDX11Renderer::DrawDynamicBufferMesh(VSDynamicBufferGeometry *pDBGeometry)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11DrawDynamicBufferMeshCommand,
		VSDynamicBufferGeometry *, pDBGeometry, pDBGeometry, ID3D11DeviceContext *, m_pDeviceContext, m_pDeviceContext,
	{
		ADD_COUNT_PROFILE(DrawGeometryNum, 1)
		m_pDeviceContext->IASetPrimitiveTopology((D3D_PRIMITIVE_TOPOLOGY)ms_dwPrimitiveType[pDBGeometry->GetMeshDataType()]);
		while (true)
		{
			HRESULT hResult = NULL;
			unsigned int uiReturn = pDBGeometry->UpdateGeometry();

			if (uiReturn == VSGeometry::UGRI_FAIL)
			{
				break;
			}
			if (!pDBGeometry->GetMeshData()->GetIndexBuffer())
			{
				m_pDeviceContext->Draw(pDBGeometry->GetMeshData()->GetVertexBuffer()->GetVertexNum(), 0);
			}
			else
			{
				m_pDeviceContext->DrawIndexed(pDBGeometry->GetActiveNum() * 3, 0, 0);
			}
			ADD_COUNT_PROFILE(DrawPrimitiveNum, pDBGeometry->GetActiveNum())
				VSMAC_ASSERT(!FAILED(hResult));

			if (uiReturn == VSGeometry::UGRI_END)
			{
				break;
			}
		}
	})
	return true;
}
bool VSDX11Renderer::DrawMesh(VSGeometry * pGeometry)
{
	struct VSDrawMeshPara
	{
		bool HasIndexBuffer;;
		D3D_PRIMITIVE_TOPOLOGY PrimitiveType;
		unsigned int uiVertexNum;
		unsigned int uiActiveNum;
		bool bInstance;
		unsigned int uiInstanceNum;
	};
	static unsigned int PrimitiveTopologyChange[] = 
	{ 
		D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST ,
		D3D11_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST ,
		D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST ,
		D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST 
	};
	VSDrawMeshPara DrawMeshPara;
	DrawMeshPara.HasIndexBuffer = pGeometry->GetMeshData()->GetIndexBuffer()!= NULL;
	DrawMeshPara.PrimitiveType = (D3D_PRIMITIVE_TOPOLOGY)ms_dwPrimitiveType[pGeometry->GetMeshDataType()];
	if (m_pDShader && m_pHShader)
	{
		DrawMeshPara.PrimitiveType = (D3D_PRIMITIVE_TOPOLOGY)PrimitiveTopologyChange[pGeometry->GetMeshDataType()];
	}
	DrawMeshPara.uiActiveNum = pGeometry->GetActiveNum();
	DrawMeshPara.uiVertexNum = pGeometry->GetMeshData()->GetVertexBuffer()->GetVertexNum();
	DrawMeshPara.bInstance = pGeometry->IsSameType(VSInstanceGeometry::ms_Type);
	DrawMeshPara.uiInstanceNum = DrawMeshPara.bInstance ? ((VSInstanceGeometry *)pGeometry)->GetInstanceNum() : 0;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11DrawMeshCommand,
		VSDrawMeshPara, DrawMeshPara, DrawMeshPara, ID3D11DeviceContext *, m_pDeviceContext, m_pDeviceContext,
		{
		ADD_COUNT_PROFILE(DrawGeometryNum, 1)
		m_pDeviceContext->IASetPrimitiveTopology(DrawMeshPara.PrimitiveType);
		if (!DrawMeshPara.HasIndexBuffer)
		{
			if (!DrawMeshPara.bInstance)
			{
				m_pDeviceContext->Draw(DrawMeshPara.uiVertexNum, 0);
			}
			else
			{
				m_pDeviceContext->DrawInstanced(DrawMeshPara.uiVertexNum, DrawMeshPara.uiInstanceNum,0, 0);
			}
			
		}
		else
		{
			if (!DrawMeshPara.bInstance)
			{
				m_pDeviceContext->DrawIndexed(DrawMeshPara.uiActiveNum * 3, 0, 0);
			}
			else
			{
				m_pDeviceContext->DrawIndexedInstanced(DrawMeshPara.uiActiveNum * 3,DrawMeshPara.uiInstanceNum, 0, 0, 0);
			}
		}
		ADD_COUNT_PROFILE(DrawPrimitiveNum, DrawMeshPara.uiActiveNum)
	})
	return true;
}
void VSDX11Renderer::CreateQuery(D3D11_QUERY Type, VSQueryID * pQueryID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11CreateQueryCommand, ID3D11Device*, m_pDevice, m_pDevice,
		D3D11_QUERY, Type, Type, VSQueryID *, pQueryID, pQueryID,
		{
			HRESULT hResult = NULL;
			D3D11_QUERY_DESC QueryDesc;
			QueryDesc.Query = Type;
			QueryDesc.MiscFlags = 0;
			hResult = m_pDevice->CreateQuery(&QueryDesc, &pQueryID->m_Query);
			VSMAC_ASSERT(!FAILED(hResult));
		})
}
void VSDX11Renderer::BeginQuery(VSQueryID* pQueryID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11BeginQueryCommand, ID3D11DeviceContext* , m_pDeviceContext, m_pDeviceContext,
		VSQueryID *, pQueryID, pQueryID,
		{
			m_pDeviceContext->Begin(pQueryID->m_Query);
		})
}
void VSDX11Renderer::EndQuery(VSQueryID* pQueryID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11BeginQueryCommand, ID3D11DeviceContext*, m_pDeviceContext, m_pDeviceContext,
		VSQueryID *, pQueryID, pQueryID,
		{
			m_pDeviceContext->End(pQueryID->m_Query);
		})
}
void VSDX11Renderer::CreateBlendState(const D3D11_BLEND_DESC &desc, VSBlendStateID * pBlendStateID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11CreateBlendStateCommand, ID3D11Device*, m_pDevice, m_pDevice,
		D3D11_BLEND_DESC, desc, desc, VSBlendStateID *, pBlendStateID, pBlendStateID,
	{
		HRESULT hResult = NULL;
		hResult = m_pDevice->CreateBlendState(&desc, &pBlendStateID->m_pBlendState);
		VSMAC_ASSERT(!FAILED(hResult));
	})
}
void VSDX11Renderer::CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC depthStencilDesc, VSDepthStencilStateID * pDepthStenciStateID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11CreateDepthStencilStateCommand, ID3D11Device*, m_pDevice, m_pDevice,
		D3D11_DEPTH_STENCIL_DESC, depthStencilDesc, depthStencilDesc, VSDepthStencilStateID *, pDepthStenciStateID, pDepthStenciStateID,
	{
		HRESULT hResult = NULL;
		hResult = m_pDevice->CreateDepthStencilState(&depthStencilDesc, &pDepthStenciStateID->m_pDepthStencilState);
		VSMAC_ASSERT(!FAILED(hResult));
	})
}
void VSDX11Renderer::CreateRasterizerState(const D3D11_RASTERIZER_DESC rasterDesc, VSRasterStateID * pRasterStateID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11CreateRasterizerStateCommand, ID3D11Device*, m_pDevice, m_pDevice,
		D3D11_RASTERIZER_DESC, rasterDesc, rasterDesc, VSRasterStateID *, pRasterStateID, pRasterStateID,
	{
		HRESULT hResult = NULL;
		hResult = m_pDevice->CreateRasterizerState(&rasterDesc, &pRasterStateID->m_pRasterState);
		VSMAC_ASSERT(!FAILED(hResult));
	})
}
void VSDX11Renderer::CreateSamplerState(const D3D11_SAMPLER_DESC& sampDesc, VSSamplerStateID * pSamplerStateID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(VSDx11CreateSamplerStateCommand, ID3D11Device*, m_pDevice, m_pDevice,
		D3D11_SAMPLER_DESC, sampDesc, sampDesc, VSSamplerStateID *, pSamplerStateID, pSamplerStateID,
	{
		HRESULT hResult = NULL;
		hResult = m_pDevice->CreateSamplerState(&sampDesc, &pSamplerStateID->m_pSamplerState);
		VSMAC_ASSERT(!FAILED(hResult));
	})
}
void VSDX11Renderer::IASetInputLayout(VSVBufferFormatID * pVBufferFormatID)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSDx11IASetInputLayoutCommand, ID3D11DeviceContext *, m_pDeviceContext, m_pDeviceContext,
		VSVBufferFormatID *, pVBufferFormatID, pVBufferFormatID,
	{
		if (pVBufferFormatID)
		{
			m_pDeviceContext->IASetInputLayout(pVBufferFormatID->m_pDeclaration);
		}
		else
		{
			m_pDeviceContext->IASetInputLayout(NULL);
		}
	})
}
#endif