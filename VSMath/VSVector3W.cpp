#include "VSVector3W.h"
#include "VSMatrix3X3W.h"
using namespace VSEngine2;
VSHalfVector3W::VSHalfVector3W()
{
	x = 0, y = 0, z = 0, w = 0;
}
VSHalfVector3W::VSHalfVector3W(unsigned short _x, unsigned short _y, unsigned short _z, unsigned short _w)
{
	x = _x; y = _y; z = _z; w = _w;
}
VSHalfVector3W VSHalfVector3W::operator + (const VSHalfVector3W &v)const
{
	VSVector3W v1 = HalfToFloat(*this);
	VSVector3W v2 = HalfToFloat(v);
	return VSHalfVector3W(FloatToHalf(v1 + v2));
}
VSHalfVector3W VSHalfVector3W::operator * (VSREAL f)const
{
	VSVector3W v1 = HalfToFloat(*this) * f;
	return VSHalfVector3W(FloatToHalf(v1));
}
/*----------------------------------------------------------------*/
VSVector3W::VSVector3W(void) 
{
	x=0, y=0, z=0, w=1.0f; 
}
/*----------------------------------------------------------------*/
VSVector3W::VSVector3W(VSREAL _x, VSREAL _y, VSREAL _z,VSREAL _w)
{
	x=_x; y=_y; z=_z; w=_w; 
}
/*----------------------------------------------------------------*/
VSVector3W::VSVector3W(unsigned char R,unsigned char G,unsigned char B,unsigned char A)
{
	r = (R * (VSREAL)1.0) / 255;
	g = (G * (VSREAL)1.0) / 255;
	b = (B * (VSREAL)1.0) / 255;
	a = (A * (VSREAL)1.0) / 255;
}
/*----------------------------------------------------------------*/
VSVector3W::VSVector3W(const VSVector3 & V)
{

	Set(V);
}
/*----------------------------------------------------------------*/
void VSVector3W::CreateFromARGB(DWORD ARGB)
{
	unsigned char a,r,g,b;
	VSDWCOLORGetARGB(ARGB,a,r,g,b);
	*this = VSVector3W(r, g, b, a);
}
void VSVector3W::CreateFromBGRA(DWORD BGRA)
{
	unsigned char a,r,g,b;
	VSDWCOLORGetBGRA(BGRA,a,r,g,b);
	*this = VSVector3W(r,g,b,a);
}
void VSVector3W::CreateFromRGBA(DWORD RGBA)
{
	unsigned char a,r,g,b;
	VSDWCOLORGetRGBA(RGBA,a,r,g,b);
	*this = VSVector3W(r,g,b,a);
}
void VSVector3W::CreateFromABGR(DWORD ABGR)
{
	unsigned char a,r,g,b;
	VSDWCOLORGetABGR(ABGR,a,r,g,b);
	*this = VSVector3W(r,g,b,a);
}
/*----------------------------------------------------------------*/
VSVector3W VSVector3W::operator * (const VSMatrix3X3W &m) const 
{
	VSVector3W vcResult;

#ifdef VS_SSE
	{
		VSVectorRegister _v = VectorLoad(this->m);
		VSVectorRegister _m0 = VectorLoad(&m.m[0]);
		VSVectorRegister _m1 = VectorLoad(&m.m[4]);
		VSVectorRegister _m2 = VectorLoad(&m.m[8]);
		VSVectorRegister _m3 = VectorLoad(&m.m[12]);
		// Splat x,y,z and w
		VSVectorRegister VTempX = VectorReplicate(_v, 0);
		VSVectorRegister VTempY = VectorReplicate(_v, 1);
		VSVectorRegister VTempZ = VectorReplicate(_v, 2);
		VSVectorRegister VTempW = VectorReplicate(_v, 3);
		// Mul by the matrix
		VTempX = VectorMultiply(VTempX, _m0);
		VTempY = VectorMultiply(VTempY, _m1);
		VTempZ = VectorMultiply(VTempZ, _m2);
		VTempW = VectorMultiply(VTempW, _m3);
		// Add them all together
		VTempX = VectorAdd(VTempX, VTempY);
		VTempZ = VectorAdd(VTempZ, VTempW);
		VTempX = VectorAdd(VTempX, VTempZ);

		VectorStore(VTempX, &vcResult);
	}
#else
	{
		vcResult.x = x*m._00 + y*m._10 + z*m._20 + w * m._30;
		vcResult.y = x*m._01 + y*m._11 + z*m._21 + w * m._31;
		vcResult.z = x*m._02 + y*m._12 + z*m._22 + w * m._32;
		vcResult.w = x*m._03 + y*m._13 + z*m._23 + w * m._33;

	}
#endif


	return vcResult;
}
/*----------------------------------------------------------------*/
void VSVector3W::operator += (const VSVector3W &v)
{

	x += v.x;   y += v.y;   z += v.z; w += v.w;

}
/*----------------------------------------------------------------*/
void VSVector3W::operator -= (const VSVector3W &v)
{
	x -= v.x;   y -= v.y;   z -= v.z; w -= v.w;
}
/*----------------------------------------------------------------*/
void VSVector3W::operator *= (const VSVector3W &v)
{

	x *= v.x;   y *= v.y;   z *= v.z; w *= v.w;

}
/*----------------------------------------------------------------*/
void VSVector3W::operator /= (const VSVector3W &v)
{

	x /= v.x;   y /= v.y;   z /= v.z; w /= v.w;

}
/*----------------------------------------------------------------*/
void VSVector3W::operator *= (VSREAL f)
{

	x *= f;   y *= f;   z *= f; w *= f;

}
/*----------------------------------------------------------------*/
void VSVector3W::operator /= (VSREAL f)
{

	x /= f;   y /= f;   z /= f; w /= f;

}
/*----------------------------------------------------------------*/
void VSVector3W::operator += (VSREAL f)
{

	x += f;   y += f;   z += f; w += f;

}
/*----------------------------------------------------------------*/
void VSVector3W::operator -= (VSREAL f)
{

	x -= f;   y -= f;   z -= f; w -= f;

}
/*----------------------------------------------------------------*/
VSVector3W VSVector3W::operator * (VSREAL f)const
{
	return VSVector3W(x*f, y*f, z*f,w*f);	
}
/*----------------------------------------------------------------*/
VSVector3W VSVector3W::operator / (VSREAL f)const
{

	return VSVector3W(x/f, y/f, z/f,w/f);

}
/*----------------------------------------------------------------*/
VSVector3W VSVector3W::operator + (VSREAL f)const
{
	return VSVector3W(x+f, y+f, z+f,w+f);
}
/*----------------------------------------------------------------*/
VSVector3W VSVector3W::operator - (VSREAL f)const
{

	return VSVector3W(x-f, y-f, z-f,w-f);

}
/*----------------------------------------------------------------*/
VSVector3W VSVector3W::operator + (const VSVector3W &v)const
{

	return VSVector3W(x+v.x, y+v.y, z+v.z,w+v.w);

}
/*----------------------------------------------------------------*/
VSVector3W VSVector3W::operator - (const VSVector3W &v)const
{

	return VSVector3W(x-v.x, y-v.y, z-v.z,w-v.w);

}
/*----------------------------------------------------------------*/
VSVector3W VSVector3W::operator * (const VSVector3W &v)const
{

	return VSVector3W(x*v.x, y*v.y, z*v.z,w*v.w);

}
/*----------------------------------------------------------------*/
VSVector3W VSVector3W::operator / (const VSVector3W &v)const
{

	return VSVector3W(x-v.x, y-v.y, z-v.z,w-v.w);

}
/*----------------------------------------------------------------*/
bool VSVector3W::operator ==(const VSVector3W &v)const
{
// 	for (unsigned int i = 0 ; i < 4 ; i++)
// 	{
// 		if (ABS(m[i] - v.m[i]) > EPSILON_E4)
// 		{
// 			return false;
// 		}
// 	}
// 	return true;
	return (ABS(x-v.x) < EPSILON_E4 && 
		ABS(y-v.y) < EPSILON_E4 &&
		ABS(z-v.z) < EPSILON_E4 &&
		ABS(w-v.w) < EPSILON_E4 );

}
/*----------------------------------------------------------------*/
DWORD VSVector3W::GetDWARGB()const
{
	unsigned char R,G,B,A;
	GetUCColor(A,R,G,B);
	
	return (DWORD)(((((A)&0xff)<<24)|(((R)&0xff)<<16)|(((G)&0xff)<<8)|((B)&0xff)));
}
/*----------------------------------------------------------------*/
DWORD VSVector3W::GetDWRGBA()const
{
	unsigned char R,G,B,A;
	GetUCColor(A,R,G,B);

	return (DWORD)(((((R)&0xff)<<24)|(((G)&0xff)<<16)|(((B)&0xff)<<8)|((A)&0xff)));

}
/*----------------------------------------------------------------*/
DWORD VSVector3W::GetDWBGRA()const
{
	unsigned char R,G,B,A;
	GetUCColor(A,R,G,B);
	return ((DWORD)((((B)&0xff)<<24)|(((G)&0xff)<<16)|(((R)&0xff)<<8)|((A)&0xff)));
}
/*----------------------------------------------------------------*/
DWORD VSVector3W::GetDWABGR()const
{
	unsigned char R,G,B,A;
	GetUCColor(A,R,G,B);
	return ((DWORD)((((A)&0xff)<<24)|(((B)&0xff)<<16)|(((G)&0xff)<<8)|((R)&0xff)));
}
/*----------------------------------------------------------------*/
void VSVector3W::GetUCColor(unsigned char &A,unsigned char &R,unsigned char &G,unsigned char &B)const
{
	A = (unsigned char)(a * 255);
	R = (unsigned char)(r * 255);
	G = (unsigned char)(g * 255);
	B = (unsigned char)(b * 255);
	
}
/*----------------------------------------------------------------*/


