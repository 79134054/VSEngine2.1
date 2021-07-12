#include "VSStaticMeshComponent.h"
#include "VSStaticMeshNode.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSStaticMeshComponent, VSMeshComponent)
BEGIN_ADD_PROPERTY(VSStaticMeshComponent, VSMeshComponent)
REGISTER_PROPERTY(m_pStaticMeshResource, StaticMeshResource, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSStaticMeshComponent)
IMPLEMENT_INITIAL_END
VSStaticMeshComponent::VSStaticMeshComponent()
{
	m_pStaticMeshResource = (VSStaticMeshNodeR *)VSStaticMeshNode::GetDefaultResource();
}
VSStaticMeshComponent::~VSStaticMeshComponent()
{
	if (m_pStaticMeshResource)
	{
		m_pStaticMeshResource->DeleteLoadEventObject(this);
	}
}
bool VSStaticMeshComponent::BeforeSave(void * pData)
{
	return true;
}
void VSStaticMeshComponent::SetStaticMeshResource(VSStaticMeshNodeR * pStaticMeshResource)
{
	if (m_pStaticMeshResource == pStaticMeshResource)
	{
		return;
	}
	m_pStaticMeshResource = pStaticMeshResource;
	m_UseID.Destroy();
	PostCreate();
}
void VSStaticMeshComponent::PostCreate()
{
	if (!m_pStaticMeshResource)
	{
		return;
	}
	m_pStaticMeshResource->AddLoadEventObject(this);
}
void VSStaticMeshComponent::LoadedEvent(VSResourceProxyBase * pResourceProxy, void * Data)
{

	if (m_pNode)
	{
		m_pNode->SetParent(NULL);
	}
	m_pNode = (VSModelMeshNode *)VSObject::CloneCreateObject(m_pStaticMeshResource->GetResource());
	m_pNode->SetParent(this);
	if (m_pStaticMeshResource->IsLoaded())
	{
		ResetUseID();
	}
	SetPostLoadNodeParam();
	m_bIsStatic = !m_pNode->IsDynamic();

}
bool VSStaticMeshComponent::PostLoad(VSStream* pStream)
{
	if (!VSMeshComponent::PostLoad(pStream))
	{
		return false;
	}
	PostCreate();
	return true;
}
