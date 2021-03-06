#include "VSAdditiveBlend.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSAdditiveBlend, VSAnimBlendFunction)
BEGIN_ADD_PROPERTY(VSAdditiveBlend, VSAnimBlendFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSAdditiveBlend)
IMPLEMENT_INITIAL_END
VSAdditiveBlend::~VSAdditiveBlend()
{

}
VSAdditiveBlend::VSAdditiveBlend()
{

}
VSAdditiveBlend::VSAdditiveBlend(const VSUsedName & ShowName, VSAnimTree * pAnimTree)
:VSAnimBlendFunction(ShowName, pAnimTree)
{
	VSString InputName0 = _T("Delta");
	VSInputNode * pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::AVT_ANIM, InputName0, this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);


	VSString InputName1 = _T("BlendAnim");
	pInputNode = NULL;
	pInputNode = VS_NEW VSInputNode(VSPutNode::AVT_ANIM, InputName1, this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);
}
void VSAdditiveBlend::AddInputNode()
{
	return;

}
void VSAdditiveBlend::DeleteInputNode()
{
	return;
}
bool VSAdditiveBlend::ComputeOutBoneMatrix(double dAppTime)
{

	VSInputNode* pInputNode1 = GetInputNode(0);
	VSInputNode* pInputNode2 = GetInputNode(1);
	VSAnimFunction *pAnimBaseFunction1 = (VSAnimFunction *)pInputNode1->GetOutputLink()->GetOwner();
	m_bHaveRootMotion = pAnimBaseFunction1->m_bHaveRootMotion;
	if (pInputNode1->GetOutputLink() && pInputNode2->GetOutputLink())
	{
		
		VSAnimFunction *pAnimBaseFunction2 = (VSAnimFunction *)pInputNode2->GetOutputLink()->GetOwner();
		AdditiveBlend(this, pAnimBaseFunction1, pAnimBaseFunction2);		
	}
	else if (pInputNode1->GetOutputLink())
	{
		AdditiveBlend(this, pAnimBaseFunction1,NULL);

	}
	else if (pInputNode2->GetOutputLink())
	{
		VSMAC_ASSERT(0);
	}
	if (m_bHaveRootMotion && m_bOnlyUpdateRootMotion)
	{
		m_RootAtom = pAnimBaseFunction1->m_RootAtom;
	}	
	return 1;
}
