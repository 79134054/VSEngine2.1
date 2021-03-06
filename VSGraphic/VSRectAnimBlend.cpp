#include "VSRectAnimBlend.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSRectAnimBlend,VSTwoParamAnimBlend)
BEGIN_ADD_PROPERTY(VSRectAnimBlend,VSTwoParamAnimBlend)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSRectAnimBlend)
IMPLEMENT_INITIAL_END
VSRectAnimBlend::~VSRectAnimBlend()
{

}
VSRectAnimBlend::VSRectAnimBlend()
{

}
VSRectAnimBlend::VSRectAnimBlend(const VSUsedName & ShowName,VSAnimTree * pAnimTree)
:VSTwoParamAnimBlend(ShowName,pAnimTree)
{


	for (unsigned int i = CL_LEFT_TOP ; i < CL_MAX ; i++)
	{
		VSString InputName = _T("Child");
		VSString ID = IntToString(m_pInput.GetNum()); 
		InputName += ID;
		VSInputNode * pInputNode = NULL;
		pInputNode = VS_NEW VSInputNode(VSPutNode::AVT_ANIM,InputName,this);
		VSMAC_ASSERT(pInputNode);
		m_pInput.AddElement(pInputNode);

	}
	unsigned int BoneNum = GetSkeletonMeshNode()->GetSkeleton()->GetBoneNum();
	m_BlendBoneMatrix[0].SetBufferNum(BoneNum);
	m_BlendBoneMatrix[1].SetBufferNum(BoneNum);
}
bool VSRectAnimBlend::SetObject(VSObject * pObject)
{
	if (VSTwoParamAnimBlend::SetObject(pObject))
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
bool VSRectAnimBlend::ComputeOutBoneMatrix(double dAppTime)
{
	VSREAL fInternal[2];
	for (unsigned int i = 0 ; i < 2 ; i++)
	{
		fInternal[i] = m_fParamMax[i] - m_fParamMin[i];
		
		if (fInternal[i] < EPSILON_E4)
		{
			return 0;
		}
	}
	


	VSREAL fWeight[2];
	for (unsigned int i = 0 ; i < 2 ; i++)
	{
		fWeight[i] = (m_fParam[i] - m_fParamMin[i]) / fInternal[i];
	}

	VSInputNode* pInputNode0 = GetInputNode(0);
	VSInputNode* pInputNode1 = GetInputNode(1);
	VSInputNode* pInputNode2 = GetInputNode(2);
	VSInputNode* pInputNode3 = GetInputNode(3);
	if(pInputNode1->GetOutputLink() && pInputNode2->GetOutputLink() 
		&& pInputNode0->GetOutputLink() && pInputNode3->GetOutputLink())
	{
		VSAnimFunction *pAnimBaseFunction0 = (VSAnimFunction *)pInputNode0->GetOutputLink()->GetOwner();
		VSAnimFunction *pAnimBaseFunction1 = (VSAnimFunction *)pInputNode1->GetOutputLink()->GetOwner();
		VSAnimFunction *pAnimBaseFunction2 = (VSAnimFunction *)pInputNode2->GetOutputLink()->GetOwner();		
		VSAnimFunction *pAnimBaseFunction3 = (VSAnimFunction *)pInputNode3->GetOutputLink()->GetOwner();
		LineBlendTwoAll(this, pAnimBaseFunction0, pAnimBaseFunction1,
			pAnimBaseFunction2, pAnimBaseFunction3, fWeight[0], fWeight[1]);
	}
	
	

	return 1;
}