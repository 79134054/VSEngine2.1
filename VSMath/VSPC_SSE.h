#pragma once
#include <emmintrin.h>
typedef __m128	VSVectorRegister;
typedef __m128i VSVectorRegisterInt;
/**
* @param A0	Selects which element (0-3) from 'A' into 1st slot in the result
* @param A1	Selects which element (0-3) from 'A' into 2nd slot in the result
* @param B2	Selects which element (0-3) from 'B' into 3rd slot in the result
* @param B3	Selects which element (0-3) from 'B' into 4th slot in the result
*/
#define SHUFFLEMASK(A0,A1,B2,B3) ( (A0) | ((A1)<<2) | ((B2)<<4) | ((B3)<<6) )

/**
* Returns a bitwise equivalent vector based on 4 DWORDs.
*
* @param X		1st unsigned int component
* @param Y		2nd unsigned int component
* @param Z		3rd unsigned int component
* @param W		4th unsigned int component
* @return		Bitwise equivalent vector with 4 floats
*/
inline VSVectorRegister MakeVectorRegister(unsigned int X, unsigned int Y, unsigned int Z, unsigned int W)
{
	union { VSVectorRegister v; VSVectorRegisterInt i; } Tmp;
	Tmp.i = _mm_setr_epi32(X, Y, Z, W);
	return Tmp.v;
}

/**
* Returns a vector based on 4 FLOATs.
*
* @param X		1st VSREAL component
* @param Y		2nd VSREAL component
* @param Z		3rd VSREAL component
* @param W		4th VSREAL component
* @return		Vector of the 4 FLOATs
*/

inline VSVectorRegister MakeVectorRegister(VSREAL X, VSREAL Y, VSREAL Z, VSREAL W)
{
	return _mm_setr_ps(X, Y, Z, W);
}

/**
* Returns a vector based on 4 int.
*
* @param X		1st int component
* @param Y		2nd int component
* @param Z		3rd int component
* @param W		4th int component
* @return		Vector of the 4 int
*/
inline VSVectorRegisterInt MakeVectorRegisterInt(int X, int Y, int Z, int W)
{
	return _mm_setr_epi32(X, Y, Z, W);
}
/*=============================================================================
*	Constants:
*============================================================================*/

#include "VSVectorRegisterConstants.h"

/*=============================================================================
*	Intrinsics:
*============================================================================*/
/**
* Returns a vector with all zeros.
*
* @return		VSVectorRegister(0.0f, 0.0f, 0.0f, 0.0f)
*/
inline VSVectorRegister VectorZero(void)
{
	return _mm_setzero_ps();
}
/**
* Returns an component from a vector.
*
* @param Vec				Vector register
* @param ComponentIndex	Which component to get, X=0, Y=1, Z=2, W=3
* @return					The component as a VSREAL
*/
inline VSREAL VectorGetComponent(VSVectorRegister Vec, unsigned int ComponentIndex)
{
	return (((VSREAL*)&(Vec))[ComponentIndex]);
}
/**
* Returns X + Y + Z.
*
* @param Vec				Vector register
* @return					The component as a VSREAL
*/
inline VSREAL VectorGetXYZSum(VSVectorRegister Vec)
{
	return (((VSREAL*)&(Vec))[0]) + (((VSREAL*)&(Vec))[1]) + (((VSREAL*)&(Vec))[2]);
}
/**
* Loads 4 FLOATs from unaligned memory.
*
* @param Ptr	Unaligned memory pointer to the 4 FLOATs
* @return		VSVectorRegister(Ptr[0], Ptr[1], Ptr[2], Ptr[3])
*/
inline VSVectorRegister VectorLoad(const void* Ptr)
{
	return _mm_loadu_ps((VSREAL*)(Ptr));
}
/**
* Loads 3 FLOATs from unaligned memory and leaves W undefined.
*
* @param Ptr	Unaligned memory pointer to the 3 FLOATs
* @return		VSVectorRegister(Ptr[0], Ptr[1], Ptr[2], undefined)
*/
#define VectorLoadFloat3( Ptr )			MakeVectorRegister( ((const VSREAL*)(Ptr))[0], ((const VSREAL*)(Ptr))[1], ((const VSREAL*)(Ptr))[2], 0.0f )

/**
* Loads 3 FLOATs from unaligned memory and sets W=0.
*
* @param Ptr	Unaligned memory pointer to the 3 FLOATs
* @return		VSVectorRegister(Ptr[0], Ptr[1], Ptr[2], 0.0f)
*/
#define VectorLoadFloat3_W0( Ptr )		MakeVectorRegister( ((const VSREAL*)(Ptr))[0], ((const VSREAL*)(Ptr))[1], ((const VSREAL*)(Ptr))[2], 0.0f )

/**
* Loads 3 FLOATs from unaligned memory and sets W=1.
*
* @param Ptr	Unaligned memory pointer to the 3 FLOATs
* @return		VSVectorRegister(Ptr[0], Ptr[1], Ptr[2], 1.0f)
*/
#define VectorLoadFloat3_W1( Ptr )		MakeVectorRegister( ((const VSREAL*)(Ptr))[0], ((const VSREAL*)(Ptr))[1], ((const VSREAL*)(Ptr))[2], 1.0f )

/**
* Loads 4 FLOATs from aligned memory.
*
* @param Ptr	Aligned memory pointer to the 4 FLOATs
* @return		VSVectorRegister(Ptr[0], Ptr[1], Ptr[2], Ptr[3])
*/
#define VectorLoadAligned( Ptr )		_mm_load_ps( (const VSREAL*)(Ptr) )

/**
* Loads 1 VSREAL from unaligned memory and replicates it to all 4 elements.
*
* @param Ptr	Unaligned memory pointer to the VSREAL
* @return		VSVectorRegister(Ptr[0], Ptr[0], Ptr[0], Ptr[0])
*/
#define VectorLoadFloat1( Ptr )			_mm_load1_ps( (const VSREAL*)(Ptr) )

/**
* Loads 2 floats from unaligned memory into X and Y and duplicates them in Z and W.
*
* @param Ptr	Unaligned memory pointer to the floats
* @return		VSVectorRegister(Ptr[0], Ptr[1], Ptr[0], Ptr[1])
*/
#define VectorLoadFloat2( Ptr )			_mm_castpd_ps(_mm_load1_pd((const double*)(Ptr)))

/**
* Creates a vector out of three FLOATs and leaves W undefined.
*
* @param X		1st VSREAL component
* @param Y		2nd VSREAL component
* @param Z		3rd VSREAL component
* @return		VSVectorRegister(X, Y, Z, undefined)
*/
#define VectorSetFloat3( X, Y, Z )		MakeVectorRegister( X, Y, Z, 0.0f )

/**
* Propagates passed in VSREAL to all registers
*
* @param F		Float to set
* @return		VSVectorRegister(F,F,F,F)
*/
#define VectorSetFloat1( F )					_mm_set1_ps( F )
/**
* Stores a vector to memory (aligned or unaligned).
*
* @param Vec	Vector to store
* @param Ptr	Memory pointer
*/
#define VectorStore( Vec, Ptr )					_mm_storeu_ps((VSREAL*)(Ptr), Vec)
/**
* Performs non-temporal store of a vector to aligned memory without polluting the caches
*
* @param Vec	Vector to store
* @param Ptr	Aligned memory pointer
*/
#define VectorStoreAlignedStreamed( Vec, Ptr )	_mm_stream_ps( (VSREAL*)(Ptr), Vec )
/**
* Stores a vector to aligned memory.
*
* @param Vec	Vector to store
* @param Ptr	Aligned memory pointer
*/
#define VectorStoreAligned( Vec, Ptr )			_mm_store_ps( (VSREAL*)(Ptr), Vec )
/**
* Stores the XYZ components of a vector to unaligned memory.
*
* @param Vec	Vector to store XYZ
* @param Ptr	Unaligned memory pointer
*/
inline void VectorStoreFloat3(const VSVectorRegister& Vec, void* Ptr)
{
	union { VSVectorRegister v; VSREAL f[4]; } Tmp;
	Tmp.v = Vec;
	VSREAL* FloatPtr = (VSREAL*)(Ptr);
	FloatPtr[0] = Tmp.f[0];
	FloatPtr[1] = Tmp.f[1];
	FloatPtr[2] = Tmp.f[2];
}
/**
* Stores the X component of a vector to unaligned memory.
*
* @param Vec	Vector to store X
* @param Ptr	Unaligned memory pointer
*/
#define VectorStoreFloat1( Vec, Ptr )			_mm_store_ss((VSREAL*)(Ptr), Vec)
/**
* Returns the absolute value (component-wise).
*
* @param Vec			Source vector
* @return				VSVectorRegister( abs(Vec.x), abs(Vec.y), abs(Vec.z), abs(Vec.w) )
*/
#define VectorAbs( Vec )						_mm_and_ps(Vec, VectorRegisterConstants::SignMask)
/**
* Returns the negated value (component-wise).
*
* @param Vec			Source vector
* @return				VSVectorRegister( -Vec.x, -Vec.y, -Vec.z, -Vec.w )
*/
#define VectorNegate( Vec )						_mm_sub_ps(_mm_setzero_ps(),Vec)
/**
* Replicates one element into all four elements and returns the new vector.
*
* @param Vec			Source vector
* @param ElementIndex	Index (0-3) of the element to replicate
* @return				VSVectorRegister( Vec[ElementIndex], Vec[ElementIndex], Vec[ElementIndex], Vec[ElementIndex] )
*/
#define VectorReplicate( Vec, ElementIndex )	_mm_shuffle_ps( Vec, Vec, SHUFFLEMASK(ElementIndex,ElementIndex,ElementIndex,ElementIndex) )
/**
* Adds two vectors (component-wise) and returns the result.
*
* @param Vec1	1st vector
* @param Vec2	2nd vector
* @return		VSVectorRegister( Vec1.x+Vec2.x, Vec1.y+Vec2.y, Vec1.z+Vec2.z, Vec1.w+Vec2.w )
*/
#define VectorAdd(Vec1, Vec2)				 _mm_add_ps(Vec1, Vec2)
/**
* Subtracts a vector from another (component-wise) and returns the result.
*
* @param Vec1	1st vector
* @param Vec2	2nd vector
* @return		VSVectorRegister( Vec1.x-Vec2.x, Vec1.y-Vec2.y, Vec1.z-Vec2.z, Vec1.w-Vec2.w )
*/
#define VectorSubtract(Vec1, Vec2)			_mm_sub_ps(Vec1, Vec2)
/**
* Multiplies two vectors (component-wise) and returns the result.
*
* @param Vec1	1st vector
* @param Vec2	2nd vector
* @return		VSVectorRegister( Vec1.x*Vec2.x, Vec1.y*Vec2.y, Vec1.z*Vec2.z, Vec1.w*Vec2.w )
*/
#define VectorMultiply(Vec1, Vec2)			_mm_mul_ps(Vec1, Vec2)
/**
* Divides two vectors (component-wise) and returns the result.
*
* @param Vec1	1st vector
* @param Vec2	2nd vector
* @return		VSVectorRegister( Vec1.x/Vec2.x, Vec1.y/Vec2.y, Vec1.z/Vec2.z, Vec1.w/Vec2.w )
*/
#define VectorDivide(Vec1, Vec2)			_mm_div_ps(Vec1, Vec2)
/**
* Multiplies two vectors (component-wise), adds in the third vector and returns the result.
*
* @param Vec1	1st vector
* @param Vec2	2nd vector
* @param Vec3	3rd vector
* @return		VectorRegister( Vec1.x*Vec2.x + Vec3.x, Vec1.y*Vec2.y + Vec3.y, Vec1.z*Vec2.z + Vec3.z, Vec1.w*Vec2.w + Vec3.w )
*/
#define VectorMultiplyAdd( Vec1, Vec2, Vec3 )	_mm_add_ps( _mm_mul_ps(Vec1, Vec2), Vec3 )
/**
* Creates a four-part mask based on component-wise == compares of the input vectors
*
* @param Vec1	1st vector
* @param Vec2	2nd vector
* @return		VSVectorRegister( Vec1.x == Vec2.x ? 0xFFFFFFFF : 0, same for yzw )
*/
#define VectorCompareEQ( Vec1, Vec2 )			_mm_cmpeq_ps( Vec1, Vec2 )

/**
* Creates a four-part mask based on component-wise != compares of the input vectors
*
* @param Vec1	1st vector
* @param Vec2	2nd vector
* @return		VSVectorRegister( Vec1.x != Vec2.x ? 0xFFFFFFFF : 0, same for yzw )
*/
#define VectorCompareNE( Vec1, Vec2 )			_mm_cmpneq_ps( Vec1, Vec2 )

/**
* Creates a four-part mask based on component-wise > compares of the input vectors
*
* @param Vec1	1st vector
* @param Vec2	2nd vector
* @return		VSVectorRegister( Vec1.x > Vec2.x ? 0xFFFFFFFF : 0, same for yzw )
*/
#define VectorCompareGT( Vec1, Vec2 )			_mm_cmpgt_ps( Vec1, Vec2 )

/**
* Creates a four-part mask based on component-wise >= compares of the input vectors
*
* @param Vec1	1st vector
* @param Vec2	2nd vector
* @return		VSVectorRegister( Vec1.x >= Vec2.x ? 0xFFFFFFFF : 0, same for yzw )
*/
#define VectorCompareGE( Vec1, Vec2 )			_mm_cmpge_ps( Vec1, Vec2 )

/**
* Creates a four-part mask based on component-wise < compares of the input vectors
*
* @param Vec1	1st vector
* @param Vec2	2nd vector
* @return		VSVectorRegister( Vec1.x < Vec2.x ? 0xFFFFFFFF : 0, same for yzw )
*/
#define VectorCompareLT( Vec1, Vec2 )			_mm_cmplt_ps( Vec1, Vec2 )

/**
* Creates a four-part mask based on component-wise <= compares of the input vectors
*
* @param Vec1	1st vector
* @param Vec2	2nd vector
* @return		VSVectorRegister( Vec1.x <= Vec2.x ? 0xFFFFFFFF : 0, same for yzw )
*/
#define VectorCompareLE( Vec1, Vec2 )			_mm_cmple_ps( Vec1, Vec2 )

/**
* Does a bitwise vector selection based on a mask (e.g., created from VectorCompareXX)
*
* @param Mask  Mask (when 1: use the corresponding bit from Vec1 otherwise from Vec2)
* @param Vec1	1st vector
* @param Vec2	2nd vector
* @return		VSVectorRegister( for each bit i: Mask[i] ? Vec1[i] : Vec2[i] )
*
*/
#define VectorSelect(Mask, Vec1,Vec2) _mm_xor_ps(Vec2, _mm_and_ps(Mask, _mm_xor_ps(Vec1, Vec2)))

/**
* Combines two vectors using bitwise OR (treating each vector as a 128 bit field)
*
* @param Vec1	1st vector
* @param Vec2	2nd vector
* @return		VSVectorRegister( for each bit i: Vec1[i] | Vec2[i] )
*/
#define VectorBitwiseOr(Vec1, Vec2)	_mm_or_ps(Vec1, Vec2)

/**
* Combines two vectors using bitwise AND (treating each vector as a 128 bit field)
*
* @param Vec1	1st vector
* @param Vec2	2nd vector
* @return		VSVectorRegister( for each bit i: Vec1[i] & Vec2[i] )
*/
#define VectorBitwiseAnd(Vec1, Vec2) _mm_and_ps(Vec1, Vec2)

/**
* Combines two vectors using bitwise XOR (treating each vector as a 128 bit field)
*
* @param Vec1	1st vector
* @param Vec2	2nd vector
* @return		VSVectorRegister( for each bit i: Vec1[i] ^ Vec2[i] )
*/
#define VectorBitwiseXor(Vec1, Vec2) _mm_xor_ps(Vec1, Vec2)

/**
* Returns an estimate of 1/SQRT(c) for each component of the vector
*
* @param Vector		Vector
* @return			VSVectorRegister(1/SQRT(t), 1/SQRT(t), 1/SQRT(t), 1/SQRT(t))
*/
#define VectorReciprocalSqrt(Vec)		_mm_rsqrt_ps( Vec )

/**
* Computes an estimate of the reciprocal of a vector (component-wise) and returns the result.
*
* @param Vec	1st vector
* @return		VSVectorRegister( (Estimate) 1.0f / Vec.x, (Estimate) 1.0f / Vec.y, (Estimate) 1.0f / Vec.z, (Estimate) 1.0f / Vec.w )
*/
#define VectorReciprocal(Vec)			_mm_rcp_ps(Vec)
/**
* Loads XYZ and sets W=0
*
* @param Vector	VSVectorRegister
* @return		VSVectorRegister(X, Y, Z, 0.0f)
*/
#define VectorSet_W0( Vec )		_mm_and_ps( Vec, VectorRegisterConstants::XYZMask )
/**
* Returns the minimum values of two vectors (component-wise).
*
* @param Vec1	1st vector
* @param Vec2	2nd vector
* @return		VSVectorRegister( min(Vec1.x,Vec2.x), min(Vec1.y,Vec2.y), min(Vec1.z,Vec2.z), min(Vec1.w,Vec2.w) )
*/
#define VectorMin( Vec1, Vec2 )			_mm_min_ps( Vec1, Vec2 )

/**
* Returns the maximum values of two vectors (component-wise).
*
* @param Vec1	1st vector
* @param Vec2	2nd vector
* @return		VSVectorRegister( max(Vec1.x,Vec2.x), max(Vec1.y,Vec2.y), max(Vec1.z,Vec2.z), max(Vec1.w,Vec2.w) )
*/
#define VectorMax( Vec1, Vec2 )			_mm_max_ps( Vec1, Vec2 )

/**
* Swizzles the 4 components of a vector and returns the result.
*
* @param Vec		Source vector
* @param X			Index for which component to use for X (literal 0-3)
* @param Y			Index for which component to use for Y (literal 0-3)
* @param Z			Index for which component to use for Z (literal 0-3)
* @param W			Index for which component to use for W (literal 0-3)
* @return			The swizzled vector
*/
#define VectorSwizzle( Vec, X, Y, Z, W )	_mm_shuffle_ps( Vec, Vec, SHUFFLEMASK(X,Y,Z,W) )

/**
* Creates a vector through selecting two components from each vector via a shuffle mask.
*
* @param Vec1		Source vector1
* @param Vec2		Source vector2
* @param X			Index for which component of Vector1 to use for X (literal 0-3)
* @param Y			Index for which component of Vector1 to use for Y (literal 0-3)
* @param Z			Index for which component of Vector2 to use for Z (literal 0-3)
* @param W			Index for which component of Vector2 to use for W (literal 0-3)
* @return			The swizzled vector
*/
#define VectorShuffle( Vec1, Vec2, X, Y, Z, W )	_mm_shuffle_ps( Vec1, Vec2, SHUFFLEMASK(X,Y,Z,W) )
/**
* Creates a vector by combining two high components from each vector
*
* @param Vec1		Source vector1
* @param Vec2		Source vector2
* @return			The combined vector
*/
#define VectorCombineHigh(Vec1,Vec2) VectorShuffle(Vec1, Vec2, 2, 3, 2, 3)


/**
* Creates a vector by combining two low components from each vector
*
* @param Vec1		Source vector1
* @param Vec2		Source vector2
* @return			The combined vector
*/
#define VectorCombineLow(Vec1,Vec2) VectorShuffle(Vec1, Vec2, 0, 1, 0, 1)

/**
* These functions return a vector mask to indicate which components pass the comparison.
* Each component is 0xffffffff if it passes, 0x00000000 if it fails.
*
* @param Vec1			1st source vector
* @param Vec2			2nd source vector
* @return				Vector with a mask for each component.
*/
#define VectorMask_LT( Vec1, Vec2 )			_mm_cmplt_ps(Vec1, Vec2)
#define VectorMask_LE( Vec1, Vec2 )			_mm_cmple_ps(Vec1, Vec2)
#define VectorMask_GT( Vec1, Vec2 )			_mm_cmpgt_ps(Vec1, Vec2)
#define VectorMask_GE( Vec1, Vec2 )			_mm_cmpge_ps(Vec1, Vec2)
#define VectorMask_EQ( Vec1, Vec2 )			_mm_cmpeq_ps(Vec1, Vec2)
#define VectorMask_NE( Vec1, Vec2 )			_mm_cmpneq_ps(Vec1, Vec2)
/**
* Returns non-zero if any element in Vec1 is greater than the corresponding element in Vec2, otherwise 0.
*
* @param Vec1			1st source vector
* @param Vec2			2nd source vector
* @return				Non-zero integer if (Vec1.x > Vec2.x) || (Vec1.y > Vec2.y) || (Vec1.z > Vec2.z) || (Vec1.w > Vec2.w)
*/
#define VectorAnyGreaterThan( Vec1, Vec2 )		_mm_movemask_ps( _mm_cmpgt_ps(Vec1, Vec2) )
/**
* Returns an integer bit-mask (0x00 - 0x0f) based on the sign-bit for each component in a vector.
*
* @param VecMask		Vector
* @return				Bit 0 = sign(VecMask.x), Bit 1 = sign(VecMask.y), Bit 2 = sign(VecMask.z), Bit 3 = sign(VecMask.w)
*/
#define VectorMaskBits( VecMask )			_mm_movemask_ps( VecMask )

inline VSREAL InvSqrt(VSREAL F)
{
	// Performs two passes of Newton-Raphson iteration on the hardware estimate
	//    v^-0.5 = x
	// => x^2 = v^-1
	// => 1/(x^2) = v
	// => F(x) = x^-2 - v
	//    F'(x) = -2x^-3

	//    x1 = x0 - F(x0)/F'(x0)
	// => x1 = x0 + 0.5 * (x0^-2 - Vec) * x0^3
	// => x1 = x0 + 0.5 * (x0 - Vec * x0^3)
	// => x1 = x0 + x0 * (0.5 - 0.5 * Vec * x0^2)
	//
	// This final form has one more operation than the legacy factorization (X1 = 0.5*X0*(3-(Y*X0)*X0)
	// but retains better accuracy (namely InvSqrt(1) = 1 exactly).

	const __m128 fOneHalf = _mm_set_ss(0.5f);
	__m128 Y0, X0, X1, X2, FOver2;
	float temp;

	Y0 = _mm_set_ss(F);
	X0 = _mm_rsqrt_ss(Y0);	// 1/sqrt estimate (12 bits)
	FOver2 = _mm_mul_ss(Y0, fOneHalf);

	// 1st Newton-Raphson iteration
	X1 = _mm_mul_ss(X0, X0);
	X1 = _mm_sub_ss(fOneHalf, _mm_mul_ss(FOver2, X1));
	X1 = _mm_add_ss(X0, _mm_mul_ss(X0, X1));

	// 2nd Newton-Raphson iteration
	X2 = _mm_mul_ss(X1, X1);
	X2 = _mm_sub_ss(fOneHalf, _mm_mul_ss(FOver2, X2));
	X2 = _mm_add_ss(X1, _mm_mul_ss(X1, X2));

	_mm_store_ss(&temp, X2);
	return temp;
}