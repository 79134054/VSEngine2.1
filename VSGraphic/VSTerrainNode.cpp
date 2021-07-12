#include "VSTerrainNode.h"
#include "VSGraphicInclude.h"
#include "VSConfig.h"
using namespace VSEngine2;
#include "VSStream.h"
IMPLEMENT_RTTI_NoCreateFun(VSTerrainNode,VSMeshNode)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSTerrainNode)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(VSTerrainNode,VSMeshNode)
REGISTER_PROPERTY(m_uiTessellationLevel,TessellationLevel,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiNumX,NumX,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiNumZ,NumZ,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_fHeightScale,HeightScale,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY_DATA(m_pHeight,m_uiTotalNum,HeightData);
END_ADD_PROPERTY

VSTerrainNode::VSTerrainNode()
{
	m_uiTessellationLevel = MIN_TESSELLATION_LEVEL;
	m_uiNumX = MIN_NUM;
	m_uiNumZ = MIN_NUM;

	m_pHeight = NULL;
	m_fHeightScale = 1.0f;
	m_uiTotalNum = 0;
}

VSTerrainNode::~VSTerrainNode()
{
	VSMAC_DELETEA(m_pHeight);
}

void VSTerrainNode::SetTessellationLevel(unsigned int uiTessellationLevel, unsigned int uiMaxTessellationLevel,
	unsigned int uiMinTessellationLevel)
{
	m_uiTessellationLevel = uiTessellationLevel;
	if(m_uiTessellationLevel > m_uiNumX)
	{
		m_uiTessellationLevel = m_uiNumX;
	}

	if(m_uiTessellationLevel > m_uiNumZ)
	{
		m_uiTessellationLevel = m_uiNumZ;
	}

	if (m_uiTessellationLevel > uiMaxTessellationLevel)
	{
		m_uiTessellationLevel = uiMaxTessellationLevel;
	}
	if (m_uiTessellationLevel < uiMinTessellationLevel)
	{
		m_uiTessellationLevel = uiMinTessellationLevel;
	}
}
void VSTerrainNode::SetNum(unsigned int uiNumX,unsigned int uiNumZ)
{
	m_uiNumX = uiNumX;
	m_uiNumZ = uiNumZ;
	if (m_uiNumX > MAX_NUM)
	{
		m_uiNumX = MAX_NUM;
	}
	if(m_uiNumX < MIN_NUM)
	{
		m_uiNumX = MIN_NUM;
	}
	
	if (m_uiNumZ > MAX_NUM)
	{
		m_uiNumZ = MAX_NUM;
	}
	if(m_uiNumZ < MIN_NUM)
	{
		m_uiNumZ = MIN_NUM;
	}
	if (m_uiTessellationLevel > m_uiNumX || m_uiTessellationLevel > m_uiNumZ)
	{
		SetTessellationLevel(m_uiTessellationLevel);
	}
	m_uiTotalNum = ((1 << m_uiNumX) + 1) * ((1 << m_uiNumZ) + 1);
}

bool VSTerrainNode::CreateTerrain(unsigned int uiNumX,unsigned int uiNumZ,unsigned int uiTessellationLevel)
{
	
	SetNum(uiNumX,uiNumZ);
	SetTessellationLevel(uiTessellationLevel);
	VSMAC_DELETEA(m_pHeight);
	m_pHeight = VS_NEW unsigned char[m_uiTotalNum];
	if (!m_pHeight)
	{
		return 0;
	}
	VSMemset(m_pHeight,0,m_uiTotalNum * sizeof(unsigned char));
	if (!CreateChild())
	{
		return 0;
	}
	m_bIsChanged = true;
	CreateLocalAABB();
	UpdateAll(0.0f);
	return 1;
}
bool VSTerrainNode::CreateTerrainFromHeightMap(const TCHAR *pFileName,unsigned int uiTessellationLevel,VSREAL fHeightScale)
{
	VSMAC_ASSERT(pFileName);

	
	VSFile *pFile = NULL;
	pFile = VS_NEW VSFile();
	VSMAC_ASSERT(pFile);

	unsigned int uiHeightSize = 0;
	VSMAC_DELETEA(m_pHeight);

	VSString TerrainPath = VSConfig::ms_TerrainPath + pFileName;
	if(!pFile->Open(TerrainPath.GetBuffer(),VSFile::OM_RB))
	{
		VSMAC_DELETE(pFile);
		return 0;
	}
	
	pFile->Read(&uiHeightSize, 1, sizeof(unsigned int));
	m_pHeight= VS_NEW unsigned char [uiHeightSize * uiHeightSize];

	VSMAC_ASSERT(m_pHeight);


	pFile->Read(m_pHeight, 1, uiHeightSize * uiHeightSize);

	VSMAC_DELETE(pFile);
	uiHeightSize = uiHeightSize - 1;
	unsigned int uiNum = FastLog2(uiHeightSize);
	
	SetNum(uiNum,uiNum);
	SetTessellationLevel(uiTessellationLevel);
	m_fHeightScale = fHeightScale;
	if (!CreateChild())
	{
		return 0;
	}
	m_bIsChanged = true;
	CreateLocalAABB();
	UpdateAll(0.0f);
	return 1;
}

unsigned char VSTerrainNode::GetRawHeight(unsigned int uiX,unsigned int uiZ)
{
	unsigned int uiLen = ((1 << m_uiNumX) + 1);
	if (m_pHeight && uiX < uiLen && uiZ < uiLen)
	{
		return m_pHeight[uiX * uiLen + uiZ];
	}
	return 0;
}
VSREAL VSTerrainNode::GetHeight(unsigned int uiX,unsigned int uiZ)
{
	unsigned int uiLen = ((1 << m_uiNumX) + 1);
	if (m_pHeight && uiX < uiLen && uiZ < uiLen)
	{
		return m_pHeight[uiX * uiLen + uiZ] * m_fHeightScale;
	}
	return 0.0f;

}
