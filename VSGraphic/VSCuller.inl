inline unsigned int VSCuller::GetPlaneNum()const
{
	return m_uiPlaneNum;

}
inline void VSCuller::SetPlaneState(unsigned int uiPlaneState)
{
	m_uiPlaneState = uiPlaneState;
}
inline unsigned int VSCuller::GetPlaneState()const
{

	return m_uiPlaneState;
}
inline unsigned int VSCuller::GetAllVisibleNum()const
{
	unsigned int uiNum = 0;
	for (unsigned int j = 0 ; j < RG_MAX ; j++)
	{
		for (unsigned int i = 0 ; i < VST_MAX ; i++)
		{
			uiNum += m_VisibleSet[j][i].GetNum();
		}
	}
	
	return uiNum;
}
inline unsigned int VSCuller::GetVisibleNum(unsigned int uiVisibleSetType,unsigned int uiRenderGroup)const
{
	if (uiVisibleSetType >= VST_MAX || uiRenderGroup >= RG_MAX)
	{
		return 0;
	}
	return m_VisibleSet[uiRenderGroup][uiVisibleSetType].GetNum();
}
inline unsigned int VSCuller::GetRenderGroupVisibleNum(unsigned int uiRenderGroup)const
{
	unsigned int uiNum = 0;
	for (unsigned int i = 0 ; i < VST_MAX ; i++)
	{
		uiNum += m_VisibleSet[uiRenderGroup][i].GetNum();
	}
	return uiNum;
}
inline void VSCuller::ClearVisibleSet()
{
	for (unsigned int j = 0 ; j < RG_MAX ; j++)
	{
		for (unsigned int i = 0 ; i < VST_MAX ; i++)
		{
			m_VisibleSet[j][i].Clear();
		}
	}
	
}
inline void VSCuller::ClearPlaneState()
{
	m_uiPlaneState =0xffffffff;
}
inline void VSCuller::ClearAllPlane()
{
	m_uiPlaneNum = 0;
}
inline void VSCuller::SetCamera(VSCamera &Camera)
{
	m_pCamera = &Camera;
}
inline VSRenderContext& VSCuller::GetVisibleSpatial(unsigned int i,unsigned int uiVisibleSetType,unsigned int uiRenderGroup)const
{
	if(uiVisibleSetType >= VST_MAX || uiRenderGroup > RG_MAX || i >=m_VisibleSet[uiRenderGroup][uiVisibleSetType].GetNum())
	{
		VSMAC_ASSERT(0);
		return VSRenderContext::ms_RenderContextNULL;
	}
	return m_VisibleSet[uiRenderGroup][uiVisibleSetType][i];
}
inline unsigned int VSCuller::GetLightNum()const
{
	return m_LightSet.GetNum();
}
inline VSLight * VSCuller::GetLight(unsigned int i)const
{
	if (i >= m_LightSet.GetNum())
	{
		return NULL;
	}

	return m_LightSet[i];
}
inline void VSCuller::ClearLight()
{
	m_LightSet.Clear();
}
inline VSCamera * VSCuller::GetCamera()const
{
	return m_pCamera;
}