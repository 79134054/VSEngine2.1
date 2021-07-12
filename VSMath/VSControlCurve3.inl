inline bool VSControlCurve3::ChangeControlPoint(unsigned int i,const VSVector3 & ControlPoint)
{
	VSMAC_ASSERT(m_pControlPoint);

	VSMAC_ASSERT(i < m_uiControlPointNum);

	m_pControlPoint[i] = ControlPoint;
	return 1;


}
/*----------------------------------------------------------------*/
inline const VSVector3 * VSControlCurve3::GetpControlPoint()const
{

	return m_pControlPoint;
}
/*----------------------------------------------------------------*/
inline VSVector3 VSControlCurve3::GetControlPoint(unsigned int i)const
{
	VSMAC_ASSERT(m_pControlPoint);

	VSMAC_ASSERT(i < m_uiControlPointNum);
	return m_pControlPoint[i];
}
/*----------------------------------------------------------------*/
inline unsigned int VSControlCurve3::GetControlPointNum()const
{

	return m_uiControlPointNum;
}
/*----------------------------------------------------------------*/
inline unsigned int VSControlCurve3::GetType()const
{
	return m_CurveType;
}