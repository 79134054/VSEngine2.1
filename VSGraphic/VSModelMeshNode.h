#pragma once
#include "VSMeshNode.h"
#include "VSGeometryNode.h"
namespace VSEngine2
{
	class VSGeometryNode;
	class VSSwitchNode;
	class VSStream;
	class VSGRAPHIC_API VSModelMeshNode : public VSMeshNode
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:

		VSModelMeshNode();
		virtual ~VSModelMeshNode();

		virtual VSSwitchNode * GetLodNode()const;
		virtual VSGeometryNode * GetGeometryNode(unsigned int uiLodLevel);
		void SetUseMaterialInstance(unsigned int SubMeshID,unsigned int UsedID);
		virtual bool PostLoad(VSStream* pStream);
		virtual bool PostClone(VSObject * pObjectSrc);
		virtual void UpdateController(double dAppTime);
		void SetMorphData(const VSUsedName & ShowName, float fData);
		virtual bool SetDrawInstance(bool bDrawInstance);
		void ClearLodMesh();
		virtual void GetStreamResource(VSArray<VSResourceProxyBase*>& pResourceProxy, StreamInformation_TYPE& StreamInformation)const;
	protected:
		bool AddLODGeometryNode(VSGeometryNode* pGeoNode);

	};
	DECLARE_Ptr(VSModelMeshNode);
	VSTYPE_MARCO(VSModelMeshNode);
}
