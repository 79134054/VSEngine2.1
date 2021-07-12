#pragma once
#include "VSOutputResource.h"
#include "VSTextureOutputInfo.h"
#include "VSTexture.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSDepthStencil : public VSOutputResource,public VSTextureOutputInfo
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		friend class VSResourceManager;
		virtual unsigned int GetOutputType()const { return OT_DEPTH_STENCIL; }
		virtual ~VSDepthStencil();
		virtual void *Lock();
		virtual void UnLock();
	protected:
		VSDepthStencil();
		VSDepthStencil(unsigned int uiWidth,unsigned int uiHeight,
			unsigned int uiMulSample,unsigned int uiFormatType, bool CPURead = false);
		VSDepthStencil(class VS2DTexture * pCreateBy, unsigned int uiMulSample = VSRenderer::MS_NONE
			, unsigned int uiLevel = 0, bool CPURead = false);
		VSDepthStencil(class VS2DTextureArray * pCreateBy, unsigned int uiMulSample = VSRenderer::MS_NONE
			, unsigned int uiLevel = 0, unsigned int First = 0, unsigned int uiArraySize = 1, bool CPURead = false);
		VSDepthStencil(class VS3DTexture * pCreateBy, unsigned int uiMulSample = VSRenderer::MS_NONE
			, unsigned int uiLevel = 0, unsigned int First = 0, unsigned int uiArraySize = 1, bool CPURead = false);
		virtual bool OnLoadResource(VSResourceIdentifier *&pID);		

	};
	DECLARE_Ptr(VSDepthStencil);
	VSTYPE_MARCO(VSDepthStencil);
}
