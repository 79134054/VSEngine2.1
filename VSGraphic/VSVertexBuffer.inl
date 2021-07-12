inline VSDataBuffer * VSVertexBuffer::GetPositionData(unsigned int uiLevel)const
{
	if(uiLevel < (unsigned int)m_pData[VSVertexFormat::VF_POSITION].GetNum())
		return m_pData[VSVertexFormat::VF_POSITION][uiLevel];
	else
		return NULL;
}
inline VSDataBuffer * VSVertexBuffer::GetNormalData(unsigned int uiLevel)const
{
	if(uiLevel <  (unsigned int)m_pData[VSVertexFormat::VF_NORMAL].GetNum())
		return m_pData[VSVertexFormat::VF_NORMAL][uiLevel];
	else
		return NULL;
}
inline VSDataBuffer * VSVertexBuffer::GetPSizeData()const
{	
	if(m_pData[VSVertexFormat::VF_PSIZE].GetNum())
		return m_pData[VSVertexFormat::VF_PSIZE][0];
	else
		return NULL;
}
inline VSDataBuffer * VSVertexBuffer::GetColorData(unsigned int uiLevel)const
{
	if(uiLevel <  (unsigned int)m_pData[VSVertexFormat::VF_COLOR].GetNum())
		return m_pData[VSVertexFormat::VF_COLOR][uiLevel];
	else
		return NULL;
}
inline VSDataBuffer * VSVertexBuffer::GetTexCoordData(unsigned int uiLevel)const
{
	if(uiLevel <  (unsigned int)m_pData[VSVertexFormat::VF_TEXCOORD].GetNum())
		return m_pData[VSVertexFormat::VF_TEXCOORD][uiLevel];
	else
		return NULL;
}
inline VSDataBuffer * VSVertexBuffer::GetMaterialInstanceData(unsigned int uiLevel)const
{
	if (uiLevel < (unsigned int)m_pData[VSVertexFormat::VF_MATERIAL_INSTANCE].GetNum())
		return m_pData[VSVertexFormat::VF_MATERIAL_INSTANCE][uiLevel];
	else
		return NULL;
}
inline VSDataBuffer * VSVertexBuffer::GetBlendWeightData()const
{
	if(m_pData[VSVertexFormat::VF_BLENDWEIGHT].GetNum())
		return m_pData[VSVertexFormat::VF_BLENDWEIGHT][0];
	else
		return NULL;
}
inline VSDataBuffer * VSVertexBuffer::GetBlendIndicesData(unsigned int uiLevel)const
{
	if(uiLevel < m_pData[VSVertexFormat::VF_BLENDINDICES].GetNum())
		return m_pData[VSVertexFormat::VF_BLENDINDICES][uiLevel];
	else
		return NULL;
}
inline VSDataBuffer * VSVertexBuffer::GetTangentData()const
{
	if(m_pData[VSVertexFormat::VF_TANGENT].GetNum())
		return m_pData[VSVertexFormat::VF_TANGENT][0];
	else
		return NULL;
}
inline VSDataBuffer * VSVertexBuffer::GetBinormalData()const
{
	if(m_pData[VSVertexFormat::VF_BINORMAL].GetNum())
		return m_pData[VSVertexFormat::VF_BINORMAL][0];
	else
		return NULL;
}
inline VSDataBuffer * VSVertexBuffer::GetFogData()const
{
	if(m_pData[VSVertexFormat::VF_FOG].GetNum())
		return m_pData[VSVertexFormat::VF_FOG][0];
	else
		return NULL;
}
inline VSDataBuffer * VSVertexBuffer::GetVertexIDData()const
{
	if(m_pData[VSVertexFormat::VF_VERTEX_ID].GetNum())
		return m_pData[VSVertexFormat::VF_VERTEX_ID][0];
	else
		return NULL;
}
inline unsigned int VSVertexBuffer::GetPositionLevel()const
{
	return GetSemanticsNum(VSVertexFormat::VF_POSITION);
}
inline unsigned int VSVertexBuffer::GetBlendIndicesLevel()const
{
	return GetSemanticsNum(VSVertexFormat::VF_BLENDINDICES);
}
inline unsigned int VSVertexBuffer::GetColorLevel()const
{
	return GetSemanticsNum(VSVertexFormat::VF_COLOR);
}
inline unsigned int VSVertexBuffer::GetNormalLevel()const
{
	return GetSemanticsNum(VSVertexFormat::VF_NORMAL);
}
inline unsigned int VSVertexBuffer::GetTexCoordLevel()const
{
	return GetSemanticsNum(VSVertexFormat::VF_TEXCOORD);
}
inline unsigned int VSVertexBuffer::GetMaterialInstanceLevel()const
{
	return GetSemanticsNum(VSVertexFormat::VF_MATERIAL_INSTANCE);
}
inline unsigned int VSVertexBuffer::GetVertexNum()const
{
	return m_uiVertexNum;
}
inline VSVertexFormat * VSVertexBuffer::GetVertexFormat()const
{
	return m_pVertexFormat;
}
inline unsigned int VSVertexBuffer::GetOneVertexSize()const
{
	return m_uiOneVertexSize;
}
inline VSDataBuffer * VSVertexBuffer::GetData(unsigned int uiVF,unsigned int uiLevel)const
{
	if(uiVF >= VSVertexFormat::VF_MAX)
		return NULL;
	if(uiLevel >= (unsigned int)m_pData[uiVF].GetNum())
		return NULL;
	return m_pData[uiVF][uiLevel];
}
inline unsigned int VSVertexBuffer::GetLevel(unsigned int uiVF)const
{
	if (uiVF >= VSVertexFormat::VF_MAX)
		return 0;
	else
		return GetSemanticsNum(uiVF);
}
inline bool VSVertexBuffer::GetLockDataOffset(unsigned int uiVF,unsigned int uiLevel,unsigned int &uiOffset)const
{
	if (!m_pVertexFormat)
	{
		VSDataBuffer * pData = NULL;
		for(unsigned int i = 0 ; i < uiVF ; i++)
		{
			for (unsigned int j = 0 ; j < m_pData[i].GetNum() ; j++)
			{
				pData = GetData(i,j);
				if(pData)
					uiOffset += pData->GetStride();
			}
		}
		for (unsigned int j = 0 ; j < uiLevel ; j++)
		{
			pData = GetData(uiVF,j);
			if(pData)
				uiOffset += pData->GetStride();
		}
		if (!pData)
		{
			return false;
		}
		return true;
	}
	else
	{
		for (unsigned int i = 0 ;i < m_pVertexFormat->m_FormatArray.GetNum() ; i++)
		{
			if(m_pVertexFormat->m_FormatArray[i].Semantics == uiVF)
			{
				if (m_pVertexFormat->m_FormatArray[i].SemanticsIndex == uiLevel)
				{
					uiOffset = m_pVertexFormat->m_FormatArray[i].Offset;
					return true;
				}
			}
		}
		return false;
	}
	
}
inline void *VSVertexBuffer::GetLockData(unsigned int uiVF,unsigned int uiLevel)const
{
	if(uiVF >= VSVertexFormat::VF_MAX || !m_pLockData)
		return NULL;
	if(GetSemanticsNum(uiVF) <= uiLevel)
		return NULL;

	unsigned int uiOffset = 0;
	if(!GetLockDataOffset(uiVF,uiLevel,uiOffset))
		return	NULL;	
	return (unsigned char *)m_pLockData + uiOffset; 
}
inline void *VSVertexBuffer::GetLockPositionData(unsigned int uiLevel)const
{
	return GetLockData(VSVertexFormat::VF_POSITION,uiLevel);
}
inline void *VSVertexBuffer::GetLockNormalData(unsigned int uiLevel)const
{
	return GetLockData(VSVertexFormat::VF_NORMAL,uiLevel);
}
inline void *VSVertexBuffer::GetLockPSizeData()const
{
	return GetLockData(VSVertexFormat::VF_PSIZE,0);
}
inline void *VSVertexBuffer::GetLockColorData(unsigned int uiLevel)const
{
	return GetLockData(VSVertexFormat::VF_COLOR,uiLevel);
}
inline void *VSVertexBuffer::GetLockBlendWeightData()const
{
	return GetLockData(VSVertexFormat::VF_BLENDWEIGHT,0);
}
inline void *VSVertexBuffer::GetLockBlendIndicesData(unsigned int uiLevel)const
{
	return GetLockData(VSVertexFormat::VF_BLENDINDICES, uiLevel);
}
inline void *VSVertexBuffer::GetLockTangentData()const
{
	return GetLockData(VSVertexFormat::VF_TANGENT,0);
}
inline void *VSVertexBuffer::GetLockBinormalData()const
{
	return GetLockData(VSVertexFormat::VF_BINORMAL,0);
}
inline void *VSVertexBuffer::GetLockFogData()const
{
	return GetLockData(VSVertexFormat::VF_FOG,0);
}
inline void *VSVertexBuffer::GetLockVertexIDData()const
{
	return GetLockData(VSVertexFormat::VF_VERTEX_ID,0);
}
inline void *VSVertexBuffer::GetLockTexCoordData(unsigned int uiLevel)const
{
	return GetLockData(VSVertexFormat::VF_TEXCOORD,uiLevel);
}
inline void *VSVertexBuffer::GetLockMaterialInstanceData(unsigned int uiLevel)const
{
	return GetLockData(VSVertexFormat::VF_MATERIAL_INSTANCE, uiLevel);
}
inline bool VSVertexBuffer::HavePositionInfo(unsigned int uiLevel)const
{
	return GetSemanticsNum(VSVertexFormat::VF_POSITION) > uiLevel;
}
inline bool VSVertexBuffer::HaveNormalInfo(unsigned int uiLevel)const
{
	return GetSemanticsNum(VSVertexFormat::VF_NORMAL) > uiLevel;
}
inline bool VSVertexBuffer::HavePSizeInfo()const
{
	return GetSemanticsNum(VSVertexFormat::VF_PSIZE) > 0;
}
inline bool VSVertexBuffer::HaveColorInfo(unsigned int uiLevel)const
{
	return GetSemanticsNum(VSVertexFormat::VF_COLOR) > uiLevel;
}
inline bool VSVertexBuffer::HaveBlendWeightInfo()const
{
	return GetSemanticsNum(VSVertexFormat::VF_BLENDWEIGHT) > 0;
}
inline bool VSVertexBuffer::HaveBlendIndicesInfo(unsigned int uiLevel)const
{
	return GetSemanticsNum(VSVertexFormat::VF_BLENDINDICES) > uiLevel;
}
inline bool VSVertexBuffer::HaveTangentInfo()const
{
	return GetSemanticsNum(VSVertexFormat::VF_TANGENT) > 0;
}
inline bool VSVertexBuffer::HaveBinormalInfo()const
{
	return GetSemanticsNum(VSVertexFormat::VF_BINORMAL) > 0;
}
inline bool VSVertexBuffer::HaveFogInfo()const
{
	return GetSemanticsNum(VSVertexFormat::VF_FOG) > 0;
}
inline bool VSVertexBuffer::HaveVertexIDInfo()const
{
	return GetSemanticsNum(VSVertexFormat::VF_VERTEX_ID) > 0;
}
inline bool VSVertexBuffer::HaveTexCoordInfo(unsigned int uiLevel)const
{
	return GetSemanticsNum(VSVertexFormat::VF_TEXCOORD) > uiLevel;
}
inline bool VSVertexBuffer::HaveMaterialInstanceInfo(unsigned int uiLevel)const
{
	return GetSemanticsNum(VSVertexFormat::VF_MATERIAL_INSTANCE) > uiLevel;
}
inline unsigned int VSVertexBuffer::PositionChannel(unsigned int uiLevel)const
{
	return GetSemanticsChannel(VSVertexFormat::VF_POSITION,uiLevel);
}
inline unsigned int VSVertexBuffer::NormalChannel(unsigned int uiLevel)const
{
	return GetSemanticsChannel(VSVertexFormat::VF_NORMAL,uiLevel);
}
inline unsigned int VSVertexBuffer::PSizeChannel()const
{
	return GetSemanticsChannel(VSVertexFormat::VF_PSIZE,0);
}
inline unsigned int VSVertexBuffer::ColorChannel(unsigned int uiLevel)const
{
	return GetSemanticsChannel(VSVertexFormat::VF_COLOR,uiLevel);
}
inline unsigned int VSVertexBuffer::BlendWeightChannel()const
{
	return GetSemanticsChannel(VSVertexFormat::VF_BLENDWEIGHT,0);
}
inline unsigned int VSVertexBuffer::BlendIndicesChannel(unsigned int uiLevel)const
{
	return GetSemanticsChannel(VSVertexFormat::VF_BLENDINDICES, uiLevel);
}
inline unsigned int VSVertexBuffer::TangentChannel()const
{
	return GetSemanticsChannel(VSVertexFormat::VF_TANGENT,0);
}
inline unsigned int VSVertexBuffer::BinormalChannel()const
{
	return GetSemanticsChannel(VSVertexFormat::VF_BINORMAL,0);
}
inline unsigned int VSVertexBuffer::FogChannel()const
{
	return GetSemanticsChannel(VSVertexFormat::VF_FOG,0);
}
inline unsigned int VSVertexBuffer::VertexIDChannel()const
{
	return GetSemanticsChannel(VSVertexFormat::VF_VERTEX_ID,0);
}
inline unsigned int VSVertexBuffer::TexCoordChannel(unsigned int uiLevel)const
{
	return GetSemanticsChannel(VSVertexFormat::VF_TEXCOORD,uiLevel);
}
inline unsigned int VSVertexBuffer::MaterialInstanceChannel(unsigned int uiLevel)const
{
	return GetSemanticsChannel(VSVertexFormat::VF_MATERIAL_INSTANCE, uiLevel);
}
inline unsigned int VSVertexBuffer::NormalDataType(unsigned int uiLevel)const
{
	return GetSemanticsDataType(VSVertexFormat::VF_NORMAL,uiLevel);
}
inline unsigned int VSVertexBuffer::TangentDataType()const
{
	return GetSemanticsDataType(VSVertexFormat::VF_TANGENT,0);
}
inline unsigned int VSVertexBuffer::BinormalDataType()const
{
	return GetSemanticsDataType(VSVertexFormat::VF_BINORMAL,0);
}