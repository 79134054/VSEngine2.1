#pragma once
#include "VSPostEffectFunction.h"
namespace VSEngine2
{
	COMMON_POST_EFFECT_FUNCTION_DECLARE_BEGIN(Gray)
	COMMON_POST_EFFECT_FUNCTION_DECLARE_END

	COMMON_POST_EFFECT_FUNCTION_DECLARE_BEGIN(GuassBlur)
	COMMON_POST_EFFECT_FUNCTION_DECLARE_END

	COMMON_POST_EFFECT_FUNCTION_DECLARE_BEGIN(Saturate)
		float Saturation = 0.5;
	COMMON_POST_EFFECT_FUNCTION_DECLARE_END

	COMMON_POST_EFFECT_FUNCTION_DECLARE_BEGIN(OldPhoto)
	COMMON_POST_EFFECT_FUNCTION_DECLARE_END

	COMMON_POST_EFFECT_FUNCTION_DECLARE_BEGIN(DownFilter4)
	COMMON_POST_EFFECT_FUNCTION_DECLARE_END

	COMMON_POST_EFFECT_FUNCTION_DECLARE_BEGIN(TexAdd)
	COMMON_POST_EFFECT_FUNCTION_DECLARE_END

	COMMON_POST_EFFECT_FUNCTION_DECLARE_BEGIN(Bloom)
		float MiddleGray = 0.08f;
		float WhiteCutoff = 0.18f;
		float Luminance = 0.8f;
	COMMON_POST_EFFECT_FUNCTION_DECLARE_END
}

