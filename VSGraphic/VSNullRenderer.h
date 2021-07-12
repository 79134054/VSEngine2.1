#pragma once
#include "VSRenderer.h"
namespace VSEngine2
{
	class VSGRAPHIC_API VSNullRenderer : public VSRenderer
	{
	public:
		VSNullRenderer();
		virtual ~VSNullRenderer();
		virtual int GetRendererType() const{ return RAT_NULL; }
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

		virtual unsigned int SetVBuffer(VSVertexBuffer * pVBuffer,unsigned int uiStream = 0);
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

		virtual unsigned int SetPSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet = false);
		virtual unsigned int SetVSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet = false);
		virtual unsigned int SetGSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet = false);
		virtual unsigned int SetDSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet = false);
		virtual unsigned int SetHSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet = false);
		virtual unsigned int SetCSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet = false);
		
		virtual bool SetDepthStencilBuffer(VSDepthStencil * pDepthStencilBuffer);
		virtual bool EndDepthStencilBuffer(VSDepthStencil * pDepthStencilBuffer);
		virtual unsigned int GetTextureCompressLen(unsigned int uiLen);
		virtual const VSAdapterInfo* GetAdapterInfo(unsigned int & uiAdapterNum);
	protected:

		virtual void SetVShaderConstant(VSVShader * pShader);
		virtual void SetVShaderBindResource(VSVShader * pShader);
		virtual void SetPShaderConstant(VSPShader * pShader);
		virtual void SetPShaderBindResource(VSPShader * pShader);
		virtual void SetGShaderConstant(VSGShader * pShader);
		virtual void SetGShaderBindResource(VSGShader * pShader);
		virtual void SetDShaderConstant(VSDShader * pShader);
		virtual void SetDShaderBindResource(VSDShader * pShader);
		virtual void SetHShaderConstant(VSHShader * pShader);
		virtual void SetHShaderBindResource(VSHShader * pShader);
		virtual void SetCShaderConstant(VSCShader * pShader);
		virtual void SetCShaderBindResource(VSCShader * pShader);


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

		virtual void * Lock(VSUnorderAccess  * pUnorderAccess);
		virtual void UnLock(VSUnorderAccess  * pUnorderAccess);

		virtual void * Lock(VSDepthStencil * pDepthStencil);
		virtual void UnLock(VSDepthStencil * pDepthStencil);

		virtual bool GetData(VSQuery * pQuery, void * pData, unsigned int uiSize);

		virtual bool CopyResourceBuffer(VS2DTexture * pSource, VSCubeTexture * pDest, unsigned int uiFace);

	public:
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

		virtual void SetMaterialPShaderConstant(MaterialShaderPara &MSPara, VSPShader *pPShader);

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

	};
	
}

