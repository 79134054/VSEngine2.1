#include "VSDLodTerrainNode.h"
#include "VSDLodTerrainGeometry.h"
#include "VSDLodTerrainSwitchNode.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSDLodTerrainNode,VSTerrainNode)
BEGIN_ADD_PROPERTY(VSDLodTerrainNode,VSTerrainNode)
REGISTER_PROPERTY(m_uiDLodExtend, DLodExtend, VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_fDLodScale, DLodScale, VSProperty::F_SAVE_LOAD_CLONE);
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSDLodTerrainNode)
IMPLEMENT_INITIAL_END
VSDLodTerrainNode::VSDLodTerrainNode()
{
	m_uiDLodExtend = 500;
	m_fDLodScale = 50000000.0f;
}

VSDLodTerrainNode::~VSDLodTerrainNode()
{
}
bool VSDLodTerrainNode::CreateChild()
{
	m_pChild.Clear();
	unsigned int uiChildNumX = GetTileNumX();
	unsigned int uiChildNumZ = GetTileNumZ();
	for (unsigned int i = 0 ; i < uiChildNumX ; i++)
	{
		for ( unsigned int j = 0 ; j < uiChildNumZ ; j++)
		{

			VSDLodTerrainSwitchNode * pDTS = NULL;
			pDTS = VS_NEW VSDLodTerrainSwitchNode(i,j);
			VSMAC_ASSERT(pDTS);

			AddChild(pDTS);
			for (unsigned int k = 0 ; k < m_uiTessellationLevel ; k++)
			{
				VSDLodTerrainGeometry * pChild = NULL;
				pChild = VS_NEW VSDLodTerrainGeometry();
				VSMAC_ASSERT(pChild);

				pDTS->AddChild(pChild);
				pChild->CreateMesh(i,j,k,m_uiTessellationLevel);

				pChild->AddMaterialInstance((VSMaterialR *)VSMaterial::GetDefaultOnlyColorResource());
				VSREAL green[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
				pChild->GetMaterialInstance(0)->SetShaderValue(_T("EmissiveColor"), green, 4);
			}

		}
	}

	return 1;
}

void VSDLodTerrainNode::UpdateNodeAll(double dAppTime)
{
	
	VSTerrainNode::UpdateNodeAll(dAppTime);

}