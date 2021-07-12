#pragma once
namespace VectorRegisterConstants
{
	static const VSVectorRegister FloatOne = MakeVectorRegister(1.0f, 1.0f, 1.0f, 1.0f);
	static const VSVectorRegister FloatZero = MakeVectorRegister(0.0f, 0.0f, 0.0f, 0.0f);
	static const VSVectorRegister FloatMinusOne = MakeVectorRegister(-1.0f, -1.0f, -1.0f, -1.0f);
	static const VSVectorRegister Float0001 = MakeVectorRegister(0.0f, 0.0f, 0.0f, 1.0f);
	static const VSVectorRegister SmallLengthThreshold = MakeVectorRegister(1.e-8f, 1.e-8f, 1.e-8f, 1.e-8f);
	static const VSVectorRegister FloatOneHundredth = MakeVectorRegister(0.01f, 0.01f, 0.01f, 0.01f);
	static const VSVectorRegister Float111_Minus1 = MakeVectorRegister(1.f, 1.f, 1.f, -1.f);
	static const VSVectorRegister FloatMinus1_111 = MakeVectorRegister(-1.f, 1.f, 1.f, 1.f);
	static const VSVectorRegister FloatOneHalf = MakeVectorRegister(0.5f, 0.5f, 0.5f, 0.5f);
	static const VSVectorRegister FloatMinusOneHalf = MakeVectorRegister(-0.5f, -0.5f, -0.5f, -0.5f);
	static const VSVectorRegister KindaSmallNumber = MakeVectorRegister(EPSILON_E8, EPSILON_E8, EPSILON_E8, EPSILON_E8);
	static const VSVectorRegister SmallNumber = MakeVectorRegister(EPSILON_E4, EPSILON_E4, EPSILON_E4, EPSILON_E4);
	static const VSVectorRegister ThreshQuatNormalized = MakeVectorRegister(0.01f, 0.01f, 0.01f, 0.01f);
	static const VSVectorRegister BigNumber = MakeVectorRegister(VSMAX_REAL, VSMAX_REAL, VSMAX_REAL, VSMAX_REAL);

	static const VSVectorRegisterInt IntOne = MakeVectorRegisterInt(1, 1, 1, 1);
	static const VSVectorRegisterInt IntZero = MakeVectorRegisterInt(0, 0, 0, 0);
	static const VSVectorRegisterInt IntMinusOne = MakeVectorRegisterInt(-1, -1, -1, -1);

	/** This is to speed up Quaternion Inverse. Static variable to keep sign of inverse **/
	static const VSVectorRegister QINV_SIGN_MASK = MakeVectorRegister(-1.f, -1.f, -1.f, 1.f);

	static const VSVectorRegister QMULTI_SIGN_MASK0 = MakeVectorRegister(1.f, -1.f, 1.f, -1.f);
	static const VSVectorRegister QMULTI_SIGN_MASK1 = MakeVectorRegister(1.f, 1.f, -1.f, -1.f);
	static const VSVectorRegister QMULTI_SIGN_MASK2 = MakeVectorRegister(-1.f, 1.f, 1.f, -1.f);

	static const VSVectorRegister DEG_TO_RAD = MakeVectorRegister(VSPI / (180.f), VSPI / (180.f), VSPI / (180.f), VSPI / (180.f));
	static const VSVectorRegister DEG_TO_RAD_HALF = MakeVectorRegister((VSPI / 180.f)*0.5f, (VSPI / 180.f)*0.5f, (VSPI / 180.f)*0.5f, (VSPI / 180.f)*0.5f);
	static const VSVectorRegister RAD_TO_DEG = MakeVectorRegister((180.f) / VSPI, (180.f) / VSPI, (180.f) / VSPI, (180.f) / VSPI);

	/** Bitmask to AND out the XYZ components in a vector */
	static const VSVectorRegister XYZMask = MakeVectorRegister((unsigned int)0xffffffff, (unsigned int)0xffffffff, (unsigned int)0xffffffff, (unsigned int)0x00000000);

	/** Bitmask to AND out the sign bit of each components in a vector */
#define SIGN_BIT ((1 << 31))
	static const VSVectorRegister SignBit = MakeVectorRegister((unsigned int)SIGN_BIT, (unsigned int)SIGN_BIT, (unsigned int)SIGN_BIT, (unsigned int)SIGN_BIT);
	static const VSVectorRegister SignMask = MakeVectorRegister((unsigned int)(~SIGN_BIT), (unsigned int)(~SIGN_BIT), (unsigned int)(~SIGN_BIT), (unsigned int)(~SIGN_BIT));
	static const VSVectorRegisterInt IntSignBit = MakeVectorRegisterInt(SIGN_BIT, SIGN_BIT, SIGN_BIT, SIGN_BIT);
	static const VSVectorRegisterInt IntSignMask = MakeVectorRegisterInt((~SIGN_BIT), (~SIGN_BIT), (~SIGN_BIT), (~SIGN_BIT));
#undef SIGN_BIT
	static const VSVectorRegister AllMask = MakeVectorRegister(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
	static const VSVectorRegisterInt IntAllMask = MakeVectorRegisterInt(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);

	/** Vector full of positive infinity */
	static const VSVectorRegister FloatInfinity = MakeVectorRegister((unsigned int)0x7F800000, (unsigned int)0x7F800000, (unsigned int)0x7F800000, (unsigned int)0x7F800000);


	static const VSVectorRegister Pi = MakeVectorRegister(VSPI, VSPI, VSPI, VSPI);
	static const VSVectorRegister TwoPi = MakeVectorRegister(2.0f*VSPI, 2.0f*VSPI, 2.0f*VSPI, 2.0f*VSPI);
	static const VSVectorRegister PiByTwo = MakeVectorRegister(0.5f*VSPI, 0.5f*VSPI, 0.5f*VSPI, 0.5f*VSPI);
	static const VSVectorRegister PiByFour = MakeVectorRegister(0.25f*VSPI, 0.25f*VSPI, 0.25f*VSPI, 0.25f*VSPI);
	static const VSVectorRegister OneOverPi = MakeVectorRegister(1.0f / VSPI, 1.0f / VSPI, 1.0f / VSPI, 1.0f / VSPI);
	static const VSVectorRegister OneOverTwoPi = MakeVectorRegister(1.0f / (2.0f*VSPI), 1.0f / (2.0f*VSPI), 1.0f / (2.0f*VSPI), 1.0f / (2.0f*VSPI));

	static const VSVectorRegister Float255 = MakeVectorRegister(255.0f, 255.0f, 255.0f, 255.0f);
	static const VSVectorRegister Float127 = MakeVectorRegister(127.0f, 127.0f, 127.0f, 127.0f);
	static const VSVectorRegister FloatNeg127 = MakeVectorRegister(-127.0f, -127.0f, -127.0f, -127.0f);
	static const VSVectorRegister Float360 = MakeVectorRegister(360.f, 360.f, 360.f, 360.f);
	static const VSVectorRegister Float180 = MakeVectorRegister(180.f, 180.f, 180.f, 180.f);

	// All float numbers greater than or equal to this have no fractional value.
	static const VSVectorRegister FloatNonFractional = MakeVectorRegister(8388608.f, 8388608.f, 8388608.f, 8388608.f);

	static const VSVectorRegister FloatTwo = MakeVectorRegister(2.0f, 2.0f, 2.0f, 2.0f);
	static const unsigned int AlmostTwoBits = 0x3fffffff;
	static const VSVectorRegister FloatAlmostTwo = MakeVectorRegister(*(float*)&AlmostTwoBits, *(float*)&AlmostTwoBits, *(float*)&AlmostTwoBits, *(float*)&AlmostTwoBits);
}

