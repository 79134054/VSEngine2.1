/*----------------------------------------------------------------*/
inline bool VSB_SplineSurface3::SetVKnotVector(unsigned int i,VSREAL Value)
{
	VSMAC_ASSERT(m_pVKnotVector);

	VSMAC_ASSERT(i < m_uiVKnotNum);

	m_pVKnotVector[i] = Value;
	return 1;

}
/*----------------------------------------------------------------*/
inline const VSREAL * VSB_SplineSurface3::GetpVKnotVector()const
{

	return m_pVKnotVector;
}
/*----------------------------------------------------------------*/
inline VSREAL VSB_SplineSurface3:: GetVKnotVectorValue(unsigned int i)const
{
	VSMAC_ASSERT(m_pVKnotVector);

	VSMAC_ASSERT(i < m_uiVKnotNum);
	return m_pVKnotVector[i];
}
/*----------------------------------------------------------------*/
inline void VSB_SplineSurface3::SetVBasisValue(int i, int j, VSREAL Value)
{
	m_pVBasis[ i * m_uiVKnotNum + j] = Value;
}
/*----------------------------------------------------------------*/
inline VSREAL VSB_SplineSurface3::GetVBasisValue(int i ,int j)const
{
	return m_pVBasis[ i * m_uiVKnotNum + j];

}
/*----------------------------------------------------------------*/
inline void VSB_SplineSurface3::SetVBasisFDValue(int i, int j, VSREAL Value)
{
	m_pVBasisFD[ i * m_uiUKnotNum + j] = Value;
}
/*----------------------------------------------------------------*/
inline VSREAL VSB_SplineSurface3::GetVBasisFDValue(int i ,int j)const
{
	return m_pVBasisFD[ i * m_uiVKnotNum + j];
}
/*----------------------------------------------------------------*/
inline unsigned int VSB_SplineSurface3::GetVDegree()const
{
	return m_uiVDegree;
}
/*----------------------------------------------------------------*/
inline unsigned int VSB_SplineSurface3::GetVKnotNum()const
{
	return m_uiVKnotNum;
}
/*----------------------------------------------------------------*/
inline bool VSB_SplineSurface3::SetUKnotVector(unsigned int i,VSREAL Value)
{
	VSMAC_ASSERT(m_pUKnotVector);

	VSMAC_ASSERT(i < m_uiUKnotNum);

	m_pUKnotVector[i] = Value;
	return 1;

}
/*----------------------------------------------------------------*/
inline const VSREAL * VSB_SplineSurface3::GetpUKnotVector()const
{

	return m_pUKnotVector;
}
/*----------------------------------------------------------------*/
inline VSREAL VSB_SplineSurface3:: GetUKnotVectorValue(unsigned int i)const
{
	VSMAC_ASSERT(m_pUKnotVector);

	VSMAC_ASSERT(i < m_uiUKnotNum);
	return m_pUKnotVector[i];
}
/*----------------------------------------------------------------*/
inline void VSB_SplineSurface3::SetUBasisValue(int i, int j, VSREAL Value)
{
	m_pUBasis[ i * m_uiUKnotNum + j] = Value;
}
/*----------------------------------------------------------------*/
inline VSREAL VSB_SplineSurface3::GetUBasisValue(int i ,int j)const
{
	return m_pUBasis[ i * m_uiUKnotNum + j];

}
/*----------------------------------------------------------------*/
inline void VSB_SplineSurface3::SetUBasisFDValue(int i, int j, VSREAL Value)
{
	m_pUBasisFD[ i * m_uiUKnotNum + j] = Value;
}
/*----------------------------------------------------------------*/
inline VSREAL VSB_SplineSurface3::GetUBasisFDValue(int i ,int j)const
{
	return m_pUBasisFD[ i * m_uiUKnotNum + j];
}
/*----------------------------------------------------------------*/
inline unsigned int VSB_SplineSurface3::GetUDegree()const
{
	return m_uiUDegree;
}
/*----------------------------------------------------------------*/
inline unsigned int VSB_SplineSurface3::GetUKnotNum()const
{
	return m_uiUKnotNum;
}
/*----------------------------------------------------------------*/