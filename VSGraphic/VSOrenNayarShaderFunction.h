#pragma once
#include "VSShaderMainFunction.h"
namespace VSEngine2
{
	// good for rough surfaces , no specular
	class VSGRAPHIC_API VSOrenNayarShaderFunction : public VSShaderMainFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		
	public:
		enum
		{
			IN_DIFFUSE_COLOR,
			IN_EMISSIVE_COLOR,
			IN_ROUGHNESS_SQUARED,
			IN_NORMAL,
			IN_ALPHA,
			IN_REFLECT_MIP,
			IN_REFLECT_POW,
			IN_WORLD_OFFSET,
			IN_TESSELLATION_VALUE,
			IN_WORLD_DISPLACEMENT,
			IN_MAX
		};
		VSOrenNayarShaderFunction(const VSUsedName & ShowName,VSMaterial * pMaterial);
		virtual ~VSOrenNayarShaderFunction();
		virtual bool GetFunctionString(VSString &OutString, MaterialShaderPara &MSPara)const;
		virtual inline unsigned int GetSMType()const
		{
			return SM_OREN_NAYAR;
		}
		bool UseLookUpTable()const
		{
			return true;
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

		virtual void SetGlobleValue(MaterialShaderPara &MSPara, VSVShader * pVShader, VSPShader * pPShader,
			VSGShader * pGShader, VSHShader * pHShader, VSDShader * pDShader);
	protected:
		VSOrenNayarShaderFunction();
		DECLARE_INITIAL
	public:
		virtual bool PostLoad(VSStream* pStream);
	};
	DECLARE_Ptr(VSOrenNayarShaderFunction);
	VSTYPE_MARCO(VSOrenNayarShaderFunction);
}
