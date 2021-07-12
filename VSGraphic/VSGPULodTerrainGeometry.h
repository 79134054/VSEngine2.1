#pragma once
#include "VSGeometry.h"
namespace VSEngine2
{

	class VSGPULodTerrainNode;
	class VSGRAPHIC_API VSGPULodTerrainGeometry : public VSGeometry
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSGPULodTerrainGeometry();
		virtual ~VSGPULodTerrainGeometry();
		bool CreateMesh(unsigned int uiTileNumX, unsigned int uiTileNumZ);
	protected:
		VSGPULodTerrainNode * GetParentTerrainNode()const;
	};
	DECLARE_Ptr(VSGPULodTerrainGeometry);
	VSTYPE_MARCO(VSGPULodTerrainGeometry);
};
