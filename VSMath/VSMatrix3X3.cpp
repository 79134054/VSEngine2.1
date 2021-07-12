#include "VSMatrix3X3.h"
#include "VSMath.h"
using namespace VSEngine2;
const VSMatrix3X3 VSMatrix3X3::ms_CameraViewRight = VSMatrix3X3(0.0f, 0.0f, -1.0f,
																0.0f,1.0f,0.0f,
																1.0f,0.0f,0.0f);
const VSMatrix3X3 VSMatrix3X3::ms_CameraViewLeft = VSMatrix3X3(0.0f, 0.0f, 1.0f,
																0.0f,1.0f,0.0f,
																-1.0f,0.0f,0.0f);
const VSMatrix3X3 VSMatrix3X3::ms_CameraViewUp = VSMatrix3X3(1.0f, 0.0f, 0.0f,
															0.0f,0.0f,-1.0f,
															 0.0f,1.0f,0.0f);
const VSMatrix3X3 VSMatrix3X3::ms_CameraViewDown = VSMatrix3X3(1.0f, 0.0f, 0.0f,
																0.0f,0.0f,1.0f,
																0.0f,-1.0f,0.0f);
const VSMatrix3X3 VSMatrix3X3::ms_CameraViewFront = VSMatrix3X3(1.0f, 0.0f, 0.0f,
																 0.0f,1.0f,0.0f,
																 0.0f,0.0f,1.0f);
const VSMatrix3X3 VSMatrix3X3::ms_CameraViewBack = VSMatrix3X3(-1.0f, 0.0f, 0.0f,
																0.0f,1.0f,0.0f,
																0.0f,0.0f,-1.0f);
const VSMatrix3X3 VSMatrix3X3::ms_Identity;
const VSMatrix3X3 VSMatrix3X3::ms_3DMax_To_Engine = VSMatrix3X3(1.0f, 0.0f, 0.0f,
															0.0f, 0.0f, 1.0f,
															0.0f, 1.0f, 0.0f);
/*----------------------------------------------------------------*/
VSMatrix3X3::VSMatrix3X3(void)
{
	Identity();

}
/*----------------------------------------------------------------*/
VSMatrix3X3::VSMatrix3X3(VSREAL m00,VSREAL m01,VSREAL m02,
						VSREAL m10,VSREAL m11,VSREAL m12,
						VSREAL m20,VSREAL m21,VSREAL m22)
{
	_00 = m00; _01 = m01; _02 = m02;
	_10 = m10; _11 = m11; _12 = m12;
	_20 = m20; _21 = m21; _22 = m22;


}
/*----------------------------------------------------------------*/
void VSMatrix3X3::CreateRotX(VSREAL a) 
{
	VSREAL fCos = COS(a);
	VSREAL fSin = SIN(a);

	_11 =  fCos;
	_12 =  fSin;
	_21 = -fSin;
	_22 =  fCos;

	_00 = 1.0f;
	_01 = _02  = _10  = _20 =  0.0f;
}
/*----------------------------------------------------------------*/

void VSMatrix3X3::CreateRotY(VSREAL a) {
	VSREAL fCos = COS(a);
	VSREAL fSin = SIN(a);

	_00 =  fCos;
	_02 = -fSin;
	_20 =  fSin;
	_22 =  fCos;

	_11 = 1.0f;
	_01 = _12 = _10 = _21 = 0.0f;
}
/*----------------------------------------------------------------*/


void VSMatrix3X3::CreateRotZ(VSREAL a) 
{
	VSREAL fCos = COS(a);
	VSREAL fSin = SIN(a);

	_00  =  fCos;
	_01  =  fSin;
	_10  = -fSin;
	_11  =  fCos;

	_22 = 1.0f;
	_02 = _12 = _20 = _21 = 0.0f;
}
/*----------------------------------------------------------------*/

void VSMatrix3X3::CreateEuler(VSREAL Z, VSREAL X, VSREAL Y)
{ 
	VSREAL SX, SY, SZ, CX, CY, CZ;

	SZ = SIN(Z);
	CZ = COS(Z);
	SY = SIN(Y);
	CY = COS(Y);
	SX = SIN(X);
	CX = COS(X);

	_00 = CY*CZ + SX * SY * SZ;
	_01 = CX * SZ; 
	_02 = -SY * CZ + SX * CY * SZ;  
	_10 = -CY*SZ + SX * SY * CZ; 
	_11 = CX*CZ;
	_12 = SY * SZ + SX * CY * CZ; ;
	_20 = CX * SY;
	_21 = -SX;
	_22 = CX * CY;
}
/*----------------------------------------------------------------*/

void VSMatrix3X3::CreateAxisAngle(const VSVector3 &_vcAxis, VSREAL a) 
{
	VSVector3 vcAxis = _vcAxis;
	VSREAL fCos = COS(a);
	VSREAL fSin = SIN(a);
	VSREAL fSum = 1.0f - fCos;

	if (vcAxis.GetSqrLength() != 1.0)
		vcAxis.Normalize();

	VSREAL xx = vcAxis.x * vcAxis.x;
	VSREAL yy = vcAxis.y * vcAxis.y;
	VSREAL zz = vcAxis.z * vcAxis.z;

	VSREAL xy = vcAxis.x * vcAxis.y;
	VSREAL yz = vcAxis.y * vcAxis.z;
	VSREAL xz = vcAxis.z * vcAxis.x;

	VSREAL zs = vcAxis.z * fSin;
	VSREAL ys = vcAxis.y * fSin;
	VSREAL xs = vcAxis.x * fSin;

	_00 = xx * fSum + fCos;
	_01 = xy * fSum + zs;
	_02 = xz * fSum - ys;

	_10 = xy * fSum - zs;
	_11 = yy * fSum + fCos;
	_12 = yz * fSum + xs;

	_20 = xz * fSum + ys;
	_21 = yz * fSum - xs;
	_22 = zz * fSum + fCos;

}
/*----------------------------------------------------------------*/
void  VSMatrix3X3::CreateRot(const VSVector3 &U,const VSVector3 &V,const VSVector3 & N)
{
	_00 = U.x;
	_01 = U.y;
	_02 = U.z;

	_10 = V.x;
	_11 = V.y;
	_12 = V.z;

	_20 = N.x;
	_21 = N.y;
	_22 = N.z;

}
/*----------------------------------------------------------------*/
void VSMatrix3X3::GetEuler(VSREAL &Roll, VSREAL &Pitch, VSREAL &Yaw)const
{
	if(_21 > 1.0f)
		Pitch = -VSPI2;
	else if(_21 < -1.0f)
		Pitch = VSPI2;
	else
		Pitch = ASIN(-_21);
	
	if (fabs(_21) > VSREAL_1)
	{
		Yaw = 0;
		Roll = ATAN2(-_10,_00);                                                                                                                  
	
	}
	else
	{
		Roll = ATAN2(_01,_11);
		Yaw = ATAN2(_20,_22);
	
	}
}
/*----------------------------------------------------------------*/
VSQuat VSMatrix3X3::GetQuat()const
{
	VSREAL x,y,z,w;
	VSREAL fTemp[4];
	int iflag = 0;
	fTemp[0] = _00 + _11 + _22;
	fTemp[1] = _00 - _11 - _22;
	fTemp[2] = _11 - _00 - _22;
	fTemp[3] = _22 - _00 - _11;
	VSREAL fBig = fTemp[0];
	for(int i = 1 ; i < 4 ; i++)
	{
		if(fBig < fTemp[i])
		{
			fBig = fTemp[i];
			iflag = i;
		}
	}
	fBig = SQRT(fBig + 1.0f) * 0.5f;
	VSREAL fMult = 0.25f / fBig;
	switch (iflag)
	{
	case 0:
		w = fBig;
		x = (_12 - _21) * fMult;
		y = (_20 - _02) * fMult;
		z = (_01 - _10) * fMult;
		break;
	case 1:
		x = fBig;
		w = (_12 - _21) * fMult;
		y = (_01 + _10) * fMult;
		z = (_20 + _02) * fMult;
 		break;
	case 2:
		y = fBig;
		w = (_20 - _02) * fMult;
		x = (_01 + _10) * fMult;
		z = (_12 + _21) * fMult;
		break;
	case 3:
		z = fBig;
		w = (_01 - _10) * fMult;
		x = (_20 + _02) * fMult;
		y = (_12 + _21) * fMult;
		break;
	}
	return VSQuat(x,y,z,w);
}
/*----------------------------------------------------------------*/
void VSMatrix3X3::GetAxisAngle(VSVector3 & Axis,VSREAL & fAngle)const
{
	fAngle = ACOS(Clamp((_00 + _11 + _22 - 1) * 0.5f,1.0f,-1.0f));
	if(fAngle < VSPI - EPSILON_E4)
	{
		VSVector3 Temp;
		Temp.Set(M[1][2] - M[2][1],M[2][0] - M[0][2],M[0][1] -  M[1][0]);
		Temp.Normalize();
		Axis = Temp;
	
	}
	else
	{
		int flag = 0;
		VSREAL fMax = _00;
		if(fMax < _11)
		{
			fMax = _11;
			flag = 1;
		}
		if(fMax < _22)
		{
			fMax = _22;
			flag = 2;
		}

		
		if(flag == 0)
		{
			VSREAL wTemp = SQRT(_00 - _11 - _22 + 1) * 0.5f;
			VSREAL InWTemp = 1.0f / wTemp;
			Axis.m[0] = wTemp;
			Axis.m[1] = M[0][1] * 0.5f * InWTemp;
			Axis.m[2] = M[0][2] * 0.5f * InWTemp;
		}
		else if(flag == 1)
		{
			VSREAL wTemp = SQRT(_11 - _00 - _22 + 1) * 0.5f;
			VSREAL InWTemp = 1.0f / wTemp;
			Axis.m[1] = wTemp;
			Axis.m[0] = M[0][1] * 0.5f * InWTemp;
			Axis.m[2] = M[1][2] * 0.5f * InWTemp;
		}
		else
		{
			VSREAL wTemp = SQRT(_22 - _00 - _11 + 1) * 0.5f;
			VSREAL InWTemp = 1.0f / wTemp;
			Axis.m[2] = wTemp;
			Axis.m[0] = M[0][2] * 0.5f * InWTemp;
			Axis.m[1] = M[1][2] * 0.5f * InWTemp;
		}
	}
}
/*----------------------------------------------------------------*/
VSMatrix3X3 VSMatrix3X3::operator +(VSREAL f)const
{
	VSMatrix3X3 Temp;

	Temp.m[0] = m[0] + f;
	Temp.m[1] = m[1] + f;
	Temp.m[2] = m[2] + f;
	Temp.m[3] = m[3] + f;
	Temp.m[4] = m[4] + f;
	Temp.m[5] = m[5] + f;
	Temp.m[6] = m[6] + f;
	Temp.m[7] = m[7] + f;
	Temp.m[8] = m[8] + f;


	return Temp;

}
/*----------------------------------------------------------------*/
VSMatrix3X3 VSMatrix3X3::operator -(VSREAL f)const
{
	VSMatrix3X3 Temp;

	Temp.m[0] = m[0] - f;
	Temp.m[1] = m[1] - f;
	Temp.m[2] = m[2] - f;
	Temp.m[3] = m[3] - f;
	Temp.m[4] = m[4] - f;
	Temp.m[5] = m[5] - f;
	Temp.m[6] = m[6] - f;
	Temp.m[7] = m[7] - f;
	Temp.m[8] = m[8] - f;

	return Temp;

}
/*----------------------------------------------------------------*/
VSMatrix3X3 VSMatrix3X3::operator *(VSREAL f)const
{
	VSMatrix3X3 Temp;

	Temp.m[0] = m[0] * f;
	Temp.m[1] = m[1] * f;
	Temp.m[2] = m[2] * f;
	Temp.m[3] = m[3] * f;
	Temp.m[4] = m[4] * f;
	Temp.m[5] = m[5] * f;
	Temp.m[6] = m[6] * f;
	Temp.m[7] = m[7] * f;
	Temp.m[8] = m[8] * f;

	return Temp;
}
/*----------------------------------------------------------------*/
VSMatrix3X3 VSMatrix3X3::operator +(const VSMatrix3X3 &Matirx)const
{

	VSMatrix3X3 Temp;

	Temp.m[0] = m[0] + Matirx.m[0];
	Temp.m[1] = m[1] + Matirx.m[1];
	Temp.m[2] = m[2] + Matirx.m[2];
	Temp.m[3] = m[3] + Matirx.m[3];
	Temp.m[4] = m[4] + Matirx.m[4];
	Temp.m[5] = m[5] + Matirx.m[5];
	Temp.m[6] = m[6] + Matirx.m[6];
	Temp.m[7] = m[7] + Matirx.m[7];
	Temp.m[8] = m[8] + Matirx.m[8];

	return Temp;	

}
/*----------------------------------------------------------------*/
VSMatrix3X3 VSMatrix3X3::operator -(const VSMatrix3X3 &Matirx)const
{
	VSMatrix3X3 Temp;

	Temp.m[0] = m[0] - Matirx.m[0];
	Temp.m[1] = m[1] - Matirx.m[1];
	Temp.m[2] = m[2] - Matirx.m[2];
	Temp.m[3] = m[3] - Matirx.m[3];
	Temp.m[4] = m[4] - Matirx.m[4];
	Temp.m[5] = m[5] - Matirx.m[5];
	Temp.m[6] = m[6] - Matirx.m[6];
	Temp.m[7] = m[7] - Matirx.m[7];
	Temp.m[8] = m[8] - Matirx.m[8];


	return Temp;	

}
/*----------------------------------------------------------------*/
void VSMatrix3X3::GetRowVector(VSVector3 Row[3])const
{
	for(int i = 0 ; i < 3 ; i++)
	{
		for(int j = 0 ; j < 3 ; j++)
			Row[i].m[j] = M[i][j];
	
	}

}
/*----------------------------------------------------------------*/
void VSMatrix3X3::GetColumnVector(VSVector3 Column[3])const
{
	for(int i = 0 ; i < 3 ; i++)
	{
		for(int j = 0 ; j < 3 ; j++)
			Column[i].m[j] = M[j][i];

	}

}
/*----------------------------------------------------------------*/
void VSMatrix3X3::LineInterpolation(VSREAL t,const VSMatrix3X3 & M1, const VSMatrix3X3 &M2)
{
	*this = M2 * t + M1 * (1.0f - t); 

}
/*----------------------------------------------------------------*/
void VSMatrix3X3::Slerp(VSREAL t,const VSMatrix3X3 & M1, const VSMatrix3X3 &M2)
{
	VSMatrix3X3  M1Transpose,Temp;
	M1Transpose.TransposeOf(M1);
	Temp  = M1Transpose * M2;
	VSREAL fAnagle;
	VSVector3 Axis;
	Temp.GetAxisAngle(Axis,fAnagle);
	//Temp.Identity();
	Temp.CreateAxisAngle(Axis,fAnagle * t);
	*this = M1 * Temp;
}
/*----------------------------------------------------------------*/
void VSMatrix3X3::operator +=(VSREAL f)
{
	m[0] = m[0] + f;
	m[1] = m[1] + f;
	m[2] = m[2] + f;
	m[3] = m[3] + f;
	m[4] = m[4] + f;
	m[5] = m[5] + f;
	m[6] = m[6] + f;
	m[7] = m[7] + f;
	m[8] = m[8] + f;
}
/*----------------------------------------------------------------*/
void VSMatrix3X3::operator -=(VSREAL f)
{

	m[0] = m[0] - f;
	m[1] = m[1] - f;
	m[2] = m[2] - f;
	m[3] = m[3] - f;
	m[4] = m[4] - f;
	m[5] = m[5] - f;
	m[6] = m[6] - f;
	m[7] = m[7] - f;
	m[8] = m[8] - f;

}
/*----------------------------------------------------------------*/
void VSMatrix3X3::operator *=(VSREAL f)
{

	m[0] = m[0] * f;
	m[1] = m[1] * f;
	m[2] = m[2] * f;
	m[3] = m[3] * f;
	m[4] = m[4] * f;
	m[5] = m[5] * f;
	m[6] = m[6] * f;
	m[7] = m[7] * f;
	m[8] = m[8] * f;

}
/*----------------------------------------------------------------*/
void VSMatrix3X3::operator +=(const VSMatrix3X3 &Matirx)
{

	m[0] = m[0] + Matirx.m[0];
	m[1] = m[1] + Matirx.m[1];
	m[2] = m[2] + Matirx.m[2];
	m[3] = m[3] + Matirx.m[3];
	m[4] = m[4] + Matirx.m[4];
	m[5] = m[5] + Matirx.m[5];
	m[6] = m[6] + Matirx.m[6];
	m[7] = m[7] + Matirx.m[7];
	m[8] = m[8] + Matirx.m[8];

}
/*----------------------------------------------------------------*/
void VSMatrix3X3::operator -=(const VSMatrix3X3 &Matirx)
{

	m[0] = m[0] - Matirx.m[0];
	m[1] = m[1] - Matirx.m[1];
	m[2] = m[2] - Matirx.m[2];
	m[3] = m[3] - Matirx.m[3];
	m[4] = m[4] - Matirx.m[4];
	m[5] = m[5] - Matirx.m[5];
	m[6] = m[6] - Matirx.m[6];
	m[7] = m[7] - Matirx.m[7];
	m[8] = m[8] - Matirx.m[8];

}
/*----------------------------------------------------------------*/
bool VSMatrix3X3::operator ==(const VSMatrix3X3 &v)const
{
	for (unsigned int i = 0 ; i < 3 ; i++)
	{
		for (unsigned int j = 0 ; j < 3 ; j++)
		{
			if (ABS(M[i][j] - v.M[i][j]) > EPSILON_E4)
			{
				return false;
			}
		}

	}
	return true;
}
/*----------------------------------------------------------------*/
void VSMatrix3X3::GetRowVector(VSVector3 &Row0,VSVector3 &Row1,VSVector3 &Row2)const
{
	Row0.x = M[0][0] ; Row0.y = M[0][1] ; Row0.z = M[0][2] ;
	Row1.x = M[1][0] ; Row1.y = M[1][1] ; Row1.z = M[1][2] ;
	Row2.x = M[2][0] ; Row2.y = M[2][1] ; Row2.z = M[2][2] ;

}
/*----------------------------------------------------------------*/
void VSMatrix3X3::GetColumnVector(VSVector3 &Column0,VSVector3 &Column1,VSVector3 &Column2)const
{
	Column0.x = M[0][0] ; Column0.y = M[1][0] ; Column0.z = M[2][0] ;
	Column1.x = M[0][1] ; Column1.y = M[1][1] ; Column1.z = M[2][1] ;
	Column2.x = M[0][2] ; Column2.y = M[1][2] ; Column2.z = M[2][2] ;


}
/*----------------------------------------------------------------*/
void VSMatrix3X3::GetUVN(VSVector3 UVN[3])const
{
	UVN[0].x = M[0][0] ; UVN[0].y = M[0][1] ; UVN[0].z = M[0][2] ; 
	UVN[1].x = M[1][0] ; UVN[1].y = M[1][1] ; UVN[1].z = M[1][2] ; 
	UVN[2].x = M[2][0] ; UVN[2].y = M[2][1] ; UVN[2].z = M[2][2] ; 

}
/*----------------------------------------------------------------*/
void VSMatrix3X3::GetUVN(VSVector3 & U,VSVector3 &V,VSVector3 &N)const
{
	U.x = M[0][0] ; U.y = M[0][1] ; U.z = M[0][2] ; 
	V.x = M[1][0] ; V.y = M[1][1] ; V.z = M[1][2] ; 
	N.x = M[2][0] ; N.y = M[2][1] ; N.z = M[2][2] ; 
}
/*----------------------------------------------------------------*/
void VSMatrix3X3::CreateScale(VSREAL fX,VSREAL fY,VSREAL fZ)
{
	Identity();
	_00 = fX;
	_11 = fY;
	_22 = fZ;

}
/*----------------------------------------------------------------*/
void VSMatrix3X3::CreateScale(const VSVector3 & Axis,VSREAL fScale)
{
	VSVector3 NorAxis = Axis;
	NorAxis.Normalize();
	VSREAL a = fScale - 1.0f;

	VSREAL ax = a * NorAxis.x;
	VSREAL ay = a * NorAxis.y;
	VSREAL az = a * NorAxis.z;

	M[0][0] = ax * NorAxis.x - 1.0f;
	M[1][1] = ay * NorAxis.y - 1.0f;
	M[2][2] = az * NorAxis.z - 1.0f;

	M[0][1] = M[1][0] = ax * NorAxis.y;
	M[0][2] = M[2][0] = ax * NorAxis.z;
	M[1][2] = M[2][1] = ay * NorAxis.z;


}
/*----------------------------------------------------------------*/
void VSMatrix3X3::GetEigenSystem(VSREAL EigenValue[3],VSVector3 Eigen[3])const
{
	VSREAL Max;
	unsigned int p,q;
	VSREAL r;
	VSREAL t;
	VSREAL cos, sin;
	VSMatrix3X3 Temp ,TempTranspose, EigenMatrix,A;
	A = *this;
	do
	{
		//取出最大值
		Max = A.M[0][2];
		for(unsigned int i = 0 ; i < 3 ; i++)
		{
			for(unsigned int j = i; j < 3 ; j++)
			{
				if(i != j)
				{
					if(ABS(A.M[i][j]) > ABS(Max))
					{
						Max = A.M[i][j];
						p = i;
						q = j;

					}
				}
			}

		}
		if(ABS(Max) < EPSILON_E4) break;
		r = (A.M[p][p] - A.M[q][q]) / Max;
		if(r > EPSILON_E4)
			t = 1.0f / (ABS(r) + SQRT( r * r + 1));
		else if( r < -EPSILON_E4)
			t =  -1.0f / (ABS(r) + SQRT( r * r + 1));
		else
			t = 1.0f;
		cos = 1.0f/ SQRT(1 + t * t);
		sin = t * cos;
		Temp.M[0][0] = cos;	Temp.M[0][1] = -sin;	Temp.M[0][2] = 0;
		Temp.M[1][0] = sin;	Temp.M[1][1] = cos;		Temp.M[1][2] = 0;
		Temp.M[2][0] = 0;	Temp.M[2][1] = 0;		Temp.M[2][2] = 1;

		EigenMatrix = EigenMatrix * Temp;
		TempTranspose.TransposeOf(Temp);

		A = TempTranspose * A * Temp;

	}
	while(ABS(Max) > EPSILON_E4);

	for(unsigned int i = 0; i < 3 ; i++)
	{
		EigenValue[i]  = A.M[i][i];
	}
	EigenMatrix.GetUVN(Eigen);
	
}
void VSMatrix3X3::GetScale(VSVector3 & Scale)const
{
	VSVector3 RowVector[3];
	GetRowVector(RowVector);

	for (unsigned int i = 0 ; i < 3 ; i++)
	{
		Scale.m[i] = RowVector[i].GetLength();
		RowVector[i] /= Scale.m[i];
	}	
}
void VSMatrix3X3::GetScaleAndRotator(VSVector3 & Scale)
{
	VSVector3 RowVector[3];
	GetRowVector(RowVector);

	for (unsigned int i = 0 ; i < 3 ; i++)
	{
		Scale.m[i] = RowVector[i].GetLength();
		RowVector[i] /= Scale.m[i];
	}
	*this = VSMatrix3X3(RowVector[0].x,RowVector[0].y,RowVector[0].z,
						RowVector[1].x,RowVector[1].y,RowVector[1].z,
						RowVector[2].x,RowVector[2].y,RowVector[2].z);
}
void VSMatrix3X3::CreateFromDirection(VSVector3 & Direction , const VSVector3 &Up)
{
	VSVector3 vcTemp, vcUp;
	VSVector3 Dir = Direction;
	Dir.Normalize();

	//计算朝向方向和上方向点积,因为Dir为单位向量,所以fDot为vcWorld在
	//Dir上的投影
	VSREAL fDot = Up.Dot(Dir);
	//计算投影长度,方向和Dir相同的向量
	vcTemp = Dir * fDot;
	//根据向量的平行4边形法则求出up
	/*
	/|
	worldup-->   / |<---up
	/__|__ 
	vcTemp
	*/				
	vcUp = Up - vcTemp;

	//求出长度
	VSREAL fL = vcUp.GetLength();

	// 长度为0,说明 Direction和worldUp几乎平行,交角几乎为0.
	if (fL < EPSILON_E4) 
	{
		VSVector3 vcY;
		//从新选择向上向量,计算up向量
		vcY.Set(0.0f, 1.0f, 0.0f);

		vcTemp = Dir * Direction.y;
		vcUp = vcY - vcTemp;

		fL = vcUp.GetLength();

		// 长度为0,说明 Direction和worldUp几乎平行,交角几乎为0.
		if (fL < EPSILON_E4) 
		{
			//从新选择向上向量,计算up向量
			vcY.Set(0.0f, 0.0f, 1.0f);

			vcTemp = Dir * Dir.z;
			vcUp = vcY - vcTemp;


			fL = vcUp.GetLength();
			if(fL < EPSILON_E4) return ;
		}
	}
	//单位化
	vcUp /= fL;


	VSVector3 vcRight;
	vcRight.Cross(vcUp, Dir);
	CreateRot(vcRight, vcUp, Dir);
}