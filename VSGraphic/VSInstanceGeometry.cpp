#include "VSInstanceGeometry.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
#include "VSRenderThread.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSInstanceGeometry, VSGeometry)
BEGIN_ADD_PROPERTY(VSInstanceGeometry, VSGeometry)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSInstanceGeometry)
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
bool VSInstanceGeometry::InitialDefaultState()
{

	return 1;
}
bool VSInstanceGeometry::TerminalDefaultState()
{

	return 1;
}
VSInstanceGeometry::VSInstanceGeometry()
{
	m_uiMaxInstanceNum = 100;
	m_uiInstanceNum = 0;
	m_pInstanceDataBuffer = NULL;
	m_pInstanceDataRenderBuffer = NULL;
	m_bReCreate = false;

	VSResourceManager::GetDelayUpdateNoTimeObject().AddMethod<VSInstanceGeometry, &VSInstanceGeometry::ClearInstanceData>(this);

}
VSInstanceGeometry::~VSInstanceGeometry()
{
	VSMAC_DELETEA(m_pInstanceDataBuffer);
	VSMAC_DELETEA(m_pInstanceDataRenderBuffer);

	VSResourceManager::GetDelayUpdateNoTimeObject().RemoveMethod<VSInstanceGeometry, &VSInstanceGeometry::ClearInstanceData>(this);

}
void VSInstanceGeometry::CreateInstanceBuffer(VSMeshData * pSourceMeshData, VSMaterialR * pMaterial)
{	
	unsigned int uiOffset = 0;
	VSArray<VSVertexFormat::VERTEXFORMAT_TYPE> FormatArray;
	VSVertexFormat::VERTEXFORMAT_TYPE Transform1;
	Transform1.Stream = 1;
	Transform1.DataType = VSDataBuffer::DT_FLOAT32_4;
	Transform1.Offset = uiOffset;
	Transform1.Semantics = VSVertexFormat::VF_POSITION;
	Transform1.SemanticsIndex = 1;
	Transform1.InstanceDataStepRate = VSVertexFormat::VERTEXFORMAT_TYPE::IDSR_PER_INSTANCE_DATA;
	FormatArray.AddElement(Transform1);
	uiOffset += VSDataBuffer::ms_uiDataTypeByte[VSDataBuffer::DT_FLOAT32_4];

	VSVertexFormat::VERTEXFORMAT_TYPE Transform2;
	Transform2.Stream = 1;
	Transform2.DataType = VSDataBuffer::DT_FLOAT32_4;
	Transform2.Offset = uiOffset;
	Transform2.Semantics = VSVertexFormat::VF_POSITION;
	Transform2.SemanticsIndex = 2;
	Transform2.InstanceDataStepRate = VSVertexFormat::VERTEXFORMAT_TYPE::IDSR_PER_INSTANCE_DATA;
	FormatArray.AddElement(Transform2);
	uiOffset += VSDataBuffer::ms_uiDataTypeByte[VSDataBuffer::DT_FLOAT32_4];

	VSVertexFormat::VERTEXFORMAT_TYPE Transform3;
	Transform3.Stream = 1;
	Transform3.DataType = VSDataBuffer::DT_FLOAT32_4;
	Transform3.Offset = uiOffset;
	Transform3.Semantics = VSVertexFormat::VF_POSITION;
	Transform3.SemanticsIndex = 3;
	Transform3.InstanceDataStepRate = VSVertexFormat::VERTEXFORMAT_TYPE::IDSR_PER_INSTANCE_DATA;
	FormatArray.AddElement(Transform3);
	uiOffset += VSDataBuffer::ms_uiDataTypeByte[VSDataBuffer::DT_FLOAT32_4];

	if (pSourceMeshData->GetVertexBuffer()->HaveBlendIndicesInfo(1))
	{
		for (unsigned int i = 0 ; i < VSResourceManager::GetSimpleInstanceAnimFloat4Num(); i++)
		{
			VSVertexFormat::VERTEXFORMAT_TYPE AninInstanceData;
			AninInstanceData.Stream = 1;
			AninInstanceData.DataType = VSDataBuffer::DT_FLOAT32_4;
			AninInstanceData.Offset = uiOffset;
			AninInstanceData.Semantics = VSVertexFormat::VF_POSITION;
			AninInstanceData.SemanticsIndex = 4 + i;
			AninInstanceData.InstanceDataStepRate = VSVertexFormat::VERTEXFORMAT_TYPE::IDSR_PER_INSTANCE_DATA;
			FormatArray.AddElement(AninInstanceData);
			uiOffset += VSDataBuffer::ms_uiDataTypeByte[VSDataBuffer::DT_FLOAT32_4];
		}
	}
	if (VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_MaterialValueInstance))
	{
		pMaterial->GetResource()->GetInstanceVertexFormat(FormatArray, uiOffset);
	}
	
	SetMeshData(pSourceMeshData);

	AddMaterialInstance(pMaterial);
	
	VSArray<VSVertexFormat::VERTEXFORMAT_TYPE> InstanceFormatArray;
	pSourceMeshData->GetVertexBuffer()->GetVertexFormat(InstanceFormatArray);
	for (unsigned int i = 0; i < FormatArray.GetNum(); i++)
	{
		InstanceFormatArray.AddElement(FormatArray[i]);
	}
	m_pVertexFormat = VSResourceManager::LoadVertexFormat(NULL, &InstanceFormatArray);
	m_uiInstanceSize = 0;
	for (unsigned int i = 0; i < FormatArray.GetNum(); i++)
	{
		m_uiInstanceSize += VSDataBuffer::ms_uiDataTypeByte[FormatArray[i].DataType];
	}
	SetMaxInstanceBuffer(FormatArray,m_uiMaxInstanceNum);
	
}
bool VSInstanceGeometry::HaveSkinInstance()
{
	return m_pMeshData->GetVertexBuffer()->HaveBlendIndicesInfo(1);
}
void VSInstanceGeometry::SetMaxInstanceBuffer(VSArray<VSVertexFormat::VERTEXFORMAT_TYPE> &FormatArray,unsigned int uiMaxNum)
{
	m_uiMaxInstanceNum = uiMaxNum;
	m_PendingDeleteInstanceBuffer = m_InstanceBuffer;
	
	m_InstanceBuffer = VS_NEW VSVertexBuffer(FormatArray, m_uiMaxInstanceNum);
	m_InstanceBuffer->SetStatic(false);
	unsigned char * pInstanceDataBuffer = VS_NEW unsigned char[m_uiMaxInstanceNum * m_uiInstanceSize];
	if (m_pInstanceDataBuffer != NULL)
	{
		VSMemcpy(pInstanceDataBuffer, m_pInstanceDataBuffer, m_uiInstanceNum * m_uiInstanceSize);
		VSMAC_DELETEA(m_pInstanceDataBuffer);
	}	
	m_pInstanceDataBuffer = pInstanceDataBuffer;
	m_bReCreate = true;
}
void VSInstanceGeometry::ClearInstanceData()
{
	if (VSResourceManager::ms_bRenderThread)
	{
		if (m_bReCreate)
		{
			VSMAC_DELETEA(m_pInstanceDataRenderBuffer);
			m_pInstanceDataRenderBuffer = VS_NEW unsigned char[m_uiMaxInstanceNum * m_uiInstanceSize];
			m_bReCreate = false;
		}

		unsigned char * TempBuffer = m_pInstanceDataRenderBuffer;
		m_pInstanceDataRenderBuffer = m_pInstanceDataBuffer;
		m_pInstanceDataBuffer = TempBuffer;
		
		m_PendingDeleteInstanceBuffer = NULL;
	}
	bHasUpdate = false;
	m_uiBoneNum = 0;
	m_AnimInstanceTexture = NULL;
	m_uiInstanceNum = 0;
}
unsigned char * VSInstanceGeometry::GetCurDataBufferAddr(unsigned int i)
{
	return m_pInstanceDataBuffer + i * m_uiInstanceSize;
}
unsigned int VSInstanceGeometry::AddInstance(const VSAABB3 WorldBound, const VSTransform & T,VSMaterialInstance * pMaterialInstance, VSArray<ANIM_INSTANCE_SHADER_DATA> * pAnimData)
{
	if (m_uiInstanceNum > m_uiMaxInstanceNum - 1)
	{
		if (!m_InstanceBuffer)
		{
			VSMAC_ASSERT(0);
			return 0;
		}
		else
		{
			VSArray<VSVertexFormat::VERTEXFORMAT_TYPE> FormatArray;
			m_InstanceBuffer->GetVertexFormat(FormatArray);
			SetMaxInstanceBuffer(FormatArray, m_uiMaxInstanceNum + 100);
		}
		
	}
	VSVector3W * pData = (VSVector3W *)GetCurDataBufferAddr(m_uiInstanceNum);
	VSVector3W ColumnVector[4];
	VSTransform TTemp = T;
	TTemp.GetCombine().GetColumnVector(ColumnVector);
	*pData = ColumnVector[0];
	pData++;
	*pData = ColumnVector[1];
	pData++;
	*pData = ColumnVector[2];
	pData++;

	if (HaveSkinInstance())
	{
		ANIM_INSTANCE_SHADER_DATA * SimpleInstanceDataTarget = (ANIM_INSTANCE_SHADER_DATA *)pData;
		VSREAL * uiAnimNumPtr = (VSREAL *)(pData + VSResourceManager::GetSimpleInstanceAnimFloat4Num());
		uiAnimNumPtr--;

		if (pAnimData && pAnimData->GetNum())
		{	
			unsigned int i = 0;
			*uiAnimNumPtr = 0.0f;
			while (true)
			{			
				if ( i < pAnimData->GetNum() && i < MAX_SIMPLE_INSTANCE_ANIM)
				{
					SimpleInstanceDataTarget[i] = (*pAnimData)[i];
					*uiAnimNumPtr += 1.0f;
				}
				else if (i < MAX_SIMPLE_INSTANCE_ANIM)
				{
					SimpleInstanceDataTarget[i] = ANIM_INSTANCE_SHADER_DATA();
				}
				else
				{
					break;
				}
				i++;
			}
			
		}
		else
		{
			SimpleInstanceDataTarget[0].fAlpha = 1.0f;
			SimpleInstanceDataTarget[0].AnimIndex = 0.0f;
			for (unsigned int i = 1; i < MAX_SIMPLE_INSTANCE_ANIM; i++)
			{
				SimpleInstanceDataTarget[i] = ANIM_INSTANCE_SHADER_DATA();
			}
			*uiAnimNumPtr = 1.0f;
		}
		pData += VSResourceManager::GetSimpleInstanceAnimFloat4Num();
	}


	if (VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_MaterialValueInstance))
	{
		unsigned int uiGetsize = 0;
		pMaterialInstance->GetInstanceData((unsigned char *)pData, uiGetsize);
#ifdef _DEBUG
		uiGetsize += sizeof(VSVector3W) * 3;
		if (HaveSkinInstance())
		{
			uiGetsize += sizeof(VSVector3W) * VSResourceManager::GetSimpleInstanceAnimFloat4Num();
		}
		VSMAC_ASSERT(uiGetsize == m_uiInstanceSize);
#endif
		
	}
	if (m_uiInstanceNum == 0)
	{
		m_LocalBV = WorldBound;
	}
	else
	{
		m_LocalBV = m_LocalBV.MergeAABB(WorldBound);
	}
	m_uiInstanceNum++;
	return m_uiInstanceNum - 1;
}
unsigned int VSInstanceGeometry::GetInstanceNum()
{
	return m_uiInstanceNum;
}
void VSInstanceGeometry::UpdateGeometryBeforeDrawCall()
{
	if (bHasUpdate)
		return;
	bHasUpdate = true;
	ENQUEUE_UNIQUE_RENDER_COMMAND_FOURPARAMETER(VSInstanceGeometry_UpdateGeometryBeforeDrawCallCommand,
	VSVertexBuffer *, m_InstanceBuffer, m_InstanceBuffer, 
	unsigned char * , m_pInstanceDataBuffer, m_pInstanceDataBuffer,
	unsigned int, m_uiInstanceNum, m_uiInstanceNum,
	unsigned int , m_uiInstanceSize, m_uiInstanceSize,
	{
		unsigned char * pData = (unsigned char *)m_InstanceBuffer->Lock();
		VSMAC_ASSERT(pData);
		VSMemcpy(pData, m_pInstanceDataBuffer, m_uiInstanceNum * m_uiInstanceSize);
		m_InstanceBuffer->UnLock();
	})
}
void VSInstanceGeometry::UpdateWorldInstanceBound()
{
	m_WorldBV = m_LocalBV;
}