#pragma once
#include "VSTexture.h"
namespace VSEngine2
{
	class VSRenderTarget;
	class VSGRAPHIC_API VS2DTexture:public VSTexture
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
	public:
		VS2DTexture(unsigned int uiWidth, unsigned int uiHeight,unsigned int uiFormatType,
					unsigned int uiMipLevel = 0,bool bIsStatic = true, bool bSRGB = false);
		virtual ~VS2DTexture();

		virtual unsigned int GetTexType()const{return TT_2D;}
		virtual void SetMipLevel();
		//bool SaveToFile(TCHAR * pSaveName);

		DECLARE_INITIAL
	public:
		friend class VSRenderer;
		friend class VSTexAllState;
		friend class VSRenderTarget;
		friend class VSDepthStencil;
		friend class VSTextureUnorderAccess;
		class VSOutputResource *GetOutputResource(unsigned int uiMipLevel = 0)const
		{
			return m_pOutputResource[uiMipLevel];
		}
	protected:
		VS2DTexture();
		
	};
	DECLARE_Ptr(VS2DTexture);
	VSTYPE_MARCO(VS2DTexture);
}