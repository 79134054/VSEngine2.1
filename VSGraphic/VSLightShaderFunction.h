#pragma once
#include "VSShaderFunction.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSLightColor : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSLightColor(const VSUsedName & ShowName,VSMaterial * pMaterial);
		virtual ~VSLightColor();
		virtual bool GetFunctionString(VSString &OutString, MaterialShaderPara &MSPara)const;
		virtual void ResetInShaderName(MaterialShaderPara &MSPara);
	protected:
		VSLightColor();
	};
	DECLARE_Ptr(VSLightColor);
	VSTYPE_MARCO(VSLightColor);

	class VSGRAPHIC_API VSLightSpecular : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSLightSpecular(const VSUsedName & ShowName,VSMaterial * pMaterial);
		virtual ~VSLightSpecular();
		virtual bool GetFunctionString(VSString &OutString, MaterialShaderPara &MSPara)const;
		virtual void ResetInShaderName(MaterialShaderPara &MSPara);
	protected:
		VSLightSpecular();
	};
	DECLARE_Ptr(VSLightSpecular);
	VSTYPE_MARCO(VSLightSpecular);


	class VSGRAPHIC_API VSLightDir : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSLightDir(const VSUsedName & ShowName,VSMaterial * pMaterial);
		virtual ~VSLightDir();
		virtual bool GetFunctionString(VSString &OutString, MaterialShaderPara &MSPara)const;
		virtual void ResetInShaderName(MaterialShaderPara &MSPara);
	protected:
		VSLightDir();
	};
	DECLARE_Ptr(VSLightDir);
	VSTYPE_MARCO(VSLightDir);


	class VSGRAPHIC_API VSLightShaderFunction : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSLightShaderFunction(const VSUsedName & ShowName,VSMaterial * pMaterial);
		virtual ~VSLightShaderFunction();
		virtual bool GetFunctionString(VSString &OutString, MaterialShaderPara &MSPara)const;
		virtual void ResetInShaderName(MaterialShaderPara &MSPara){};
		virtual bool IsValidNodeToThis(VSShaderFunction * pShaderFunction, MaterialShaderPara &MSPara);


	protected:
		VSLightShaderFunction();
	};
	DECLARE_Ptr(VSLightShaderFunction);
	VSTYPE_MARCO(VSLightShaderFunction);
}
