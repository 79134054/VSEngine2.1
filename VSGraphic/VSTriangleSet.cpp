#include "VSTriangleSet.h"
#include "VSNode.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSTriangleSet,VSMeshData)
BEGIN_ADD_PROPERTY(VSTriangleSet,VSMeshData)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSTriangleSet)
IMPLEMENT_INITIAL_END
VSTriangleSet::VSTriangleSet()
{
	m_uiMeshDataType = MDT_TRIANGLE;
}
VSTriangleSet::~VSTriangleSet()
{

}
bool VSTriangleSet::CreateFanIndex()
{
	if(m_pVertexBuffer == NULL)
		return 0;
	unsigned int Num = m_pVertexBuffer->GetPositionData(0)->GetNum();
	if(Num < 3)
		return 0;
	Num = (Num - 1) * 3;
	VSDataBuffer * pIndex = NULL;
	pIndex = new VSDataBuffer();
	if(!pIndex)
		return 0;
	if(!pIndex->CreateEmptyBuffer(Num,VSDataBuffer::DT_USHORT))
		return 0;

	VSUSHORT_INDEX * pIndexBuffer = (VSUSHORT_INDEX *)pIndex->GetData();
	if(!pIndexBuffer)
		return 0;
	for(unsigned int i = 0 ; i < Num / 3; i++)
	{
		pIndexBuffer[i * 3] = 0;
		pIndexBuffer[i * 3 + 1] = i + 1;
		pIndexBuffer[i * 3 + 2] = i + 2;
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
bool VSTriangleSet::CreateNormal(unsigned int uiVertexLevel)
{
	if(m_pVertexBuffer == NULL)
		return 0;
	if(m_pVertexBuffer->GetNormalData(uiVertexLevel))
		return 1;
	unsigned int Num = m_pVertexBuffer->GetVertexNum();
	if(Num < 3)
		return 0;

	if(m_pIndexBuffer == NULL)
		return 0;

	//Noraml
	VSDataBuffer * pNormal= NULL;
	pNormal = new VSDataBuffer();
	if(!pNormal)
		return 0;
	if(!pNormal->CreateEmptyBuffer(Num,VSDataBuffer::DT_FLOAT32_3))
		return 0;

	VSVector3 * pNormalBuffer = (VSVector3 *)pNormal->GetData();

	//计数
	unsigned int * pVertexTemp = VS_NEW unsigned int[Num];
	VSMemset((void *)pVertexTemp, 0, sizeof(unsigned int) * Num);


	unsigned int TriangleNum = GetTotalNum();

	VSDataBuffer * pIndexData = m_pIndexBuffer->GetIndexData();
	if (!pIndexData)
	{
		return 0;
	}
	// 获取索引指针
	VSUSHORT_INDEX * pIndexBuffer = (VSUSHORT_INDEX *)pIndexData->GetData();
	if(!pIndexBuffer)
		return 0;
	
	VSDataBuffer * pData = m_pVertexBuffer->GetPositionData(uiVertexLevel);
	if(!pData)
		return 0;
	if(pData->GetChannel() != 3)
		return 0;
	//获取顶点指针
	VSVector3 * pVertexBuffer = (VSVector3 *)pData->GetData();
	if(!pVertexBuffer)
		return 0;
	
	//遍历所有三角形
	for (unsigned int poly = 0 ; poly < TriangleNum; poly++)
	{

		VSUSHORT_INDEX i0 = pIndexBuffer[poly*3+0];
		VSUSHORT_INDEX i1 = pIndexBuffer[poly*3+1];
		VSUSHORT_INDEX i2 = pIndexBuffer[poly*3+2];

		VSVector3 u = pVertexBuffer[i1] - pVertexBuffer[i0];
		VSVector3 v = pVertexBuffer[i2] - pVertexBuffer[i0];

		VSVector3 n;
		n.Cross(u,v);

		pVertexTemp[i0]++;
		pVertexTemp[i1]++;
		pVertexTemp[i2]++;

		pNormalBuffer[i0] += n;
		pNormalBuffer[i1] += n;
		pNormalBuffer[i2] += n;



	} // end for poly


	for (unsigned int vertex = 0; vertex < Num; vertex++)
	{


		if (pVertexTemp[vertex] > 1)
		{
			pNormalBuffer[vertex] /= (VSREAL)1.0 * pVertexTemp[vertex];
		
			
		} // end if
		pNormalBuffer[vertex].Normalize();
	} // end for
	VSMAC_DELETEA(pVertexTemp);

	m_pVertexBuffer->SetData(pNormal,VSVertexFormat::VF_NORMAL);
	return 1;
}
/*
	(P0 - P1) = (u0 - u1) U + (v0 - v1) V
	(P0 - P2) = (u0 - u2) U + (v0 - v2) V

	(v0 - v2)(P0 - P1) = (v0 - v2)(u0 - u1) U + (v0 - v2)(v0 - v1) V
	(v0 - v1)(P0 - P2) = (v0 - v1)(u0 - u2) U + (v0 - v2)(v0 - v1) V

	U = ((v0 - v2)(P0 - P1) - (v0 - v1)(P0 - P2)) / ((v0 - v2)(u0 - u1) - (v0 - v1)(u0 - u2))

	(u0 - u2)(P0 - P1) = (u0 - u2)(u0 - u1) U + (u0 - u2)(v0 - v1) V
	(u0 - u1)(P0 - P2) = (u0 - u2)(u0 - u1) U + (v0 - v2)(u0 - u1) V

	V = ((u0 - u1)(P0 - P2) - (u0 - u2)(P0 - P1))/((v0 - v2)(u0 - u1) - (v0 - v1)(u0 - u2))
*/
bool VSTriangleSet::CreateTangent(unsigned int uiTexCoordLevel)
{
	if(!m_pVertexBuffer || !m_pVertexBuffer->GetTexCoordData(uiTexCoordLevel) || !m_pIndexBuffer)
		return 0;
	if(!m_pVertexBuffer->GetPositionData(0))
		return 0;
	if(m_pVertexBuffer->GetTangentData() && m_pVertexBuffer->GetBinormalData())
		return 1;
	else if(m_pVertexBuffer->GetTangentData() || m_pVertexBuffer->GetBinormalData())
		return 0;
	unsigned int Num = m_pVertexBuffer->GetVertexNum();
	if(Num < 3)
		return 0;

	if(!m_pVertexBuffer->GetNormalData(0))
	{
		if(!CreateNormal(0))
			return 0;
	}
	
	//TangentNoraml
	VSDataBuffer * pTangent = NULL;
	pTangent = new VSDataBuffer();
	if(!pTangent)
		return 0;
	if(!pTangent->CreateEmptyBuffer(Num,VSDataBuffer::DT_FLOAT32_3))
		return 0;

	VSVector3 * pTangentBuffer = (VSVector3 *)pTangent->GetData();

	//BiNoraml
	VSDataBuffer * pBiNormal = NULL;
	pBiNormal = new VSDataBuffer();
	if(!pBiNormal)
		return 0;
	if(!pBiNormal->CreateEmptyBuffer(Num,VSDataBuffer::DT_FLOAT32_3))
		return 0;

	VSVector3 * pBiNormalBuffer = (VSVector3 *)pBiNormal->GetData();


	unsigned int TriangleNum = GetTotalNum();


	VSDataBuffer * pIndexData = m_pIndexBuffer->GetIndexData();
	if (!pIndexData)
	{
		return 0;
	}

	// 获取索引指针
	VSUSHORT_INDEX * pIndexBuffer = (VSUSHORT_INDEX *)pIndexData->GetData();
	if(!pIndexBuffer)
		return 0;

	//获取顶点
	VSDataBuffer * pData = m_pVertexBuffer->GetPositionData(0);
	if(!pData)
		return 0;
	if(pData->GetChannel() != 3)
		return 0;
	//获取顶点指针
	VSVector3 * pVertexBuffer = (VSVector3 *)pData->GetData();
	if(!pVertexBuffer)
		return 0;
	
	// get normal 
	pData = m_pVertexBuffer->GetNormalData(0);
	if(!pData)
		return 0;
	VSVector3 * pNormalBuffer = (VSVector3 *)pData->GetData();
	if(!pNormalBuffer)
		return 0;

	// get texcoord
	pData = m_pVertexBuffer->GetTexCoordData(uiTexCoordLevel);
	if(!pData)
		return 0;
	VSVector2 * pTexCoordBuffer = (VSVector2 *)pData->GetData();
	if(!pTexCoordBuffer)
		return 0;

	for (unsigned int poly = 0 ; poly < TriangleNum; poly++)
	{
		VSUSHORT_INDEX i0 = pIndexBuffer[poly*3+0];
		VSUSHORT_INDEX i1 = pIndexBuffer[poly*3+1];
		VSUSHORT_INDEX i2 = pIndexBuffer[poly*3+2];
		
		//U = ((v0 - v2)(P0 - P1) - (v0 - v1)(P0 - P2)) / ((v0 - v2)(u0 - u1) - (v0 - v1)(u0 - u2))
		VSVector3 TangentNoraml;
		TangentNoraml = 
			(	(pVertexBuffer[i0] - pVertexBuffer[i1]) * (pTexCoordBuffer[i0].y - pTexCoordBuffer[i2].y) - 
				(pVertexBuffer[i0] - pVertexBuffer[i2]) * (pTexCoordBuffer[i0].y - pTexCoordBuffer[i1].y))/
			(	(pTexCoordBuffer[i0].x - pTexCoordBuffer[i1].x) * (pTexCoordBuffer[i0].y - pTexCoordBuffer[i2].y) - 
				(pTexCoordBuffer[i0].x - pTexCoordBuffer[i2].x) * (pTexCoordBuffer[i0].y - pTexCoordBuffer[i1].y));
		TangentNoraml.Normalize();
		pTangentBuffer[i0] += TangentNoraml;
		pTangentBuffer[i1] += TangentNoraml;
		pTangentBuffer[i2] += TangentNoraml;

		//V = ((u0 - u1)(P0 - P2) - (u0 - u2)(P0 - P1))/((v0 - v2)(u0 - u1) - (v0 - v1)(u0 - u2))
		VSVector3 BiNoraml;
		BiNoraml =
			(	(pVertexBuffer[i0] - pVertexBuffer[i1]) * (pTexCoordBuffer[i0].x - pTexCoordBuffer[i2].x) - 
				(pVertexBuffer[i0] - pVertexBuffer[i2]) * (pTexCoordBuffer[i0].x - pTexCoordBuffer[i1].x))/
			(	(pTexCoordBuffer[i0].x - pTexCoordBuffer[i2].x) * (pTexCoordBuffer[i0].y - pTexCoordBuffer[i1].y) -
				(pTexCoordBuffer[i0].x - pTexCoordBuffer[i1].x) * (pTexCoordBuffer[i0].y - pTexCoordBuffer[i2].y));
		BiNoraml.Normalize();
		pBiNormalBuffer[i0] += BiNoraml;
		pBiNormalBuffer[i1] += BiNoraml;
		pBiNormalBuffer[i2] += BiNoraml;

	}
	for (unsigned int i = 0 ;i < m_pVertexBuffer->GetVertexNum() ;i++)
	{
		VSVector3 N1 = pTangentBuffer[i];
		VSVector3 N2 = pBiNormalBuffer[i];
		VSVector3 Normal = pNormalBuffer[i];

		VSVector3 Tangent = N1 - Normal * (( N1 * Normal) / (Normal * Normal));

		VSVector3 Binormal = N2 - Normal * (( N2 * Normal) / (Normal * Normal)) - Tangent * (( N2 * Tangent) / (Tangent * Tangent));
		
		Tangent.Normalize();
		Binormal.Normalize();
		pTangentBuffer[i] = Tangent;
		pBiNormalBuffer[i] = Binormal;	
	}
	m_pVertexBuffer->SetData(pBiNormal,VSVertexFormat::VF_BINORMAL);
	m_pVertexBuffer->SetData(pTangent,VSVertexFormat::VF_TANGENT);
	return 1;
}
unsigned int VSTriangleSet::GetTotalNum()const
{
	if (!m_pIndexBuffer && !m_pVertexBuffer)
	{
		return 0;
	}
	else if(!m_pIndexBuffer)
	{
		return m_pVertexBuffer->GetVertexNum() / 3;
	}
	else
	{
		return m_pIndexBuffer->GetNum() / 3;
	}
}
unsigned int VSTriangleSet::GetGirdNum(unsigned int uiInputNum)const
{

	return uiInputNum / 3;
}
unsigned int VSTriangleSet::GetActiveNum()const
{
	return GetTotalNum();
}