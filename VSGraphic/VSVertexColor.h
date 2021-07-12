#pragma once
#include "VSShaderFunction.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSVertexColor : public VSShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		VSVertexColor(const VSUsedName & ShowName,VSMaterial * pMaterial);
		virtual ~VSVertexColor();
		virtual bool GetFunctionString(VSString &OutString, MaterialShaderPara &MSPara)const;
		virtual void ResetInShaderName(MaterialShaderPara &MSPara);
	protected:
		VSVertexColor();
		unsigned int m_uiColorIndex;

	public:
		enum //Color Index
		{
			CI_1,
			CI_2,
			CI_MAX
		};
		inline void SetColorIndex(unsigned int uiIndex)
		{
			if (uiIndex >= CI_MAX)
			{
				return;
			}
			m_uiColorIndex = uiIndex;
		}
	};
	DECLARE_Ptr(VSVertexColor);
	VSTYPE_MARCO(VSVertexColor);
}
