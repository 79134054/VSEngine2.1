#pragma once
#include "VSShaderFunction.h"
namespace VSEngine2
{
	class VSGRAPHIC_API VSPostEffectShaderFunction : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		enum
		{
			IN_COLOR,
			IN_MAX
		};
		VSPostEffectShaderFunction(const VSUsedName & ShowName, VSMaterial * pMaterial);
		virtual ~VSPostEffectShaderFunction();
		virtual bool GetFunctionString(VSString &OutString, MaterialShaderPara &MSPara)const;
		virtual void ResetInShaderName(MaterialShaderPara &MSPara){};
		virtual bool IsValidNodeToThis(VSShaderFunction * pShaderFunction, MaterialShaderPara &MSPara);

	protected:
		VSPostEffectShaderFunction();
	};
	DECLARE_Ptr(VSPostEffectShaderFunction);
	VSTYPE_MARCO(VSPostEffectShaderFunction);
}
