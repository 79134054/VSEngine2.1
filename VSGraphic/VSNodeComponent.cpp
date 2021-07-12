#include "VSNodeComponent.h"
#include "VSGraphicInclude.h"
#include "VSSkeletonMeshComponent.h"
#include "VSSocketNode.h"
#include "VSStreamingManager.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSNodeComponent, VSNode)
BEGIN_ADD_PROPERTY(VSNodeComponent, VSNode)
REGISTER_PROPERTY(m_AttachSocketName, AttachSocketName, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSNodeComponent)
IMPLEMENT_INITIAL_END
VSNodeComponent::VSNodeComponent()
{
	m_pAttachSocket = NULL;
}
VSNodeComponent::~VSNodeComponent()
{
	
}
void VSNodeComponent::OnDestroy()
{
	
}
void VSNodeComponent::AttachParentSocket(const VSUsedName & AttackSocketName)
{
	VSSkeletonMeshComponent * pParent = DynamicCast<VSSkeletonMeshComponent>(m_pParent);
	if (pParent)
	{
		m_pAttachSocket = pParent->GetSocketNode(AttackSocketName);
		m_AttachSocketName = AttackSocketName;
	}
}
bool VSNodeComponent::PostLoad(VSStream* pStream)
{
	VSSkeletonMeshComponent * pParent = DynamicCast<VSSkeletonMeshComponent>(m_pParent);
	if (pParent)
	{
		m_pAttachSocket = pParent->GetSocketNode(m_AttachSocketName);
	}
	return true;
}
bool VSNodeComponent::PostClone(VSObject * pObjectSrc)
{
	VSSkeletonMeshComponent * pParent = DynamicCast<VSSkeletonMeshComponent>(m_pParent);
	if (pParent)
	{
		m_pAttachSocket = pParent->GetSocketNode(m_AttachSocketName);
	}
	return true;
}
void VSNodeComponent::UpdateLightState(double dAppTime)
{
	if (m_pAllLight.GetNum() > 0)
		m_pAllLight.Clear();
	for (unsigned int i = 0; i < m_pChild.GetNum(); i++)
	{
		if (m_pChild[i])
		{
			if (m_pChild[i]->m_pAllLight.GetNum() > 0)
				m_pAllLight.AddElement(m_pChild[i]->m_pAllLight, 0, m_pChild[i]->m_pAllLight.GetNum() - 1);
		}
	}
}
void VSNodeComponent::UpdateCameraState(double dAppTime)
{
	if (m_pAllCamera.GetNum() > 0)
		m_pAllCamera.Clear();
	for (unsigned int i = 0; i < m_pChild.GetNum(); i++)
	{
		if (m_pChild[i])
		{
			if (m_pChild[i]->m_pAllCamera.GetNum() > 0)
				m_pAllCamera.AddElement(m_pChild[i]->m_pAllCamera, 0, m_pChild[i]->m_pAllCamera.GetNum() - 1);
		}
	}
}
void VSNodeComponent::UpdateNodeAll(double dAppTime)
{

	if (dAppTime > 0.0f)
	{
		UpdateController(dAppTime);
	}


	UpdateTransform(dAppTime);


	for (unsigned int i = 0; i < m_pChild.GetNum(); i++)
	{
		if (m_pChild[i])
			m_pChild[i]->UpdateNodeAll(dAppTime);
	}
	UpdateLightState(dAppTime);
	UpdateCameraState(dAppTime);

	if (m_bIsChanged)
	{
		UpdateWorldBound(dAppTime);
	}
	m_bIsChanged = false;

}
void VSNodeComponent::UpdateTransform(double dAppTime)
{
	if (m_pAttachSocket)
	{

		if (!m_pAttachSocket->m_bIsStatic)
		{
			m_bIsStatic = 0;
		}
		if (m_pAttachSocket->m_bIsChanged)
		{
			m_bIsChanged = true;
		}

		if (m_bIsChanged)
		{

			unsigned int TransformFlag = ((unsigned int)m_bInheritScale) | ((unsigned int)m_bInheritRotate << 1) | ((unsigned int)m_bInheritTranslate << 2);
			m_World.Product(m_Local, m_pAttachSocket->m_World, TransformFlag);
		}
	}
	else
	{
		VSNode::UpdateTransform(dAppTime);
		
	}
}
void VSNodeComponent::GetStreamResource(VSArray<VSResourceProxyBase*>& pResourceProxy, StreamInformation_TYPE& StreamInformation)const
{
	for (unsigned int i = 0; i < m_pChild.GetNum(); i++)
	{
		VSStreamingManager::GetStreamInfo(m_pChild[i]->GetWorldAABB(), StreamInformation);
		m_pChild[i]->GetStreamResource(pResourceProxy, StreamInformation);

	}
}
