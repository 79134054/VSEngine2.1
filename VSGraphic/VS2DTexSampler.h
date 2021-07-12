#pragma once
#include "VSTexSampler.h"
#include "VSViewFamily.h"
namespace VSEngine2
{
	class VSGRAPHIC_API VS2DTexSampler : public VSTexSampler
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
		
	public:
		VS2DTexSampler(const VSUsedName & ShowName,VSMaterial * pMaterial);
		virtual ~VS2DTexSampler();
		virtual void SetTexture(VSTexAllStateR * pTexAllState);
		
		
		virtual unsigned int GetTexType()const
		{
			return VSTexture::TT_2D;
		}

		virtual bool GetFunctionString(VSString &OutString, MaterialShaderPara &MSPara)const;
		virtual void ResetInShaderName(MaterialShaderPara &MSPara);
		virtual bool GetOutputValueString(VSString &OutString, MaterialShaderPara &MSPara)const;
	protected:
		VS2DTexSampler();
	};
	DECLARE_Ptr(VS2DTexSampler);
	VSTYPE_MARCO(VS2DTexSampler);
}
