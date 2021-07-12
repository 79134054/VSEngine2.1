#pragma once
#include "VSTexture.h"
#include "VSResource.h"
#include "VSSamplerState.h"
#include "VSString.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSTexAllState : public VSObject , public VSResource
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;

	public:
		virtual ~VSTexAllState();
		VSTexAllState();
		DECLARE_INITIAL

		static bool InitialDefaultState();
		static bool TerminalDefaultState();

		void SetTexture(VSTexture * pTexture);
		inline VSTexture * GetTexture()const
		{
			return m_pTex;
		}
		virtual unsigned int GetCurStreamLevel()const;
	protected:
		VSTexturePtr		m_pTex;
		VSSamplerDesc		m_SamplerDesc;
		VSSamplerStatePtr	m_pSamplerState;
		bool				m_bSRGB;
		bool				m_bNormal;
		bool				m_bMip;
		unsigned int m_uiArraySize;
		unsigned int m_uiFormatType;
		unsigned int m_uiWidth;
		unsigned int m_uiHeight;
		unsigned int m_uiLength;
		VSArray<unsigned char> m_SourceData;
	protected:
		static VSPointer<VSTexAllState> ms_pOrenNayarLookUpTable;
	public:
		friend class VSResourceManager;
		friend class VSRenderer;
		inline bool GetSRGB()const
		{
			return m_bSRGB;
		}
		inline unsigned int GetWidth()const
		{
			return m_uiWidth;
		}
		inline unsigned int GetHeight()const
		{
			return m_uiHeight;
		}
		inline unsigned int GetLength()const
		{
			return m_uiLength;
		}
		inline unsigned int GetArraySize()const
		{
			return m_uiArraySize;
		}
		inline unsigned int GetFormatType()const
		{
			return m_uiFormatType;
		}
		void SetSRGBEable(bool bEnable);
	
		static const VSTexAllState * GetOrenNayarLookUpTable()
		{
			return ms_pOrenNayarLookUpTable;
		}
		void SetSamplerState(VSSamplerState * pSamplerState)
		{
			if (!pSamplerState)
			{
				pSamplerState = (VSSamplerState *)VSSamplerState::GetDefault();
			}
			if (m_pSamplerState != pSamplerState)
			{
				m_pSamplerState = pSamplerState;
				m_SamplerDesc = pSamplerState->GetSamplerDesc();
			}
		}
		DECLARE_RESOURCE(VSTexAllState, RA_ASYN_LOAD | RA_ASYN_POSTLOAD | RA_ENABLE_GC | RA_NEED_CACHE, RT_TEXTURE,TEXTURE,Resource/Texture, VSTextureCache)
		const VSSamplerState * GetSamplerState()const
		{
			return m_pSamplerState;
		}
		virtual bool BeforeSave(VSStream* pStream);
		virtual bool PostLoad(VSStream* pStream);
		virtual bool PostClone(VSObject * pObjectSrc);
		virtual bool IsCanSave()
		{
			if (!m_SourceData.GetNum())
			{
				return false;
			}
			return true;
		}
		virtual VSObject* CreateToStreamObject(unsigned int uiWantSteamLevel, const VSCacheResource* pCacheResouce)const;
		virtual void StreamEnd(VSObject* pStreamResource);
	};
	DECLARE_Ptr(VSTexAllState);
	DECLARE_Proxy(VSTexAllState);
	VSTYPE_MARCO(VSTexAllState);
}
