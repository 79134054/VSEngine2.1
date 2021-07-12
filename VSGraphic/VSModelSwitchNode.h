#pragma once
#include "VSSwitchNode.h"
#include "VSGeometryNode.h"
namespace VSEngine2
{
	class VSGRAPHIC_API VSModelSwitchNode : public VSSwitchNode
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSModelSwitchNode();
		virtual ~VSModelSwitchNode();
	public:
		virtual VSGeometryNode * GetGeometryNode(unsigned int uiLodLevel);
		virtual void UpdateView(VSCuller & Culler, double dAppTime);
		void SetUseMaterialInstance(unsigned int SubMeshID, unsigned int UsedID);
		virtual void GetStreamResource(VSArray<VSResourceProxyBase*>& pResourceProxy, StreamInformation_TYPE& StreamInformation)const;
	};
	DECLARE_Ptr(VSModelSwitchNode);
	VSTYPE_MARCO(VSModelSwitchNode);

}
