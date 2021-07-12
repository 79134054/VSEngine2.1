#pragma once
#include "VSTerrainNode.h"
#include "VSTexAllState.h"
namespace VSEngine2
{
	class VSGRAPHIC_API VSGPULodTerrainNode : public VSTerrainNode
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:

		VSGPULodTerrainNode();
		virtual ~VSGPULodTerrainNode();
	public:
// 		inline void SetDLodExtend(int uiDLodExtend)
// 		{
// 			m_uiDLodExtend = uiDLodExtend;
// 		}
// 		inline int GetDLodExtend()const
// 		{
// 			return m_uiDLodExtend;
// 		}
// 
// 		inline void SetDLodScale(VSREAL fDLodScale)
// 		{
// 			m_fDLodScale = fDLodScale;
// 		}
// 
// 		inline VSREAL GetDLodScale()const { return m_fDLodScale; }

	protected:
// 		int m_uiDLodExtend;
// 		VSREAL m_fDLodScale;
		VSTexAllStatePtr m_LodTexture;
		virtual bool CreateChild();
		virtual void UpdateNodeAll(double dAppTime);
	public:
		virtual bool CreateTerrain(unsigned int uiNumX, unsigned int uiNumZ, unsigned int uiTessellationLevel);
	};
	DECLARE_Ptr(VSGPULodTerrainNode);
	VSTYPE_MARCO(VSGPULodTerrainNode);


}
