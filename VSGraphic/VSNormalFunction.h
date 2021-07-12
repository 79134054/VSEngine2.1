#pragma once
#include "VSShaderFunction.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSWorldNormal : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSWorldNormal(const VSUsedName & ShowName,VSMaterial * pMaterial);
		virtual ~VSWorldNormal();
		virtual bool GetFunctionString(VSString &OutString, MaterialShaderPara &MSPara)const;
		virtual void ResetInShaderName(MaterialShaderPara &MSPara);
		virtual bool GetOutputValueString(VSString &OutString, MaterialShaderPara &MSPara)const;
	protected:
		VSWorldNormal();
	public:
		enum
		{
			WNT_PIXEL,
			WNT_VERTEX,
			WNT_MAX
		};
		unsigned int m_uiNormalType;
	};
	DECLARE_Ptr(VSWorldNormal);
	VSTYPE_MARCO(VSWorldNormal);


	class VSGRAPHIC_API VSViewNormal : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSViewNormal(const VSUsedName & ShowName,VSMaterial * pMaterial);
		virtual ~VSViewNormal();
		virtual bool GetFunctionString(VSString &OutString, MaterialShaderPara &MSPara)const;
		virtual void ResetInShaderName(MaterialShaderPara &MSPara);
		virtual bool GetOutputValueString(VSString &OutString, MaterialShaderPara &MSPara)const;
	protected:
		VSViewNormal();


	public:
		enum
		{
			VNT_PIXEL,
			VNT_VERTEX,
			VNT_MAX
		};
		unsigned int m_uiNormalType;
	};
	DECLARE_Ptr(VSViewNormal);
	VSTYPE_MARCO(VSViewNormal);
}
