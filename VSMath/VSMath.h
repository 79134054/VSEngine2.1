#pragma once
#include "VSSystem.h"
#include <math.h>  
#include "VSMemManager.h"
#ifdef VSMATH_EXPORTS
	#define  VSMATH_API __declspec(dllexport) 
#else
	#define  VSMATH_API __declspec(dllimport)
#endif
namespace VSEngine2
{
#define IABS(x)		abs(x)
#define LABS(x)		labs(x)
#define LLABS(x)	llabs(x)
#define  VSFLOAT 
#ifdef VSFLOAT 
#define VSREAL 		float
#define SIN(x)		sinf(x)
#define COS(x)		cosf(x)
#define TAN(x)		tanf(x)
#define ASIN(x)		asinf(x)
#define ACOS(x)		acosf(x)
#define ATAN(x)		atanf(x)
#define ATAN2(y,x)	atan2f(y,x)
#define SQRT(x)		sqrtf(x)
#define POW(x,y)	powf(x,y)
#define CEIL(x)		ceilf(x)
#define EXP(x)		expf(x)
#define EXP2(x)		POW(2.0f,x)
#define FMOD(x,y)	fmodf(x,y)
#define	FLOOR(x)	floorf(x)
#define	LOG(x)		logf(x)
#define	LOG2(x)		LOG(x) * 1.4426950f
#define FABS(x)		fabsf(x)
#else ifdef VSDOUBLE
#define VSREAL 		double
#define SIN(x)		sin(x)
#define COS(x)		cos(x)
#define TAN(x)		tan(x)
#define ASIN(x)		asin(x)
#define ACOS(x)		acos(x)
#define ATAN(x) 	atan(x)
#define ATAN2(y,x) 	atan2(y,x)
#define SQRT(x)		sqrt(x)
#define POW(x,y)	pow(x,y)
#define CEIL(x)		ceil(x);
#define EXP(x)		exp(x);
#define EXP2(x)		POW(2.0f,x)
#define FMOD(x,y)	fmod(x,y)
#define	FLOOR(x)	floor(x)
#define	LOG(x)		log(x)
#define	LOG2(x)		LOG(x) * 1.4426950f
#define FABS(x)		fabs(x)
#endif

#define EPSILON_E3 (VSREAL)(1E-3) 
#define EPSILON_E4 (VSREAL)(1E-4) 
#define EPSILON_E5 (VSREAL)(1E-5)
#define EPSILON_E6 (VSREAL)(1E-6)
#define EPSILON_E8 (VSREAL)(1E-8)

#define VSFRONT			0
#define VSBACK			1
#define VSON			2

#define VSCLIPPED		3
#define VSCULLED		4
#define VSVISIBLE		5

#define VSINTERSECT		3
#define VSOUT			4
#define VSIN			5

#define VSNOINTERSECT	6

#define VSMIN_CHAR		(-128)      
#define VSMAX_CHAR		127      
#define VSMIN_INT32		(-2147483647 - 1) 
#define VSMAX_INT32     2147483647    
#define VSMAX_REAL		3.402823466e+38F
#define VSMIN_REAL		1.175494351e-38F
#define VSMAX_UINT32	0xffffffff
#define VSREAL_1		0.99999f		
#define VSPI			3.1415926535897932f
#define VSPI2			1.57079632679f
#define VS2PI			6.2831853071795864f
#define VSINV_PI		0.31830988618f
#define VSFASTASIN_VSPI2	1.5707963050f

#define VSSHADOWON		0
#define VSSHADOWBACK	1
#define VSNOSHADOW		2


#define VSUSHORT_INDEX	unsigned short
#define VSUINT_INDEX	unsigned int

template<> inline VSREAL ABS(VSREAL t)
{
	return FABS(t);
}

template<> inline int ABS(int t)
{
	return IABS(t);
}
template<> inline long ABS(long t)
{
	return LABS(t);
}
template<> inline long long ABS(long long t)
{
	return LLABS(t);
}
inline int VSstoi(const TCHAR * p)
{
	return _tstoi(p);
}
inline VSREAL VSstof(const TCHAR * p)
{
	return (VSREAL)_tstof(p);
}
inline VSREAL FastAtan2(VSREAL Y, VSREAL X)
{
	//return atan2f(Y,X);
	// atan2f occasionally returns NaN with perfectly valid input (possibly due to a compiler or library bug).
	// We are replacing it with a minimax approximation with a max relative error of 7.15255737e-007 compared to the C library function.
	// On PC this has been measured to be 2x faster than the std C version.

	const VSREAL absX = FABS(X);
	const VSREAL absY = FABS(Y);
	const bool yAbsBigger = (absY > absX);
	VSREAL t0 = yAbsBigger ? absY : absX; // Max(absY, absX)
	VSREAL t1 = yAbsBigger ? absX : absY; // Min(absX, absY)

	if (t0 == 0.f)
		return 0.f;

	VSREAL t3 = t1 / t0;
	VSREAL t4 = t3 * t3;

	static const VSREAL c[7] = {
		+7.2128853633444123e-03f,
		-3.5059680836411644e-02f,
		+8.1675882859940430e-02f,
		-1.3374657325451267e-01f,
		+1.9856563505717162e-01f,
		-3.3324998579202170e-01f,
		+1.0f
	};

	t0 = c[0];
	t0 = t0 * t4 + c[1];
	t0 = t0 * t4 + c[2];
	t0 = t0 * t4 + c[3];
	t0 = t0 * t4 + c[4];
	t0 = t0 * t4 + c[5];
	t0 = t0 * t4 + c[6];
	t3 = t0 * t3;

	t3 = yAbsBigger ? (0.5f * VSPI) - t3 : t3;
	t3 = (X < 0.0f) ? VSPI - t3 : t3;
	t3 = (Y < 0.0f) ? -t3 : t3;

	return t3;
}

inline VSREAL Fmod(VSREAL X, VSREAL Y)
{
	if (FABS(Y) <= EPSILON_E8)
	{
		return 0.f;
	}
	const float Div = (X / Y);
	// All floats where abs(f) >= 2^23 (8388608) are whole numbers so do not need truncation, and avoid overflow in TruncToFloat as they get even larger.
	const float Quotient = FABS(Div) < 8388608.f ? int(Div) : Div;
	float IntPortion = Y * Quotient;

	// Rounding and imprecision could cause IntPortion to exceed X and cause the result to be outside the expected range.
	// For example Fmod(55.8, 9.3) would result in a very small negative value!
	if (FABS(IntPortion) > FABS(X))
	{
		IntPortion = X;
	}

	const float Result = X - IntPortion;
	return Result;
}
/** Return true if value is NaN (not a number). */
inline bool IsNaN(VSREAL A)
{
	return ((*(unsigned int*)&A) & 0x7FFFFFFF) > 0x7F800000;
}
/** Return true if value is finite (not NaN and not Infinity). */
inline bool IsFinite(VSREAL A)
{
	return ((*(unsigned int*)&A) & 0x7F800000) != 0x7F800000;
}
inline bool IsNegativeFloat(const VSREAL& A)
{
	return ((*(unsigned int*)&A) >= (unsigned int)0x80000000); // Detects sign bit.
}
inline void SinCos(float* ScalarSin, float* ScalarCos, float  Value)
{
	// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
	VSREAL quotient = (VSINV_PI*0.5f)*Value;
	if (Value >= 0.0f)
	{
		quotient = (VSREAL)((int)(quotient + 0.5f));
	}
	else
	{
		quotient = (VSREAL)((int)(quotient - 0.5f));
	}
	VSREAL y = Value - (2.0f*VSPI)*quotient;

	// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
	VSREAL sign;
	if (y > VSPI2)
	{
		y = VSPI - y;
		sign = -1.0f;
	}
	else if (y < -VSPI2)
	{
		y = -VSPI - y;
		sign = -1.0f;
	}
	else
	{
		sign = +1.0f;
	}

	VSREAL y2 = y * y;

	// 11-degree minimax approximation
	*ScalarSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

	// 10-degree minimax approximation
	VSREAL p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
	*ScalarCos = sign * p;
}


// Note:  We use VSFASTASIN_VSPI2 instead of HALF_PI inside of FastASin(), since it was the value that accompanied the minimax coefficients below.
// It is important to use exactly the same value in all places inside this function to ensure that FastASin(0.0f) == 0.0f.
// For comparison:
//		VSPI2				== 1.57079632679f == 0x3fC90FDB
//		VSHALF_PI3			== 1.5707963050f  == 0x3fC90FDA

/**
* Computes the ASin of a scalar value.
*
* @param Value  input angle
* @return ASin of Value
*/
inline VSREAL FastAsin(float Value)
{
	// Clamp input to [-1,1].
	bool nonnegative = (Value >= 0.0f);
	VSREAL x = ABS(Value);
	VSREAL omx = 1.0f - x;
	if (omx < 0.0f)
	{
		omx = 0.0f;
	}
	VSREAL root = SQRT(omx);
	// 7-degree minimax approximation
	VSREAL result = ((((((-0.0012624911f * x + 0.0066700901f) * x - 0.0170881256f) * x + 0.0308918810f) * x - 0.0501743046f) * x + 0.0889789874f) * x - 0.2145988016f) * x + VSFASTASIN_VSPI2;
	result *= root;  // acos(|x|)
	// acos(x) = pi - acos(-x) when x < 0, asin(x) = pi/2 - acos(x)
	return (nonnegative ? VSFASTASIN_VSPI2 - result : result - VSFASTASIN_VSPI2);
}
#define VS_SSE
#ifdef VS_SSE
	#include "VSPC_SSE.h"
#endif
class VSMATH_API VSMathInstance
{
public:
	VSMathInstance();
	~VSMathInstance();
	VSREAL GetFastSin(unsigned int i);
	VSREAL GetFastCos(unsigned int i);
	unsigned int CRC32Compute(const void *pData, unsigned int uiDataSize);
	static VSMathInstance & GetMathInstance()
	{
		static VSMathInstance g_MathInitial;
		return g_MathInitial;
	}
protected:
	void VSInitCRCTable();
	VSREAL FastSin[361];
	VSREAL FastCos[361];
	unsigned int CRCTable[256];
};
inline unsigned int CalcAlign(unsigned int n, unsigned int align)
{
	return ((n + align - 1) & (~(align - 1)));
}
inline VSREAL RadianToAngle(VSREAL Radian)
{
	return 57.29578f * Radian;
	//return ( Radian * 180.0f ) / VSPI ;
}
inline VSREAL AngleToRadian(VSREAL Angle)
{
	//return ( Angle * VSPI ) /  180.0f;
	return 0.01745329f * Angle;
}
inline bool IsTwoPower(unsigned int uiN)
{
	return !(uiN & (uiN - 1));
}
inline unsigned short FloatToHalf(VSREAL Value)
{
	unsigned int Result;

	unsigned int IValue = ((unsigned int *)(&Value))[0];
	unsigned int Sign = (IValue & 0x80000000U) >> 16U;
	IValue = IValue & 0x7FFFFFFFU;      // Hack off the sign

	if (IValue > 0x47FFEFFFU)
	{
		// The number is too large to be represented as a half.  Saturate to infinity.
		Result = 0x7FFFU;
	}
	else
	{
		if (IValue < 0x38800000U)
		{
			// The number is too small to be represented as a normalized half.
			// Convert it to a denormalized value.
			unsigned int Shift = 113U - (IValue >> 23U);
			IValue = (0x800000U | (IValue & 0x7FFFFFU)) >> Shift;
		}
		else
		{
			// Rebias the exponent to represent the value as a normalized half.
			IValue += 0xC8000000U;
		}

		Result = ((IValue + 0x0FFFU + ((IValue >> 13U) & 1U)) >> 13U)&0x7FFFU; 
	}
	return (unsigned short)(Result|Sign);
}
inline VSREAL HalfToFloat(unsigned short Value)
{


	unsigned int Mantissa;
	unsigned int Exponent;
	unsigned int Result;

	Mantissa = (unsigned int)(Value & 0x03FF);

	if ((Value & 0x7C00) != 0)  // The value is normalized
	{
		Exponent = (unsigned int)((Value >> 10) & 0x1F);
	}
	else if (Mantissa != 0)     // The value is denormalized
	{
		// Normalize the value in the resulting VSREAL
		Exponent = 1;

		do
		{
			Exponent--;
			Mantissa <<= 1;
		} while ((Mantissa & 0x0400) == 0);

		Mantissa &= 0x03FF;
	}
	else                        // The value is zero
	{
		Exponent = (unsigned int)-112;
	}

	Result = ((Value & 0x8000) << 16) | // Sign
		((Exponent + 112) << 23) | // Exponent
		(Mantissa << 13);          // Mantissa

	return *(VSREAL*)&Result;
}
//f must [0,1]
inline unsigned int CompressUnitFloat(VSREAL f, unsigned int Bit = 16)
{
	unsigned int nIntervals = 1 << Bit;
	VSREAL scaled = f * (nIntervals - 1);
	unsigned int rounded = (unsigned int)(scaled + 0.5f);
	if (rounded > nIntervals - 1)
	{
		rounded = nIntervals - 1;
	}
	return rounded;
	
}
inline unsigned int CompressFloat(VSREAL f, VSREAL Max , VSREAL Min ,unsigned int Bit = 16)
{
	VSREAL Unitf = (f - Min)/(Max - Min);
	return CompressUnitFloat(Unitf,Bit);
}
inline VSREAL DecompressUnitFloat(unsigned int quantized,unsigned int Bit = 16)
{
	unsigned int nIntervals = 1 << Bit;
	VSREAL IntervalSize = 1.0f / (nIntervals - 1);
	
	return quantized * IntervalSize;
}
inline VSREAL DecompressFloat(unsigned int quantized,VSREAL Max , VSREAL Min ,unsigned int Bit = 16)
{
	VSREAL Unitf = DecompressUnitFloat(quantized,Bit);
	return (Min + Unitf * (Max - Min));
}
inline  VSREAL ClampAxis(VSREAL Angle)
{
	// returns Angle in the range (-360,360)
	Angle = FMOD(Angle, 360.f);

	if (Angle < 0.f)
	{
		// shift to [0,360) range
		Angle += 360.f;
	}

	return Angle;
}


inline  VSREAL NormalizeAxis(VSREAL Angle)
{
	// returns Angle in the range [0,360)
	Angle = ClampAxis(Angle);

	if (Angle > 180.f)
	{
		// shift to (-180,180]
		Angle -= 360.f;
	}

	return Angle;
}
inline int FastLog2(int x)
{
	float fx;
	unsigned int ix, exp;
	fx = (float)x;
	ix = *(unsigned int*)&fx;
	exp = (ix >> 23) & 0xFF;
	return exp - 127;
}
//-360.....360 can use
inline VSREAL ClampAngle(VSREAL AngleDegrees, VSREAL MinAngleDegrees, VSREAL MaxAngleDegrees)
{
	VSREAL MaxDelta = ClampAxis(MaxAngleDegrees - MinAngleDegrees) * 0.5f;			// 0..180
	VSREAL RangeCenter = ClampAxis(MinAngleDegrees + MaxDelta);						// 0..360
	VSREAL DeltaFromCenter = NormalizeAxis(AngleDegrees - RangeCenter);				// -180..180

	// maybe clamp to nearest edge
	if (DeltaFromCenter > MaxDelta)
	{
		return NormalizeAxis(RangeCenter + MaxDelta);
	}
	else if (DeltaFromCenter < -MaxDelta)
	{
		return NormalizeAxis(RangeCenter - MaxDelta);
	}

	// already in range, just return it
	return NormalizeAxis(AngleDegrees);
}

inline int VSRand() { return rand(); }
inline void VSRandInit(int Seed) { srand(Seed); }
inline VSREAL VSFRand() { return VSRand() / (VSREAL)RAND_MAX; }

inline unsigned int HashCombine(unsigned int A, unsigned int C)
{
	unsigned int B = 0x9e3779b9;
	A += B;

	A -= B; A -= C; A ^= (C >> 13);
	B -= C; B -= A; B ^= (A << 8);
	C -= A; C -= B; C ^= (B >> 13);
	A -= B; A -= C; A ^= (C >> 12);
	B -= C; B -= A; B ^= (A << 16);
	C -= A; C -= B; C ^= (B >> 5);
	A -= B; A -= C; A ^= (C >> 3);
	B -= C; B -= A; B ^= (A << 10);
	C -= A; C -= B; C ^= (B >> 15);

	return C;
}
inline unsigned int PointerHash(const void* Key, unsigned int C = 0)
{
	USIZE_TYPE PtrInt = 0;
#if WINDOWS_PLATFORM
	#if _WIN64
		PtrInt = reinterpret_cast<USIZE_TYPE>(Key) >> 4;
	
	#else
		PtrInt = unsigned int(Key);
	#endif
#else
	VSMAC_ASSERT(0);
#endif
	return HashCombine((unsigned int)PtrInt, C);
}
inline unsigned int GetTypeHash(const unsigned char A)
{
	return A;
}

inline unsigned int GetTypeHash(const char A)
{
	return A;
}

inline unsigned int GetTypeHash(const unsigned short A)
{
	return A;
}

inline unsigned int GetTypeHash(const short A)
{
	return A;
}

inline unsigned int GetTypeHash(const int A)
{
	return A;
}

inline unsigned int GetTypeHash(const unsigned int A)
{
	return A;
}

inline unsigned int GetTypeHash(const unsigned long long A)
{
	return (unsigned int)A + ((unsigned int)(A >> 32) * 23);
}

inline unsigned int GetTypeHash(const long long A)
{
	return (unsigned int)A + ((unsigned int)(A >> 32) * 23);
}

inline unsigned int GetTypeHash(VSREAL A)
{
	return *(unsigned int *)&A;
}
inline unsigned int GetTypeHash(double A)
{
	return GetTypeHash(*(const unsigned long long *)&A);
}
inline unsigned int GetTypeHash(const void * A)
{
	return PointerHash(A);
}
inline unsigned int GetTypeHash(void * A)
{
	return PointerHash(A);
}

bool VSMATH_API VSInitMath();

inline unsigned int GetTypeHash(const TCHAR * A)
{
	return VSMathInstance::GetMathInstance().CRC32Compute(A,VSStrLen(A));
}
inline unsigned int Rounding(VSREAL fNum)
{
	return unsigned int(fNum + 0.5f);
}
inline unsigned int FloorLog2(unsigned int Value)
{
	unsigned int pos = 0;
	if (Value >= 1 << 16) { Value >>= 16; pos += 16; }
	if (Value >= 1 << 8) { Value >>= 8; pos += 8; }
	if (Value >= 1 << 4) { Value >>= 4; pos += 4; }
	if (Value >= 1 << 2) { Value >>= 2; pos += 2; }
	if (Value >= 1 << 1) { pos += 1; }
	return (Value == 0) ? 0 : pos;
}
inline unsigned int CountLeadingZeros(unsigned int Value)
{
	if (Value == 0) return 32;
	return 31 - FloorLog2(Value);
}
inline unsigned int CeilLogTwo(unsigned int Arg)
{
	unsigned int Bitmask = ((unsigned int)(CountLeadingZeros(Arg) << 26)) >> 31;
	return (32 - CountLeadingZeros(Arg - 1)) & (~Bitmask);
}
}
