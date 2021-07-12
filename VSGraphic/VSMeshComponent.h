#pragma once
#include "VSNodeComponent.h"
namespace VSEngine2
{

	DECLARE_Ptr(VSModelMeshNode);
	class VSGRAPHIC_API VSMeshComponent : public VSNodeComponent
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSMeshComponent();
		virtual ~VSMeshComponent() = 0;
		virtual bool IsNeedDraw()
		{
			return true;
		}
	protected:
		virtual void UpdateWorldBound(double dAppTime);//更新世界边界体 

		virtual void UpdateNodeAll(double dAppTime);
		virtual void ComputeNodeVisibleSet(VSCuller & Culler, bool bNoCull, double dAppTime);
		virtual void SetPostLoadNodeParam();
	public:
		virtual void SetIsVisibleUpdate(bool bIsVisibleUpdate);
		virtual void CreateLocalAABB();
		void SetIsDrawBoundVolume(bool bIsDrawBoundVolume);

		void SetMorphData(const VSUsedName & ShowName, float fPara);

		void CastShadow(bool bCastShadow);
		void UpdateView(VSCuller& Culler, double dAppTime);

		VSMaterialInstance * GetUseMaterialInstance(unsigned int SubMeshID, unsigned int LOD = 0)const;

		VSMaterialInstance * GetMaterialInstance(unsigned int SubMeshID,unsigned int UseID, unsigned int LOD = 0)const;

		bool SetUseMaterialInstance(unsigned int SubMeshID, unsigned int UseID);

		unsigned int GetSubMeshNum();

		virtual void GetStreamResource(VSArray<VSResourceProxyBase*>& pResourceProxy, StreamInformation_TYPE& StreamInformation)const;
	protected:
		VSModelMeshNodePtr m_pNode;
		VSArray<unsigned int> m_UseID;
		bool m_bCastShadow;
		void ResetUseID();		
	};
	DECLARE_Ptr(VSMeshComponent);
	VSTYPE_MARCO(VSMeshComponent);
}
