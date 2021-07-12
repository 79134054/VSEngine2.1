#pragma once
#include "VSMemBind.h"
#include "VSRenderer.h"
#include "VSBitArray.h"
#include "VSSlot.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSTexture : public VSMemBind, public VSSlot
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY;
	public:
		enum	//Tex Type
		{
			TT_1D,
			TT_2D,
			TT_3D,
			TT_CUBE,
			TT_2DARRAY,
			TT_MAX
		};
	protected:
		VSTexture(unsigned int uiFormatType, unsigned int uiWidth,
			unsigned int uiHeight, unsigned int uiLength, unsigned int uiArraySize = 1,
					unsigned int uiMipLevel = 0,bool bIsStatic = true,bool bSRGB = false);
		VSTexture();
	public:
		virtual ~VSTexture()= 0;

		virtual unsigned int GetTexType()const = 0;
		virtual void SetMipLevel() = 0;
		inline unsigned int GetFormatType()const;
		inline unsigned int GetBytePerPixel()const;
		inline unsigned int GetChannelPerPixel()const;
		unsigned char * GetBuffer(unsigned int uiLevel, unsigned int uiFace = 0)const;
		virtual unsigned int GetByteSize(unsigned int uiLevel)const;
		virtual unsigned int GetMemPitch(unsigned int uiLevel)const;
		virtual unsigned int GetMemSlicePitch(unsigned int uiLevel)const;
		virtual unsigned int GetByteSize()const;
		unsigned int GetWidth(unsigned int uiLevel)const;
		unsigned int GetHeight(unsigned int uiLevel)const;
		unsigned int GetLength(unsigned int uiLevel)const;
		inline unsigned int GetMipLevel()const;
		inline bool GetSRGB()const
		{
			return m_bSRGB;
		}
		inline bool IsCompress()const
		{
			if (m_uiFormatType >= VSRenderer::SFT_BC1 && m_uiFormatType <= VSRenderer::SFT_BC5)
			{
				return true;
			}
			return false;
		}
		void CreateRAMData();
		void CopyRAMData(const VSTexture* pSourceTexture);
		inline bool IsHasAlpha()const
		{
			if(m_uiFormatType == VSRenderer::SFT_A8R8G8B8 ||
				m_uiFormatType == VSRenderer::SFT_A16B16G16R16F || m_uiFormatType == VSRenderer::SFT_A32B32G32R32F
				|| m_uiFormatType == VSRenderer::SFT_BC2 || m_uiFormatType == VSRenderer::SFT_BC3)
			{
				return true;
			}
			return false;
		}
		virtual unsigned int GetArraySize()const
		{
			return m_uiArraySize;
		}


	public:

		void *Lock(unsigned int uiLevel = 0,unsigned int uiFace = 0);
		void UnLock(unsigned int uiLevel = 0,unsigned int uiFace = 0);

		inline void * GetLockDataPtr(unsigned int uiLevel = 0,unsigned int uiFace = 0)const
		{
			VSMAC_ASSERT(uiLevel < GetMipLevel() && uiFace < 6)
			return m_pLockData[uiLevel][uiFace];
		}
		virtual void ClearInfo();
		
		
	protected:
		VSArray<VSArray<unsigned char>> m_DataBufferArray;
		unsigned int m_uiArraySize;
		unsigned int m_uiFormatType;
		unsigned int m_uiWidth;
		unsigned int m_uiHeight;
		unsigned int m_uiLength;
		unsigned int m_uiMipLevel;		
		void * m_pLockData[MAX_MIP_LEVEL][6];
		bool	m_bSRGB;
	protected:
		virtual bool OnLoadResource(VSResourceIdentifier *&pID);		
		virtual bool SetOutput(class VSOutputResource *pOutputResource);
		VSArray<class VSOutputResource *> m_pOutputResource;
		class VSTextureOutputInfo * GetTextureOutputInfo(class VSOutputResource * pOutputResource);
	};
	#include "VSTexture.inl"
	DECLARE_Ptr(VSTexture);
	VSTYPE_MARCO(VSTexture);

	class VSGRAPHIC_API VSTextureCache :public VSCacheResource
	{
	public:
		virtual ~VSTextureCache();
		VSTextureCache(VSObject * pCacheObject = NULL);
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL;
		DECLARE_CACHE_RESOURCE(CacheTexture, Resource/Texture, Cache/Texture, false,false)
		virtual bool SetCacheResource(VSObject* pOwner);
		virtual bool SetStreamResouce(VSResourceProxyBase* pOwner);
		inline const VSTexture* GetTexture() const
		{
			return m_pTex;
		}
	protected:
		VSTexturePtr		m_pTex;
	};
	DECLARE_Ptr(VSTextureCache);
	VSTYPE_MARCO(VSTextureCache);
}