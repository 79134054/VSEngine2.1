/*----------------------------------------------------------------*/
inline bool VSNURBSSurface3::ChangeWeight(unsigned int U,unsigned int V,VSREAL Weight)
{
	VSMAC_ASSERT(U <= m_uiRow && V <= m_uiColumn);

	m_pWeight[U * m_uiColumn + V] = Weight;
	return 1;

}
/*----------------------------------------------------------------*/
inline const VSREAL * VSNURBSSurface3::GetWeight()const
{
	return m_pWeight;
}
/*----------------------------------------------------------------*/
inline VSREAL VSNURBSSurface3::GetWeight(unsigned int U,unsigned int V)const
{
	VSMAC_ASSERT(U <= m_uiRow && V <= m_uiColumn);
	return m_pWeight[U * m_uiColumn + V];

}
/*----------------------------------------------------------------*/