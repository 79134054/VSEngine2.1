#pragma once
#include "VSTexAllState.h"
#include "VSViewFamily.h"
#include "VSCubeTexture.h"
namespace VSEngine2
{
	class VSGRAPHIC_API VSCaptureTexAllState : public VSTexAllState
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;

		DECLARE_INITIAL
	public:
		VSCaptureTexAllState();

		virtual ~VSCaptureTexAllState();

		bool SetViewCapture(const VSString & ViewCaptureName);

		virtual bool PostLoad(VSStream* pStream);
		virtual bool PostClone(VSObject * pObjectSrc);
		virtual void ForceUpdate(bool OnlyUpdateOneTime);
		virtual void NotifyEndDraw();
		virtual bool BeforeSave(VSStream* pStream);
		virtual bool PostSave(VSStream* pStream);
		void SetMipLevel(unsigned int uiMipLevel);
		VSCaptureViewFamily * GetViewFamily();
	protected:
		VSString  m_ViewCaptureName;
		bool m_bDynamic;
		VSTexturePtr m_pStaticTexture;
		unsigned int m_uiMipLevel;
		void CreateStaticTexture();
	};
	DECLARE_Ptr(VSCaptureTexAllState);
	VSTYPE_MARCO(VSCaptureTexAllState);
	DECLARE_Proxy(VSCaptureTexAllState);
}