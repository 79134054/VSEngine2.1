#pragma once
#include "VSControlSurface3.h"
namespace VSEngine2
{
class VSMATH_API VSB_SplineSurface3 : public VSControlSurface3
{
public:
	VSB_SplineSurface3();
	virtual ~VSB_SplineSurface3();
	bool Copy(const VSB_SplineSurface3 & B_SplineSurface3);
	bool Set(const VSVector3 * pControlPoint,unsigned int uiRow,unsigned int uiColumn);

	bool SetUKnotVector(const VSREAL * pUKnotVector,unsigned int uiUDegree,unsigned int uiRow);
	bool SetVKnotVector(const VSREAL * pVKnotVector,unsigned int uiVDegree,unsigned int uiColumn);

	bool CreateOpenUniformUKnotVector(unsigned int uiDegree,unsigned int uiRow);
	bool CreatePeriodicUKnotVector(unsigned int uiDegree,unsigned int uiRow);

	bool CreateOpenUniformVKnotVector(unsigned int uiDegree,unsigned int uiColumn);
	bool CreatePeriodicVKnotVector(unsigned int uiDegree,unsigned int uiColumn);
	int GetUKey(VSREAL t)const;
	int GetVKey(VSREAL t)const;

	virtual VSVector3 GetPoint(VSREAL U,VSREAL V) = 0;
	virtual VSVector3 GetUPartialDerivative(VSREAL U,VSREAL V) = 0;
	virtual VSVector3 GetVPartialDerivative(VSREAL U,VSREAL V) = 0;
public:
	/***************************************inline************************************/
	inline bool SetUKnotVector(unsigned int i,VSREAL Value);
	inline const VSREAL * GetpUKnotVector()const;
	inline VSREAL GetUKnotVectorValue(unsigned int i)const;

	inline bool SetVKnotVector(unsigned int i,VSREAL Value);
	inline const VSREAL * GetpVKnotVector()const;
	inline VSREAL GetVKnotVectorValue(unsigned int i)const;
protected:
	inline void SetUBasisValue(int i, int j, VSREAL Value);
	inline VSREAL GetUBasisValue(int i ,int j)const;

	inline void SetUBasisFDValue(int i, int j, VSREAL Value);
	inline VSREAL GetUBasisFDValue(int i ,int j)const;


	inline unsigned int GetUDegree()const;
	inline unsigned int GetUKnotNum()const;


	inline void SetVBasisValue(int i, int j, VSREAL Value);
	inline VSREAL GetVBasisValue(int i ,int j)const;

	inline void SetVBasisFDValue(int i, int j, VSREAL Value);
	inline VSREAL GetVBasisFDValue(int i ,int j)const;


	inline unsigned int GetVDegree()const;
	inline unsigned int GetVKnotNum()const;

protected:
	bool GetUFuncValue(VSREAL t,int &i,unsigned int uiOrder);
	bool GetVFuncValue(VSREAL t,int &i,unsigned int uiOrder);

	VSREAL		* m_pUKnotVector;
	unsigned int m_uiUDegree;
	unsigned int m_uiUKnotNum;
	VSREAL		*m_pUBasis;
	VSREAL		*m_pUBasisFD;
	unsigned int m_uiUBasisNum;


	VSREAL		* m_pVKnotVector;
	unsigned int m_uiVDegree;
	unsigned int m_uiVKnotNum;
	VSREAL		*m_pVBasis;
	VSREAL		*m_pVBasisFD;
	unsigned int m_uiVBasisNum;
};
#include "VSB_SplineSurface3.inl"
}
