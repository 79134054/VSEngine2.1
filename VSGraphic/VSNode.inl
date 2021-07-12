inline unsigned int VSNode::GetNodeNum()const
{
	return m_pChild.GetNum();
}
inline VSArray<VSSpatialPtr> * VSNode::GetChildList()
{
	return &m_pChild;
}
