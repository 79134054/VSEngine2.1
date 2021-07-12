#include "VSMath.h"
/*----------------------------------------------------------------*/
inline void VSMatrix3X3::Identity(void) 
{
	VSMemset(m, 0, sizeof(VSMatrix3X3));
	_00 = _11 = _22 =1.0f;
}
/*----------------------------------------------------------------*/
inline void VSMatrix3X3::TransposeOf(const VSMatrix3X3 &Matrix) 
{
	_00 = Matrix._00;
	_10 = Matrix._01;
	_20 = Matrix._02;


	_01 = Matrix._10;
	_11 = Matrix._11;
	_21 = Matrix._12;


	_02 = Matrix._20;
	_12 = Matrix._21;
	_22 = Matrix._22;

}

/*----------------------------------------------------------------*/
inline VSREAL VSMatrix3X3::Det()const
{
	return (M[0][0] * (M[1][1] * M[2][2] - M[2][1] * M[1][2]) -
		M[0][1] * (M[1][0] * M[2][2] - M[2][0] * M[1][2]) +
		M[0][2] * (M[1][0] * M[2][1] - M[2][0] * M[1][1]));

}
inline void VSMatrix3X3::InverseOf(const VSMatrix3X3 &Matirx) 
{

	VSREAL det = Matirx.Det();

	if (ABS(det) < EPSILON_E4)
		return;


	VSREAL det_inv = 1.0f / det;

	M[0][0] = det_inv * (Matirx.M[1][1] * Matirx.M[2][2] - Matirx.M[2][1] * Matirx.M[1][2]);
	M[1][0] = -det_inv * (Matirx.M[1][0] * Matirx.M[2][2] - Matirx.M[2][0] * Matirx.M[1][2]);
	M[2][0] = det_inv * (Matirx.M[1][0] * Matirx.M[2][1] - Matirx.M[2][0] * Matirx.M[1][1]);

	M[0][1] = -det_inv * (Matirx.M[0][1] * Matirx.M[2][2] - Matirx.M[2][1] * Matirx.M[0][2]);
	M[1][1] = det_inv * (Matirx.M[0][0] * Matirx.M[2][2] - Matirx.M[2][0] * Matirx.M[0][2]);
	M[2][1] = -det_inv * (Matirx.M[0][0] * Matirx.M[2][1] - Matirx.M[2][0] * Matirx.M[0][1]);

	M[0][2] = det_inv * (Matirx.M[0][1] * Matirx.M[1][2] - Matirx.M[1][1] * Matirx.M[0][2]);
	M[1][2] = -det_inv * (Matirx.M[0][0] * Matirx.M[1][2] - Matirx.M[1][0] * Matirx.M[0][2]);
	M[2][2] = det_inv * (Matirx.M[0][0] * Matirx.M[1][1] - Matirx.M[1][0] * Matirx.M[0][1]);


} // func
/*----------------------------------------------------------------*/

inline VSMatrix3X3 VSMatrix3X3::operator * (const VSMatrix3X3 &Matirx) const 
{
	VSMatrix3X3 mResult;
	mResult.M[0][0] = 0;
	mResult.M[1][1] = 0;
	mResult.M[2][2] = 0;
#ifdef VS_SSE
 	{
		VSVectorRegister A0 = MakeVectorRegister(M[0][0],M[0][1],M[0][2],0.0f);
		VSVectorRegister A1 = MakeVectorRegister(M[1][0],M[1][1],M[1][2],0.0f);
		VSVectorRegister A2 = MakeVectorRegister(M[2][0],M[2][1],M[2][2],0.0f);

		VSVectorRegister B0 = MakeVectorRegister(Matirx.M[0][0],Matirx.M[0][1],Matirx.M[0][2],0.0f);
		VSVectorRegister B1 = MakeVectorRegister(Matirx.M[1][0],Matirx.M[1][1],Matirx.M[1][2],0.0f);
		VSVectorRegister B2 = MakeVectorRegister(Matirx.M[2][0],Matirx.M[2][1],Matirx.M[2][2],0.0f);
		
		 // First row of result (Matrix1[0] * Matrix2).
		 VSVectorRegister Temp = VectorMultiply(VectorReplicate(A0, 0), B0);
		 Temp = VectorMultiplyAdd(VectorReplicate(A0, 1), B1, Temp);
		 VSVectorRegister R0 = VectorMultiplyAdd(VectorReplicate(A0, 2), B2, Temp);


		 // Second row of result (Matrix1[1] * Matrix2).
		 Temp = VectorMultiply(VectorReplicate(A1, 0), B0);
		 Temp = VectorMultiplyAdd(VectorReplicate(A1, 1), B1, Temp);
		 VSVectorRegister R1 = VectorMultiplyAdd(VectorReplicate(A1, 2), B2, Temp);


		 // Third row of result (Matrix1[2] * Matrix2).
		 Temp = VectorMultiply(VectorReplicate(A2, 0), B0);
		 Temp = VectorMultiplyAdd(VectorReplicate(A2, 1), B1, Temp);
		 VSVectorRegister R2 = VectorMultiplyAdd(VectorReplicate(A2, 2), B2, Temp);

		 VectorStoreFloat3(R0, &mResult.m[0]);
		 VectorStoreFloat3(R1, &mResult.m[3]);
		 VectorStoreFloat3(R2, &mResult.m[6]);
	}
#else
	{		
		for(unsigned char i = 0; i < 3; i++) 
			for(unsigned char j = 0; j < 3; j++) 
				for(unsigned int k = 0 ; k < 3 ; k++)
					mResult.M[i][j] += M[i][k] * Matirx.M[k][j];
	}
#endif
	return mResult;
} 
/*----------------------------------------------------------------*/
inline VSVector3 VSMatrix3X3::operator * (const VSVector3 &vc) const
{
	return vc * (*this);
}

/*----------------------------------------------------------------*/
inline void VSMatrix3X3::CreateFromTwoVector(const VSVector3 & v1,const VSVector3 & v2)
{

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			M[i][j] = v1.m[i] * v2.m[j];

		}
	}

}
/*----------------------------------------------------------------*/
inline void VSMatrix3X3::SetZero()
{
	VSMemset(m, 0, sizeof(VSMatrix3X3));
}
