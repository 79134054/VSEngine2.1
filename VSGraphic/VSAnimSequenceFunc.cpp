#include "VSAnimSequenceFunc.h"
#include "VSBoneNode.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSAnimSequenceFunc,VSAnimFunction)
BEGIN_ADD_PROPERTY(VSAnimSequenceFunc,VSAnimFunction)
REGISTER_PROPERTY(m_AnimName,AnimName,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_bOnlyAddtiveOutput, OnlyAddtiveOutput, VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSAnimSequenceFunc)
IMPLEMENT_INITIAL_END
VSAnimSequenceFunc::VSAnimSequenceFunc(const VSUsedName & ShowName,VSAnimTree * pAnimTree)
					:VSAnimFunction(ShowName,pAnimTree)
{
	VSString OutputName = _T("Output");
	VSOutputNode * pOutputNode = NULL;
	pOutputNode = VS_NEW VSOutputNode(VSPutNode::AVT_ANIM,OutputName,this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);

	m_pAnimR = NULL;
	m_LastKey.Clear();
	m_AdditiveLastKey.Clear();
	m_uiRepeatType = VSController::RT_WRAP;
	m_bOnlyAddtiveOutput = false;
	
}
bool VSAnimSequenceFunc::SetObject(VSObject * pObject)
{
	if(VSAnimFunction::SetObject(pObject))
	{
		VSSkeleton * pSkeleton = GetSkeletonMeshNode()->GetSkeleton();
		VSMAC_ASSERT(pSkeleton);
		unsigned int BoneNum = pSkeleton->GetBoneNum();
		m_UsedBoneIndexInAnim.SetBufferNum(BoneNum);
		m_LastKey.SetBufferNum(BoneNum);
		VSMemset(&m_LastKey[0],0,sizeof(LAST_KEY_TYPE) * BoneNum);
		SetAnim(m_AnimName);
		return true;
	}

	return false;
}
VSAnimSequenceFunc::VSAnimSequenceFunc()
{
	m_pAnimR = NULL;
	m_uiRepeatType = VSController::RT_WRAP;
	m_bOnlyAddtiveOutput = false;
}
VSAnimSequenceFunc::~VSAnimSequenceFunc()
{
	if (m_pAnimR)
	{
		m_pAnimR->DeleteLoadEventObject(this);
		if (m_pAnimR->IsLoaded())
		{
			VSAnim* pAnim = m_pAnimR->GetResource();			
			VSAnimR* pBlendAnim = pAnim->GetBlendAnim();
			if (pBlendAnim)
			{
				pBlendAnim->DeleteLoadEventObject(this);
			}
		}

	}
	m_pAnimR = NULL;
}
void VSAnimSequenceFunc::SetAnim(const VSUsedName&  AnimName)
{
	m_AnimName = AnimName;
	const VSSkeletonMeshNode * pMesh = GetSkeletonMeshNode();
	if(!pMesh)
	{
		return;
	}
	VSSkeleton * pSkeleton = pMesh->GetSkeleton();
	if (!pSkeleton)
	{
		return;
	}

	const VSAnimSet * pAnimSet = pMesh->GetAnimSet();
	if (!pAnimSet)
	{
		return;
	}

	VSAnimRPtr pAnimR = pAnimSet->GetAnim(AnimName);
	if (!pAnimR)
		return ;
	if (m_pAnimR == pAnimR)
	{
		if (m_pAnimR->IsLoaded())
		{
			m_bStart = false;
			m_LastRootAtom = m_RootStartAtom;
			for (unsigned int i = 0; i < m_LastKey.GetNum(); i++)
			{
				m_LastKey[i].Reset();
			}
			for (unsigned int i = 0; i < m_AdditiveLastKey.GetNum(); i++)
			{
				m_AdditiveLastKey[i].Reset();
			}
		}
		return;
	}
	m_pAnimR = pAnimR;
	m_uiIndexInAnimSet = pAnimSet->GetAnimIndex(pAnimR);
	m_pAnimR->AddLoadEventObject(this);
}
void VSAnimSequenceFunc::LoadedEvent(VSResourceProxyBase * pResourceProxy, void * Data)
{
	const VSSkeletonMeshNode * pMesh = GetSkeletonMeshNode();
	VSMAC_ASSERT(pMesh);

	VSSkeleton * pSkeleton = pMesh->GetSkeleton();
	VSMAC_ASSERT(pSkeleton);

	VSAnim * pAnim = m_pAnimR->GetResource();
	if (m_pAnimR == pResourceProxy)
	{
		m_dMaxTime = pAnim->GetAnimLength();
		VSMemset(&m_LastKey[0], 0, sizeof(LAST_KEY_TYPE) * m_LastKey.GetNum());

		for (unsigned int i = 0; i < m_UsedBoneIndexInAnim.GetNum(); i++)
		{
			VSBoneNode * pBone = pSkeleton->GetBoneNode(i);
			if (pBone)
			{
				bool bIsFound = false;
				for (unsigned int j = 0; j < pAnim->GetBoneKeyNum(); j++)
				{
					if (pBone->m_cName == pAnim->GetBoneKey(j)->m_cName)
					{
						m_UsedBoneIndexInAnim[i] = j;
						bIsFound = true;
						break;
					}
				}
				if (!bIsFound)
				{
					m_UsedBoneIndexInAnim[i] = VSMAX_UINT32;
				}
			}

		}
		if (pAnim->IsRootAnim())
		{
			GetRootStartAtom(m_RootStartAtom, pAnim);
			m_LastRootAtom = m_RootStartAtom;
		}
		VSAnimR * pBlendAnim = pAnim->GetBlendAnim();
		if (pBlendAnim)
		{
			m_AdditiveLastKey.Destroy();
			m_AdditiveBoneOutput.Destroy();
			pBlendAnim->AddLoadEventObject(this);
		}
	}
	else if (pAnim->GetBlendAnim() == pResourceProxy)
	{
		m_AdditiveLastKey.SetBufferNum(m_LastKey.GetNum());
		VSMemset(&m_AdditiveLastKey[0], 0, sizeof(LAST_KEY_TYPE)* m_AdditiveLastKey.GetNum());
		m_AdditiveBoneOutput.SetBufferNum(m_UsedBoneIndexInAnim.GetNum());
		for (unsigned int i = 0; i < m_AdditiveBoneOutput.GetNum(); i++)
		{
			m_AdditiveBoneOutput[i] = m_BoneOutput[i];
		}
		if (pAnim->IsRootAnim() && !m_bOnlyAddtiveOutput)
		{
			VSAnim * pBlendAnim = pAnim->GetBlendAnim()->GetResource();
			VSAnimAtom AdditiveRootStartAtom;
			GetRootStartAtom(AdditiveRootStartAtom, pBlendAnim);			
			m_RootStartAtom += AdditiveRootStartAtom;
			m_LastRootAtom = m_RootStartAtom;
		}
	}
}
void VSAnimSequenceFunc::ComputeAnim(VSAnim * pAnim, VSArray<LAST_KEY_TYPE> & LastKey, VSArray<VSAnimAtom> &BoneOutput)
{
	unsigned int uiUpdateBoneNum = m_UsedBoneIndexInAnim.GetNum();
	if (m_bOnlyUpdateRootMotion)
	{
		uiUpdateBoneNum = 1;
	}
	for (unsigned int i = 0; i < uiUpdateBoneNum; i++)
	{
		if (m_UsedBoneIndexInAnim[i] != VSMAX_UINT32)
		{
			VSBoneKey * pBoneKey = pAnim->GetBoneKey(m_UsedBoneIndexInAnim[i]);
			unsigned int ScaleNum = (unsigned int)pBoneKey->m_ScaleArray.GetNum();
			if (ScaleNum)
			{

				if (pBoneKey->m_ScaleArray[LastKey[i].uiLKScale].m_dKeyTime > m_dNowAnimTime)
				{
					LastKey[i].uiLKScale = 0;
				}
				unsigned int j;
				for (j = LastKey[i].uiLKScale; j < ScaleNum; j++)
				{
					if (m_dNowAnimTime <= pBoneKey->m_ScaleArray[j].m_dKeyTime)
					{
						break;
					}
				}
				if (j == ScaleNum)
				{
					j = ScaleNum - 1;
				}
				unsigned int Key1;
				unsigned int Key2;
				if (j == 0)
					Key1 = j;
				else
					Key1 = j - 1;
				Key2 = j;
				LastKey[i].uiLKScale = Key1;
				double dDiff = pBoneKey->m_ScaleArray[Key2].m_dKeyTime - pBoneKey->m_ScaleArray[Key1].m_dKeyTime;
				if (dDiff <= 0.0)
					dDiff = 1.0;
				VSREAL fFactor = (VSREAL)((m_dNowAnimTime - pBoneKey->m_ScaleArray[Key1].m_dKeyTime) / dDiff);

				if (fFactor < 0)
					fFactor = 0;
				if (fFactor > 1.0f)
					fFactor = 1.0f;

				VSVector3 Scale = pBoneKey->m_ScaleArray[Key1].m_Vector * (1 - fFactor) + 
					pBoneKey->m_ScaleArray[Key2].m_Vector * fFactor;
				BoneOutput[i].m_fScale = Scale;

			}

			unsigned int RotatorNum = (unsigned int)pBoneKey->m_RotatorArray.GetNum();
			if (RotatorNum)
			{
				if (pBoneKey->m_RotatorArray[LastKey[i].uiLKRotator].m_dKeyTime > m_dNowAnimTime)
				{
					LastKey[i].uiLKRotator = 0;
				}
				unsigned int j;
				for (j = LastKey[i].uiLKRotator; j < RotatorNum; j++)
				{
					if (m_dNowAnimTime <= pBoneKey->m_RotatorArray[j].m_dKeyTime)
					{
						break;
					}
				}
				if (j == RotatorNum)
				{
					j = RotatorNum - 1;
				}
				unsigned int Key1;
				unsigned int Key2;
				if (j == 0)
					Key1 = j;
				else
					Key1 = j - 1;
				Key2 = j;
				LastKey[i].uiLKRotator = Key1;
				double dDiff = pBoneKey->m_RotatorArray[Key2].m_dKeyTime - pBoneKey->m_RotatorArray[Key1].m_dKeyTime;
				if (dDiff <= 0.0)
					dDiff = 1.0;
				VSREAL fFactor = (VSREAL)((m_dNowAnimTime - pBoneKey->m_RotatorArray[Key1].m_dKeyTime) / dDiff);

				if (fFactor < 0)
					fFactor = 0;
				if (fFactor > 1.0f)
					fFactor = 1.0f;

				VSQuat Rotator;
#ifdef USE_ROTATOR_LINE_INTERPOLATION
				Rotator.Slerp(fFactor, pBoneKey->m_RotatorArray[Key1].m_Quat, pBoneKey->m_RotatorArray[Key2].m_Quat);
#else
				Rotator = LineInterpolation(pBoneKey->m_RotatorArray[Key1].m_Quat, 
					pBoneKey->m_RotatorArray[Key2].m_Quat, fFactor);
#endif
				BoneOutput[i].m_Rotator = Rotator;

			}

			unsigned int TranslationNum = (unsigned int)pBoneKey->m_TranslationArray.GetNum();
			if (TranslationNum)
			{
				if (pBoneKey->m_TranslationArray[LastKey[i].uiLKTranslation].m_dKeyTime > m_dNowAnimTime)
				{
					LastKey[i].uiLKTranslation = 0;
				}
				unsigned int j;
				for (j = LastKey[i].uiLKTranslation; j < TranslationNum; j++)
				{
					if (m_dNowAnimTime <= pBoneKey->m_TranslationArray[j].m_dKeyTime)
					{
						break;
					}
				}
				if (j == TranslationNum)
				{
					j = TranslationNum - 1;
				}
				unsigned int Key1;
				unsigned int Key2;
				if (j == 0)
					Key1 = j;
				else
					Key1 = j - 1;
				Key2 = j;
				LastKey[i].uiLKTranslation = Key1;
				double dDiff = pBoneKey->m_TranslationArray[Key2].m_dKeyTime - 
					pBoneKey->m_TranslationArray[Key1].m_dKeyTime;
				if (dDiff <= 0.0)
					dDiff = 1.0;
				VSREAL fFactor = (VSREAL)((m_dNowAnimTime - pBoneKey->m_TranslationArray[Key1].m_dKeyTime) / dDiff);

				if (fFactor < 0)
					fFactor = 0;
				if (fFactor > 1.0f)
					fFactor = 1.0f;

				VSVector3 Translation = pBoneKey->m_TranslationArray[Key1].m_Vector * (1 - fFactor) 
					+ pBoneKey->m_TranslationArray[Key2].m_Vector * fFactor;
				BoneOutput[i].m_Pos = Translation;
			}
		}
		else
		{
			BoneOutput[i].Identity();
		}

	}
}
bool VSAnimSequenceFunc::IsSupportSimpleInstance()
{
	if (m_pAnimR == NULL)
	{
		return false;
	}
	while (!m_pAnimR->IsLoaded());

	VSAnim * pAnim = m_pAnimR->GetResource();
	if (pAnim->IsAdditive())
	{
		return false;
	}
	return true;
}
bool VSAnimSequenceFunc::Update(double dAppTime)
{
	if(!VSAnimFunction::Update(dAppTime))
		return false;
	
	VSSkeletonMeshNode * pMesh = GetSkeletonMeshNode();
	VSMAC_ASSERT(pMesh);

	VSSkeleton * pSkeleton = pMesh->GetSkeleton();
	VSMAC_ASSERT(pSkeleton);

	if (m_pAnimR == NULL)
	{
		return false;	
	}

	if (!m_pAnimR->IsLoaded())
	{
		return false;
	}				
	VSAnim * pAnim = m_pAnimR->GetResource();
	m_bHaveRootMotion = pAnim->IsRootAnim();
	if (m_bOnlyUpdateRootMotion && !m_bHaveRootMotion)
	{
		return false;
	}
	VSAnim * pBlendAnim = NULL;
	if (pAnim->IsAdditive())
	{
		VSAnimR * pBlendAnimR = pAnim->GetBlendAnim();
		if (!pBlendAnimR->IsLoaded())
		{
			return false;
		}

		pBlendAnim = pBlendAnimR->GetResource();
	}
	
	ComputeAnim(pAnim, m_LastKey, m_BoneOutput);
	
	if (pBlendAnim && !m_bOnlyAddtiveOutput)
	{
		ComputeAnim(pBlendAnim, m_AdditiveLastKey, m_AdditiveBoneOutput);

		for (unsigned int i = 0; i < m_BoneOutput.GetNum();i++)
		{
			m_BoneOutput[i] += m_AdditiveBoneOutput[i];
		}
	}
	if (m_bHaveRootMotion)
	{
		m_RootAtom = m_BoneOutput[0] - m_LastRootAtom;
		m_LastRootAtom = m_BoneOutput[0];
		VSAnimAtom Temp = VSAnimAtom::ms_Identity;
		Temp.m_fScale = m_RootStartAtom.m_fScale;
		m_BoneOutput[0] = Temp;
	}

	ANIM_INSTANCE_DATA InstanceData = { m_uiIndexInAnimSet,(VSREAL)m_dNowAnimTime,1.0f };
	m_AnimDataInstance.AddElement(InstanceData);
	return true;

}
VSREAL VSAnimSequenceFunc::GetAnimTime()
{
	if (!m_bEnable)
	{
		return 0.0f;
	}
	const VSSkeletonMeshNode * pMesh = GetSkeletonMeshNode();
	VSMAC_ASSERT(pMesh);

	VSSkeleton * pSkeleton = pMesh->GetSkeleton();
	VSMAC_ASSERT(pSkeleton);

	if (!m_pAnimR || !m_pAnimR->IsLoaded())
	{
		return 0.0f;
	}
	VSAnim * pAnim = m_pAnimR->GetResource();
	if (pAnim->GetBlendAnim())
	{
		if (!pAnim->GetBlendAnim()->IsLoaded())
		{
			return 0.0f;
		}
	}
	
	return pAnim->GetAnimLength() * (VSREAL)m_dFrequency;
}
void VSAnimSequenceFunc::GetRootStartAtom(VSAnimAtom & OutAtom, VSAnim * pAnim)
{
	if (m_UsedBoneIndexInAnim[0] != VSMAX_UINT32)
	{
		VSBoneKey * pBoneKey = pAnim->GetBoneKey(m_UsedBoneIndexInAnim[0]);
		unsigned int ScaleNum = (unsigned int)pBoneKey->m_ScaleArray.GetNum();
		if (ScaleNum)
		{			
			OutAtom.m_fScale = pBoneKey->m_ScaleArray[0].m_Vector;
		}
		else
		{
			OutAtom.m_fScale = VSVector3::ms_One;
		}
		unsigned int RotatorNum = (unsigned int)pBoneKey->m_RotatorArray.GetNum();
		if (RotatorNum)
		{			
			OutAtom.m_Rotator = pBoneKey->m_RotatorArray[0].m_Quat;
		}
		else
		{
			OutAtom.m_Rotator.Set(0.0f, 0.0f, 0.0f, 1.0f);
		}

		unsigned int TranslationNum = (unsigned int)pBoneKey->m_TranslationArray.GetNum();
		if (TranslationNum)
		{
			OutAtom.m_Pos = pBoneKey->m_TranslationArray[0].m_Vector;
		}
		else
		{
			OutAtom.m_fScale = VSVector3::ms_Zero;
		}
	}
	else
	{
		OutAtom.Identity();
	}
}
void VSAnimSequenceFunc::UpdateBone()
{
	if (!m_bEnable)
	{
		return;
	}
	if (m_bOnlyUpdateRootMotion)
	{
		return;
	}
	VSSkeletonMeshNode * pMesh = GetSkeletonMeshNode();
	VSMAC_ASSERT(pMesh);

	VSSkeleton * pSkeleton = pMesh->GetSkeleton();
	VSMAC_ASSERT(pSkeleton);

	if (!m_pAnimR || !m_pAnimR->IsLoaded())
	{
		return;
	}
	VSAnim * pAnim = m_pAnimR->GetResource();
	if (pAnim->GetBlendAnim())
	{
		if (!pAnim->GetBlendAnim()->IsLoaded())
		{
			return ;
		}
	}

	for (unsigned int i = 0; i < pSkeleton->GetBoneNum(); i++)
	{
		VSBoneNode * pBone = pSkeleton->GetBoneNode(i);
		if (pBone)
		{
			VSMatrix3X3W BoneOutMat;
			m_BoneOutput[i].GetMatrix(BoneOutMat);
			pBone->SetLocalMat(BoneOutMat);
		}
	}

}