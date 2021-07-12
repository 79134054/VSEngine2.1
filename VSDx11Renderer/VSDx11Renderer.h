#pragma once
#include "VSRenderer.h"
#include "VSResourceManager.h"
#include "VSDx11ShaderInclude.h"
#ifdef WINDOWS_PLATFORM
#ifdef VSDX11RENDERER_EXPORTS
#define  VSDX11RENDERER_API __declspec(dllexport) 
#else
#define  VSDX11RENDERER_API __declspec(dllimport)
#endif
#include <d3d11.h>
#include <dxgi.h>
#include <d3dx11.h>
#include <D3Dcompiler.h>
#endif
namespace VSEngine2
{
#ifdef WINDOWS_PLATFORM
	#define VSMAC_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
	class VSVProgramID;
	class VSPProgramID;
	class VSGProgramID;
	class VSDProgramID;
	class VSHProgramID;
	class VSCProgramID;
	class VSTextureID;
	class VSDepthStencilID;
	class VSRenderTargetID;
	class VSVBufferID;
	class VSIBufferID;
	class VSSBufferID;
	class VSUnorderAccessID;
	class VSSamplerStateID;
	class VSBlendStateID;
	class VSRasterStateID;
	class VSDepthStencilStateID;
	class VSVBufferFormatID;
	class VSQueryID;
	class VSDX11RENDERER_API VSDX11Renderer : public VSRenderer
	{
	public:
		VSDX11Renderer(HWND hMainWindow, unsigned int uiScreenWidth, unsigned int uiScreenHeight, bool bIsWindowed,
			unsigned int uiAnisotropy, unsigned int uiMultisample,
			ChildWindowInfo * pChildWindow, int uiNumChildWindow);
		virtual ~VSDX11Renderer();
		virtual int GetRendererType() const;
	protected:
		static VSDx11ShaderInclude * ms_pDx11IncludeShader;
		static IDXGIFactory* ms_pDXGI;
		ID3D11Device* m_pDevice;
		ID3D11DeviceContext* m_pDeviceContext;
		IDXGIAdapter* m_pAdapter;
		IDXGIOutput* m_pAdapterOutput;

		IDXGISwapChain** m_pChain;
		IDXGISwapChain* m_pMainChain;
		DXGI_SWAP_CHAIN_DESC m_SwapChainDesc;

		ID3D11Buffer* m_pFontVertexBuffer = NULL;
		ID3D11Buffer* m_pFontIndexBuffer = NULL;


		ID3D11RenderTargetView* m_pMainRenderTargetView;
		ID3D11RenderTargetView** m_pChainRenderTargetView;

		ID3D11Texture2D*           m_pMainDepthStencil;
		ID3D11DepthStencilView*    m_pMainDepthStencilView;

		ID3D11Texture2D**           m_pChainDepthStencil;
		ID3D11DepthStencilView**    m_pChainDepthStencilView;

		ID3D11Buffer*		m_pVShaderBuffer;
		ID3D11Buffer*		m_pPShaderBuffer;
		ID3D11Buffer*		m_pHShaderBuffer;
		ID3D11Buffer*		m_pDShaderBuffer;
		ID3D11Buffer*		m_pGShaderBuffer;
		ID3D11Buffer*		m_pCShaderBuffer;

		static DWORD ms_dwAlphaBlend[];
		static DWORD ms_dwAlphaBlendOP[];
		static DWORD ms_dwCullType[];



		static DWORD ms_dwStencilOperation[];

		static DWORD ms_dwCompare[];

		static DWORD ms_dwTextureFormatType[];
		static DWORD ms_dwSRGBTextureFormatType[];
		static DWORD ms_dwTextureFilterType[VSSamplerDesc::FM_MAX][VSSamplerDesc::FM_MAX][VSSamplerDesc::FM_MAX];
		static DWORD ms_dwTextureAddrType[];
		static DWORD ms_dwQueryType[];

		static DWORD ms_dwPrimitiveType[];
		static DWORD ms_dwDevTypes[];
		static DWORD ms_dwMultiSampleTypes[];

		static DWORD ms_dwLockFlag[];
		static DWORD ms_dwVertexFormatDataType[];
		static TCHAR* ms_dwVertexFormatClareType[];
		static TCHAR* ms_dwVertexFormatSemantics[];

		static unsigned int ms_uiVertexShaderProgramVersion;
		static unsigned int ms_uiPixelShaderProgramVersion;
		static TCHAR ms_cVertexShaderProgramVersion[];
		static TCHAR ms_cPixelShaderProgramVersion[];
		static TCHAR ms_cGeometryShaderProgramVersion[];
		static TCHAR ms_cDomainShaderProgramVersion[];
		static TCHAR ms_cHullShaderProgramVersion[];
		static TCHAR ms_cComputerShaderProgramVersion[];
		static VSString ms_cTessellationPartitioning[];
		static VSREAL ms_fHSMaxTessFactor;

		static UINT ms_dwMultisampleQualityLevel[VSRenderer::SFT_MAX][VSRenderer::MS_MAX];
		enum
		{
			MAX_SHADER_BUFFER = 4096 * 16,
		};
		unsigned char m_uiShaderBuffer[MAX_SHADER_BUFFER];

		
		//PRIORITY
		DECLARE_PRIORITY
		DECLARE_INITIAL_ONLY
		static bool InitialDefaultState();
		static bool TerminalDefaultState();

		static ID3DBlob* CreateVertexFormatShader(const VSArray<VSVertexFormat::VERTEXFORMAT_TYPE> & Layouts);
	public:
		virtual void ClearBuffers(unsigned int uiClearFlag);
		virtual void ClearBackBuffer();
		virtual void ClearZBuffer();
		virtual void ClearStencilBuffer();
		virtual void ClearBuffers();

		virtual void ClearBuffers(unsigned int uiClearFlag, int iXPos, int iYPos, int iWidth,
			int iHeight);
		virtual void ClearBackBuffer(int iXPos, int iYPos, int iWidth,
			int iHeight);
		virtual void ClearZBuffer(int iXPos, int iYPos, int iWidth,
			int iHeight);
		virtual void ClearStencilBuffer(int iXPos, int iYPos, int iWidth,
			int iHeight);
		virtual void ClearBuffers(int iXPos, int iYPos, int iWidth,
			int iHeight);

		virtual bool SetViewPort(VSViewPort *pViewPort = NULL);
		virtual bool UseWindow(int uiWindowID = -1);

		virtual bool CooperativeLevel();
		virtual bool BeginRendering();
		virtual bool EndRendering();

		virtual void DeviceLost();
		virtual void ResetDevice();
		virtual bool ChangeScreenSize(unsigned int uiWidth, unsigned int uiHeight, bool bWindow);
		virtual bool DrawMesh(VSGeometry * pGeometry, VSRenderState * pRenderState, VSVShader * pVShader, VSPShader * pPShader, 
			VSGShader * pGShader, VSHShader * pHShader, VSDShader * pDShader);
		virtual bool DrawScreen(SCREEN_QUAD_TYPE ScreenQuad[4]);
		virtual bool DrawScreen(SCREEN_QUAD_TYPE * pScreenBuffer, unsigned int uiVertexNum,
			VSUSHORT_INDEX * pIndexBuffer, unsigned int uiIndexNum);
		virtual bool DrawScreenFont(SCREEN_FONT_TYPE * pScreenFontBuffer, unsigned int uiVertexNum,
			VSUSHORT_INDEX * pIndexBuffer, unsigned int uiIndexNum);
		virtual unsigned int SetBlendState(VSBlendState * pBlendState, bool bForceSet = false);
		virtual unsigned int SetDepthStencilState(VSDepthStencilState * pDepthStencilState, bool bForceSet = false);
		virtual unsigned int SetRasterizerState(VSRasterizerState * pRasterizerState, bool bForceSet = false);
		virtual void SetClipPlane(VSArray<VSPlane3> &Plane, bool bEnable);
		virtual void SetScissorRect(VSArray<VSRect2> &Rect, bool bEnable);


		virtual unsigned int SetVBuffer(VSVertexBuffer * pVBuffer, unsigned int uiStream = 0);
		virtual unsigned int SetIBuffer(VSIndexBuffer* pIBuffer);

		virtual unsigned int BeginQuery(VSQuery * pQuery);
		virtual unsigned int EndQuery(VSQuery * pQuery);

		virtual unsigned int SetPTexture(VSTexture* pTexture, unsigned int i);
		virtual unsigned int SetVTexture(VSTexture* pTexture, unsigned int i);
		virtual unsigned int SetGTexture(VSTexture* pTexture, unsigned int i);
		virtual unsigned int SetDTexture(VSTexture* pTexture, unsigned int i);
		virtual unsigned int SetHTexture(VSTexture* pTexture, unsigned int i);
		virtual unsigned int SetCTexture(VSTexture* pTexture, unsigned int i);
		virtual unsigned int SetCBufferResource(VSBufferResource* pBufferResource, unsigned int i);
		virtual unsigned int SetVBufferResource(VSBufferResource* pBufferResource, unsigned int i);
		virtual unsigned int SetPBufferResource(VSBufferResource* pBufferResource, unsigned int i);
		virtual unsigned int SetGBufferResource(VSBufferResource* pBufferResource, unsigned int i);
		virtual unsigned int SetHBufferResource(VSBufferResource* pBufferResource, unsigned int i);
		virtual unsigned int SetDBufferResource(VSBufferResource* pBufferResource, unsigned int i);

		virtual void Dispath(unsigned int uiThreadGroupCountX, unsigned int uiThreadGroupCountY,
			unsigned int uiThreadGroupCountZ);

		virtual unsigned int SetVSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet = false);
		virtual unsigned int SetPSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet = false);
		virtual unsigned int SetGSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet = false);
		virtual unsigned int SetDSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet = false);
		virtual unsigned int SetHSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet = false);
		virtual unsigned int SetCSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet = false);

		virtual bool SetRenderTargets(VSRenderTarget *pRenderTarget[], unsigned int uiNum);
		virtual bool EndRenderTargets(VSRenderTarget *pRenderTarget[], unsigned int uiNum);
		virtual bool SetDepthStencilBuffer(VSDepthStencil * pDepthStencilBuffer);
		virtual bool EndDepthStencilBuffer(VSDepthStencil * pDepthStencilBuffer);
		virtual bool SetCSUnorderAccesses(VSUnorderAccess * pUnorderAccesses[], unsigned int uiNum);
		virtual bool EndCSUnorderAccesses(VSUnorderAccess * pUnorderAccesses[], unsigned int uiNum);

		virtual unsigned int GetTextureCompressLen(unsigned int uiLen);

		virtual const VSAdapterInfo* GetAdapterInfo(unsigned int & uiAdapterNum);

	protected:
		virtual void SetVShaderConstant(VSVShader * pShader);
		virtual void SetPShaderConstant(VSPShader * pShader);
		virtual void SetGShaderConstant(VSGShader * pShader);
		virtual void SetDShaderConstant(VSDShader * pShader);
		virtual void SetHShaderConstant(VSHShader * pShader);
		virtual void SetCShaderConstant(VSCShader * pShader);



		virtual bool SetVShaderProgram(VSVShader*	pVShaderProgram);
		virtual bool SetPShaderProgram(VSPShader*	pPShaderProgram);
		virtual bool SetGShaderProgram(VSGShader*	pGShaderProgram);
		virtual bool SetDShaderProgram(VSDShader*	pDShaderProgram);
		virtual bool SetHShaderProgram(VSHShader*	pHShaderProgram);
		virtual bool SetCShaderProgram(VSCShader*	pCShaderProgram);

		//Load Release
		virtual bool OnLoadVShaderProgram(VSVShader * pVShaderProgram, VSResourceIdentifier *&pID);

		virtual bool OnLoadPShaderProgram(VSPShader* pPShaderProgram, VSResourceIdentifier *&pID);

		virtual bool OnLoadGShaderProgram(VSGShader * pGShaderProgram, VSResourceIdentifier *&pID);

		virtual bool OnLoadDShaderProgram(VSDShader * pDShaderProgram, VSResourceIdentifier *&pID);

		virtual bool OnLoadHShaderProgram(VSHShader * pHShaderProgram, VSResourceIdentifier *&pID);

		virtual bool OnLoadCShaderProgram(VSCShader * pCShaderProgram, VSResourceIdentifier *&pID);

		virtual bool OnLoadTexture(VSTexture * pTexture, VSResourceIdentifier *&pID);

		virtual bool OnLoadDepthStencil(VSDepthStencil * pDepthStencil, VSResourceIdentifier *&pID);

		virtual bool OnLoadRenderTarget(VSRenderTarget * pRenderTarget, VSResourceIdentifier *&pID);

		virtual bool OnLoadUnorderAccess(VSBufferUnorderAccess  * pUnorderAccess, VSResourceIdentifier *&pID);
		virtual bool OnLoadUnorderAccess(VSTextureUnorderAccess  * pUnorderAccess, VSResourceIdentifier *&pID);

		virtual bool OnLoadVBufferData(VSVertexBuffer * pVBuffer, VSResourceIdentifier *&pID);

		virtual bool OnLoadVBufferFormat(VSVertexFormat * pVertexFormat, VSResourceIdentifier *&pID);

		virtual bool OnLoadIBuffer(VSIndexBuffer* pIBuffer, VSResourceIdentifier *&pID);

		virtual bool OnLoadSBuffer(VSBufferResource* pSBuffer, VSResourceIdentifier *&pID);

		virtual bool OnLoadBlendState(VSBlendState * pBlendState, VSResourceIdentifier *&pID);

		virtual bool OnLoadDepthStencilState(VSDepthStencilState * pDepthStencilState, VSResourceIdentifier *&pID);

		virtual bool OnLoadRasterizerState(VSRasterizerState * pRasterizerState, VSResourceIdentifier *&pID);

		virtual bool OnLoadSamplerState(VSSamplerState * pSamplerState, VSResourceIdentifier *&pID);

		virtual bool OnLoadQuery(VSQuery * pQuery, VSResourceIdentifier *&pID);

		virtual unsigned int SetVertexFormat(VSVertexFormat * pVertexFormat);

		virtual void * Lock(VSVertexBuffer * pVertexBuffer);
		virtual void UnLock(VSVertexBuffer * pVertexBuffer);

		virtual void * Lock(VSIndexBuffer * pIndexBuffer);
		virtual void UnLock(VSIndexBuffer * pIndexBuffer);

		virtual void * Lock(VSBufferResource * pStructBuffer);
		virtual void UnLock(VSBufferResource * pStructBuffer);

		virtual void * Lock(VSTexture * pTexture, unsigned int uiLevel, unsigned int uiFace);
		virtual void UnLock(VSTexture * pTexture, unsigned int uiLevel, unsigned int uiFace);

		virtual void * Lock(VSRenderTarget * pRenderTarget);
		virtual void UnLock(VSRenderTarget * pRenderTarget);

		virtual void * Lock(VSDepthStencil * pDepthStencil);
		virtual void UnLock(VSDepthStencil * pDepthStencil);

		virtual void * Lock(VSUnorderAccess  * pUnorderAccess);
		virtual void UnLock(VSUnorderAccess  * pUnorderAccess);

		virtual bool GetData(VSQuery * pQuery, void * pData, unsigned int uiSize);

		virtual bool CopyResourceBuffer(VS2DTexture * pSource, VSCubeTexture * pDest, unsigned int uiFace);

		D3D_SHADER_MACRO * GetDefine(VSMap<VSString, VSString> & Define);


		
	protected:
		void ClearDepthStencilView(UINT ClearFlags, VSREAL Depth, unsigned char Stencil);
		void OMSetDepthStencilState(VSDepthStencilStateID *pDepthStencilStateID, UINT StencilRef);
		void RSSetState(VSRasterStateID *pRasterStateID);
		void RSSetViewports(D3D11_VIEWPORT& View);
		bool EndRender();
		void UseWindowEx(int uiWindowID);
		void PSSetShaderResources(UINT StartSlot, VSTextureID *pTextureID);
		void VSSetShaderResources(UINT StartSlot, VSTextureID *pTextureID);
		void GSSetShaderResources(UINT StartSlot, VSTextureID *pTextureID);
		void DSSetShaderResources(UINT StartSlot, VSTextureID *pTextureID);
		void HSSetShaderResources(UINT StartSlot, VSTextureID *pTextureID);
		void CSSetShaderResources(UINT StartSlot, VSTextureID *pTextureID);

		void CSSetBufferResources(UINT StartSlot, VSSBufferID *pBufferResourceID);
		void VSSetBufferResources(UINT StartSlot, VSSBufferID *pBufferResourceID);
		void PSSetBufferResources(UINT StartSlot, VSSBufferID *pBufferResourceID);
		void GSSetBufferResources(UINT StartSlot, VSSBufferID *pBufferResourceID);
		void HSSetBufferResources(UINT StartSlot, VSSBufferID *pBufferResourceID);
		void DSSetBufferResources(UINT StartSlot, VSSBufferID *pBufferResourceID);

		bool CreateInputLayout(const VSArray<VSVertexFormat::VERTEXFORMAT_TYPE>& FormatArray, VSVBufferFormatID * pVBufferFormatID);
		void OMSetBlendState(VSBlendStateID* pBlendStateID);
		void RSSetScissorRects(UINT NumRects, D3D11_RECT *pRects);
		void UpdateSubresource(ID3D11Resource **pDstResource, void *pSrcData, unsigned int uiDataSize);
		void IASetVertexBuffers(unsigned int uiStream,VSVBufferID* pVBufferID, unsigned int uiStrides);
		void IASetIndexBuffer(VSIBufferID* pIBufferID, DXGI_FORMAT Format);

		void BeginQuery(VSQueryID* pQueryID);
		void EndQuery(VSQueryID* pQueryID);

		void VSSetSamplers(UINT StartSlot, VSSamplerStateID* pSamplerStateID);
		void PSSetSamplers(UINT StartSlot, VSSamplerStateID* pSamplerStateID);
		void GSSetSamplers(UINT StartSlot, VSSamplerStateID* pSamplerStateID);
		void DSSetSamplers(UINT StartSlot, VSSamplerStateID* pSamplerStateID);
		void HSSetSamplers(UINT StartSlot, VSSamplerStateID* pSamplerStateID);
		void CSSetSamplers(UINT StartSlot, VSSamplerStateID* pSamplerStateID);

		void SetRenderTargetsEx(VSRenderTargetID *pRenderTarget[], unsigned int uiNum, unsigned int uiBackMacthType);
		void EndRenderTargetsEx(VSRenderTarget *pRenderTarget[],VSRenderTargetID *pRenderTargetID[], unsigned int uiNum);
		void SetDepthStencilBufferEx(VSDepthStencilID *pDepthStencilID);
		void EndDepthStencilBufferEx(VSDepthStencil * pDepthStencil, VSDepthStencilID *pDepthStencilID);
		void SetCSUnorderAccessesEx(VSUnorderAccessID *pUnorderAccess[], unsigned int uiNum);
		void EndCSUnorderAccessesEx(VSUnorderAccess *pUnorderAccess[], VSUnorderAccessID *pUnorderAccessID[], unsigned int uiNum);

		void VSSetShader(VSVProgramID *pVProgramID);
		void PSSetShader(VSPProgramID *pPProgramID);
		void GSSetShader(VSGProgramID *pGProgramID);
		void DSSetShader(VSDProgramID *pDProgramID);
		void HSSetShader(VSHProgramID *pHProgramID);
		void CSSetShader(VSCProgramID *pCProgramID);

		void CreateVShaderFromString(VSVShader * pVShaderProgram, VSVProgramID *pID);
		void CreateVShaderFromCache(VSVShader * pVShaderProgram, VSVProgramID *pID);

		void CreatePShaderFromString(VSPShader* pPShaderProgram, VSPProgramID *pID);
		void CreatePShaderFromCache(VSPShader* pPShaderProgram, VSPProgramID *pID);

		void CreateGShaderFromString(VSGShader* pGShaderProgram, VSGProgramID *pID);
		void CreateGShaderFromCache(VSGShader* pGShaderProgram, VSGProgramID *pID);

		void CreateDShaderFromString(VSDShader* pDShaderProgram, VSDProgramID *pID);
		void CreateDShaderFromCache(VSDShader* pDShaderProgram, VSDProgramID *pID);

		void CreateHShaderFromString(VSHShader* pHShaderProgram, VSHProgramID *pID);
		void CreateHShaderFromCache(VSHShader* pHShaderProgram, VSHProgramID *pID);

		void CreateCShaderFromString(VSCShader* pCShaderProgram, VSCProgramID *pID);
		void CreateCShaderFromCache(VSCShader* pCShaderProgram, VSCProgramID *pID);

		void CreateTexture2D(VS2DTexture *pTexture, D3D11_TEXTURE2D_DESC &desc, VSTextureID * pTextureID);
		void CreateTexture3D(VS3DTexture *pTexture, D3D11_TEXTURE3D_DESC &desc, VSTextureID * pTextureID);
		void CreateTexture2DArray(VS2DTextureArray *pTexture, D3D11_TEXTURE2D_DESC &desc,VSTextureID * pTextureID);
		void CreateTexture1D(VS1DTexture *pTexture, D3D11_TEXTURE1D_DESC &desc, VSTextureID * pTextureID);
		void OnLoadDepthStencilEx(unsigned int Width, unsigned int Height, unsigned int Format, unsigned int MSType,
			unsigned int LockFlag, unsigned int uiTextureType, unsigned int uiDepthStencilLevel, unsigned int uiFirst,
			unsigned int uiArraySize, VSTextureID * pTextureID, VSDepthStencilID *pDepthStencilID);
		void OnLoadRenderTargetEx(unsigned int Width, unsigned int Height, unsigned int Format, unsigned int MSType,
			unsigned int LockFlag, unsigned int uiTextureType,unsigned int uiRenderTargetLevel,unsigned int uiFirst,
			unsigned int uiArraySize,VSTextureID * pTextureID, VSRenderTargetID *pRenderTargetID);
		void OnLoadUnOrderAccessEx(unsigned int uiNum, unsigned int uiDataType,unsigned int uiByteWidth,
			unsigned int uiStructureByteStride, unsigned int LockFlag, VSSBufferID * pSBufferID, VSUnorderAccessID *pUnorderAccessID);
		void OnLoadTextureUnorderAccessEx(unsigned int Width, unsigned int Height, unsigned int Format,
			unsigned int LockFlag, unsigned int uiTextureType, unsigned int uiUnorderAccessLevel, unsigned int uiFirst,
			unsigned int uiArraySize, VSTextureID * pTextureID, VSUnorderAccessID *pUnorderAccessID);
		void OnLoadVertexBufferEx(D3D11_BUFFER_DESC& desc,VSVertexBuffer * pVBuffer, VSVBufferID * pVBufferID);
		void OnLoadIndexBufferEx(D3D11_BUFFER_DESC& desc,void* pBuffer, VSIBufferID * pIBufferID);
		void OnLoadSBufferEX(D3D11_BUFFER_DESC &Desc, D3D11_SHADER_RESOURCE_VIEW_DESC& SRVDesc,void* pBuffer, VSSBufferID * pSBufferID);
		bool DrawDynamicBufferMesh(VSDynamicBufferGeometry *pDBGeometry);
		bool DrawMesh(VSGeometry * pGeometry);
		void DrawDynamicScreenData(void * pScreenBuffer, unsigned int uiVertexNum, unsigned int uiVertexStride, VSUSHORT_INDEX * pIndexBuffer, unsigned int uiIndexNum);
		void OnLoadVShaderFromString(VSVShader * pVShaderProgram, VSVProgramID *pVProgramID);
		void OnLoadPShaderFromString(VSPShader* pPShaderProgram, VSPProgramID *pPProgramID);
		void OnLoadGShaderFromString(VSGShader* pGShaderProgram, VSGProgramID *pGProgramID);
		void OnLoadDShaderFromString(VSDShader* pDShaderProgram, VSDProgramID *pDProgramID);
		void OnLoadHShaderFromString(VSHShader* pHShaderProgram, VSHProgramID *pHProgramID);
		void OnLoadCShaderFromString(VSCShader* pCShaderProgram, VSCProgramID *pCProgramID);

		void CreateQuery(D3D11_QUERY Type, VSQueryID * pID);
		void CreateBlendState(const D3D11_BLEND_DESC &desc, VSBlendStateID * pBlendStateID);
		void CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC depthStencilDesc, VSDepthStencilStateID * pDepthStenciStateID);
		void CreateRasterizerState(const D3D11_RASTERIZER_DESC rasterDesc, VSRasterStateID * pRasterStateID);
		void CreateSamplerState(const D3D11_SAMPLER_DESC& sampDesc, VSSamplerStateID * pSamplerStateID);
		void IASetInputLayout(VSVBufferFormatID * pVBufferFormatID);

		void OnDispath(unsigned int uiThreadGroupCountX, unsigned int uiThreadGroupCountY,
			unsigned int uiThreadGroupCountZ);
	public:
		enum	//Register Type
		{
			RT_T,
			RT_S,
			RT_MAX
		};
		//shader function
		virtual VSString TexColorBuffer(const VSColorBuffer * pColorBuffer) const;
		virtual VSString Tex2D(const VS2DTexSampler * p2DTexSampler, unsigned int uiShaderType) const;
		virtual VSString Tex1D(const VS1DTexSampler * p1DTexSampler, unsigned int uiShaderType) const;
		
		virtual VSString Mul(const VSMul * pMul) const;
		virtual VSString Div(const VSDiv * pDiv) const;
		virtual VSString Dot(const VSDot * pDot) const;
		virtual VSString Length(const VSLength * pLength) const;
		virtual VSString Sub(const VSSub * pSub) const;
		virtual VSString Add(const VSAdd * pAdd) const;
		virtual VSString MakeValue(const VSMakeValue * pMakeValue) const;
		virtual VSString Saturate(const VSSaturate * pSaturate) const;
		VSString Float(unsigned int uiIndex)const;
		virtual VSString Sampler(unsigned int uiType)const;
		virtual VSString FloatByChannelNum(unsigned int ChannelNum)const;
		virtual VSString Float()const;
		virtual VSString Float2()const;
		virtual VSString Float3()const;
		virtual VSString Float4()const;
		virtual VSString Return()const;

		virtual VSString FloatConst(const VSString & Value1)const;
		virtual VSString Float2Const(const VSString & Value1, const VSString & Value2)const;
		virtual VSString Float3Const(const VSString & Value1,
			const VSString & Value2, const VSString & Value3)const;
		virtual VSString Float4Const(const VSString & Value1, const VSString & Value2,
			const VSString & Value3, const VSString & Value4)const;

		virtual void CustomPointLightUseString(VSString & PointLightUseString, VSString & PointAttenuationDivString);

		virtual void CustomSpotLightUseString(VSString & SpotLightUseString, VSString & SpotAttenuationDivString);


		virtual VSString GetDeclareValue(const VSString &ShowName, unsigned int uiValueType, unsigned int uiRegisterIndex)const;
		virtual VSString GetDeclareSampler(const VSString &ShowName, unsigned int SamplerType, unsigned int uiRegisterIndex, unsigned int SamplerSource)const;
		virtual void GetDeclareShareSampler(unsigned int SamplerSource, unsigned int uiRegisterIndex, VSString & OutString)const;

		virtual VSString SetRegister(unsigned int uiRegisterType, unsigned int uiRegisterIndex)const;


		//shader custom function
		virtual void CreateVInputDeclare(MaterialShaderPara &MSPara,
			VSString & OutString);
		virtual void CreateVOutputDeclare(MaterialShaderPara &MSPara,
			VSString & OutString);
		virtual void CreateVUserConstant(VSVShader * pVShader, MaterialShaderPara &MSPara,
			VSString & OutString);
		virtual void CreateVFunction(MaterialShaderPara &MSPara,
			VSString & OutString);

		virtual void SetMaterialVShaderConstant(MaterialShaderPara &MSPara, VSVShader *pVShader);

		virtual void CreatePInputDeclare(MaterialShaderPara &MSPara,
			VSString & OutString);
		virtual void CreatePOutputDeclare(MaterialShaderPara &MSPara,
			VSString & OutString);
		virtual void CreatePUserConstant(VSPShader * pPShader, MaterialShaderPara &MSPara,
			VSString & OutString);
		virtual void CreatePFunction(MaterialShaderPara &MSPara,
			VSString & OutString);

		virtual void SetMaterialPShaderConstant(MaterialShaderPara &MSPara,VSPShader *pPShader);

		virtual void CreateGInputDeclare(MaterialShaderPara &MSPara,
			VSString & OutString);
		virtual void CreateGOutputDeclare(MaterialShaderPara &MSPara,
			VSString & OutString);
		virtual void CreateGUserConstant(VSGShader * pGShader, MaterialShaderPara &MSPara,
			VSString & OutString);
		virtual void CreateGFunction(MaterialShaderPara &MSPara,
			VSString & OutString);
		virtual void SetMaterialGShaderConstant(MaterialShaderPara &MSPara,VSGShader *pGShader);

		virtual void CreateHInputDeclare(MaterialShaderPara &MSPara,
			VSString & OutString);
		virtual void CreateHOutputDeclare(MaterialShaderPara &MSPara,
			VSString & OutString);
		virtual void CreateHUserConstant(VSHShader * pHShader, MaterialShaderPara &MSPara,
			VSString & OutString);
		virtual void CreateHFunction(MaterialShaderPara &MSPara,
			VSString & OutString);
		virtual void SetMaterialHShaderConstant(MaterialShaderPara &MSPara, VSHShader * pHShader);

		virtual void CreateDInputDeclare(MaterialShaderPara &MSPara,
			VSString & OutString);
		virtual void CreateDOutputDeclare(MaterialShaderPara &MSPara,
			VSString & OutString);
		virtual void CreateDUserConstant(VSDShader * pDShader, MaterialShaderPara &MSPara,
			VSString & OutString);
		virtual void CreateDFunction(MaterialShaderPara &MSPara,
			VSString & OutString);
		virtual void SetMaterialDShaderConstant(MaterialShaderPara &MSPara, VSDShader *pDShader);

		virtual void GetIncludeShader(VSString &OutString);

		virtual void GetDynamicShader(VSString &OutString);


		virtual void SkyLight(const VSString &WorldNormal, const VSString &UpColor, const VSString & DownColor, const VSString & DiffuseColor, VSString & OutString);
		virtual void DirectionalLight(int iLightNum, const VSString &Diffuse, const VSString &Specular, const VSString &SpecularPow,
			const VSString &WorldNormal, const VSString &WorldCameraDir, VSArray<VSString> ShadowString,
			VSString & OutString)const;
		virtual void CustomDirectionalLight(int iLightNum, const VSString &CustomString, VSArray<VSString> ShadowString, VSString & OutString)const;

		virtual void OrenNayarDirectionalLight(int iLightNum, const VSString &Diffuse, const VSString &RoughnessSquared,
			const VSString &WorldNormal, const VSString &WorldCameraDir, bool bLookUpTable, VSArray<VSString> ShadowString,
			VSString & OutString)const;

		virtual void PointLight(int iLightNum, const VSString &Diffuse, const VSString &Specular, const VSString &SpecularPow,
			const VSString & WorldPos, const VSString &WorldNormal, const VSString &WorldCameraDir, VSArray<VSString> ShadowString,
			VSString & OutString)const;
		virtual void CustomPointLight(int iLightNum, const VSString &CustomString, const VSString & WorldPos, VSArray<VSString> ShadowString, VSString & OutString)const;

		virtual void OrenNayarPointLight(int iLightNum, const VSString &Diffuse, const VSString &RoughnessSquared,
			const VSString & WorldPos, const VSString &WorldNormal, const VSString &WorldCameraDir, bool bLookUpTable, VSArray<VSString> ShadowString,
			VSString & OutString)const;
	

		virtual void SpotLight(int iLightNum, const VSString &Diffuse, const VSString &Specular, const VSString &SpecularPow,
			const VSString & WorldPos, const VSString &WorldNormal, const VSString &WorldCameraDir, VSArray<VSString> ShadowString,
			VSString & OutString)const;
		virtual void CustomSpotLight(int iLightNum, const VSString &CustomString, const VSString & WorldPos, VSArray<VSString> ShadowString, VSString & OutString)const;
		virtual void OrenNayarSpotLight(int iLightNum, const VSString &Diffuse, const VSString &RoughnessSquared,
			const VSString & WorldPos, const VSString &WorldNormal, const VSString &WorldCameraDir, bool bLookUpTable, VSArray<VSString> ShadowString,
			VSString & OutString)const;

		virtual void LocalToWorldPos(const VSString & LocalPos, VSString &OutString)const;
		virtual void WorldToViewPos(const VSString & LocalPos, VSString &OutString)const;

		virtual void LocalToWorldNormal(const VSString & LocalNormal, VSString &OutString)const;

		virtual void WorldToViewNormal(const VSString & WorldNormal, VSString &OutString)const;

		virtual void GetWorldViewDir(VSString &OutString)const;

		virtual void TransProjPos(const VSString & Pos, const VSString & Matrix, VSString &OutString)const;
		virtual void BumpNormal(const VSString &TexNormal, VSString &OutString)const;

		virtual void ComputeDir(const VSString & Orgin, const VSString & End, VSString &OutString)const;
		virtual void ComputeLength(const VSString & Orgin, const VSString & End, VSString &OutString)const;
		virtual void GetLightType(unsigned int i, VSString &OutString)const;
		virtual void GetLightTypeTemp(VSString &OutString)const;
		virtual void ComputeBoneVector(const VSString& BoneIndex, const VSString & BoneWeight, const VSString &BoneVector,
			const  VSString & U, const VSString & V, const VSString &N, VSString &OutString)const;
		virtual void ComputeBoneVector(const VSString& BoneIndex, const VSString & BoneWeight,
			const VSString &AnimDataResource, const VSString & AnimAllInfo,
			const VSString &AnimInstanceInfo, const VSString &AnimNum,
			const  VSString & U, const VSString & V, const VSString &N, VSString &OutString)const;

		virtual void BoneTranPos(const VSString & LocalPos, const  VSString & U, const VSString & V, const VSString &N, VSString &OutString)const;
		virtual void BoneTranNormal(const VSString & LocalNormal, const  VSString & U, const VSString & V, const VSString &N, VSString &OutString)const;

		virtual void DecodeNormal1(const VSString & Normal, VSString &OutString) const;
		virtual void RecodeNormal1(const VSString & Normal, VSString &OutString) const;
		virtual void DecodeNormal2(const VSString & Normal, const VSString & Tengent, VSString &OutString) const;

		virtual void DecodeNormal3(const VSString & Normal, VSString &OutString) const;
		virtual void RecodeNormal3(const VSString & Normal, VSString &OutString) const;

		virtual void Reciprocal(const VSString & Value, VSString &OutString)const;
		virtual void Clip(const VSString & ClipValue, VSString &OutString)const;
		virtual void GreaterZeroPow(const VSString & Value, VSREAL Exp, VSString &OutString) const;
		virtual void SaturatePow(const VSString & Value, VSREAL Exp, VSString &OutString) const;
		virtual void Saturate(const VSString & Value, VSString &OutString)const;

		virtual void PointLightCubeShadow(const VSString & PointLightName, const VSString & CubeShadowSampler, const VSString & ViewMatrix,
			const VSString & WorldPos, VSString& OutString)const;

		virtual void PointLightCubeShadow(const VSString & PointLightName, const VSString & CubeShadowSampler,
			const VSString & WorldPos, VSString& OutString)const;

		virtual void PointLightVolumeShadow(const VSString & PointLightName, const VSString & VolumeShadowSampler,
			const VSString & WorldPos, VSString& OutString)const;

		virtual void PointLightDualParaboloidShadow(const VSString & PointLightName, const VSString & ShadowSampler,
			const VSString & WorldPos, VSString& OutString)const;
		virtual void DirectionLightVolumeShadow(const VSString & DirLightName, const VSString & VolumeShadowSampler,
			const VSString & WorldPos, VSString& OutString)const;

		virtual void DirectionLightCSMShadow(const VSString & DirLightName, const VSString & ShadowSampler,
			const VSString & WorldPos, const VSString & ViewPos, const VSString &FarZ, VSString& OutString)const;

		virtual void DirectionLightShadow(const VSString & DirLightName, const VSString & ShadowSampler,
			const VSString & WorldPos, VSString& OutString)const;

		virtual void SpotLightShadow(const VSString & SpotLightName, const VSString & ShadowSampler,
			const VSString & WorldPos, VSString& OutString)const;

		virtual void TranLightToTemp(VSArray<VSLight *> LightArray, VSString & OutString)const;

		virtual void GetLightFunction(VSArray<VSLight *> LightArray, const VSString & WorldPos, VSString & OutString)const;

		virtual void EncodeReflect(const VSString & Mip, const VSString & Pow, VSString& OutString);

		virtual void DecodeReflect(const VSString & Mip, const VSString & Pow, const VSString & Value, VSString& OutString);
	private:
		// create user constant
		void CreateUserConstantLightShadowMatrix(VSShader * pShader, unsigned int& ID,
			VSString & OutString);

		void CreateUserConstantProjectMatrix(VSShader * pShader, unsigned int& ID,
			VSString & OutString);

		void CreateUserConstantViewProjectMatrix(VSShader * pShader, unsigned int& ID,
			VSString & OutString);

		void CreateUserConstantViewMatrix(VSShader * pShader, unsigned int& ID,
			VSString & OutString);
		void CreateUserConstantLightWorldPos(VSShader * pShader, unsigned int& ID,
			VSString & OutString);

		void CreateUserConstantLightDirection(VSShader * pShader, unsigned int& ID,
			VSString & OutString);

		void CreateUserConstantFarZ(VSShader * pShader, unsigned int& uiRegisterID,
			VSString & OutString);
		void CreateUserConstantSkyLightUpColor(VSShader * pShader, unsigned int& uiRegisterID,
			VSString & OutString);
		void CreateUserConstantSkyLightDownColor(VSShader * pShader, unsigned int& uiRegisterID,
			VSString & OutString);

		void CreateUserConstantLight(VSShader * pShader, MaterialShaderPara &MSPara, unsigned int& uiRegisterID,
			VSString & OutString);
		void CreateUserConstantPointLightRange(VSShader * pShader, unsigned int& uiRegisterID,
			VSString & OutString);
		void CreateUserConstantSkin(VSGeometry * pGeometry, VSShader * pShader, unsigned int& ID,
			VSString & OutString);
		void CreateUserConstantSimpleSkinInstanceTexture(VSGeometry * pGeometry, VSShader * pShader, unsigned int& uiTexRegisterID,
			VSString & OutString);

		void CreateUserConstantMorph(VSGeometry * pGeometry, VSShader * pShader, unsigned int& ID,
			VSString & OutString);
		void CreateUserConstantMorphTexture(VSGeometry * pGeometry, VSShader * pShader, unsigned int& uiTexRegisterID,
			VSString & OutString);

		void CreateUserConstantWorldMatrix(VSShader * pShader, unsigned int& ID,
			VSString & OutString);
		void CreateUserConstantCameraWorldPos(VSShader * pShader, unsigned int& ID,
			VSString & OutString);
		void CreateUserConstantDirectionLight(VSShader * pShader, unsigned int& ID, unsigned int uiLightNum,
			VSString & OutString);
		void CreateUserConstantPointLight(VSShader * pShader, unsigned int& ID, unsigned int uiLightNum,
			VSString & OutString);
		void CreateUserConstantSpotLight(VSShader * pShader, unsigned int& ID, unsigned int uiLightNum,
			VSString & OutString);

		void CreateUserConstantInvRTWidth(VSShader * pShader, unsigned int& uiRegisterID,
			VSString & OutString);

		void CreateUserConstantShadow(VSPShader* pPShader, VSArray<VSLight*> & LightArray, unsigned int &ID, VSString & OutString);
		void CreateUserConstantLightFunction(VSPShader* pPShader, VSArray<VSLight*> & LightArray, unsigned int &ID, VSString & OutString);
		void CreateLightModleLookUpTable(VSPShader* pPShader, MaterialShaderPara &MSPara, unsigned int &uiTexRegisterID, VSString & OutString);

			

		// set user constant
		void SetUserConstantLightWorldPos(MaterialShaderPara &MSPara, VSShader * pShader, unsigned int& ID);
		void SetUserConstantLightWorldDirection(MaterialShaderPara &MSPara, VSShader * pShader, unsigned int& ID);
		void SetUserLightShadowMatrix(MaterialShaderPara &MSPara, VSShader * pShader, unsigned int& ID);

		void SetUserConstantSkin(MaterialShaderPara &MSPara, VSShader * pShader, unsigned int& ID);
		void SetUserConstantSimpleAnimInstanceTexture(MaterialShaderPara &MSPara, VSShader * pShader, unsigned int& ID);
		void SetUserConstantWorldMatrix(MaterialShaderPara &MSPara, VSShader * pShader, unsigned int& ID);
		void SetUserConstantViewProjectMatrix(MaterialShaderPara &MSPara, VSShader * pShader, unsigned int& ID);
		void SetUserConstantViewMatrix(MaterialShaderPara &MSPara, VSShader * pShader, unsigned int& ID);
		void SetUserConstantProjectMatrix(MaterialShaderPara &MSPara, VSShader * pShader, unsigned int& ID);
		void SetUserConstantCameraPos(MaterialShaderPara &MSPara, VSShader * pShader, unsigned int& ID);
		void SetUserConstantSkyUpColor(MaterialShaderPara &MSPara, VSShader * pShader, unsigned int& ID);
		void SetUserConstantSkyDownColor(MaterialShaderPara &MSPara, VSShader * pShader, unsigned int& ID);
		void SetUserConstantLight(MaterialShaderPara &MSPara, VSShader * pShader, unsigned int& ID);
		void SetUserConstantShadowSampler(MaterialShaderPara &MSPara, VSShader * pShader);
		void SetUserConstantLightFunctionSampler(MaterialShaderPara &MSPara, VSShader * pShader);
		void SetUserConstantFarZ(MaterialShaderPara &MSPara, VSShader * pShader, unsigned int& ID);
		void SetUserConstantPointLightRange(MaterialShaderPara &MSPara, VSShader * pShader, unsigned int& ID);
		void SetUserConstantInvRTWidth(MaterialShaderPara &MSPara, VSShader * pShader, unsigned int& ID);
		void SetUserConstantMorph(MaterialShaderPara &MSPara, VSShader * pShader, unsigned int& ID);
		void SetUserConstantMorphTexture(MaterialShaderPara &MSPara, VSShader * pShader, unsigned int& ID);
		// create vsshader input
		void CreateVInputDeclareInstancePosition(MaterialShaderPara &MSPara,
			VSString & OutString);
		void CreateVInputDeclareMaterialInstance(MaterialShaderPara &MSPara,
			VSString & OutString);
		void CreateVInputDeclarePosition(MaterialShaderPara &MSPara,
			VSString & OutString);
		void CreateVInputDeclareTexCoord(MaterialShaderPara &MSPara,
			VSString & OutString);
		void CreateVInputDeclareNormal(MaterialShaderPara &MSPara,
			VSString & OutString);
		void CreateVInputDeclareColor(MaterialShaderPara &MSPara,
			VSString & OutString);
		void CreateVInputDeclareSkin(MaterialShaderPara &MSPara,
			VSString & OutString);

		//create vsshader function
		void CreateVFunctionDualParaboloidPosition(MaterialShaderPara &MSPara, VSString & FunctionBody);
		void CreateVFunctionVolumeShadowPosition(MaterialShaderPara &MSPara, VSString & FunctionBody);
		void CreateVFunctionVolumeShadowNormal(MaterialShaderPara &MSPara, VSString & FunctionBody);

		void CreateVFunctionMorphData(MaterialShaderPara &MSPara, VSString & FunctionBody);
		void CreateVFunctionSkinInfo(MaterialShaderPara &MSPara, VSString & FunctionBody);
		void CreateVFunctionNormal(MaterialShaderPara &MSPara, VSString & FunctionBody);
		void CreateVFunctionMaterialInstance(MaterialShaderPara &MSPara, VSString & FunctionBody);
		void CreateVFunctionPosition(MaterialShaderPara &MSPara, VSString & FunctionBody);
		void CreateVFunctionColor(MaterialShaderPara &MSPara, VSString & FunctionBody);
		void CreateVFunctionTexCoord(MaterialShaderPara &MSPara, VSString & FunctionBody);
		void CreateVFunctionProjectZ(MaterialShaderPara &MSPara, VSString & FunctionBody);
		void CreateVFunctionPost(MaterialShaderPara &MSPara, VSString & FunctionBody);
			
		// create  shader input / output
		void CreateDeclareTexCoord(MaterialShaderPara &MSPara, unsigned int& ID,
			VSString & OutString, bool bInput);
		void CreateDeclareNormal(MaterialShaderPara &MSPara, unsigned int& ID,
			VSString & OutString, bool bInput);
		void CreateDeclareColor(MaterialShaderPara &MSPara,
			VSString & OutString, bool bInput);
		void CreateDeclareTessellationValue(MaterialShaderPara &MSPara, VSString & OutString, bool bInput);
		void CreateDeclareInstancePos(MaterialShaderPara &MSPara, unsigned int& ID,
			VSString & OutString, bool bInput);
		void CreateDeclareMaterialInstance(MaterialShaderPara &MSPara,
			VSString & OutString, bool bInput);
		void CreateDeclareWorldPos(MaterialShaderPara &MSPara, unsigned int& ID, VSString & OutString, bool bInput);
		void CreateDeclareProjectZ(MaterialShaderPara &MSPara, unsigned int& ID,
			VSString & OutString, bool bInput);

		//create hsshader function
		void CreateHFunctionWorldPos(MaterialShaderPara &MSPara, VSString & FunctionBody);
		void CreateHFunctionTexCoord(MaterialShaderPara &MSPara, VSString & FunctionBody);
		void CreateHFunctionNormal(MaterialShaderPara &MSPara, VSString & FunctionBody);
		void CreateHFunctionColor(MaterialShaderPara &MSPara, VSString & FunctionBody);
		void CreateHFunctionInstancePos(MaterialShaderPara &MSPara, VSString & FunctionBody);
		void CreateHFunctionMaterialInstance(MaterialShaderPara &MSPara, VSString & FunctionBody);

		void CreateDFunctionWorldPos(MaterialShaderPara &MSPara, VSString & FunctionBody);
		void CreateDFunctionTexCoord(MaterialShaderPara &MSPara, VSString & FunctionBody);
		void CreateDFunctionNormal(MaterialShaderPara &MSPara, VSString & FunctionBody);
		void CreateDFunctionColor(MaterialShaderPara &MSPara, VSString & FunctionBody);
		void CreateDFunctionInstancePos(MaterialShaderPara &MSPara, VSString & FunctionBody);
		void CreateDFunctionMaterialInstance(MaterialShaderPara &MSPara, VSString & FunctionBody);

		void CreateDeclarePosition(MaterialShaderPara &MSPara,
			VSString & OutString, bool bInput);
		void CreateGBufferSampler(VSPShader* pPShader, MaterialShaderPara &MSPara, unsigned int &uiTexRegisterID, VSString & OutString);

	protected:
		static VSString ms_IncludeShader;

		static VSString ms_DynamicShader;

		static VSAdapterInfo	ms_AdapterInfo[5];
		static UINT			ms_uiAdapterNum;
	};

#endif
}
