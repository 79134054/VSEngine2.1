#pragma once
#include "VSAnimBlendFunction.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSTwoParamAnimBlend : public VSAnimBlendFunction
	{
		//RTTI
		DECLARE_RTTI
		DECLARE_INITIAL
	public:
		virtual ~VSTwoParamAnimBlend();

		VSTwoParamAnimBlend(const VSUsedName & ShowName,VSAnimTree * pAnimTree);
		void CreateSlot(unsigned int uiWidth, unsigned int uiHeight);
		virtual bool SetObject(VSObject * pObject);
		static void LineBlendTwoAll(VSTwoParamAnimBlend * pOut, 
			VSAnimFunction * pAnimFunction1, VSAnimFunction * pAnimFunction2,
			VSAnimFunction * pAnimFunction3, VSAnimFunction * pAnimFunction4,
			VSREAL fWeight1, VSREAL fWeight2);
	protected:
		VSREAL m_fParam[2];
		VSREAL m_fParamMax[2];
		VSREAL m_fParamMin[2];
		VSArray<VSAnimAtom> m_BlendBoneMatrix[2];
		VSAnimAtom m_BlendRootMatrix[2];
		unsigned int m_uiWidth;
		unsigned int m_uiHeight;
		
		VSTwoParamAnimBlend();
		virtual bool ComputePara(double dAppTime);
		virtual bool ComputeOutBoneMatrix(double dAppTime);
		VSAnimFunction * GetAnimFunction(unsigned int i, unsigned int j);
	private:
		static void LineBlendTwo(VSArray<VSAnimAtom> *pOut,
			const VSArray<VSAnimAtom> * pIn1, const VSArray<VSAnimAtom> *pIn2, VSREAL fWeight);
		static void LineBlendTwo(VSArray<VSAnimAtom> *pOut,
			VSAnimFunction * pAnimFunction1, VSAnimFunction * pAnimFunction2, VSREAL fWeight);
		static bool LineBlendTwoRoot(VSAnimAtom &pOut,
			VSAnimFunction * pAnimFunction1, VSAnimFunction * pAnimFunction2, VSREAL fWeight);
	};
	DECLARE_Ptr(VSTwoParamAnimBlend);
	VSTYPE_MARCO(VSTwoParamAnimBlend);
}
