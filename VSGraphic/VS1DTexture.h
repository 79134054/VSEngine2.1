#pragma once
#include "VSTexture.h"
namespace VSEngine2
{
	class VSGRAPHIC_API VS1DTexture:public VSTexture
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VS1DTexture(unsigned int uiWidth,unsigned int uiFormatType,
					unsigned int uiMipLevel = 0,bool bIsStatic = true, bool bSRGB = false);
		virtual ~VS1DTexture();

		virtual unsigned int GetTexType()const{return TT_1D;}
	public:
		friend class VSRenderer;
		friend class VSTexAllState;
		virtual void SetMipLevel();
	protected:
		VS1DTexture();

	};
	VSTYPE_MARCO(VS1DTexture);
	DECLARE_Ptr(VS1DTexture);


}