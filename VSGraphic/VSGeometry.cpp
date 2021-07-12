#include "VSGeometry.h"
#include "VSBoneNode.h"
#include "VSResourceManager.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
#include "VSTriangleSet.h"
#include "VSRenderThread.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSGeometry,VSSpatial)
BEGIN_ADD_PROPERTY(VSGeometry,VSSpatial)
REGISTER_PROPERTY(m_LocalBV, LocalBV, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_pMeshData,MeshData,VSProperty::F_SAVE_LOAD_COPY)
REGISTER_PROPERTY(m_BoneName, BoneName, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_GeometryName, GeometryName, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_pMaterialInstance, MaterialInstance, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME );
REGISTER_PROPERTY(m_MorphData, MorphData, VSProperty::F_CLONE);
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSGeometry)
ADD_PRIORITY(VSMaterial)
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION_WITH_PRIORITY(TerminalDefaultState)
IMPLEMENT_INITIAL_END
VSPointer<VSGeometry> VSGeometry::ms_Quad = NULL;
VSPointer<VSGeometry> VSGeometry::ms_DefaultCube = NULL;
VSPointer<VSGeometry> VSGeometry::ms_DefaultCubCone = NULL;
VSPointer<VSGeometry> VSGeometry::ms_DefaultCone = NULL;
VSPointer<VSGeometry> VSGeometry::ms_DefaultRenderCube = NULL;
bool VSGeometry::InitialDefaultState()
{
	ms_Quad = VS_NEW VSGeometry();
	if (!ms_Quad)
	{
		return false;
	}
	
	ms_DefaultCube = VS_NEW VSGeometry();
	if (!ms_DefaultCube)
	{
		return false;
	}

	ms_DefaultCubCone = VS_NEW VSGeometry();
	if (!ms_DefaultCubCone)
	{
		return false;
	}

	ms_DefaultCone = VS_NEW VSGeometry();
	if (!ms_DefaultCone)
	{
		return false;
	}
	ms_DefaultRenderCube = VS_NEW VSGeometry();
	if (!ms_DefaultRenderCube)
	{
		return false;
	}
	LoadDefault();
	return 1;
}
bool VSGeometry::TerminalDefaultState()
{
	ms_Quad = NULL;
	ms_DefaultCube = NULL;
	ms_DefaultCubCone = NULL;
	ms_DefaultCone = NULL;
	ms_DefaultRenderCube = NULL;
	return 1;
}
void VSGeometry::LoadDefault()
{
	//Quad
	{
		VSArray<VSVector3W> VertexArray;
		VSArray<VSVector2> m_TexCoordArray;
		VSArray<VSUSHORT_INDEX> IndexArray;

		VertexArray.AddElement(VSVector3W(-1.0f,  1.0f, 0.0f,1.0f));
		VertexArray.AddElement(VSVector3W( 1.0f,  1.0f, 0.0f,1.0f));
		VertexArray.AddElement(VSVector3W( 1.0f, -1.0f, 0.0f,1.0f));
		VertexArray.AddElement(VSVector3W(-1.0f, -1.0f, 0.0f,1.0f));

		m_TexCoordArray.AddElement(VSVector2(0.0f, 0.0f));
		m_TexCoordArray.AddElement(VSVector2(1.0f, 0.0f));
		m_TexCoordArray.AddElement(VSVector2(1.0f, 1.0f));
		m_TexCoordArray.AddElement(VSVector2(0.0f, 1.0f));

		IndexArray.AddElement(0);
		IndexArray.AddElement(1);
		IndexArray.AddElement(2);
		IndexArray.AddElement(0);
		IndexArray.AddElement(2);
		IndexArray.AddElement(3);


		VSDataBufferPtr  pVertexData = VS_NEW VSDataBuffer;
		pVertexData->SetData(&VertexArray[0],(unsigned int)VertexArray.GetNum(),VSDataBuffer::DT_FLOAT32_4);

		VSDataBufferPtr pTexCoord = VS_NEW VSDataBuffer;
		pTexCoord->SetData(&m_TexCoordArray[0],(unsigned int)m_TexCoordArray.GetNum(),VSDataBuffer::DT_FLOAT32_2);

		VSDataBufferPtr pIndex = VS_NEW VSDataBuffer;
		pIndex->SetData(&IndexArray[0],(unsigned int)IndexArray.GetNum(),VSDataBuffer::DT_USHORT);

		//创建顶点BUFFER
		VSVertexBufferPtr pVertexBuffer = VS_NEW VSVertexBuffer(true);
		pVertexBuffer->SetData(pVertexData,VSVertexFormat::VF_POSITION);
		pVertexBuffer->SetData(pTexCoord,VSVertexFormat::VF_TEXCOORD);

		VSIndexBufferPtr pIndexBuffer = VS_NEW VSIndexBuffer();

		pIndexBuffer->SetData(pIndex);

		VSTriangleSetPtr pTriangleSetData = VS_NEW VSTriangleSet();
		pTriangleSetData->SetVertexBuffer(pVertexBuffer);
		pTriangleSetData->SetIndexBuffer(pIndexBuffer);


		ms_Quad->SetMeshData(pTriangleSetData);
		ms_Quad->m_GeometryName = _T("DefaultQuad");
	}
	

	//Cub
	{
		VSArray<VSVector3> VertexArray;

		VSArray<VSUSHORT_INDEX> IndexArray;

		VertexArray.AddElement(VSVector3(-1.0f,  1.0f, 0.0f));
		VertexArray.AddElement(VSVector3( 1.0f,  1.0f, 0.0f));
		VertexArray.AddElement(VSVector3( 1.0f, -1.0f, 0.0f));
		VertexArray.AddElement(VSVector3(-1.0f, -1.0f, 0.0f));

		VertexArray.AddElement(VSVector3(-1.0f,  1.0f, 1.0f));
		VertexArray.AddElement(VSVector3( 1.0f,  1.0f, 1.0f));
		VertexArray.AddElement(VSVector3( 1.0f, -1.0f, 1.0f));
		VertexArray.AddElement(VSVector3(-1.0f, -1.0f, 1.0f));

		//front
		IndexArray.AddElement(0);
		IndexArray.AddElement(1);
		IndexArray.AddElement(2);
		IndexArray.AddElement(0);
		IndexArray.AddElement(2);
		IndexArray.AddElement(3);

		//back
		IndexArray.AddElement(4);
		IndexArray.AddElement(6);
		IndexArray.AddElement(5);
		IndexArray.AddElement(4);
		IndexArray.AddElement(7);
		IndexArray.AddElement(6);

		//left
		IndexArray.AddElement(0);
		IndexArray.AddElement(7);
		IndexArray.AddElement(4);
		IndexArray.AddElement(0);
		IndexArray.AddElement(3);
		IndexArray.AddElement(7);

		//right
		IndexArray.AddElement(1);
		IndexArray.AddElement(5);
		IndexArray.AddElement(6);
		IndexArray.AddElement(1);
		IndexArray.AddElement(6);
		IndexArray.AddElement(2);

		//up
		IndexArray.AddElement(0);
		IndexArray.AddElement(4);
		IndexArray.AddElement(5);
		IndexArray.AddElement(0);
		IndexArray.AddElement(5);
		IndexArray.AddElement(1);

		//bottom
		IndexArray.AddElement(3);
		IndexArray.AddElement(6);
		IndexArray.AddElement(7);
		IndexArray.AddElement(3);
		IndexArray.AddElement(2);
		IndexArray.AddElement(6);

		VSDataBufferPtr  pVertexData = VS_NEW VSDataBuffer;
		pVertexData->SetData(&VertexArray[0],(unsigned int)VertexArray.GetNum(),VSDataBuffer::DT_FLOAT32_3);

		VSDataBufferPtr pIndex = VS_NEW VSDataBuffer;
		pIndex->SetData(&IndexArray[0],(unsigned int)IndexArray.GetNum(),VSDataBuffer::DT_USHORT);

		//创建顶点BUFFER
		VSVertexBufferPtr pVertexBuffer = VS_NEW VSVertexBuffer(true);
		pVertexBuffer->SetData(pVertexData,VSVertexFormat::VF_POSITION);

		VSIndexBufferPtr pIndexBuffer = VS_NEW VSIndexBuffer();

		pIndexBuffer->SetData(pIndex);

		VSTriangleSetPtr pTriangleSetData = VS_NEW VSTriangleSet();
		pTriangleSetData->SetVertexBuffer(pVertexBuffer);
		pTriangleSetData->SetIndexBuffer(pIndexBuffer);


		ms_DefaultCube->SetMeshData(pTriangleSetData);
		ms_DefaultCube->m_GeometryName = _T("DefaultCub");
	}

	//Cub Cone
	{
		VSArray<VSVector3> VertexArray;

		VSArray<VSUSHORT_INDEX> IndexArray;

		VertexArray.AddElement(VSVector3( 0.0f,  0.0f, 0.0f));
		

		VertexArray.AddElement(VSVector3(-1.0f,  1.0f, 1.0f));
		VertexArray.AddElement(VSVector3( 1.0f,  1.0f, 1.0f));
		VertexArray.AddElement(VSVector3( 1.0f, -1.0f, 1.0f));
		VertexArray.AddElement(VSVector3(-1.0f, -1.0f, 1.0f));

		//front
		IndexArray.AddElement(1);
		IndexArray.AddElement(3);
		IndexArray.AddElement(2);
		IndexArray.AddElement(1);
		IndexArray.AddElement(4);
		IndexArray.AddElement(3);


		//right
		IndexArray.AddElement(0);
		IndexArray.AddElement(2);
		IndexArray.AddElement(3);


		//left
		IndexArray.AddElement(0);
		IndexArray.AddElement(4);
		IndexArray.AddElement(1);


		//up
		IndexArray.AddElement(0);
		IndexArray.AddElement(1);
		IndexArray.AddElement(2);


		//bottom
		IndexArray.AddElement(0);
		IndexArray.AddElement(3);
		IndexArray.AddElement(4);


		VSDataBufferPtr  pVertexData = VS_NEW VSDataBuffer;
		pVertexData->SetData(&VertexArray[0],(unsigned int)VertexArray.GetNum(),VSDataBuffer::DT_FLOAT32_3);

		VSDataBufferPtr pIndex = VS_NEW VSDataBuffer;
		pIndex->SetData(&IndexArray[0],(unsigned int)IndexArray.GetNum(),VSDataBuffer::DT_USHORT);

		//创建顶点BUFFER
		VSVertexBufferPtr pVertexBuffer = VS_NEW VSVertexBuffer(true);
		pVertexBuffer->SetData(pVertexData,VSVertexFormat::VF_POSITION);

		VSIndexBufferPtr pIndexBuffer = VS_NEW VSIndexBuffer();

		pIndexBuffer->SetData(pIndex);

		VSTriangleSetPtr pTriangleSetData = VS_NEW VSTriangleSet();
		pTriangleSetData->SetVertexBuffer(pVertexBuffer);
		pTriangleSetData->SetIndexBuffer(pIndexBuffer);


		ms_DefaultCubCone->SetMeshData(pTriangleSetData);
		ms_DefaultCubCone->m_GeometryName = _T("DefaultCubCone");
	}

	//Cone
	{
		unsigned int uiLevel = 1;
		unsigned int CircleLevel = 5;


		VSArray<VSVector3> VertexArray;

		VSArray<VSUSHORT_INDEX> IndexArray;

		VertexArray.AddElement(VSVector3( 0.0f,  0.0f, 0.0f));


		VSREAL RangeStep = 1.0f / uiLevel;
		for (unsigned int i = 0 ; i < uiLevel ; i++)
		{
			VSREAL Z = RangeStep * (i + 1);
			for (unsigned int j = 0 ; j < CircleLevel ; j++)
			{
				VSREAL Radio = ( j * 1.0f ) / CircleLevel * VS2PI;
				VSREAL X = Z * COS(Radio);
				VSREAL Y = Z * SIN(Radio);
				VertexArray.AddElement(VSVector3(X,Y,Z));
			}
		}
		VertexArray.AddElement(VSVector3(0.0f,0.0f, 1.0f));
		

		// first level
		for (unsigned int i = 0 ; i < CircleLevel ; i++)
		{
			IndexArray.AddElement(0);

			unsigned int Index1 = i + 2;
			unsigned int Index2 = i + 1;
			if (Index1 = CircleLevel + 1)
			{
				Index1 = 1;
			}
			IndexArray.AddElement(Index1);
			IndexArray.AddElement(Index2);
		}

		//
		for (unsigned int i = 1 ; i < uiLevel ; i++)
		{
			for (unsigned int j = 0 ; j < CircleLevel ; j++)
			{
				unsigned int TopIndex1 = (i - 1) * CircleLevel + 1 + j;
				unsigned int TopIndex2 = (i - 1) * CircleLevel + 1 + j + 1;
				
				
				
				unsigned int BottomIndex1 = i * CircleLevel + 1 + j;
				unsigned int BottomIndex2 = i * CircleLevel + 1 + j + 1;

				if (j + 1 == CircleLevel)
				{
					TopIndex2 = (i - 1) * CircleLevel + 1 ;
					BottomIndex2 = i * CircleLevel + 1;
				}

				IndexArray.AddElement(TopIndex1);
				IndexArray.AddElement(TopIndex2);
				IndexArray.AddElement(BottomIndex1);
				IndexArray.AddElement(TopIndex2);
				IndexArray.AddElement(BottomIndex1);
				IndexArray.AddElement(BottomIndex2);
			}
		}

		// last level
		for (unsigned int i = 0 ; i < CircleLevel ; i++)
		{
			IndexArray.AddElement(VertexArray.GetNum() - 1);

			unsigned int Index1 = VertexArray.GetNum() - 3 - i;
			unsigned int Index2 = VertexArray.GetNum() - 2 - i;
			
			if (i == CircleLevel - 1)
			{
				Index1 = VertexArray.GetNum() - 2;
			}
			IndexArray.AddElement(Index1);
			IndexArray.AddElement(Index2);
		}

		

		VSDataBufferPtr  pVertexData = VS_NEW VSDataBuffer;
		pVertexData->SetData(&VertexArray[0],(unsigned int)VertexArray.GetNum(),VSDataBuffer::DT_FLOAT32_3);

		VSDataBufferPtr pIndex = VS_NEW VSDataBuffer;
		pIndex->SetData(&IndexArray[0],(unsigned int)IndexArray.GetNum(),VSDataBuffer::DT_USHORT);

		//创建顶点BUFFER
		VSVertexBufferPtr pVertexBuffer = VS_NEW VSVertexBuffer(true);
		pVertexBuffer->SetData(pVertexData,VSVertexFormat::VF_POSITION);

		VSIndexBufferPtr pIndexBuffer = VS_NEW VSIndexBuffer();

		pIndexBuffer->SetData(pIndex);

		VSTriangleSetPtr pTriangleSetData = VS_NEW VSTriangleSet();
		pTriangleSetData->SetVertexBuffer(pVertexBuffer);
		pTriangleSetData->SetIndexBuffer(pIndexBuffer);


		ms_DefaultCone->SetMeshData(pTriangleSetData);
		ms_DefaultCone->m_GeometryName = _T("DefaultCone");
	}

	//Quad Cub
	{
		VSArray<VSVector3> VertexArray;
		VSArray<VSVector2> TexCoordArray;
		VSArray<VSVector3> NormalArray;
		VSArray<VSUSHORT_INDEX> IndexArray;

		//pos uv
		VertexArray.AddElement(VSVector3(-1.0f, 1.0f, 1.0f));
		VertexArray.AddElement(VSVector3(1.0f, 1.0f, 1.0f));
		VertexArray.AddElement(VSVector3(1.0f, -1.0f, 1.0f));
		VertexArray.AddElement(VSVector3(-1.0f, -1.0f, 1.0f));

		TexCoordArray.AddElement(VSVector2(0.0f, 0.0f));
		TexCoordArray.AddElement(VSVector2(0.0f, 1.0f));
		TexCoordArray.AddElement(VSVector2(1.0f, 1.0f));
		TexCoordArray.AddElement(VSVector2(1.0f, 0.0f));

		NormalArray.AddElement(VSVector3(0.0f, 0.0f, 1.0f));
		NormalArray.AddElement(VSVector3(0.0f, 0.0f, 1.0f));
		NormalArray.AddElement(VSVector3(0.0f, 0.0f, 1.0f));
		NormalArray.AddElement(VSVector3(0.0f, 0.0f, 1.0f));

		//pos uv
		VertexArray.AddElement(VSVector3(-1.0f, 1.0f, -1.0f));
		VertexArray.AddElement(VSVector3(1.0f, 1.0f, -1.0f));
		VertexArray.AddElement(VSVector3(1.0f, -1.0f, -1.0f));
		VertexArray.AddElement(VSVector3(-1.0f, -1.0f, -1.0f));

		TexCoordArray.AddElement(VSVector2(0.0f, 0.0f));
		TexCoordArray.AddElement(VSVector2(0.0f, 1.0f));
		TexCoordArray.AddElement(VSVector2(1.0f, 1.0f));
		TexCoordArray.AddElement(VSVector2(1.0f, 0.0f));

		NormalArray.AddElement(VSVector3(0.0f, 0.0f, -1.0f));
		NormalArray.AddElement(VSVector3(0.0f, 0.0f, -1.0f));
		NormalArray.AddElement(VSVector3(0.0f, 0.0f, -1.0f));
		NormalArray.AddElement(VSVector3(0.0f, 0.0f, -1.0f));

		//pos uv
		VertexArray.AddElement(VSVector3(-1.0f, 1.0f, 1.0f));
		VertexArray.AddElement(VSVector3(-1.0f, 1.0f, -1.0f));
		VertexArray.AddElement(VSVector3(-1.0f, -1.0f, -1.0f));
		VertexArray.AddElement(VSVector3(-1.0f, -1.0f, 1.0f));

		TexCoordArray.AddElement(VSVector2(0.0f, 0.0f));
		TexCoordArray.AddElement(VSVector2(0.0f, 1.0f));
		TexCoordArray.AddElement(VSVector2(1.0f, 1.0f));
		TexCoordArray.AddElement(VSVector2(1.0f, 0.0f));

		NormalArray.AddElement(VSVector3(-1.0f, 0.0f, 0.0f));
		NormalArray.AddElement(VSVector3(-1.0f, 0.0f, 0.0f));
		NormalArray.AddElement(VSVector3(-1.0f, 0.0f, 0.0f));
		NormalArray.AddElement(VSVector3(-1.0f, 0.0f, 0.0f));

		//pos uv
		VertexArray.AddElement(VSVector3(1.0f, 1.0f, 1.0f));
		VertexArray.AddElement(VSVector3(1.0f, -1.0f, 1.0f));
		VertexArray.AddElement(VSVector3(1.0f, -1.0f, -1.0f));
		VertexArray.AddElement(VSVector3(1.0f, 1.0f, -1.0f));

		TexCoordArray.AddElement(VSVector2(0.0f, 0.0f));
		TexCoordArray.AddElement(VSVector2(0.0f, 1.0f));
		TexCoordArray.AddElement(VSVector2(1.0f, 1.0f));
		TexCoordArray.AddElement(VSVector2(1.0f, 0.0f));

		NormalArray.AddElement(VSVector3(1.0f, 0.0f, 0.0f));
		NormalArray.AddElement(VSVector3(1.0f, 0.0f, 0.0f));
		NormalArray.AddElement(VSVector3(1.0f, 0.0f, 0.0f));
		NormalArray.AddElement(VSVector3(1.0f, 0.0f, 0.0f));

		//pos uv
		VertexArray.AddElement(VSVector3(1.0f, 1.0f, 1.0f));
		VertexArray.AddElement(VSVector3(1.0f, 1.0f, -1.0f));
		VertexArray.AddElement(VSVector3(-1.0f, 1.0f, -1.0f));
		VertexArray.AddElement(VSVector3(-1.0f, 1.0f, 1.0f));

		TexCoordArray.AddElement(VSVector2(0.0f, 0.0f));
		TexCoordArray.AddElement(VSVector2(0.0f, 1.0f));
		TexCoordArray.AddElement(VSVector2(1.0f, 1.0f));
		TexCoordArray.AddElement(VSVector2(1.0f, 0.0f));

		NormalArray.AddElement(VSVector3(0.0f, 1.0f, 0.0f));
		NormalArray.AddElement(VSVector3(0.0f, 1.0f, 0.0f));
		NormalArray.AddElement(VSVector3(0.0f, 1.0f, 0.0f));
		NormalArray.AddElement(VSVector3(0.0f, 1.0f, 0.0f));

		//pos uv
		VertexArray.AddElement(VSVector3(1.0f, -1.0f, 1.0f));
		VertexArray.AddElement(VSVector3(-1.0f, -1.0f, 1.0f));
		VertexArray.AddElement(VSVector3(-1.0f, -1.0f, -1.0f));
		VertexArray.AddElement(VSVector3(1.0f, -1.0f, -1.0f));

		TexCoordArray.AddElement(VSVector2(0.0f, 0.0f));
		TexCoordArray.AddElement(VSVector2(0.0f, 1.0f));
		TexCoordArray.AddElement(VSVector2(1.0f, 1.0f));
		TexCoordArray.AddElement(VSVector2(1.0f, 0.0f));

		NormalArray.AddElement(VSVector3(0.0f, -1.0f, 0.0f));
		NormalArray.AddElement(VSVector3(0.0f, -1.0f, 0.0f));
		NormalArray.AddElement(VSVector3(0.0f, -1.0f, 0.0f));
		NormalArray.AddElement(VSVector3(0.0f, -1.0f, 0.0f));

		//front
		IndexArray.AddElement(0);
		IndexArray.AddElement(2);
		IndexArray.AddElement(1);
		IndexArray.AddElement(0);
		IndexArray.AddElement(3);
		IndexArray.AddElement(2);

		//back
		IndexArray.AddElement(4);
		IndexArray.AddElement(5);
		IndexArray.AddElement(6);
		IndexArray.AddElement(4);
		IndexArray.AddElement(6);
		IndexArray.AddElement(7);

		//left
		IndexArray.AddElement(8);
		IndexArray.AddElement(9);
		IndexArray.AddElement(10);
		IndexArray.AddElement(8);
		IndexArray.AddElement(10);
		IndexArray.AddElement(11);

		//right
		IndexArray.AddElement(12);
		IndexArray.AddElement(13);
		IndexArray.AddElement(14);
		IndexArray.AddElement(12);
		IndexArray.AddElement(14);
		IndexArray.AddElement(15);

		//up
		IndexArray.AddElement(16);
		IndexArray.AddElement(17);
		IndexArray.AddElement(18);
		IndexArray.AddElement(16);
		IndexArray.AddElement(18);
		IndexArray.AddElement(19);

		//bottom
		IndexArray.AddElement(20);
		IndexArray.AddElement(21);
		IndexArray.AddElement(22);
		IndexArray.AddElement(20);
		IndexArray.AddElement(22);
		IndexArray.AddElement(23);

		VSDataBufferPtr  pVertexData = VS_NEW VSDataBuffer;
		pVertexData->SetData(&VertexArray[0], (unsigned int)VertexArray.GetNum(), VSDataBuffer::DT_FLOAT32_3);

		VSDataBufferPtr pTexCoord = VS_NEW VSDataBuffer;
		pTexCoord->SetData(&TexCoordArray[0], TexCoordArray.GetNum(), VSDataBuffer::DT_FLOAT32_2);

		VSDataBufferPtr  pNormalData = VS_NEW VSDataBuffer;
		pNormalData->SetData(&NormalArray[0], (unsigned int)NormalArray.GetNum(), VSDataBuffer::DT_FLOAT32_3);

		VSDataBufferPtr pIndex = VS_NEW VSDataBuffer;
		pIndex->SetData(&IndexArray[0], (unsigned int)IndexArray.GetNum(), VSDataBuffer::DT_USHORT);

		//创建顶点BUFFER
		VSVertexBufferPtr pVertexBuffer = VS_NEW VSVertexBuffer(true);
		pVertexBuffer->SetData(pVertexData, VSVertexFormat::VF_POSITION);
		pVertexBuffer->SetData(pTexCoord, VSVertexFormat::VF_TEXCOORD);
		pVertexBuffer->SetData(pNormalData, VSVertexFormat::VF_NORMAL);

		VSIndexBufferPtr pIndexBuffer = VS_NEW VSIndexBuffer();

		pIndexBuffer->SetData(pIndex);

		VSTriangleSetPtr pTriangleSetData = VS_NEW VSTriangleSet();
		pTriangleSetData->SetVertexBuffer(pVertexBuffer);
		pTriangleSetData->SetIndexBuffer(pIndexBuffer);


		ms_DefaultRenderCube->SetMeshData(pTriangleSetData);
		ms_DefaultRenderCube->m_GeometryName = _T("DefaultRenderCube");

		ms_DefaultRenderCube->AddMaterialInstance((VSMaterialR *)VSMaterial::GetDefaultResource());
	}
}
void VSGeometry::UpdateGeometryBeforeDrawCall()
{
	
}
void VSGeometry::SetMorphData(unsigned int Index, float fData)
{
	unsigned int i = m_MorphData.ValueMap.Find(Index);
	if (i != m_MorphData.ValueMap.GetNum())
	{
		m_MorphData.ValueMap[i].Value = fData;
	}
}
bool VSGeometry::HasMorphTarget()const
{
	return m_MorphData.MorphVDataTexture != NULL;
}
void VSGeometry::CreateMorphMeshData(VSMap<unsigned int, VSVertexBuffer *> & MorphDataSet)
{
	if (!m_pMeshData)
	{
		return;
	}
	unsigned int DataNum = 1;
	VSVertexBuffer * pVertexBuffer = m_pMeshData->GetVertexBuffer();
	if (pVertexBuffer->GetNormalData(0))
	{
		DataNum++;
		if (pVertexBuffer->GetTangentData())
		{
			DataNum++;
			if (pVertexBuffer->GetBinormalData())
			{
				DataNum++;
			}
		}
	}
	unsigned int MorphNum = 0;
	for (unsigned int i = 0 ; i < MorphDataSet.GetNum() ; i++)
	{
		if (MorphDataSet[i].Value)
		{
			MorphNum++;
			m_MorphData.ValueMap.AddElement(i, 0.0f);
		}
	}	
	if (!MorphNum)
	{
		return;
	}
	unsigned int TexSizeX = VSResourceManager::GetGpuMorphTextureSize();

	unsigned int Layer = (pVertexBuffer->GetVertexNum() / TexSizeX) + 1;
	m_MorphData.Layer = Layer;
	unsigned int MaxY = Layer * DataNum * MorphNum;
	unsigned int TexSizeY = 0;
	for (unsigned int i = 0; i <= 10; i++)
	{
		unsigned int Datai = 1 << i;
		unsigned int Datai_1 = (1 + 1) << i;
		if (MaxY > Datai && MaxY <= Datai_1)
		{
			TexSizeY = Datai_1;
			break;
		}
	}
	VSMAC_ASSERT(TexSizeY);
	m_MorphData.TextureSizeY = TexSizeY;
#define DEFINE_RGBA32_MORPH_DATA(Texture,GetSemanticsDataFunction)\
	{\
		VSVector3W * pData = VS_NEW VSVector3W[TexSizeX * TexSizeY];\
		unsigned int uiNum = 0;\
		for (unsigned int i = 0; i < MorphDataSet.GetNum(); i++)\
		{\
			VSVertexBuffer * pMorphVertexBuffer = MorphDataSet[i].Value;\
			if (!pVertexBuffer)\
			{\
				continue;\
			}\
			VSVector3W * pDataTemp = pData + uiNum * TexSizeX * Layer;\
			VSVector3 *pTemp = (VSVector3 *)pMorphVertexBuffer->GetSemanticsDataFunction->GetData();\
			for (unsigned int j = 0; j < pMorphVertexBuffer->GetVertexNum(); j++)\
			{\
				pDataTemp->x = pTemp->x;\
				pDataTemp->y = pTemp->y;\
				pDataTemp->z = pTemp->z;\
				pDataTemp++; pTemp++;\
			}\
			uiNum++;\
		}\
		m_MorphData.Texture = VSResourceManager::Create2DTexture(TexSizeX, TexSizeY, VSRenderer::SFT_A32B32G32R32F, 1, pData);\
		m_MorphData.Texture->SetSamplerState((VSSamplerState*)VSSamplerState::GetClamp());\
		VSMAC_DELETEA(pData);\
	}


#define DEFINE_RGBA8_MORPH_DATA(Texture,GetSemanticsDataFunction)\
	{\
		DWORD * pData = VS_NEW DWORD[TexSizeX * TexSizeY];\
		unsigned int uiNum = 0;\
		for (unsigned int i = 0; i < MorphDataSet.GetNum(); i++)\
		{\
			VSVertexBuffer * pMorphVertexBuffer = MorphDataSet[i].Value;\
			if (!pVertexBuffer)\
			{\
				continue;\
			}\
			DWORD * pDataTemp = pData + uiNum * TexSizeX * Layer;\
			DWORD *pTemp = (DWORD *)pMorphVertexBuffer->GetSemanticsDataFunction->GetData();\
			for (unsigned int j = 0; j < pMorphVertexBuffer->GetVertexNum(); j++)\
			{\
				*pDataTemp = *pTemp;\
				pDataTemp++; pTemp++;\
			}\
			uiNum++;\
		}\
		m_MorphData.Texture = VSResourceManager::Create2DTexture(TexSizeX, TexSizeY, VSRenderer::SFT_A8R8G8B8, 1, pData);\
		m_MorphData.Texture->SetSamplerState((VSSamplerState*)VSSamplerState::GetClamp());\
		VSMAC_DELETEA(pData);\
	}


	DEFINE_RGBA32_MORPH_DATA(MorphVDataTexture, GetPositionData(0));
	VSDataBuffer * pNormal = pVertexBuffer->GetNormalData(0);
	if (pNormal->GetDT() == VSDataBuffer::DT_FLOAT32_3)
	{
		DEFINE_RGBA32_MORPH_DATA(MorphNDataTexture, GetNormalData(0));
	}
	else
	{
		DEFINE_RGBA8_MORPH_DATA(MorphNDataTexture, GetNormalData(0));
	}
	VSDataBuffer * pTangent = pVertexBuffer->GetTangentData();
	if (pTangent)
	{
		if (pTangent->GetDT() == VSDataBuffer::DT_FLOAT32_3)
		{
			DEFINE_RGBA32_MORPH_DATA(MorphTDataTexture, GetTangentData());
			DEFINE_RGBA32_MORPH_DATA(MorphBDataTexture, GetBinormalData());
		}
		else
		{
			DEFINE_RGBA8_MORPH_DATA(MorphTDataTexture, GetTangentData());
		}
	}
	if (!VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_VertexIDInShader))
	{
		VSDataBuffer * pIDBuffer = VS_NEW VSDataBuffer();
		VSArray<float> IDArray;
		for (unsigned int i = 0 ; i < pVertexBuffer->GetVertexNum() ; i++)
		{
			IDArray.AddElement(i * 1.0f);
		}
		pIDBuffer->SetData(&IDArray[0], IDArray.GetNum(), VSDataBuffer::DT_FLOAT32_1);
		pVertexBuffer->SetData(pIDBuffer, VSVertexFormat::VF_VERTEX_ID);
	}
}
void VSGeometry::AddMorphAABB(VSVertexBuffer * pMorphVertexBuffer)
{
	VSMAC_ASSERT(pMorphVertexBuffer);

	//LinkBoneNode();
	VSMAC_ASSERT(m_pMeshData && m_pMeshData->GetVertexBuffer())
	{
		VSAABB3 NewAABB;

		VSVertexBuffer * pVerBuffer = m_pMeshData->GetVertexBuffer();
		VSMAC_ASSERT(pMorphVertexBuffer->GetPositionData(0));


		VSVector3 * pVer = (VSVector3*)pMorphVertexBuffer->GetPositionData(0)->GetData();
		VSMAC_ASSERT(pVer);

		unsigned int uiVertexNum = pVerBuffer->GetPositionData(0)->GetNum();
		VSTransform World = m_pParent->GetWorldTransform();
		if (GetAffectSkeleton())
		{
			VSDataBuffer* pBlendIndex = pVerBuffer->GetBlendIndicesData(0);
			VSDataBuffer* pBoneWeight = pVerBuffer->GetBlendWeightData();

			if (!pBlendIndex || !pBoneWeight)
			{
				return;
			}
			VSArray<VSVector3>TempBuffer;
			TempBuffer.SetBufferNum(uiVertexNum);

			if (pBlendIndex->GetDT() == VSDataBuffer::DT_UBYTE4)
			{
				DWORD * pBlendIndexData = (DWORD *)pBlendIndex->GetData();
				DWORD * pBoneWeightData = (DWORD *)pBoneWeight->GetData();
				for (unsigned int i = 0; i < uiVertexNum; i++)
				{
					VSVector3W BoneWeight;
					BoneWeight.CreateFromABGR(pBoneWeightData[i]);
					unsigned char BlendIndex[4];
					VSDWCOLORGetABGR(pBlendIndexData[i], BlendIndex[0], BlendIndex[1], BlendIndex[2], BlendIndex[3]);
					TempBuffer[i].Set(0.0f, 0.0f, 0.0f);
					for (unsigned int k = 0; k < 4; k++)
					{

						VSBoneNode * pBone = GetAffectBone(BlendIndex[k]);
						if (pBone)
						{
							VSTransform BoneWorld = pBone->GetWorldTransform();

							VSMatrix3X3W TempBone = pBone->GetBoneOffsetMatrix() * BoneWorld.GetCombine() * World.GetCombineInverse();
							TempBuffer[i] += pVer[i] * TempBone * BoneWeight.m[k];
						}
					}

				}
			}
			else
			{
				VSVector3W * pBlendIndexData = (VSVector3W *)pBlendIndex->GetData();
				VSVector3W * pBoneWeightData = (VSVector3W *)pBoneWeight->GetData();
				for (unsigned int i = 0; i < uiVertexNum; i++)
				{
					TempBuffer[i].Set(0.0f, 0.0f, 0.0f);
					for (unsigned int k = 0; k < 4; k++)
					{
						unsigned int BlendIndex = (unsigned int)pBlendIndexData[i].m[k];
						VSBoneNode * pBone = GetAffectBone(BlendIndex);
						if (pBone)
						{
							VSTransform BoneWorld = pBone->GetWorldTransform();

							VSMatrix3X3W TempBone = pBone->GetBoneOffsetMatrix() * BoneWorld.GetCombine() * World.GetCombineInverse();
							TempBuffer[i] += pVer[i] * TempBone * pBoneWeightData[i].m[k];
						}
					}

				}
			}

			NewAABB.CreateAABB(TempBuffer.GetBuffer(), uiVertexNum);
		}
		else
		{
			NewAABB.CreateAABB(pVer, uiVertexNum);
		}
		m_LocalBV = m_LocalBV.MergeAABB(NewAABB);

	}
}
void VSGeometry::CreateLocalAABB()
{
	LinkBoneNode();
	VSMAC_ASSERT(m_pMeshData && m_pMeshData->GetVertexBuffer())
	{
		VSAABB3 NewAABB;

		VSVertexBuffer * pVerBuffer = m_pMeshData->GetVertexBuffer();
		VSMAC_ASSERT(pVerBuffer->GetPositionData(0))


		VSVector3 * pVer = (VSVector3*)pVerBuffer->GetPositionData(0)->GetData();
		VSMAC_ASSERT(pVer);

		unsigned int uiVertexNum = pVerBuffer->GetPositionData(0)->GetNum();
		VSTransform World  = m_pParent->GetWorldTransform();
		if (GetAffectSkeleton())
		{
			VSDataBuffer* pBlendIndex = pVerBuffer->GetBlendIndicesData(0);
			VSDataBuffer* pBoneWeight = pVerBuffer->GetBlendWeightData();

			if (!pBlendIndex || !pBoneWeight)
			{
				return ;
			}
			VSArray<VSVector3>TempBuffer;
			TempBuffer.SetBufferNum(uiVertexNum);

			if (pBlendIndex->GetDT() == VSDataBuffer::DT_UBYTE4)
			{
				DWORD * pBlendIndexData = (DWORD *)pBlendIndex->GetData();
				DWORD * pBoneWeightData = (DWORD *)pBoneWeight->GetData();	
				for (unsigned int i = 0; i < uiVertexNum ;i++)
				{
					VSVector3W BoneWeight;
					BoneWeight.CreateFromABGR(pBoneWeightData[i]);
					unsigned char BlendIndex[4];
					VSDWCOLORGetABGR(pBlendIndexData[i],BlendIndex[0],BlendIndex[1],BlendIndex[2],BlendIndex[3]);
					TempBuffer[i].Set(0.0f,0.0f,0.0f);
					for (unsigned int k = 0 ; k < 4 ; k++)
					{

						VSBoneNode * pBone = GetAffectBone(BlendIndex[k]);
						if(pBone)
						{
							VSTransform BoneWorld = pBone->GetWorldTransform();

							VSMatrix3X3W TempBone = pBone->GetBoneOffsetMatrix() * BoneWorld.GetCombine() * World.GetCombineInverse();	
							TempBuffer[i] += pVer[i] * TempBone * BoneWeight.m[k];
						}
					}
					
				}
			}
			else
			{
				VSVector3W * pBlendIndexData = (VSVector3W *)pBlendIndex->GetData();
				VSVector3W * pBoneWeightData = (VSVector3W *)pBoneWeight->GetData();	
				for (unsigned int i = 0; i < uiVertexNum ;i++)
				{
					TempBuffer[i].Set(0.0f,0.0f,0.0f);
					for (unsigned int k = 0 ; k < 4 ; k++)
					{
						unsigned int BlendIndex = (unsigned int) pBlendIndexData[i].m[k];
						VSBoneNode * pBone = GetAffectBone(BlendIndex);
						if(pBone)
						{
							VSTransform BoneWorld = pBone->GetWorldTransform();

							VSMatrix3X3W TempBone = pBone->GetBoneOffsetMatrix() * BoneWorld.GetCombine() * World.GetCombineInverse();	
							TempBuffer[i] += pVer[i] * TempBone * pBoneWeightData[i].m[k];
						}
					}

				}
			}
			
			NewAABB.CreateAABB(TempBuffer.GetBuffer(),uiVertexNum);
		}
		else
		{		
			NewAABB.CreateAABB(pVer,uiVertexNum);
		}
		m_LocalBV = NewAABB;
		
	}
	
}
void VSGeometry::UpdateWorldBound(double dAppTime)
{
	m_WorldBV.Transform(m_LocalBV,m_World.GetCombine());
	if (m_pParent)
	{
		m_pParent->m_bIsChanged = true;;
	}
	
}
void VSGeometry::UpdateNodeAll(double dAppTime)
{
	
	if (dAppTime > 0.0f)
	{
		UpdateController(dAppTime);
	}

	UpdateTransform(dAppTime);
	if (dAppTime > 0.0f)
	{
		UpdateOther(dAppTime);
	}
	if(m_bIsChanged)
	{
		UpdateWorldBound(dAppTime); 
	}
	m_bIsChanged = false;

	
}
void VSGeometry::UpdateView(VSCuller & Culler, double dAppTime)
{
	VSSpatial::UpdateView(Culler,dAppTime);
	VSCamera * pCamera = Culler.GetCamera();
	VSMAC_ASSERT(pCamera);

	if (Culler.GetCullerType() == VSCuller::CUT_MAIN)
	{
		
	}
}
void VSGeometry::ComputeNodeVisibleSet(VSCuller & Culler,bool bNoCull,double dAppTime)
{
	if (Culler.CullGeometry(this))
	{
		return;
	}
	UpdateView(Culler,dAppTime);
	VSMeshNode * pMeshNode = GetMeshNode();
	VSMAC_ASSERT(pMeshNode);

	unsigned int uiRenderGroup = pMeshNode->GetRenderGroup();
	VSMAC_ASSERT(uiRenderGroup < VSCuller::RG_MAX);

	VSMaterialInstance * pMaterialInstance = NULL;
	if (Culler.GetUseMaterialIndex() == -1 )
	{
		pMaterialInstance = GetUseMaterialInstance();
		if (!pMaterialInstance)
		{
			AddMaterialInstance((VSMaterialR *)VSMaterial::GetDefaultResource());
			pMaterialInstance = GetUseMaterialInstance();
		}
	}
	else
	{
		pMaterialInstance = GetMaterialInstance(Culler.GetUseMaterialIndex());
	}
	if (!pMaterialInstance)
	{
		return;
	}

	VSMaterial * pMaterial = pMaterialInstance->GetMaterial();
	for (unsigned int i = 0 ; i < pMaterial->GetShaderMainFunctionNum() ;i++)
	{
		
		VSRenderContext VisibleContext;
		VisibleContext.m_pGeometry = this;
		VisibleContext.m_pMaterialInstance = pMaterialInstance;
		VisibleContext.m_uiPassId = i;
		VisibleContext.m_pMaterial = pMaterial;
		VisibleContext.m_pMeshNode = pMeshNode;
		const VSBlendDesc & BlendDest = pMaterial->GetRenderState(i).GetBlendState()->GetBlendDesc();
		if (Culler.GetCullerType() == VSCuller::CUT_SHADOW)
		{

			if (BlendDest.IsBlendUsed())
			{
				return;
			}
			else
			{
				Culler.InsertObject(VisibleContext, VSCuller::VST_BASE, uiRenderGroup);
				return;
			}
		}
		else
		{
			if (pMaterial->GetCombine())
			{
				Culler.InsertObject(VisibleContext, VSCuller::VST_COMBINE, uiRenderGroup);
			}
			else
			{
				if (BlendDest.IsBlendUsed())
				{
					Culler.InsertObject(VisibleContext, VSCuller::VST_ALPHABLEND, uiRenderGroup);
				}
				else
					Culler.InsertObject(VisibleContext, VSCuller::VST_BASE, uiRenderGroup);
			}
		}	
	}
	
}
bool VSGeometry::IsSwapCull()
{
	VSMeshNode * pMeshNode = GetMeshNode();
	if (pMeshNode)
	{
		VSREAL fDet = pMeshNode->GetWorldTransform().GetRotate().Det();
		return (fDet < 0.0f);
	}
	else
	{
		return false;
	}

	
}
VSGeometry::VSGeometry()
{
	m_pMeshData = NULL;
	m_uiActiveNum = VSMAX_UINT32;
	m_uiCurUseMaterial = 0;
}
VSGeometry::~VSGeometry()
{
	m_pMeshData = NULL;
}
void VSGeometry::SetAffectBoneArray(const VSArray<VSBoneNode *> & pBoneNodeArray)
{
	if(pBoneNodeArray.GetNum())
	{
		m_pBoneNode.Clear();
		m_BoneName.Clear();
		m_pBoneNode = pBoneNodeArray;
		for (unsigned int i = 0 ;i  < m_pBoneNode.GetNum(); i++)
		{
			if(m_pBoneNode[i])
				m_BoneName.AddElement(m_pBoneNode[i]->m_cName);
		}
	}

}
void VSGeometry::SetAffectBoneArray(const VSArray<VSUsedName> & BoneNodeArray)
{
	if(BoneNodeArray.GetNum())
	{
		m_pBoneNode.Clear();
		m_BoneName.Clear();
		m_BoneName = BoneNodeArray;
		//LinkBoneNode();
	}
}
bool VSGeometry::PostLoad(VSStream* pStream)
{
	if (!VSSpatial::PostLoad(pStream))
	{
		return false;
	}
	LinkBoneNode();
	if (m_pMaterialInstance.GetNum() == 0)
	{
		AddMaterialInstance((VSMaterialR *)VSMaterial::GetDefaultResource());
	}
	return 1;
}
bool VSGeometry::PostClone(VSObject * pObjectSrc)
{
	if (!VSSpatial::PostClone(pObjectSrc))
	{
		return false;
	}
	LinkBoneNode();
	if (m_pMaterialInstance.GetNum() == 0)
	{
		AddMaterialInstance((VSMaterialR *)VSMaterial::GetDefaultResource());
	}
// 	VSGeometry * pGeometry = DynamicCast<VSGeometry>(pObjectSrc);
// 	m_MorphData = pGeometry->m_MorphData;
	return true;
}
void VSGeometry::LinkBoneNode()
{
	m_pBoneNode.Clear();
	if (m_BoneName.GetNum())
	{
		VSSkeleton * pSke = GetAffectSkeleton();
		if (pSke)
		{
			m_pBoneNode.Clear();
			for (unsigned int i = 0 ; i < m_BoneName.GetNum() ; i++)
			{
				VSBoneNode * pBoneNode = pSke->GetBoneNode(m_BoneName[i]);
				if (!pBoneNode)
				{
					VSMAC_ASSERT(0);
				}
				m_pBoneNode.AddElement(pBoneNode);
			}
		}
	}
	m_SkinWeightBuffer.SetBufferNum(GetAffectBoneNum() * 3);
}
VSMeshNode * VSGeometry::GetMeshNode()const
{
	VSMeshNode * pMeshNode = NULL;
	VSSpatial* pParent = m_pParent;
	while(!pMeshNode && pParent)
	{
		pMeshNode = DynamicCast<VSMeshNode>(pParent);
		if (!pMeshNode)
		{
			pParent = pParent->GetParent();

		}
	}
	return pMeshNode;

}
VSSkeleton * VSGeometry::GetAffectSkeleton()const
{
	VSSkeletonMeshNode * pSkeMeshNode = NULL;
	VSSpatial* pParent = m_pParent;
	while(!pSkeMeshNode && pParent)
	{
		pSkeMeshNode = DynamicCast<VSSkeletonMeshNode>(pParent);
		if (!pSkeMeshNode)
		{
			pParent = pParent->GetParent();

		}
	}
	if (pSkeMeshNode)
	{
		return pSkeMeshNode->GetSkeleton();
	}
	else
	{
		return NULL;
	}
}
unsigned int VSGeometry::GetVertexNum()const
{
	VSMAC_ASSERT(m_pMeshData);

	VSMAC_ASSERT(m_pMeshData->GetVertexBuffer());

	return m_pMeshData->GetVertexBuffer()->GetVertexNum();

}
void VSGeometry::SetMeshData(VSMeshData * pMeshData)
{
	m_pMeshData = pMeshData;
}
VSMeshData * VSGeometry::GetMeshData()const
{
	return m_pMeshData;
}
VSMeshData * VSGeometry::GetOriginMeshData()const
{
	return m_pMeshData;
}
unsigned int VSGeometry::GetActiveNum()
{
	if (m_pMeshData)
	{
		if (m_pMeshData->GetIndexBuffer() && (m_uiActiveNum == VSMAX_UINT32))
			m_uiActiveNum = m_pMeshData->GetActiveNum();
		else if (m_uiActiveNum == VSMAX_UINT32)
			return 0;

		return m_uiActiveNum;

	}
	return 0;
}
bool VSGeometry::SetActiveNum(unsigned int uiActiveNum)
{
	VSMAC_ASSERT(uiActiveNum <= GetTotalNum());
	m_uiActiveNum = uiActiveNum;
	return 1;

}
unsigned int VSGeometry::GetTotalNum()const
{
	VSMAC_ASSERT(m_pMeshData);

	return m_pMeshData->GetTotalNum();
}
unsigned int VSGeometry::GetMeshDataType()
{
	VSMAC_ASSERT(m_pMeshData);

	return m_pMeshData->GetMeshDataType();


}
unsigned int VSGeometry::AddMaterialInstance(VSMaterialR * pMaterial)
{
	VSMAC_ASSERT(pMaterial);
	VSMaterialInstance * pMaterialInstance = VS_NEW VSMaterialInstance(pMaterial);
	m_pMaterialInstance.AddElement(pMaterialInstance);

	return m_pMaterialInstance.GetNum();
}
unsigned int VSGeometry::AddMaterialInstance(VSMaterialInstance * pMaterial)
{
	VSMAC_ASSERT(pMaterial && pMaterial->GetMaterial());
	VSMaterialInstance * pMaterialInstance = (VSMaterialInstance *)VSObject::CloneCreateObject(pMaterial);
	m_pMaterialInstance.AddElement(pMaterialInstance);

	return m_pMaterialInstance.GetNum();
}
bool VSGeometry::SetMaterialInstance(VSMaterialR * pMaterial, unsigned int uiIndex)
{
	VSMAC_ASSERT(uiIndex < m_pMaterialInstance.GetNum() && pMaterial);

	VSMaterialInstance * pMaterialInstance = VS_NEW VSMaterialInstance(pMaterial);
	m_pMaterialInstance[uiIndex] = pMaterialInstance;
	return true;

}
bool VSGeometry::SetMaterialInstance(VSMaterialInstance * pMaterial, unsigned int uiIndex)
{
	VSMAC_ASSERT(uiIndex < m_pMaterialInstance.GetNum() && pMaterial);

	m_pMaterialInstance[uiIndex] = (VSMaterialInstance *)VSObject::CloneCreateObject(pMaterial);

	return true;
}
bool VSGeometry::SetUseMaterialInstance(unsigned int uiIndex)
{
	VSMAC_ASSERT(uiIndex < m_pMaterialInstance.GetNum());
	m_uiCurUseMaterial = uiIndex;
	return true;
}
void VSGeometry::ClearAllMaterialInstance()
{
	m_pMaterialInstance.Clear();
}
void VSGeometry::DeleteMaterialInstance(unsigned int i)
{
	VSMAC_ASSERT(i < m_pMaterialInstance.GetNum());
	m_pMaterialInstance[i] = NULL;
}
unsigned int VSGeometry::DeleteMaterialInstance(VSMaterialInstance * pMaterial)
{
	VSMAC_ASSERT(pMaterial);
	for (unsigned int i = 0; i < m_pMaterialInstance.GetNum(); i++)
	{
		if (pMaterial == m_pMaterialInstance[i])
		{
			m_pMaterialInstance[i] = NULL;
			return i;
		}
	}
	return VSMAX_UINT32;
}
VSMaterialInstance * VSGeometry::GetUseMaterialInstance()const
{
	VSMaterialInstance * pMaterialInstance = NULL;
	if (m_uiCurUseMaterial < m_pMaterialInstance.GetNum())
	{
		pMaterialInstance = m_pMaterialInstance[m_uiCurUseMaterial];
	}
	return pMaterialInstance;
}
VSMaterialInstance * VSGeometry::GetMaterialInstance(unsigned int i)const
{
	VSMAC_ASSERT(i < m_pMaterialInstance.GetNum());
	return m_pMaterialInstance[i];
}
void VSGeometry::UpdateOther(double dAppTime)
{
	VSMAC_ASSERT(m_pMeshData);

	VSVertexBuffer * pVBuffer = GetMeshData()->GetVertexBuffer();
	if(GetAffectBoneNum() && pVBuffer->HaveBlendWeightInfo() && pVBuffer->HaveBlendIndicesInfo(0))
	{
		VSTransform World  = m_pParent->GetWorldTransform();
		
		for (unsigned int i = 0 ; i < GetAffectBoneNum() ; i++)
		{
			VSBoneNode * pBone = GetAffectBone(i);
			if(pBone)
			{
				VSTransform BoneWorld = pBone->GetWorldTransform();
				VSMatrix3X3W TempBone = pBone->GetBoneOffsetMatrix() * BoneWorld.GetCombine() * World.GetCombineInverse();
				VSVector3W ColumnVector[4];
				//TempBone.Identity();
				TempBone.GetColumnVector(ColumnVector);
				m_SkinWeightBuffer[i * 3] = ColumnVector[0];
				m_SkinWeightBuffer[i * 3 + 1] = ColumnVector[1];
				m_SkinWeightBuffer[i * 3 + 2] = ColumnVector[2];
			}
			else
			{
				m_SkinWeightBuffer[i * 3].Set(1.0f,0.0f,0.0f,0.0f);
				m_SkinWeightBuffer[i * 3 + 1].Set(0.0f,1.0f,0.0f,0.0f);
				m_SkinWeightBuffer[i * 3 + 2].Set(0.0f,0.0f,1.0f,0.0f);
			}

		}

	}
}
void VSGeometry::GetStreamResource(VSArray<VSResourceProxyBase*>& pResourceProxy, StreamInformation_TYPE& StreamInformation) const
{
	for (unsigned int i = 0; i < m_pMaterialInstance.GetNum(); i++)
	{
		if (m_pMaterialInstance[i])
		{
			m_pMaterialInstance[i]->GetStreamResource(pResourceProxy, StreamInformation);
		}
	}
}
IMPLEMENT_RTTI(VSShadowVolumeGeometry,VSGeometry)
BEGIN_ADD_PROPERTY(VSShadowVolumeGeometry,VSGeometry)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSShadowVolumeGeometry)
IMPLEMENT_INITIAL_END
VSShadowVolumeGeometry::VSShadowVolumeGeometry()
{

}
VSShadowVolumeGeometry::~VSShadowVolumeGeometry()
{

}