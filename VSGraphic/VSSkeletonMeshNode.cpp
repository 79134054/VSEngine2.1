#include "VSSkeletonMeshNode.h"
#include "VSBoneNode.h"
#include "VSAnimTree.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSSkeletonMeshNode,VSModelMeshNode)
BEGIN_ADD_PROPERTY(VSSkeletonMeshNode,VSModelMeshNode)
REGISTER_PROPERTY(m_pSkeleton,Skeleton,VSProperty::F_SAVE_LOAD_CLONE| VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_pAnimSet, AnimSet, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_pAnimTree, pAnimTree, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pSocketArray, SocketArray, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_AnimData, AnimData, VSProperty::F_SAVE_LOAD)
REGISTER_PROPERTY(m_HalfAnimData, HalfAnimData, VSProperty::F_SAVE_LOAD)
REGISTER_PROPERTY(m_uiAnimInstanceTextureWidth, AnimInstanceTextureWidth, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiAnimInstanceTextureHeight, AnimInstanceTextureHeight, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiKeyFrameLengthArrray, m_uiKeyFrameLengthArrray, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_AnimInstanceTexture, AnimInstanceTexture, VSProperty::F_COPY)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSSkeletonMeshNode)
ADD_PRIORITY(VSGeometry)
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION_WITH_PRIORITY(TerminalDefaultState)
IMPLEMENT_INITIAL_END
IMPLEMENT_RESOURCE(VSSkeletonMeshNode)
VSSkeletonMeshNode::VSSkeletonMeshNode()
{
	m_pSkeleton = NULL;
	m_pAnimSet = NULL;
	m_pAnimSequence = NULL;

	m_pAnimTree = NULL;
	m_pAnimTreeInstance = NULL;
	m_uiAnimInstanceTextureHeight = 0;
}
#define ONE_SECOND_FRAME_NUM 30
#define ONE_FRAME_TIME (1000.0f / ONE_SECOND_FRAME_NUM)
#define USE_HALF_ANIM_DATA 1
void VSSkeletonMeshNode::CreateAnimInstanceData()
{

	for (unsigned int i = 0; i < m_pAnimSet->GetAnimNum(); i++)
	{
		VSAnimR * pAnimR = m_pAnimSet->GetAnim(i);

		while (!pAnimR->IsLoaded()) {};
		VSUsedName AnimName = m_pAnimSet->GetAnimName(i);
		VSAnim * pAnim = pAnimR->GetResource();
		unsigned int uiFrameNum = 0;
		for (VSREAL f = 0.0f; f <= pAnim->GetAnimLength() + ONE_FRAME_TIME; f += ONE_FRAME_TIME, uiFrameNum++)
		{
			
		}	
		if (m_uiKeyFrameLengthArrray.GetNum() == 0)
		{
			m_uiKeyFrameLengthArrray.AddElement(uiFrameNum);
		}
		else
		{
			m_uiKeyFrameLengthArrray.AddElement(uiFrameNum + m_uiKeyFrameLengthArrray[m_uiKeyFrameLengthArrray.GetNum() - 1]);
		}
	}

	m_uiAnimInstanceTextureWidth = m_uiKeyFrameLengthArrray[m_uiKeyFrameLengthArrray.GetNum() - 1];
	VSMAC_ASSERT(m_uiAnimInstanceTextureWidth <= 4096);	
	//m_uiAnimInstanceTextureWidth = 1 << (CeilLogTwo(m_uiAnimInstanceTextureWidth));
	m_uiAnimInstanceTextureHeight = m_pSkeleton->GetBoneNum() * 3;
	//m_uiAnimInstanceTextureHeight = 1 << (CeilLogTwo(m_uiAnimInstanceTextureHeight));

#if USE_HALF_ANIM_DATA > 0	
	m_HalfAnimData.SetBufferNum(m_uiAnimInstanceTextureWidth * m_uiAnimInstanceTextureHeight);
	VSHalfVector3W * pTexData = m_HalfAnimData.GetBuffer();
#else
	m_AnimData.SetBufferNum(m_uiAnimInstanceTextureWidth * m_uiAnimInstanceTextureHeight);
	VSVector3W * pTexData = m_AnimData.GetBuffer();	
#endif
	VSArray<VSMatrix3X3W> SaveBoneMatrix;
	SaveBoneMatrix.SetBufferNum(m_pSkeleton->GetBoneNum());
	for (unsigned int i = 0; i < m_pSkeleton->GetBoneNum(); i++)
	{
		VSBoneNode * pBone = m_pSkeleton->GetBoneNode(i);
		if (pBone)
		{
			VSTransform T = pBone->GetLocalTransform();
			SaveBoneMatrix[i] = T.GetCombine();
		}
	}
	unsigned int LastFrame = 0;
	for (unsigned int i = 0; i < m_pAnimSet->GetAnimNum(); i++)
	{
		VSAnimR * pAnimR = m_pAnimSet->GetAnim(i);
		
		VSUsedName AnimName = m_pAnimSet->GetAnimName(i);
		VSAnim * pAnim = pAnimR->GetResource();
		PlayAnim(AnimName.GetString(), 1.0f, VSController::RT_CLAMP);		
		unsigned int uiFrameNum = 0;
		for (VSREAL f = 0.0f; f <= pAnim->GetAnimLength() + ONE_FRAME_TIME; f += ONE_FRAME_TIME, uiFrameNum++)
		{
			unsigned int CurFrame = LastFrame + uiFrameNum;
			unsigned int Col = CurFrame % m_uiAnimInstanceTextureWidth;
			m_pAnimSequence->ClearFlag();
			m_pAnimSequence->Update(f);
			m_pAnimSequence->UpdateBone();
			m_pSkeleton->UpdateAll(0.0f);
#if USE_HALF_ANIM_DATA > 0			
			VSHalfVector3W * pCurTexData = NULL;
#else
			VSVector3W * pCurTexData = NULL;
#endif
			pCurTexData = pTexData + Col;
			for (unsigned int j = 0; j < m_pSkeleton->GetBoneNum(); j++)
			{
				VSBoneNode * pBone = m_pSkeleton->GetBoneNode(j);
				VSMatrix3X3W TempBone;
				if (pBone)
				{
					VSTransform BoneWorldT = pBone->GetWorldTransform();
					if (j == 0)
					{
						BoneWorldT.Indetity();
					}
					TempBone = pBone->GetBoneOffsetMatrix() * BoneWorldT.GetCombine();
				}
				VSVector3W ColumnVector[4];
				TempBone.GetColumnVector(ColumnVector);
#if USE_HALF_ANIM_DATA > 0
				*pCurTexData = FloatToHalf(ColumnVector[0]);
				pCurTexData += m_uiAnimInstanceTextureWidth;
				*pCurTexData = FloatToHalf(ColumnVector[1]);
				pCurTexData += m_uiAnimInstanceTextureWidth;
				*pCurTexData = FloatToHalf(ColumnVector[2]);
				pCurTexData += m_uiAnimInstanceTextureWidth;
#else
				*pCurTexData = ColumnVector[0];
				pCurTexData += m_uiAnimInstanceTextureWidth;
				*pCurTexData = ColumnVector[1];
				pCurTexData += m_uiAnimInstanceTextureWidth;
				*pCurTexData = ColumnVector[2];
				pCurTexData += m_uiAnimInstanceTextureWidth;
#endif
			}
			
		}
		LastFrame = m_uiKeyFrameLengthArrray[i];
	}
	m_pAnimSequence = NULL;
	for (unsigned int i = 0; i < m_pSkeleton->GetBoneNum(); i++)
	{
		VSBoneNode * pBone = m_pSkeleton->GetBoneNode(i);
		if (pBone)
		{
			pBone->SetLocalMat(SaveBoneMatrix[i]);
		}
	}

}
void VSSkeletonMeshNode::UpdateInstanceTexture()
{
	unsigned int uiTextureW = m_uiAnimInstanceTextureWidth;
	unsigned int uiTextureH = m_uiAnimInstanceTextureHeight;
	VSArray<VSVector3W> AnimDataTemp;
	VSArray<VSHalfVector3W> HalfAnimDataTemp;

	bool CopyDirect = true;
	if (!VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_NoPow2Texture))
	{
		if (!IsTwoPower(m_uiAnimInstanceTextureWidth) || !IsTwoPower(m_uiAnimInstanceTextureHeight))
		{
			uiTextureW = 1 << (CeilLogTwo(uiTextureW));
			uiTextureH = 1 << (CeilLogTwo(uiTextureH));
			CopyDirect = false;

			if (m_AnimData.GetNum())
			{
				AnimDataTemp.SetBufferNum(uiTextureW * uiTextureH);
				VSVector3W * pDestBuffer = AnimDataTemp.GetBuffer();
				VSVector3W * pSourceBuffer = m_AnimData.GetBuffer();
				for (unsigned int i = 0; i < m_uiAnimInstanceTextureHeight; i++)
				{
					VSMemcpy(pDestBuffer, pSourceBuffer, uiTextureW * sizeof(VSVector3W));
					pDestBuffer += uiTextureW;
					pSourceBuffer += m_uiAnimInstanceTextureWidth;
				}
				m_AnimData.Destroy();
			}
			else if(m_HalfAnimData.GetNum())
			{
				HalfAnimDataTemp.SetBufferNum(uiTextureW * uiTextureH);
				VSHalfVector3W * pDestBuffer = HalfAnimDataTemp.GetBuffer();
				VSHalfVector3W * pSourceBuffer = m_HalfAnimData.GetBuffer();
				for (unsigned int i = 0; i < m_uiAnimInstanceTextureHeight; i++)
				{
					VSMemcpy(pDestBuffer, pSourceBuffer, uiTextureW * sizeof(VSHalfVector3W));
					pDestBuffer += uiTextureW;
					pSourceBuffer += m_uiAnimInstanceTextureWidth;
				}
				m_HalfAnimData.Destroy();
			}
		}		
	}
	if (CopyDirect)
	{
		if (m_AnimData.GetNum())
		{
			m_AnimInstanceTexture = VSResourceManager::Create2DTexture(uiTextureW, uiTextureH, VSRenderer::SFT_A32B32G32R32F, 1, m_AnimData.GetBuffer());
			m_AnimData.Destroy();
		}
		else if (m_HalfAnimData.GetNum())
		{
			m_AnimInstanceTexture = VSResourceManager::Create2DTexture(uiTextureW, uiTextureH, VSRenderer::SFT_A16B16G16R16F, 1, m_HalfAnimData.GetBuffer());			
			m_HalfAnimData.Destroy();
		}
	}
	else
	{
		if (AnimDataTemp.GetNum())
		{
			m_AnimInstanceTexture = VSResourceManager::Create2DTexture(uiTextureW, uiTextureH, VSRenderer::SFT_A32B32G32R32F, 1, AnimDataTemp.GetBuffer());
			AnimDataTemp.Destroy();
		}
		else if (HalfAnimDataTemp.GetNum())
		{
			m_AnimInstanceTexture = VSResourceManager::Create2DTexture(uiTextureW, uiTextureH, VSRenderer::SFT_A16B16G16R16F, 1, HalfAnimDataTemp.GetBuffer());
			HalfAnimDataTemp.Destroy();
		}
	}
	m_AnimInstanceTexture->SetSamplerState((VSSamplerState*)VSSamplerState::GetClamp());
}
bool VSSkeletonMeshNode::SetDrawInstance(bool bDrawInstance)
{
	if (!m_pAnimSet || !m_pSkeleton)
	{
		m_bDrawInstance = false;
		return false;
	}
	if (!VSModelMeshNode::SetDrawInstance(bDrawInstance))
	{
		return false;
	}
	if (m_bDrawInstance)
	{
		CreateAnimInstanceData();
		return true;
	}
	else
	{
		m_HalfAnimData.Clear();
		m_AnimData.Clear();
		m_uiAnimInstanceTextureHeight = 0;
		m_AnimInstanceTexture = NULL;
		return false;
	}
}
VSSkeletonMeshNode::~VSSkeletonMeshNode()
{
	if (m_pAnimSet)
	{
		m_pAnimSet->m_AddAnimEvent.RemoveMethod<VSSkeletonMeshNode, &VSSkeletonMeshNode::UpdateLocalAABB>(&(*this));
	}
	if (m_pAnimTree)
	{
		m_pAnimTree->DeleteLoadEventObject(this);
	}
	m_pSkeleton = NULL;
	m_pAnimSet = NULL;
	m_pAnimSequence = NULL;
	m_pAnimTree = NULL;
	m_pAnimTreeInstance = NULL;

}
bool VSSkeletonMeshNode::InitialDefaultState()
{
	ms_Default = VS_NEW VSSkeletonMeshNode();
	VSGeometryNodePtr GeometryNode = VS_NEW VSGeometryNode();
	ms_Default->AddChild(GeometryNode);
	VSGeometryPtr Geometry = (VSGeometry *)VSObject::CloneCreateObject(VSGeometry::GetDefaultRenderCube());
	GeometryNode->AddChild(Geometry);
	ms_Default->CreateLocalAABB();
	GeometryNode->SetLocalScale(VSVector3(100.0f, 100.0f, 100.0f));
	ms_DefaultResource = VSSkeletonMeshNodeR::Create(ms_Default);
	return true;
}
bool VSSkeletonMeshNode::TerminalDefaultState()
{
	ms_DefaultResource = NULL;

	ms_Default = NULL;
	return true;
}
void VSSkeletonMeshNode::SetSkeleton(VSSkeleton * pSkeleton)
{	
	if(m_pSkeleton)
	{
		m_pSkeleton->m_pParent = NULL;
	}
	m_pSkeleton = pSkeleton;
	m_pSkeleton->m_pParent = this;
	
}
VSSocketNode * VSSkeletonMeshNode::GetSocket(const VSUsedName &SocketName)
{
	for (unsigned int i = 0; i < m_pSocketArray.GetNum(); i++)
	{
		if (m_pSocketArray[i]->m_cName == SocketName)
		{
			return m_pSocketArray[i];
		}
	}
	return NULL;
}
VSSocketNode * VSSkeletonMeshNode::CreateSocket(const VSUsedName & BoneName, const VSUsedName &SocketName)
{
	VSMAC_ASSERT(m_pSkeleton);

	VSBoneNode * pBone = m_pSkeleton->GetBoneNode(BoneName);
	VSMAC_ASSERT(pBone);


	for (unsigned int i = 0; i < m_pSocketArray.GetNum(); i++)
	{
		if (m_pSocketArray[i]->m_cName == SocketName)
		{
			return NULL;
		}
	}

	VSSocketNode * pSocketNode = VS_NEW VSSocketNode();
	pBone->AddChild(pSocketNode);
	pSocketNode->m_cName = SocketName;
	m_pSocketArray.AddElement(pSocketNode);
	return pSocketNode;
}
void VSSkeletonMeshNode::DeleteSocket(const VSUsedName &SocketName)
{
	for (unsigned int i = 0; i < m_pSocketArray.GetNum(); i++)
	{
		if (m_pSocketArray[i]->m_cName == SocketName)
		{
			VSBoneNode * pSocketParent = (VSBoneNode *)(m_pSocketArray[i]->GetParent());
			VSMAC_ASSERT(pSocketParent);
			pSocketParent->DeleteChild(m_pSocketArray[i]);
			m_pSocketArray.Erase(i);
			return;
		}
	}
}
void VSSkeletonMeshNode::SetAnimSet(VSAnimSet * pAnimSet)
{
	if (m_pAnimSet == pAnimSet)
	{
		return;
	}
	if (m_pAnimSet)
	{
		m_pAnimSet->m_AddAnimEvent.RemoveMethod<VSSkeletonMeshNode, &VSSkeletonMeshNode::UpdateLocalAABB>(&(*this));
	}
	m_pAnimSet = pAnimSet;
	m_pAnimSet->m_AddAnimEvent.AddMethod<VSSkeletonMeshNode, &VSSkeletonMeshNode::UpdateLocalAABB>(&(*this));
	if (m_pAnimTreeInstance)
	{
		m_pAnimTreeInstance->ResetAnimFunction();
	}
	UpdateLocalAABB();
}
void VSSkeletonMeshNode::UpdateLocalAABB()
{
	if (m_pAnimSet && m_pSkeleton)
	{
		VSArray<VSMatrix3X3W> SaveBoneMatrix;
		SaveBoneMatrix.SetBufferNum(m_pSkeleton->GetBoneNum());
		for (unsigned int i = 0; i < m_pSkeleton->GetBoneNum(); i++)
		{
			VSBoneNode * pBone = m_pSkeleton->GetBoneNode(i);
			if (pBone)
			{
				VSTransform T = pBone->GetLocalTransform();
				SaveBoneMatrix[i] = T.GetCombine();
			}
		}

#define SAMPLE_M_SCEOND 200.0f		
		VSVector3 MaxPos = m_pSkeleton->m_OriginLocalBV.GetMaxPoint();
		VSVector3 MinPos = m_pSkeleton->m_OriginLocalBV.GetMinPoint();
		VSTransform SkeletonLocalT = m_pSkeleton->GetLocalTransform();
		for (unsigned int i = 0; i < m_pAnimSet->GetAnimNum(); i++)
		{
			VSAnimR * pAnimR = m_pAnimSet->GetAnim(i);
			while (!pAnimR->IsLoaded()){};

			VSAnim * pAnim = pAnimR->GetResource();
			VSREAL AnimLength = pAnim->GetAnimLength();
			VSUsedName AnimName = m_pAnimSet->GetAnimName(i);
			PlayAnim(AnimName.GetString(), 1.0f, VSController::RT_CLAMP);

			for (VSREAL f = 0.0f; f < AnimLength + SAMPLE_M_SCEOND; f += SAMPLE_M_SCEOND)
			{
				m_pAnimSequence->ClearFlag();
				m_pAnimSequence->Update(f);
				m_pAnimSequence->UpdateBone();
				m_pSkeleton->UpdateAll(0.0f);
				for (unsigned int j = 0; j < m_pSkeleton->GetBoneNum(); j++)
				{
					VSBoneNode * pBone = m_pSkeleton->GetBoneNode(j);
					if (pBone)
					{
						//change to local space
						VSVector3 Pos = pBone->GetWorldTranslate() * SkeletonLocalT.GetCombineInverse();
						for (int t = 0; t < 3; t++)
						{
							if (MinPos.m[t] > Pos.m[t])
							{
								MinPos.m[t] = Pos.m[t];
							}

							if (MaxPos.m[t] < Pos.m[t])
							{
								MaxPos.m[t] = Pos.m[t];
							}
						}

					}
				}
				
			}
		}
		m_pAnimSequence = NULL;
		for (unsigned int i = 0; i < m_pSkeleton->GetBoneNum(); i++)
		{
			VSBoneNode * pBone = m_pSkeleton->GetBoneNode(i);
			if (pBone)
			{
				pBone->SetLocalMat(SaveBoneMatrix[i]);
			}
		}
		VSAABB3 SkeletonBV;
		SkeletonBV.Set(MaxPos, MinPos);
		m_pSkeleton->SetLocalBV(SkeletonBV);
	}
}
void VSSkeletonMeshNode::SetAnimTree(VSAnimTreeR * pAnimTree)
{
	if(pAnimTree)
	{
		m_pAnimTree = pAnimTree;
		m_pAnimTree->AddLoadEventObject(this);
	}
}

void VSSkeletonMeshNode::LoadedEvent(VSResourceProxyBase * pResourceProxy, void * Data)
{
	if (m_pAnimTree == pResourceProxy)
	{
		m_pAnimTreeInstance = (VSAnimTree *)VSObject::CloneCreateObject(m_pAnimTree->GetResource());
		m_pAnimTreeInstance->SetObject(this);
		if (!m_pAnimTreeInstance->IsSupportSimpleInstance() && VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_AdvanceInstance))
		{
			VSMAC_ASSERT(0);
			m_bDrawInstance = false;
		}
	}
}
bool VSSkeletonMeshNode::PostLoad(VSStream* pStream)
{
	VSModelMeshNode::PostLoad(pStream);
	if (m_pAnimTree)
	{
		m_pAnimTree->AddLoadEventObject(this);
	}
	if (m_bDrawInstance && m_uiAnimInstanceTextureHeight && m_uiAnimInstanceTextureWidth)
	{
		UpdateInstanceTexture();
	}
	return true;
}
bool VSSkeletonMeshNode::PostClone(VSObject * pObjectSrc)
{
	VSModelMeshNode::PostClone(pObjectSrc);
	if (m_pAnimTree)
	{
		m_pAnimTree->AddLoadEventObject(this);
	}
	
	return true;
}
void VSSkeletonMeshNode::SetAnimTreeNodePara(const VSUsedName & ShowName, void * pPara)
{
	if (m_pAnimTreeInstance)
	{
		m_pAnimTreeInstance->SetNodePara(ShowName, pPara);
	}
}
void VSSkeletonMeshNode::UpdateAll(double dAppTime)
{
	VSModelMeshNode::UpdateAll(dAppTime);
}
void VSSkeletonMeshNode::UpdateNodeAll(double dAppTime)
{

	if (dAppTime > 0.0f)
	{
		UpdateController(dAppTime);
	}
	
	
	UpdateTransform(dAppTime);

	// no visible no update
	if (m_bEnable && m_pSkeleton)
	{
		m_pSkeleton->UpdateNodeAll(dAppTime);
	}
	else if (m_pSkeleton)
	{
		m_pSkeleton->UpdateNoChild(dAppTime);
	}
	
	for (unsigned int i = 0; i < m_pChild.GetNum(); i++)	
	{ 
		if (m_pChild[i]) 
			m_pChild[i]->UpdateNodeAll(dAppTime); 
	} 

	if(m_bIsChanged)
	{
		UpdateWorldBound(dAppTime); 
	}
	m_bIsChanged = false;

}
void VSSkeletonMeshNode::ComputeNodeVisibleSet(VSCuller & Culler,bool bNoCull,double dAppTime)
{

	if (!Culler.CullConditionNode(this))
	{
		UpdateView(Culler,dAppTime);
		for(unsigned int i = 0 ; i < m_pChild.GetNum() ; i++)
		{
			if(m_pChild[i])
			{

				m_pChild[i]->ComputeVisibleSet(Culler,true,dAppTime);


			}

		}
		if (m_pSkeleton)
		{
			m_pSkeleton->ComputeNodeVisibleSet(Culler, bNoCull, dAppTime);
		}
		
	}

}
void VSSkeletonMeshNode::UpdateWorldBound(double dAppTime)
{
	bool bFoundFirstBound = false; 
	for (unsigned int i = 0; i < m_pChild.GetNum(); i++) 
	{ 	
		if(m_pChild[i])
		{
			if(!bFoundFirstBound)
			{
				m_WorldBV = m_pChild[i]->m_WorldBV;
				bFoundFirstBound = true; 
			}
			else
			{

				m_WorldBV = m_WorldBV.MergeAABB(m_pChild[i]->m_WorldBV);
			}
		}

	}
	if (m_pSkeleton)
	{
		if(!bFoundFirstBound)
		{
			m_WorldBV = m_pSkeleton->m_WorldBV;
			bFoundFirstBound = true; 
		}
		else
		{
			VSVector3 MaxPos = m_pSkeleton->m_WorldBV.GetMaxPoint();
			VSVector3 MinPos = m_pSkeleton->m_WorldBV.GetMinPoint();

			VSREAL fA[3];
			m_WorldBV.GetfA(fA);

			MaxPos = MaxPos + VSVector3(fA[0], fA[1], fA[2]);
			MinPos = MinPos - VSVector3(fA[0], fA[1], fA[2]);
			m_WorldBV.Set(MaxPos, MinPos);
		}
	}
	if (!bFoundFirstBound)
	{
		VSREAL fA[3];
		m_WorldBV.GetfA(fA);
		m_WorldBV.Set(GetWorldTranslate(), fA);
	}
	if (m_pParent)
	{
		m_pParent->m_bIsChanged = true;
	}
}
bool VSSkeletonMeshNode::PlayAnim(const VSString & AnimName,VSREAL fRatio,unsigned int uiRepeatType)
{
	if (m_pAnimSequence == NULL)
	{
		m_pAnimSequence = VS_NEW VSAnimSequenceFunc();
		m_pAnimSequence->SetObject(this);
		m_bIsStatic = false;
	}

	m_pAnimSequence->m_bEnable = true;
	m_pAnimSequence->SetAnim(AnimName);
	m_pAnimSequence->m_uiRepeatType = uiRepeatType;
	m_pAnimSequence->m_dFrequency = fRatio;

	return 1;;
}
void VSSkeletonMeshNode::StopAnim()
{
	if (m_pAnimSequence)
	{
		m_pAnimSequence->m_bEnable = false;
	}
}
void VSSkeletonMeshNode::UpdateController(double dAppTime)
{
	VSModelMeshNode::UpdateController(dAppTime);
	VSArray<ANIM_INSTANCE_DATA> TempAnimInstanceData;
	if (m_pAnimTreeInstance)
	{
		if (m_pAnimSequence)
		{
			m_pAnimSequence->m_bEnable = false;
		}
		m_pAnimTreeInstance->SetOnlyUpdateRootMotion(!m_bEnable);
		m_pAnimTreeInstance->Update(dAppTime);
		if (m_AnimInstanceTexture && m_bDrawInstance)
		{
			m_pAnimTreeInstance->GetAnimInstanceData(TempAnimInstanceData);
		}		
	}
	else if(m_pAnimSequence && m_pAnimSequence->m_bEnable)
	{
		m_pAnimSequence->SetOnlyUpdateRootMotion(!m_bEnable);
		m_pAnimSequence->ClearFlag();
		m_pAnimSequence->Update(dAppTime);
		m_pAnimSequence->UpdateBone();
		if (m_AnimInstanceTexture && m_bDrawInstance)
		{
			m_pAnimSequence->GetAnimInstanceData(TempAnimInstanceData);
		}		
	}
	class ANIM_INSTANCE_DATA_SORT
	{
	public:
		inline bool operator()(ANIM_INSTANCE_DATA & e1, ANIM_INSTANCE_DATA& e2)
		{
			return e1.fAlpha > e2.fAlpha;
		}
	};
	if (m_AnimInstanceTexture && m_bDrawInstance)
	{
		m_AnimInstanceData.Clear();
		if (TempAnimInstanceData.GetNum() > 0)
		{			
			
			if (TempAnimInstanceData.GetNum() > MAX_SIMPLE_INSTANCE_ANIM)
			{
				TempAnimInstanceData.SortAll(ANIM_INSTANCE_DATA_SORT());
				TempAnimInstanceData.Erase(MAX_SIMPLE_INSTANCE_ANIM, TempAnimInstanceData.GetNum() - 1);
				float TotleWeight = 0.0f;
				for (unsigned int i = 0; i < TempAnimInstanceData.GetNum(); i++)
				{
					TotleWeight += TempAnimInstanceData[i].fAlpha;
				}
				for (unsigned int i = 0; i < TempAnimInstanceData.GetNum(); i++)
				{
					TempAnimInstanceData[i].fAlpha /= TotleWeight;
				}
			}
			
			for (unsigned int i = 0; i < TempAnimInstanceData.GetNum(); i++)
			{
				ANIM_INSTANCE_SHADER_DATA T;
				if (TempAnimInstanceData[i].AnimIndex > 0)
				{
					T.AnimIndex = m_uiKeyFrameLengthArrray[TempAnimInstanceData[i].AnimIndex - 1] + TempAnimInstanceData[i].fTime / ONE_FRAME_TIME;
				}
				else
				{
					T.AnimIndex = TempAnimInstanceData[i].fTime / ONE_FRAME_TIME;
				}

				T.fAlpha = TempAnimInstanceData[i].fAlpha;
				m_AnimInstanceData.AddElement(T);
			}
		}
		else
		{

			ANIM_INSTANCE_SHADER_DATA T = { 0.0f,1.0f };
			m_AnimInstanceData.AddElement(T);
		}
	}
}
const VSAnimAtom &VSSkeletonMeshNode::GetRootDelta()
{
	if (m_pAnimTreeInstance)
	{
		return m_pAnimTreeInstance->GetRootDelta();
	}
	if (m_pAnimSequence && m_pAnimSequence->m_bEnable)
	{
		return m_pAnimSequence->m_RootAtom;		
	}
	return VSAnimAtom::ms_Identity;
}
