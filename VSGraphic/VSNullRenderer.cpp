#include "VSNullRenderer.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
VSNullRenderer::VSNullRenderer()
{
	
}
VSNullRenderer::~VSNullRenderer()
{
	ReleaseDefaultValue();
}
void VSNullRenderer::ClearBuffers(unsigned int uiClearFlag){VSMAC_ASSERT(0);return ;}
void VSNullRenderer::ClearBackBuffer(){VSMAC_ASSERT(0);return ;}
void VSNullRenderer::ClearZBuffer(){VSMAC_ASSERT(0);return ;}
void VSNullRenderer::ClearStencilBuffer(){VSMAC_ASSERT(0);return ;}
void VSNullRenderer::ClearBuffers(){VSMAC_ASSERT(0);return ;}
void VSNullRenderer::ClearBuffers(unsigned int uiClearFlag, int iXPos, int iYPos, int iWidth,
	int iHeight){
	VSMAC_ASSERT(0); return;
}
void VSNullRenderer::ClearBackBuffer(int iXPos, int iYPos, int iWidth,
	int iHeight){
	VSMAC_ASSERT(0); return;
}
void VSNullRenderer::ClearZBuffer(int iXPos, int iYPos, int iWidth,
	int iHeight){
	VSMAC_ASSERT(0); return;
}
void VSNullRenderer::ClearStencilBuffer(int iXPos, int iYPos, int iWidth,
	int iHeight){
	VSMAC_ASSERT(0); return;
}
void VSNullRenderer::ClearBuffers(int iXPos, int iYPos, int iWidth,
	int iHeight){
	VSMAC_ASSERT(0); return;
}
bool VSNullRenderer::SetViewPort(VSViewPort *pViewPort){VSMAC_ASSERT(0);return 0;}
bool VSNullRenderer::UseWindow(int uiWindowID){VSMAC_ASSERT(0);return 0;}

bool VSNullRenderer::CooperativeLevel(){VSMAC_ASSERT(0);return 0;}
bool VSNullRenderer::BeginRendering(){VSMAC_ASSERT(0);return 0;}
bool VSNullRenderer::EndRendering(){VSMAC_ASSERT(0);return 0;}

void VSNullRenderer::DeviceLost(){VSMAC_ASSERT(0);return ;}
void VSNullRenderer::ResetDevice(){VSMAC_ASSERT(0);return ;}
bool VSNullRenderer::ChangeScreenSize(unsigned int uiWidth, unsigned int uiHeight, bool bWindow){VSMAC_ASSERT(0);return 0 ;}
bool VSNullRenderer::DrawMesh(VSGeometry * pGeometry, VSRenderState * pRenderState, VSVShader * pVShader, VSPShader * pPShader, 
	VSGShader * pGShader, VSHShader * pHShader, VSDShader * pDShader){
	VSMAC_ASSERT(0); return 0;
}
bool VSNullRenderer::DrawScreen(SCREEN_QUAD_TYPE ScreenQuad[4]){VSMAC_ASSERT(0);return 0;}
bool VSNullRenderer::DrawScreen(SCREEN_QUAD_TYPE * pScreenBuffer, unsigned int uiVertexNum,
	VSUSHORT_INDEX * pIndexBuffer, unsigned int uiIndexNum){VSMAC_ASSERT(0);return 0;}
bool VSNullRenderer::DrawScreenFont(SCREEN_FONT_TYPE * pScreenFontBuffer, unsigned int uiVertexNum,
	VSUSHORT_INDEX * pIndexBuffer, unsigned int uiIndexNum){
	VSMAC_ASSERT(0); return 0;
}
const VSAdapterInfo* VSNullRenderer::GetAdapterInfo(unsigned int & uiAdapterNum)
{
	VSMAC_ASSERT(0); return NULL;
}
unsigned int VSNullRenderer::SetBlendState(VSBlendState * pBlendState, bool bForceSet){ VSMAC_ASSERT(0); return 0; }
unsigned int VSNullRenderer::SetDepthStencilState(VSDepthStencilState * pDepthStencilState, bool bForceSet){ VSMAC_ASSERT(0); return 0; }
unsigned int VSNullRenderer::SetRasterizerState(VSRasterizerState * pRasterizerState, bool bForceSet){ VSMAC_ASSERT(0); return 0; }
void VSNullRenderer::SetClipPlane(VSArray<VSPlane3> &Plane, bool bEnable){VSMAC_ASSERT(0);return ;}
void VSNullRenderer::SetScissorRect(VSArray<VSRect2> &Rect, bool bEnable){VSMAC_ASSERT(0);return ;}

unsigned int VSNullRenderer::SetVBuffer(VSVertexBuffer * pVBuffer, unsigned int uiStream){ VSMAC_ASSERT(0); return 0; }
unsigned int VSNullRenderer::SetIBuffer(VSIndexBuffer* pIBuffer){VSMAC_ASSERT(0);return 0;}
unsigned int VSNullRenderer::BeginQuery(VSQuery * pQuery) { VSMAC_ASSERT(0); return 0; }
unsigned int VSNullRenderer::EndQuery(VSQuery * pQuery) { VSMAC_ASSERT(0); return 0; }
unsigned int VSNullRenderer::SetPTexture(VSTexture* pTexture, unsigned int i){VSMAC_ASSERT(0);return 0;}
unsigned int VSNullRenderer::SetVTexture(VSTexture* pTexture, unsigned int i){VSMAC_ASSERT(0);return 0;}
unsigned int VSNullRenderer::SetGTexture(VSTexture* pTexture, unsigned int i){ VSMAC_ASSERT(0); return 0; }
unsigned int VSNullRenderer::SetDTexture(VSTexture* pTexture, unsigned int i){ VSMAC_ASSERT(0); return 0; }
unsigned int VSNullRenderer::SetHTexture(VSTexture* pTexture, unsigned int i){ VSMAC_ASSERT(0); return 0; }
unsigned int VSNullRenderer::SetCTexture(VSTexture* pTexture, unsigned int i) { VSMAC_ASSERT(0); return 0; }
unsigned int VSNullRenderer::SetCBufferResource(VSBufferResource* pBufferResource, unsigned int i) { VSMAC_ASSERT(0); return 0; }
unsigned int VSNullRenderer::SetVBufferResource(VSBufferResource* pBufferResource, unsigned int i) { VSMAC_ASSERT(0); return 0; }
unsigned int VSNullRenderer::SetPBufferResource(VSBufferResource* pBufferResource, unsigned int i) { VSMAC_ASSERT(0); return 0; }
unsigned int VSNullRenderer::SetGBufferResource(VSBufferResource* pBufferResource, unsigned int i) { VSMAC_ASSERT(0); return 0; }
unsigned int VSNullRenderer::SetHBufferResource(VSBufferResource* pBufferResource, unsigned int i) { VSMAC_ASSERT(0); return 0; }
unsigned int VSNullRenderer::SetDBufferResource(VSBufferResource* pBufferResource, unsigned int i) { VSMAC_ASSERT(0); return 0; }
void VSNullRenderer::Dispath(unsigned int uiThreadGroupCountX, unsigned int uiThreadGroupCountY,
	unsigned int uiThreadGroupCountZ) 
{
	VSMAC_ASSERT(0); return;
}
unsigned int VSNullRenderer::SetPSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet){ VSMAC_ASSERT(0); return 0; }
unsigned int VSNullRenderer::SetVSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet){ VSMAC_ASSERT(0); return 0; }
unsigned int VSNullRenderer::SetGSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet){ VSMAC_ASSERT(0); return 0; }
unsigned int VSNullRenderer::SetDSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet){ VSMAC_ASSERT(0); return 0; }
unsigned int VSNullRenderer::SetHSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet){ VSMAC_ASSERT(0); return 0; }
unsigned int VSNullRenderer::SetCSamplerState(VSSamplerState * pSamplerState, unsigned int i, bool bForceSet) { VSMAC_ASSERT(0); return 0; }

bool VSNullRenderer::SetDepthStencilBuffer(VSDepthStencil * pDepthStencilBuffer){VSMAC_ASSERT(0);return 0;}
bool VSNullRenderer::EndDepthStencilBuffer(VSDepthStencil * pDepthStencilBuffer){VSMAC_ASSERT(0);return 0;}
void VSNullRenderer::SetVShaderConstant(VSVShader * pShader){ VSMAC_ASSERT(0); return; }
void VSNullRenderer::SetVShaderBindResource(VSVShader * pShader){ VSMAC_ASSERT(0); return; }
void VSNullRenderer::SetPShaderConstant(VSPShader * pShader){ VSMAC_ASSERT(0); return; }
void VSNullRenderer::SetPShaderBindResource(VSPShader * pShader){ VSMAC_ASSERT(0); return; }
void VSNullRenderer::SetGShaderConstant(VSGShader * pShader){ VSMAC_ASSERT(0); return; }
void VSNullRenderer::SetGShaderBindResource(VSGShader * pShader){ VSMAC_ASSERT(0); return; }
void VSNullRenderer::SetDShaderConstant(VSDShader * pShader){ VSMAC_ASSERT(0); return; }
void VSNullRenderer::SetDShaderBindResource(VSDShader * pShader){ VSMAC_ASSERT(0); return; }
void VSNullRenderer::SetHShaderConstant(VSHShader * pShader){ VSMAC_ASSERT(0); return; }
void VSNullRenderer::SetHShaderBindResource(VSHShader * pShader){ VSMAC_ASSERT(0); return; }
void VSNullRenderer::SetCShaderConstant(VSCShader * pShader) { VSMAC_ASSERT(0); return; }
void VSNullRenderer::SetCShaderBindResource(VSCShader * pShader) { VSMAC_ASSERT(0); return; }


bool VSNullRenderer::SetVShaderProgram(VSVShader*	pVShaderProgram){VSMAC_ASSERT(0);return 0;}
bool VSNullRenderer::SetPShaderProgram(VSPShader*pPShaderProgram){VSMAC_ASSERT(0);return 0;}
bool VSNullRenderer::SetGShaderProgram(VSGShader*pGShaderProgram){ VSMAC_ASSERT(0); return 0; }
bool VSNullRenderer::SetDShaderProgram(VSDShader*	pDShaderProgram){ VSMAC_ASSERT(0); return 0; }
bool VSNullRenderer::SetHShaderProgram(VSHShader*	pHShaderProgram){ VSMAC_ASSERT(0); return 0; }
bool VSNullRenderer::SetCShaderProgram(VSCShader*	pCShaderProgram) { VSMAC_ASSERT(0); return 0; }
//Load Release
bool VSNullRenderer::OnLoadVShaderProgram(VSVShader * pVShaderProgram, VSResourceIdentifier *&pID){VSMAC_ASSERT(0);return 0;}

bool VSNullRenderer::OnLoadPShaderProgram(VSPShader* pPShaderProgram, VSResourceIdentifier *&pID){VSMAC_ASSERT(0);return 0;}

bool VSNullRenderer::OnLoadGShaderProgram(VSGShader* pGShaderProgram, VSResourceIdentifier *&pID){ VSMAC_ASSERT(0); return 0; }

bool VSNullRenderer::OnLoadDShaderProgram(VSDShader * pDShaderProgram, VSResourceIdentifier *&pID){ VSMAC_ASSERT(0); return 0; }

bool VSNullRenderer::OnLoadHShaderProgram(VSHShader * pHShaderProgram, VSResourceIdentifier *&pID){ VSMAC_ASSERT(0); return 0; }

bool VSNullRenderer::OnLoadCShaderProgram(VSCShader * pCShaderProgram, VSResourceIdentifier *&pID) { VSMAC_ASSERT(0); return 0; }

bool VSNullRenderer::OnLoadTexture(VSTexture * pTexture, VSResourceIdentifier *&pID){VSMAC_ASSERT(0);return 0;}

bool VSNullRenderer::OnLoadDepthStencil(VSDepthStencil * pDepthStencil, VSResourceIdentifier *&pID){VSMAC_ASSERT(0);return 0;}

bool VSNullRenderer::OnLoadRenderTarget(VSRenderTarget * pRenderTarget, VSResourceIdentifier *&pID){VSMAC_ASSERT(0);return 0;}

bool VSNullRenderer::OnLoadUnorderAccess(VSBufferUnorderAccess  * pUnorderAccess, VSResourceIdentifier *&pID) { VSMAC_ASSERT(0); return 0; }
bool VSNullRenderer::OnLoadUnorderAccess(VSTextureUnorderAccess  * pUnorderAccess, VSResourceIdentifier *&pID) { VSMAC_ASSERT(0); return 0; }

bool VSNullRenderer::OnLoadVBufferData(VSVertexBuffer * pVBuffer, VSResourceIdentifier *&pID){VSMAC_ASSERT(0);return 0;}

bool VSNullRenderer::OnLoadVBufferFormat(VSVertexFormat * pVertexFormat, VSResourceIdentifier *&pID){VSMAC_ASSERT(0);return 0;}

bool VSNullRenderer::OnLoadIBuffer(VSIndexBuffer* pIBuffer, VSResourceIdentifier *&pID){VSMAC_ASSERT(0);return 0;}

bool VSNullRenderer::OnLoadSBuffer(VSBufferResource* pSBuffer, VSResourceIdentifier *&pID) { VSMAC_ASSERT(0); return 0; }

bool VSNullRenderer::OnLoadBlendState(VSBlendState * pBlendState, VSResourceIdentifier *&pID){VSMAC_ASSERT(0);return 0;}

bool VSNullRenderer::OnLoadDepthStencilState(VSDepthStencilState * pDepthStencilState, VSResourceIdentifier *&pID){VSMAC_ASSERT(0);return 0;}

bool VSNullRenderer::OnLoadRasterizerState(VSRasterizerState * pRasterizerState, VSResourceIdentifier *&pID){VSMAC_ASSERT(0);return 0;}

bool VSNullRenderer::OnLoadSamplerState(VSSamplerState * pSamplerState, VSResourceIdentifier *&pID){VSMAC_ASSERT(0);return 0;}

bool VSNullRenderer::OnLoadQuery(VSQuery * pQuery, VSResourceIdentifier *&pID) { VSMAC_ASSERT(0); return 0; }

unsigned int VSNullRenderer::SetVertexFormat(VSVertexFormat * pVertexFormat){VSMAC_ASSERT(0);return 0;}

void * VSNullRenderer::Lock(VSVertexBuffer * pVertexBuffer){VSMAC_ASSERT(0);return NULL;}
void VSNullRenderer::UnLock(VSVertexBuffer * pVertexBuffer){VSMAC_ASSERT(0);return ;}

void * VSNullRenderer::Lock(VSIndexBuffer * pIndexBuffer){ VSMAC_ASSERT(0); return NULL; }
void VSNullRenderer::UnLock(VSIndexBuffer * pIndexBuffer){VSMAC_ASSERT(0);return ;}

void * VSNullRenderer::Lock(VSBufferResource * pStructBuffer) { VSMAC_ASSERT(0); return NULL; }
void VSNullRenderer::UnLock(VSBufferResource * pStructBuffer) { VSMAC_ASSERT(0); return; }

void * VSNullRenderer::Lock(VSTexture * pTexture, unsigned int uiLevel, unsigned int uiFace){ VSMAC_ASSERT(0); return NULL; }
void VSNullRenderer::UnLock(VSTexture * pTexture, unsigned int uiLevel, unsigned int uiFace){VSMAC_ASSERT(0);return ;}

bool VSNullRenderer::CopyResourceBuffer(VS2DTexture * pSource, VSCubeTexture * pDest, unsigned int uiFace){VSMAC_ASSERT(0);return 0;}

bool VSNullRenderer::GetData(VSQuery * pQuery, void * pData, unsigned int uiSize) { VSMAC_ASSERT(0); return NULL; };

void * VSNullRenderer::Lock(VSRenderTarget * pRenderTarget) { VSMAC_ASSERT(0); return NULL; };
void VSNullRenderer::UnLock(VSRenderTarget * pRenderTarget) { VSMAC_ASSERT(0); return; };

void * VSNullRenderer::Lock(VSUnorderAccess  * pUnorderAccess) { VSMAC_ASSERT(0); return NULL; };
void VSNullRenderer::UnLock(VSUnorderAccess  * pUnorderAccess) { VSMAC_ASSERT(0); return; };

void * VSNullRenderer::Lock(VSDepthStencil * pDepthStencil) { VSMAC_ASSERT(0); return NULL; };
void VSNullRenderer::UnLock(VSDepthStencil * pDepthStencil) { VSMAC_ASSERT(0); return; };

VSString VSNullRenderer::Tex2D(const VS2DTexSampler * p2DTexSampler, unsigned int uiShaderType) const{ VSMAC_ASSERT(0); return VSString::ms_StringNULL; }
VSString VSNullRenderer::Tex1D(const VS1DTexSampler * p1DTexSampler, unsigned int uiShaderType) const{ VSMAC_ASSERT(0); return VSString::ms_StringNULL; }
VSString VSNullRenderer::TexColorBuffer(const VSColorBuffer * pColorBuffer) const{ VSMAC_ASSERT(0); return VSString::ms_StringNULL; }
VSString VSNullRenderer::Mul(const VSMul * pMul) const{ VSMAC_ASSERT(0); return VSString::ms_StringNULL; }
VSString VSNullRenderer::Saturate(const VSSaturate * pSaturate) const{ VSMAC_ASSERT(0); return VSString::ms_StringNULL; }
VSString VSNullRenderer::Div(const VSDiv * pDiv) const{ VSMAC_ASSERT(0); return VSString::ms_StringNULL; }
VSString VSNullRenderer::Dot(const VSDot * pDot) const{ VSMAC_ASSERT(0); return VSString::ms_StringNULL; }
VSString VSNullRenderer::Length(const VSLength * pLength) const { VSMAC_ASSERT(0); return VSString::ms_StringNULL; }
VSString VSNullRenderer::Sub(const VSSub * pSub) const{ VSMAC_ASSERT(0); return VSString::ms_StringNULL; }
VSString VSNullRenderer::Add(const VSAdd * pAdd) const{ VSMAC_ASSERT(0); return VSString::ms_StringNULL; }
VSString VSNullRenderer::MakeValue(const VSMakeValue * pMakeValue)const{ VSMAC_ASSERT(0); return VSString::ms_StringNULL; }

VSString VSNullRenderer::FloatByChannelNum(unsigned int ChannelNum)const{ VSMAC_ASSERT(0); return VSString::ms_StringNULL; }
VSString VSNullRenderer::Float()const{ VSMAC_ASSERT(0); return VSString::ms_StringNULL; }
VSString VSNullRenderer::Float2()const{ VSMAC_ASSERT(0); return VSString::ms_StringNULL; }
VSString VSNullRenderer::Float3()const{ VSMAC_ASSERT(0); return VSString::ms_StringNULL; }
VSString VSNullRenderer::Float4()const{ VSMAC_ASSERT(0); return VSString::ms_StringNULL; }
VSString VSNullRenderer::Return()const{ VSMAC_ASSERT(0); return VSString::ms_StringNULL; }

VSString VSNullRenderer::GetDeclareValue(const VSString &ShowName, unsigned int uiValueType, unsigned int uiRegisterIndex)const{ VSMAC_ASSERT(0); return VSString::ms_StringNULL; }
VSString VSNullRenderer::GetDeclareSampler(const VSString &ShowName, unsigned int SamplerType, unsigned int uiRegisterIndex, unsigned int SamplerSource)const{ VSMAC_ASSERT(0); return VSString::ms_StringNULL; }
void VSNullRenderer::GetDeclareShareSampler(unsigned int SamplerSource, unsigned int uiRegisterIndex, VSString & OutString)const
{
	VSMAC_ASSERT(0);
	return;
}
VSString VSNullRenderer::FloatConst(const VSString & Value1)const{ VSMAC_ASSERT(0); return VSString::ms_StringNULL; }
VSString VSNullRenderer::Float2Const(const VSString & Value1, const VSString & Value2)const{ VSMAC_ASSERT(0); return VSString::ms_StringNULL; }
VSString VSNullRenderer::Float3Const(const VSString & Value1,
	const VSString & Value2, const VSString & Value3)const{
	VSMAC_ASSERT(0); return VSString::ms_StringNULL;
}
VSString VSNullRenderer::Float4Const(const VSString & Value1, const VSString & Value2,
	const VSString & Value3, const VSString & Value4)const{
	VSMAC_ASSERT(0); return VSString::ms_StringNULL;
}

void VSNullRenderer::CustomPointLightUseString(VSString & PointLightUseString, VSString & PointAttenuationDivString){VSMAC_ASSERT(0);return ;}

void VSNullRenderer::CustomSpotLightUseString(VSString & SpotLightUseString, VSString & SpotAttenuationDivString){VSMAC_ASSERT(0);return ;}




//shader custom function
void VSNullRenderer::CreateVInputDeclare(MaterialShaderPara &MSPara,
	VSString & OutString){VSMAC_ASSERT(0);return ;}
void VSNullRenderer::CreateVOutputDeclare(MaterialShaderPara &MSPara,
	VSString & OutString){VSMAC_ASSERT(0);return ;}
void VSNullRenderer::CreateVUserConstant(VSVShader * pVShader, MaterialShaderPara &MSPara,
	VSString & OutString){VSMAC_ASSERT(0);return ;}
void VSNullRenderer::CreateVFunction(MaterialShaderPara &MSPara,
	VSString & OutString){VSMAC_ASSERT(0);return ;}

void VSNullRenderer::SetMaterialVShaderConstant(MaterialShaderPara &MSPara,VSVShader *pVShader){VSMAC_ASSERT(0);return ;}

void VSNullRenderer::CreatePInputDeclare(MaterialShaderPara &MSPara,
	VSString & OutString){VSMAC_ASSERT(0);return ;}
void VSNullRenderer::CreatePOutputDeclare(MaterialShaderPara &MSPara,
	VSString & OutString){VSMAC_ASSERT(0);return ;}
void VSNullRenderer::CreatePUserConstant(VSPShader * pPShader, MaterialShaderPara &MSPara,
	VSString & OutString){VSMAC_ASSERT(0);return ;}
void VSNullRenderer::CreatePFunction(MaterialShaderPara &MSPara,
	VSString & OutString){VSMAC_ASSERT(0);return ;}

void VSNullRenderer::SetMaterialPShaderConstant(MaterialShaderPara &MSPara,VSPShader *pPShader){VSMAC_ASSERT(0);return ;}

void VSNullRenderer::CreateGInputDeclare(MaterialShaderPara &MSPara,
	VSString & OutString){VSMAC_ASSERT(0);return ;}
void VSNullRenderer::CreateGOutputDeclare(MaterialShaderPara &MSPara,
	VSString & OutString){VSMAC_ASSERT(0);return ;}
void VSNullRenderer::CreateGUserConstant(VSGShader * pGShader, MaterialShaderPara &MSPara,
	VSString & OutString){VSMAC_ASSERT(0);return ;}
void VSNullRenderer::CreateGFunction(MaterialShaderPara &MSPara,
	VSString & OutString){VSMAC_ASSERT(0);return ;}
void VSNullRenderer::SetMaterialGShaderConstant(MaterialShaderPara &MSPara, VSGShader *pGShader){ VSMAC_ASSERT(0); return; }

void VSNullRenderer::CreateHInputDeclare(MaterialShaderPara &MSPara,
	VSString & OutString) {VSMAC_ASSERT(0); return;}
void VSNullRenderer::CreateHOutputDeclare(MaterialShaderPara &MSPara,
	VSString & OutString){VSMAC_ASSERT(0);return ;}
void VSNullRenderer::CreateHUserConstant(VSHShader * pHShader, MaterialShaderPara &MSPara,
	VSString & OutString){VSMAC_ASSERT(0);return ;}
void VSNullRenderer::CreateHFunction(MaterialShaderPara &MSPara,
	VSString & OutString){VSMAC_ASSERT(0);return ;}
void VSNullRenderer::SetMaterialHShaderConstant(MaterialShaderPara &MSPara, VSHShader * pHShader) { VSMAC_ASSERT(0); return; }

void VSNullRenderer::CreateDInputDeclare(MaterialShaderPara &MSPara,
	VSString & OutString) {
	VSMAC_ASSERT(0); return;
}
void VSNullRenderer::CreateDOutputDeclare(MaterialShaderPara &MSPara,
	VSString & OutString) {
	VSMAC_ASSERT(0); return;
}
void VSNullRenderer::CreateDUserConstant(VSDShader * pDShader, MaterialShaderPara &MSPara,
	VSString & OutString) {
	VSMAC_ASSERT(0); return;
}
void VSNullRenderer::CreateDFunction(MaterialShaderPara &MSPara,
	VSString & OutString) {
	VSMAC_ASSERT(0); return;
}
void VSNullRenderer::SetMaterialDShaderConstant(MaterialShaderPara &MSPara, VSDShader *pDShader) { VSMAC_ASSERT(0); return; }

void VSNullRenderer::GetIncludeShader(VSString &OutString){VSMAC_ASSERT(0);return ;}

void VSNullRenderer::GetDynamicShader(VSString &OutString){VSMAC_ASSERT(0);return ;}


void VSNullRenderer::SkyLight(const VSString &WorldNormal, const VSString &UpColor, const VSString & DownColor, const VSString & DiffuseColor, VSString & OutString){VSMAC_ASSERT(0);return ;}
void VSNullRenderer::DirectionalLight(int iLightNum, const VSString &Diffuse, const VSString &Specular, const VSString &SpecularPow,
	const VSString &WorldNormal, const VSString &WorldCameraDir, VSArray<VSString> ShadowString,
	VSString & OutString)const{VSMAC_ASSERT(0);return ;}
void VSNullRenderer::CustomDirectionalLight(int iLightNum, const VSString &CustomString, VSArray<VSString> ShadowString, VSString & OutString)const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::OrenNayarDirectionalLight(int iLightNum, const VSString &Diffuse, const VSString &RoughnessSquared,
	const VSString &WorldNormal, const VSString &WorldCameraDir, bool bLookUpTable, VSArray<VSString> ShadowString,
	VSString & OutString)const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::PointLight(int iLightNum, const VSString &Diffuse, const VSString &Specular, const VSString &SpecularPow,
	const VSString & WorldPos, const VSString &WorldNormal, const VSString &WorldCameraDir, VSArray<VSString> ShadowString,
	VSString & OutString)const{VSMAC_ASSERT(0);return ;}
void VSNullRenderer::CustomPointLight(int iLightNum, const VSString &CustomString, const VSString & WorldPos, VSArray<VSString> ShadowString, VSString & OutString)const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::OrenNayarPointLight(int iLightNum, const VSString &Diffuse, const VSString &RoughnessSquared,
	const VSString & WorldPos, const VSString &WorldNormal, const VSString &WorldCameraDir, bool bLookUpTable, VSArray<VSString> ShadowString,
	VSString & OutString)const{VSMAC_ASSERT(0);return ;}


void VSNullRenderer::SpotLight(int iLightNum, const VSString &Diffuse, const VSString &Specular, const VSString &SpecularPow,
	const VSString & WorldPos, const VSString &WorldNormal, const VSString &WorldCameraDir, VSArray<VSString> ShadowString,
	VSString & OutString)const{VSMAC_ASSERT(0);return ;}
void VSNullRenderer::CustomSpotLight(int iLightNum, const VSString &CustomString, const VSString & WorldPos, VSArray<VSString> ShadowString, VSString & OutString)const{VSMAC_ASSERT(0);return ;}
void VSNullRenderer::OrenNayarSpotLight(int iLightNum, const VSString &Diffuse, const VSString &RoughnessSquared,
	const VSString & WorldPos, const VSString &WorldNormal, const VSString &WorldCameraDir, bool bLookUpTable, VSArray<VSString> ShadowString,
	VSString & OutString)const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::LocalToWorldPos(const VSString & LocalPos, VSString &OutString)const{VSMAC_ASSERT(0);return ;}
void VSNullRenderer::WorldToViewPos(const VSString & LocalPos, VSString &OutString)const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::LocalToWorldNormal(const VSString & LocalNormal, VSString &OutString)const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::WorldToViewNormal(const VSString & WorldNormal, VSString &OutString)const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::GetWorldViewDir(VSString &OutString)const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::TransProjPos(const VSString & Pos, const VSString & Matrix, VSString &OutString)const{VSMAC_ASSERT(0);return ;}
void VSNullRenderer::BumpNormal(const VSString &TexNormal, VSString &OutString)const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::ComputeDir(const VSString & Orgin, const VSString & End, VSString &OutString)const{VSMAC_ASSERT(0);return ;}
void VSNullRenderer::ComputeLength(const VSString & Orgin, const VSString & End, VSString &OutString)const{VSMAC_ASSERT(0);return ;}
void VSNullRenderer::ComputeBoneVector(const VSString& BoneIndex, const VSString & BoneWeight, const VSString &BoneVector,
	const  VSString & U, const VSString & V, const VSString &N, VSString &OutString)const{VSMAC_ASSERT(0);return ;}
void VSNullRenderer::ComputeBoneVector(const VSString& BoneIndex, const VSString & BoneWeight,
	const VSString &AnimDataResource, const VSString & AnimAllInfo,
	const VSString &AnimInstanceInfo, const VSString &AnimNum,
	const  VSString & U, const VSString & V, const VSString &N, VSString &OutString)const
{
	VSMAC_ASSERT(0); return;
}
void VSNullRenderer::BoneTranPos(const VSString & LocalPos, const  VSString & U, const VSString & V, const VSString &N, VSString &OutString)const{VSMAC_ASSERT(0);return ;}
void VSNullRenderer::BoneTranNormal(const VSString & LocalNormal, const  VSString & U, const VSString & V, const VSString &N, VSString &OutString)const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::DecodeNormal1(const VSString & Normal, VSString &OutString) const{VSMAC_ASSERT(0);return ;}
void VSNullRenderer::RecodeNormal1(const VSString & Normal, VSString &OutString) const{VSMAC_ASSERT(0);return ;}
void VSNullRenderer::DecodeNormal2(const VSString & Normal, const VSString & Tengent, VSString &OutString) const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::DecodeNormal3(const VSString & Normal, VSString &OutString) const{VSMAC_ASSERT(0);return ;}
void VSNullRenderer::RecodeNormal3(const VSString & Normal, VSString &OutString) const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::Reciprocal(const VSString & Value, VSString &OutString)const{ VSMAC_ASSERT(0); return; }
void VSNullRenderer::Clip(const VSString & ClipValue, VSString &OutString)const{VSMAC_ASSERT(0);return ;}
void VSNullRenderer::GreaterZeroPow(const VSString & Value, VSREAL Exp, VSString &OutString) const{VSMAC_ASSERT(0);return ;}
void VSNullRenderer::SaturatePow(const VSString & Value, VSREAL Exp, VSString &OutString) const{VSMAC_ASSERT(0);return ;}
void VSNullRenderer::Saturate(const VSString & Value, VSString &OutString)const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::PointLightCubeShadow(const VSString & PointLightName, const VSString & CubeShadowSampler, const VSString & ViewMatrix,
	const VSString & WorldPos, VSString& OutString)const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::PointLightCubeShadow(const VSString & PointLightName, const VSString & CubeShadowSampler,
	const VSString & WorldPos, VSString& OutString)const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::PointLightVolumeShadow(const VSString & PointLightName, const VSString & VolumeShadowSampler,
	const VSString & WorldPos, VSString& OutString)const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::PointLightDualParaboloidShadow(const VSString & PointLightName, const VSString & ShadowSampler,
	const VSString & WorldPos, VSString& OutString)const{VSMAC_ASSERT(0);return ;}
void VSNullRenderer::DirectionLightVolumeShadow(const VSString & DirLightName, const VSString & VolumeShadowSampler,
	const VSString & WorldPos, VSString& OutString)const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::DirectionLightCSMShadow(const VSString & DirLightName, const VSString & ShadowSampler,
	const VSString & WorldPos, const VSString & ViewPos, const VSString &FarZ, VSString& OutString)const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::DirectionLightShadow(const VSString & DirLightName, const VSString & ShadowSampler,
	const VSString & WorldPos, VSString& OutString)const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::SpotLightShadow(const VSString & SpotLightName, const VSString & ShadowSampler,
	const VSString & WorldPos, VSString& OutString)const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::TranLightToTemp(VSArray<VSLight *> LightArray, VSString & OutString)const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::GetLightFunction(VSArray<VSLight *> LightArray, const VSString & WorldPos, VSString & OutString)const{VSMAC_ASSERT(0);return ;}

void VSNullRenderer::EncodeReflect(const VSString & Mip, const VSString & Pow, VSString& OutString){VSMAC_ASSERT(0);return ;}

void VSNullRenderer::DecodeReflect(const VSString & Mip, const VSString & Pow, const VSString & Value, VSString& OutString){VSMAC_ASSERT(0);return ;}
unsigned int VSNullRenderer::GetTextureCompressLen(unsigned int uiLen)
{
	return Max((unsigned int)1, (unsigned int)(uiLen / 4));
}
