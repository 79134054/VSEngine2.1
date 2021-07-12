#include "VSGPULodTerrainNode.h"
#include "VSGraphicInclude.h"
#include "VSGPULodTerrainGeometry.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSGPULodTerrainNode, VSTerrainNode)
BEGIN_ADD_PROPERTY(VSGPULodTerrainNode, VSTerrainNode)
// REGISTER_PROPERTY(m_uiDLodExtend, DLodExtend, VSProperty::F_SAVE_LOAD_CLONE);
// REGISTER_PROPERTY(m_fDLodScale, DLodScale, VSProperty::F_SAVE_LOAD_CLONE);
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSGPULodTerrainNode)
IMPLEMENT_INITIAL_END
VSGPULodTerrainNode::VSGPULodTerrainNode()
{
// 	m_uiDLodExtend = 500;
// 	m_fDLodScale = 50000000.0f;
}

VSGPULodTerrainNode::~VSGPULodTerrainNode()
{
}
bool VSGPULodTerrainNode::CreateChild()
{
	m_pChild.Clear();

	VSGPULodTerrainGeometry * pGLG = NULL;
	pGLG = VS_NEW VSGPULodTerrainGeometry();
	if (!pGLG->CreateMesh(GetTileNumX(), GetTileNumZ()))
	{
		return false;
	}
	AddChild(pGLG);
	pGLG->AddMaterialInstance((VSMaterialR *)VSMaterial::GetDefaultOnlyColorResource());
	VSREAL green[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
	pGLG->GetMaterialInstance(0)->SetShaderValue(_T("EmissiveColor"), green, 4);
	
	return 1;
}

void VSGPULodTerrainNode::UpdateNodeAll(double dAppTime)
{

	VSTerrainNode::UpdateNodeAll(dAppTime);

}
bool VSGPULodTerrainNode::CreateTerrain(unsigned int uiNumX, unsigned int uiNumZ, unsigned int uiTessellationLevel)
{
	SetNum(uiNumX, uiNumZ);
	SetTessellationLevel(uiTessellationLevel,MAX_GPU_TESSELLATION_LEVEL);
	VSMAC_DELETEA(m_pHeight);
	m_pHeight = VS_NEW unsigned char[m_uiTotalNum];
	VSMAC_ASSERT(m_pHeight);

	VSMemset(m_pHeight, 0, m_uiTotalNum * sizeof(unsigned char));
	m_LodTexture = VSResourceManager::Create2DTexture(GetSizeX(), GetSizeZ(), VSRenderer::SFT_R16F, 1, m_pHeight);
	if (!CreateChild())
	{
		return 0;
	}
	m_bIsChanged = true;
	CreateLocalAABB();
	UpdateAll(0.0f);
	return true;
}