#pragma once
#include "VSVertexBuffer.h"
#include "VSIndexBuffer.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSMeshData : public VSObject
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY;
	public:
		enum //MeshData Type
		{
			MDT_POINT,
			MDT_LINE,
			MDT_TRIANGLE,
			MDT_CONTROL_POINT_4,
			MDT_MAX
		};
		virtual ~VSMeshData() = 0;
		VSMeshData();
		bool SetIndexBuffer(VSIndexBuffer * pIndexBuffer);
		inline VSIndexBuffer * GetIndexBuffer()const;

		bool SetVertexBuffer(VSVertexBuffer *pVertexBuffer);
		inline VSVertexBuffer *GetVertexBuffer()const;

		virtual unsigned int GetTotalNum()const = 0;

		inline unsigned int GetMeshDataType()
		{
			return m_uiMeshDataType;
		};
		virtual unsigned int GetActiveNum()const{ return GetTotalNum();}
		virtual unsigned int GetGirdNum(unsigned int uiInputNum)const = 0;
	protected:
		
		friend class VSGeometry;
		enum
		{
			DRAW_MATH_ELEMENT_LENGTH = 10000
		};
		VSVertexBufferPtr	m_pVertexBuffer;
		VSIndexBufferPtr	m_pIndexBuffer;
		unsigned int m_uiMeshDataType;
	};
	DECLARE_Ptr(VSMeshData);
	VSTYPE_MARCO(VSMeshData);
#include "VSMeshData.inl"
}
