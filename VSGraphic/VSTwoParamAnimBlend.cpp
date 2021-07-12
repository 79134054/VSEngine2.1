#include "VSTwoParamAnimBlend.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSTwoParamAnimBlend, VSAnimBlendFunction)
IMPLEMENT_INITIAL_BEGIN(VSTwoParamAnimBlend)
IMPLEMENT_INITIAL_END
BEGIN_ADD_PROPERTY(VSTwoParamAnimBlend,VSAnimBlendFunction)
REGISTER_PROPERTY(m_fParamMax[0],ParamMaxX,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_fParamMax[1],ParamMaxY,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_fParamMin[0],ParamMinX,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_fParamMin[1],ParamMinY,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiWidth, uiWidth, VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiHeight, uiHeight, VSProperty::F_SAVE_LOAD_CLONE);
END_ADD_PROPERTY
VSTwoParamAnimBlend::~VSTwoParamAnimBlend()
{

}
VSTwoParamAnimBlend::VSTwoParamAnimBlend()
{
	m_fParam[0] = m_fParam[1] =0.0f;
	m_fParamMax[0] = m_fParamMax[0] =1.0f;
	m_fParamMin[1] = m_fParamMin[1] =-1.0f;
	m_uiWidth = 0;
	m_uiHeight = 0;
}
VSTwoParamAnimBlend::VSTwoParamAnimBlend(const VSUsedName & ShowName,VSAnimTree * pAnimTree)
:VSAnimBlendFunction(ShowName,pAnimTree)
{
	m_fParam[0] = m_fParam[1] =0.0f;
	m_fParamMax[0] = m_fParamMax[0] =1.0f;
	m_fParamMin[1] = m_fParamMin[1] =-1.0f;
	m_uiWidth = 0;
	m_uiHeight = 0;
	unsigned int BoneNum = GetSkeletonMeshNode()->GetSkeleton()->GetBoneNum();
	m_BlendBoneMatrix[0].SetBufferNum(BoneNum);
	m_BlendBoneMatrix[1].SetBufferNum(BoneNum);
}
bool VSTwoParamAnimBlend::ComputePara(double dAppTime)
{
	for (unsigned int i = 0 ; i < 2 ; i++)
	{
		if (m_fParam[i] < m_fParamMin[i])
		{
			m_fParam[i] = m_fParamMin[i];
		}
		else if (m_fParam[i] > m_fParamMax[i])
		{
			m_fParam[i] = m_fParamMax[i];
		}
	}
	
	return 1;
}
bool VSTwoParamAnimBlend::SetObject(VSObject * pObject)
{
	if (VSAnimBlendFunction::SetObject(pObject))
	{
		const VSSkeletonMeshNode* pMesh = GetSkeletonMeshNode();
		VSMAC_ASSERT(pMesh);

		VSSkeleton * pSkeleton = pMesh->GetSkeleton();
		VSMAC_ASSERT(pSkeleton);

		unsigned int BoneNum = pSkeleton->GetBoneNum();
		m_BlendBoneMatrix[0].SetBufferNum(BoneNum);
		m_BlendBoneMatrix[1].SetBufferNum(BoneNum);
		return true;
	}


	return false;
}
void VSTwoParamAnimBlend::CreateSlot(unsigned int uiWidth, unsigned int uiHeight)
{
	m_pInput.Clear();
	m_uiWidth = uiWidth;
	m_uiHeight = uiHeight;
	for (unsigned int i = 0; i < uiWidth; i++)
	{
		for (unsigned int j = 0; j < uiHeight; j++)
		{
			VSString InputName = _T("Child");
			VSString ID = IntToString(i);
			InputName += ID;
			ID = IntToString(j);
			InputName += ID;
			VSInputNode * pInputNode = NULL;
			pInputNode = VS_NEW VSInputNode(VSPutNode::AVT_ANIM, InputName, this);
			VSMAC_ASSERT(pInputNode);
			m_pInput.AddElement(pInputNode);
		}
	}
}
bool VSTwoParamAnimBlend::ComputeOutBoneMatrix(double dAppTime)
{
	if (m_pInput.GetNum() == 0)
	{
		return false;
	}

	unsigned int Index1[2] = { 0, 0 };
	unsigned int Index2[2] = { 0, 0 };
	VSREAL fWeight[2] = { 0.0f, 0.0f };
	if (m_uiWidth > 1)
	{
		VSREAL fInternal = m_fParamMax[0] - m_fParamMin[0];
		VSREAL fInternalSeg = fInternal / (m_uiWidth - 1);
		if (fInternalSeg < EPSILON_E4)
		{
			return 0;
		}


		VSREAL fTemp = (m_fParam[0] - m_fParamMin[0]) / fInternalSeg;
		Index1[0] = (unsigned int)fTemp;
		Index2[0] = Index1[0] + 1;
		fWeight[0] = fTemp - (VSREAL)Index1[0];
	}
	if (m_uiHeight > 1)
	{
		VSREAL fInternal = m_fParamMax[1] - m_fParamMin[1];
		VSREAL fInternalSeg = fInternal / (m_uiHeight - 1);
		if (fInternalSeg < EPSILON_E4)
		{
			return 0;
		}


		VSREAL fTemp = (m_fParam[1] - m_fParamMin[1]) / fInternalSeg;
		Index1[1] = (unsigned int)fTemp;
		Index2[1] = Index1[1] + 1;
		fWeight[1] = fTemp - (VSREAL)Index1[1];
	}

	VSAnimFunction *pAnimBaseFunction1 = GetAnimFunction(Index1[1], Index1[0]);
	VSAnimFunction *pAnimBaseFunction2 = GetAnimFunction(Index1[1], Index2[0]);
	VSAnimFunction *pAnimBaseFunction3 = GetAnimFunction(Index2[1], Index1[0]);
	VSAnimFunction *pAnimBaseFunction4 = GetAnimFunction(Index2[1], Index2[0]);




	if (!m_bOnlyUpdateRootMotion)
	{
		LineBlendTwo(&m_BlendBoneMatrix[0], pAnimBaseFunction1, pAnimBaseFunction2, fWeight[0]);
		LineBlendTwo(&m_BlendBoneMatrix[1], pAnimBaseFunction3, pAnimBaseFunction4, fWeight[0]);
		LineBlendTwo(&m_BoneOutput, &m_BlendBoneMatrix[0], &m_BlendBoneMatrix[1], fWeight[1]);
	}
	m_bHaveRootMotion = pAnimBaseFunction1->m_bHaveRootMotion | pAnimBaseFunction2->m_bHaveRootMotion
		| pAnimBaseFunction3->m_bHaveRootMotion | pAnimBaseFunction4->m_bHaveRootMotion;
	if (m_bHaveRootMotion)
	{
		bool b1 = LineBlendTwoRoot(m_BlendRootMatrix[0], pAnimBaseFunction1, pAnimBaseFunction2, fWeight[0]);
		bool b2 = LineBlendTwoRoot(m_BlendRootMatrix[1], pAnimBaseFunction3, pAnimBaseFunction4, fWeight[0]);
		if (b1 && b2)
		{
			m_RootAtom.Interpolation(m_BlendRootMatrix[0], m_BlendRootMatrix[1], fWeight[1]);
		}
		else if (b1)
		{
			m_RootAtom = m_BlendRootMatrix[0];
		}
		else if (b2)
		{
			m_RootAtom = m_BlendRootMatrix[1];
		}
	}
	return 1;
}
VSAnimFunction * VSTwoParamAnimBlend::GetAnimFunction(unsigned int i, unsigned int j)
{
	VSInputNode* pInputNode1 = GetInputNode(i * m_uiWidth + j);
	if (pInputNode1)
	{
		VSAnimFunction *pAnimBaseFunction1 = (VSAnimFunction *)pInputNode1->GetOutputLink()->GetOwner();
		return pAnimBaseFunction1;
	}
	return NULL;
}
void VSTwoParamAnimBlend::LineBlendTwo(VSArray<VSAnimAtom> *pOut,
	const VSArray<VSAnimAtom> * pIn1, const VSArray<VSAnimAtom> *pIn2,
	VSREAL fWeight)
{

	VSMAC_ASSERT(pOut && fWeight >= 0.0f && fWeight <= 1.0f);

	unsigned int uiBoneNum = pOut->GetNum();
	VSMAC_ASSERT(uiBoneNum);

	if (pIn1 && pIn2)
	{

		VSMAC_ASSERT(uiBoneNum == pIn1->GetNum());

		VSMAC_ASSERT(uiBoneNum == pIn2->GetNum());


		for (unsigned int i = 0; i < uiBoneNum; i++)
		{
			(*pOut)[i].Interpolation((*pIn1)[i], (*pIn2)[i], fWeight);
		}
	}
	else if (pIn1)
	{
		VSMAC_ASSERT(uiBoneNum == pIn1->GetNum());

		for (unsigned int i = 0; i < uiBoneNum; i++)
		{
			(*pOut)[i] = (*pIn1)[i];
		}
	}
	else if (pIn2)
	{
		VSMAC_ASSERT(uiBoneNum == pIn2->GetNum());


		for (unsigned int i = 0; i < uiBoneNum; i++)
		{
			(*pOut)[i] = (*pIn2)[i];
		}
	}
}
void VSTwoParamAnimBlend::LineBlendTwo(VSArray<VSAnimAtom> *pOut,
	VSAnimFunction * pAnimFunction1, VSAnimFunction * pAnimFunction2,
	VSREAL fWeight)
{
	VSMAC_ASSERT(pOut && fWeight >= 0.0f && fWeight <= 1.0f);
	unsigned int uiBoneNum = pOut->GetNum();
	VSMAC_ASSERT(uiBoneNum);

	if (pAnimFunction1 && pAnimFunction2)
	{

		VSMAC_ASSERT(uiBoneNum == pAnimFunction1->m_BoneOutput.GetNum());
		if (uiBoneNum != pAnimFunction2->m_BoneOutput.GetNum())
		{
			return;
		}

		for (unsigned int i = 0; i < uiBoneNum; i++)
		{
			(*pOut)[i].Interpolation(pAnimFunction1->m_BoneOutput[i], pAnimFunction2->m_BoneOutput[i], fWeight);
		}
	}
	else if (pAnimFunction1)
	{
		VSMAC_ASSERT(uiBoneNum == pAnimFunction1->m_BoneOutput.GetNum());

		for (unsigned int i = 0; i < uiBoneNum; i++)
		{
			(*pOut)[i] = pAnimFunction1->m_BoneOutput[i];
		}
	}
	else if (pAnimFunction2)
	{
		VSMAC_ASSERT(uiBoneNum == pAnimFunction2->m_BoneOutput.GetNum());


		for (unsigned int i = 0; i < uiBoneNum; i++)
		{
			(*pOut)[i] = pAnimFunction2->m_BoneOutput[i];
		}
	}
}
bool VSTwoParamAnimBlend::LineBlendTwoRoot(VSAnimAtom &pOut,
	VSAnimFunction * pAnimFunction1, VSAnimFunction * pAnimFunction2, VSREAL fWeight)
{
	VSMAC_ASSERT(fWeight >= 0.0f && fWeight <= 1.0f);


	if (pAnimFunction1 && pAnimFunction2)
	{
		if (pAnimFunction1->m_bHaveRootMotion && pAnimFunction2->m_bHaveRootMotion)
		{
			pOut.Interpolation(pAnimFunction1->m_RootAtom, pAnimFunction2->m_RootAtom, fWeight);
			return true;
		}
		else if (pAnimFunction1->m_bHaveRootMotion)
		{
			pOut = pAnimFunction1->m_RootAtom;
			return true;
		}
		else if (pAnimFunction2->m_bHaveRootMotion)
		{
			pOut = pAnimFunction2->m_RootAtom;
			return true;
		}
		return false;
	}
	else if (pAnimFunction1)
	{

		pOut = pAnimFunction1->m_RootAtom;
		return pAnimFunction1->m_bHaveRootMotion;
	}
	else if (pAnimFunction2)
	{
		pOut = pAnimFunction2->m_RootAtom;
		return pAnimFunction2->m_bHaveRootMotion;
	}
	return false;
}
void VSTwoParamAnimBlend::LineBlendTwoAll(VSTwoParamAnimBlend * pOut, 
	VSAnimFunction * pAnimFunction1, VSAnimFunction * pAnimFunction2, 
	VSAnimFunction * pAnimFunction3, VSAnimFunction * pAnimFunction4, 
	VSREAL fWeight1, VSREAL fWeight2)
{
	if (!pOut->m_bOnlyUpdateRootMotion)
	{
		LineBlendTwo(&pOut->m_BlendBoneMatrix[0], pAnimFunction1, pAnimFunction2, fWeight1);
		LineBlendTwo(&pOut->m_BlendBoneMatrix[1], pAnimFunction3, pAnimFunction4, fWeight1);
		LineBlendTwo(&pOut->m_BoneOutput, &pOut->m_BlendBoneMatrix[0], &pOut->m_BlendBoneMatrix[1], fWeight2);
	}
	pOut->m_bHaveRootMotion = pAnimFunction1->m_bHaveRootMotion | pAnimFunction2->m_bHaveRootMotion
		| pAnimFunction3->m_bHaveRootMotion | pAnimFunction4->m_bHaveRootMotion;
	if (pOut->m_bHaveRootMotion)
	{
		bool b1 = LineBlendTwoRoot(pOut->m_BlendRootMatrix[0], pAnimFunction1, pAnimFunction2, fWeight1);
		bool b2 = LineBlendTwoRoot(pOut->m_BlendRootMatrix[1], pAnimFunction3, pAnimFunction4, fWeight1);
		if (b1 && b2)
		{
			pOut->m_RootAtom.Interpolation(pOut->m_BlendRootMatrix[0], pOut->m_BlendRootMatrix[1], fWeight2);
		}
		else if (b1)
		{
			pOut->m_RootAtom = pOut->m_BlendRootMatrix[0];
		}
		else if (b2)
		{
			pOut->m_RootAtom = pOut->m_BlendRootMatrix[1];
		}
	}

}