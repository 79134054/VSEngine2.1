#include "VSSceneRender.h"
#include "VSGraphicInclude.h"
#include "VSNormalDepthPass.h"
#include "VSPreZPass.h"
#include "VSMaterial.h"
#include "VSMeshData.h"
#include "VSProfiler.h"
using namespace VSEngine2;
VSSceneRenderInterface::VSSceneRenderInterface()
{
	m_uiClearFlag = VSRenderer::CF_USE_ALL;
	m_ClearColorRGBA = VSColorRGBA(0.0f,0.0f,0.0f,1.0f);
	m_fClearDepth = 1.0f;
	m_uiClearStencil = 0;
	
	m_bUseViewPortClear = false;
	m_uiRTWidth = 0;
	m_uiRTHeight = 0;
	m_uiMulSample = 0;
}
VSSceneRenderInterface::~VSSceneRenderInterface()
{
	m_pTargetArray.Clear();
}
VSRenderTarget * VSSceneRenderInterface::GetRenderTarget(unsigned int uiIndex)
{
	if (uiIndex >= m_pTargetArray.GetNum())
	{
		return NULL;
	}
	return m_pTargetArray[uiIndex];
}

bool VSSceneRenderInterface::AddRenderTarget(VSRenderTarget * pTarget)
{
	for (unsigned int i = 0 ; i < m_pTargetArray.GetNum() ; i++)
	{
		if (pTarget == m_pTargetArray[i])
		{
			return false;
		}
	}
	if (!pTarget)
	{
		return false;
	}
	if (!pTarget->GetWidth() || !pTarget->GetHeight())
	{
		return false;
	}
	if (!m_uiRTWidth && !m_uiRTHeight)
	{
		m_uiRTWidth = pTarget->GetWidth();
		m_uiRTHeight = pTarget->GetHeight();
		m_uiMulSample = pTarget->GetMulSample();
	}
	else if (m_uiRTHeight && m_uiRTWidth)
	{
		if (m_uiRTWidth != pTarget->GetWidth() || m_uiRTHeight != pTarget->GetHeight() || m_uiMulSample != pTarget->GetMulSample())
		{
			VSMAC_ASSERT(false);
			return false;
		}
	}
	else
	{
		VSMAC_ASSERT(false);
		return false;
	}

	m_pTargetArray.AddElement(pTarget);

	return true;
}
void VSSceneRenderInterface::SetRenderTargets()
{

	VSRenderTarget * pRenderTarget[RENDERTARGET_LEVEL] = { NULL };
	unsigned int uiRTNum = m_pTargetArray.GetNum() > VSRenderer::ms_pRenderer->GetMaxRTNum() ? VSRenderer::ms_pRenderer->GetMaxRTNum() : m_pTargetArray.GetNum();

	for (unsigned int i = 0; i < uiRTNum; i++)
	{
		pRenderTarget[i] = m_pTargetArray[i];
		//VSRenderer::ms_pRenderer->SetRenderTarget(m_pTargetArray[i],i);
	}
	if (uiRTNum > 0)
	{
		VSRenderer::ms_pRenderer->SetRenderTargets(pRenderTarget, uiRTNum);
	}

}
void VSSceneRenderInterface::EndRenderTargets()
{
	VSRenderTarget * pRenderTarget[RENDERTARGET_LEVEL] = { NULL };
	unsigned int uiRTNum = m_pTargetArray.GetNum() > VSRenderer::ms_pRenderer->GetMaxRTNum() ? VSRenderer::ms_pRenderer->GetMaxRTNum() : m_pTargetArray.GetNum();

	for (unsigned int i = 0; i < uiRTNum; i++)
	{
		pRenderTarget[i] = m_pTargetArray[i];
		//VSRenderer::ms_pRenderer->EndRenderTarget(m_pTargetArray[i],i);
	}
	if (uiRTNum > 0)
	{
		VSRenderer::ms_pRenderer->EndRenderTargets(pRenderTarget, uiRTNum);
	}
}
VSOcclusionQuerySceneRender::VSOcclusionQuerySceneRender(VSCuller & Culler, VSDepthStencilPtr pDepthStencil[VSCuller::RG_MAX])
{
	if (!VSCullerManager::GetCullerManager().IsRegister(&Culler))
	{
		return;
	}
	for (unsigned int i = 0 ; i < VSCuller::RG_MAX ; i++)
	{
		SetDepthStencil(pDepthStencil[i], i);
	}
	VSRenderTarget * pTempRenderTarget = VSResourceManager::GetRenderTarget(m_uiRTWidth, m_uiRTHeight, VSRenderer::SFT_A8R8G8B8, m_uiMulSample,false);
	AddRenderTarget(pTempRenderTarget);
	Draw(Culler,0.0);
}
VSOcclusionQuerySceneRender::~VSOcclusionQuerySceneRender()
{
	VSRenderTargetPtr pTarget = GetRenderTarget(0);
	if (pTarget)
	{
		VSResourceManager::DisableOutputResource(pTarget);
	}	
}
bool VSOcclusionQuerySceneRender::OnDraw(VSCuller & Culler, unsigned int uiRenderGroup, double dAppTime)
{
// 	VSArray<VSGeometryOcclusionQueryData> & GeometryOcclusionQueryData = VSCullerManager::GetCullerManager().GetGroupGeometryOcclusionQueryData(&Culler, uiRenderGroup);
// 	for (unsigned int i = 0; i < GeometryOcclusionQueryData.GetNum(); i++)
// 	{
// 		VSGeometryOcclusionQueryData & GOQD = GeometryOcclusionQueryData[i];
// 		VSPreZPass * pPreZPass = VSPreZPass::GetDefault();
// 
// 		VSMaterialInstance * pMaterialInstance = NULL;
// 		if (Culler.GetUseMaterialIndex() == VSEngineFlag::EUML_NONE)
// 		{
// 			pMaterialInstance = GOQD.pGeometry->GetUseMaterialInstance();
// 		}
// 		else
// 		{
// 			pMaterialInstance = GOQD.pGeometry->GetMaterialInstance(Culler.GetUseMaterialIndex());
// 		}
// 
// 		pPreZPass->SetDrawContent(pMaterialInstance, GOQD.pGeometry,
// 			Culler.GetCamera(), 0);
// 		VSQuery * pQuery = GOQD.OcclusionQueryType.pQuery;
// 		VSRenderer::ms_pRenderer->BeginQuery(pQuery);
// 		pPreZPass->Draw(VSRenderer::ms_pRenderer);
// 		VSRenderer::ms_pRenderer->EndQuery(pQuery);
// 	}
	VSCustomMaterial * pOcclustionCullPass = VSCustomMaterial::GetOcclustionCullPass();
	VSCamera * pCamera = Culler.GetCamera();
	VSVShader * pVShader = pOcclustionCullPass->GetCurVShader(*VSShaderKey::ms_NULLShaderKey);
	VSPShader * pPShader = pOcclustionCullPass->GetCurPShader(*VSShaderKey::ms_NULLShaderKey);

	VSArray<VSGeometryOcclusionQueryData> & GeometryOcclusionQueryData = VSCullerManager::GetCullerManager().GetGroupGeometryOcclusionQueryData(&Culler, uiRenderGroup);
	for (unsigned int i = 0 ; i < GeometryOcclusionQueryData.GetNum() ; i++)
	{
		VSGeometryOcclusionQueryData & GOQD = GeometryOcclusionQueryData[i];

		VSTransform T = GOQD.pGeometry->GetWorldTransform();
		VSAABB3 LocalBV = GOQD.pGeometry->GetLocalBV();
		VSREAL fA[3];
		LocalBV.GetfA(fA);
		VSVector3 Scale = T.GetScale() * VSVector3(fA[0], fA[1], fA[2]);
		T.SetScale(Scale);
		VSMatrix3X3W WorldMatrix = T.GetCombine();
		VSMatrix3X3W ViewProjectMatrix = pCamera->GetViewMatrix() * pCamera->GetProjMatrix();
		pVShader->SetParam(VSUsedName::ms_cWorldMatrix, &WorldMatrix);
		pVShader->SetParam(VSUsedName::ms_cViewProjectMatrix, &ViewProjectMatrix);
		VSQuery * pQuery = GOQD.OcclusionQueryType.pQuery;
		VSRenderer::ms_pRenderer->BeginQuery(pQuery);
		if (!VSRenderer::ms_pRenderer->DrawMesh(VSGeometry::GetDefaultRenderCube(), &pOcclustionCullPass->GetRenderState(), pVShader, pPShader,
			NULL, NULL, NULL))
		{
			return false;
		}
		VSRenderer::ms_pRenderer->EndQuery(pQuery);
	}
	return true;
}
bool VSOcclusionQuerySceneRender::Draw(VSCuller & Culler, double dAppTime)
{
	if (Culler.GetCamera()->GetViewPortNum())
	{
		for (unsigned int i = 0; i < Culler.GetCamera()->GetViewPortNum(); i++)
		{

			VSViewPort * pViewPort = Culler.GetCamera()->GetViewPort(i);
			unsigned int uiRtWidth = m_pTargetArray[0]->GetWidth();
			unsigned int uiRtHeight = m_pTargetArray[0]->GetHeight();
			unsigned int X = Rounding(pViewPort->XMin * uiRtWidth);
			unsigned int Y = Rounding(pViewPort->YMin * uiRtHeight);
			unsigned int Width = Rounding(pViewPort->XMax * uiRtWidth) - X;
			unsigned int Height = Rounding(pViewPort->YMax * uiRtHeight) - Y;



			VSREAL fAspect = Culler.GetCamera()->GetAspect();
			if (pViewPort && pViewPort->bChangeAspect)
			{
				VSREAL NewRatio = (Width * 1.0f) / (Height);
				Culler.GetCamera()->SetAspect(NewRatio);
			}
			SetRenderTargets();
			VSRenderer::ms_pRenderer->SetViewPort(pViewPort);
			for (unsigned int uiRenderGroup = 0; uiRenderGroup < VSCuller::RG_MAX; uiRenderGroup++)
			{
				if (m_pDepthStencil[uiRenderGroup])
				{
					VSRenderer::ms_pRenderer->SetDepthStencilBuffer(m_pDepthStencil[uiRenderGroup]);
					OnDraw(Culler,uiRenderGroup,dAppTime);
					VSRenderer::ms_pRenderer->EndDepthStencilBuffer(m_pDepthStencil[uiRenderGroup]);
				}
			}
			EndRenderTargets();
			if (pViewPort && pViewPort->bChangeAspect)
			{
				Culler.GetCamera()->SetAspect(fAspect);
			}
		}

	}
	else
	{
		SetRenderTargets();
		VSRenderer::ms_pRenderer->SetViewPort(NULL);
		for (unsigned int uiRenderGroup = 0; uiRenderGroup < VSCuller::RG_MAX; uiRenderGroup++)
		{
			if (m_pDepthStencil[uiRenderGroup])
			{
				VSRenderer::ms_pRenderer->SetDepthStencilBuffer(m_pDepthStencil[uiRenderGroup]);
				OnDraw(Culler, uiRenderGroup, dAppTime);
				VSRenderer::ms_pRenderer->EndDepthStencilBuffer(m_pDepthStencil[uiRenderGroup]);
			}
		}
		EndRenderTargets();
	}
	return 1;
}
VSSceneRender::VSSceneRender()
{
	for (unsigned int i = 0 ; i < VSCuller::RG_MAX ; i++)
	{
		m_pDepthStencil[i] = NULL;
	}
	m_uiRenderStateInheritFlag = 0;
}
VSSceneRender::~VSSceneRender()
{
	
	for (unsigned int i = 0 ; i < VSCuller::RG_MAX ; i++)
	{
		m_pDepthStencil[i] = NULL;
	}
}
void VSSceneRender::SetUseState(VSRenderState & RenderState, unsigned int uiRenderStateInheritFlag)
{
	m_UseState.GetAll(&RenderState);
	m_uiRenderStateInheritFlag = uiRenderStateInheritFlag;
}
void VSSceneRender::ClearUseState()
{
	m_uiRenderStateInheritFlag = 0;
}
bool VSSceneRender::SetDepthStencil(VSDepthStencil * pDepthStencil,unsigned int uiRenderGroup)
{
	if (uiRenderGroup >= VSCuller::RG_MAX)
	{
		return false;
	}
	if(pDepthStencil)
	{
		if (!pDepthStencil->GetWidth() || !pDepthStencil->GetHeight())
		{
			return false;
		}
		if (!m_uiRTHeight && !m_uiRTWidth)
		{
			m_uiRTWidth = pDepthStencil->GetWidth();
			m_uiRTHeight = pDepthStencil->GetHeight();
			m_uiMulSample = pDepthStencil->GetMulSample();
		}
		else if (m_uiRTHeight && m_uiRTWidth)
		{
			if (m_uiRTWidth != pDepthStencil->GetWidth() || m_uiRTHeight != pDepthStencil->GetHeight() || m_uiMulSample != pDepthStencil->GetMulSample())
			{
				VSMAC_ASSERT(false);
				return false;
			}
		}
		else
		{
			VSMAC_ASSERT(false);
			return false;
		}
	}
	
	
	
	
	m_pDepthStencil[uiRenderGroup] = pDepthStencil;
	return true;
}
void VSSceneRender::ClearRTAndDepth()
{
	m_pTargetArray.Clear();
	m_uiRTWidth = 0;
	m_uiRTHeight = 0;
	m_uiMulSample = 0;
	for (unsigned int i = 0 ; i < VSCuller::RG_MAX ; i++)
	{
		m_pDepthStencil[i] = NULL;
	}
}
void VSSceneRender::DrawUseCurView(VSCuller & Culler,double dAppTime)
{

}
void VSSceneRender::DisableUseCurView(VSCuller & Culler,double dAppTime)
{

}
bool VSSceneRender::Draw(VSCuller & Culler,double dAppTime)
{

	if (m_uiClearFlag <= VSRenderer::CF_USE_ALL)
	{

		VSColorRGBA ClearColorRGBA = VSRenderer::ms_pRenderer->GetClearColor();
		VSREAL fClearDepth = VSRenderer::ms_pRenderer->GetClearDepth();
		unsigned int uiClearStencil = VSRenderer::ms_pRenderer->GetClearStencil();

		m_SaveRenderState = VSRenderer::ms_pRenderer->GetUseState();
		unsigned int uiSaveRenderStateInheritFlag = VSRenderer::ms_pRenderer->GetRenderStateInheritFlag();

		VSRenderer::ms_pRenderer->SetClearColor(m_ClearColorRGBA);
		VSRenderer::ms_pRenderer->SetClearDepth(m_fClearDepth);
		VSRenderer::ms_pRenderer->SetClearStencil(m_uiClearStencil);

		if (m_uiRenderStateInheritFlag)
		{			
			VSRenderer::ms_pRenderer->SetUseState(m_UseState,m_uiRenderStateInheritFlag);		
		}

		if (Culler.GetCamera()->GetViewPortNum())
		{
			if (!m_bUseViewPortClear)
			{
				SetRenderTargets();
				VSRenderer::ms_pRenderer->ClearBuffers(m_uiClearFlag & VSRenderer::CF_COLOR);
				EndRenderTargets();
			}
			for (unsigned int i = 0 ; i< Culler.GetCamera()->GetViewPortNum() ;i++)
			{

				VSViewPort * pViewPort = Culler.GetCamera()->GetViewPort(i);
				unsigned int uiRtWidth = m_pTargetArray[0]->GetWidth();
				unsigned int uiRtHeight = m_pTargetArray[0]->GetHeight();
				unsigned int X = Rounding(pViewPort->XMin * uiRtWidth);
				unsigned int Y = Rounding(pViewPort->YMin * uiRtHeight);
				unsigned int Width = Rounding(pViewPort->XMax * uiRtWidth) - X;
				unsigned int Height = Rounding(pViewPort->YMax * uiRtHeight) - Y;

				
				
				VSREAL fAspect = Culler.GetCamera()->GetAspect();
				if (pViewPort && pViewPort->bChangeAspect)
				{	
					VSREAL NewRatio = (Width * 1.0f) / (Height) ;
					Culler.GetCamera()->SetAspect(NewRatio);
				}

				DrawUseCurView(Culler,dAppTime);

				SetRenderTargets();			
				VSRenderer::ms_pRenderer->SetViewPort(pViewPort);
				if (m_bUseViewPortClear)
				{		
					VSRenderer::ms_pRenderer->ClearBuffers(m_uiClearFlag & VSRenderer::CF_COLOR,X,Y,Width,Height);
				}
				for (unsigned int uiRenderGroup = 0 ; uiRenderGroup < VSCuller::RG_MAX ;uiRenderGroup++)
				{	
					if (!Culler.GetRenderGroupVisibleNum(uiRenderGroup))
					{
						continue;
					}
					if (m_pDepthStencil[uiRenderGroup])
					{
						VSRenderer::ms_pRenderer->SetDepthStencilBuffer(m_pDepthStencil[uiRenderGroup]);
					
						if (!m_bUseViewPortClear)
						{
							VSRenderer::ms_pRenderer->ClearBuffers((m_uiClearFlag & VSRenderer::CF_DEPTH) | (m_uiClearFlag & VSRenderer::CF_STENCIL));
						}
						else
						{
							VSRenderer::ms_pRenderer->ClearBuffers((m_uiClearFlag & VSRenderer::CF_DEPTH) | (m_uiClearFlag & VSRenderer::CF_STENCIL),X,Y,Width,Height);
						}
						OnDraw(Culler,uiRenderGroup,dAppTime);
						VSRenderer::ms_pRenderer->EndDepthStencilBuffer(m_pDepthStencil[uiRenderGroup]);
					}
					else
					{
						if (!m_bUseViewPortClear)
						{
							VSRenderer::ms_pRenderer->ClearBuffers((m_uiClearFlag & VSRenderer::CF_DEPTH) | (m_uiClearFlag & VSRenderer::CF_STENCIL));
						}
						else
						{
							VSRenderer::ms_pRenderer->ClearBuffers((m_uiClearFlag & VSRenderer::CF_DEPTH) | (m_uiClearFlag & VSRenderer::CF_STENCIL),X,Y,Width,Height);
						}
						OnDraw(Culler,uiRenderGroup,dAppTime);
					}
				}		
				EndRenderTargets();
				DisableUseCurView(Culler,dAppTime);
				if (pViewPort && pViewPort->bChangeAspect)
				{
					Culler.GetCamera()->SetAspect(fAspect);
				}
			}

		}
		else
		{
			DrawUseCurView(Culler,dAppTime);
			SetRenderTargets();
			VSRenderer::ms_pRenderer->SetViewPort(NULL);
			VSRenderer::ms_pRenderer->ClearBuffers(m_uiClearFlag);
			for (unsigned int uiRenderGroup = 0 ; uiRenderGroup < VSCuller::RG_MAX ;uiRenderGroup++)
			{
				if (!Culler.GetRenderGroupVisibleNum(uiRenderGroup))
				{
					continue;
				}
				if (m_pDepthStencil[uiRenderGroup])
				{
					VSRenderer::ms_pRenderer->SetDepthStencilBuffer(m_pDepthStencil[uiRenderGroup]);
					VSRenderer::ms_pRenderer->ClearBuffers((m_uiClearFlag & VSRenderer::CF_DEPTH) | (m_uiClearFlag & VSRenderer::CF_STENCIL));									
					OnDraw(Culler,uiRenderGroup,dAppTime);
					VSRenderer::ms_pRenderer->EndDepthStencilBuffer(m_pDepthStencil[uiRenderGroup]);
				}
				else
				{
					VSRenderer::ms_pRenderer->ClearBuffers((m_uiClearFlag & VSRenderer::CF_DEPTH) | (m_uiClearFlag & VSRenderer::CF_STENCIL));									
					OnDraw(Culler,uiRenderGroup,dAppTime);
				}
			}
			EndRenderTargets();
			DisableUseCurView(Culler,dAppTime);
		}
		
		VSRenderer::ms_pRenderer->SetClearColor(ClearColorRGBA);
		VSRenderer::ms_pRenderer->SetClearDepth(fClearDepth);
		VSRenderer::ms_pRenderer->SetClearStencil(uiClearStencil);
		if (m_uiRenderStateInheritFlag)
		{
			VSRenderer::ms_pRenderer->SetUseState(m_SaveRenderState,uiSaveRenderStateInheritFlag);
		}
	}




	return true;
}
VSDebugDrawSceneRender::VSDebugDrawSceneRender()
{
	for (unsigned int i =0 ; i < VSCuller::RG_MAX ;i++)
	{
		m_pDebugDraw[i] = VS_NEW VSDebugDraw();

	}
}
VSDebugDrawSceneRender::~VSDebugDrawSceneRender()
{
	for (unsigned int i =0 ; i < VSCuller::RG_MAX ;i++)
	{
		m_pDebugDraw[i] = NULL;
	}
}
bool VSDebugDrawSceneRender::OnDraw(VSCuller & Culler,unsigned int uiRenderGroup,double dAppTime)
{
	if (m_pDebugDraw[uiRenderGroup])
	{
		m_pDebugDraw[uiRenderGroup]->DrawDebugInfo(Culler.GetCamera());
	}
	return true;
}
VSMaterialSceneRender::VSMaterialSceneRender()
{
	m_pNormalDepthTexture = NULL;
}
VSMaterialSceneRender::~VSMaterialSceneRender()
{

}
void VSMaterialSceneRender::DrawUseCurView(VSCuller & Culler,double dAppTime)
{
	for (unsigned int i = 0 ;i < Culler.GetLightNum() ; i++)
	{
		VSLocalLight * pLocalLight = DynamicCast<VSLocalLight>(Culler.GetLight(i));
		if (pLocalLight)
		{
			pLocalLight->DrawDependenceShadowMap(Culler,dAppTime);
			pLocalLight->DrawNoDependenceShadowMap(Culler, dAppTime);
		}

	}
}
void VSMaterialSceneRender::DisableUseCurView(VSCuller & Culler,double dAppTime)
{
	for (unsigned int i = 0 ;i < Culler.GetLightNum() ; i++)
	{
		VSLocalLight * pLocalLight = DynamicCast<VSLocalLight>(Culler.GetLight(i));
		if (pLocalLight)
		{
			pLocalLight->DisableDependenceShadowMap(Culler,dAppTime);
		}

	}
}
void VSMaterialSceneRender::GetGroup(VSCuller & Culler, unsigned int uiRenderGroup)
{
	m_NormalAndDepth.Clear();
	m_NoNormalOrDepth.Clear();
	m_Combine.Clear();
	m_AlphaBlend.Clear();
	for (unsigned int t = 0; t <= VSCuller::VST_COMBINE; t++)
	{	
		for(unsigned int j = 0; j < Culler.GetVisibleNum(t,uiRenderGroup) ; j++)
		{
			
			VSRenderContext& VisibleContext = Culler.GetVisibleSpatial(j, t, uiRenderGroup);
			if (!VisibleContext.IsCanDraw())
			{
				continue;
			}
			VSGeometry * pGeometry = VisibleContext.m_pGeometry;
			VSMaterial * pMaterial = VisibleContext.m_pMaterialInstance->GetMaterial();
			if (t < VSCuller::VST_ALPHABLEND)
			{
				const VSDepthStencilDesc & DepthStencilDest = pMaterial->GetRenderState(VisibleContext.m_uiPassId).GetDepthStencilState()->GetDepthStencilDesc();
				unsigned int uiNormalLevel = pGeometry->GetMeshData()->GetVertexBuffer()->GetNormalLevel();
				// cull no write depth or no normal
				if (!DepthStencilDest.m_bDepthWritable || !uiNormalLevel)
				{
					m_NoNormalOrDepth.AddElement(&VisibleContext);
					continue;
				}
				else
				{
					m_NormalAndDepth.AddElement(&VisibleContext);
				}
			}
			else if (t == VSCuller::VST_ALPHABLEND)
			{
				m_AlphaBlend.AddElement(&VisibleContext);
			}
			else if (t == VSCuller::VST_COMBINE)
			{
				m_Combine.AddElement(&VisibleContext);
			}
		}
	}
}
void VSMaterialSceneRender::DrawGroup(VSCuller & Culler, unsigned int uiRenderGroup, VSArray<VSRenderContext *> & Group, double dAppTime)
{
	for (unsigned int i = 0; i < Group.GetNum(); i++)
	{
		VSRenderContext& VisibleContext = *Group[i];
		if (VisibleContext.m_pIndirectLight.GetNum() == 0)
		{
			continue;
		}

		VSIndirectRenderPass * pIndirectRenderPass = VisibleContext.m_pMaterialInstance->GetMaterial()->GetIndirectRenderPass();
		for (unsigned int l = 0; l < VisibleContext.m_pIndirectLight.GetNum(); l++)
		{
			pIndirectRenderPass->AddLight(VisibleContext.m_pIndirectLight[l]);
		}
		pIndirectRenderPass->SetDrawContent(VisibleContext.m_pMaterialInstance, VisibleContext.m_pGeometry,
			Culler.GetCamera(), VisibleContext.m_uiPassId); 
		pIndirectRenderPass->Draw(VSRenderer::ms_pRenderer);
	}
	if (&Group == &m_NormalAndDepth)
	{
		DrawVolumeShadow(Culler, uiRenderGroup, dAppTime);
	}
	
	for (unsigned int i = 0; i < Group.GetNum(); i++)
	{
		VSRenderContext& VisibleContext = *Group[i];
		if (VisibleContext.m_pDirectLight.GetNum() == 0)
		{
			continue;
		}

		VSMaterialPass * pMaterialPass = VisibleContext.m_pMaterialInstance->GetMaterial()->GetMaterialPass();
		for (unsigned int l = 0; l < VisibleContext.m_pDirectLight.GetNum(); l++)
		{
			pMaterialPass->AddLight(VisibleContext.m_pDirectLight[l]);
		}
		pMaterialPass->SetDrawContent(VisibleContext.m_pMaterialInstance, VisibleContext.m_pGeometry,
			Culler.GetCamera(), VisibleContext.m_uiPassId);
		pMaterialPass->Draw(VSRenderer::ms_pRenderer);
	}
}
void VSMaterialSceneRender::DrawVolumeShadow(VSCuller & Culler, unsigned int uiRenderGroup, double dAppTime)
{
	if (uiRenderGroup == VSCuller::RG_NORMAL)
	{
		if (Culler.GetCamera()->GetViewPortNum())
		{
			return;
		}
		for (unsigned int i = 0; i < Culler.GetLightNum(); i++)
		{
			VSLocalLight * pLocalLight = DynamicCast<VSLocalLight>(Culler.GetLight(i));
			if (pLocalLight)
			{
				pLocalLight->DrawVolumeShadow(Culler, dAppTime);
			}
		}
	}
}
void VSMaterialSceneRender::DrawProjectShadow(VSCuller & Culler, unsigned int uiRenderGroup, double dAppTime)
{
	if (uiRenderGroup == VSCuller::RG_NORMAL && m_pNormalDepthTexture)
	{
		if (Culler.GetCamera()->GetViewPortNum())
		{
			return;
		}
		for (unsigned int i = 0; i < Culler.GetLightNum(); i++)
		{
			VSLocalLight * pLocalLight = DynamicCast<VSLocalLight>(Culler.GetLight(i));
			if (pLocalLight)
			{
				pLocalLight->DrawPorjectShadow(Culler, dAppTime, m_pNormalDepthTexture);
			}
		}
	}
}
bool VSMaterialSceneRender::OnDraw(VSCuller & Culler,unsigned int uiRenderGroup,double dAppTime)
{
	GetGroup(Culler, uiRenderGroup);
	DrawGroup(Culler, uiRenderGroup, m_NormalAndDepth, dAppTime);
	DrawProjectShadow(Culler, uiRenderGroup, dAppTime);
	DrawGroup(Culler, uiRenderGroup, m_NoNormalOrDepth, dAppTime);
	DrawGroup(Culler, uiRenderGroup, m_Combine, dAppTime);
	DrawGroup(Culler, uiRenderGroup, m_AlphaBlend, dAppTime);
	
	return true;
}
VSNormalDepthSceneRender::VSNormalDepthSceneRender()
{

}
VSNormalDepthSceneRender::~VSNormalDepthSceneRender()
{

}
bool VSNormalDepthSceneRender::OnDraw(VSCuller & Culler,unsigned int uiRenderGroup,double dAppTime)
{

	for (unsigned int t = 0 ; t < VSCuller::VST_MAX ; t++)
	{
		// cull alpha geometry and combine
		if (t == VSCuller::VST_ALPHABLEND || t == VSCuller::VST_COMBINE)
		{
			continue;
		}
		for(unsigned int j = 0; j < Culler.GetVisibleNum(t,uiRenderGroup) ; j++)
		{
			VSRenderContext& VisibleContext = Culler.GetVisibleSpatial(j,t,uiRenderGroup);
			if (!VisibleContext.IsCanDraw())
			{
				continue ;
			}
			

// 			// cull Combine Group no write depth
// 			const VSBlendDesc & BlendDest = pMaterial->GetRenderState(VisibleContext.m_uiPassId).GetBlendState()->GetBlendDesc();
// 			if (BlendDest.IsBlendUsed())
// 			{
// 				continue;
// 			}
			VSMaterial * pMaterial = VisibleContext.m_pMaterialInstance->GetMaterial();
			const VSDepthStencilDesc & DepthStencilDest = pMaterial->GetRenderState(VisibleContext.m_uiPassId).GetDepthStencilState()->GetDepthStencilDesc();

			unsigned int uiNormalLevel = VisibleContext.m_pGeometry->GetMeshData()->GetVertexBuffer()->GetNormalLevel();
			// cull no write depth or no normal
			if (!DepthStencilDest.m_bDepthWritable || !uiNormalLevel)
			{
				continue;
			}
					
			VSNormalDepthPass * pNormalDepthPass = VisibleContext.m_pMaterialInstance->GetMaterial()->GetNormalDepthPass();
			pNormalDepthPass->SetDrawContent(VisibleContext.m_pMaterialInstance, VisibleContext.m_pGeometry,
				Culler.GetCamera(), VisibleContext.m_uiPassId);
			pNormalDepthPass->Draw(VSRenderer::ms_pRenderer);
		}
	}
	return true;
}

VSPreZSceneRender::VSPreZSceneRender()
{

}
VSPreZSceneRender::~VSPreZSceneRender()
{

}
bool VSPreZSceneRender::OnDraw(VSCuller & Culler,unsigned int uiRenderGroup,double dAppTime)
{
	//PreZ
	for (unsigned int t = 0 ; t < VSCuller::VST_MAX ; t++)
	{
		// cull alpha geometry
		if (t == VSCuller::VST_ALPHABLEND)
		{
			continue;
		}
		for(unsigned int j = 0; j < Culler.GetVisibleNum(t,uiRenderGroup) ; j++)
		{
			VSRenderContext& VisibleContext = Culler.GetVisibleSpatial(j,t,uiRenderGroup);
			if (!VisibleContext.IsCanDraw())
			{
				continue;
			}
			VSMaterial * pMaterial = VisibleContext.m_pMaterialInstance->GetMaterial();
			//cull geometry , the VST_COMBINE has alpha 
			const VSBlendDesc & BlendDest = pMaterial->GetRenderState(VisibleContext.m_uiPassId).GetBlendState()->GetBlendDesc();
			if (BlendDest.IsBlendUsed())
			{
				continue;
			}
			const VSDepthStencilDesc & DepthStencilDest = pMaterial->GetRenderState(VisibleContext.m_uiPassId).GetDepthStencilState()->GetDepthStencilDesc();

			// cull no write depth
			if (!DepthStencilDest.m_bDepthWritable)
			{
				continue;
			}
			VSPreZPass * pPreZPass = VSPreZPass::GetDefault();
			pPreZPass->SetDrawContent(VisibleContext.m_pMaterialInstance, VisibleContext.m_pGeometry,
				Culler.GetCamera(), 0);
			pPreZPass->Draw(VSRenderer::ms_pRenderer);
		}
	}
	return true;
}
VSPostEffectSceneRender::VSPostEffectSceneRender()
{
	m_uiClearFlag = VSRenderer::CF_NONE;
	m_pCustomMaterial = NULL;
	m_pFinalRenderTarget = NULL;
}
VSPostEffectSceneRender::~VSPostEffectSceneRender()
{
	m_pFinalRenderTarget = NULL;
	m_pCustomMaterial = NULL;
}
void VSPostEffectSceneRender::GetRT(unsigned int uiWidth,unsigned int uiHeight)
{
	ClearRTAndDepth();
	VSMAC_ASSERT(!m_pFinalRenderTarget);
	m_pFinalRenderTarget = VSResourceManager::GetRenderTarget(uiWidth,uiHeight,VSRenderer::SFT_A8R8G8B8,0);
	AddRenderTarget(m_pFinalRenderTarget);
}
void VSPostEffectSceneRender::SetNoUseRTRenderSize(unsigned int uiWidth,unsigned int uiHeight)
{
	m_uiRTHeight = uiHeight;
	m_uiRTWidth = uiWidth;
}
void VSPostEffectSceneRender::DisableRT()
{
	for (unsigned int i = 0 ; i < m_pTargetArray.GetNum() ;i++)
	{
		if (m_pTargetArray[i])
		{
			VSResourceManager::DisableOutputResource(m_pTargetArray[i]);
		}		
	}	
	ClearRTAndDepth();
}
void VSPostEffectSceneRender::ClearRTAndDepth()
{
	m_pTargetArray.Clear();
	m_uiRTWidth = 0;
	m_uiRTHeight = 0;
	m_uiMulSample = 0;
	m_pFinalRenderTarget = NULL;
}
bool VSPostEffectSceneRender::SetDepthStencil(VSDepthStencil * pDepthStencil)
{
	if (!pDepthStencil)
	{
		return false;
	}
	if (!pDepthStencil->GetWidth() || !pDepthStencil->GetHeight())
	{
		return false;
	}
	if (m_uiRTHeight && m_uiRTWidth)
	{
		if (m_uiRTWidth != pDepthStencil->GetWidth() || m_uiRTHeight != pDepthStencil->GetHeight())
		{
			return false;
		}
	}
	else
	{
		m_uiRTWidth = pDepthStencil->GetWidth();
		m_uiRTHeight = pDepthStencil->GetHeight();
	}
	m_pDepthStencil = pDepthStencil;
	return true;
}
bool VSPostEffectSceneRender::AddRenderTarget(VSRenderTarget * pTarget)
{
	if (!VSSceneRenderInterface::AddRenderTarget(pTarget))
	{
		return false;
	}
	if (m_pFinalRenderTarget == NULL)
	{
		m_pFinalRenderTarget = pTarget;
	}
	return true;
}
bool VSPostEffectSceneRender::OnDraw(VSCuller & Culler, double dAppTime)
{
	VSMAC_ASSERT(m_pCustomMaterial);

	if (!m_uiRTWidth || !m_uiRTHeight)
	{
		return false;
	}

	VSREAL Inv_Width = 1.0f / m_uiRTWidth;
	VSREAL Inv_Height = 1.0f / m_uiRTHeight;
	m_pCustomMaterial->SetVShaderValue(VSUsedName::ms_cPostInv_Width, &Inv_Width);
	m_pCustomMaterial->SetVShaderValue(VSUsedName::ms_cPostInv_Height, &Inv_Height);
	if (!m_pCustomMaterial->UseMaterial())
		return false;
	VSRenderer::ms_pRenderer->DrawScreen(NULL);

	return true;
}
bool VSPostEffectSceneRender::Draw(VSCuller & Culler,double dAppTime)
{
	SetRenderTargets();
	if (m_uiClearFlag > 0 && m_uiClearFlag <= VSRenderer::CF_USE_ALL)
	{
		VSColorRGBA ClearColorRGBA = VSRenderer::ms_pRenderer->GetClearColor();
		VSREAL fClearDepth = VSRenderer::ms_pRenderer->GetClearDepth();
		unsigned int uiClearStencil = VSRenderer::ms_pRenderer->GetClearStencil();

		VSRenderer::ms_pRenderer->SetClearColor(m_ClearColorRGBA);
		VSRenderer::ms_pRenderer->SetClearDepth(m_fClearDepth);
		VSRenderer::ms_pRenderer->SetClearStencil(m_uiClearStencil);

		VSRenderer::ms_pRenderer->SetViewPort(NULL);
		VSRenderer::ms_pRenderer->ClearBuffers(m_uiClearFlag);
		OnDraw(Culler, dAppTime);

		VSRenderer::ms_pRenderer->SetClearColor(ClearColorRGBA);
		VSRenderer::ms_pRenderer->SetClearDepth(fClearDepth);
		VSRenderer::ms_pRenderer->SetClearStencil(uiClearStencil);
	}
	else
	{
		VSRenderer::ms_pRenderer->SetViewPort(NULL);
		OnDraw(Culler, dAppTime);
	}
	EndRenderTargets();
	return true;
}
VSCommonPESceneRender::VSCommonPESceneRender()
{
	for (unsigned int i = 0; i < POSTEFFECT_MAX_INPUT; i++)
	{
		m_pTexAllState[i] = VS_NEW VSTexAllState();
		m_pTexAllState[i]->SetSamplerState((VSSamplerState*)VSSamplerState::GetTwoLine());
	}
};
VSCommonPESceneRender::~VSCommonPESceneRender()
{
	for (unsigned int i = 0; i < POSTEFFECT_MAX_INPUT; i++)
	{
		m_pTexAllState[i] = NULL;
	}
};
void VSCommonPESceneRender::OnUpdate(double dAppTime)
{

}
void VSCommonPESceneRender::GetRT(unsigned int uiWidth, unsigned int uiHeight)
{
	ClearRTAndDepth();
	VSMAC_ASSERT(!m_pFinalRenderTarget);
	m_uiRTWidth = (unsigned int)(uiWidth * m_fGetRTScale);
	m_uiRTHeight = (unsigned int)(uiHeight * m_fGetRTScale);
	m_pFinalRenderTarget = VSResourceManager::GetRenderTarget(m_uiRTWidth, m_uiRTHeight, VSRenderer::SFT_A8R8G8B8, 0);
	AddRenderTarget(m_pFinalRenderTarget);
}
bool VSCommonPESceneRender::OnDraw(VSCuller & Culler, double dAppTime)
{
	VSMAC_ASSERT(m_pCustomMaterial);
	if (!m_uiRTWidth || !m_uiRTHeight)
	{
		return false;
	}
	OnUpdate(dAppTime);
	VSREAL Inv_Width = 1.0f / m_uiRTWidth;
	VSREAL Inv_Height = 1.0f / m_uiRTHeight;
	m_pCustomMaterial->SetVShaderValue(VSUsedName::ms_cPostInv_Width, &Inv_Width);
	m_pCustomMaterial->SetVShaderValue(VSUsedName::ms_cPostInv_Height, &Inv_Height);
	for (unsigned int i = 0 ; i < POSTEFFECT_MAX_INPUT; i++)
	{
		if (m_pTexAllState[i]->GetTexture())
		{
			m_pCustomMaterial->SetPShaderTexture(VSUsedName::ms_cPostInputTexture[i], m_pTexAllState[i]);
		}		
	}
	if (!m_pCustomMaterial->UseMaterial())
		return false;
	VSRenderer::ms_pRenderer->DrawScreen(NULL);
	return true;
}
VSTexture * VSCommonPESceneRender::GetInputTexture(unsigned int i) const
{
	return m_pTexAllState[i]->GetTexture();
}
void VSCommonPESceneRender::ClearRTAndDepth()
{
	VSPostEffectSceneRender::ClearRTAndDepth();
	for (unsigned int i = 0; i < POSTEFFECT_MAX_INPUT; i++)
	{
		m_pTexAllState[i]->SetTexture(NULL);
	}
}
void VSCommonPESceneRender::SetSourceTarget(VSTexture * pTexture, unsigned int i)
{
	m_pTexAllState[i]->SetTexture(pTexture);
}

VSCombinePostEffectSceneRender::~VSCombinePostEffectSceneRender()
{

};
void VSCombinePostEffectSceneRender::OnUpdate(unsigned int uiPassID, double dAppTime)
{
	if (uiPassID == 0)
	{
		VSMAC_ASSERT(m_pTexAllState[0]->GetTexture());
		m_CPEArray[0]->SetSourceTarget(m_pTexAllState[0]->GetTexture());
	}	
}
bool VSCombinePostEffectSceneRender::Draw(VSCuller & Culler, double dAppTime)
{
	for (unsigned int i = 0; i < m_CPEArray.GetNum(); i++)
	{
		OnUpdate(i, dAppTime);
		m_CPEArray[i]->Draw(Culler, dAppTime);
		if (i + 1 < m_CPEArray.GetNum())
		{
			m_CPEArray[i + 1]->SetSourceTarget((VSTexture *)m_CPEArray[i]->GetRenderTarget(0)->GetCreateBy());
		}
	}
	for (unsigned int i = 0; i < m_CPEArray.GetNum() - 1; i++)
	{
		m_CPEArray[i]->DisableRT();
	}
	return true;
}
void VSCombinePostEffectSceneRender::GetRT(unsigned int uiWidth, unsigned int uiHeight)
{
	m_uiRTWidth = (unsigned int)(uiWidth * m_fGetRTScale);
	m_uiRTHeight = (unsigned int)(uiHeight * m_fGetRTScale);
	for (unsigned int i = 0; i < m_CPEArray.GetNum(); i++)
	{
		m_CPEArray[i]->GetRT(m_uiRTWidth, m_uiRTHeight);
	}
}
void VSCombinePostEffectSceneRender::DisableRT()
{
	VSCommonPESceneRender::DisableRT();
	m_CPEArray[m_CPEArray.GetNum() - 1]->DisableRT();
}
bool VSCombinePostEffectSceneRender::AddRenderTarget(VSRenderTarget * pTarget)
{
	m_CPEArray[m_CPEArray.GetNum() - 1]->AddRenderTarget(pTarget);
	for (unsigned int i = 0; i < m_CPEArray.GetNum() - 1; i++)
	{
		m_CPEArray[i]->GetRT(pTarget->GetWidth(), pTarget->GetHeight());
	}
	return true;
}
VSRenderTarget * VSCombinePostEffectSceneRender::GetRenderTarget(unsigned int uiIndex)
{
	return m_CPEArray[m_CPEArray.GetNum() - 1]->GetRenderTarget(uiIndex);
}



// VSPESSRSceneRender::VSPESSRSceneRender()
// {
// 	m_pCustomMaterial = VSCustomMaterial::GetSSR();
// 	m_pNormalDepthTexture = VS_NEW VSTexAllState();
// 	m_pNormalDepthTexture->SetSamplerState((VSSamplerState*)VSSamplerState::GetTwoLineBorderOne());
// 	m_pSceneColorTexture = VS_NEW VSTexAllState();
// 	m_pSceneColorTexture->SetSamplerState((VSSamplerState*)VSSamplerState::GetTwoLineBorderZero());
// };
// VSPESSRSceneRender::~VSPESSRSceneRender()
// {
// 	m_pNormalDepthTexture = NULL;
// 	m_pSceneColorTexture = NULL;
// };
// void VSPESSRSceneRender::SetUseTexture(VSTexture * pNormalDepthTexture, VSTexture *pSceneColorTexture)
// {
// 	m_pNormalDepthTexture->m_pTex = pNormalDepthTexture;
// 	m_pSceneColorTexture->m_pTex = pSceneColorTexture;
// 
// 	static VSUsedName NormalDepthSamplerString = _T("NormalDepthSampler");
// 	static VSUsedName SceneColorSamplerString = _T("SceneColorSampler");
// 
// 	m_pCustomMaterial->SetPShaderTexture(NormalDepthSamplerString,m_pNormalDepthTexture);
// 	m_pCustomMaterial->SetPShaderTexture(SceneColorSamplerString,m_pSceneColorTexture);
// }
// bool VSPESSRSceneRender::OnDraw(VSCuller & Culler, double dAppTime)
// {
// 	VSMAC_ASSERT(m_pCustomMaterial);
// 
// 	if (!m_uiRTWidth || !m_uiRTHeight)
// 	{
// 		return false;
// 	}
// 
// 	VSREAL Inv_Width = 1.0f / m_uiRTWidth;
// 	VSREAL Inv_Height = 1.0f / m_uiRTHeight;
// 	m_pCustomMaterial->SetVShaderValue(VSUsedName::ms_cPostInv_Width,&Inv_Width);
// 	m_pCustomMaterial->SetVShaderValue(VSUsedName::ms_cPostInv_Height,&Inv_Height);
// 	
// 	static VSUsedName ProjectMatrixString = _T("ProjectMatrix");
// 	VSMatrix3X3W ProjectMatrix = Culler.GetCamera()->GetProjMatrix();
// 	m_pCustomMaterial->SetPShaderValue(ProjectMatrixString,&ProjectMatrix);
// 
// 
// 
// 	static VSUsedName InvRtWidthString = _T("InvRtWidth");
// 	unsigned int uiWidth = m_pNormalDepthTexture->m_pTex->GetWidth(0);
// 	unsigned int uiHeight = m_pNormalDepthTexture->m_pTex->GetHeight(0);
// 	VSVector2 InvRtWidth = VSVector2(1.0f / uiWidth , 1.0f / uiHeight);
// 	m_pCustomMaterial->SetPShaderValue(InvRtWidthString, &InvRtWidth);
// 
// 
// 	static VSUsedName FarZString = _T("FarZ");
// 	VSREAL FarZ = Culler.GetCamera()->GetZFar();
// 	m_pCustomMaterial->SetPShaderValue(FarZString, &FarZ);
// 
// 	if(!m_pCustomMaterial->UseMaterial())
// 		return false;
// 	VSRenderer::ms_pRenderer->DrawScreen(NULL);
// 
// 	m_pNormalDepthTexture->m_pTex = NULL;
// 	m_pSceneColorTexture->m_pTex = NULL;
// 	return true;
// }

VSPEMaterialSceneRender::VSPEMaterialSceneRender()
{


};
VSPEMaterialSceneRender::~VSPEMaterialSceneRender()
{

};
bool VSPEMaterialSceneRender::OnDraw(VSCuller & Culler, double dAppTime)
{
	VSMaterial * pMaterial = m_pMaterialInstacne->GetMaterial();
	VSPostEffectPass * pPostEffectPass = pMaterial->GetPostEffectPass();
	pPostEffectPass->SetDrawContent(m_pMaterialInstacne, VSGeometry::GetDefaultQuad(),NULL, 0);
	pPostEffectPass->m_uiRTWidth = m_uiRTWidth;
	pPostEffectPass->m_uiRTHeight = m_uiRTHeight;
	pPostEffectPass->m_PColorBuffer = m_pTexAllState[0];
	pPostEffectPass->Draw(VSRenderer::ms_pRenderer);
	return true;
}
VSPEVolumeShadowMapSceneRender::VSPEVolumeShadowMapSceneRender()
{
	m_pCustomMaterial = VSCustomMaterial::GetPostVolumeShadowMap();

};
VSPEVolumeShadowMapSceneRender::~VSPEVolumeShadowMapSceneRender()
{
	
};


VSShadowMapSceneRender::VSShadowMapSceneRender(unsigned int uiShadowMapType)
{

	m_uiShadowMapType = uiShadowMapType;
}
VSShadowMapSceneRender::~VSShadowMapSceneRender()
{

}
bool VSShadowMapSceneRender::Draw(VSCuller & Culler,double dAppTime)
{
	if (m_uiClearFlag <= VSRenderer::CF_USE_ALL)
	{

		VSColorRGBA ClearColorRGBA = VSRenderer::ms_pRenderer->GetClearColor();
		VSREAL fClearDepth = VSRenderer::ms_pRenderer->GetClearDepth();
		unsigned int uiClearStencil = VSRenderer::ms_pRenderer->GetClearStencil();


		VSRenderer::ms_pRenderer->SetClearColor(m_ClearColorRGBA);
		VSRenderer::ms_pRenderer->SetClearDepth(m_fClearDepth);
		VSRenderer::ms_pRenderer->SetClearStencil(m_uiClearStencil);
	
		SetRenderTargets();
		VSRenderer::ms_pRenderer->SetViewPort(NULL);
		VSRenderer::ms_pRenderer->ClearBuffers(m_uiClearFlag & VSRenderer::CF_COLOR);
		for (unsigned int uiRenderGroup = 0 ; uiRenderGroup < VSCuller::RG_MAX ;uiRenderGroup++)
		{
			if (!Culler.GetRenderGroupVisibleNum(uiRenderGroup))
			{
				continue;
			}
			if (m_pDepthStencil[uiRenderGroup])
			{
				VSRenderer::ms_pRenderer->SetDepthStencilBuffer(m_pDepthStencil[uiRenderGroup]);
				VSRenderer::ms_pRenderer->ClearBuffers((m_uiClearFlag & VSRenderer::CF_DEPTH) | (m_uiClearFlag & VSRenderer::CF_STENCIL));									
				OnDraw(Culler,uiRenderGroup,dAppTime);
				VSRenderer::ms_pRenderer->EndDepthStencilBuffer(m_pDepthStencil[uiRenderGroup]);
			}
			else
			{
				VSRenderer::ms_pRenderer->ClearBuffers((m_uiClearFlag & VSRenderer::CF_DEPTH) | (m_uiClearFlag & VSRenderer::CF_STENCIL));									
				OnDraw(Culler,uiRenderGroup,dAppTime);
			}
		}
		EndRenderTargets();
 
		VSRenderer::ms_pRenderer->SetClearColor(ClearColorRGBA);
		VSRenderer::ms_pRenderer->SetClearDepth(fClearDepth);
		VSRenderer::ms_pRenderer->SetClearStencil(uiClearStencil);
	}

	return true;
}
bool VSShadowMapSceneRender::OnDraw(VSCuller & Culler,unsigned int uiRenderGroup,double dAppTime)
{
	
	if (uiRenderGroup != VSCuller::RG_NORMAL)
	{
		return true;
	}
	if(m_uiShadowMapType == SMT_CUB)
	{
		for (unsigned int t = 0 ; t < VSCuller::VST_MAX ; t++)
		{
			for(unsigned int j = 0; j < Culler.GetVisibleNum(t,uiRenderGroup) ; j++)
			{

				VSRenderContext& VisibleContext = Culler.GetVisibleSpatial(j,t,uiRenderGroup);
				if (!VisibleContext.IsCanDraw())
				{
					continue;
				}

				VSMaterialInstance * pMaterialInstance = VisibleContext.m_pMaterialInstance;

				VSCubeShadowPass * pShadowPass = pMaterialInstance->GetMaterial()->GetCubeShadowPass();

				pShadowPass->SetDrawContent(pMaterialInstance, VisibleContext.m_pGeometry, Culler.GetCamera(), m_pLocalLight,VisibleContext.m_uiPassId);

				pShadowPass->Draw(VSRenderer::ms_pRenderer);

			}
		}
	}
	else if (m_uiShadowMapType == SMT_SHADOWMAP)
	{
		for (unsigned int t = 0 ; t < VSCuller::VST_MAX ; t++)
		{
			for(unsigned int j = 0; j < Culler.GetVisibleNum(t,uiRenderGroup) ; j++)
			{

				VSRenderContext& VisibleContext = Culler.GetVisibleSpatial(j,t,uiRenderGroup);
				if (!VisibleContext.IsCanDraw())
				{
					continue;
				}

				VSMaterialInstance * pMaterialInstance = VisibleContext.m_pMaterialInstance;

				VSShadowPass * pShadowPass = pMaterialInstance->GetMaterial()->GetShadowMapPass();

				pShadowPass->SetDrawContent(pMaterialInstance, VisibleContext.m_pGeometry, 
					Culler.GetCamera(), m_pLocalLight, VisibleContext.m_uiPassId);
				pShadowPass->Draw(VSRenderer::ms_pRenderer);

			}
		}
	}
	else if (m_uiShadowMapType == SMT_DUAL_PARABOLOID)
	{
		for (unsigned int t = 0 ; t < VSCuller::VST_MAX ; t++)
		{
			for(unsigned int j = 0; j < Culler.GetVisibleNum(t,uiRenderGroup) ; j++)
			{

				VSRenderContext& VisibleContext = Culler.GetVisibleSpatial(j,t,uiRenderGroup);
				if (!VisibleContext.IsCanDraw())
				{
					continue;
				}

				VSMaterialInstance * pMaterialInstance = VisibleContext.m_pMaterialInstance;

				VSDualParaboloidShadowPass * pShadowPass = pMaterialInstance->GetMaterial()->GetDualParaboloidShadowPass();

				pShadowPass->SetDrawContent(pMaterialInstance, VisibleContext.m_pGeometry,
					Culler.GetCamera(), m_pLocalLight, VisibleContext.m_uiPassId);
				pShadowPass->Draw(VSRenderer::ms_pRenderer);

			}
		}
	}

	return true;
}
VSLightMaterialSceneRender::VSLightMaterialSceneRender()
{

}
VSLightMaterialSceneRender::~VSLightMaterialSceneRender()
{

}
void VSLightMaterialSceneRender::ClearRTAndDepth()
{
	m_pTargetArray.Clear();
	m_pDepthStencil = NULL;
	m_uiRTWidth = 0;
	m_uiRTHeight = 0;
	m_uiMulSample = 0;
}
bool VSLightMaterialSceneRender::Draw(VSCuller & Culler,double dAppTime)
{
	SetRenderTargets();

	if (m_pDepthStencil)
	{
		if(!VSRenderer::ms_pRenderer->SetDepthStencilBuffer(m_pDepthStencil))
		{
			VSMAC_ASSERT(0);
			return false;
		}
	}

	if (m_uiClearFlag > 0 && m_uiClearFlag <= VSRenderer::CF_USE_ALL)
	{

		VSColorRGBA ClearColorRGBA = VSRenderer::ms_pRenderer->GetClearColor();
		VSREAL fClearDepth = VSRenderer::ms_pRenderer->GetClearDepth();
		unsigned int uiClearStencil = VSRenderer::ms_pRenderer->GetClearStencil();


		unsigned int uiSaveRenderStateInheritFlag = VSRenderer::ms_pRenderer->GetRenderStateInheritFlag();

		VSRenderer::ms_pRenderer->SetClearColor(m_ClearColorRGBA);
		VSRenderer::ms_pRenderer->SetClearDepth(m_fClearDepth);
		VSRenderer::ms_pRenderer->SetClearStencil(m_uiClearStencil);

		VSRenderer::ms_pRenderer->SetViewPort(NULL);
		VSRenderer::ms_pRenderer->ClearBuffers(m_uiClearFlag);
		OnDraw(Culler,dAppTime);



		VSRenderer::ms_pRenderer->SetClearColor(ClearColorRGBA);
		VSRenderer::ms_pRenderer->SetClearDepth(fClearDepth);
		VSRenderer::ms_pRenderer->SetClearStencil(uiClearStencil);
	}


	if (m_pDepthStencil)
	{
		if(!VSRenderer::ms_pRenderer->EndDepthStencilBuffer(m_pDepthStencil))
		{
			VSMAC_ASSERT(0);
			return false;
		}
	}
	EndRenderTargets();
	return true;
}
bool VSLightMaterialSceneRender::OnDraw(VSCuller & Culler, double dAppTime)
{

	if (!m_pTargetArray.GetNum())
	{
		return false;
	}
	if (!m_pTargetArray[0])
	{
		return false;
	}
	VSMaterial * pMaterial = m_pMaterialInstacne->GetMaterial();
	VSLightFunPass * pLightFunPass = pMaterial->GetLightFunPass();
	pLightFunPass->SetDrawContent(m_pMaterialInstacne, VSGeometry::GetDefaultQuad(),
		NULL, m_pLight, 0);
	pLightFunPass->Draw(VSRenderer::ms_pRenderer);

	return true;
}
bool VSLightMaterialSceneRender::SetDepthStencil(VSDepthStencil * pDepthStencil)
{
	if (!pDepthStencil)
	{
		return false;
	}
	if (!pDepthStencil->GetWidth() || !pDepthStencil->GetHeight())
	{
		return false;
	}
	if (m_uiRTHeight && m_uiRTWidth)
	{
		if (m_uiRTWidth != pDepthStencil->GetWidth() || m_uiRTHeight != pDepthStencil->GetHeight())
		{
			return false;
		}
	}
	else
	{	
		m_uiRTWidth = pDepthStencil->GetWidth();
		m_uiRTHeight = pDepthStencil->GetHeight();
	}
	m_pDepthStencil = pDepthStencil;
	return true;
}
VSProjectShadowSceneRender::VSProjectShadowSceneRender()
{
	m_pTexAllState = VS_NEW VSTexAllState();
	m_pTexAllState->SetSamplerState((VSSamplerState*)VSSamplerState::GetTwoLine());
	m_pNormalDepthTexture = NULL;
	static VSUsedName LightTypeString = _T("LIGHT_TYPE");
	m_DirectionLightShaderKey.SetTheKey(LightTypeString, 0);
	m_PointLightShaderKey.SetTheKey(LightTypeString, 1);
	m_SpotLightShaderKey.SetTheKey(LightTypeString, 2);
}
VSProjectShadowSceneRender::~VSProjectShadowSceneRender()
{

}
bool VSProjectShadowSceneRender::Draw(VSCuller & Culler,double dAppTime)
{
	if (m_uiClearFlag <= VSRenderer::CF_USE_ALL)
	{

		VSColorRGBA ClearColorRGBA = VSRenderer::ms_pRenderer->GetClearColor();
		VSREAL fClearDepth = VSRenderer::ms_pRenderer->GetClearDepth();
		unsigned int uiClearStencil = VSRenderer::ms_pRenderer->GetClearStencil();



		VSRenderer::ms_pRenderer->SetClearColor(m_ClearColorRGBA);
		VSRenderer::ms_pRenderer->SetClearDepth(m_fClearDepth);
		VSRenderer::ms_pRenderer->SetClearStencil(m_uiClearStencil);

		VSRenderer::ms_pRenderer->SetViewPort(NULL);
		VSRenderer::ms_pRenderer->ClearBuffers(m_uiClearFlag);
		OnDraw(Culler, 0, dAppTime);

		VSRenderer::ms_pRenderer->SetClearColor(ClearColorRGBA);
		VSRenderer::ms_pRenderer->SetClearDepth(fClearDepth);
		VSRenderer::ms_pRenderer->SetClearStencil(uiClearStencil);

	}
	return true;
}
bool VSProjectShadowSceneRender::OnDraw(VSCuller & Culler,unsigned int uiRenderGroup,double dAppTime)
{
		VSCamera * pCamera = Culler.GetCamera();
		VSVShader * pVShader = NULL;
		VSPShader * pPShader = NULL;
		{
			VSCustomMaterial * pCustomMaterial = VSCustomMaterial::GetProjectShadowPre();

			
			pVShader = pCustomMaterial->GetCurVShader(*VSShaderKey::ms_NULLShaderKey);
			pPShader = pCustomMaterial->GetCurPShader(*VSShaderKey::ms_NULLShaderKey);

			
			//vs


			VSMatrix3X3W ProjectShadowMatrix = m_pLocalLight->m_LightShadowMatrix.GetInverse();
			pVShader->SetParam(VSUsedName::ms_cProjectShadowMatrix, &ProjectShadowMatrix);

			VSMatrix3X3W WorldViewProjectMatrix = pCamera->GetViewMatrix() * pCamera->GetProjMatrix();
			pVShader->SetParam(VSUsedName::ms_cWorldViewProjectMatrix,&WorldViewProjectMatrix);

			if(!VSRenderer::ms_pRenderer->DrawMesh(VSGeometry::GetDefaultCube(),&pCustomMaterial->GetRenderState(),pVShader,pPShader,
				NULL,NULL,NULL))
			{
				return false;
			}
		}

		{
			VSCustomMaterial * pCustomMaterial = VSCustomMaterial::GetProjectShadow();
			pVShader = pCustomMaterial->GetCurVShader(*VSShaderKey::ms_NULLShaderKey);

			if (m_pLocalLight->GetLightType() == VSLight::LT_POINT)
			{
				pPShader = pCustomMaterial->GetCurPShader(m_PointLightShaderKey);
			}
			else if (m_pLocalLight->GetLightType() == VSLight::LT_SPOT)
			{
				pPShader = pCustomMaterial->GetCurPShader(m_SpotLightShaderKey);
			}
			else
			{				
				pPShader = pCustomMaterial->GetCurPShader(m_DirectionLightShaderKey);
			}
			

			m_pTexAllState->SetTexture(m_pNormalDepthTexture);

			//VS


			VSMatrix3X3W ProjectShadowMatrix = m_pLocalLight->m_LightShadowMatrix.GetInverse();
			pVShader->SetParam(VSUsedName::ms_cProjectShadowMatrix, &ProjectShadowMatrix);

			VSMatrix3X3W WorldViewProjectMatrix = pCamera->GetViewMatrix() * pCamera->GetProjMatrix();
			pVShader->SetParam(VSUsedName::ms_cWorldViewProjectMatrix,&WorldViewProjectMatrix);

			//ps
			unsigned int uiRTWidth = m_pLocalLight->GetShadowResolution();
			VSREAL fOffsetX = 0.5f + (0.5f / (VSREAL)(uiRTWidth));
			VSREAL fOffsetY = 0.5f + (0.5f / (VSREAL)(uiRTWidth));
			VSMatrix3X3W texScaleBiasMat(	0.5f,     0.0f,     0.0f,       0.0f,
				0.0f,    -0.5f,     0.0f,       0.0f,
				0.0f,     0.0f,     1.0f,		0.0f,
				fOffsetX, fOffsetY, 0.0f,       1.0f );
			static VSUsedName ShadowMatrixString = _T("ShadowMatrix");
			VSMatrix3X3W ShadowMatrix = m_pLocalLight->m_LightShadowMatrix * texScaleBiasMat;
			pPShader->SetParam(ShadowMatrixString, &ShadowMatrix);


			static VSUsedName InvViewString = _T("InvView");
			VSMatrix3X3W InvView = pCamera->GetViewMatrix().GetInverse();
			pPShader->SetParam(InvViewString, &InvView);


			static VSUsedName ProjectString = _T("Project");
			VSVector2 Project = VSVector2(pCamera->GetProjMatrix()._00,pCamera->GetProjMatrix()._11);
			pPShader->SetParam(ProjectString, &Project);


			static VSUsedName FarZString = _T("FarZ");
			VSREAL FarZ = pCamera->GetZFar();
			pPShader->SetParam(FarZString, &FarZ);

			static VSUsedName ZBiasString = _T("ZBias");
			VSREAL ZBias = m_pLocalLight->m_ZBias;
			pPShader->SetParam(ZBiasString, &ZBias);

			static VSUsedName NormalDepthInvRtWidthString = _T("NormalDepthInvRtWidth");
			VSVector2 NormalDepthInvRtWidth = VSVector2(1.0f / m_pTexAllState->GetTexture()->GetWidth(0) * 1.0f,1.0f / m_pTexAllState->GetTexture()->GetHeight(0) * 1.0f);
			pPShader->SetParam(NormalDepthInvRtWidthString, &NormalDepthInvRtWidth);

			static VSUsedName ShadowRtWidthString = _T("ShadowRtWidth");
			VSREAL ShadowRtWidth = m_pLocalLight->GetShadowResolution() * 1.0f;
			pPShader->SetParam(ShadowRtWidthString, &ShadowRtWidth);

			static VSUsedName ShadowSamplerString = _T("ShadowTexture");
			pPShader->SetParam(ShadowSamplerString,m_pLocalLight->GetShadowTexture());

			static VSUsedName NormalDepthSamplerString = _T("NormalDepthTexture");
			pPShader->SetParam(NormalDepthSamplerString,m_pTexAllState);

			static VSUsedName ProjectShadowColorString = _T("ProjectShadowColor");
			pPShader->SetParam(ProjectShadowColorString,&m_pLocalLight->m_ProjectShadowColor);

			static VSUsedName ProjectShadowFallOffString = _T("ProjectShadowFallOff");
			pPShader->SetParam(ProjectShadowFallOffString, &m_pLocalLight->m_ProjectShadowFallOff);
			if (m_pLocalLight->GetLightType() == VSLight::LT_POINT || m_pLocalLight->GetLightType() == VSLight::LT_SPOT)
			{
				static VSUsedName LightPosString = _T("LightWorldPos");
				VSVector3 LightPos = m_pLocalLight->GetWorldTranslate();
				pPShader->SetParam(LightPosString, &LightPos);

				static VSUsedName LightRangeString = _T("LightRange");
				pPShader->SetParam(LightRangeString, &m_fLightRange);

				if (m_pLocalLight->GetLightType() == VSLight::LT_SPOT)
				{
					static VSUsedName LightWorldDirectionString = _T("LightWorldDirection");
					pPShader->SetParam(LightWorldDirectionString, &m_LightWorldDirection);

					static VSUsedName FalloffString = _T("Falloff");
					pPShader->SetParam(FalloffString, &m_Falloff);

					static VSUsedName ThetaString = _T("Theta");
					pPShader->SetParam(ThetaString, &m_Theta);

					static VSUsedName PhiString = _T("Phi");
					pPShader->SetParam(PhiString, &m_Phi);
				}
			}
			

			if (!VSRenderer::ms_pRenderer->DrawMesh(VSGeometry::GetDefaultCube(), &pCustomMaterial->GetRenderState(), pVShader, pPShader, 
					NULL, NULL, NULL))
			{
				return false;
			}
		}
			
			
	return true;
}
VSVolumeShadowSceneRender::VSVolumeShadowSceneRender()
{

}

VSVolumeShadowSceneRender::~VSVolumeShadowSceneRender()
{

}
bool VSVolumeShadowSceneRender::Draw(VSCuller & Culler, double dAppTime)
{
	if (m_uiClearFlag <= VSRenderer::CF_USE_ALL)
	{

		VSColorRGBA ClearColorRGBA = VSRenderer::ms_pRenderer->GetClearColor();
		VSREAL fClearDepth = VSRenderer::ms_pRenderer->GetClearDepth();
		unsigned int uiClearStencil = VSRenderer::ms_pRenderer->GetClearStencil();



		VSRenderer::ms_pRenderer->SetClearColor(m_ClearColorRGBA);
		VSRenderer::ms_pRenderer->SetClearDepth(m_fClearDepth);
		VSRenderer::ms_pRenderer->SetClearStencil(m_uiClearStencil);

		VSRenderer::ms_pRenderer->SetViewPort(NULL);
		VSRenderer::ms_pRenderer->ClearBuffers(m_uiClearFlag);
		OnDraw(Culler, VSCuller::RG_NORMAL, dAppTime);

		VSRenderer::ms_pRenderer->SetClearColor(ClearColorRGBA);
		VSRenderer::ms_pRenderer->SetClearDepth(fClearDepth);
		VSRenderer::ms_pRenderer->SetClearStencil(uiClearStencil);

	}
	return true;
}
bool VSVolumeShadowSceneRender::OnDraw(VSCuller & Culler, unsigned int uiRenderGroup, double dAppTime)
{
	for (unsigned int t = 0; t < VSCuller::VST_MAX; t++)
	{
		for (unsigned int j = 0; j < Culler.GetVisibleNum(t, uiRenderGroup); j++)
		{

			VSRenderContext& VisibleContext = Culler.GetVisibleSpatial(j, t, uiRenderGroup);
			if (!VisibleContext.IsCanDraw())
			{
				continue;
			}

			VSMaterialInstance * pMaterialInstance = VisibleContext.m_pMaterialInstance;

			VSVolumeShadowPass * pShadowPass = pMaterialInstance->GetMaterial()->GetVolumeShadowPass();

			pShadowPass->SetDrawContent(pMaterialInstance, VisibleContext.m_pGeometry,
				Culler.GetCamera(), m_pLocalLight, VisibleContext.m_uiPassId);

			pShadowPass->Draw(VSRenderer::ms_pRenderer);


		}
	}
	return true;
}
COMMON_POST_EFFECT_SCENE_RENDER_IMPLEMENT_BEGIN(Gray)
POST_EFFECT_SCENE_RENDER_END

COMMON_POST_EFFECT_SCENE_RENDER_IMPLEMENT_BEGIN(GammaCorrect)
POST_EFFECT_SCENE_RENDER_END

COMMON_POST_EFFECT_SCENE_RENDER_IMPLEMENT_BEGIN(ScreenQuad)
POST_EFFECT_SCENE_RENDER_END

COMMON_POST_EFFECT_SCENE_RENDER_IMPLEMENT_BEGIN(Saturate)
COMMON_POST_EFFECT_SCENE_RENDER_IMPLEMENT_VALUE(Saturation)
POST_EFFECT_SCENE_RENDER_END

COMMON_POST_EFFECT_SCENE_RENDER_IMPLEMENT_BEGIN(OldPhoto)
POST_EFFECT_SCENE_RENDER_END

COMMON_POST_EFFECT_SCENE_RENDER_IMPLEMENT_BEGIN(GuassBlurH)
float Size = m_uiRTWidth * 1.0f;
COMMON_POST_EFFECT_SCENE_RENDER_IMPLEMENT_VALUE(Size)
POST_EFFECT_SCENE_RENDER_END

COMMON_POST_EFFECT_SCENE_RENDER_IMPLEMENT_BEGIN(GuassBlurV)
float Size = m_uiRTHeight * 1.0f;
COMMON_POST_EFFECT_SCENE_RENDER_IMPLEMENT_VALUE(Size)
POST_EFFECT_SCENE_RENDER_END

COMMON_POST_EFFECT_SCENE_RENDER_IMPLEMENT_BEGIN(Filter3X3)
VSVector2 Size(m_uiRTWidth * 1.0f, m_uiRTHeight * 1.0f);
COMMON_POST_EFFECT_SCENE_RENDER_IMPLEMENT_VALUE(Size)
POST_EFFECT_SCENE_RENDER_END

COMMON_POST_EFFECT_SCENE_RENDER_IMPLEMENT_BEGIN(Bright)
COMMON_POST_EFFECT_SCENE_RENDER_IMPLEMENT_VALUE(MiddleGray)
COMMON_POST_EFFECT_SCENE_RENDER_IMPLEMENT_VALUE(WhiteCutoff)
COMMON_POST_EFFECT_SCENE_RENDER_IMPLEMENT_VALUE(Luminance)
POST_EFFECT_SCENE_RENDER_END

COMBINE_POST_EFFECT_SCENE_RENDER_IMPLEMENT_BEGIN(GuassBlur)
POST_EFFECT_SCENE_RENDER_END

COMMON_POST_EFFECT_SCENE_RENDER_IMPLEMENT_BEGIN(TexAdd)
POST_EFFECT_SCENE_RENDER_END

COMBINE_POST_EFFECT_SCENE_RENDER_IMPLEMENT_BEGIN(Bloom)
if (uiPassID == 3)
{
	GetVSPETexAddSceneRender()->SetSourceTarget(GetInputTexture(0), 1);
}
else if (uiPassID == 1)
{
	GetVSPEBloomBrightSceneRender()->MiddleGray = MiddleGray;
	GetVSPEBloomBrightSceneRender()->WhiteCutoff = WhiteCutoff;
	GetVSPEBloomBrightSceneRender()->Luminance = Luminance;
}
POST_EFFECT_SCENE_RENDER_END