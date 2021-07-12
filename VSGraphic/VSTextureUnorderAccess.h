#pragma once
#include "VSUnorderAccess.h"
#include "VSTextureOutputInfo.h"
#include "VSTexture.h"
namespace VSEngine2
{
	class VSGRAPHIC_API VSTextureUnorderAccess :public VSUnorderAccess, public VSTextureOutputInfo
	{

		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL

	public:
		friend class VSResourceManager;
		virtual ~VSTextureUnorderAccess();
		virtual unsigned int GetOutputType()const { return OT_TEXTURE_UNORDER_ACCESS; }
		virtual void *Lock();
		virtual void UnLock();
	protected:
		VSTextureUnorderAccess(unsigned int uiWidth, unsigned int uiHeight, unsigned int uiFormatType,
			bool CPURead = false);
		VSTextureUnorderAccess(class VS2DTexture * pCreateBy,unsigned int uiLevel = 0, bool CPURead = false);
		VSTextureUnorderAccess(class VS2DTextureArray * pCreateBy,unsigned int uiLevel = 0, 
			unsigned int First = 0, unsigned int uiArraySize = 1, bool CPURead = false);
		VSTextureUnorderAccess(class VS3DTexture * pCreateBy,unsigned int uiLevel = 0, 
			unsigned int First = 0, unsigned int uiArraySize = 1, bool CPURead = false);
		VSTextureUnorderAccess();

		virtual bool OnLoadResource(VSResourceIdentifier *&pID);
	};
	DECLARE_Ptr(VSTextureUnorderAccess);
	VSTYPE_MARCO(VSTextureUnorderAccess);
}

