#pragma once
#include "VSObject.h"
#include "VSTexAllState.h"
#include "VS2DTexture.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSFont : public VSObject, public VSResource
	{
		//PRIORITY
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		enum
		{
			ASCII_START = 32,
			ASCII_END = 126,
			ASCII_NUM = ASCII_END - ASCII_START + 1
		};
		VSFont(const TCHAR* acFace, unsigned int uiSize, bool bBold,
				bool bItalic);
		virtual ~VSFont();
	protected:
		VSFont();
	public:


		static bool InitialDefaultState();
		static bool TerminalDefaultState();

	public:
		friend class VSRenderer;

		inline const VSString & GetFace()const {return m_Face;}
		inline unsigned int GetSize()const {return m_uiSize;}
		inline bool GetBold()const {return m_bBold;}
		inline bool GetItalic()const {return m_bItalic;}
		inline bool IsValid()const
		{
			return m_pFontTex != NULL && !m_pCharacterData && m_pFontTexAllState != NULL;
		}
		void SetFontData(VS2DTexture * pTexture,VSREAL * pCharacterData);
		inline VSREAL GetFontData(unsigned char Index)const
		{
			return m_pCharacterData[Index];
		}
		VSTexAllState * GetTexAllState()const
		{
			return m_pFontTexAllState;
		}
		DECLARE_RESOURCE(VSFont, RA_ASYN_LOAD | RA_ASYN_POSTLOAD | RA_ENABLE_GC, RT_FONTTYPE,FONT, Resource/Font, VSCacheResource)
		virtual bool PostLoad(VSStream* pStream);
		virtual bool PostClone(VSObject * pObjectSrc);
	protected:
		VSString m_Face;
		unsigned int m_uiSize;
		bool m_bBold;
		bool m_bItalic;
		VS2DTexturePtr m_pFontTex;
		VSREAL *m_pCharacterData;
		VSTexAllStatePtr m_pFontTexAllState;
	};
	DECLARE_Ptr(VSFont);
	DECLARE_Proxy(VSFont);
	VSTYPE_MARCO(VSFont);


}
