inline unsigned int VSTexture::GetFormatType()const
{
	return m_uiFormatType;
}
inline unsigned int VSTexture::GetBytePerPixel()const
{
	return VSRenderer::GetBytesPerPixel(m_uiFormatType);
}
inline unsigned int VSTexture::GetChannelPerPixel()const
{
	return VSRenderer::GetChannelPerPixel(m_uiFormatType);
}
inline unsigned int VSTexture::GetMipLevel()const
{
	return m_uiMipLevel;
}

