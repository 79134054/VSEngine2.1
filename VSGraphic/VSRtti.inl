/********************************************************************************/
inline const VSString &VSRtti::GetName()const
{
	return m_cRttiName;

}//end GetName
/********************************************************************************/
inline bool VSRtti::IsSameType(const VSRtti &Type) const
{
	return  (&Type == this);
}//end IsSameType
/********************************************************************************/
inline bool VSRtti::IsDerived(const VSRtti &Type) const
{
	const VSRtti * pTemp = this;
	while(!pTemp->IsSameType(Type))
	{
		if(pTemp->m_pBase)
		{
			pTemp = pTemp->m_pBase;
		
		}
		else
		{
			return 0;
		}
	
	}
	return 1;
}//end IsDerived
/********************************************************************************/