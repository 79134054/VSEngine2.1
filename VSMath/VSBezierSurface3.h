#pragma once
#include "VSControlSurface3.h"
/*
Beizer曲面，由2维控制点和2个多项式控制，U,V在0-1之间



*/
namespace VSEngine2
{
class VSMATH_API VSBezierSurface3 : VSControlSurface3
{
public:
	VSBezierSurface3();
	virtual ~VSBezierSurface3();
	//设置控制点
	bool Set(const VSVector3 * pControlPoint,unsigned int uiRow,unsigned int uiColumn);
	//拷贝
	bool Copy(const VSBezierSurface3 & BezierSurface3);
	//求点
	virtual VSVector3 GetPoint(VSREAL U,VSREAL V);
	//求偏导
	virtual VSVector3 GetUPartialDerivative(VSREAL U,VSREAL V);
	virtual VSVector3 GetVPartialDerivative(VSREAL U,VSREAL V);
private:
	/***************************************inline************************************/
	inline VSREAL GetUC(int i,int j)const;
	inline VSREAL SetUC(int i,int j,VSREAL Value);
	inline VSREAL GetVC(int i,int j)const;
	inline VSREAL SetVC(int i,int j,VSREAL Value);
	inline void SetUPD(int i, int j,const VSVector3 & Value);
	inline VSVector3 GetUPD(int i, int j);
	inline void SetVPD(int i, int j,const VSVector3 & Value);
	inline VSVector3 GetVPD(int i, int j);
	VSREAL * m_UC;
	VSREAL * m_VC;
	VSVector3 * m_UPDValue;
	VSVector3 * m_VPDValue;
};

#include "VSBezierSurface3.inl"
}
