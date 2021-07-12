#include "VSAnimBaseFunction.h"
#include "VSBoneNode.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI_NoCreateFun(VSAnimBaseFunction,VSController)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSAnimBaseFunction)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(VSAnimBaseFunction,VSController)
REGISTER_PROPERTY(m_ShowName,ShowName,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_pInput,Input,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_pOutput,pOutput,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_pOwner,pOwner,VSProperty::F_SAVE_LOAD_CLONE);
END_ADD_PROPERTY
VSAnimBaseFunction::VSAnimBaseFunction()
{
	m_pInput.Clear();
	m_pOutput.Clear();
	m_pOwner = NULL;
	m_bIsVisited = 0;
	m_bOnlyUpdateRootMotion = 0;
	m_bHaveRootMotion = 0;
}
VSAnimBaseFunction::VSAnimBaseFunction(const VSUsedName & ShowName,VSAnimTree * pAnimTree)
{
	m_ShowName = ShowName;
	m_pInput.Clear();
	m_pOutput.Clear();
	VSMAC_ASSERT(pAnimTree);
	m_pOwner = pAnimTree;

	m_pOwner->AddAnimFunction(this);
	m_bIsVisited = 0;

}
VSAnimBaseFunction::~VSAnimBaseFunction()
{
	for(unsigned int i = 0 ; i < m_pInput.GetNum() ; i++)
	{
		VSMAC_DELETE(m_pInput[i]);
	}
	for(unsigned int i = 0 ; i < m_pOutput.GetNum() ; i++)
	{
		VSMAC_DELETE(m_pOutput[i]);
	}
	if (m_pOwner)
	{
		m_pOwner->DeleteAnimFunction(this);
	}
	
}
bool VSAnimBaseFunction::Update(double dAppTime)
{
	if (m_bIsVisited)
		return false;
	m_bIsVisited = 1;

	if (!VSController::Update(dAppTime))
		return false;

	return true;
}
VSSkeletonMeshNode * VSAnimBaseFunction::GetSkeletonMeshNode()const
{
	return DynamicCast<VSSkeletonMeshNode>(m_pObject);
}
VSInputNode * VSAnimBaseFunction::GetInputNode(unsigned int uiNodeID)const
{
	VSMAC_ASSERT(uiNodeID < m_pInput.GetNum());
	return m_pInput[uiNodeID];
	
}
VSInputNode * VSAnimBaseFunction::GetInputNode(const VSString & NodeName)const
{
	for (unsigned int i  =0 ; i < m_pInput.GetNum(); i++)
	{
		if(m_pInput[i]->GetNodeName() == NodeName)
			return m_pInput[i];
	}
	return NULL;
}

VSOutputNode * VSAnimBaseFunction::GetOutputNode(unsigned int uiNodeID)const
{
	VSMAC_ASSERT(uiNodeID < m_pOutput.GetNum());
	return m_pOutput[uiNodeID];
}
VSOutputNode * VSAnimBaseFunction::GetOutputNode(const VSString & NodeName)const
{
	for (unsigned int i  =0 ; i < m_pInput.GetNum(); i++)
	{
		if(m_pOutput[i]->GetNodeName() == NodeName)
			return m_pOutput[i];
	}
	return NULL;
}

void VSAnimBaseFunction::ClearFlag()
{
	m_bIsVisited = false;
	m_bNoLeafStart = false;
	m_bHaveRootMotion = false;
	m_AnimDataInstance.Clear();
}

bool VSAnimBaseFunction::IsLeafNode()
{
	return false;
}
void  VSAnimBaseFunction::GetLeafArray(VSArray<VSAnimBaseFunction *> & LeafNode)
{
	if (IsLeafNode())
	{
		LeafNode.AddElement(this);
	}
	for (unsigned int i = 0; i < m_pInput.GetNum(); i++)
	{
		if (m_pInput[i]->GetOutputLink())
		{
			VSAnimBaseFunction *pAnimBaseFunction = (VSAnimBaseFunction *)m_pInput[i]->GetOutputLink()->GetOwner();
			if (pAnimBaseFunction )
			{
				pAnimBaseFunction->GetLeafArray(LeafNode);
			}
		}
	}
}
void VSAnimBaseFunction::NoLeafStart()
{
	m_bNoLeafStart = true;
}
void VSAnimBaseFunction::EnableLeafStart()
{
	if (m_bNoLeafStart)
	{
		return;
	}
	else
	{
		m_bStart = false;
	}
}
void VSAnimBaseFunction::GetAnimDataInstance(VSAnimBaseFunction * pSource, VSAnimBaseFunction * pTarget, VSREAL fWeight)
{
	for (unsigned int i = 0 ; i < pSource->m_AnimDataInstance.GetNum() ; i++)
	{
		ANIM_INSTANCE_DATA InstanceData = pSource->m_AnimDataInstance[i];
		InstanceData.fAlpha *= fWeight;
		pTarget->m_AnimDataInstance.AddElement(InstanceData);
	}
}