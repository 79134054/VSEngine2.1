#include "VSControlPointPatch.h"
#include "VSNode.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSControlPointPatch, VSMeshData)
BEGIN_ADD_PROPERTY(VSControlPointPatch, VSMeshData)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSControlPointPatch)
IMPLEMENT_INITIAL_END
VSControlPointPatch::VSControlPointPatch()
{
	m_uiMeshDataType = MDT_CONTROL_POINT_4;
}
VSControlPointPatch::~VSControlPointPatch()
{

}
unsigned int VSControlPointPatch::GetTotalNum()const
{
	if (!m_pIndexBuffer && !m_pVertexBuffer)
	{
		return 0;
	}
	else if (!m_pIndexBuffer)
	{
		return m_pVertexBuffer->GetVertexNum() / 3;
	}
	else
	{
		return m_pIndexBuffer->GetNum() / 3;
	}
}
unsigned int VSControlPointPatch::GetGirdNum(unsigned int uiInputNum)const
{

	return uiInputNum / 3;
}
unsigned int VSControlPointPatch::GetActiveNum()const
{
	return GetTotalNum();
}