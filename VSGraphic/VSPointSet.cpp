#include "VSPointSet.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSPointSet,VSMeshData)
BEGIN_ADD_PROPERTY(VSPointSet,VSMeshData)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSPointSet)
IMPLEMENT_INITIAL_END
VSPointSet::VSPointSet()
{
	m_uiMeshDataType = MDT_POINT;
}
VSPointSet::~VSPointSet()
{

}
bool VSPointSet::CreateIndex()
{
	if(m_pVertexBuffer == NULL)
		return 0;
	unsigned int Num = m_pVertexBuffer->GetVertexNum();
	if(Num < 1)
		return 0;
	VSDataBuffer * pIndex = NULL;
	pIndex = new VSDataBuffer();
	if(!pIndex)
		return 0;
	if(!pIndex->CreateEmptyBuffer(Num,VSDataBuffer::DT_USHORT))
		return 0;

	VSUSHORT_INDEX * pIndexBuffer = (VSUSHORT_INDEX *)pIndex->GetData();
	if(!pIndexBuffer)
		return 0;
	for(unsigned int i = 0 ; i < Num ; i++)
	{
		pIndexBuffer[i] = i;
	}

	if (!m_pIndexBuffer)
	{
		m_pIndexBuffer = VS_NEW VSIndexBuffer();
		if (!m_pIndexBuffer)
		{
			return 0;
		}
	}

	m_pIndexBuffer->SetData(pIndex);
	return 1;
}
unsigned int VSPointSet::GetTotalNum()const
{
	if (!m_pIndexBuffer && !m_pVertexBuffer)
	{
		return 0;
	}
	else if(!m_pIndexBuffer)
	{
		return m_pVertexBuffer->GetVertexNum();
	}
	else
	{
		return m_pIndexBuffer->GetNum();
	}
}
unsigned int VSPointSet::GetGirdNum(unsigned int uiInputNum)const
{
	return uiInputNum;
}
VSPointSet::VSPointSet(const VSVector3 & Point,VSREAL fSize)
{
	VSDataBuffer * pVertex = VS_NEW VSDataBuffer;

	pVertex->SetData(&Point, 1,VSDataBuffer::DT_FLOAT32_3);
	
	VSDataBuffer *PSize = VS_NEW VSDataBuffer;
	PSize->SetData(&fSize,1,VSDataBuffer::DT_FLOAT32_1);

	m_pVertexBuffer = VS_NEW VSVertexBuffer(true);
	m_pVertexBuffer->SetData(pVertex,VSVertexFormat::VF_POSITION);
	m_pVertexBuffer->SetData(PSize,VSVertexFormat::VF_PSIZE);

	bool bResult = CreateIndex();
	VSMAC_ASSERT(bResult);
}