inline const VSVector3 & VSSegment3::GetEnd()const 
{
	return m_End;
}
/*----------------------------------------------------------------*/
inline VSREAL VSSegment3::GetLen()const 
{
	return m_fLen;
}
/*----------------------------------------------------------------*/
inline void VSSegment3::Set(const VSVector3 &Orig,const VSVector3 &End)
{
	VSVector3 Dir = End - Orig;
	m_fLen = Dir.GetLength();

	VSLine3::Set(Orig,Dir);

}
/*----------------------------------------------------------------*/
inline void VSSegment3::Set(const VSVector3 &Orig,const VSVector3 &Dir,VSREAL fLen)
{


	VSLine3::Set(Orig,Dir);
	m_fLen = fLen;
	m_End = Orig + (m_Dir * m_fLen);

}
/*----------------------------------------------------------------*/
inline VSVector3 VSSegment3::GetParameterPoint(VSREAL fSegmentParameter)const
{
	if(fSegmentParameter > m_fLen)
		fSegmentParameter = m_fLen;

	return VSRay3::GetParameterPoint(fSegmentParameter);
}
/*----------------------------------------------------------------*/