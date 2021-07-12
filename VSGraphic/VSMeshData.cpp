#include "VSMeshData.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI_NoCreateFun(VSMeshData,VSObject)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSMeshData)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(VSMeshData,VSObject)
REGISTER_PROPERTY(m_pVertexBuffer,VertexBuffer,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_pIndexBuffer,IndexBuffer,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiMeshDataType, MeshDataType, VSProperty::F_SAVE_LOAD_CLONE);
END_ADD_PROPERTY
VSMeshData::VSMeshData()
{

	m_pVertexBuffer = NULL;
	m_pIndexBuffer = NULL;
	m_uiMeshDataType = MDT_TRIANGLE;
}
VSMeshData::~VSMeshData()
{
	m_pVertexBuffer = NULL;
	m_pIndexBuffer = NULL;
}
bool VSMeshData::SetIndexBuffer(VSIndexBuffer * pIndexBuffer)
{
	VSMAC_ASSERT(pIndexBuffer);

	VSMAC_ASSERT(pIndexBuffer->GetNum());

	m_pIndexBuffer = pIndexBuffer;
	return 1;

}
bool VSMeshData::SetVertexBuffer(VSVertexBuffer *pVertexBuffer)
{
	VSMAC_ASSERT(pVertexBuffer);
	VSMAC_ASSERT(pVertexBuffer->GetVertexNum());

	m_pVertexBuffer = pVertexBuffer;
	return 1;
}
