#include "VSAnimFunction.h"
#include "VSBoneNode.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI_NoCreateFun(VSAnimFunction,VSAnimBaseFunction)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSAnimFunction)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(VSAnimFunction,VSAnimBaseFunction)
END_ADD_PROPERTY
VSAnimFunction::VSAnimFunction()
{
	m_BoneOutput.Clear();

}
VSAnimFunction::VSAnimFunction(const VSUsedName & ShowName,VSAnimTree * pAnimTree)
:VSAnimBaseFunction(ShowName,pAnimTree)
{

}
bool VSAnimFunction::SetObject(VSObject * pObject)
{
	if(VSAnimBaseFunction::SetObject(pObject))
	{
		const VSSkeletonMeshNode* pMesh = GetSkeletonMeshNode(); 
		VSMAC_ASSERT(pMesh);

		VSSkeleton * pSkeleton = pMesh->GetSkeleton();
		VSMAC_ASSERT(pSkeleton);

		unsigned int BoneNum = pSkeleton->GetBoneNum();
		m_BoneOutput.SetBufferNum(BoneNum);
		for(unsigned int i = 0 ; i < m_BoneOutput.GetNum() ; i++)
		{
			m_BoneOutput[i].FromTransform(pSkeleton->GetBoneNode(i)->GetLocalTransform());

		}
		return true;
	}

	
	return false;
}
VSAnimFunction::~VSAnimFunction()
{
	m_BoneOutput.Clear();
}
bool VSAnimFunction::IsSupportSimpleInstance()
{ 
	for (unsigned int i = 0; i < m_pInput.GetNum(); i++)
	{
		if (m_pInput[i]->GetOutputLink())
		{
			VSAnimBaseFunction *pAnimBaseFunction = (VSAnimBaseFunction *)m_pInput[i]->GetOutputLink()->GetOwner();
			if (!pAnimBaseFunction->IsSupportSimpleInstance())
			{
				return false;
			}
		}
	}
	return true;
}
bool VSAnimFunction::Update(double dAppTime)
{
	if (!VSAnimBaseFunction::Update(dAppTime))
		return false;

	for (unsigned int i = 0; i < m_pInput.GetNum(); i++)
	{
		if (m_pInput[i]->GetOutputLink())
		{
			VSAnimBaseFunction *pAnimBaseFunction = (VSAnimBaseFunction *)m_pInput[i]->GetOutputLink()->GetOwner();
			if (pAnimBaseFunction)
			{
				pAnimBaseFunction->Update(dAppTime);
			}
		}
	}
	return true;
}
