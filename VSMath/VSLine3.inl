/*----------------------------------------------------------------*/
void VSLine3::Set(const VSVector3 & Orig,const VSVector3 &Dir)
{
	m_Orig = Orig;
	m_Dir = Dir;
	m_Dir.Normalize();
}
/*----------------------------------------------------------------*/
inline const VSVector3 & VSLine3::GetOrig()const 
{
	return m_Orig;
}
/*----------------------------------------------------------------*/
inline const VSVector3 & VSLine3::GetDir()const 
{
	return m_Dir;
}
/*----------------------------------------------------------------*/
inline VSVector3 VSLine3::GetParameterPoint(VSREAL fLineParameter)const
{
	return  (m_Orig + m_Dir * fLineParameter); 
 
}
/*----------------------------------------------------------------*/
