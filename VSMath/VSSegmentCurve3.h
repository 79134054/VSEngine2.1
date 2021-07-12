#pragma once
#include "VSCurve3.h"
#include "VSSegment3.h"
/*
ÇúÏß¶Î

*/
namespace VSEngine2
{
	class VSMATH_API VSSegmentCurve3:public VSCurve3
	{
	public:
		VSSegmentCurve3();
		~VSSegmentCurve3();
		void Set(const VSSegment3 &Segment);
		VSSegment3 & GetSegment();
		virtual unsigned int GetCurveType(){return CT_SEGMENT;}
		virtual void GetFrenetFrameBNT(VSREAL t,VSVector3 & B,VSVector3 & N,VSVector3 &T);
		inline virtual VSVector3	GetFirstDerivative(VSREAL t);
		inline virtual VSVector3	GetSecondDerivative(VSREAL t);
		inline virtual VSVector3	GetThirdDerivative(VSREAL t);
		inline virtual VSREAL		GetLength(VSREAL t1 ,VSREAL t2,unsigned int uiIterations);
		inline virtual VSVector3	GetPoint(VSREAL t);
		inline virtual VSREAL		GetTotalLength(unsigned int uiIterations);
		inline virtual VSREAL GetTime (VSREAL fLength, unsigned int uiIterations = 32,
										VSREAL fTolerance = EPSILON_E4);
		inline virtual VSREAL GetCurvature(VSREAL fTime);
		inline virtual VSREAL GetTorsion(VSREAL fTime);

	private:
		VSSegment3 m_Segment;
	};

#include "VSSegmentCurve3.inl"
}