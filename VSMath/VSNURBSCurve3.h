#pragma once
#include "VSB_SplineCurve3.h"
/*
NURB������
*/
namespace VSEngine2
{
class VSMATH_API VSNURBSCurve3 : public VSB_SplineCurve3
{
public:
	VSNURBSCurve3();
	virtual ~VSNURBSCurve3();
	//����Ȩ��
	bool Set(const VSREAL * pWeight,unsigned int uiControlPointNum);
	//����
	bool Copy(const VSNURBSCurve3 &NURBSCurve3);
	/***************************************inline************************************/
	//�ı�Ȩ��
	inline bool ChangeWeight(unsigned int i,VSREAL Weight);
	//��ȡȨ��ָ��
	inline const VSREAL * GetWeight()const;
	//�õ�Ȩ��
	inline VSREAL GetWeight(unsigned int i)const;
	//ȡ�õ���
	virtual VSVector3	GetFirstDerivative(VSREAL t);
	virtual VSVector3	GetSecondDerivative(VSREAL t);
	virtual VSVector3	GetThirdDerivative(VSREAL t);
	//virtual VSREAL		GetLength(VSREAL t1 ,VSREAL t2);
	//�õ���
	virtual VSVector3	GetPoint(VSREAL t);
protected:
	bool Get(unsigned int Order,VSREAL t ,VSVector3 *D0,VSVector3 *D1,VSVector3 *D2,VSVector3 *D3);
private:
	VSREAL *m_pWeight;

};
#include "VSNURBSCurve3.inl"
}