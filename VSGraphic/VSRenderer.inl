inline DWORD VSRenderer::ShaderVersion(UINT Major,UINT Minor)
{
	return (0xFFFE0000|((Major)<<8)|(Minor));
}
inline unsigned int VSRenderer::GetScreenWith()const
{	
	return m_uiScreenWidth;
}
inline unsigned int VSRenderer::GetScreenHeight()const
{
	return m_uiScreenHeight;
}
inline unsigned int VSRenderer::GetCurAnisotropy()const
{
	return m_uiCurAnisotropy;
}
inline unsigned int VSRenderer::GetCurMultisample()const
{	
	return m_uiCurMultisample;
}
inline unsigned int VSRenderer::GetMaxMultisample()const
{
	return m_uiMaxMultisample;
}
inline unsigned int VSRenderer::GetDepthStencilFormat()const
{
	return m_uiDepthStencilFormat;
}
inline bool VSRenderer::IsWindowed()const
{
	return m_bWindowed;
}
inline unsigned int VSRenderer::WindoweNum()const
{
	if (m_bWindowed)
	{
		return m_iNumChildWindow;
	}
	return 0;
}
inline unsigned int VSRenderer::GetMaxTextureWidth()const
{
	return m_uiMaxTextureWidth;
}
inline unsigned int VSRenderer::GetMaxTextureHeight()const
{
	return m_uiMaxTextureHeight;
}
inline unsigned int VSRenderer::GetMaxUseClipPlane()const
{
	return m_uiMaxUseClipPlane;
}
inline unsigned int VSRenderer::GetMaxBindResource(unsigned int uiShaderType)const
{
	return m_uiMaxBindResource[uiShaderType];
}
inline unsigned int VSRenderer::GetMaxSampler(unsigned int uiShaderType)const
{
	return m_uiMaxSampler[uiShaderType];
}
inline unsigned int VSRenderer::GetBufferFormat()const
{
	return m_uiBufferFormat;
}
inline unsigned int VSRenderer::GetMaxAnisotropy()const
{
	return m_uiMaxAnisotropy;
}
inline unsigned int VSRenderer::GetMaxRTNum()const
{
	return m_uiMaxRTNum;
}
inline unsigned int VSRenderer::GetMaxUAVNum()const
{
	return m_uiMaxUAVNum;
}
inline VSString VSRenderer::GetAdapterName()const
{
	return m_AdapterName;
}
//----------------------------------------------------------------------------
inline void VSRenderer::SetClearColor (const VSColorRGBA& ClearColor)
{
	m_ClearColor = ClearColor;

}
//----------------------------------------------------------------------------
inline const VSColorRGBA& VSRenderer::GetClearColor () const
{
	return m_ClearColor;
}
//----------------------------------------------------------------------------
inline void VSRenderer::SetClearDepth (VSREAL fClearDepth)
{
	m_fClearDepth = fClearDepth;

}
//----------------------------------------------------------------------------
inline VSREAL VSRenderer::GetClearDepth () const
{
	return m_fClearDepth;
}
//----------------------------------------------------------------------------
inline void VSRenderer::SetClearStencil (unsigned int uiClearStencil)
{
	m_uiClearStencil = uiClearStencil;

}
//----------------------------------------------------------------------------
inline unsigned int VSRenderer::GetClearStencil () const
{
	return m_uiClearStencil;
}
inline unsigned int VSRenderer::GetCurRTWidth()const
{
	return m_uiCurRTWidth;
}
inline unsigned int VSRenderer::GetCurRTHeight()const
{
	return m_uiCurRTHeight;
}
inline unsigned int VSRenderer::GetCurRTMultisampler()const
{
	return m_uiCurRTMultisample;
}
inline unsigned int VSRenderer::GetSuitableMultisample(unsigned int Multisample)const
{
	return Multisample > m_uiMaxMultisample ? m_uiMaxMultisample : Multisample;
}

