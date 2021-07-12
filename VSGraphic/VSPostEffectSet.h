#pragma once
#include "VSObject.h"
#include "VSResource.h"
#include "VSCuller.h"
namespace VSEngine2
{
	class VSPostEffectFunction;
	class VSPEBeginFunction;
	class VSPEEndFunction;
	class VSScene;
	class VSCamera;
	class VS2DTexture;
	class VSRenderTarget;
	class VSStream;
	class VSGRAPHIC_API VSPostEffectSet : public VSObject , public VSResource
	{
		//RTTI
		DECLARE_RTTI;

		DECLARE_INITIAL
	protected:
		VSPostEffectSet();
	public:
		VSPostEffectSet(const VSUsedName &ShowName);
		virtual ~VSPostEffectSet();

	public:


	
		void AddPostEffectFunction(VSPostEffectFunction * pPostEffectFunction);
		void DeletePostEffectFunction(VSPostEffectFunction * pPostEffectFunction);
		void Draw(VSCuller & Culler,double dAppTime);
		VSRenderTarget * GetEndTarget(unsigned int i);
		void SetBeginTargetArray(VSArray<VSRenderTarget *> * pBeginTargetArray);
		void DisableRT();

		DECLARE_RESOURCE(VSPostEffectSet, RA_ASYN_LOAD | RA_ASYN_POSTLOAD | RA_ENABLE_GC, RT_POSTEFFECT,POSTEFFECT, Resource/MaterialEffect, VSCacheResource)
		VSPostEffectFunction * GetPEFunctionFromShowName(const VSUsedName & ShowName);
		unsigned int GetCurRTWidth()const
		{
			return m_uiCurRTWidth;
		}
		unsigned int GetCurRTHeight()const
		{
			return m_uiCurRTHeight;
		}
	protected:
		VSArray<VSPostEffectFunction *> m_pPostEffectFunctionArray;
		VSPEBeginFunction*  m_pPEBeginFunc;
		VSPEEndFunction * m_pPEEndFunc;
		VSUsedName m_ShowName;
		unsigned int m_uiCurRTWidth = 0;
		unsigned int m_uiCurRTHeight = 0;
	};
	DECLARE_Ptr(VSPostEffectSet);
	VSTYPE_MARCO(VSPostEffectSet);
	DECLARE_Proxy(VSPostEffectSet);
	class VSPEGray;
	class VSGRAPHIC_API VSPESetGray : public VSPostEffectSet
	{
	public:
		VSPESetGray(const VSUsedName &ShowName);
		virtual ~VSPESetGray();
	};
	DECLARE_Ptr(VSPESetGray);
	VSTYPE_MARCO(VSPESetGray);

	DECLARE_Ptr(VSMaterial);
	DECLARE_Proxy(VSMaterial);
	class VSGRAPHIC_API VSPESetMaterialAndGray : public VSPostEffectSet
	{
	public:
		VSPESetMaterialAndGray(const VSUsedName &ShowName,VSMaterialR * pMaterial);
		virtual ~VSPESetMaterialAndGray();
	};
	DECLARE_Ptr(VSPESetMaterialAndGray);
	VSTYPE_MARCO(VSPESetMaterialAndGray);

	class VSGRAPHIC_API VSPESetMaterial_Gray_Bloom : public VSPostEffectSet
	{
	public:
		VSPESetMaterial_Gray_Bloom(const VSUsedName &ShowName, VSMaterialR * pMaterial);
		virtual ~VSPESetMaterial_Gray_Bloom();
	};
	DECLARE_Ptr(VSPESetMaterial_Gray_Bloom);
	VSTYPE_MARCO(VSPESetMaterial_Gray_Bloom);
}
