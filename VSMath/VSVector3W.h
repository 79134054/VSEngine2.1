#pragma once
#include "VSMath.h"
#include "VSVector3.h"
namespace VSEngine2
{
	class VSMatrix3X3W;
	class VSMATH_API VSHalfVector3W
	{
	public:
		union
		{
			unsigned short m[4];
			struct
			{
				unsigned short x, y, z, w;
			};
			struct
			{
				unsigned short r, g, b, a;
			};
		};
		VSHalfVector3W operator * (VSREAL f)const;
		VSHalfVector3W operator + (const VSHalfVector3W &v)const;
		VSHalfVector3W();
		VSHalfVector3W(unsigned short _x, unsigned short _y, unsigned short _z, unsigned short _w);
	};
	//4ÏòÁ¿ ÑÕÉ«
	class  VSMATH_API VSVector3W 
	{
	public:
		union
		{
			VSREAL m[4];
			struct  
			{
				VSREAL x, y, z, w;
			};
			struct  
			{
				VSREAL r,g,b,a;
			};
		};


		VSVector3W(void);
		VSVector3W(VSREAL _x, VSREAL _y, VSREAL _z,VSREAL _w = 1.0f);
		VSVector3W(const VSVector3 & V);

		VSVector3W(unsigned char R,unsigned char G,unsigned char B,unsigned char A = 255);
		inline void GetV3(VSVector3 & V)const;
		inline VSVector3 GetV3()const;
		inline void Set(const VSVector3 & V);
		inline void Set(VSREAL _x, VSREAL _y, VSREAL _z, VSREAL _w = 1.0f);    
		VSVector3W operator * (const VSMatrix3X3W &m)const; 
		
		void operator += (const VSVector3W &v);          
		void operator -= (const VSVector3W &v);          
		void operator *= (const VSVector3W &v);          
		void operator /= (const VSVector3W &v); 

		void operator *= (VSREAL f);                     
		void operator /= (VSREAL f);                     
		void operator += (VSREAL f);                     
		void operator -= (VSREAL f);                     

		

		VSVector3W operator * (VSREAL f)const;            
		VSVector3W operator / (VSREAL f)const;            
		VSVector3W operator + (VSREAL f)const;            
		VSVector3W operator - (VSREAL f)const;            


		VSVector3W operator + (const VSVector3W &v)const; 
		VSVector3W operator - (const VSVector3W &v)const;
		VSVector3W operator * (const VSVector3W &v)const;
		VSVector3W operator / (const VSVector3W &v)const;

		bool operator ==(const VSVector3W &v)const;

		DWORD GetDWARGB()const;
		DWORD GetDWRGBA()const;

		DWORD GetDWBGRA()const;
		DWORD GetDWABGR()const;
		void GetUCColor(unsigned char &R,unsigned char &G,unsigned char &B,unsigned char &A)const;

		void CreateFromARGB(DWORD ARGB);
		void CreateFromBGRA(DWORD BGRA);
		void CreateFromRGBA(DWORD RGBA);
		void CreateFromABGR(DWORD ABGR);
	};  // class

	typedef class VSVector3W VSColorRGBA;
	inline  DWORD VSDWCOLORARGB(unsigned char a, unsigned char r, unsigned char g,unsigned char b) 
	{
		return ((DWORD)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)));
	}
	inline  DWORD VSDWCOLORBGRA(unsigned char a, unsigned char r, unsigned char g,unsigned char b) 
	{
		return ((DWORD)((((b)&0xff)<<24)|(((g)&0xff)<<16)|(((r)&0xff)<<8)|((a)&0xff)));
	}
	inline  DWORD VSDWCOLORRGBA(unsigned char a, unsigned char r, unsigned char g,unsigned char b) 
	{
		return ((DWORD)((((r)&0xff)<<24)|(((g)&0xff)<<16)|(((b)&0xff)<<8)|((a)&0xff)));
	}
	inline  DWORD VSDWCOLORABGR(unsigned char a, unsigned char r, unsigned char g,unsigned char b) 
	{
		return ((DWORD)((((a)&0xff)<<24)|(((b)&0xff)<<16)|(((g)&0xff)<<8)|((r)&0xff)));
	}

	inline  void VSDWCOLORGetARGB(DWORD ARGB,unsigned char &a, unsigned char &r, unsigned char &g,unsigned char &b) 
	{
		a = (ARGB>>24) & 0xff;
		r = (ARGB>>16) & 0xff;
		g = (ARGB>>8) & 0xff;
		b = (ARGB) & 0xff;
	}
	inline  void VSDWCOLORGetBGRA(DWORD BGRA,unsigned char &a, unsigned char &r, unsigned char &g,unsigned char &b) 
	{
		b = (BGRA>>24) & 0xff;
		g = (BGRA>>16) & 0xff;
		r = (BGRA>>8) & 0xff;
		a = (BGRA) & 0xff;
	}
	inline  void VSDWCOLORGetRGBA(DWORD RGBA,unsigned char &a, unsigned char &r, unsigned char &g,unsigned char &b) 
	{
		r = (RGBA>>24) & 0xff;
		g = (RGBA>>16) & 0xff;
		b = (RGBA>>8) & 0xff;
		a = (RGBA) & 0xff;
	}
	inline  void VSDWCOLORGetABGR(DWORD ABGR,unsigned char &a, unsigned char &r, unsigned char &g,unsigned char &b) 
	{
		a = (ABGR>>24) & 0xff;
		b = (ABGR>>16) & 0xff;
		g = (ABGR>>8) & 0xff;
		r = (ABGR) & 0xff;
	}
	inline VSHalfVector3W FloatToHalf(const VSVector3W & v)
	{
		return VSHalfVector3W(FloatToHalf(v.x), FloatToHalf(v.y), FloatToHalf(v.z), FloatToHalf(v.w));
	}
	inline VSVector3W HalfToFloat(const VSHalfVector3W & v)
	{
		return VSVector3W(HalfToFloat(v.x), HalfToFloat(v.y), HalfToFloat(v.z), HalfToFloat(v.w));
	}
	#include "VSVector3W.inl"
}