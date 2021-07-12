#include "VSSkeletonMeshComponent.h"
#include "VSSkeletonMeshNode.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSSkeletonMeshComponent, VSMeshComponent)
BEGIN_ADD_PROPERTY(VSSkeletonMeshComponent, VSMeshComponent)
REGISTER_PROPERTY(m_pSkeletonMeshResource, SkeletonMeshResource, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSSkeletonMeshComponent)
IMPLEMENT_INITIAL_END
VSSkeletonMeshComponent::VSSkeletonMeshComponent()
{
	m_bIsStatic = false;
	m_pSkeletonMeshResource = (VSSkeletonMeshNodeR *)VSSkeletonMeshNode::GetDefaultResource();
}
VSSkeletonMeshComponent::~VSSkeletonMeshComponent()
{
	if (m_pSkeletonMeshResource)
	{
		m_pSkeletonMeshResource->DeleteLoadEventObject(this);
	}
}
bool VSSkeletonMeshComponent::BeforeSave(void * pData)
{
	return true;
}
void VSSkeletonMeshComponent::SetSkeletonMeshResource(VSSkeletonMeshNodeR * pSkeletonMeshResource)
{
	if (m_pSkeletonMeshResource == pSkeletonMeshResource)
	{
		return;
	}
	m_pSkeletonMeshResource = pSkeletonMeshResource;
	m_UseID.Destroy();
	PostCreate();
}
void VSSkeletonMeshComponent::PostCreate()
{
	if (!m_pSkeletonMeshResource)
	{
		return;
	}
	m_pSkeletonMeshResource->AddLoadEventObject(this);
}
void VSSkeletonMeshComponent::LoadedEvent(VSResourceProxyBase * pResourceProxy, void * Data)
{

	if (m_pNode)
	{
		m_pNode->SetParent(NULL);
	}	
	m_pNode = (VSModelMeshNode *)VSObject::CloneCreateObject(m_pSkeletonMeshResource->GetResource());
	m_pNode->SetParent(this);
	if (m_pSkeletonMeshResource->IsLoaded())
	{
		ResetUseID();
	}
	SetPostLoadNodeParam();

}
void VSSkeletonMeshComponent::SetIsDrawSkeleton(bool bIsDrawSkeleton)
{
	VSMeshNode * pMeshNode = m_pNode;
	if (pMeshNode)
	{
		((VSSkeletonMeshNode *)pMeshNode)->SetIsDrawSkeleton(bIsDrawSkeleton);
	}
	
}
bool VSSkeletonMeshComponent::PlayAnim(const VSString & AnimName, VSREAL fRatio, unsigned int uiRepeatType)
{
	VSMeshNode * pMeshNode = m_pNode;
	if (pMeshNode)
	{
		((VSSkeletonMeshNode *)pMeshNode)->PlayAnim(AnimName,fRatio,uiRepeatType);
	}
	return true;
}
VSSocketNode * VSSkeletonMeshComponent::GetSocketNode(const VSUsedName & SocketName)
{
	VSSkeletonMeshNode * pSkeletonNode = (VSSkeletonMeshNode *)m_pNode.GetObject();
	if (pSkeletonNode)
	{
		return pSkeletonNode->GetSocket(SocketName);
	}
	return NULL;
}
void VSSkeletonMeshComponent::SetAnimTreeNodePara(const VSUsedName & ShowName, void * pPara)
{

	VSSkeletonMeshNode * pSkeletonNode = (VSSkeletonMeshNode *)m_pNode.GetObject();
	if (pSkeletonNode)
	{
		return pSkeletonNode->SetAnimTreeNodePara(ShowName, pPara);
	}
}
bool VSSkeletonMeshComponent::PostLoad(VSStream* pStream)
{
	if (!VSMeshComponent::PostLoad(pStream))
	{
		return false;
	}
	PostCreate();
	return true;
}
const VSAnimAtom &VSSkeletonMeshComponent::GetRootDelta()
{
	VSSkeletonMeshNode * pSkeletonNode = (VSSkeletonMeshNode *)m_pNode.GetObject();
	if (pSkeletonNode)
	{
		return pSkeletonNode->GetRootDelta();
	}
	return VSAnimAtom::ms_Identity;
}
