#pragma once
#include "VSShaderMainFunction.h"
namespace VSEngine2
{
	// good for plastics
	class VSGRAPHIC_API VSPhoneShaderFunction : public VSShaderMainFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		
	public:
		enum
		{
			IN_DIFFUSE_COLOR,
			IN_EMISSIVE_COLOR,
			IN_SPECULAR_COLOR,
			IN_SPECULAR_POW,
			IN_NORMAL,
			IN_ALPHA,
			IN_REFLECT_MIP,
			IN_REFLECT_POW,
			IN_WORLD_OFFSET,
			IN_TESSELLATION_VALUE,
			IN_WORLD_DISPLACEMENT,
			IN_MAX
		};
		enum //Specular Type
		{
			ST_BlinnPhong,
			ST_Phong,
			ST_MAX
		};
		VSPhoneShaderFunction(const VSUsedName & ShowName,VSMaterial * pMaterial);
		virtual ~VSPhoneShaderFunction();
		virtual bool GetFunctionString(VSString &OutString, MaterialShaderPara &MSPara)const;
		virtual inline unsigned int GetSMType()const
		{
			return SM_PHONE;
		}
		inline void SetSpecularType(unsigned char uiSpecularType)
		{
			m_uiSpecularType = uiSpecularType;
		}
		inline unsigned char GetSpecularType()const
		{
			return m_uiSpecularType;
		}
		virtual VSInputNode * GetDiffuseNode()const
		{
			return GetInputNode(IN_DIFFUSE_COLOR);
		}
		virtual VSInputNode * GetNormalNode()const
		{
			return GetInputNode(IN_NORMAL);
		}
		virtual VSInputNode * GetWorldOffsetNode()const
		{
			return GetInputNode(IN_WORLD_OFFSET);
		}
		virtual VSInputNode * GetTessellationValueNode()const
		{
			return GetInputNode(IN_TESSELLATION_VALUE);
		}
		virtual VSInputNode * GetWorldDisplacementNode()const
		{
			return GetInputNode(IN_WORLD_DISPLACEMENT);
		}
		virtual VSInputNode * GetAlphaNode()const
		{
			return GetInputNode(IN_ALPHA);
		}
		virtual VSInputNode * GetEmissiveNode()const
		{
			return GetInputNode(IN_EMISSIVE_COLOR);
		}
		virtual VSInputNode * GetReflectMipNode()const
		{
			return GetInputNode(IN_REFLECT_MIP);
		}
		virtual VSInputNode * GetReflectPowNode()const
		{
			return GetInputNode(IN_REFLECT_POW);
		}

	protected:
		VSPhoneShaderFunction();
		unsigned int m_uiSpecularType;
		DECLARE_INITIAL
	public:
		virtual bool PostLoad(VSStream* pStream);
	};
	DECLARE_Ptr(VSPhoneShaderFunction);
	VSTYPE_MARCO(VSPhoneShaderFunction);
}
