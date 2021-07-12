#include "VSBufferUnorderAccess.h"
#include "VSVertexFormat.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSBufferUnorderAccess, VSBind)
BEGIN_ADD_PROPERTY(VSBufferUnorderAccess, VSBind)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSBufferUnorderAccess)
IMPLEMENT_INITIAL_END
VSBufferUnorderAccess::VSBufferUnorderAccess()
{
	m_pLockData = NULL;
	m_uiNum = 0;
	m_uiDT = VSDataBuffer::DT_FLOAT32_1;
	m_uiStructStride = 0;
}
VSBufferUnorderAccess::~VSBufferUnorderAccess()
{
	ReleaseResource();
	
}
VSBufferUnorderAccess::VSBufferUnorderAccess(unsigned int uiNum, unsigned int uiDT, unsigned int uiStructStride, bool CPURead)
{
	VSMAC_ASSERT(uiNum);
	if (uiDT == VSDataBuffer::DT_UBYTE)
	{
		VSMAC_ASSERT(uiNum % 4 == 0)
	}
	VSMAC_ASSERT(uiDT != VSDataBuffer::DT_STRUCT || uiStructStride > 0)
		m_uiNum = uiNum;
	m_uiDT = uiDT;
	m_uiStructStride = uiStructStride;
	m_bUsed = false;
	m_bIsStatic = !CPURead;
	m_uiLockFlag = CPURead ? VSInheritBind::LF_READONLY : VSInheritBind::LF_NOOVERWRITE;
	m_pLockData = NULL;
}
VSBufferUnorderAccess::VSBufferUnorderAccess(VSBufferResource * pCreateBy, bool CPURead)
{
	m_pCreateBy = pCreateBy;
	m_uiNum = pCreateBy->GetNum();
	m_uiDT = pCreateBy->GetDataType();
	m_uiStructStride = pCreateBy->GetStructStride();
	m_bUsed = false;
	m_bIsStatic = !CPURead;
	m_uiLockFlag = CPURead ? VSInheritBind::LF_READONLY : VSInheritBind::LF_NOOVERWRITE;
	m_pLockData = NULL;
	pCreateBy->SetOutput(this);
}
unsigned int VSBufferUnorderAccess::GetByteSize()const
{
	return (m_uiDT == VSDataBuffer::DT_STRUCT ? m_uiStructStride : VSDataBuffer::ms_uiDataTypeByte[m_uiDT]) * m_uiNum;
}
bool VSBufferUnorderAccess::OnLoadResource(VSResourceIdentifier *&pID)
{

	if (!m_pUser->OnLoadUnorderAccess(this, pID))
		return 0;
	return 1;
}
void *VSBufferUnorderAccess::Lock()
{
	if (m_pLockData)
	{
		return NULL;
	}

	m_pLockData = m_pUser->Lock(this);

	return m_pLockData;
}
void VSBufferUnorderAccess::UnLock()
{
	if (!m_pLockData)
	{
		return;
	}

	m_pUser->UnLock(this);

	m_pLockData = NULL;
}