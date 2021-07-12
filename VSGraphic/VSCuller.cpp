#include "VSCuller.h"
#include "VSSpatial.h"
#include "VSGeometry.h"
#include "VSGraphicInclude.h"
#include "VSSceneManager.h"
#include "VSPointLight.h"
#include "VSGeometry.h"
#include "VSSpotLight.h"
#include "VSMaterial.h"
#include "VSSortLight.h"
#include "VSInstanceGeometry.h"
#include "VSQuery.h"
#include "VSTimer.h"
#include "VSRenderThread.h"
#include "VSProfiler.h"
using namespace VSEngine2;
VSRenderContext VSRenderContext::ms_RenderContextNULL;
DECLEAR_TIME_PROFILENODE(CullerSort, ViewFamilyUpdate)
DECLEAR_TIME_PROFILENODE(CollectDynamicInstance, ViewFamilyUpdate)
DECLEAR_TIME_PROFILENODE(OcclusionQuery, Update)
bool VSRenderContext::IsCanDraw()const
{
	if (!m_pGeometry || !m_pMaterialInstance || bOcclusion)
	{
		return false;
	}
	VSMaterial * pMaterial = m_pMaterialInstance->GetMaterial();
	if (!pMaterial)
	{
		return false;
	}
	return true;
}
VSCuller::VSCuller()
{
	m_uiPlaneNum = 0;
	m_uiPlaneState =0xffffffff;
	for (unsigned int j = 0 ; j < RG_MAX ; j++)
	{
		for (unsigned int i = 0 ; i < VST_MAX ; i++)
		{
			m_VisibleSet[j][i].Clear();
		}
	}
	
	m_LightSet.Clear();
	m_pCamera = NULL;
	m_iUseMaterialIndex = -1;
	m_uiBeignDynamicInstanceNum = 4;
}
VSCuller::~VSCuller()
{

}
bool VSCuller::PushPlane(const VSPlane3 & Plane)
{
	VSMAC_ASSERT(m_uiPlaneNum <= VS_MAX_PLANE_NUM);
	m_Plane[m_uiPlaneNum] = Plane;
	m_uiPlaneNum++;
	return true;
}
bool VSCuller::PushCameraPlane(VSCamera &Camera)
{
	VSPlane3 Plane[VSCamera::CP_MAX];
	Camera.GetPlane(Plane);
	for(unsigned int i =0 ; i < VSCamera::CP_MAX; i++)
	{
		if(!PushPlane(Plane[i]))
			return 0;
	}
	VSArray<VSPlane3> & CustomCullPlane = Camera.m_CustomCullPlane;
	for (unsigned int i = 0; i < CustomCullPlane.GetNum(); i++)
	{
		if (!PushPlane(CustomCullPlane[i]))
			return 0;
	}
	m_pCamera = &Camera;
	return 1;
}
bool VSCuller::PopPlane(VSPlane3 &Plane)
{
	VSMAC_ASSERT(m_uiPlaneNum);
	Plane = m_Plane[m_uiPlaneNum];
	m_uiPlaneNum--;
	return true;
}
bool VSCuller::InsertObject(VSRenderContext &VisibleContext,unsigned int uiVisibleSetType,unsigned int uiRenderGroup)
{
	VSMAC_ASSERT(uiVisibleSetType < VST_MAX && uiRenderGroup < RG_MAX);
	m_VisibleSet[uiRenderGroup][uiVisibleSetType].AddElement(VisibleContext);
	return 1;
}
bool VSCuller::HasLight(VSLight * pLight)
{
	return m_LightSet.FindElement(pLight) != m_LightSet.GetNum();
}
void VSCuller::GetAndSortLight()
{
	if (GetLightNum() == 0)
	{
		return;
	}
	for (unsigned int uiRenderGroup = 0; uiRenderGroup < RG_MAX; uiRenderGroup++)
	{
		for (unsigned int t = 0; t <= VST_MAX; t++)
		{
			for (unsigned int j = 0; j < GetVisibleNum(t, uiRenderGroup); j++)
			{
				VSRenderContext& VisibleContext = GetVisibleSpatial(j, t, uiRenderGroup);

				VSGeometry * pGeometry = VisibleContext.m_pGeometry;

				for (unsigned int l = 0; l < GetLightNum(); l++)
				{
					VSLight * pLight = GetLight(l);
					if (pLight->IsRelative(pGeometry))
					{
						if (pLight->GetLightType() == VSLight::LT_SKY)
						{
							VisibleContext.m_pIndirectLight.AddElement(pLight);
						}
						else
						{
							VisibleContext.m_pDirectLight.AddElement(pLight);
						}

					}
				}
				VisibleContext.m_pIndirectLight.SortAll(LightPriority());
				VisibleContext.m_pDirectLight.SortAll(LightPriority());
			}
		}
	}
}
unsigned int VSCuller::IsVisible(const VSSphere3 &S,bool bClearState)
{
	VSMAC_ASSERT(m_uiPlaneNum);
	int iP = m_uiPlaneNum - 1;
	unsigned int uiMask = 1 << iP;
	unsigned int uiPlaneInNum = 0;
	for (unsigned int i = 0; i < m_uiPlaneNum; i++, iP--, uiMask >>= 1)
	{
		if (m_uiPlaneState & uiMask)
		{
			int iSide = S.RelationWith(m_Plane[iP]);

			if (iSide == VSFRONT)
			{
				if (bClearState == true)
				{
					ClearPlaneState();
				}
				return VSF_NONE;
			}

			if (iSide == VSBACK)
			{

				m_uiPlaneState &= ~uiMask;
				uiPlaneInNum++;
			}
		}
		else
		{
			uiPlaneInNum++;
		}
	}
	if (uiPlaneInNum == m_uiPlaneNum)
	{
		if (bClearState == true)
		{
			ClearPlaneState();
		}
		return VSF_ALL;
	}
	else
	{
		if (bClearState == true)
		{
			ClearPlaneState();
		}
		return VSF_PARTIAL;
	}
}
unsigned int VSCuller::IsVisible(const VSVector3 &Point,bool bClearState)
{
	VSMAC_ASSERT(m_uiPlaneNum);
	int iP = m_uiPlaneNum - 1;
	unsigned int uiMask = 1 << iP;
	unsigned int uiPlaneInNum = 0;
	for (unsigned int i = 0; i < m_uiPlaneNum; i++, iP--, uiMask >>= 1)
	{
		if (m_uiPlaneState & uiMask)
		{
			int iSide = Point.RelationWith(m_Plane[iP]);

			if (iSide == VSFRONT)
			{
				if (bClearState == true)
				{
					ClearPlaneState();
				}
				return VSF_NONE;
			}

			if (iSide == VSBACK)
			{

				m_uiPlaneState &= ~uiMask;
				uiPlaneInNum++;
			}
		}
		else
		{
			uiPlaneInNum++;
		}
	}
	if (uiPlaneInNum == m_uiPlaneNum)
	{
		if (bClearState == true)
		{
			ClearPlaneState();
		}
		return VSF_ALL;
	}
	else
	{
		if (bClearState == true)
		{
			ClearPlaneState();
		}
		return VSF_PARTIAL;
	}
}
unsigned int VSCuller::IsVisible(const VSAABB3 &BV,bool bClearState)
{
	VSMAC_ASSERT(m_uiPlaneNum);
	int iP = m_uiPlaneNum - 1;
	unsigned int uiMask = 1 << iP;
	unsigned int uiPlaneInNum = 0;
	for (unsigned int i = 0; i < m_uiPlaneNum; i++, iP--, uiMask >>= 1)
	{
		if (m_uiPlaneState & uiMask)
		{
			int iSide = BV.RelationWith(m_Plane[iP]);

			if (iSide == VSFRONT)
			{
				if (bClearState == true)
				{
					ClearPlaneState();
				}
				return VSF_NONE;
			}

			if (iSide == VSBACK)
			{
				
				m_uiPlaneState &= ~uiMask;
				uiPlaneInNum++;
			}
		}
		else
		{
			uiPlaneInNum++;
		}
	}
	if (uiPlaneInNum == m_uiPlaneNum)
	{
		if (bClearState == true)
		{
			ClearPlaneState();
		}
		return VSF_ALL;
	}
	else
	{
		if (bClearState == true)
		{
			ClearPlaneState();
		}
		return VSF_PARTIAL;
	}
}
void VSCuller::GetSceneContent(VSCamera & pCamera, VSArray<VSScene *> & pSceneArray, double dAppTime,bool bSort)
{
	GetSceneGeometryContent(pCamera,pSceneArray, dAppTime, bSort);
	GetSceneLightContent(pCamera, pSceneArray, dAppTime);
	CollectDynamicInstance();
}
void VSCuller::GetSceneLightContent(VSCamera & pCamera, VSArray<class VSScene *> & pSceneArray, double dAppTime)
{
	for (unsigned int i = 0; i < pSceneArray.GetNum(); i++)
	{
		VSScene * pScene = pSceneArray[i];
		if (!pScene)
		{
			continue;
		}
		for (unsigned int i = 0; i < pScene->GetAllLightNum(); i++)
		{
			VSLight * pLight = pScene->GetAllLight(i);
			if (pLight)
			{
				pLight->Cullby(*this);
			}
		}
	}
	GetAndSortLight();
}
void VSCuller::GetSceneGeometryContent(VSCamera & pCamera, VSArray<VSScene *> & pSceneArray, double dAppTime, bool bSort)
{
	ClearAll();
	if (pSceneArray.GetNum() > 0)
	{
		PushCameraPlane(pCamera);

		for (unsigned int i = 0; i < pSceneArray.GetNum(); i++)
		{
			VSScene * pScene = pSceneArray[i];
			if (!pScene)
			{
				continue;
			}

			pScene->ComputeVisibleSet(*this, false, dAppTime);

		}
		if (bSort)
		{
			Sort();
		}		
	}
}
bool VSCuller::RenderPriority::operator()(VSRenderContext & p1,VSRenderContext & p2)
{


	VSMaterial * pMaterial1 = p1.m_pMaterialInstance->GetMaterial();
	VSMaterial * pMaterial2 = p2.m_pMaterialInstance->GetMaterial();


	int uiMaterialCompareFlag = ComparePointer(p1.m_pMaterialInstance->GetMaterial(), p2.m_pMaterialInstance->GetMaterial());

	int uiMeshDataCompareFlag = ComparePointer(p1.m_pGeometry->GetMeshData(), p2.m_pGeometry->GetMeshData());
	if (pMaterial1->m_uiCustomLayer == pMaterial2->m_uiCustomLayer)
	{
		if (uiMaterialCompareFlag > 0)
		{
			return 1;
		}
		else if (uiMaterialCompareFlag < 0)
		{
			return 0;
		}
		else
		{
			
			if (uiMeshDataCompareFlag > 0)
			{
				return 1;
			}
			else if (uiMeshDataCompareFlag < 0)
			{
				return 0;
			}
			else
			{
				if (p1.m_pMeshNode->GetDrawInstance() == p2.m_pMeshNode->GetDrawInstance())
				{
					if (p1.m_uiPassId > p2.m_uiPassId)
					{
						return 0;
					}
					else if (p1.m_uiPassId < p2.m_uiPassId)
					{
						return 1;
					}
					else
					{
						return 1;
					}
				}
				else if (p1.m_pMeshNode->GetDrawInstance() > p2.m_pMeshNode->GetDrawInstance())
				{
					return 0;
				}
				else
				{
					return 1;
				}
			}
			
		}
	}
	else if (pMaterial1->m_uiCustomLayer > pMaterial2->m_uiCustomLayer)
	{
		return 0;
	}
	else 
	{
		return 1;
	}


}
void VSCuller::CollectDynamicInstance()
{
	for (unsigned int j = 0; j < RG_MAX; j++)
	{
		CollectDynamicInstance(m_VisibleSet[j][VST_BASE]);
		CollectDynamicInstance(m_VisibleSet[j][VST_ALPHABLEND]);
	}
}
void VSCuller::CollectDynamicInstance(VSArray<VSRenderContext> & inContext)
{
	ADD_TIME_PROFILE(CollectDynamicInstance);
	if (!inContext.GetNum())
	{
		return;
	}
	unsigned int uiStartIndex = 0;
	VSMeshData* pIndexMeshData = inContext[0].m_pGeometry->GetMeshData();
	VSMaterialR * pIndexMaterial = inContext[0].m_pMaterialInstance->GetMaterialR();
	VSMeshNode * pIndexMeshNode = inContext[0].m_pMeshNode;
	unsigned int uiEndIndex = uiStartIndex + 1;
	unsigned int CollectNum = 1;
	unsigned int uiMaxPassID = 0;
	while (uiEndIndex < inContext.GetNum())
	{
		VSGeometry* pTempGeometry = inContext[uiEndIndex].m_pGeometry;
		VSMeshData* pTempMeshData = pTempGeometry->GetMeshData();
		VSMaterialR * pTempMaterial = inContext[uiEndIndex].m_pMaterialInstance->GetMaterialR();
		VSMeshNode * pTempMeshNode = inContext[uiEndIndex].m_pMeshNode;
		unsigned int uiTempPassID = inContext[uiEndIndex].m_uiPassId;

		VSSkeletonMeshNode * pIndexSK = DynamicCast<VSSkeletonMeshNode>(pIndexMeshNode);
		VSSkeletonMeshNode * pTempSK = DynamicCast<VSSkeletonMeshNode>(pTempMeshNode);
		bool CanInstance = pIndexSK && pTempSK && pIndexSK->GetAnimTree() && pTempSK->GetAnimTree();;
		CanInstance |= (!pIndexSK) && (!pTempSK);
		
		if (CanInstance && pIndexMaterial == pTempMaterial
			&& pTempMeshData == pIndexMeshData
			&& (pIndexMeshNode->GetDrawInstance() && pTempMeshNode->GetDrawInstance()) && pTempMaterial->IsLoaded())
		{
			if (uiMaxPassID < inContext[uiEndIndex].m_uiPassId)
			{
				uiMaxPassID = inContext[uiEndIndex].m_uiPassId;
			}
			CollectNum++;		
		}
		else
		{		
			if (CollectNum > m_uiBeignDynamicInstanceNum)
			{
				VSArray<VSLight *> pIndirectLightCollection;
				VSArray<VSLight *> pDirectLightCollection;
				VSInstanceGeometry * pIG = VSResourceManager::GetDynamicInstanceGeometry(pIndexMeshData, pIndexMaterial);
				if (pIndexSK)
				{
					pIG->m_AnimInstanceTexture = pIndexSK->GetAnimInstanceTexture();
					pIG->m_uiBoneNum = pIndexSK->GetSkeleton()->GetBoneNum();
					pIG->SetMaterialInstance(inContext[uiStartIndex].m_pMaterialInstance, 0);
				}
				for (unsigned int uiInstanceIndex = uiStartIndex; uiInstanceIndex < uiEndIndex; uiInstanceIndex++)
				{
					if (!inContext[uiInstanceIndex].m_uiPassId)
					{
						if (inContext[uiInstanceIndex].m_pGeometry->GetAffectBoneNum())
						{
							VSSkeletonMeshNode * pSkeMeshNode = (VSSkeletonMeshNode *)inContext[uiInstanceIndex].m_pMeshNode;
							pIG->AddInstance(inContext[uiInstanceIndex].m_pGeometry->GetWorldAABB(), inContext[uiInstanceIndex].m_pGeometry->GetWorldTransform(),
								inContext[uiInstanceIndex].m_pMaterialInstance, &pSkeMeshNode->m_AnimInstanceData);
						}
						else
						{
							pIG->AddInstance(inContext[uiInstanceIndex].m_pGeometry->GetWorldAABB(),inContext[uiInstanceIndex].m_pGeometry->GetWorldTransform(),
								inContext[uiInstanceIndex].m_pMaterialInstance);
						}
						
						pIndirectLightCollection.GetNoHaveElement(inContext[uiInstanceIndex].m_pIndirectLight);
						pDirectLightCollection.GetNoHaveElement(inContext[uiInstanceIndex].m_pDirectLight);
					}
				}
				pIG->UpdateWorldInstanceBound();
				for (unsigned int uiPassID = 0; uiPassID <= uiMaxPassID; uiPassID++)
				{
					inContext[uiStartIndex + uiPassID].m_pMeshNode = NULL;
					inContext[uiStartIndex + uiPassID].m_pMaterialInstance = pIG->GetMaterialInstance(0);
					inContext[uiStartIndex + uiPassID].m_pMaterial = pIG->GetMaterialInstance(0)->GetMaterial();
					inContext[uiStartIndex + uiPassID].m_pGeometry = pIG;
					inContext[uiStartIndex + uiPassID].m_uiPassId = uiPassID;
					inContext[uiStartIndex + uiPassID].m_pDirectLight = pDirectLightCollection;
					inContext[uiStartIndex + uiPassID].m_pIndirectLight = pIndirectLightCollection;
				}
				uiStartIndex = uiStartIndex + uiMaxPassID + 1;
				inContext.Erase(uiStartIndex, uiEndIndex - 1);
				uiEndIndex = uiStartIndex;
			}
			CollectNum = 1;
			uiMaxPassID = 0;
			uiStartIndex = uiEndIndex;
			pIndexMeshData = pTempMeshData;
			pIndexMaterial = pTempMaterial;
			pIndexMeshNode = pTempMeshNode;
		}
		uiEndIndex++;
	}
	if (CollectNum > m_uiBeignDynamicInstanceNum)
	{
		VSArray<VSLight *> pIndirectLightCollection;
		VSArray<VSLight *> pDirectLightCollection;
		VSInstanceGeometry * pIG = VSResourceManager::GetDynamicInstanceGeometry(pIndexMeshData, pIndexMaterial);
		VSSkeletonMeshNode * pIndexSK = DynamicCast<VSSkeletonMeshNode>(pIndexMeshNode);
		if (pIndexSK)
		{
			pIG->m_AnimInstanceTexture = pIndexSK->GetAnimInstanceTexture();
			pIG->m_uiBoneNum = pIndexSK->GetSkeleton()->GetBoneNum();
			pIG->SetMaterialInstance(inContext[uiStartIndex].m_pMaterialInstance, 0);
		}
		for (unsigned int uiInstanceIndex = uiStartIndex; uiInstanceIndex < uiEndIndex; uiInstanceIndex++)
		{
			if (!inContext[uiInstanceIndex].m_uiPassId)
			{
				if (inContext[uiInstanceIndex].m_pGeometry->GetAffectBoneNum())
				{
					VSSkeletonMeshNode * pSkeMeshNode = (VSSkeletonMeshNode *)inContext[uiInstanceIndex].m_pMeshNode;
					pIG->AddInstance(inContext[uiInstanceIndex].m_pGeometry->GetWorldAABB(),inContext[uiInstanceIndex].m_pGeometry->GetWorldTransform(),
						inContext[uiInstanceIndex].m_pMaterialInstance, &pSkeMeshNode->m_AnimInstanceData);
				}
				else
				{ 
					pIG->AddInstance(inContext[uiInstanceIndex].m_pGeometry->GetWorldAABB(),inContext[uiInstanceIndex].m_pGeometry->GetWorldTransform(),
						inContext[uiInstanceIndex].m_pMaterialInstance);
				}				
				pIndirectLightCollection.GetNoHaveElement(inContext[uiInstanceIndex].m_pIndirectLight);
				pDirectLightCollection.GetNoHaveElement(inContext[uiInstanceIndex].m_pDirectLight);
			}
		}
		pIG->UpdateWorldInstanceBound();
		for (unsigned int uiPassID = 0; uiPassID <= uiMaxPassID; uiPassID++)
		{
			inContext[uiStartIndex + uiPassID].m_pMeshNode = NULL;
			inContext[uiStartIndex + uiPassID].m_pMaterialInstance = pIG->GetMaterialInstance(0);
			inContext[uiStartIndex + uiPassID].m_pMaterial = pIG->GetMaterialInstance(0)->GetMaterial();
			inContext[uiStartIndex + uiPassID].m_pGeometry = pIG;
			inContext[uiStartIndex + uiPassID].m_uiPassId = uiPassID;
			inContext[uiStartIndex + uiPassID].m_pDirectLight = pDirectLightCollection;
			inContext[uiStartIndex + uiPassID].m_pIndirectLight = pIndirectLightCollection;
		}
		uiStartIndex = uiStartIndex + uiMaxPassID + 1;
		inContext.Erase(uiStartIndex, uiEndIndex - 1);
	}
}
void VSCuller::Sort()
{
	ADD_TIME_PROFILE(CullerSort);
	VSCamera * pCamera = m_pCamera;
	for (unsigned int j = 0 ; j < RG_MAX ; j++)
	{
		if (m_VisibleSet[j][VST_BASE].GetNum() > 0)
		{
			m_VisibleSet[j][VST_BASE].SortAll(RenderPriority());

		}
		if (m_VisibleSet[j][VST_ALPHABLEND].GetNum() > 0 && pCamera)
		{
			m_VisibleSet[j][VST_ALPHABLEND].SortAll(AlphaPriority(pCamera));

		}
	}
	VSCullerManager::GetCullerManager().ProcessOcclusionCull(this);
}
bool VSCuller::AlphaPriority::operator()(VSRenderContext & p1,VSRenderContext & p2)
{
	VSMAC_ASSERT(m_pCamera);

	VSVector3 vLength1 = m_pCamera->GetWorldTranslate() - p1.m_pGeometry->GetWorldTranslate(); 
	VSVector3 vLength2 = m_pCamera->GetWorldTranslate() - p2.m_pGeometry->GetWorldTranslate();

	if (vLength1.GetSqrLength() > vLength2.GetSqrLength())
	{
		return 0;
	}
	return 1;

}
void VSCuller::InsertLight(VSLight * pLight)
{
	VSMAC_ASSERT(pLight);

	m_LightSet.AddElement(pLight);

}
bool VSCuller::ForceNoCull(const VSSpatial * pSpatial)
{
	return false;
}
bool VSCuller::CullConditionNode(const VSMeshNode * pMeshNode)
{
	return false;
}
bool VSCuller::CullGeometry(VSGeometry * pGeometry)
{
	VSShadowVolumeGeometry * pSVG = DynamicCast<VSShadowVolumeGeometry>(pGeometry);
	if (pSVG)
	{
		return true;
	}
	return false;

}
void VSCuller::GetAllVisibleAABB(unsigned int uiVisibleSetType, unsigned int uiRenderGroup, VSArray<VSAABB3> & AABBArray)
{

	for (unsigned int i = 0 ; i < m_VisibleSet[uiRenderGroup][uiVisibleSetType].GetNum() ;i++)
	{
		VSGeometry * pGeometry = m_VisibleSet[uiRenderGroup][uiVisibleSetType][i].m_pGeometry;


		AABBArray.AddElement(pGeometry->GetWorldAABB());
		
		
	}

}
VSShadowCuller::VSShadowCuller()
{

}
VSShadowCuller::~VSShadowCuller()
{

}
void VSShadowCuller::GetSceneContent(VSCamera & pCamera, VSArray<class VSScene *> & pSceneArray, VSLocalLight * pLocalLight, double dAppTime, bool bCollectDynamicInstance)
{
	m_pLocalLight = pLocalLight;
	VSCuller::GetSceneGeometryContent(pCamera, pSceneArray,dAppTime, true);
	if (bCollectDynamicInstance)
	{
		CollectDynamicInstance();
	}
}
bool VSShadowCuller::CullConditionNode(const VSMeshNode * pMeshNode)
{
	if (pMeshNode->m_bCastShadow == true)
	{
		return false;
	}
	return true;
}
bool VSShadowCuller::TestObjectWithCameraIntersectOnLightDirPlane(const VSSpatial * pSpatial)
{

	VSAABB3  ObjectAABB = pSpatial->GetWorldAABB();
	VSAABB3 ReceiveAABB = m_pCamera->GetFrustumAABB();
	VSVector3 ObjectToReceive = ReceiveAABB.GetCenter() - ObjectAABB.GetCenter();

	VSDirectionLight * pDirLight = (VSDirectionLight *)m_pLocalLight;
	VSVector3 Dir, Up, Right;
	pDirLight->GetWorldDir(Dir, Up, Right);
	VSREAL ProjectOnDir = ObjectToReceive.Dot(Dir);
	if (ProjectOnDir < 0.0f)
	{
		return false;
	}
	
	VSVector3 ProjectToReceiveCenterPlane = ObjectToReceive - (Dir * ProjectOnDir);

	VSREAL ReceiveRadius = (ReceiveAABB.GetMaxPoint() - ReceiveAABB.GetCenter()).GetLength();
	VSREAL ObjectRadius = (ObjectAABB.GetMaxPoint() - ObjectAABB.GetCenter()).GetLength();
	VSREAL TestDistance = ProjectToReceiveCenterPlane.GetSqrLength();

	VSREAL CombineRadius = ReceiveRadius + ObjectRadius;

	if (TestDistance > CombineRadius * CombineRadius)
	{
		return false;
	}

	return true;
}
bool VSShadowCuller::RenderPriority::operator()(VSRenderContext & p1, VSRenderContext & p2)
{


	VSMaterial * pMaterial1 = p1.m_pMaterialInstance->GetMaterial();
	VSMaterial * pMaterial2 = p2.m_pMaterialInstance->GetMaterial();

	int uiMaterialCompareFlag = ComparePointer(p1.m_pMaterialInstance->GetMaterial(), p2.m_pMaterialInstance->GetMaterial());

	int uiMeshDataCompareFlag = ComparePointer(p1.m_pGeometry->GetMeshData(), p2.m_pGeometry->GetMeshData());

	if (uiMaterialCompareFlag > 0)
	{
		return 1;
	}
	else if (uiMaterialCompareFlag < 0)
	{
		return 0;
	}
	else
	{
		if (uiMeshDataCompareFlag > 0)
		{
			return 1;
		}
		else if (uiMeshDataCompareFlag < 0)
		{
			return 0;
		}
		else
		{
			if (p1.m_pMeshNode->GetDrawInstance() == p2.m_pMeshNode->GetDrawInstance())
			{
					return 1;
			}
			else if (p1.m_pMeshNode->GetDrawInstance() > p2.m_pMeshNode->GetDrawInstance())
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}

	}
}

void VSShadowCuller::Sort()
{
	for (unsigned int j = 0; j < RG_MAX; j++)
	{
		if (m_VisibleSet[j][VST_BASE].GetNum() > 0)
		{
			m_VisibleSet[j][VST_BASE].Sort(0, m_VisibleSet[j][VST_BASE].GetNum() - 1, RenderPriority());

		}
	}
}
VSVolumeShadowMapCuller::VSVolumeShadowMapCuller()
{

}
VSVolumeShadowMapCuller::~VSVolumeShadowMapCuller()
{

}
bool VSVolumeShadowMapCuller::ForceNoCull(const VSSpatial * pSpatial)
{
	if (m_pLocalLight->GetLightType() == VSLight::LT_POINT)
	{
		VSPointLight * pPointLight = (VSPointLight *)m_pLocalLight;
		VSAABB3 AABB(pPointLight->GetWorldTranslate(), pPointLight->GetRange(), pPointLight->GetRange(), pPointLight->GetRange());
		if (AABB.RelationWith(pSpatial->GetWorldAABB()) == VSINTERSECT)
		{
			return true;
		}
	}
	else if (m_pLocalLight->GetLightType() == VSLight::LT_DIRECTION)
	{
		VSAABB3  ObjectAABB = pSpatial->GetWorldAABB();
		int iSide = ObjectAABB.RelationWith(m_Plane[VSCamera::CP_FAR]);
		if (iSide == VSFRONT)
		{
			return false;
		}
		return TestObjectWithCameraIntersectOnLightDirPlane(pSpatial);
	}
	return false;
}
bool VSVolumeShadowMapCuller::CullGeometry(VSGeometry * pGeometry)
{
	
	VSShadowVolumeGeometry * pSVG = DynamicCast<VSShadowVolumeGeometry>(pGeometry);
	if (VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_GS))
	{
		if (pSVG)
		{
			return true;
		}
		return false;
	}
	else
	{
		if (pSVG)
		{
			return false;
		}
		return true;
	}
	
}
VSDirShadowMapCuller::VSDirShadowMapCuller()
{

}
VSDirShadowMapCuller::~VSDirShadowMapCuller()
{

}
bool VSDirShadowMapCuller::ForceNoCull(const VSSpatial * pSpatial)
{
	VSDirectionLight * pDirLight = (VSDirectionLight *)m_pLocalLight;
	if (pDirLight->GetShadowType() == VSDirectionLight::ST_OSM)
	{
		VSAABB3  ObjectAABB = pSpatial->GetWorldAABB();
		int iSide = ObjectAABB.RelationWith(m_Plane[VSCamera::CP_FAR]);
		if (iSide == VSFRONT)
		{
			return false;
		}
	}	
	return TestObjectWithCameraIntersectOnLightDirPlane(pSpatial);

}
VSOcclusionQueryPool::VSOcclusionQueryPool()
{

}
VSOcclusionQueryPool::~VSOcclusionQueryPool()
{
	DeleteAllReleaseOcclusionQuery();
}
void VSOcclusionQueryPool::DeleteAllReleaseOcclusionQuery()
{
	for (unsigned int i = 0; i < m_ElementArray.GetNum(); i++)
	{
		VSMAC_DELETE(m_ElementArray[i].pQuery);
	}
	m_ElementArray.Clear();
	m_FreeElement.Clear();
}
VSOcclusionQueryType VSOcclusionQueryPool::GetOcclusionQuery()
{
	if (m_FreeElement.GetNum() == 0)
	{
		unsigned int ID = m_ElementArray.AddElement(VSOcclusionQueryType());
		m_ElementArray[ID].ID = ID;
		m_ElementArray[ID].pQuery = VS_NEW VSQuery(VSQuery::QT_OCCLUSION);
		m_ElementArray[ID].pQuery->LoadResource(VSRenderer::ms_pRenderer);
		return m_ElementArray[ID];
	}
	else
	{
		unsigned int ID;
		m_FreeElement.Pop(ID);
		return m_ElementArray[ID];
	}
}
void VSOcclusionQueryPool::ReleaseAllOcclusionQuery()
{
	m_FreeElement.Clear();
	for (unsigned int i = 0 ; i < m_ElementArray.GetNum() ;i++)
	{
		m_FreeElement.Push(i);
	}
}
void VSCullerManager::ClearAllOcclusionQuery()
{
	m_OcclusionQueryPool.DeleteAllReleaseOcclusionQuery();
}
bool VSCullerManager::ProcessOcclusionCull(VSCuller * pCuller)
{
	unsigned int uiCullIndex = m_CullerArray.FindElement(pCuller);
	if (uiCullIndex == m_CullerArray.GetNum())
	{
		return false;
	}
	for (unsigned int uiRenderGroup = 0; uiRenderGroup < VSCuller::RG_MAX; uiRenderGroup++)
	{
		for (unsigned int t = 0; t < VSCuller::VST_MAX; t++)
		{
			// cull alpha geometry
			if (t == VSCuller::VST_ALPHABLEND)
			{
				continue;
			}
			for (unsigned int j = 0; j < pCuller->GetVisibleNum(t, uiRenderGroup); j++)
			{
				VSRenderContext& VisibleContext = pCuller->GetVisibleSpatial(j, t, uiRenderGroup);
				if (!VisibleContext.m_pGeometry || !VisibleContext.m_pMaterialInstance)
				{
					continue;
				}
				VSMaterial * pMaterial = VisibleContext.m_pMaterialInstance->GetMaterial();
				if (!pMaterial)
				{
					continue;
				}

				//cull geometry , the VST_COMBINE has alpha 
				const VSBlendDesc & BlendDest = pMaterial->GetRenderState(VisibleContext.m_uiPassId).GetBlendState()->GetBlendDesc();
				if (BlendDest.IsBlendUsed())
				{
					continue;
				}
				const VSDepthStencilDesc & DepthStencilDest = pMaterial->GetRenderState(VisibleContext.m_uiPassId).GetDepthStencilState()->GetDepthStencilDesc();

				VSGeometry * pGeometry = VisibleContext.m_pGeometry;


				// cull no write depth
				if (!DepthStencilDest.m_bDepthWritable)
				{
					continue;
				}

				unsigned int uiIndex = m_NoVisibleGeometry[uiRenderGroup][uiCullIndex].FindElement(pGeometry);
				if (uiIndex != m_NoVisibleGeometry[uiRenderGroup][uiCullIndex].GetNum())
				{
					VisibleContext.bOcclusion = true;
				}
				VSGeometryOcclusionQueryData Temp(pGeometry,pCuller);
				Temp.OcclusionQueryType = m_OcclusionQueryPool.GetOcclusionQuery();
				(*(m_GOQBufferPointer[uiRenderGroup][0]))[uiCullIndex].AddElement(Temp);
			}
		}
		m_NoVisibleGeometry[uiRenderGroup][uiCullIndex].Clear();
	}
	
	return true;
}
void VSCullerManager::Exchange()
{
	for (unsigned int i = 0 ; i < m_PendingRegister.GetNum() ; i++)
	{
		OnRegisterCuller(m_PendingRegister[i]);
	}
	m_PendingRegister.Clear();
	for (unsigned int i = 0; i < m_PendingDelete.GetNum(); i++)
	{
		OnRegisterCuller(m_PendingDelete[i]);
	}
	m_PendingDelete.Clear();
	for (unsigned int uiRenderGroup = 0; uiRenderGroup < VSCuller::RG_MAX; uiRenderGroup++)
	{
		if (VSResourceManager::ms_bRenderThread)
		{
			GeometryOcclusionQueryDataType * T = m_GOQBufferPointer[uiRenderGroup][2];
			m_GOQBufferPointer[uiRenderGroup][2] = m_GOQBufferPointer[uiRenderGroup][0];
			m_GOQBufferPointer[uiRenderGroup][0] = m_GOQBufferPointer[uiRenderGroup][1];
			m_GOQBufferPointer[uiRenderGroup][1] = T;
		}
	}
}
VSCullerManager::VSCullerManager()
{
	VSResourceManager::GetDelayUpdateNoTimeObject().AddMethod<VSCullerManager,&VSCullerManager::Exchange>(this);
	for (unsigned int uiRenderGroup = 0; uiRenderGroup < VSCuller::RG_MAX; uiRenderGroup++)
	{
		m_GOQBufferPointer[uiRenderGroup][0] = &m_GeometryOcclusionQueryBuffer[uiRenderGroup][0];
		if (VSResourceManager::ms_bRenderThread)
		{
			m_GOQBufferPointer[uiRenderGroup][1] = &m_GeometryOcclusionQueryBuffer[uiRenderGroup][1];
			m_GOQBufferPointer[uiRenderGroup][2] = &m_GeometryOcclusionQueryBuffer[uiRenderGroup][2];
		}
		else
		{
			m_GOQBufferPointer[uiRenderGroup][1] = &m_GeometryOcclusionQueryBuffer[uiRenderGroup][0];
			m_GOQBufferPointer[uiRenderGroup][2] = &m_GeometryOcclusionQueryBuffer[uiRenderGroup][0];
		}
	}
}
VSCullerManager::~VSCullerManager()
{
	VSResourceManager::GetDelayUpdateNoTimeObject().RemoveMethod<VSCullerManager, &VSCullerManager::Exchange>(this);
}
bool VSOcclusionQueryType::GetQueryData(void * pData, unsigned int uiDataSize)
{
	return pQuery->GetQueryData(pData, uiDataSize);
}
void VSCullerManager::GetQueryData()
{
	ADD_TIME_PROFILE(OcclusionQuery);
	struct GOQBufferPointerGroupType
	{
		GeometryOcclusionQueryDataType *GOQBufferPointer[VSCuller::RG_MAX];
	};
	GOQBufferPointerGroupType GOQBufferPointerGroup;
	for (unsigned int uiRenderGroup = 0; uiRenderGroup < VSCuller::RG_MAX; uiRenderGroup++)
	{
		GOQBufferPointerGroup.GOQBufferPointer[uiRenderGroup] = (m_GOQBufferPointer[uiRenderGroup][2]);
	}
	for (unsigned int uiCullIndex = 0; uiCullIndex < m_CullerArray.GetNum(); uiCullIndex++)
	{
		ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSQueryDataCommand,
		GOQBufferPointerGroupType, GOQBufferPointerGroup, GOQBufferPointerGroup,
		unsigned int, uiCullIndex, uiCullIndex,
			{
				for (unsigned int uiRenderGroup = 0; uiRenderGroup < VSCuller::RG_MAX; uiRenderGroup++)
				{
					for (unsigned int i = 0; i < (*(GOQBufferPointerGroup.GOQBufferPointer[uiRenderGroup]))[uiCullIndex].GetNum(); i++)
					{
						VSGeometryOcclusionQueryData & OQD = (*(GOQBufferPointerGroup.GOQBufferPointer[uiRenderGroup]))[uiCullIndex][i];
						unsigned int Data;
						if (!OQD.OcclusionQueryType.GetQueryData(&Data, sizeof(Data)))
						{
							OQD.uiPixedNum = VSMAX_UINT32;
						}
						else
						{
							OQD.uiPixedNum = Data;
						}
					}
				}

			})
	}
}
void VSGeometryOcclusionQueryData::ComputeVisible()
{
	if (uiPixedNum < 10)
	{
		bVisible = false;
	}
}
void VSCullerManager::FillGeometryOcclusionQueryData()
{		
	GetQueryData();
	for (unsigned int uiCullIndex = 0; uiCullIndex < m_CullerArray.GetNum(); uiCullIndex++)
	{
		for (unsigned int uiRenderGroup = 0; uiRenderGroup < VSCuller::RG_MAX; uiRenderGroup++)
		{		
			for (unsigned int i = 0; i < (*(m_GOQBufferPointer[uiRenderGroup][0]))[uiCullIndex].GetNum(); i++)
			{
				VSGeometryOcclusionQueryData & OQD = (*(m_GOQBufferPointer[uiRenderGroup][0]))[uiCullIndex][i];
				OQD.ComputeVisible();
				if (!OQD.bVisible)
				{
					m_NoVisibleGeometry[uiRenderGroup][uiCullIndex].AddElement(OQD.pGeometry);
				}
			}
			(*(m_GOQBufferPointer[uiRenderGroup][0]))[uiCullIndex].Clear();
		}
	}
	m_OcclusionQueryPool.ReleaseAllOcclusionQuery();
}
VSArray<VSGeometryOcclusionQueryData> & VSCullerManager::GetGroupGeometryOcclusionQueryData(VSCuller * pCuller, unsigned int uiGroup)
{
	unsigned int uiCullIndex = m_CullerArray.FindElement(pCuller);
	if (uiCullIndex == m_CullerArray.GetNum())
	{
		VSMAC_ASSERT(0);
	}
	return (*(m_GOQBufferPointer[uiGroup][0]))[uiCullIndex];
}
void VSCullerManager::RegisterCuller(VSCuller * pCuller)
{
	m_PendingRegister.AddElement(pCuller);
}
void VSCullerManager::UnRegisterCuller(VSCuller * pCuller)
{
	m_PendingDelete.AddElement(pCuller);
}
bool VSCullerManager::IsRegister(VSCuller * pCuller)
{
	unsigned int uiCullIndex = m_CullerArray.FindElement(pCuller);
	if (uiCullIndex == m_CullerArray.GetNum())
	{
		return false;
	}
	return true;
}
void VSCullerManager::OnRegisterCuller(VSCuller * pCuller)
{
	unsigned int uiCullIndex = m_CullerArray.FindElement(pCuller);
	if (uiCullIndex == m_CullerArray.GetNum())
	{
		m_CullerArray.AddElement(pCuller);
		for (unsigned int uiRenderGroup = 0; uiRenderGroup < VSCuller::RG_MAX; uiRenderGroup++)
		{
			VSArray<VSGeometryOcclusionQueryData> Temp;
			m_GeometryOcclusionQueryBuffer[uiRenderGroup][0].AddElement(Temp);
			m_GeometryOcclusionQueryBuffer[uiRenderGroup][1].AddElement(Temp);
			m_GeometryOcclusionQueryBuffer[uiRenderGroup][2].AddElement(Temp);
			VSArrayOrder<VSGeometry *> T;
			m_NoVisibleGeometry[uiRenderGroup].AddElement(T);
		}
	}
}
void VSCullerManager::OnUnRegisterCuller(VSCuller * pCuller)
{
	unsigned int uiCullIndex = m_CullerArray.FindElement(pCuller);
	if (uiCullIndex != m_CullerArray.GetNum())
	{
		m_CullerArray.Erase(uiCullIndex);
		for (unsigned int uiRenderGroup = 0; uiRenderGroup < VSCuller::RG_MAX; uiRenderGroup++)
		{
			m_GeometryOcclusionQueryBuffer[uiRenderGroup][0].Erase(uiCullIndex);
			m_GeometryOcclusionQueryBuffer[uiRenderGroup][1].Erase(uiCullIndex);
			m_GeometryOcclusionQueryBuffer[uiRenderGroup][2].Erase(uiCullIndex);
			m_NoVisibleGeometry[uiRenderGroup].Erase(uiCullIndex);
		}
	}
}