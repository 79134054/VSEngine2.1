#pragma once
#include "VSShaderFunction.h"
namespace VSEngine2
{
	class VSStream;

	class VSGRAPHIC_API VSViewWorldDir : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSViewWorldDir(const VSUsedName & ShowName,VSMaterial * pMaterial);
		virtual ~VSViewWorldDir();
		virtual bool GetFunctionString(VSString &OutString, MaterialShaderPara &MSPara)const;
		virtual void ResetInShaderName(MaterialShaderPara &MSPara);
	protected:
		VSViewWorldDir();
	};
	DECLARE_Ptr(VSViewWorldDir);
	VSTYPE_MARCO(VSViewWorldDir);


	class VSGRAPHIC_API VSCameraWorldPos : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSCameraWorldPos(const VSUsedName & ShowName,VSMaterial * pMaterial);
		virtual ~VSCameraWorldPos();
		virtual bool GetFunctionString(VSString &OutString, MaterialShaderPara &MSPara)const;
		virtual void ResetInShaderName(MaterialShaderPara &MSPara);
	protected:
		VSCameraWorldPos();
	};
	DECLARE_Ptr(VSCameraWorldPos);
	VSTYPE_MARCO(VSCameraWorldPos);

}
