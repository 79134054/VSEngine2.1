#include "VSAnimBlendFunction.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI_NoCreateFun(VSAnimBlendFunction,VSAnimFunction)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSAnimBlendFunction)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(VSAnimBlendFunction,VSAnimFunction)
END_ADD_PROPERTY
VSAnimBlendFunction::VSAnimBlendFunction()
{

}
VSAnimBlendFunction::VSAnimBlendFunction(const VSUsedName & ShowName,VSAnimTree * pAnimTree)
:VSAnimFunction(ShowName,pAnimTree)
{


	VSString OutputName = _T("Output");
	VSOutputNode * pOutputNode = NULL;
	pOutputNode = VS_NEW VSOutputNode(VSPutNode::AVT_ANIM,OutputName,this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);

	

}
VSAnimBlendFunction::~VSAnimBlendFunction()
{
	
}
bool VSAnimBlendFunction::Update(double dAppTime)
{
	if (!VSAnimFunction::Update(dAppTime))
	{
		return 0;

	}
	if(!ComputePara(dAppTime))
		return 0;
	if(!ComputeOutBoneMatrix(dAppTime))
		return 0;
	return 1;
}
void VSAnimBlendFunction::AdditiveBlend(VSAnimFunction * pOut, VSAnimFunction * pAnimFunction1, VSAnimFunction * pAnimFunction2)
{
	VSMAC_ASSERT(pOut);
	if (pOut->m_bOnlyUpdateRootMotion)
	{
		return;
	}
	unsigned int uiBoneNum = pOut->m_BoneOutput.GetNum();
	VSMAC_ASSERT(uiBoneNum);

	if (pAnimFunction1 && pAnimFunction2)
	{

		VSMAC_ASSERT(uiBoneNum == pAnimFunction1->m_BoneOutput.GetNum());

		VSMAC_ASSERT(uiBoneNum == pAnimFunction2->m_BoneOutput.GetNum());


		for (unsigned int i = 0; i < uiBoneNum; i++)
		{
			pOut->m_BoneOutput[i] = pAnimFunction1->m_BoneOutput[i] + pAnimFunction2->m_BoneOutput[i];
		}
	}
	else if (pAnimFunction1)
	{
		VSMAC_ASSERT(uiBoneNum == pAnimFunction1->m_BoneOutput.GetNum());

		for (unsigned int i = 0; i < uiBoneNum; i++)
		{
			pOut->m_BoneOutput[i] = pAnimFunction1->m_BoneOutput[i];
		}
	}
	else if (pAnimFunction2)
	{
		VSMAC_ASSERT(uiBoneNum == pAnimFunction2->m_BoneOutput.GetNum());


		for (unsigned int i = 0; i < uiBoneNum; i++)
		{
			pOut->m_BoneOutput[i] = pAnimFunction2->m_BoneOutput[i];
		}
	}
}
void VSAnimBlendFunction::LineBlendTwo(VSAnimFunction * pOut,
									   VSAnimFunction * pAnimFunction1,VSAnimFunction * pAnimFunction2,
									   VSREAL fWeight)
{

	VSMAC_ASSERT(pOut && fWeight >= 0.0f && fWeight <= 1.0f);
	if (pOut->m_bOnlyUpdateRootMotion)
	{
		return;
	}
	unsigned int uiBoneNum = pOut->m_BoneOutput.GetNum();
	VSMAC_ASSERT(uiBoneNum);

	
	if (pAnimFunction1 && pAnimFunction2)
	{

		VSMAC_ASSERT(uiBoneNum == pAnimFunction1->m_BoneOutput.GetNum());
		VSMAC_ASSERT(uiBoneNum == pAnimFunction2->m_BoneOutput.GetNum())

		for (unsigned int i = 0 ; i < uiBoneNum ;i++)
		{
			pOut->m_BoneOutput[i].Interpolation(pAnimFunction1->m_BoneOutput[i],pAnimFunction2->m_BoneOutput[i],fWeight);
		}

		GetAnimDataInstance(pAnimFunction1, pOut, 1.0f - fWeight);
		GetAnimDataInstance(pAnimFunction2, pOut, fWeight);
	}
	else if (pAnimFunction1)
	{
		VSMAC_ASSERT(uiBoneNum == pAnimFunction1->m_BoneOutput.GetNum());

		for (unsigned int i = 0 ; i < uiBoneNum ;i++)
		{
			pOut->m_BoneOutput[i] = pAnimFunction1->m_BoneOutput[i];
		}
		GetAnimDataInstance(pAnimFunction1, pOut, 1.0f);
	}
	else if (pAnimFunction2)
	{
		VSMAC_ASSERT(uiBoneNum == pAnimFunction2->m_BoneOutput.GetNum());

		for (unsigned int i = 0 ; i < uiBoneNum ;i++)
		{
			pOut->m_BoneOutput[i] = pAnimFunction2->m_BoneOutput[i];
		}
		GetAnimDataInstance(pAnimFunction2, pOut, 1.0f);
	}
}
void VSAnimBlendFunction::LineBlendTwoRoot(VSAnimFunction * pOut,
	VSAnimFunction * pAnimFunction1, VSAnimFunction * pAnimFunction2, VSREAL fWeight)
{
	VSMAC_ASSERT(fWeight >= 0.0f && fWeight <= 1.0f);


	if (pAnimFunction1 && pAnimFunction2)
	{
		pOut->m_bHaveRootMotion = pAnimFunction1->m_bHaveRootMotion | pAnimFunction2->m_bHaveRootMotion;
		if (pAnimFunction1->m_bHaveRootMotion && pAnimFunction2->m_bHaveRootMotion)
		{
			pOut->m_RootAtom.Interpolation(pAnimFunction1->m_RootAtom, pAnimFunction2->m_RootAtom, fWeight);
		}
		else if (pAnimFunction1->m_bHaveRootMotion)
		{
			pOut->m_RootAtom = pAnimFunction1->m_RootAtom;
		}
		else if (pAnimFunction2->m_bHaveRootMotion)
		{
			pOut->m_RootAtom = pAnimFunction2->m_RootAtom;
		}
	}
	else if (pAnimFunction1)
	{
		
		if (pAnimFunction1->m_bHaveRootMotion)
		{
			pOut->m_bHaveRootMotion = pAnimFunction1->m_bHaveRootMotion;
			pOut->m_RootAtom = pAnimFunction1->m_RootAtom;
		}
	}
	else if (pAnimFunction2)
	{
		if (pAnimFunction2->m_bHaveRootMotion)
		{
			pOut->m_bHaveRootMotion = pAnimFunction2->m_bHaveRootMotion;
			pOut->m_RootAtom = pAnimFunction2->m_RootAtom;
		}
	}
}
void VSAnimBlendFunction::LineBlendTwoAll(VSAnimFunction * pOut,
	VSAnimFunction * pAnimFunction1, VSAnimFunction * pAnimFunction2, VSREAL fWeight)
{
	LineBlendTwo(pOut,pAnimFunction1,pAnimFunction2,fWeight);
	LineBlendTwoRoot(pOut, pAnimFunction1, pAnimFunction2, fWeight);
}




