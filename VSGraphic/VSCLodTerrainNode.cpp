#include "VSCLodTerrainNode.h"
#include "VSCLodTerrainGeometry.h"
#include "VSRoamTerrainGemotry.h"
#include "VSQuadTerrainGeometry.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSCLodTerrainNode,VSTerrainNode)
BEGIN_ADD_PROPERTY(VSCLodTerrainNode,VSTerrainNode)
REGISTER_PROPERTY(m_uiTerrainNodeType,TerrainNodeType,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_fCLODScale, CLODScale, VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSCLodTerrainNode)
IMPLEMENT_INITIAL_END
VSCLodTerrainNode::VSCLodTerrainNode()
{
	
	m_fCLODScale = 50000000.0f;

	m_uiTerrainNodeType = TNT_ROAM;
}
void VSCLodTerrainNode::SetTerrainNodeType(unsigned int uiTerrainNodeType)
{
	VSMAC_ASSERT(uiTerrainNodeType < TNT_MAX);
	m_uiTerrainNodeType = uiTerrainNodeType;

}
VSCLodTerrainNode::~VSCLodTerrainNode()
{

}
unsigned int VSCLodTerrainNode::AddChild(VSSpatial * pChild)
{
	VSMAC_ASSERT(DynamicCast<VSCLodTerrainGeometry>(pChild));

	return VSTerrainNode::AddChild(pChild);
}

bool VSCLodTerrainNode::CreateChild()
{
	m_pChild.Clear();
	unsigned int uiChildNumX = GetTileNumX();
	unsigned int uiChildNumZ = GetTileNumZ();
	for (unsigned int i = 0 ; i < uiChildNumX ; i++)
	{
		for ( unsigned int j = 0 ; j < uiChildNumZ ; j++)
		{
			VSCLodTerrainGeometry * pTerrainGeo = NULL;
			if (m_uiTerrainNodeType == TNT_ROAM)
			{
				pTerrainGeo = VS_NEW VSRoamTerrainGeometry();
			}
			else if (m_uiTerrainNodeType == TNT_QUAD)
			{
				pTerrainGeo = VS_NEW VSQuadTerrainGeometry();
			}

			if (!pTerrainGeo)
			{
				return 0;
			}
			AddChild(pTerrainGeo);
			if (!pTerrainGeo->CreateMeshData(i,j,m_uiTessellationLevel))
			{
				return 0;
			}
			pTerrainGeo->AddMaterialInstance((VSMaterialR *)VSMaterial::GetDefaultOnlyColorResource());
			VSREAL green[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
			pTerrainGeo->GetMaterialInstance(0)->SetShaderValue(_T("EmissiveColor"), green, 4);
			

		}
	}

	LinkNeighbor();
	ComputeVariance();
	return 1;
}


void VSCLodTerrainNode::ComputeVariance()
{
	VSCLodTerrainGeometry * pTerrainGeo = NULL;
	unsigned int uiCurIndex;
	unsigned int uiChildNumX = GetTileNumX();
	unsigned int uiChildNumZ = GetTileNumZ();
	if (m_uiTerrainNodeType == TNT_ROAM)
	{
		for (unsigned int i = 0 ; i < uiChildNumX ; i++)
		{
			for ( unsigned int j = 0 ; j < uiChildNumZ ; j++)
			{

				uiCurIndex = i * uiChildNumZ + j;
				pTerrainGeo = (VSCLodTerrainGeometry *)(GetChild(uiCurIndex));		
				pTerrainGeo->ComputeVariance();
			}
		}
	}
	else if(m_uiTerrainNodeType == TNT_QUAD)
	{
		for (int uiLevel = m_uiTessellationLevel - 1 ; uiLevel >= 0 ; uiLevel--)
		{
			for (unsigned int i = 0 ; i < uiChildNumX ; i++)
			{
				for ( unsigned int j = 0 ; j < uiChildNumZ ; j++)
				{

					uiCurIndex = i * uiChildNumZ + j;
					pTerrainGeo = (VSCLodTerrainGeometry *)(GetChild(uiCurIndex));		
					pTerrainGeo->ComputeVariance(uiLevel);
				}
			}
		}
		
		
	}
	
}
void VSCLodTerrainNode::LinkNeighbor()
{
	unsigned int uiLeft;
	unsigned int uiRight;
	unsigned int uiTop;
	unsigned int uiBottom;
	unsigned int uiChildNumX = GetTileNumX();
	unsigned int uiChildNumZ = GetTileNumZ();
	unsigned int uiCurIndex;
	VSCLodTerrainGeometry * pTerrainGeo = NULL;
	for (unsigned int i = 0 ; i < uiChildNumX ; i++)
	{
		for ( unsigned int j = 0 ; j < uiChildNumZ ; j++)
		{
			
			uiCurIndex = i * uiChildNumZ + j;
			if (i == 0)
			{
				uiLeft = VSMAX_UINT32;

			}
			else
			{
				uiLeft = (i - 1) * uiChildNumZ + j;

			}

			if (i == uiChildNumX - 1)
			{
				uiRight = VSMAX_UINT32;

			}
			else
			{
				uiRight = (i + 1) * uiChildNumZ + j;

			}

			if (j == 0)
			{
				uiBottom = VSMAX_UINT32;

			}
			else
			{
				uiBottom = i * uiChildNumZ + j - 1;

			}

			if (j == uiChildNumZ - 1)
			{
				uiTop = VSMAX_UINT32;
			}
			else
			{
				uiTop = i * uiChildNumZ + j + 1;
			}

			 
			pTerrainGeo = (VSCLodTerrainGeometry *)(GetChild(uiCurIndex));
			if (uiTop != VSMAX_UINT32)
			{
				pTerrainGeo->AddNeighbor((VSCLodTerrainGeometry *)GetChild(uiTop),VSCLodTerrainGeometry::NT_TOP);
			}
			
			if (uiBottom != VSMAX_UINT32)
			{
				pTerrainGeo->AddNeighbor((VSCLodTerrainGeometry *)GetChild(uiBottom),VSCLodTerrainGeometry::NT_BOTTOM);
			}

			if (uiLeft != VSMAX_UINT32)
			{
				pTerrainGeo->AddNeighbor((VSCLodTerrainGeometry *)GetChild(uiLeft),VSCLodTerrainGeometry::NT_LEFT);
			}

			if (uiRight != VSMAX_UINT32)
			{
				pTerrainGeo->AddNeighbor((VSCLodTerrainGeometry *)GetChild(uiRight),VSCLodTerrainGeometry::NT_RIGHT);
			}
			pTerrainGeo->LinkNeighbor();
		}
	}

}
void VSCLodTerrainNode::SetCLODScale(VSREAL fCLODScale)
{
	if (fCLODScale <= 0.0f)
	{
		return ;
	}
	m_fCLODScale = fCLODScale;
}


void VSCLodTerrainNode::UpdateNodeAll(double dAppTime)
{
	VSTerrainNode::UpdateNodeAll(dAppTime);
	if (dAppTime > 0.0f)
	{
		for (unsigned int i = 0 ; i < m_pChild.GetNum() ; i++)
		{
			VSSpatial * pChild = m_pChild[i];
			((VSCLodTerrainGeometry *)pChild)->ClearInfo();
		}
	}
}

