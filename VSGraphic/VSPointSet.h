#pragma once
#include "VSMeshData.h"
#include "VSVector3.h"
namespace VSEngine2
{
	class VSGRAPHIC_API VSPointSet : public VSMeshData
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSPointSet();
		VSPointSet(const VSVector3 & Point,VSREAL fSize);
		virtual ~VSPointSet();
		bool CreateIndex();
		virtual unsigned int GetTotalNum()const;
		virtual unsigned int GetGirdNum(unsigned int uiInputNum)const;


	
	
	};
	DECLARE_Ptr(VSPointSet);
	VSTYPE_MARCO(VSPointSet);

}
