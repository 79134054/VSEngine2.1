#include "VSModelMeshNode.h"
#include "VSSwitchNode.h"
#include "VSGraphicInclude.h"
#include "VSModelSwitchNode.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSModelMeshNode,VSMeshNode)
BEGIN_ADD_PROPERTY(VSModelMeshNode,VSMeshNode)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSModelMeshNode)
IMPLEMENT_INITIAL_END
VSModelMeshNode::VSModelMeshNode()
{
	
}
VSModelMeshNode::~VSModelMeshNode()
{

}
void VSModelMeshNode::SetUseMaterialInstance(unsigned int SubMeshID,unsigned int UsedID)
{
	VSModelSwitchNode* Temp = DynamicCast<VSModelSwitchNode>(m_pChild[0]);
	if (!Temp)
	{
		VSGeometryNode* pGeometryNode = DynamicCast<VSGeometryNode>(m_pChild[0]);
		pGeometryNode->GetNormalGeometry(SubMeshID)->SetUseMaterialInstance(UsedID);
	}
	else
	{
		return	Temp->SetUseMaterialInstance(SubMeshID, UsedID);
	}
}
VSGeometryNode * VSModelMeshNode::GetGeometryNode(unsigned int uiLodLevel)
{
	if (m_pChild.GetNum() > 1)
	{
		return NULL;
	}
	else
	{
		VSModelSwitchNode* Temp = DynamicCast<VSModelSwitchNode>(m_pChild[0]);
		if (!Temp)
		{
			VSGeometryNode * pGeometryNode = DynamicCast<VSGeometryNode>(m_pChild[0]);
			return pGeometryNode;
		}
		else
		{
			return	Temp->GetGeometryNode(uiLodLevel);
		}
	}
}
VSSwitchNode * VSModelMeshNode::GetLodNode()const
{
	if (m_pChild.GetNum())
	{
		return DynamicCast<VSSwitchNode>(m_pChild[0]);
	}
	return NULL;
}
bool VSModelMeshNode::PostLoad(VSStream* pStream)
{
	VSMeshNode::PostLoad(pStream);
	return true;
}
bool VSModelMeshNode::PostClone(VSObject * pObjectSrc)
{
	VSMeshNode::PostClone(pObjectSrc);

	return true;
}
void VSModelMeshNode::UpdateController(double dAppTime)
{
	VSMeshNode::UpdateController(dAppTime);

}
void VSModelMeshNode::SetMorphData(const VSUsedName & ShowName,float fData)
{
	VSGeometryNode * pGeometryNode = GetGeometryNode(0);
	pGeometryNode->SetMorphData(ShowName, fData);
}
bool VSModelMeshNode::SetDrawInstance(bool bDrawInstance)
{
	VSGeometryNode * pGeometryNode = GetGeometryNode(0);
	if (pGeometryNode->GetMorphSet())
	{
		m_bDrawInstance = false;
		return false;
	}
	else
	{
		m_bDrawInstance = bDrawInstance;
		return true;
	}
	
}
void VSModelMeshNode::ClearLodMesh()
{
	VSModelSwitchNode* LodNode = DynamicCast<VSModelSwitchNode>(m_pChild[0]);
	if (!LodNode)
	{
		return;
	}
	m_pChild[0] = LodNode->GetChild(0);
}
bool VSModelMeshNode::AddLODGeometryNode(VSGeometryNode* pGeoNode)
{
	VSModelSwitchNode* LodNode = DynamicCast<VSModelSwitchNode>(GetChild(0));
	VSGeometryNode* FirstLOD = NULL;
	if (!LodNode)
	{
		FirstLOD = DynamicCast<VSGeometryNode>(GetChild(0));
		LodNode = VS_NEW VSModelSwitchNode();
		VSSpatialPtr GeoNode = GetChild(0);
		DeleteAllChild();
		LodNode->AddChild(GeoNode);
		AddChild(LodNode);
	}
	else
	{
		FirstLOD = DynamicCast<VSGeometryNode>(LodNode->GetChild(0));
	}
	if (FirstLOD->GetNormalGeometryNum() == pGeoNode->GetNormalGeometryNum())
	{
		LodNode->AddChild(pGeoNode);
		return true;
	}
	else
	{
		return false;
	}
}
void VSModelMeshNode::GetStreamResource(VSArray<VSResourceProxyBase*>& pResourceProxy, StreamInformation_TYPE& StreamInformation)const
{
	VSModelSwitchNode* Temp = DynamicCast<VSModelSwitchNode>(m_pChild[0]);
	if (!Temp)
	{
		VSGeometryNode* pGeometryNode = DynamicCast<VSGeometryNode>(m_pChild[0]);
		pGeometryNode->GetStreamResource(pResourceProxy, StreamInformation);
	}
	else
	{
		return	Temp->GetStreamResource(pResourceProxy, StreamInformation);
	}
}