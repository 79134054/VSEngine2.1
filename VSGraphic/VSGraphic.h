#pragma once
#ifdef VSGRAPHIC_EXPORTS
#define  VSGRAPHIC_API __declspec(dllexport) 
#else
#define  VSGRAPHIC_API __declspec(dllimport)
#endif
#define PROFILER
#include "VSMath.h"
namespace VSEngine2
{
#define STREAM_LEVEL 16 
#define RENDERTARGET_LEVEL 8
#define UNORDERACCESSS_LEVEL 8
#define POSTEFFECT_MAX_INPUT 4
#define UV_LEVEL 4
#define MAX_DYNAMIC_LIGHT 3
#define INVALID_SIMPLAR_REGISTER 16 
#define MAX_SIMPLE_INSTANCE_ANIM 3
#define MAX_MIP_LEVEL 20
	class VSGRAPHIC_API VSEngineFlag
	{
	public:
		static bool IsRunEditor;
		enum //Sampler Source
		{
			SS_FROM_TEXTURE,
			SS_SHARE_3L_C,
			SS_SHARE_3L_W,
 			SS_SHARE_2L_B1,
 			SS_SHARE_2L_B0,
			SS_MAX
		};
		enum // Shader Type
		{
			ST_VERTEX,
			ST_PIXEL,
			ST_GEOMETRY,
			ST_HULL,
			ST_DOMAIN,
			ST_COMPUTE,
			ST_MAX
		};
		static const unsigned int ms_uiShaderSamplerNum = SS_MAX - 1;
	};
	struct ANIM_INSTANCE_DATA
	{
		unsigned int AnimIndex = 0;
		VSREAL fTime = 0.0f;
		VSREAL fAlpha = 0.0f;
	};
	struct ANIM_INSTANCE_SHADER_DATA
	{
		VSREAL AnimIndex = 0;
		VSREAL fAlpha = 0.0f;
	};
}
