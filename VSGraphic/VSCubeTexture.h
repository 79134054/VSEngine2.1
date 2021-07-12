#pragma once
#include "VSTexture.h"
#include "VS2DTextureArray.h"
namespace VSEngine2
{
	class VSRenderTarget;
	class VSGRAPHIC_API VSCubeTexture : public VS2DTextureArray
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		enum	//FACE
		{
			F_RIGHT,
			F_LEFT,	
			F_TOP,
			F_BOTTOM,
			F_FRONT,
			F_BACK,				
			F_MAX
		};
		VSCubeTexture(unsigned int uiWidth,unsigned int uiFormatType,
					unsigned int uiMipLevel = 0,bool bIsStatic = true, bool bSRGB = false);
		virtual ~VSCubeTexture();
		virtual unsigned int GetTexType()const { return TT_CUBE; }
	protected:
		VSCubeTexture();

	};
	DECLARE_Ptr(VSCubeTexture);
	VSTYPE_MARCO(VSCubeTexture);
}