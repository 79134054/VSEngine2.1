#pragma once
#include "VSAnimBlendFunction.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSAdditiveBlend : public VSAnimBlendFunction
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL


	public:
		virtual ~VSAdditiveBlend();

		VSAdditiveBlend(const VSUsedName & ShowName, VSAnimTree * pAnimTree);

		virtual void AddInputNode();
		virtual void DeleteInputNode();

		VSAdditiveBlend();
		virtual bool ComputePara(double dAppTime){ return true; }
		virtual bool ComputeOutBoneMatrix(double dAppTime);
		virtual bool IsSupportSimpleInstance() { return false; }
	};
	DECLARE_Ptr(VSAdditiveBlend);
	VSTYPE_MARCO(VSAdditiveBlend);
}

