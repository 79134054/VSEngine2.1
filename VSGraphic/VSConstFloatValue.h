#pragma once
#include "VSConstValue.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSConstFloatValue : public VSConstValue
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSConstFloatValue(const VSUsedName & ShowName,VSMaterial * pMaterial,unsigned int uiValueNum,bool bIsCustom);
		virtual ~VSConstFloatValue();
		virtual unsigned int GetType()
		{
			return VSUserConstant::VT_FLOAT;
		}
		void SetValue(unsigned int uiIndex,VSREAL Value);

		virtual bool GetOutputValueString(VSString &OutString, MaterialShaderPara &MSPara)const;
		virtual bool GetFunctionString(VSString &OutString, MaterialShaderPara &MSPara)const;
		virtual unsigned int GetValueNum()const
		{
			return m_Value.GetNum();
		}
		virtual unsigned int GetSize()const
		{
			return m_Value.GetNum() * sizeof(VSREAL);
		}
		bool GetDeclareString(VSString &OutString, unsigned int uiRegisterID, MaterialShaderPara &MSPara)const;
		virtual void ResetInShaderName(MaterialShaderPara &MSPara);
		VSArray<VSREAL>& GetValue() { return m_Value;}
	protected:
		VSConstFloatValue();
		VSArray<VSREAL> m_Value;
	};
	DECLARE_Ptr(VSConstFloatValue);
	VSTYPE_MARCO(VSConstFloatValue);

}
