#include "VSUseBuffer.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;

VSUseBuffer:: ~VSUseBuffer()
{
	VSMAC_DELETEA(m_pBuffer);
	m_BufferElemetnArray.Clear();
}
VSUseBuffer::VSUseBuffer(bool IsStatic,unsigned int uiBufferSize)
{
	m_bIsStatic = IsStatic;
	VSMAC_ASSERT(uiBufferSize);
	m_uiBufferSize = uiBufferSize;
	m_uiCurSize = 0;
	m_pBuffer = NULL;
	m_pBuffer = VS_NEW unsigned char[uiBufferSize];
	VSMAC_ASSERT(m_pBuffer);
	m_BufferElemetnArray.Clear();
	m_uiOneAddLimitSize = 1024;
}
void * VSUseBuffer::NewGet(unsigned int uiDataSize)
{
	VSMAC_ASSERT(uiDataSize && uiDataSize <= m_uiOneAddLimitSize);

	unsigned int uiAddSize = 0;
	unsigned int uiCount = 0;

	bool IsNeedRealloc = false;
	while(uiDataSize > m_uiBufferSize - m_uiCurSize + uiAddSize)
	{
		if (!m_bIsStatic)
		{
			uiCount++;
			uiAddSize += m_uiBufferSize * uiCount;		
			IsNeedRealloc = true;
		}
		else
		{
			return NULL;
		}


	}

	if (IsNeedRealloc)
	{
		unsigned char * pBuffer = NULL;
		pBuffer = VS_NEW unsigned char[m_uiBufferSize + uiAddSize];
		if(!pBuffer)
			return false;
		VSMemcpy(pBuffer,m_pBuffer,m_uiCurSize);

		VSMAC_DELETEA(m_pBuffer);
		m_pBuffer = pBuffer;

		m_uiBufferSize += uiAddSize;
	}

	VSMemset(m_pBuffer + m_uiCurSize , 0 , uiDataSize);
	VSBufferElementInfo Element;
	Element.m_uiStart = m_uiCurSize;
	m_uiCurSize += uiDataSize;
	Element.m_uiEnd = m_uiCurSize;

	m_BufferElemetnArray.AddElement(Element);
	return (void *)(m_pBuffer + Element.m_uiStart);
}
bool VSUseBuffer::Add(const void * pData,unsigned int uiDataSize)
{
	VSMAC_ASSERT(pData && uiDataSize && uiDataSize <= m_uiOneAddLimitSize);

	unsigned int uiAddSize = 0;
	unsigned int uiCount = 0;

	bool IsNeedRealloc = false;
	while(uiDataSize > m_uiBufferSize - m_uiCurSize + uiAddSize)
	{
		if (!m_bIsStatic)
		{
			uiCount++;
			uiAddSize += m_uiBufferSize * uiCount;		
			IsNeedRealloc = true;
		}
		else
		{
			return true;
		}
	
	
	}
	
	if (IsNeedRealloc)
	{
		unsigned char * pBuffer = NULL;
		pBuffer = VS_NEW unsigned char[m_uiBufferSize + uiAddSize];
		if(!pBuffer)
			return false;
		VSMemcpy(pBuffer,m_pBuffer,m_uiCurSize);

		VSMAC_DELETEA(m_pBuffer);
		m_pBuffer = pBuffer;

		m_uiBufferSize += uiAddSize;
	}
	

	VSMemcpy(m_pBuffer + m_uiCurSize , pData , uiDataSize);
	VSBufferElementInfo Element;
	Element.m_uiStart = m_uiCurSize;
	m_uiCurSize += uiDataSize;
	Element.m_uiEnd = m_uiCurSize;

	m_BufferElemetnArray.AddElement(Element);
	return true;
}

void VSUseBuffer::Clear()
{
	m_BufferElemetnArray.Clear();
	m_uiCurSize = 0;
}
unsigned int VSUseBuffer::GetSizeByElementIndex(unsigned int i0,unsigned int i1)
{
	VSMAC_ASSERT(i1 >= i0);

	VSMAC_ASSERT(i1 < m_BufferElemetnArray.GetNum());

	unsigned int uiSize = 0;
	for (unsigned int i = i0 ; i <= i1 ; i++)
	{
		uiSize += m_BufferElemetnArray[i].GetSize();
	}
	return uiSize;
}
unsigned int VSUseBuffer::GetSizeByElementIndex(unsigned int i)
{
	return GetSizeByElementIndex(i,i);
}
VSUseBuffer::VSBufferElementInfo ElementNULL;