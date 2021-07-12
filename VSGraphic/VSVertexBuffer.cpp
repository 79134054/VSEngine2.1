#include "VSVertexBuffer.h"
#include "VSShaderStringFactory.h"
#include "VSGraphicInclude.h"
#include "VSResourceManager.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSVertexBuffer,VSBind)
BEGIN_ADD_PROPERTY(VSVertexBuffer,VSBind)
REGISTER_PROPERTY(m_pData[VSVertexFormat::VF_POSITION], PositionData, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pData[VSVertexFormat::VF_TEXCOORD], TextureData, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pData[VSVertexFormat::VF_NORMAL], NormalData, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pData[VSVertexFormat::VF_TANGENT], TangetData, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pData[VSVertexFormat::VF_BINORMAL], BinormalData, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pData[VSVertexFormat::VF_PSIZE], PSizeData, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pData[VSVertexFormat::VF_COLOR], ColorData, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pData[VSVertexFormat::VF_FOG], FogData, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pData[VSVertexFormat::VF_VERTEX_ID], DepthData, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pData[VSVertexFormat::VF_BLENDWEIGHT], BlendWeightData, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pData[VSVertexFormat::VF_BLENDINDICES], BlendIndicesData, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pData[VSVertexFormat::VF_MATERIAL_INSTANCE], MaterialInstanceData, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiVertexNum,VertexNum,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiOneVertexSize,OneVertexSize,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSVertexBuffer)
IMPLEMENT_INITIAL_END
VSVertexBuffer::VSVertexBuffer(bool bIsStatic)
{
	for(unsigned int i = 0 ; i < VSVertexFormat::VF_MAX ; i++)
	{
		m_pData[i].Clear();
	}
	m_uiVertexNum = 0;
	m_bIsStatic = bIsStatic;
	m_pVertexFormat = NULL;
	m_uiOneVertexSize = 0;
	m_pLockData = NULL;
}
VSVertexBuffer::VSVertexBuffer()
{
	
	for(unsigned int i = 0 ; i < VSVertexFormat::VF_MAX ; i++)
	{
		m_pData[i].Clear();
	}
	m_uiVertexNum = 0;
	m_pVertexFormat = NULL;
	m_uiOneVertexSize = 0;
	m_pLockData = NULL;
}
VSVertexBuffer::~VSVertexBuffer()
{
	ReleaseResource();
	for(unsigned int i = 0 ; i < VSVertexFormat::VF_MAX ; i++)
	{
		m_pData[i].Clear();
	}
	m_uiVertexNum = 0;
	m_pVertexFormat = NULL;
	m_uiOneVertexSize = 0;
	m_pLockData = NULL;
}


bool VSVertexBuffer::SetData(VSDataBuffer * pData,unsigned int uiVF)
{
	VSMAC_ASSERT(pData && !m_pVertexFormat && uiVF < VSVertexFormat::VF_MAX);

	VSMAC_ASSERT(pData->GetData());
	if(uiVF == VSVertexFormat::VF_POSITION)
	{
		
				
		m_pData[uiVF].AddElement(pData);
		

	}
	else if(uiVF == VSVertexFormat::VF_NORMAL)
	{

		m_pData[uiVF].AddElement(pData);
	}
	else if(uiVF == VSVertexFormat::VF_PSIZE)
	{
		if(!m_pData[uiVF].GetNum())
			m_pData[uiVF].AddElement(pData);
		else
			return 0;
	}
	else if(uiVF == VSVertexFormat::VF_COLOR)
	{
		if(m_pData[uiVF].GetNum() < 2)
			m_pData[uiVF].AddElement(pData);
		else
			return 0;
	}
	else if(uiVF == VSVertexFormat::VF_BLENDWEIGHT)
	{
		if(!m_pData[uiVF].GetNum())
			m_pData[uiVF].AddElement(pData);
		else
			return 0;
	}
	else if(uiVF == VSVertexFormat::VF_BLENDINDICES)
	{
		m_pData[uiVF].AddElement(pData);
	}
	else if(uiVF == VSVertexFormat::VF_BINORMAL)
	{
		if(!m_pData[uiVF].GetNum())
			m_pData[uiVF].AddElement(pData);
		else
			return 0;
	}
	else if(uiVF == VSVertexFormat::VF_TANGENT)
	{
		if(!m_pData[uiVF].GetNum())
			m_pData[uiVF].AddElement(pData);
		else
			return 0;
	}
	else if(uiVF == VSVertexFormat::VF_FOG)
	{
		if(!m_pData[uiVF].GetNum())
			m_pData[uiVF].AddElement(pData);
		else
			return 0;
	}
	else if(uiVF == VSVertexFormat::VF_VERTEX_ID)
	{
		if(!m_pData[uiVF].GetNum())
			m_pData[uiVF].AddElement(pData);
		else
			return 0;
	}
	else if(uiVF == VSVertexFormat::VF_TEXCOORD)
	{
		m_pData[uiVF].AddElement(pData);
	}
	else	
		return 0;

	if(!m_uiVertexNum)
		m_uiVertexNum = pData->GetNum();
	else
	{
		if(m_uiVertexNum != pData->GetNum())
			return 0;
	}
	m_uiOneVertexSize += pData->GetStride();
	return 1;
}
bool VSVertexBuffer::GetVertexFormat(VSArray<VSVertexFormat::VERTEXFORMAT_TYPE> &FormatArray)
{
	
	if (m_pVertexFormat)
	{
		FormatArray = m_pVertexFormat->m_FormatArray;

	}
	else
	{
		VSDataBuffer * pData;
		VSVertexFormat::VERTEXFORMAT_TYPE Element;
		unsigned int iVertexSize = 0;
		for(unsigned int i = 0 ; i < GetPositionLevel(); i++)
		{
			pData = GetPositionData(i);
			if(pData)
			{
				Element.Offset = (WORD)iVertexSize;
				iVertexSize += pData->GetStride();
				Element.DataType = pData->GetDT();
				Element.Semantics = VSVertexFormat::VF_POSITION;
				Element.SemanticsIndex = i;
				FormatArray.AddElement(Element);
			}

		}

		for(unsigned int i = 0 ; i < GetTexCoordLevel(); i++)
		{
			pData = GetTexCoordData(i);
			if(pData)
			{
				Element.Offset = (WORD)iVertexSize;
				iVertexSize += pData->GetStride();
				Element.DataType = pData->GetDT();
				Element.Semantics = VSVertexFormat::VF_TEXCOORD;
				Element.SemanticsIndex = i;
				FormatArray.AddElement(Element);
			}
		}

		for(unsigned int i = 0 ; i < GetNormalLevel(); i++)
		{
			pData = GetNormalData(i);
			if(pData)
			{
				Element.Offset = (WORD)iVertexSize;
				iVertexSize += pData->GetStride();
				Element.DataType = pData->GetDT();
				Element.Semantics = VSVertexFormat::VF_NORMAL;
				Element.SemanticsIndex = i;
				FormatArray.AddElement(Element);
			}
		}

		pData = GetTangentData();
		if(pData)
		{
			Element.Offset = (WORD)iVertexSize;
			iVertexSize += pData->GetStride();
			Element.DataType = pData->GetDT();
			Element.Semantics = VSVertexFormat::VF_TANGENT;
			Element.SemanticsIndex = 0;
			FormatArray.AddElement(Element);
		}

		pData = GetBinormalData();
		if(pData)
		{
			Element.Offset = (WORD)iVertexSize;
			iVertexSize += pData->GetStride();
			Element.DataType = pData->GetDT();
			Element.Semantics = VSVertexFormat::VF_BINORMAL;
			Element.SemanticsIndex = 0;
			FormatArray.AddElement(Element);
		}

		pData = GetPSizeData();
		if(pData)
		{
			Element.Offset = (WORD)iVertexSize;
			iVertexSize += pData->GetStride();
			Element.DataType = pData->GetDT();
			Element.Semantics = VSVertexFormat::VF_PSIZE;
			Element.SemanticsIndex = 0;
			FormatArray.AddElement(Element);
		}

		for(unsigned int i = 0 ; i < GetColorLevel(); i++)
		{

			pData = GetColorData(i);
			if(pData)
			{
				Element.Offset = (WORD)iVertexSize;
				iVertexSize += pData->GetStride();
				Element.DataType = pData->GetDT();
				Element.Semantics = VSVertexFormat::VF_COLOR;
				Element.SemanticsIndex = i;
				FormatArray.AddElement(Element);
			}
		}

		pData = GetFogData();
		if(pData)
		{
			Element.Offset = (WORD)iVertexSize;
			iVertexSize += pData->GetStride();
			Element.DataType = pData->GetDT();
			Element.Semantics = VSVertexFormat::VF_FOG;
			Element.SemanticsIndex = 0;
			FormatArray.AddElement(Element);
		}

		pData = GetVertexIDData();
		if(pData)
		{
			Element.Offset = (WORD)iVertexSize;
			iVertexSize += pData->GetStride();
			Element.DataType = pData->GetDT();
			Element.Semantics = VSVertexFormat::VF_VERTEX_ID;
			Element.SemanticsIndex = 0;
			FormatArray.AddElement(Element);
		}

		pData = GetBlendWeightData();
		if(pData)
		{
			Element.Offset = (WORD)iVertexSize;
			iVertexSize += pData->GetStride();
			Element.DataType = pData->GetDT();
			Element.Semantics = VSVertexFormat::VF_BLENDWEIGHT;
			Element.SemanticsIndex = 0;
			FormatArray.AddElement(Element);
		}
		for (unsigned int i = 0; i < GetBlendIndicesLevel(); i++)
		{
			pData = GetBlendIndicesData(i);
			if (pData)
			{
				Element.Offset = (WORD)iVertexSize;
				iVertexSize += pData->GetStride();
				Element.DataType = pData->GetDT();
				Element.Semantics = VSVertexFormat::VF_BLENDINDICES;
				Element.SemanticsIndex = i;
				FormatArray.AddElement(Element);
			}
		}
	}
	
	
	return (FormatArray.GetNum() > 0);

	
}
void VSVertexBuffer::ClearInfo()
{
	if(m_uiSwapChainNum == m_InfoArray.GetNum())
	{
		if (m_uiMemType == MT_VRAM)
		{
			for (unsigned int i = 0 ; i < VSVertexFormat::VF_MAX ; i++)
			{
				m_pData[i].Destroy();
			}
		}
	}
}
bool VSVertexBuffer::LoadResource(VSRenderer * pRender)
{
	if (m_uiMemType == MT_RAM)
	{
		return 1;
	}
	if(m_uiSwapChainNum == m_InfoArray.GetNum())
		return 1;
	if(!m_pVertexFormat)
	{
		//´´½¨
		VSResourceManager::LoadVertexFormat(this);
	}
	if(!m_pVertexFormat->LoadResource(pRender))
		return 0;
	

	if(!VSMemBind::LoadResource(pRender))
		return 0;

	return 1;

}

bool VSVertexBuffer::OnLoadResource(VSResourceIdentifier *&pID)
{
	VSMAC_ASSERT(m_pUser);
	if(!m_pUser->OnLoadVBufferData(this,pID))
		return 0;
	return 1;

}
unsigned int VSVertexBuffer::GetByteSize()const
{
	return m_uiVertexNum * m_uiOneVertexSize;
}

void *VSVertexBuffer::Lock()
{
	if (m_bIsStatic || !m_pUser)
	{
		return NULL;
	}

	VSMAC_ASSERT(m_pLockData == NULL);
	m_pLockData = m_pUser->Lock(this);

	return m_pLockData;
}
void VSVertexBuffer::UnLock()
{
	if (m_bIsStatic || !m_pUser)
	{
		return;
	}

	VSMAC_ASSERT(m_pLockData);

	m_pUser->UnLock(this);

	m_pLockData = NULL;
}
VSVertexBuffer::VSVertexBuffer(VSArray<VSVertexFormat::VERTEXFORMAT_TYPE>& FormatArray,unsigned int uiNum)
{
	VSMAC_ASSERT(FormatArray.GetNum() && uiNum);
	m_pVertexFormat = NULL;
	m_uiOneVertexSize = 0;
	m_uiVertexNum = uiNum;
	m_pLockData = NULL;
	for (unsigned int i = 0 ; i < FormatArray.GetNum() ; i++)
	{
		m_uiOneVertexSize += VSDataBuffer::ms_uiDataTypeByte[FormatArray[i].DataType];
	}
	m_pVertexFormat = VSResourceManager::LoadVertexFormat(this,&FormatArray);
	VSMAC_ASSERT(m_pVertexFormat);
}
unsigned int VSVertexBuffer::GetSemanticsNum(unsigned int uiSemantics)const
{
	if (!m_pVertexFormat)
	{
		return m_pData[uiSemantics].GetNum();
	}
	else
	{
		unsigned int uiNum = 0;
		for (unsigned int i = 0 ; i < m_pVertexFormat->m_FormatArray.GetNum() ; i++)
		{
			if (m_pVertexFormat->m_FormatArray[i].Semantics == uiSemantics)
			{
				uiNum++;
			}
		}
		return uiNum;
	}
	
}
unsigned int VSVertexBuffer::GetSemanticsChannel(unsigned int uiSemantics,unsigned int uiLevel)const
{
	if (!m_pVertexFormat)
	{
		if (uiLevel >= m_pData[uiSemantics].GetNum())
		{
			return 0;
		}
		return m_pData[uiSemantics][uiLevel]->GetChannel();
	}
	else
	{
		unsigned int uiNum = 0;
		for (unsigned int i = 0 ; i < m_pVertexFormat->m_FormatArray.GetNum() ; i++)
		{
			if (m_pVertexFormat->m_FormatArray[i].Semantics == uiSemantics)
			{
				if (uiLevel == uiNum)
				{
					return VSDataBuffer::ms_uiDataTypeChannel[m_pVertexFormat->m_FormatArray[i].DataType];
				}
				uiNum++;
			}
		}
		return 0;
	}
}
unsigned int VSVertexBuffer::GetSemanticsDataType(unsigned int uiSemantics,unsigned int uiLevel)const
{
	if (!m_pVertexFormat)
	{
		if (uiLevel >= m_pData[uiSemantics].GetNum())
		{
			return 0;
		}
		return m_pData[uiSemantics][uiLevel]->GetDT();
	}
	else
	{
		unsigned int uiNum = 0;
		for (unsigned int i = 0 ; i < m_pVertexFormat->m_FormatArray.GetNum() ; i++)
		{
			if (m_pVertexFormat->m_FormatArray[i].Semantics == uiSemantics)
			{
				if (uiLevel == uiNum)
				{
					return m_pVertexFormat->m_FormatArray[i].DataType;
				}
				uiNum++;
			}
		}
		return 0;
	}
}	