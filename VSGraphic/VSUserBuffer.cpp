#include "VSUserBuffer.h"
#include "VSResourceManager.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSUserBuffer, VSObject)
BEGIN_ADD_PROPERTY(VSUserBuffer, VSObject)
REGISTER_PROPERTY(m_uiDT, DataType, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_Name, Name, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiRegisterIndex, RegisterIndex, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiRegisterNum, RegisterNum, VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSUserBuffer)
IMPLEMENT_INITIAL_END
VSUserBuffer::VSUserBuffer()
{
	m_uiDT = VSDataBuffer::DT_MAXNUM;
	m_uiRegisterNum = 1;
	m_pBufferResourceArray.Clear();
}
VSUserBuffer::~VSUserBuffer()
{
	m_pBufferResourceArray.Clear();

}
VSUserBuffer::VSUserBuffer(const VSUsedName & Name, unsigned int uiDataType, unsigned int uiRegisterIndex, unsigned int uiRegisterNum)
{
	m_Name = Name;
	m_uiDT = uiDataType;
	m_uiRegisterIndex = uiRegisterIndex;
	m_uiRegisterNum = uiRegisterNum;
	m_pBufferResourceArray.SetBufferNum(m_uiRegisterNum);
}
bool VSUserBuffer::PostLoad(VSStream* pStream)
{
	m_pBufferResourceArray.SetBufferNum(m_uiRegisterNum);
	return true;
}
bool VSUserBuffer::PostClone(VSObject * pObjectSrc)
{
	m_pBufferResourceArray.SetBufferNum(m_uiRegisterNum);
	return true;
}
