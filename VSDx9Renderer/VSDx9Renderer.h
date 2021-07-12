#pragma once
#include "VSRenderer.h"
#include "VSResourceManager.h"
#ifdef WINDOWS_PLATFORM
#include <d3d9.h>
#include <d3dx9tex.h>
#ifdef VSDX9RENDERER_EXPORTS
#define  VSDX9RENDERER_API __declspec(dllexport) 
#else
#define  VSDX9RENDERER_API __declspec(dllimport)
#endif
#endif
namespace VSEngine2
{
#ifdef WINDOWS_PLATFORM
	#define VSMAC_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
	class VSDx9ShaderInclude;
	class VSVProgramID;
	class VSPProgramID;
	class VSQueryID;
	class VSDX9RENDERER_API VSDX9Renderer : public VSRenderer
	{
	public:
		VSDX9Renderer(HWND hMainWindow,unsigned int uiScreenWidth,unsigned int uiScreenHeight,bool bIsWindowed,
					unsigned int uiAnisotropy,unsigned int uiMultisample,
					ChildWindowInfo * pChildWindow,int uiNumChildWindow);
		virtual ~VSDX9Renderer();
		virtual int GetRendererType () const;
		virtual void ClearBackBuffer ();
		virtual void ClearZBuffer ();
		virtual void ClearStencilBuffer ();
		virtual void ClearBuffers ();
		virtual void ClearBuffers(unsigned int uiClearFlag);
		//virtual void DisplayBackBuffer ();
		virtual void DeviceLost();
		virtual void ResetDevice();
		virtual bool ChangeScreenSize(unsigned int uiWidth,unsigned int uiHeight,bool bWindow);

		virtual void ClearBackBuffer (int iXPos, int iYPos, int iWidth,
			int iHeight);
		virtual void ClearZBuffer (int iXPos, int iYPos, int iWidth,
			int iHeight);
		virtual void ClearStencilBuffer (int iXPos, int iYPos, int iWidth,
			int iHeight);
		virtual void ClearBuffers (int iXPos, int iYPos, int iWidth,
			int iHeight);
		virtual void ClearBuffers(unsigned int uiClearFlag,int iXPos, int iYPos, int iWidth,
			int iHeight);
		virtual unsigned int GetTextureCompressLen(unsigned int uiLen);	

		virtual bool CooperativeLevel();
		virtual bool BeginRendering();
		virtual bool EndRendering();
		virtual bool DrawMesh(VSGeometry * pGeometry, VSRenderState * pRenderState, VSVShader * pVShader, VSPShader * pPShader, 
			VSGShader * pGShader, VSHShader * pHShader, VSDShader * pDShader);
		virtual bool DrawScreen(SCREEN_QUAD_TYPE ScreenQuad[4]);
		virtual bool DrawScreen(SCREEN_QUAD_TYPE * pScreenBuffer,unsigned int uiVertexNum,
								VSUSHORT_INDEX * pIndexBuffer,unsigned int uiIndexNum);
		virtual bool DrawScreenFont(SCREEN_FONT_TYPE * pScreenFontBuffer, unsigned int uiVertexNum,
								VSUSHORT_INDEX * pIndexBuffer, unsigned int uiIndexNum);
	protected:
		
// 		virtual bool SetVProgramConstant (VSString &NameInShaderProgram,void * pData,unsigned int uiSize);
// 		virtual bool SetPProgramConstant (VSString &NameInShaderProgram,void * pData,unsigned int uiSize);
		virtual bool SetVProgramConstant (unsigned int uiStartRegister,void * pData,
											unsigned int RegisterNum,unsigned int uiType);
		virtual bool SetPProgramConstant (unsigned int uiStartRegister,void * pData,
											unsigned int RegisterNum,unsigned int uiType);
		//Load Release
		virtual bool OnLoadVShaderProgram (VSVShader * pVShaderProgram,VSResourceIdentifier *&pID);

		virtual bool OnLoadPShaderProgram (VSPShader* pPShaderProgram,VSResourceIdentifier *&pID);

		virtual bool OnLoadGShaderProgram(VSGShader * pGShaderProgram, VSResourceIdentifier *&pID);

		virtual bool OnLoadDShaderProgram(VSDShader * pDShaderProgram, VSResourceIdentifier *&pID);

		virtual bool OnLoadHShaderProgram(VSHShader * pHShaderProgram, VSResourceIdentifier *&pID);

		virtual bool OnLoadCShaderProgram(VSCShader * pCShaderProgram, VSResourceIdentifier *&pID);

		virtual bool OnLoadTexture (VSTexture * pTexture,VSResourceIdentifier *&pID);

		virtual bool OnLoadDepthStencil (VSDepthStencil * pDepthStencil,VSResourceIdentifier *&pID);
		
		virtual bool OnLoadRenderTarget (VSRenderTarget * pRenderTarget,VSResourceIdentifier *&pID);

		virtual bool OnLoadUnorderAccess(VSBufferUnorderAccess  * pUnorderAccess, VSResourceIdentifier *&pID);
		virtual bool OnLoadUnorderAccess(VSTextureUnorderAccess  * pUnorderAccess, VSResourceIdentifier *&pID);

		virtual bool OnLoadVBufferData (VSVertexBuffer * pVBuffer,VSResourceIdentifier *&pID);

		virtual bool OnLoadVBufferFormat (VSVertexFormat * pVertexFormat,VSResourceIdentifier *&pID);

		virtual bool OnLoadIBuffer (VSIndexBuffer* pIBuffer,VSResourceIdentifier *&pID);

		virtual bool OnLoadSBuffer(VSBufferResource* pSBuffer, VSResourceIdentifier *&pID);

		virtual bool OnLoadBlendState (VSBlendState * pBlendState,VSResourceIdentifier *&pID);

		virtual bool OnLoadDepthStencilState (VSDepthStencilState * pDepthStencilState,VSResourceIdentifier *&pID);

		virtual bool OnLoadRasterizerState(VSRasterizerState * pRasterizerState,VSResourceIdentifier *&pID);

		virtual bool OnLoadSamplerState(VSSamplerState * pSamplerState,VSResourceIdentifier *&pID);

		virtual bool OnLoadQuery(VSQuery * pQuery, VSResourceIdentifier *&pID);

		virtual bool SetVShaderProgram(VSVShader*	pVShaderProgram);
		virtual bool SetPShaderProgram(VSPShader*	pPShaderProgram);
		virtual bool SetGShaderProgram(VSGShader*	pGShaderProgram);
		virtual bool SetDShaderProgram(VSDShader*	pDShaderProgram);
		virtual bool SetHShaderProgram(VSHShader*	pHShaderProgram);
		virtual bool SetCShaderProgram(VSCShader*	pCShaderProgram);

		virtual unsigned int SetVertexFormat(VSVertexFormat * pVertexFormat);

		

		
		virtual void SetVShaderConstant(VSVShader * pShader);
		virtual void SetPShaderConstant(VSPShader * pShader);
		virtual void SetGShaderConstant(VSGShader * pShader);
		virtual void SetDShaderConstant(VSDShader * pShader);
		virtual void SetHShaderConstant(VSHShader * pShader);
		virtual void SetCShaderConstant(VSCShader * pShader);




		D3DXMACRO * GetDefine(VSMap<VSString,VSString> & Define);


		virtual void * Lock(VSVertexBuffer * pVertexBuffer);
		virtual void UnLock(VSVertexBuffer * pVertexBuffer);
		virtual void * Lock(VSIndexBuffer * pIndexBuffer);
		virtual void UnLock(VSIndexBuffer * pIndexBuffer);
		virtual void * Lock(VSBufferResource * pStructBuffer);
		virtual void UnLock(VSBufferResource * pStructBuffer);
		virtual void * Lock(VSTexture * pTexture,unsigned int uiLevel,unsigned int uiFace);
		virtual void UnLock(VSTexture * pTexture,unsigned int uiLevel,unsigned int uiFace);
		virtual void * Lock(VSRenderTarget * pRenderTarget);
		virtual void UnLock(VSRenderTarget * pRenderTarget);

		virtual void * Lock(VSDepthStencil * pDepthStencil);
		virtual void UnLock(VSDepthStencil * pDepthStencil);

		virtual void * Lock(VSUnorderAccess  * pUnorderAccess);
		virtual void UnLock(VSUnorderAccess  * pUnorderAccess);

		virtual bool GetData(VSQuery * pQuery, void * pData, unsigned int uiSize);

		virtual bool CopyResourceBuffer(VS2DTexture * pSource,VSCubeTexture * pDest,unsigned int uiFace);
	private:
		
 		bool SetRenderState(D3DRENDERSTATETYPE State,DWORD Value);
 		bool SetSamplerState(DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD Value);
		bool Clear(DWORD Count,CONST D3DRECT* pRects,DWORD Flags,D3DCOLOR Color,VSREAL Z,DWORD Stencil);
		bool SetClipPlane(DWORD Index,CONST VSREAL* pPlane);
		bool SetScissorRect(CONST RECT* pRect);
		bool SetViewport(CONST D3DVIEWPORT9* pViewport);
		bool SetVertexShaderConstant(unsigned int uiStartRegister,void * pData,
									unsigned int RegisterNum,unsigned int uiType);
		bool SetPixelShaderConstant(unsigned int uiStartRegister,void * pData,
			unsigned int RegisterNum,unsigned int uiType);
		bool DrawDynamicBufferMesh(VSDynamicBufferGeometry *pDBGeometry);
		bool DrawMesh(VSGeometry * pGeometry);
		bool DrawScreenEX2(SCREEN_QUAD_TYPE * pScreenBuffer,unsigned int uiVertexNum,
			VSUSHORT_INDEX * pIndexBuffer,unsigned int uiNumTri);
		bool DrawScreenEX1(SCREEN_QUAD_TYPE ScreenQuad[4]);
		
		bool DrawScreenFontEX(SCREEN_FONT_TYPE * pScreenFontBuffer, unsigned int uiVertexNum,
			VSUSHORT_INDEX * pIndexBuffer, unsigned int uiNumTri);
		bool BeginRender();
		bool EndRender();
		bool UseWindowEx(int uiWindowID);


		//resource 

		bool SetVertexShader(IDirect3DVertexShader9** pShader);
		bool SetPixelShader(IDirect3DPixelShader9** pShader);
		bool SetTexture(DWORD Stage,IDirect3DBaseTexture9** pTexture);
		bool SetStreamSource(UINT StreamNumber,IDirect3DVertexBuffer9** pStreamData,UINT OffsetInBytes,UINT Stride);
		bool SetVertexDeclaration(IDirect3DVertexDeclaration9** pDecl);
		bool SetIndices(IDirect3DIndexBuffer9** pIndexData);
		bool BeginQuery(IDirect3DQuery9** pQuery);
		bool EndQuery(IDirect3DQuery9** pQuery);
		bool SetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9** pRenderTarget);
		bool SetDepthStencilSurface(IDirect3DSurface9** pNewZStencil);
		bool SetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9** pRenderTarget,IDirect3DSurface9** ppRenderTarget);
		bool EndRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9** pRenderTarget);
		bool SetDepthStencilSurface(IDirect3DSurface9** pNewZStencil,IDirect3DSurface9** ppZStencilSurface);
		bool EndDepthStencilSurface(IDirect3DSurface9** pNewZStencil);

		bool GetRenderTargetData(IDirect3DSurface9 **pRenderTarget,IDirect3DSurface9 **pDestSurface);
		// only copy between render target
		bool StretchRect(IDirect3DSurface9** pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9** pDestSurface,CONST RECT* pDestRect,D3DTEXTUREFILTERTYPE Filter);
		// only copy between render target
		bool CopyResourceBuffer(unsigned int uiLevel,LPDIRECT3DTEXTURE9 * pSource, LPDIRECT3DCUBETEXTURE9 * pDest,CONST RECT* pRect,D3DTEXTUREFILTERTYPE Filter ,D3DCUBEMAP_FACES CubFace);
		
		void CreateIndexBuffer(unsigned int Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer9** ppIndexBuffer,void * pCopyBuffer,DWORD LockFlag);
		void CreateVertexBuffer(UINT Length,DWORD Usage,D3DPOOL Pool,IDirect3DVertexBuffer9** ppVertexBuffer,DWORD Flags,VSVertexBuffer * pVBuffer,unsigned int uiOneVertexSize);
		void CreateVertexDeclaration(const VSArray<D3DVERTEXELEMENT9> & Element, IDirect3DVertexDeclaration9** ppDecl);
		void CreateDepthStencilSurface(D3DFORMAT FormatType ,D3DMULTISAMPLE_TYPE MultiSampleTypes,
			unsigned int uiWidth ,unsigned int uiHeight,IDirect3DSurface9** ppSurface);
		void GetSurfaceLevel(LPDIRECT3DTEXTURE9 * pTexture,unsigned int uiLevel,IDirect3DSurface9** ppSurfaceLevel);
		void GetCubeMapSurface(LPDIRECT3DCUBETEXTURE9 * pCubTexture,D3DCUBEMAP_FACES FaceType,UINT Level,IDirect3DSurface9** ppCubeMapSurface);
		void CreateRenderTarget(D3DFORMAT FormatType,D3DMULTISAMPLE_TYPE MultiSampleType,unsigned int uiWidth, unsigned int uiHeight,IDirect3DSurface9** ppSurface);
		void Create2DTexture(VSTexture * pTexture,
			DWORD                     Usage,
			D3DFORMAT                 Format,
			D3DPOOL                   Pool,
			DWORD						LockFlag,
			IDirect3DBaseTexture9**   ppTexture);
		void Create2DStagTexture(unsigned int uiWidth,
			unsigned int uiHeight,
			D3DFORMAT                 Format,
			IDirect3DTexture9**   ppTexture);
		void Create1DTexture(VSTexture * pTexture,
			DWORD                     Usage,
			D3DFORMAT                 Format,
			D3DPOOL                   Pool,
			DWORD						LockFlag,
			IDirect3DBaseTexture9**   ppTexture);
		void CreateCubeTexture(VSTexture * pTexture,
			DWORD                     Usage,
			D3DFORMAT                 Format,
			D3DPOOL                   Pool,
			DWORD						LockFlag,
			IDirect3DBaseTexture9**   ppTexture);
		void CreateVolumeTexture(VSTexture * pTexture,
			DWORD                     Usage,
			D3DFORMAT                 Format,
			D3DPOOL                   Pool,
			DWORD						LockFlag,
			IDirect3DBaseTexture9**   ppTexture);
		
		
		void CreateVShaderFromString(VSVShader * pVShaderProgram,VSVProgramID *pID);
		void CreateVShaderFromCache(VSVShader * pVShaderProgram,VSVProgramID *pID);

		void CreatePShaderFromString(VSPShader* pPShaderProgram,VSPProgramID *pID);
		void CreatePShaderFromCache(VSPShader* pPShaderProgram,VSPProgramID *pID);

		void GetShaderInstruction(ID3DXBuffer * pShader,unsigned int &uiArithmeticInstructionSlots ,unsigned int &uiTextureInstructionSlots);
		void EnablenInstanceRender(unsigned int uiInstanceNum);
		void DisableInstanceRender();
		bool SetStreamSourceFreq(unsigned int StreamNumber, unsigned int  Setting);

		void OnLoadVShaderFromString(VSVShader * pVShaderProgram, VSVProgramID *pID);
		void OnLoadPShaderFromString(VSPShader* pPShaderProgram, VSPProgramID *pID);
		void CreateQuery(D3DQUERYTYPE Type, VSQueryID * pID);
public:


		virtual void SetPTexAllState(VSTexAllState * pTexAllState,unsigned int i);

		virtual unsigned int SetVBuffer(VSVertexBuffer * pVBuffer, unsigned int uiStream = 0);
		virtual unsigned int SetIBuffer (VSIndexBuffer * pIBuffer);

		virtual unsigned int BeginQuery(VSQuery * pQuery);
		virtual unsigned int EndQuery(VSQuery * pQuery);

		virtual unsigned int SetPTexture (VSTexture* pTexture,unsigned int i);
		virtual unsigned int SetVTexture (VSTexture* pTexture,unsigned int i);
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

		virtual unsigned int SetNormalMesh(VSGeometry *pGeometry);
		virtual unsigned int SetInstanceMesh(VSInstanceGeometry *pInstanceGeometry);

		virtual unsigned int SetPSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet = false);
		virtual unsigned int SetVSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet = false);
		virtual unsigned int SetGSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet = false);
		virtual unsigned int SetDSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet = false);
		virtual unsigned int SetHSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet = false);
		virtual unsigned int SetCSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet = false);

		virtual unsigned int SetBlendState(VSBlendState * pBlendState, bool bForceSet = false);
		virtual unsigned int SetDepthStencilState(VSDepthStencilState * pDepthStencilState, bool bForceSet = false);
		virtual unsigned int SetRasterizerState(VSRasterizerState * pRasterizerState, bool bForceSet = false);
		virtual void SetClipPlane(VSArray<VSPlane3> &Plane,bool bEnable);
		virtual void SetScissorRect(VSArray<VSRect2> &Rect,bool bEnable);

		virtual bool SetRenderTarget(VSRenderTarget *pRenderTarget,unsigned int i);
		virtual bool EndRenderTarget(VSRenderTarget *pRenderTarget,unsigned int i);
		virtual bool SetDepthStencilBuffer(VSDepthStencil * pDepthStencilBuffer);
		virtual bool EndDepthStencilBuffer(VSDepthStencil * pDepthStencilBuffer);
		virtual bool SetViewPort(VSViewPort *pViewPort = NULL);
		virtual bool UseWindow(int uiWindowID);
		
		virtual const VSAdapterInfo* GetAdapterInfo(unsigned int & uiAdapterNum);
	protected:
		
		static VSDx9ShaderInclude * ms_pDx9IncludeShader;
		static LPDIRECT3D9 ms_pMain;
		LPDIRECT3DDEVICE9 m_pDevice;
		LPDIRECT3DSWAPCHAIN9  * m_pChain;	
		LPDIRECT3DSWAPCHAIN9  m_pMainChain;
		D3DPRESENT_PARAMETERS m_Present;

		LPDIRECT3DSURFACE9	m_pMainDepthStencilBuffer;
		LPDIRECT3DSURFACE9*	m_pChainnDepthStencilBuffer;

		static DWORD ms_dwAlphaBlend[];
		static DWORD ms_dwAlphaBlendOP[];
		static DWORD ms_dwCullType[];



		static DWORD ms_dwStencilOperation[];

		static DWORD ms_dwCompare[];

		static DWORD ms_dwTextureFormatType[];
		static DWORD ms_dwTextureFilterType[];
		static DWORD ms_dwTextureAddrType[];

		static DWORD ms_dwPrimitiveType[];
		static DWORD ms_dwDevTypes[]; 
		static DWORD ms_dwMultiSampleTypes[]; 
		static DWORD ms_dwCubeMapFace[];
		static DWORD ms_dwLockFlag[];
		static DWORD ms_dwVertexFormatDataType[];
		static DWORD ms_dwVertexFormatSemantics[];
		static DWORD ms_dwQueryType[];

		static TCHAR ms_cVertexShaderProgramVersion[];
		static TCHAR ms_cPixelShaderProgramVersion[];

		bool m_bInstanceRenderMode;
		enum
		{
			MAX_SHADER_BUFFER = 1024,
		};
		VSREAL	m_fFloatShaderBuffer[MAX_SHADER_BUFFER];
		int		m_iIntShaderBuffer[MAX_SHADER_BUFFER];
		bool	m_bBoolShaderBuffer[MAX_SHADER_BUFFER];

		VSArray<bool> m_bSRGB;
		//PRIORITY
		DECLARE_PRIORITY
		DECLARE_INITIAL_ONLY
		static bool InitialDefaultState();
		static bool TerminalDefaultState();

	public:
		enum	//Register Type
		{
			RT_B,
			RT_C,
			RT_S,
			RT_MAX
		};
		//function in d3d  so output is string
		virtual VSString TexColorBuffer(const VSColorBuffer * pColorBuffer) const;
		virtual VSString Tex2D(const VS2DTexSampler * p2DTexSampler, unsigned int uiShaderType) const;
		virtual VSString Tex1D(const VS1DTexSampler * p1DTexSampler, unsigned int uiShaderType) const;
		virtual VSString Mul(const VSMul * pMul) const;
		virtual VSString Dot(const VSDot * pDot) const;
		virtual VSString Length(const VSLength * pLength) const;
		virtual VSString Sub(const VSSub * pSub) const;
		virtual VSString Add(const VSAdd * pAdd) const;
		virtual VSString Div(const VSDiv * pDiv) const;
		virtual VSString MakeValue(const VSMakeValue * pMakeValue) const;
		virtual VSString Saturate(const VSSaturate * pSaturate) const;
		//
		virtual VSString Sampler(unsigned int uiType)const;
		virtual VSString FloatByChannelNum(unsigned int ChannelNum)const;
		virtual VSString Float()const;
		virtual VSString Float2()const;
		virtual VSString Float3()const;
		virtual VSString Float4()const;
		virtual VSString Return()const;
		virtual VSString FloatConst(const VSString & Value1)const;
		virtual VSString Float2Const(const VSString & Value1,const VSString & Value2)const;
		virtual VSString Float3Const(const VSString & Value1,
			const VSString & Value2,const VSString & Value3)const;
		virtual VSString Float4Const(const VSString & Value1,const VSString & Value2,
			const VSString & Value3,const VSString & Value4)const;

		virtual void CustomPointLightUseString(VSString & PointLightUseString ,VSString & PointAttenuationDivString);
		virtual void CustomSpotLightUseString(VSString & SpotLightUseString ,VSString & SpotAttenuationDivString);

		virtual VSString GetDeclareValue(const VSString &ShowName, unsigned int uiValueType, unsigned int uiRegisterIndex)const;
		virtual VSString GetDeclareSampler(const VSString &ShowName, unsigned int SamplerType, unsigned int uiRegisterIndex, unsigned int SamplerSource)const;
		virtual void GetDeclareShareSampler(unsigned int SamplerSource, unsigned int uiRegisterIndex, VSString & OutString)const;
		virtual VSString SetRegister(unsigned int uiRegisterType,unsigned int uiRegisterIndex)const;


		virtual void CreateVInputDeclare(MaterialShaderPara &MSPara,
										VSString & OutString);
		virtual void CreateVOutputDeclare(MaterialShaderPara &MSPara,
										VSString & OutString);
		virtual void CreateVUserConstant(VSVShader * pVShader,MaterialShaderPara &MSPara,
										VSString & OutString);
		virtual void CreateVFunction(MaterialShaderPara &MSPara,
									VSString & OutString);	
		virtual void SetMaterialVShaderConstant(MaterialShaderPara &MSPara,VSVShader *pVShader);


		virtual void CreatePInputDeclare(MaterialShaderPara &MSPara,
										VSString & OutString);
		virtual void CreatePOutputDeclare(MaterialShaderPara &MSPara,
										VSString & OutString);
		virtual void CreatePUserConstant(VSPShader* pPShader,MaterialShaderPara &MSPara,
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
		virtual void SetMaterialGShaderConstant(MaterialShaderPara &MSPara, VSGShader *pGShader);

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
		/************************************************************************************/
		virtual void GetIncludeShader(VSString &OutString);

		virtual void GetDynamicShader(VSString &OutString);
		virtual void SkyLight(const VSString &WorldNormal,const VSString &UpColor,const VSString & DownColor,const VSString & DiffuseColor,VSString & OutString);
		//fun is custorm output is paramter
		virtual void DirectionalLight(int iLightNum,const VSString &Diffuse,const VSString &Specular ,const VSString &SpecularPow,
			const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,VSString & OutString)const;

		virtual void CustomDirectionalLight(int iLightNum,const VSString &CustomString,VSArray<VSString> ShadowString,VSString & OutString)const;

		virtual void OrenNayarDirectionalLight(int iLightNum,const VSString &Diffuse,const VSString &RoughnessSquared,
					const VSString &WorldNormal,const VSString &WorldCameraDir,bool bLookUpTable,VSArray<VSString> ShadowString,
					VSString & OutString)const;
		

		virtual void PointLight(int iLightNum,const VSString &Diffuse,const VSString &Specular ,const VSString &SpecularPow,
								const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
								VSString & OutString)const;
		virtual void CustomPointLight(int iLightNum,const VSString &CustomString,const VSString & WorldPos,VSArray<VSString> ShadowString,VSString & OutString)const;

		virtual void OrenNayarPointLight(int iLightNum,const VSString &Diffuse,const VSString &RoughnessSquared,
			const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,bool bLookUpTable,VSArray<VSString> ShadowString,
			VSString & OutString)const;


		virtual void SpotLight(int iLightNum,const VSString &Diffuse,const VSString &Specular ,const VSString &SpecularPow,
								const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,VSArray<VSString> ShadowString,
								VSString & OutString)const;
		virtual void CustomSpotLight(int iLightNum,const VSString &CustomString,const VSString & WorldPos,VSArray<VSString> ShadowString,VSString & OutString)const;

		virtual void OrenNayarSpotLight(int iLightNum,const VSString &Diffuse,const VSString &RoughnessSquared,
										const VSString & WorldPos,const VSString &WorldNormal,const VSString &WorldCameraDir,bool bLookUpTable,VSArray<VSString> ShadowString,
										VSString & OutString)const;

		virtual void LocalToWorldPos(const VSString & LocalPos,VSString &OutString)const;
		virtual void WorldToViewPos(const VSString & WorldPos,VSString &OutString)const;
		virtual void LocalToWorldNormal(const VSString & LocalNormal,VSString &OutString)const;
		virtual void WorldToViewNormal(const VSString & WorldNormal,VSString &OutString)const;
		virtual void GetWorldViewDir(VSString &OutString)const;
		virtual void TransProjPos(const VSString & Pos,const VSString & Matrix,VSString &OutString)const;
		virtual void BumpNormal(const VSString &TexNormal,VSString &OutString)const;

		virtual void ComputeDir(const VSString & Orgin,const VSString & End,VSString &OutString)const;
		virtual void ComputeLength(const VSString & Orgin,const VSString & End,VSString &OutString)const;
		virtual void GetLightType(unsigned int i,VSString &OutString)const;
		virtual void GetLightTypeTemp(VSString &OutString)const;
		virtual void ComputeBoneVector(const VSString& BoneIndex, const VSString & BoneWeight,const VSString &BoneVector,
			 const  VSString & U,const VSString & V,const VSString &N,VSString &OutString)const;
		virtual void ComputeBoneVector(const VSString& BoneIndex, const VSString & BoneWeight, 
			const VSString &AnimDataResource, const VSString & AnimAllInfo,
			const VSString &AnimInstanceInfo, const VSString &AnimNum,
			const  VSString & U, const VSString & V, const VSString &N, VSString &OutString)const;

		virtual void BoneTranPos( const VSString & LocalPos,const  VSString & U,const VSString & V,const VSString &N,VSString &OutString)const;
		virtual void BoneTranNormal( const VSString & LocalNormal,const  VSString & U,const VSString & V,const VSString &N,VSString &OutString)const;

		virtual void DecodeNormal1(const VSString & Normal,VSString &OutString) const;
		virtual void RecodeNormal1(const VSString & Normal,VSString &OutString) const;
		virtual void DecodeNormal2(const VSString & Normal,const VSString & Tengent,VSString &OutString) const;
		virtual void DecodeNormal3(const VSString & Normal,VSString &OutString) const;
		virtual void RecodeNormal3(const VSString & Normal,VSString &OutString) const;
		virtual void EncodeReflect(const VSString & Mip , const VSString & Pow , VSString& OutString);
		virtual void DecodeReflect(const VSString & Mip , const VSString & Pow , const VSString & Value,VSString& OutString);

		virtual void Reciprocal(const VSString & Value, VSString &OutString)const;
		virtual void Clip(const VSString & ClipValue,VSString &OutString)const;
		virtual void GreaterZeroPow(const VSString & Value,VSREAL Exp,VSString &OutString) const;
		virtual void SaturatePow(const VSString & Value,VSREAL Exp,VSString &OutString) const;
		virtual void Saturate(const VSString & Value,VSString &OutString)const;
		virtual void PointLightCubeShadow(const VSString & PointLightName , const VSString & CubeShadowSampler , const VSString & ViewMatrix,
			const VSString & WorldPos, VSString &OutString)const;

		virtual void PointLightCubeShadow(const VSString & PointLightName , const VSString & CubeShadowSampler ,
			const VSString & WorldPos, VSString &OutString)const;

		virtual void PointLightVolumeShadow(const VSString & PointLightName , const VSString & VolumeShadowSampler , 
			const VSString & WorldPos, VSString& OutString)const;

		virtual void PointLightDualParaboloidShadow(const VSString & PointLightName , const VSString & ShadowSampler , 
			const VSString & WorldPos, VSString& OutString)const;
		virtual void DirectionLightVolumeShadow(const VSString & DirLightName , const VSString & VolumeShadowSampler , 
			const VSString & WorldPos, VSString& OutString)const;
		virtual void DirectionLightCSMShadow(const VSString & DirLightName , const VSString & ShadowSampler , 
			const VSString & WorldPos, const VSString & ViewPos,const VSString &FarZ,VSString& OutString)const;
		virtual void DirectionLightShadow(const VSString & DirLightName , const VSString & ShadowSampler , 
			const VSString & WorldPos, VSString& OutString)const;
		virtual void SpotLightShadow(const VSString & SpotLightName , const VSString & ShadowSampler , 
			const VSString & WorldPos, VSString& OutString)const;


		virtual void TranLightToTemp(VSArray<VSLight *> LightArray,VSString & OutString)const;
		virtual void GetLightFunction(VSArray<VSLight *> LightArray,const VSString & WorldPos,VSString & OutString)const;
	private:
		
		// create user constant
		void CreateUserConstantLightShadowMatrix(VSShader * pShader,unsigned int& ID,
			VSString & OutString);

		void CreateUserConstantProjectMatrix(VSShader * pShader, unsigned int& ID,
			VSString & OutString);

		void CreateUserConstantViewProjectMatrix(VSShader * pShader, unsigned int& ID,
			VSString & OutString);

		void CreateUserConstantViewMatrix(VSShader * pShader,unsigned int& ID,
			VSString & OutString);
		void CreateUserConstantLightWorldPos(VSShader * pShader,unsigned int& ID,
			VSString & OutString);

		void CreateUserConstantLightDirection(VSShader * pShader,unsigned int& ID,
			VSString & OutString);

		void CreateUserConstantFarZ(VSShader * pShader,unsigned int& uiRegisterID,
			VSString & OutString);
		void CreateUserConstantSkyLightUpColor(VSShader * pShader,unsigned int& uiRegisterID,
			VSString & OutString);
		void CreateUserConstantSkyLightDownColor(VSShader * pShader,unsigned int& uiRegisterID,
			VSString & OutString);

		void CreateUserConstantLight(VSShader * pShader,MaterialShaderPara &MSPara,unsigned int& uiRegisterID,
			VSString & OutString);
		void CreateUserConstantPointLightRange(VSShader * pShader,unsigned int& uiRegisterID,
			VSString & OutString);
		void CreateUserConstantSkin(VSGeometry * pGeometry,VSShader * pShader,unsigned int& ID,
			VSString & OutString);
		void CreateUserConstantSimpleSkinInstanceTexture(VSGeometry * pGeometry, VSShader * pShader, unsigned int& uiTexRegisterID,
			VSString & OutString);

		void CreateUserConstantMorph(VSGeometry * pGeometry, VSShader * pShader, unsigned int& ID,
			VSString & OutString);
		void CreateUserConstantMorphTexture(VSGeometry * pGeometry, VSShader * pShader, unsigned int& uiTexRegisterID,
			VSString & OutString);

		void CreateUserConstantWorldMatrix(VSShader * pShader,unsigned int& ID,
			VSString & OutString);
		void CreateUserConstantCameraWorldPos(VSShader * pShader,unsigned int& ID,
			VSString & OutString);
		void CreateUserConstantDirectionLight(VSShader * pShader,unsigned int& ID,unsigned int uiLightNum,
			VSString & OutString);
		void CreateUserConstantPointLight(VSShader * pShader,unsigned int& ID,unsigned int uiLightNum,
			VSString & OutString);
		void CreateUserConstantSpotLight(VSShader * pShader,unsigned int& ID,unsigned int uiLightNum,
			VSString & OutString);

		void CreateUserConstantInvRTWidth(VSShader * pShader,unsigned int& uiRegisterID,
			VSString & OutString);

		void CreateUserConstantShadow(VSPShader* pPShader,VSArray<VSLight*> & LightArray,unsigned int &ID,VSString & OutString);
		void CreateUserConstantLightFunction(VSPShader* pPShader,VSArray<VSLight*> & LightArray,unsigned int &ID,VSString & OutString);
		void CreateLightModleLookUpTable(VSPShader* pPShader,MaterialShaderPara &MSPara, unsigned int &uiTexRegisterID,VSString & OutString);

		//set user constant
		void SetUserConstantLightWorldPos(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID);
		void SetUserConstantLightWorldDirection(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID);
		void SetUserLightShadowMatrix(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID);

		void SetUserConstantMorph(MaterialShaderPara &MSPara, VSShader * pShader, unsigned int& ID);
		void SetUserConstantMorphTexture(MaterialShaderPara &MSPara, VSShader * pShader, unsigned int& ID);
		void SetUserConstantSkin(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID);
		void SetUserConstantSimpleAnimInstanceTexture(MaterialShaderPara &MSPara, VSShader * pShader, unsigned int& ID);
		void SetUserConstantWorldMatrix(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID);
		void SetUserConstantViewProjectMatrix(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID);
		void SetUserConstantViewMatrix(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID);
		void SetUserConstantProjectMatrix(MaterialShaderPara &MSPara, VSShader * pShader, unsigned int& ID);
		void SetUserConstantCameraPos(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID);
		void SetUserConstantSkyUpColor(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID);
		void SetUserConstantSkyDownColor(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID);
		void SetUserConstantLight(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID);
		void SetUserConstantShadowSampler(MaterialShaderPara &MSPara,VSShader * pShader);
		void SetUserConstantLightFunctionSampler(MaterialShaderPara &MSPara,VSShader * pShader);
		void SetUserConstantFarZ(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID);
		void SetUserConstantPointLightRange(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID);
		void SetUserConstantInvRTWidth(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID);

		//create vsshader input
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
		void CreateVInputDeclareInstancePosition(MaterialShaderPara &MSPara,
			VSString & OutString);
		void CreateVInputDeclareVertexID(MaterialShaderPara &MSPara,
			VSString & OutString);
		
		// create vsshader function
		void CreateVFunctionDualParaboloidPosition(MaterialShaderPara &MSPara,VSString & FunctionBody);
		void CreateVFunctionVolumeShadowPosition(MaterialShaderPara &MSPara,VSString & FunctionBody,unsigned int uiPass);

		void CreateVFunctionMorphData(MaterialShaderPara &MSPara, VSString & FunctionBody);
		void CreateVFunctionSkinInfo(MaterialShaderPara &MSPara, VSString & FunctionBody);
		void CreateVFunctionNormal(MaterialShaderPara &MSPara, VSString & FunctionBody);
		void CreateVFunctionVolumeShadowNormal(MaterialShaderPara &MSPara, VSString & FunctionBody);
		void CreateVFunctionPosition(MaterialShaderPara &MSPara,VSString & FunctionBody);	
		void CreateVFunctionColor(MaterialShaderPara &MSPara,VSString & FunctionBody);
		void CreateVFunctionTexCoord(MaterialShaderPara &MSPara,VSString & FunctionBody);
		void CreateVFunctionProjectZ(MaterialShaderPara &MSPara,VSString & FunctionBody);
		void CreateVFunctionPost(MaterialShaderPara &MSPara,VSString & FunctionBody);


		void CreateDeclarePosition(MaterialShaderPara &MSPara,
			VSString & OutString, bool bInput);
		void CreateDeclareTexCoord(MaterialShaderPara &MSPara, unsigned int& ID,
			VSString & OutString, bool bInput);
		void CreateDeclareNormal(MaterialShaderPara &MSPara, unsigned int& ID,
			VSString & OutString, bool bInput);
		void CreateDeclareColor(MaterialShaderPara &MSPara,
			VSString & OutString, bool bInput);

		void CreateDeclareInstancePos(MaterialShaderPara &MSPara, unsigned int& ID,
			VSString & OutString, bool bInput);

		void CreateDeclareWorldPos(MaterialShaderPara &MSPara, unsigned int& ID,
			VSString & OutString, bool bInput);

		void CreateDeclareProjectZ(MaterialShaderPara &MSPara, unsigned int& ID,
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
