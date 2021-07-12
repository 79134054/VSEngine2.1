#pragma once
#include "VSObject.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSDataBuffer : public VSObject
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		//modify the value
		enum	//Data Type
		{
			DT_FLOAT32_1,// 1D VSREAL expanded to (value, 0., 0., 1.)
			DT_FLOAT32_2,// 2D VSREAL expanded to (value.x, value.y, 0., 1.)
			DT_FLOAT32_3,// 3D VSREAL expanded to (value.x, value.y, value.z, 1.)
			DT_FLOAT32_4,// 4D VSREAL expanded to (value.x, value.y, value.z, value.w.)

			DT_USHORT,	// Index buffer 
			DT_UINT,	// Index buffer		 

			DT_UBYTE4,	// 4D unsigned byte
			DT_UBYTE4N, // Each of 4 bytes is normalized by dividing to 255.0

			DT_SHORT2,	// 2D signed short expanded to (value, value, 0., 1.)
			DT_SHORT2N,	// 2D signed short normalized (v[0]/32767.0,v[1]/32767.0,0,1)
			DT_USHORT2N,// 2D unsigned short normalized (v[0]/65535.0,v[1]/65535.0,0,1)

			DT_SHORT4,	// 4D signed short expanded to (value, value, value, value)
			DT_SHORT4N,	// 4D signed short normalized (v[0]/32767.0,v[1]/32767.0,v[2]/32767.0,v[3]/32767.0)
			DT_USHORT4N,// 4D unsigned short normalized (v[0]/65535.0,v[1]/65535.0,v[2]/65535.0,v[3]/65535.0)

			DT_FLOAT16_2,// Two 16-bit floating point values, expanded to (value, value, 0, 1)
			DT_FLOAT16_4,// Four 16-bit floating point values

			DT_COLOR,	// 4D packed unsigned bytes mapped to 0. to 1. range
						// Input is in D3DCOLOR format (ARGB) expanded to (R, G, B, A)
			DT_SINT,	
			DT_SINT2,
			DT_SINT3,
			DT_SINT4,

			DT_UINT2,
			DT_UINT3,
			DT_UINT4,

			DT_FLOAT16_1,
			DT_UBYTE,			// use byte address buffer

			DT_STRUCT = 127,	// use struct buffer
			DT_MAXNUM = 128
		};
		
		virtual ~VSDataBuffer();
		VSDataBuffer();
	public:
		inline unsigned int GetDT()const{return m_uiDT;}
		inline unsigned int GetStride()const{return m_uiDT == DT_STRUCT ? m_uiStructStride : ms_uiDataTypeByte[m_uiDT];}
		inline unsigned int GetChannel()const{return ms_uiDataTypeChannel[m_uiDT];}
		inline unsigned int GetNum()const{return m_uiNum;}
		inline unsigned int GetSize()const{return GetStride() * m_uiNum;}
		inline void *GetData()const {return m_pData;}
		inline unsigned int GetStructStride()const {return m_uiStructStride;}
		//如果你添加数据通道数大于规定数则返回0
		bool SetData(const void *pData,unsigned int uiNum,unsigned int uiDT , unsigned int uiStructStride = 0);
		//如果你添加数据通道数大于规定数则返回0
		bool AddData(const void *pData,unsigned int uiNum,unsigned int uiDT, unsigned int uiStructStride = 0);
		bool CreateEmptyBuffer(unsigned int uiNum,unsigned int uiDT, unsigned int uiStructStride = 0);
		static unsigned int ms_uiDataTypeByte[DT_MAXNUM];
		static unsigned int ms_uiDataTypeChannel[DT_MAXNUM];
	protected:
		unsigned int m_uiDT;
		unsigned int m_uiNum;
		unsigned char *m_pData;
		unsigned int m_uiSize;
		unsigned int m_uiStructStride;
	};
	DECLARE_Ptr(VSDataBuffer);
	VSTYPE_MARCO(VSDataBuffer);
}
