inline bool VSSurface3Subdivision::SetSurface(VSSurface3 * pSurface)
{

	if(!pSurface)
		return 0;
	m_pSurface = pSurface;
	return 1;
}
inline VSVector3 * VSSurface3Subdivision::GetPoints()const
{
	return m_pPoint;
}
inline unsigned int VSSurface3Subdivision::GetRow()const
{
	return m_uiRow;
}
inline unsigned int VSSurface3Subdivision::GetColumn()const
{

	return m_uiColumn;
}
inline VSVector2 * VSSurface3Subdivision::GetUV()const
{

	return m_pUV;
}