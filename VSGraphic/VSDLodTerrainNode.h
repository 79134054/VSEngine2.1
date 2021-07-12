#pragma once
#include "VSTerrainNode.h"
namespace VSEngine2
{
	class VSGRAPHIC_API VSDLodTerrainNode : public VSTerrainNode
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		
		VSDLodTerrainNode();
		virtual ~VSDLodTerrainNode();
	public:
		inline void SetDLodExtend(int uiDLodExtend)
		{
			m_uiDLodExtend = uiDLodExtend;
		}
		inline int GetDLodExtend()const
		{
			return m_uiDLodExtend;
		}

		inline void SetDLodScale(VSREAL fDLodScale)
		{
			m_fDLodScale = fDLodScale;
		}

		inline VSREAL GetDLodScale()const { return m_fDLodScale; }
		
	protected:
		int m_uiDLodExtend;
		VSREAL m_fDLodScale;
		virtual bool CreateChild();
		virtual void UpdateNodeAll(double dAppTime);
	public:
		
	};
	DECLARE_Ptr(VSDLodTerrainNode);
	VSTYPE_MARCO(VSDLodTerrainNode);


}
