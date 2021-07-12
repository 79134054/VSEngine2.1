inline bool VSNURBSCurve3::ChangeWeight(unsigned int i,VSREAL Weight)
{
	VSMAC_ASSERT(m_pWeight);
	VSMAC_ASSERT(i < m_uiControlPointNum);
	m_pWeight[i] = Weight;
	return 1;
}
/*----------------------------------------------------------------*/
inline const VSREAL * VSNURBSCurve3::GetWeight()const
{
	return m_pWeight;

}
/*----------------------------------------------------------------*/
inline VSREAL VSNURBSCurve3::GetWeight(unsigned int i)const
{
	VSMAC_ASSERT(m_pWeight);

	VSMAC_ASSERT(i < m_uiControlPointNum);

	return m_pWeight[i];

}
/*----------------------------------------------------------------*/