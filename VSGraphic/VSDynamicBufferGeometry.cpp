#include "VSDynamicBufferGeometry.h"
#include "VSPointSet.h"
#include "VSLineSet.h"
#include "VSTriangleSet.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSDynamicBufferGeometry,VSGeometry)
VSDynamicBufferGeometry::VSDynamicBufferGeometry()
{

}
VSDynamicBufferGeometry::~VSDynamicBufferGeometry()
{

}
void VSDynamicBufferGeometry::Draw(VSCamera * pCamera)
{
	if (HaveData())
	{
		VSMaterialInstance *pMaterialInstance = GetUseMaterialInstance();
		VSMaterial *pMaterial = pMaterialInstance->GetMaterial();
		if(!pMaterialInstance || !pMaterial)
			return;
		for (unsigned int i = 0 ; i < pMaterial->GetShaderMainFunctionNum() ;i++)
		{

			VSIndirectRenderPass * pBasePass = pMaterialInstance->GetMaterial()->GetIndirectRenderPass();
			pBasePass->SetDrawContent(pMaterialInstance, this, pCamera, i);
			pBasePass->Draw(VSRenderer::ms_pRenderer);
		}
	}
}
IMPLEMENT_RTTI(VSDVGeometry,VSDynamicBufferGeometry)
VSDVGeometry::VSDVGeometry()
{
	m_pVertexUseBuffer = NULL;
	m_uiCurVUseBufferElementIndex = 0;
	m_pVertexUseBufferRender = NULL;
	VSResourceManager::GetDelayUpdateNoTimeObject().AddMethod<VSDVGeometry, &VSDVGeometry::ClearInfo>(this);

}
VSDVGeometry::~VSDVGeometry()
{
	m_pVertexUseBuffer = NULL;
	m_pVertexUseBufferRender = NULL;
	VSResourceManager::GetDelayUpdateNoTimeObject().RemoveMethod<VSDVGeometry, &VSDVGeometry::ClearInfo>(this);

}
void VSDVGeometry::Create(VSArray<VSVertexFormat::VERTEXFORMAT_TYPE> &FormatArray,
						   unsigned int uiMeshDataType,
						   unsigned int uiVertexNum)
{
	VSMAC_ASSERT(uiMeshDataType < VSMeshData::MDT_MAX && FormatArray.GetNum() && uiVertexNum);

	VSMeshData * pMeshData = NULL;
	if (uiMeshDataType == VSMeshData::MDT_POINT)
	{
		pMeshData = VS_NEW VSPointSet();
	}
	else if ( uiMeshDataType == VSMeshData::MDT_LINE)
	{
		pMeshData = VS_NEW VSLineSet();
	}
	else if ( uiMeshDataType == VSMeshData::MDT_TRIANGLE)
	{
		pMeshData = VS_NEW VSTriangleSet();
	}

	VSMAC_ASSERT(pMeshData);


	VSVertexBuffer * pVertexBuffer = NULL;
	pVertexBuffer = VS_NEW VSVertexBuffer(FormatArray,uiVertexNum);
	VSMAC_ASSERT(pVertexBuffer);

	pVertexBuffer->SetStatic(false);
	pVertexBuffer->SetMemType(VSMemBind::MT_VRAM);
	pVertexBuffer->SetLockFlag(VSInheritBind::LF_DISCARD);
	pMeshData->SetVertexBuffer(pVertexBuffer);

	SetMeshData(pMeshData);
	m_pVertexUseBuffer = NULL;
	m_uiCurVUseBufferElementIndex = 0;

	m_pVertexUseBuffer = VS_NEW VSUseBuffer(false);
	m_pVertexUseBuffer->SetOneAddLimitSize(pVertexBuffer->GetOneVertexSize() * pVertexBuffer->GetVertexNum());
	if (VSResourceManager::ms_bRenderThread)
	{
		m_pVertexUseBufferRender = VS_NEW VSUseBuffer(false);
		m_pVertexUseBufferRender->SetOneAddLimitSize(pVertexBuffer->GetOneVertexSize() * pVertexBuffer->GetVertexNum());
	}
	else
	{
		m_pVertexUseBufferRender = m_pVertexUseBuffer;
	}
}


bool VSDVGeometry::Add(const void * pVeretexData,unsigned int uiVertexSize)
{
	if (!pVeretexData || !uiVertexSize)
	{
		return 0;
	}

	
	if(m_pVertexUseBuffer->Add(pVeretexData,uiVertexSize) == false)
		return false;
	return 1;
}
void * VSDVGeometry::NewGetV(unsigned int uiVertexSize)
{
	if (!uiVertexSize)
	{
		return 0;
	}

	return m_pVertexUseBuffer->NewGet(uiVertexSize);
}
unsigned int VSDVGeometry::UpdateGeometry()
{

	VSVertexBuffer * pVertexBuffer = m_pMeshData->GetVertexBuffer();
	if (!pVertexBuffer || !m_pVertexUseBufferRender->GetElementNum())
	{
		return UGRI_FAIL;
	}
	unsigned int uiActiveNum = 0;
	unsigned int uiVElementEndIndex = m_uiCurVUseBufferElementIndex;
	unsigned int uiVSize = 0;
	for (unsigned int i = m_uiCurVUseBufferElementIndex ; i < m_pVertexUseBufferRender->GetElementNum() ; i++)
	{
		const VSUseBuffer::VSBufferElementInfo * pVElement = m_pVertexUseBufferRender->GetElementInfo(i);
		if (pVElement)
		{

			if (uiVSize + pVElement->GetSize() > pVertexBuffer->GetByteSize())
			{
				break;
			}
			else
			{
				uiVElementEndIndex = i;
				uiVSize += pVElement->GetSize();
			}
		}
		else
		{
			return UGRI_FAIL;
		}
	}

	unsigned char * pVertexData = (unsigned char *)pVertexBuffer->Lock();

	if (!pVertexData)
	{
		return UGRI_FAIL;
	}

	VSMemcpy(pVertexData,m_pVertexUseBufferRender->GetBuffer() + m_pVertexUseBufferRender->GetElementInfo(m_uiCurVUseBufferElementIndex)->m_uiStart,uiVSize);
	pVertexBuffer->UnLock();
	if (!uiActiveNum)
	{
		uiActiveNum = m_pMeshData->GetGirdNum(uiVSize / pVertexBuffer->GetOneVertexSize());
	}
	
	SetActiveNum(uiActiveNum);

	m_uiCurVUseBufferElementIndex = uiVElementEndIndex + 1;
	
	if (m_uiCurVUseBufferElementIndex >= m_pVertexUseBufferRender->GetElementNum())
	{
		m_uiCurVUseBufferElementIndex = 0;
		return UGRI_END;
	}
	else
	{
		return UGRI_CONTINUME;
	}
	return UGRI_END;
}
void VSDVGeometry::ClearInfo()
{
	if (VSResourceManager::ms_bRenderThread)
	{
		Swap(m_pVertexUseBuffer,m_pVertexUseBufferRender);
	}
	m_pVertexUseBuffer->Clear();
	m_uiCurVUseBufferElementIndex = 0;
}
unsigned int VSDVGeometry::GetMaxRenderVertexNum()const
{
	return m_pMeshData->GetVertexBuffer()->GetVertexNum();
}

IMPLEMENT_RTTI(VSDVDIGeometry,VSDynamicBufferGeometry)
VSDVDIGeometry::VSDVDIGeometry()
{
	m_pVertexUseBuffer = NULL;
	m_pIndexUseBuffer = NULL;
	m_uiCurVUseBufferElementIndex = 0;
	m_uiCurIUseBufferElementIndex = 0;

	m_pVertexUseBufferRender = NULL;
	m_pIndexUseBufferRender = NULL;
	if (VSResourceManager::ms_bRenderThread)
	{
		VSResourceManager::GetDelayUpdateNoTimeObject().AddMethod<VSDVDIGeometry, &VSDVDIGeometry::ClearInfo>(this);
	}
}
VSDVDIGeometry::~VSDVDIGeometry()
{
	m_pVertexUseBuffer = NULL;
	m_pIndexUseBuffer = NULL;

	m_pVertexUseBufferRender = NULL;
	m_pIndexUseBufferRender = NULL;
	if (VSResourceManager::ms_bRenderThread)
	{
		VSResourceManager::GetDelayUpdateNoTimeObject().RemoveMethod<VSDVDIGeometry, &VSDVDIGeometry::ClearInfo>(this);
	}
}
void VSDVDIGeometry::Create(VSArray<VSVertexFormat::VERTEXFORMAT_TYPE> &FormatArray,
												 unsigned int uiMeshDataType,
												 unsigned int uiVertexNum,
												 unsigned int uiIndexNum)
{
	VSMAC_ASSERT(uiMeshDataType < VSMeshData::MDT_MAX && FormatArray.GetNum() && uiVertexNum && uiIndexNum);

	VSMeshData * pMeshData = NULL;
	if (uiMeshDataType == VSMeshData::MDT_POINT)
	{
		pMeshData = VS_NEW VSPointSet();
	}
	else if ( uiMeshDataType == VSMeshData::MDT_LINE)
	{
		pMeshData = VS_NEW VSLineSet();
	}
	else if ( uiMeshDataType == VSMeshData::MDT_TRIANGLE)
	{
		pMeshData = VS_NEW VSTriangleSet();
	}

	VSMAC_ASSERT(pMeshData);


	VSVertexBuffer * pVertexBuffer = NULL;
	pVertexBuffer = VS_NEW VSVertexBuffer(FormatArray,uiVertexNum);
	VSMAC_ASSERT(pVertexBuffer);

	pVertexBuffer->SetStatic(false);
	pVertexBuffer->SetMemType(VSMemBind::MT_VRAM);
	pVertexBuffer->SetLockFlag(VSInheritBind::LF_DISCARD);
	pMeshData->SetVertexBuffer(pVertexBuffer);

	VSIndexBuffer * pIndexBuffer = VS_NEW VSIndexBuffer(uiIndexNum);
	pIndexBuffer->SetStatic(false);
	pIndexBuffer->SetMemType(VSMemBind::MT_VRAM);
	pIndexBuffer->SetLockFlag(VSInheritBind::LF_DISCARD);
	pMeshData->SetIndexBuffer(pIndexBuffer);

	SetMeshData(pMeshData);
	m_pVertexUseBuffer = NULL;
	m_pIndexUseBuffer = NULL;
	m_uiCurVUseBufferElementIndex = 0;
	m_uiCurIUseBufferElementIndex = 0;


	m_pVertexUseBuffer = VS_NEW VSUseBuffer(false);
	m_pVertexUseBuffer->SetOneAddLimitSize(pVertexBuffer->GetOneVertexSize() * pVertexBuffer->GetVertexNum());

	m_pIndexUseBuffer = VS_NEW VSUseBuffer(false);
	m_pIndexUseBuffer->SetOneAddLimitSize(pIndexBuffer->GetByteSize());

	if (VSResourceManager::ms_bRenderThread)
	{
		m_pVertexUseBufferRender = VS_NEW VSUseBuffer(false);
		m_pVertexUseBufferRender->SetOneAddLimitSize(pVertexBuffer->GetOneVertexSize() * pVertexBuffer->GetVertexNum());

		m_pIndexUseBufferRender = VS_NEW VSUseBuffer(false);
		m_pIndexUseBufferRender->SetOneAddLimitSize(pIndexBuffer->GetByteSize());
	}
	else
	{
		m_pVertexUseBufferRender = m_pVertexUseBuffer;
		m_pIndexUseBufferRender = m_pIndexUseBuffer;
	}
}

void * VSDVDIGeometry::NewGetV(unsigned int uiVertexSize)
{
	if (!uiVertexSize)
	{
		return 0;
	}

	return m_pVertexUseBuffer->NewGet(uiVertexSize);
}
void * VSDVDIGeometry::NewGetI(unsigned int uiIndexSize)
{
	if (!uiIndexSize)
	{
		return 0;
	}

	return m_pIndexUseBuffer->NewGet(uiIndexSize);
}
bool VSDVDIGeometry::Add(const void * pVeretexData,unsigned int uiVertexSize,
								  const void * pIndexData,unsigned int uiIndexSize)
{
	VSMAC_ASSERT(pVeretexData && uiVertexSize && pIndexData && uiIndexSize);
	if (!pVeretexData || !uiVertexSize || !pIndexData || !uiIndexSize)
	{
		return 0;
	}

	if(m_pVertexUseBuffer->Add(pVeretexData,uiVertexSize) == false)
		return false;

	
		

	if(m_pIndexUseBuffer->Add(pIndexData,uiIndexSize) == false)
		return false;

	unsigned int uiLastVertexNum = 0;
	unsigned int uiVElementNum = m_pVertexUseBuffer->GetElementNum();
	if (uiVElementNum)
	{
		VSVertexBuffer * pVertexBuffer = m_pMeshData->GetVertexBuffer();
		if (pVertexBuffer)
		{
			return false;
		}
		const VSUseBuffer::VSBufferElementInfo * pElementInfo = m_pVertexUseBuffer->GetElementInfo(uiVElementNum - 1);
		uiLastVertexNum = pElementInfo->m_uiStart / pVertexBuffer->GetOneVertexSize();
	}
	unsigned char *pBuffer = m_pIndexUseBuffer->GetBuffer();
	unsigned int uiIElementNum = m_pIndexUseBuffer->GetElementNum();
	pBuffer += m_pIndexUseBuffer->GetElementInfo(uiIElementNum - 1)->m_uiStart;

	VSUSHORT_INDEX * pIndexDataBuffer = (VSUSHORT_INDEX *)pBuffer;
	for ( ; (USIZE_TYPE)pIndexDataBuffer < (USIZE_TYPE)m_pIndexUseBuffer->GetElementInfo(uiIElementNum - 1)->m_uiEnd ; pIndexDataBuffer++)
	{
		*pIndexDataBuffer += uiLastVertexNum;
	}
	if ((USIZE_TYPE)pIndexDataBuffer != (USIZE_TYPE)m_pIndexUseBuffer->GetElementInfo(uiIElementNum - 1)->m_uiEnd)
	{
		return false;
	}
	
	VSMAC_ASSERT(m_pVertexUseBuffer->GetElementNum() == m_pIndexUseBuffer->GetElementNum());
	return 1;
}
unsigned int VSDVDIGeometry::UpdateGeometry()
{
	VSMAC_ASSERT(m_pMeshData && m_pVertexUseBufferRender && m_pIndexUseBufferRender);
	if (!m_pMeshData || !m_pVertexUseBufferRender || !m_pIndexUseBufferRender)
	{
		return UGRI_FAIL;
	}


	VSVertexBuffer * pVertexBuffer = m_pMeshData->GetVertexBuffer();
	VSIndexBuffer * pIndexBuffer = m_pMeshData->GetIndexBuffer();
	if (!pVertexBuffer || !pIndexBuffer || !m_pVertexUseBufferRender->GetElementNum() || !m_pIndexUseBufferRender->GetElementNum())
	{
		return UGRI_FAIL;
	}
	unsigned int uiActiveNum = 0;
	unsigned int uiVElementEndIndex = m_uiCurVUseBufferElementIndex;
	unsigned int uiVSize = 0;
	for (unsigned int i = m_uiCurVUseBufferElementIndex ; i < m_pVertexUseBufferRender->GetElementNum() ; i++)
	{
		const VSUseBuffer::VSBufferElementInfo * pVElement = m_pVertexUseBufferRender->GetElementInfo(i);
		if (pVElement)
		{

			if (uiVSize + pVElement->GetSize() > pVertexBuffer->GetByteSize())
			{
				break;
			}
			else
			{
				uiVElementEndIndex = i;
				uiVSize += pVElement->GetSize();
			}
		}
		else
		{
			return UGRI_FAIL;
		}
	}




	unsigned int uiIElementEndIndex = m_uiCurIUseBufferElementIndex;
	unsigned int uiISize = 0;
	for (unsigned int i = m_uiCurIUseBufferElementIndex ; i < m_pIndexUseBufferRender->GetElementNum() ; i++)
	{
		const VSUseBuffer::VSBufferElementInfo * pIElement = m_pIndexUseBufferRender->GetElementInfo(i);
		if (pIElement)
		{

			if (uiISize + pIElement->GetSize() > pIndexBuffer->GetByteSize())
			{
				break;
			}
			else
			{
				uiIElementEndIndex = i;
				uiISize += pIElement->GetSize();
			}
		}
		else
		{
			return UGRI_FAIL;
		}
	}

	unsigned int uiVCount = uiVElementEndIndex - m_uiCurVUseBufferElementIndex;
	unsigned int uiICount = uiIElementEndIndex - m_uiCurIUseBufferElementIndex;
	if ( uiVCount > uiICount)
	{
		uiVSize = 0;
		uiVElementEndIndex = m_uiCurVUseBufferElementIndex + uiICount;
		for (unsigned int i = m_uiCurVUseBufferElementIndex ; i <= uiVElementEndIndex ; i++)
		{
			const VSUseBuffer::VSBufferElementInfo * pVElement = m_pVertexUseBufferRender->GetElementInfo(i);
			if (pVElement)
			{
				uiVSize += pVElement->GetSize();
			}
			else
			{
				return UGRI_FAIL;
			}
		}
	}
	else if(uiVCount < uiICount)
	{
		uiISize = 0;
		uiIElementEndIndex = m_uiCurIUseBufferElementIndex + uiVCount;
		for (unsigned int i = m_uiCurIUseBufferElementIndex ; i <= uiIElementEndIndex ; i++)
		{
			const VSUseBuffer::VSBufferElementInfo * pVElement = m_pVertexUseBufferRender->GetElementInfo(i);
			if (pVElement)
			{
				uiISize += pVElement->GetSize();
			}
			else
			{
				return UGRI_FAIL;
			}
		}
	}

	unsigned char * pIndexData = (unsigned char *)pIndexBuffer->Lock();
	if (!pIndexData)
	{
		return UGRI_FAIL;
	}
	VSMemcpy(pIndexData,m_pIndexUseBufferRender->GetBuffer() + m_pIndexUseBufferRender->GetElementInfo(m_uiCurIUseBufferElementIndex)->m_uiStart,uiISize);
	pIndexBuffer->UnLock();
	uiActiveNum = m_pMeshData->GetGirdNum(uiISize / sizeof(VSUSHORT_INDEX));

	m_uiCurIUseBufferElementIndex = uiIElementEndIndex + 1;

	
	unsigned char * pVertexData = (unsigned char *)pVertexBuffer->Lock();

	if (!pVertexData)
	{
		return UGRI_FAIL;
	}

	VSMemcpy(pVertexData,m_pVertexUseBufferRender->GetBuffer() + m_pVertexUseBufferRender->GetElementInfo(m_uiCurVUseBufferElementIndex)->m_uiStart,uiVSize);
	pVertexBuffer->UnLock();
	if (!uiActiveNum)
	{
		uiActiveNum = m_pMeshData->GetGirdNum(uiVSize / pVertexBuffer->GetOneVertexSize());
	}

	SetActiveNum(uiActiveNum);

	m_uiCurVUseBufferElementIndex = uiVElementEndIndex + 1;

	//因为UseVBuffer 和 UseIBuffer Element个数一样,添加渲染的个数也保证一样，所以这里只判断一个。
	if (m_uiCurVUseBufferElementIndex >= m_pVertexUseBufferRender->GetElementNum())
	{
		m_uiCurVUseBufferElementIndex = 0;
		m_uiCurIUseBufferElementIndex = 0;
		return UGRI_END;
	}
	else
	{
		return UGRI_CONTINUME;
	}
	return UGRI_END;
}
void VSDVDIGeometry::ClearInfo()
{
	if (VSResourceManager::ms_bRenderThread)
	{
		Swap(m_pVertexUseBuffer,m_pVertexUseBufferRender);
		Swap(m_pIndexUseBuffer,m_pIndexUseBufferRender);
	}
	m_pIndexUseBuffer->Clear();
	m_pVertexUseBuffer->Clear();
	m_uiCurVUseBufferElementIndex = 0;
	m_uiCurIUseBufferElementIndex = 0;
}


IMPLEMENT_RTTI(VSSVDIGeometry,VSDynamicBufferGeometry)
VSSVDIGeometry::VSSVDIGeometry()
{

	m_pIndexUseBuffer = NULL;

	m_uiCurIUseBufferElementIndex = 0;

	m_pIndexUseBufferRender = NULL;
	if (VSResourceManager::ms_bRenderThread)
	{
		VSResourceManager::GetDelayUpdateNoTimeObject().AddMethod<VSSVDIGeometry, &VSSVDIGeometry::ClearInfo>(this);
	}
}
VSSVDIGeometry::~VSSVDIGeometry()
{

	m_pIndexUseBuffer = NULL;

	m_pIndexUseBufferRender = NULL;
	if (VSResourceManager::ms_bRenderThread)
	{
		VSResourceManager::GetDelayUpdateNoTimeObject().RemoveMethod<VSSVDIGeometry, &VSSVDIGeometry::ClearInfo>(this);
	}
}
void VSSVDIGeometry::Create(unsigned int uiMeshDataType,VSVertexBuffer * pVertexBuffer,unsigned int uiIndexNum)
{
	VSMAC_ASSERT(uiMeshDataType < VSMeshData::MDT_MAX && uiIndexNum && pVertexBuffer && pVertexBuffer->IsStatic());
	VSMeshData * pMeshData = NULL;
	if (uiMeshDataType == VSMeshData::MDT_POINT)
	{
		pMeshData = VS_NEW VSPointSet();
	}
	else if ( uiMeshDataType == VSMeshData::MDT_LINE)
	{
		pMeshData = VS_NEW VSLineSet();
	}
	else if ( uiMeshDataType == VSMeshData::MDT_TRIANGLE)
	{
		pMeshData = VS_NEW VSTriangleSet();
	}

	VSMAC_ASSERT(pMeshData);

	pMeshData->SetVertexBuffer(pVertexBuffer);

	VSIndexBuffer * pIndexBuffer = VS_NEW VSIndexBuffer(uiIndexNum);
	pIndexBuffer->SetStatic(false);
	pIndexBuffer->SetMemType(VSMemBind::MT_VRAM);
	pIndexBuffer->SetLockFlag(VSInheritBind::LF_DISCARD);
	pMeshData->SetIndexBuffer(pIndexBuffer);

	SetMeshData(pMeshData);

	m_pIndexUseBuffer = NULL;

	m_uiCurIUseBufferElementIndex = 0;


	m_pIndexUseBuffer = VS_NEW VSUseBuffer(false);
	m_pIndexUseBuffer->SetOneAddLimitSize(pIndexBuffer->GetByteSize());

	if (VSResourceManager::ms_bRenderThread)
	{

		m_pIndexUseBufferRender = VS_NEW VSUseBuffer(false);
		m_pIndexUseBufferRender->SetOneAddLimitSize(pIndexBuffer->GetByteSize());
	}
	else
	{
		m_pIndexUseBufferRender = m_pIndexUseBuffer;
	}
}

void * VSSVDIGeometry::NewGetI(unsigned int uiIndexSize)
{
	if (!uiIndexSize)
	{
		return 0;
	}

	return m_pIndexUseBuffer->NewGet(uiIndexSize);
}
bool VSSVDIGeometry::Add(const void * pIndexData,unsigned int uiIndexSize)
{
	VSMAC_ASSERT(pIndexData && uiIndexSize);
	if (!pIndexData || !uiIndexSize)
	{
		return 0;
	}



	if(m_pIndexUseBuffer->Add(pIndexData,uiIndexSize) == false)
		return false;

	return 1;
}
unsigned int VSSVDIGeometry::UpdateGeometry()
{
	VSMAC_ASSERT(m_pMeshData && m_pIndexUseBufferRender);
	if (!m_pMeshData || !m_pIndexUseBufferRender)
	{
		return UGRI_FAIL;
	}


	VSIndexBuffer * pIndexBuffer = m_pMeshData->GetIndexBuffer();
	if (!pIndexBuffer || !m_pIndexUseBufferRender->GetElementNum())
	{
		return UGRI_FAIL;
	}
	unsigned int uiActiveNum = 0;
	




	unsigned int uiIElementEndIndex = m_uiCurIUseBufferElementIndex;
	unsigned int uiISize = 0;
	for (unsigned int i = m_uiCurIUseBufferElementIndex ; i < m_pIndexUseBufferRender->GetElementNum() ; i++)
	{
		const VSUseBuffer::VSBufferElementInfo * pIElement = m_pIndexUseBufferRender->GetElementInfo(i);
		if (pIElement)
		{

			if (uiISize + pIElement->GetSize() > pIndexBuffer->GetByteSize())
			{
				break;
			}
			else
			{
				uiIElementEndIndex = i;
				uiISize += pIElement->GetSize();
			}
		}
		else
		{
			return UGRI_FAIL;
		}
	}


	unsigned char * pIndexData = (unsigned char *)pIndexBuffer->Lock();
	if (!pIndexData)
	{
		return UGRI_FAIL;
	}
	VSMemcpy(pIndexData,m_pIndexUseBufferRender->GetBuffer() + m_pIndexUseBufferRender->GetElementInfo(m_uiCurIUseBufferElementIndex)->m_uiStart,uiISize);
	pIndexBuffer->UnLock();
	uiActiveNum = m_pMeshData->GetGirdNum(uiISize / sizeof(VSUSHORT_INDEX));

	m_uiCurIUseBufferElementIndex = uiIElementEndIndex + 1;

	SetActiveNum(uiActiveNum);


	//因为UseVBuffer 和 UseIBuffer Element个数一样每天添加渲染的个数也保证一样，所以这里只判断一个。
	if (m_uiCurIUseBufferElementIndex >= m_pIndexUseBufferRender->GetElementNum())
	{
		m_uiCurIUseBufferElementIndex = 0;
		return UGRI_END;
	}
	else
	{
		return UGRI_CONTINUME;
	}
	return UGRI_END;
}
void VSSVDIGeometry::ClearInfo()
{
	if (VSResourceManager::ms_bRenderThread)
	{

		Swap(m_pIndexUseBuffer,m_pIndexUseBufferRender);
	}
	m_pIndexUseBuffer->Clear();

	m_uiCurIUseBufferElementIndex = 0;
}


IMPLEMENT_RTTI(VSDVSIGeometry,VSDynamicBufferGeometry)
VSDVSIGeometry::VSDVSIGeometry()
{
	m_pVertexUseBuffer = NULL;
	m_uiCurVUseBufferElementIndex = 0;
	m_pVertexUseBufferRender = NULL;
	if (VSResourceManager::ms_bRenderThread)
	{
		VSResourceManager::GetDelayUpdateNoTimeObject().AddMethod<VSDVSIGeometry, &VSDVSIGeometry::ClearInfo>(this);
	}
}
VSDVSIGeometry::~VSDVSIGeometry()
{
	m_pVertexUseBuffer = NULL;
	m_pVertexUseBufferRender = NULL;
	if (VSResourceManager::ms_bRenderThread)
	{
		VSResourceManager::GetDelayUpdateNoTimeObject().RemoveMethod<VSDVSIGeometry, &VSDVSIGeometry::ClearInfo>(this);
	}
}
void VSDVSIGeometry::Create(VSArray<VSVertexFormat::VERTEXFORMAT_TYPE> &FormatArray,
							unsigned int uiMeshDataType,
							unsigned int uiVertexNum,
							VSIndexBuffer * pIndexBuffer)
{
	VSMAC_ASSERT(uiMeshDataType < VSMeshData::MDT_MAX && FormatArray.GetNum() && uiVertexNum && pIndexBuffer);
	VSMAC_ASSERT(pIndexBuffer->IsStatic());
	VSMeshData * pMeshData = NULL;
	if (uiMeshDataType == VSMeshData::MDT_POINT)
	{
		pMeshData = VS_NEW VSPointSet();
	}
	else if ( uiMeshDataType == VSMeshData::MDT_LINE)
	{
		pMeshData = VS_NEW VSLineSet();
	}
	else if ( uiMeshDataType == VSMeshData::MDT_TRIANGLE)
	{
		pMeshData = VS_NEW VSTriangleSet();
	}

	VSMAC_ASSERT(pMeshData);


	VSVertexBuffer * pVertexBuffer = NULL;
	pVertexBuffer = VS_NEW VSVertexBuffer(FormatArray,uiVertexNum);
	VSMAC_ASSERT(pVertexBuffer);

	pVertexBuffer->SetStatic(false);
	pVertexBuffer->SetMemType(VSMemBind::MT_VRAM);
	pVertexBuffer->SetLockFlag(VSInheritBind::LF_DISCARD);
	pMeshData->SetVertexBuffer(pVertexBuffer);
	pMeshData->SetIndexBuffer(pIndexBuffer);
	SetMeshData(pMeshData);
	m_pVertexUseBuffer = NULL;
	m_uiCurVUseBufferElementIndex = 0;

	m_pVertexUseBuffer = VS_NEW VSUseBuffer(false);
	m_pVertexUseBuffer->SetOneAddLimitSize(pVertexBuffer->GetOneVertexSize() * pVertexBuffer->GetVertexNum());
	if (VSResourceManager::ms_bRenderThread)
	{
		m_pVertexUseBufferRender = VS_NEW VSUseBuffer(false);
		m_pVertexUseBufferRender->SetOneAddLimitSize(pVertexBuffer->GetOneVertexSize() * pVertexBuffer->GetVertexNum());
	}
	else
	{
		m_pVertexUseBufferRender = m_pVertexUseBuffer;
	}
}
bool VSDVSIGeometry::Add(const void * pVeretexData,unsigned int uiVertexSize)
{
	if (!pVeretexData || !uiVertexSize)
	{
		return 0;
	}


	if(m_pVertexUseBuffer->Add(pVeretexData,uiVertexSize) == false)
		return false;
	return 1;
}
void * VSDVSIGeometry::NewGetV(unsigned int uiVertexSize)
{
	if (!uiVertexSize)
	{
		return 0;
	}

	return m_pVertexUseBuffer->NewGet(uiVertexSize);
}
unsigned int VSDVSIGeometry::UpdateGeometry()
{

	VSVertexBuffer * pVertexBuffer = m_pMeshData->GetVertexBuffer();
	if (!pVertexBuffer || !m_pVertexUseBufferRender->GetElementNum())
	{
		return UGRI_FAIL;
	}
	unsigned int uiVElementEndIndex = m_uiCurVUseBufferElementIndex;
	unsigned int uiVSize = 0;
	for (unsigned int i = m_uiCurVUseBufferElementIndex ; i < m_pVertexUseBufferRender->GetElementNum() ; i++)
	{
		const VSUseBuffer::VSBufferElementInfo * pVElement = m_pVertexUseBufferRender->GetElementInfo(i);
		if (pVElement)
		{

			if (uiVSize + pVElement->GetSize() > pVertexBuffer->GetByteSize())
			{
				break;
			}
			else
			{
				uiVElementEndIndex = i;
				uiVSize += pVElement->GetSize();
			}
		}
		else
		{
			return UGRI_FAIL;
		}
	}

	unsigned char * pVertexData = (unsigned char *)pVertexBuffer->Lock();

	if (!pVertexData)
	{
		return UGRI_FAIL;
	}

	VSMemcpy(pVertexData,m_pVertexUseBufferRender->GetBuffer() + m_pVertexUseBufferRender->GetElementInfo(m_uiCurVUseBufferElementIndex)->m_uiStart,uiVSize);
	pVertexBuffer->UnLock();
	m_uiCurVUseBufferElementIndex = uiVElementEndIndex + 1;

	//因为UseVBuffer 和 UseIBuffer Element个数一样每天添加渲染的个数也保证一样，所以这里只判断一个。
	if (m_uiCurVUseBufferElementIndex >= m_pVertexUseBufferRender->GetElementNum())
	{
		m_uiCurVUseBufferElementIndex = 0;
		return UGRI_END;
	}
	else
	{
		return UGRI_CONTINUME;
	}
	return UGRI_END;
}
void VSDVSIGeometry::ClearInfo()
{
	if (VSResourceManager::ms_bRenderThread)
	{
		Swap(m_pVertexUseBuffer,m_pVertexUseBufferRender);
	}
	m_pVertexUseBuffer->Clear();
	m_uiCurVUseBufferElementIndex = 0;
}
