/*----------------------------------------------------------------*/
inline bool VSB_SplineCurve3::SetKnotVector(unsigned int i,VSREAL Value)
{
	VSMAC_ASSERT(m_pKnotVector);

	VSMAC_ASSERT(i < m_uiKnotNum);

	m_pKnotVector[i] = Value;
	return 1;

}
/*----------------------------------------------------------------*/
inline const VSREAL * VSB_SplineCurve3::GetpKnotVector()const
{

	return m_pKnotVector;
}
/*----------------------------------------------------------------*/
inline VSREAL VSB_SplineCurve3:: GetKnotVectorValue(unsigned int i)const
{
	VSMAC_ASSERT(m_pKnotVector);

	VSMAC_ASSERT(i < m_uiKnotNum);
	return m_pKnotVector[i];
}
/*----------------------------------------------------------------*/
inline void VSB_SplineCurve3::SetBasisValue(int i, int j, VSREAL Value)
{
	m_pBasis[ i * m_uiKnotNum + j] = Value;
}
/*----------------------------------------------------------------*/
inline VSREAL VSB_SplineCurve3::GetBasisValue(int i ,int j)const
{
	return m_pBasis[ i * m_uiKnotNum + j];

}
/*----------------------------------------------------------------*/
inline void VSB_SplineCurve3::SetBasisFDValue(int i, int j, VSREAL Value)
{
	m_pBasisFD[ i * m_uiKnotNum + j] = Value;
}
/*----------------------------------------------------------------*/
inline VSREAL VSB_SplineCurve3::GetBasisFDValue(int i ,int j)const
{
	return m_pBasisFD[ i * m_uiKnotNum + j];
}
/*----------------------------------------------------------------*/
inline void VSB_SplineCurve3::SetBasisSDValue(int i, int j, VSREAL Value)
{
	m_pBasisSD[ i * m_uiKnotNum + j] = Value;
}
/*----------------------------------------------------------------*/
inline VSREAL VSB_SplineCurve3::GetBasisSDValue(int i ,int j)const
{
	return m_pBasisSD[ i * m_uiKnotNum + j];
}
/*----------------------------------------------------------------*/
inline void VSB_SplineCurve3::SetBasisTDValue(int i, int j, VSREAL Value)
{
	m_pBasisTD[ i * m_uiKnotNum + j] = Value;
}
/*----------------------------------------------------------------*/
inline VSREAL VSB_SplineCurve3::GetBasisTDValue(int i ,int j)const
{
	return m_pBasisTD[ i * m_uiKnotNum + j];

}
/*----------------------------------------------------------------*/
inline unsigned int VSB_SplineCurve3::GetDegree()const
{
	return m_uiDegree;
}
/*----------------------------------------------------------------*/
inline unsigned int VSB_SplineCurve3::GetKnotNum()const
{
	return m_uiKnotNum;
}
/*----------------------------------------------------------------*/
