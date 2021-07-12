#include "VSPointLight.h"
#include "VSGraphicInclude.h"
#include "VSSceneRender.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSPointLight,VSLocalLight)
BEGIN_ADD_PROPERTY(VSPointLight,VSLocalLight)
REGISTER_PROPERTY(m_Range, Range, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_WorldRenderBV, WorldRenderBV, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_ENUM_PROPERTY(m_uiShadowType, ShadowType, PointLightShadowType, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
BEGIN_ADD_ENUM
ADD_ENUM(PointLightShadowType, ST_CUB)
ADD_ENUM(PointLightShadowType, ST_VOLUME)
ADD_ENUM(PointLightShadowType, ST_DUAL_PARABOLOID)
ADD_ENUM(PointLightShadowType, ST_PROJECT)
END_ADD_ENUM
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSPointLight)
IMPLEMENT_INITIAL_END
VSPointLight::VSPointLight()

{

	m_Range = 1000.0f;
	m_uiRTWidth = 1024;
	SetShadowType(ST_CUB);
	m_ZBias = 0.001f;
	m_LightFunScale = VSVector2(0.001f,0.001f);
	m_LightFunOffset = VSVector2(0.0f,0.0f);
	m_ProjectShadowColor = VSColorRGBA(0.0f, 0.0f, 0.0f, 1.0f);
}
VSPointLight::~VSPointLight()
{

}
void VSPointLight::GetLightRange()
{	
	
	VSVector3 Point3 = GetWorldTranslate();
	m_WorldRenderBV = VSSphere3(Point3, m_Range);
	
}
bool VSPointLight::IsRelative(VSGeometry * pGeometry)
{
	if (!VSLocalLight::IsRelative(pGeometry))
	{
		return false;
	}
	VSAABB3 GeometryAABB = pGeometry->GetWorldAABB();
	if (GeometryAABB.RelationWith(m_WorldRenderBV) == VSNOINTERSECT)
	{
		return false;
	}
	return true;
}
void VSPointLight::ResetShadow()
{
	m_pShadowTexture.Clear();
	for (unsigned int i = 0 ; i < VSCubeTexture::F_MAX ; i++)
	{
		m_pCubRenderTarget[i] = NULL;
		m_ShadowCuller[i].ClearAll();
	}
	m_VolumeShadowCuller.ClearAll();
	m_pVolumeShadowFirstPassRenderTarget = NULL;
	m_pPEVolumeSMSceneRender = NULL;
	m_pVolumeShadowRenderTarget = NULL;

	m_pShadowMapSceneRender = NULL;

	m_pProjectShadowSceneRender = NULL;
}
void VSPointLight::SetShadowType(unsigned int uiShadowType)
{

	ResetShadow();
	if (uiShadowType == ST_VOLUME)
	{
		
		m_pShadowTexture.AddElement(VS_NEW VSTexAllState());

		m_pShadowTexture[0]->SetSamplerState((VSSamplerState*)VSSamplerState::GetTwoLine());

		m_pPEVolumeSMSceneRender = VS_NEW VSPEVolumeShadowMapSceneRender();
		m_pPEVolumeSMSceneRender->SetParam(VSRenderer::CF_COLOR,VSColorRGBA(1.0f,1.0f,1.0f,1.0f),1.0f,0);
		
		m_pVolumeShadowSceneRender = VS_NEW VSVolumeShadowSceneRender();
		m_pVolumeShadowSceneRender->m_pLocalLight = this;
		m_pVolumeShadowSceneRender->SetParam(VSRenderer::CF_STENCIL, VSColorRGBA(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 15);
		
	}
	else if (uiShadowType == ST_CUB)
	{
		

		m_pShadowTexture.AddElement(VS_NEW VSTexAllState());

		m_pShadowTexture[0]->SetTexture(VS_NEW VSCubeTexture(m_uiRTWidth,VSRenderer::SFT_R16F, 1, true));
		m_pShadowTexture[0]->SetSamplerState((VSSamplerState*)VSSamplerState::GetTwoLineBorderOne());
		for (unsigned int i = 0 ; i < VSCubeTexture::F_MAX ; i++)
		{
			m_pCubRenderTarget[i] = VSResourceManager::CreateRenderTarget(StaticCast<VS2DTextureArray>(m_pShadowTexture[0]->GetTexture()),0,0,i);
		}
		
		m_pShadowMapSceneRender = VS_NEW VSShadowMapSceneRender(VSShadowMapSceneRender::SMT_CUB);
		m_pShadowMapSceneRender->m_pLocalLight = this;
		m_pShadowMapSceneRender->SetParam(VSRenderer::CF_USE_ALL, VSColorRGBA(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0);
	}
	else if (uiShadowType == ST_DUAL_PARABOLOID)
	{
	
		m_pShadowTexture.AddElement(VS_NEW VSTexAllState());
		m_pShadowTexture.AddElement(VS_NEW VSTexAllState());
		m_pShadowTexture[0]->SetSamplerState((VSSamplerState*)VSSamplerState::GetTwoLineBorderOne());
		m_pShadowTexture[1]->SetSamplerState((VSSamplerState*)VSSamplerState::GetTwoLineBorderOne());
		m_pShadowMapSceneRender = VS_NEW VSShadowMapSceneRender(VSShadowMapSceneRender::SMT_DUAL_PARABOLOID);
		m_pShadowMapSceneRender->m_pLocalLight = this;
		m_pShadowMapSceneRender->SetParam(VSRenderer::CF_USE_ALL, VSColorRGBA(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0);
	}
	else if (uiShadowType == ST_PROJECT)
	{
		m_pShadowTexture.AddElement(VS_NEW VSTexAllState());
		m_pShadowTexture[0]->SetSamplerState((VSSamplerState*)VSSamplerState::GetTwoLineBorderOne());
		m_pShadowMapSceneRender = VS_NEW VSShadowMapSceneRender(VSShadowMapSceneRender::SMT_SHADOWMAP);
		m_pShadowMapSceneRender->m_pLocalLight = this;
		m_pShadowMapSceneRender->SetParam(VSRenderer::CF_USE_ALL, VSColorRGBA(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0);
		m_pProjectShadowSceneRender = VS_NEW VSProjectShadowSceneRender();
		m_pProjectShadowSceneRender->m_pLocalLight = this;
		m_pProjectShadowSceneRender->SetParam(VSRenderer::CF_STENCIL, VSColorRGBA(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 15);
	}
	m_uiShadowType = uiShadowType;

}
bool VSPointLight::PostClone(VSObject * pObjectSrc)
{
	VSLight::PostClone(pObjectSrc);

	SetShadowType(m_uiShadowType);
	return true;
}
bool VSPointLight::PostLoad(VSStream* pStream)
{
	VSLight::PostLoad(pStream);

	SetShadowType(m_uiShadowType);
	return true;
}
void VSPointLight::DrawNoDependenceShadowMap(VSCuller & CurCuller, double dAppTime)
{
	if (m_bEnable && m_bIsCastShadow)
	{
		if (m_bShadowMapDrawEnd == false)
		{
			m_bShadowMapDrawEnd = true;
		}
		else
		{
			return;
		}
		if (m_uiShadowType == ST_CUB)
		{
			if (CurCuller.GetCamera()->GetFov() > AngleToRadian(90.0f))
			{
				DrawNormalCubShadow(CurCuller, dAppTime);
			}

		}
		else if (m_uiShadowType == ST_DUAL_PARABOLOID)
		{
			DrawDualParaboloidShadow(CurCuller, dAppTime);
		}
	}
}
void VSPointLight::DisableNoDependenceShadowMap(double dAppTime)
{
	VSLocalLight::DisableNoDependenceShadowMap(dAppTime);
	if (m_uiShadowType == ST_DUAL_PARABOLOID)
	{
		if (m_pDualParaboloidRenderTarget0)
		{
			VSResourceManager::DisableOutputResource(m_pDualParaboloidRenderTarget0);
		}

		if (m_pDualParaboloidRenderTarget1)
		{
			VSResourceManager::DisableOutputResource(m_pDualParaboloidRenderTarget1);
		}
		for (unsigned int i = 0; i < m_pShadowTexture.GetNum(); i++)
		{
			m_pShadowTexture[i]->SetTexture(NULL);
		}
	}
}
void VSPointLight::DisableDependenceShadowMap(VSCuller & CurCuller, double dAppTime)
{
	if (m_uiShadowType == ST_VOLUME || m_uiShadowType == ST_PROJECT)
	{
		if (m_pVolumeShadowRenderTarget)
		{
			VSResourceManager::DisableOutputResource(m_pVolumeShadowRenderTarget);
		}
		for (unsigned int i = 0 ; i < m_pShadowTexture.GetNum() ; i++)
		{
			m_pShadowTexture[i]->SetTexture(NULL);
		}
	}
}
void VSPointLight::DrawNormalCubShadow(VSCuller & CurCuller,double dAppTime)
{
	VSMatrix3X3 MatTemp[VSCubeTexture::F_MAX] = {VSMatrix3X3::ms_CameraViewRight,VSMatrix3X3::ms_CameraViewLeft,VSMatrix3X3::ms_CameraViewUp,VSMatrix3X3::ms_CameraViewDown,VSMatrix3X3::ms_CameraViewFront,VSMatrix3X3::ms_CameraViewBack};

	
	VSCamera CubCamera[VSCubeTexture::F_MAX];

	VSVector3 WorldPos = GetWorldTranslate();

	if (m_pScene.GetNum() > 0)
	{
		for (unsigned int Index = 0 ; Index < VSCubeTexture::F_MAX ;Index++)
		{
			CubCamera[Index].CreateFromEuler(WorldPos, 0.0f, 0.0f, 0.0f);
			CubCamera[Index].SetLocalRotate(MatTemp[Index]);
			CubCamera[Index].SetPerspectiveFov(AngleToRadian(90.0f), 1.0f, 1.0f, m_Range);
			m_ShadowCuller[Index].GetSceneContent(CubCamera[Index], m_pScene, this, dAppTime);
		}
	}

	VSDepthStencil * pDepthStencil = VSResourceManager::GetDepthStencil(m_uiRTWidth,m_uiRTWidth,VSRenderer::SFT_D24S8,0);
	
	for (unsigned int k = 0 ; k < VSCubeTexture::F_MAX ;k++)
	{
		m_pShadowMapSceneRender->ClearRTAndDepth();
		m_pShadowMapSceneRender->AddRenderTarget(m_pCubRenderTarget[k]);
		m_pShadowMapSceneRender->SetDepthStencil(pDepthStencil,VSCuller::RG_NORMAL);
		m_pShadowMapSceneRender->Draw(m_ShadowCuller[k], dAppTime);
	}
	VSResourceManager::DisableOutputResource(pDepthStencil);
}
void VSPointLight::DrawNormalProjectShadow(VSCuller & CurCuller,double dAppTime)
{
	VSMatrix3X3 MatTemp[VSCubeTexture::F_MAX] = {VSMatrix3X3::ms_CameraViewRight,VSMatrix3X3::ms_CameraViewLeft,VSMatrix3X3::ms_CameraViewUp,VSMatrix3X3::ms_CameraViewDown,VSMatrix3X3::ms_CameraViewFront,VSMatrix3X3::ms_CameraViewBack};

	VSVector3 WorldPos = GetWorldTranslate();


	if (m_pScene.GetNum() > 0)
	{
		for (unsigned int Index = 0 ; Index < VSCubeTexture::F_MAX ;Index++)
		{
			VSCamera TempCamera;
			TempCamera.CreateFromEuler(WorldPos, 0.0f, 0.0f, 0.0f);
			TempCamera.SetLocalRotate(MatTemp[Index]);
			TempCamera.SetPerspectiveFov(VSPI2, 1.0f, 1.0f, m_Range);
			if (!CurCuller.IsVisible(TempCamera.GetFrustumAABB()))
			{
				continue;
			}
			m_ShadowCuller[Index].GetSceneContent(TempCamera, m_pScene, this, dAppTime);

			m_LightShadowMatrix = TempCamera.GetViewMatrix() * TempCamera.GetProjMatrix();

			m_pProjectShadowRenderTarget = VSResourceManager::GetRenderTarget(m_uiRTWidth, m_uiRTWidth, VSRenderer::SFT_R32F, 0);
			VSDepthStencil * pDepthStencil = VSResourceManager::GetDepthStencil(m_uiRTWidth,m_uiRTWidth,VSRenderer::SFT_D24S8,0);

			m_pShadowMapSceneRender->ClearRTAndDepth();
			m_pShadowMapSceneRender->SetDepthStencil(pDepthStencil,VSCuller::RG_NORMAL);
			m_pShadowMapSceneRender->AddRenderTarget(m_pProjectShadowRenderTarget);
			m_pShadowMapSceneRender->Draw(m_ShadowCuller[Index],dAppTime);
			m_pShadowTexture[0]->SetTexture((VSTexture *)m_pProjectShadowRenderTarget->GetCreateBy());
			VSResourceManager::DisableOutputResource(pDepthStencil);

			m_pProjectShadowSceneRender->Draw(CurCuller,dAppTime);

			DisableDependenceShadowMap(m_ShadowCuller[Index],dAppTime);

		}
	}
}
void VSPointLight::DrawFovProjectShadow(VSCuller & CurCuller,double dAppTime)
{
	VSMatrix3X3 MatTemp[VSCubeTexture::F_MAX] = {VSMatrix3X3::ms_CameraViewRight,VSMatrix3X3::ms_CameraViewLeft,VSMatrix3X3::ms_CameraViewUp,VSMatrix3X3::ms_CameraViewDown,VSMatrix3X3::ms_CameraViewFront,VSMatrix3X3::ms_CameraViewBack};

	VSMatrix3X3 ViewTran;
	CurCuller.GetCamera()->GetViewMatrix().Get3X3(ViewTran);
	VSMatrix3X3 New;
	New.InverseOf(ViewTran);
	for (unsigned int Index = 0 ; Index < VSCubeTexture::F_MAX ;Index++)
	{
		MatTemp[Index] = MatTemp[Index] * New;
	}
	VSCameraPtr CubCameraPtr[VSCubeTexture::F_MAX];

	VSVector3 WorldPos = GetWorldTranslate();

	VSPlane3 CameraPlane[VSCamera::CP_MAX];
	CurCuller.GetCamera()->GetPlane(CameraPlane);
	unsigned int CullFlag = 0;
	for (unsigned int i = 0 ; i < VSCamera::CP_MAX ; i++)
	{
		int iSide = WorldPos.RelationWith(CameraPlane[i]);
		if (iSide == VSBACK)
		{
			CullFlag = CullFlag | (1 << i);
		}
	}
	if (m_pScene.GetNum() > 0)
	{
		for (unsigned int Index = 0 ; Index < VSCubeTexture::F_MAX ;Index++)
		{
			
			if (((1 << Index) & CullFlag) == false)
			{
				continue;
			}

			VSCamera TempCamera;
			TempCamera.CreateFromEuler(WorldPos, 0.0f, 0.0f, 0.0f);
			TempCamera.SetLocalRotate(MatTemp[Index]);
			TempCamera.SetPerspectiveFov(VSPI2, 1.0f, 1.0f, m_Range);
			if (!CurCuller.IsVisible(TempCamera.GetFrustumAABB()))
			{
				continue;
			}
			m_ShadowCuller[Index].GetSceneContent(TempCamera, m_pScene, this, dAppTime);

			m_LightShadowMatrix = TempCamera.GetViewMatrix() * TempCamera.GetProjMatrix();

			m_pProjectShadowRenderTarget = VSResourceManager::GetRenderTarget(m_uiRTWidth, m_uiRTWidth, VSRenderer::SFT_R32F, 0);
			VSDepthStencil * pDepthStencil = VSResourceManager::GetDepthStencil(m_uiRTWidth,m_uiRTWidth,VSRenderer::SFT_D24S8,0);
			m_pShadowMapSceneRender->ClearRTAndDepth();
			m_pShadowMapSceneRender->SetDepthStencil(pDepthStencil,VSCuller::RG_NORMAL);
			m_pShadowMapSceneRender->AddRenderTarget(m_pProjectShadowRenderTarget);
			m_pShadowMapSceneRender->Draw(m_ShadowCuller[Index],dAppTime);
			m_pShadowTexture[0]->SetTexture((VSTexture *)m_pProjectShadowRenderTarget->GetCreateBy());
			VSResourceManager::DisableOutputResource(pDepthStencil);

			m_pProjectShadowSceneRender->Draw(CurCuller,dAppTime);
			DisableDependenceShadowMap(m_ShadowCuller[Index],dAppTime);
		}		
	}

}
void VSPointLight::DrawFovCubShadow(VSCuller & CurCuller,double dAppTime)
{

	VSMatrix3X3 MatTemp[VSCubeTexture::F_MAX] = {VSMatrix3X3::ms_CameraViewRight,VSMatrix3X3::ms_CameraViewLeft,VSMatrix3X3::ms_CameraViewUp,VSMatrix3X3::ms_CameraViewDown,VSMatrix3X3::ms_CameraViewFront,VSMatrix3X3::ms_CameraViewBack};

	VSMatrix3X3 ViewTran;
	CurCuller.GetCamera()->GetViewMatrix().Get3X3(ViewTran);
	VSMatrix3X3 New;
	New.InverseOf(ViewTran);
	for (unsigned int Index = 0 ; Index < VSCubeTexture::F_MAX ;Index++)
	{
		MatTemp[Index] = MatTemp[Index] * New;
	}
	VSCamera CubCamera[VSCubeTexture::F_MAX];

	VSVector3 WorldPos = GetWorldTranslate();

	VSPlane3 CameraPlane[VSCamera::CP_MAX];
	CurCuller.GetCamera()->GetPlane(CameraPlane);
	unsigned int CullFlag = 0;
	for (unsigned int i = 0 ; i < VSCamera::CP_MAX ; i++)
	{
		int iSide = WorldPos.RelationWith(CameraPlane[i]);
		if (iSide == VSBACK)
		{
			CullFlag = CullFlag | (1 << i);
		}
	}
	
	if (m_pScene.GetNum() > 0)
	{
		for (unsigned int Index = 0 ; Index < VSCubeTexture::F_MAX ;Index++)
		{

			if (((1 << Index) & CullFlag) == false)
			{
				continue;
			}
			CubCamera[Index].CreateFromEuler(WorldPos, 0.0f, 0.0f, 0.0f);
			CubCamera[Index].SetLocalRotate(MatTemp[Index]);
			CubCamera[Index].SetPerspectiveFov(AngleToRadian(90.0f), 1.0f, 1.0f, m_Range);
			m_ShadowCuller[Index].GetSceneContent(CubCamera[Index], m_pScene, this, dAppTime);
		}
	}

	VSDepthStencil * pDepthStencil = VSResourceManager::GetDepthStencil(m_uiRTWidth,m_uiRTWidth,VSRenderer::SFT_D24S8,0);

	for (unsigned int k = 0 ; k < VSCubeTexture::F_MAX ;k++)
	{
		m_pShadowMapSceneRender->ClearRTAndDepth();
		m_pShadowMapSceneRender->AddRenderTarget(m_pCubRenderTarget[k]);
		m_pShadowMapSceneRender->SetDepthStencil(pDepthStencil,VSCuller::RG_NORMAL);
		m_pShadowMapSceneRender->Draw(m_ShadowCuller[k], dAppTime);

	}
	VSResourceManager::DisableOutputResource(pDepthStencil);
}
void VSPointLight::BuildSceneInfo(VSScene * pScene)
{
	VSLocalLight::BuildSceneInfo(pScene);
	if (m_uiShadowType == ST_CUB && m_bIsCastShadow && !m_bIsStatic)
	{
		
	}
}
void VSPointLight::DrawVolumeShadow(VSCuller & CurCuller, double dAppTime)
{
	if (m_uiShadowType != ST_VOLUME)
	{
		return;
	}
	m_VolumeShadowCuller.GetSceneContent(*CurCuller.GetCamera(), m_pScene, this, dAppTime);
	
	m_pVolumeShadowSceneRender->Draw(m_VolumeShadowCuller, dAppTime);

	m_pVolumeShadowRenderTarget = VSResourceManager::GetRenderTarget(VSRenderer::ms_pRenderer->GetCurRTWidth(), VSRenderer::ms_pRenderer->GetCurRTHeight(), VSRenderer::SFT_A8R8G8B8, 0);

	m_pPEVolumeSMSceneRender->ClearRTAndDepth();
	m_pPEVolumeSMSceneRender->AddRenderTarget(m_pVolumeShadowRenderTarget);
	m_pPEVolumeSMSceneRender->Draw(CurCuller, dAppTime);

	m_pShadowTexture[0]->SetTexture((VSTexture *)m_pVolumeShadowRenderTarget->GetCreateBy());
}
void VSPointLight::DrawDependenceShadowMap(VSCuller & CurCuller, double dAppTime)
{
	if (m_bEnable && m_bIsCastShadow)
	{
		if (m_uiShadowType == ST_CUB)
		{
			if (CurCuller.GetCamera()->GetFov() <= AngleToRadian(90.0f))
			{
				DrawFovCubShadow(CurCuller, dAppTime);
			}
		}
	}
}
void VSPointLight::DrawDualParaboloidShadow(VSCuller & CurCuller, double dAppTime)
{
	VSVector3 WorldPos = GetWorldTranslate();
	VSCamera DPCamera[2];
	
	if (m_pScene.GetNum() > 0)
	{
		for (unsigned int Index = 0; Index < 2; Index++)
		{
			VSREAL ZDirection = (Index == 0) ? 1.0f : -1.0f;
			DPCamera[Index].CreateFromLookDir(WorldPos, VSVector3(0.0f, 0.0f, ZDirection));
			DPCamera[Index].SetOrthogonal(2.0f * m_Range,2.0f * m_Range,0,m_Range);
			m_ShadowCuller[Index].GetSceneContent(DPCamera[Index], m_pScene, this, dAppTime);
			DPCamera[Index].ClearProject();
		}

	}

	VSDepthStencil * pDepthStencil = VSResourceManager::GetDepthStencil(m_uiRTWidth, m_uiRTWidth, VSRenderer::SFT_D24S8, 0);
	m_pDualParaboloidRenderTarget0 = VSResourceManager::GetRenderTarget(m_uiRTWidth, m_uiRTWidth, VSRenderer::SFT_R16F, 0);
	m_pDualParaboloidRenderTarget1 = VSResourceManager::GetRenderTarget(m_uiRTWidth, m_uiRTWidth, VSRenderer::SFT_R16F, 0);
	for (unsigned int Index = 0; Index < 2; Index++)
	{
		VSRenderTarget * pTarget = Index == 0 ? m_pDualParaboloidRenderTarget0 : m_pDualParaboloidRenderTarget1;

		m_pShadowMapSceneRender->ClearRTAndDepth();
		m_pShadowMapSceneRender->SetDepthStencil(pDepthStencil, VSCuller::RG_NORMAL);
		m_pShadowMapSceneRender->AddRenderTarget(pTarget);

		m_pShadowMapSceneRender->Draw(m_ShadowCuller[Index], dAppTime);

		m_pShadowTexture[Index]->SetTexture((VSTexture *)pTarget->GetCreateBy());


	}
	VSResourceManager::DisableOutputResource(pDepthStencil);
}
bool VSPointLight::Cullby(VSCuller & Culler)
{
	unsigned int uiVSF = Culler.IsVisible(m_WorldRenderBV,true);
	if (uiVSF == VSCuller::VSF_ALL || uiVSF == VSCuller::VSF_PARTIAL)
	{
		m_bEnable = true;
		Culler.InsertLight(this);

	}	
	
	return true;
}
void VSPointLight::DrawPorjectShadow(VSCuller & CurCuller,double dAppTime,VS2DTexture * pNormalDepthTexture)
{
	if (m_bEnable && m_bIsCastShadow)
	{
		if (m_uiShadowType == ST_PROJECT)
		{
			m_pProjectShadowSceneRender->m_pNormalDepthTexture = pNormalDepthTexture;
			m_pProjectShadowSceneRender->m_fLightRange = m_Range;
			if (CurCuller.GetCamera()->GetFov() <= AngleToRadian(90.0f))
			{
	 			DrawFovProjectShadow(CurCuller,dAppTime);
	 		}
	 		else
	 		{
				DrawNormalProjectShadow(CurCuller,dAppTime);
			}
		}
	}
}