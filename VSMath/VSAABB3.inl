inline void VSAABB3::Set(const VSVector3 & Max , const VSVector3 & Min)
{
	m_Max = Max;
	m_Min = Min;
	m_Center = (Max + Min) / 2.0f;
	VSVector3 Temp = (Max - Min) / 2.0f;
	for(int i = 0 ; i < 3; i++)
		m_fA[i] = Temp.m[i];

}
/*----------------------------------------------------------------*/
inline void VSAABB3::Set(const VSVector3 & Center,VSREAL fA0,VSREAL fA1,VSREAL fA2)
{
	m_fA[0] = ABS(fA0);
	m_fA[1] = ABS(fA1);
	m_fA[2] = ABS(fA2);
	
	m_Center = Center;
	
	m_Max.Set(m_Center.x + m_fA[0],m_Center.y + m_fA[1],m_Center.z + m_fA[2]);
	m_Min.Set(m_Center.x - m_fA[0],m_Center.y - m_fA[1],m_Center.z - m_fA[2]);
}
/*----------------------------------------------------------------*/
inline void VSAABB3::Set(const VSVector3 & Center,VSREAL fA[3])
{
	m_fA[0] = ABS(fA[0]);
	m_fA[1] = ABS(fA[1]);
	m_fA[2] = ABS(fA[2]);

	m_Center = Center;

	m_Max.Set(m_Center.x + m_fA[0],m_Center.y + m_fA[1],m_Center.z + m_fA[2]);
	m_Min.Set(m_Center.x - m_fA[0],m_Center.y - m_fA[1],m_Center.z - m_fA[2]);

}
/*----------------------------------------------------------------*/
inline void VSAABB3::GetfA(VSREAL fA[3])const
{
	for(int i =0 ;i< 3 ; i++)
	{
		fA[i] = m_fA[i];
	}

}
/*----------------------------------------------------------------*/
inline const VSVector3 & VSAABB3::GetCenter()const
{
	
	return m_Center;
}
/*----------------------------------------------------------------*/
inline VSVector3 VSAABB3::GetParameterPoint(VSREAL fAABBParameter[3])const
{

	return VSVector3(m_Center.x + fAABBParameter[0],m_Center.y + fAABBParameter[1] ,m_Center.z + fAABBParameter[2]);
}
/*----------------------------------------------------------------*/
inline VSVector3 VSAABB3::GetParameterPoint(VSREAL fAABBParameter0,VSREAL fAABBParameter1,VSREAL fAABBParameter2)const
{

	return VSVector3(m_Center.x + fAABBParameter0,m_Center.y + fAABBParameter1 ,m_Center.z + fAABBParameter2);
}
/*----------------------------------------------------------------*/
inline const VSVector3 & VSAABB3::GetMaxPoint()const
{
	return m_Max;
}
/*----------------------------------------------------------------*/
inline const VSVector3 & VSAABB3::GetMinPoint()const
{	
	return m_Min;
}