#include "VSBufferResource.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSBufferResource, VSBind)
BEGIN_ADD_PROPERTY(VSBufferResource, VSBind)
REGISTER_PROPERTY(m_pData, StructData, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiNum, Num, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiDT, DataType, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiStructStride, StructStride, VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSBufferResource)
IMPLEMENT_INITIAL_END
VSBufferResource::VSBufferResource()
{
	m_pData = NULL;
	m_uiNum = 0;
	m_uiDT = VSDataBuffer::DT_FLOAT32_1;
	m_pLockData = NULL;
	m_uiStructStride = 0;
}
VSBufferResource::~VSBufferResource()
{
	ReleaseResource();
	m_pData = NULL;
	m_pLockData = NULL;
}
void VSBufferResource::ClearInfo()
{
	if (m_uiSwapChainNum == m_InfoArray.GetNum())
	{
		if (m_uiMemType == MT_VRAM)
		{
			m_pData = NULL;
		}
	}
}
bool VSBufferResource::OnLoadResource(VSResourceIdentifier *&pID)
{
	VSMAC_ASSERT(m_pUser);
	if (!m_pUser->OnLoadSBuffer(this, pID))
		return 0;
	return 1;
}
void *VSBufferResource::Lock()
{
	if (m_bIsStatic || !m_pUser)
	{
		return NULL;
	}

	VSMAC_ASSERT(m_pLockData == NULL);
	m_pLockData = m_pUser->Lock(this);


	return m_pLockData;
}
void VSBufferResource::UnLock()
{
	if (m_bIsStatic || !m_pUser)
	{
		return;
	}

	VSMAC_ASSERT(m_pLockData);
	m_pUser->UnLock(this);
	m_pLockData = NULL;
}
bool VSBufferResource::SetData(VSDataBuffer * pData)
{
	VSMAC_ASSERT(pData->GetNum() && pData->GetData());

	m_pData = pData;
	m_uiNum = pData->GetNum();
	m_uiDT = pData->GetDT();
	m_uiStructStride = pData->GetStructStride();
	return 1;
}
unsigned int VSBufferResource::GetByteSize()const
{
	return (m_uiDT == VSDataBuffer::DT_STRUCT ? m_uiStructStride : VSDataBuffer::ms_uiDataTypeByte[m_uiDT]) * m_uiNum;
}
VSBufferResource::VSBufferResource(unsigned int uiNum, unsigned int uiDT, unsigned int uiStructStride)
{
	VSMAC_ASSERT(uiNum);
	if (uiDT == VSDataBuffer::DT_UBYTE)
	{
		VSMAC_ASSERT(uiNum % 4 == 0)
	}
	VSMAC_ASSERT(uiDT != VSDataBuffer::DT_STRUCT || uiStructStride > 0)
	m_pData = NULL;
	m_uiNum = uiNum;
	m_uiDT = uiDT;
	m_pLockData = NULL;
	m_uiStructStride = uiStructStride;
}
bool VSBufferResource::SetOutput(class VSBufferUnorderAccess *pOutputResource)
{
	VSMAC_ASSERT(pOutputResource);

	if (IsBindResource())
	{
		return false;
	}
	if (m_uiOutputType == VSOutputResource::OT_NONE)
	{
		m_uiOutputType = pOutputResource->GetOutputType();
	}
	else if (m_uiOutputType != pOutputResource->GetOutputType())
	{
		return false;
	}
	if (m_pCurUnorderAccess)
	{
		m_pCurUnorderAccess->m_pCreateBy = NULL;
	}	
	m_pCurUnorderAccess = pOutputResource;
	return true;
}