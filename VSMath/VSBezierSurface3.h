#pragma once
#include "VSControlSurface3.h"
/*
Beizer���棬��2ά���Ƶ��2������ʽ���ƣ�U,V��0-1֮��



*/
namespace VSEngine2
{
class VSMATH_API VSBezierSurface3 : VSControlSurface3
{
public:
	VSBezierSurface3();
	virtual ~VSBezierSurface3();
	//���ÿ��Ƶ�
	bool Set(const VSVector3 * pControlPoint,unsigned int uiRow,unsigned int uiColumn);
	//����
	bool Copy(const VSBezierSurface3 & BezierSurface3);
	//���
	virtual VSVector3 GetPoint(VSREAL U,VSREAL V);
	//��ƫ��
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
