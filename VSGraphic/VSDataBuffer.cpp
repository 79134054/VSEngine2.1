#include "VSDataBuffer.h"
#include "VSMath.h"
#include "VSVector3.h"
#include "VSVector2.h"
#include "VSVector3W.h"
#include "VSShaderStringFactory.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSDataBuffer,VSObject)
BEGIN_ADD_PROPERTY(VSDataBuffer,VSObject)
REGISTER_PROPERTY(m_uiNum,Num,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiDT,DataType,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY_DATA(m_pData,m_uiSize,Data)
REGISTER_PROPERTY(m_uiStructStride, StructStride, VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSDataBuffer)
IMPLEMENT_INITIAL_END
unsigned int VSDataBuffer::ms_uiDataTypeByte[DT_MAXNUM] =
{
	4,		//DT_FLOAT32_1
	8,		//DT_FLOAT32_2
	12,		//DT_FLOAT32_3
	16,		//DT_FLOAT32_4

	2,		//DT_USHORT
	4,		//DT_UINT

	4,		//DT_UBYTE4
	4,		//DT_UBYTE4N

	4,		//DT_SHORT2
	4,		//DT_SHORT2N
	4,		//DT_USHORT2N

	8,		//DT_SHORT4
	8,		//DT_SHORT4N
	8,		//DT_USHORT4N

	4,		//DT_FLOAT16_2
	8,		//DT_FLOAT16_4

	4,		//DT_COLOR
	
	4,		//DT_SINT
	8,		//DT_SINT2
	12,		//DT_SINT3
	16,		//DT_SINT4

	8,		//DT_UINT2
	12,		//DT_UINT3
	16,		//DT_UINT4

	2,		//DT_FLOAT16_1

	1,		//DT_UBYTE
};
unsigned int VSDataBuffer::ms_uiDataTypeChannel[DT_MAXNUM] =
{
	1,		//DT_FLOAT32_1
	2,		//DT_FLOAT32_2
	3,		//DT_FLOAT32_3
	4,		//DT_FLOAT32_4

	1,		//DT_USHORT
	1,		//DT_UINT

	4,		//DT_UBYTE4
	4,		//DT_UBYTE4N
	
	2,		//DT_SHORT2
	2,		//DT_SHORT2N
	2,		//DT_USHORT2N

	4,		//DT_SHORT4
	4,		//DT_SHORT2N
	4,		//DT_USHORT4N

	2,		//DT_FLOAT16_2
	4,		//DT_FLOAT16_4

	4,		//DT_COLOR

	1,		//DT_SINT
	2,		//DT_SINT2
	3,		//DT_SINT3
	4,		//DT_SINT4

	2,		//DT_UINT2
	3,		//DT_UINT3
	4,		//DT_UINT4

	1,		//DT_FLOAT16_1

	1,		//DT_UBYTE
};
VSDataBuffer::VSDataBuffer()
{
	m_uiDT = DT_MAXNUM;

	m_uiNum = 0;
	m_pData = NULL;
	m_uiStructStride = 0;
}
VSDataBuffer::~VSDataBuffer()
{
	VSMAC_DELETEA(m_pData);
}

bool VSDataBuffer::SetData(const void *pData,unsigned int uiNum,unsigned int uiDT, unsigned int uiStructStride)
{
	VSMAC_ASSERT(uiDT < DT_MAXNUM && pData && uiNum);

	m_uiDT = uiDT;
	m_uiNum = uiNum;
	m_uiStructStride = uiStructStride;
	VSMAC_DELETEA(m_pData);

	m_pData = VS_NEW unsigned char[GetSize()];

	if(!m_pData)
		return 0;
	VSMemcpy(m_pData,pData,GetSize());
	m_uiSize = GetSize();
	return 1;
}
bool VSDataBuffer::AddData(const void *pData,unsigned int uiNum,unsigned int uiDT, unsigned int uiStructStride)
{
	VSMAC_ASSERT(uiDT < DT_MAXNUM && pData && uiNum);
	if (m_uiDT != DT_MAXNUM)
	{
		if (m_uiDT != uiDT)
		{
			return 0;
		}
		else if (uiStructStride != m_uiStructStride)
		{
			return 0;;
		}
	}
	else
	{
		m_uiDT = uiDT;
		m_uiStructStride = uiStructStride;
	}

	

	unsigned char * Temp = NULL;
	Temp = VS_NEW unsigned char[ ( uiNum + m_uiNum )* GetStride()];
	if(!Temp)
		return 0;
	VSMemcpy(Temp,m_pData,GetStride() * m_uiNum);
	VSMemcpy(Temp + GetStride() * m_uiNum,pData,GetStride() * uiNum);

	VSMAC_DELETEA(m_pData);
	m_uiNum += uiNum;
	m_pData = Temp;
	m_uiSize = GetSize();
	return 1;
}
bool VSDataBuffer::CreateEmptyBuffer(unsigned int uiNum,unsigned int uiDT, unsigned int uiStructStride)
{
	VSMAC_ASSERT(uiDT < DT_MAXNUM && uiNum);

	m_uiDT = uiDT;
	m_uiNum = uiNum;
	m_uiStructStride = uiStructStride;
	VSMAC_DELETEA(m_pData);

	m_pData = VS_NEW unsigned char[GetSize()];

	if(!m_pData)
		return 0;
	VSMemset(m_pData,0,GetSize());
	m_uiSize = GetSize();
	return 1;
}



