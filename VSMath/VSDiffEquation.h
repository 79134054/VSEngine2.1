#pragma once
#include "VSFunction1.h"
/*
	��һԪ΢�ַ���


*/
namespace VSEngine2
{
class VSMATH_API VSDiffEquation
{
public:
	VSDiffEquation();
	~VSDiffEquation();
	//����ԭ����
	inline void SetFunction1(VSFunction1 * Func){if(!Func) return ; m_pFunc = Func;}
	inline VSFunction1 *GetFunc(){return m_pFunc;}
    //���ò���
	inline void SetStep(VSREAL Step){if(ABS(Step) < EPSILON_E4) return; m_Step = Step;}
	inline VSREAL GetStep(){return m_Step;}
	//���ÿ�ʼ�Ա���
	inline void SetBeginT(VSREAL BeginT){m_BeginT = BeginT;}
	inline VSREAL GetBeginT(){return m_BeginT;}
	//���ÿ�ʼֵ
	inline void SetBeginX(VSREAL BeginX){m_BeginX = BeginX;}
	inline VSREAL GetBeginX(){return m_BeginX;}
	//����
	void Update();
protected:
	VSFunction1 * m_pFunc;
	VSREAL m_Step;
	VSREAL m_BeginT;
	VSREAL m_BeginX;

};



}
