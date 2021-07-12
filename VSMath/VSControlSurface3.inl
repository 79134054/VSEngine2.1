inline VSVector3 VSControlSurface3::GetControlPoint(unsigned int U,unsigned int V)const
{

	VSMAC_ASSERT(U <= m_uiRow && V <= m_uiColumn);

	return m_pControlPoint[U * m_uiColumn + V];

}
/*----------------------------------------------------------------*/
inline bool VSControlSurface3::ChangeControlPoint(unsigned int U,unsigned int V,const VSVector3 &Value)
{
	VSMAC_ASSERT(U <= m_uiRow && V <= m_uiColumn);
	m_pControlPoint[U * m_uiColumn + V] = Value;
	return 1;
}
/*----------------------------------------------------------------*/
inline const VSVector3 *VSControlSurface3::GetControlPoint()const
{
	return m_pControlPoint;

}
/*----------------------------------------------------------------*/
inline unsigned int VSControlSurface3::GetControlPointNum()const
{
	return m_uiControlPointNum;
}
/*----------------------------------------------------------------*/
inline unsigned int VSControlSurface3::GetRow()const
{
	return m_uiRow;
}
/*----------------------------------------------------------------*/
inline unsigned int VSControlSurface3::GetColumn()const
{
	return m_uiColumn;
}
/*----------------------------------------------------------------*/