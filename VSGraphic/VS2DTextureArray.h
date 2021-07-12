#pragma once
#include "VSTexture.h"
#include "VS2DTexture.h"
namespace VSEngine2
{
	class VSRenderTarget;
	class VSGRAPHIC_API VS2DTextureArray : public VSTexture
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VS2DTextureArray(unsigned int uiWidth, unsigned int uiHeight, unsigned int uiFormatType,
			unsigned int uiMipLevel, unsigned int uiArraySize,bool bIsStatic, bool bSRGB = false);
		VS2DTextureArray(VSArray<VS2DTexture *>& pTextureArray);
		virtual ~VS2DTextureArray();

		virtual unsigned int GetTexType()const { return TT_2DARRAY; }

		virtual void SetMipLevel();
		friend class VSRenderTarget;
		friend class VSDepthStencil;
		friend class VSTextureUnorderAccess;
		class VSOutputResource *GetOutputResource(unsigned int uiIndex)const
		{
			return m_pOutputResource[uiIndex];
		}
		virtual unsigned int GetArraySize()const
		{
			return m_uiArraySize;
		}
	protected:
		VS2DTextureArray();	
		virtual bool SetOutput(class VSOutputResource *pOutputResource);

	};
	DECLARE_Ptr(VS2DTextureArray);
	VSTYPE_MARCO(VS2DTextureArray);
}
