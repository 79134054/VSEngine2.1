#include "VSBezierCurve3.h"
#include "VSMemManager.h"
using namespace VSEngine2;
/*----------------------------------------------------------------*/
VSBezierCurve3::VSBezierCurve3()
{
	m_pControlPoint = NULL;
	m_uiControlPointNum = 0;
	m_C = NULL;
	m_FDValue = NULL;
	m_SDValue = NULL;
	m_TDValue = NULL;
	m_CurveType = BEZIER;
}
/*----------------------------------------------------------------*/
VSBezierCurve3::~VSBezierCurve3()
{
	VSMAC_DELETEA(m_pControlPoint);
	m_uiControlPointNum = 0;
	VSMAC_DELETEA(m_C);
	VSMAC_DELETEA(m_FDValue);
	VSMAC_DELETEA(m_SDValue);
	VSMAC_DELETEA(m_TDValue);

}
/*----------------------------------------------------------------*/
bool VSBezierCurve3::Set(const VSVector3 * pControlPoint,unsigned int uiControlPointNum)
{
	VSMAC_ASSERT(pControlPoint && uiControlPointNum);

	VSMAC_DELETEA(m_pControlPoint);
	m_uiControlPointNum = uiControlPointNum;
	VSMAC_DELETEA(m_C);
	VSMAC_DELETEA(m_FDValue);
	VSMAC_DELETEA(m_SDValue);
	VSMAC_DELETEA(m_TDValue);

	m_pControlPoint = VS_NEW VSVector3[uiControlPointNum];
	VSMAC_ASSERT(m_pControlPoint);

	VSMemcpy(m_pControlPoint,pControlPoint,sizeof(VSVector3) * m_uiControlPointNum);


	m_C = VS_NEW VSREAL [uiControlPointNum * uiControlPointNum];
	VSMAC_ASSERT(m_C);

	VSMemset(m_C,0,sizeof(VSREAL) * uiControlPointNum * uiControlPointNum);
	SetC(0,0,1.0f);
	SetC(1,0,1.0f);
	SetC(1,1,1.0f);
	for (unsigned int i = 2; i <  uiControlPointNum ; i++)
	{
		SetC(i,0,1.0f);
		SetC(i,i,1.0f);
		
		for (unsigned int j = 1; j < i; j++)
		{

			SetC(i,j,GetC(i - 1,j - 1) + GetC(i - 1,j));
		}
	}
	
	m_FDValue = VS_NEW VSVector3[m_uiControlPointNum-1];
	VSMAC_ASSERT(m_FDValue);
	for (unsigned int i = 0; i < m_uiControlPointNum - 1; i++)
	{
		m_FDValue[i] = m_pControlPoint[i + 1] - m_pControlPoint[i];
	}

	
	m_SDValue = VS_NEW VSVector3[m_uiControlPointNum - 2];
	VSMAC_ASSERT(m_SDValue);
	for (unsigned int i = 0; i < m_uiControlPointNum - 2; i++)
	{
		m_SDValue[i] = m_FDValue[i + 1] - m_FDValue[i];
	}

	
	if (m_uiControlPointNum >= 4)
	{
		m_TDValue = VS_NEW VSVector3[m_uiControlPointNum - 3];
		if(!m_TDValue)
			return 0;
		for (unsigned int i = 0; i < m_uiControlPointNum - 3; i++)
		{
			m_TDValue[i] = m_SDValue[i + 1] - m_SDValue[i];
		}
	}
	return 1;
}
/*----------------------------------------------------------------*/
VSVector3	VSBezierCurve3::GetFirstDerivative(VSREAL t)
{
	VSMAC_ASSERT(m_FDValue);

	VSREAL fOmTime = (VSREAL)1.0 - t;
	VSREAL fPowTime = (VSREAL)1.0;
	
	unsigned int uiDegree = m_uiControlPointNum - 2;

	VSVector3 kResult(0,0,0);
	for (unsigned int i = 0; i <= uiDegree; i++)
	{
		VSREAL fCoeff = GetC(uiDegree,i) * fPowTime;
		VSREAL P =  VSREAL(uiDegree - i);
		kResult = kResult + m_FDValue[i] * fCoeff  * POW(fOmTime,P);
		fPowTime *= t;
	}
	kResult *= VSREAL(m_uiControlPointNum - 1);
	return kResult;

}
/*----------------------------------------------------------------*/
VSVector3	VSBezierCurve3::GetSecondDerivative(VSREAL t)
{
	VSMAC_ASSERT(m_SDValue);

	VSREAL fOmTime = (VSREAL)1.0 - t;
	VSREAL fPowTime = (VSREAL)1.0;

	unsigned int uiDegree = m_uiControlPointNum - 3;

	VSVector3 kResult(0,0,0);
	for (unsigned int i = 0; i <= uiDegree; i++)
	{
		VSREAL fCoeff = GetC(uiDegree,i) * fPowTime;
		VSREAL P =  VSREAL(uiDegree - i);
		kResult = kResult + m_SDValue[i] * fCoeff  * POW(fOmTime,P);
		fPowTime *= t;
	}
	kResult *= VSREAL((m_uiControlPointNum - 1) * (m_uiControlPointNum - 2));
	return kResult;

}
/*----------------------------------------------------------------*/
VSVector3	VSBezierCurve3::GetThirdDerivative(VSREAL t)
{
	VSMAC_ASSERT(m_TDValue);

	VSREAL fOmTime = (VSREAL)1.0 - t;
	VSREAL fPowTime = (VSREAL)1.0;
	
	unsigned int uiDegree = m_uiControlPointNum - 4;

	VSVector3 kResult(0,0,0);
	for (unsigned int i = 0; i <= uiDegree; i++)
	{
		VSREAL fCoeff = GetC(uiDegree,i) * fPowTime;
		VSREAL P =  VSREAL(uiDegree - i);
		kResult = kResult + m_TDValue[i] * fCoeff  * POW(fOmTime,P);
		fPowTime *= t;
	}
	kResult *= VSREAL((m_uiControlPointNum - 3) * (m_uiControlPointNum - 1) * (m_uiControlPointNum - 2));
	return kResult;

}
/*----------------------------------------------------------------*/
VSVector3	VSBezierCurve3::GetPoint(VSREAL t)
{
	VSREAL fOmTime = (VSREAL)1.0 - t;
	VSREAL fPowTime = (VSREAL)1.0;
	unsigned int uiDegree = m_uiControlPointNum - 1;

	VSVector3 kResult(0.0f,0.0f,0.0f);

	for (unsigned int i = 0; i <= uiDegree; i++)
	{
		VSREAL fCoeff = GetC(uiDegree,i) * fPowTime;
		VSREAL P =  VSREAL(uiDegree - i);
		kResult = kResult + m_pControlPoint[i] * fCoeff  * POW(fOmTime,P);
		fPowTime *= t;
	}


	return kResult;

}
/*----------------------------------------------------------------*/
bool VSBezierCurve3::Copy(const VSBezierCurve3 & BezierCurve3)
{
	
	if(!VSControlCurve3::Copy(BezierCurve3))
		return 0;
	VSMAC_ASSERT(BezierCurve3.m_C && BezierCurve3.m_FDValue && BezierCurve3.m_SDValue && BezierCurve3.m_TDValue);

	VSMAC_DELETEA(m_C);
	VSMAC_DELETEA(m_FDValue);
	VSMAC_DELETEA(m_SDValue);
	VSMAC_DELETEA(m_TDValue);
	m_C = VS_NEW VSREAL [m_uiControlPointNum * m_uiControlPointNum];
	VSMAC_ASSERT(m_C);

	VSMemcpy(m_C,BezierCurve3.m_C,sizeof(VSREAL) * m_uiControlPointNum * m_uiControlPointNum);

	m_FDValue = VS_NEW VSVector3[m_uiControlPointNum-1];
	VSMAC_ASSERT(m_FDValue);

	VSMemcpy(m_FDValue,BezierCurve3.m_FDValue,sizeof(VSVector3) * (m_uiControlPointNum - 1));

	m_SDValue = VS_NEW VSVector3[m_uiControlPointNum - 2];
	VSMAC_ASSERT(m_SDValue);

	VSMemcpy(m_SDValue,BezierCurve3.m_SDValue,sizeof(VSVector3) * (m_uiControlPointNum - 2));
	if (m_uiControlPointNum >= 4)
	{
		m_TDValue = VS_NEW VSVector3[m_uiControlPointNum - 3];
		VSMAC_ASSERT(m_TDValue);
		VSMemcpy(m_TDValue,BezierCurve3.m_TDValue,sizeof(VSVector3) * (m_uiControlPointNum - 1));
	}
	return 1;
}
/*----------------------------------------------------------------*/
