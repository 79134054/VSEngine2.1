#pragma once
#include "VSTexture.h"
namespace VSEngine2
{
	class VSGRAPHIC_API VS3DTexture:public VSTexture
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VS3DTexture(unsigned int uiWidth, unsigned int uiHeight,unsigned int uiLength,
			unsigned int uiFormatType,unsigned int uiMipLevel = 0,bool bIsStatic = true, bool bSRGB = false);
		virtual ~VS3DTexture();

		virtual unsigned int GetTexType()const{return TT_3D;}
		virtual void SetMipLevel();
		friend class VSRenderTarget;
		friend class VSDepthStencil;
		friend class VSTextureUnorderAccess;
	protected:
		VS3DTexture();
		virtual bool SetOutput(class VSOutputResource *pOutputResource);
	};
	DECLARE_Ptr(VS3DTexture);
	VSTYPE_MARCO(VS3DTexture);
	
}