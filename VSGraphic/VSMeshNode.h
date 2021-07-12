#pragma once
#include "VSNode.h"
#include "VSResource.h"
namespace VSEngine2
{
	class VSGeometryNode;
	class VSSwitchNode;
	class VSStream;
	class VSGRAPHIC_API VSMeshNode : public VSNode
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:

		VSMeshNode();
		virtual ~VSMeshNode();
		inline void SetIsDrawBoundVolume(bool bIsDrawBoundVolume)
		{
			m_bIsDrawBoundVolume = bIsDrawBoundVolume;
		}
		inline unsigned int GetRenderGroup()const
		{
			return m_uiRenderGroup;
		}
		inline void SetRenderGroup(unsigned int uiRenderGroup)
		{
			m_uiRenderGroup = uiRenderGroup;
		}
		virtual bool SetDrawInstance(bool bDrawInstance) {return false;}
		virtual void UpdateView(VSCuller & Culler,double dAppTime);

		virtual void ComputeNodeVisibleSet(VSCuller & Culler,bool bNoCull,double dAppTime);
		bool m_bReceiveShadow;
		bool m_bCastShadow;
		bool m_bLighted;
		inline bool GetDrawInstance()const
		{
			return m_bDrawInstance;
		}
	protected:
		bool m_bDrawInstance;
		bool m_bIsDrawBoundVolume;
		unsigned int m_uiRenderGroup;
	};
	DECLARE_Ptr(VSMeshNode);
	VSTYPE_MARCO(VSMeshNode);
}
