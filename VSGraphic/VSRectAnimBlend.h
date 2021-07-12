#pragma once
#include "VSTwoParamAnimBlend.h"
namespace VSEngine2
{
	/*
						
			LeftTop	 __________  RightTop
					|			|
					|			| 
					|			|
		LeftBottom	|_________	| RightBottom
					  


	   CrossFading IMMEDIATE 
					

				
					0 0,0____________1 0,1
						|				|
						|				|
						|				|
						|				|
						|				|
					2 1,0------------3 1,1

	 

	*/
	class VSStream;
	class VSGRAPHIC_API VSRectAnimBlend : public VSTwoParamAnimBlend
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
		

		enum	//Child Location
		{
			CL_LEFT_TOP,
			CL_RIGTH_TOP,
			CL_LEFT_BOTTOM,
			CL_RIGHT_BOTTOM,
			CL_MAX
		};

	public:
		virtual ~VSRectAnimBlend();

		VSRectAnimBlend(const VSUsedName & ShowName,VSAnimTree * pAnimTree);

	protected:
		VSRectAnimBlend();
		virtual bool ComputeOutBoneMatrix(double dAppTime);
	public:
		virtual bool SetObject(VSObject * pObject);

	};
	DECLARE_Ptr(VSRectAnimBlend);
	VSTYPE_MARCO(VSRectAnimBlend);
}
