#pragma once
#include "VSNode.h"

namespace VSEngine2
{
	class VSSocketNode;
	class VSGRAPHIC_API VSNodeComponent : public VSNode
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSNodeComponent();
		virtual ~VSNodeComponent();
		
		friend class VSSocketNode;
		virtual bool IsNeedDraw()
		{
			return false;
		}
		template<typename T>
		static T * CreateComponent();
		virtual void PostCreate(){};
		virtual void OnDestroy();
		void AttachParentSocket(const VSUsedName & AttackSocketName);
		virtual bool PostLoad(VSStream* pStream);
		virtual bool PostClone(VSObject * pObjectSrc);
		virtual void UpdateTransform(double dAppTime);
		virtual void UpdateLightState(double dAppTime);
		virtual void UpdateCameraState(double dAppTime);
		virtual void GetStreamResource(VSArray<VSResourceProxyBase*>& pResourceProxy, StreamInformation_TYPE& StreamInformation)const;
	protected:
		virtual void UpdateNodeAll(double dAppTime);
		VSUsedName m_AttachSocketName;
		VSSocketNode *  m_pAttachSocket;
	};
 	DECLARE_Ptr(VSNodeComponent);
 	VSTYPE_MARCO(VSNodeComponent);

	template<typename T>
	T * VSNodeComponent::CreateComponent()
	{
		
		if (T::ms_Type.IsDerived(VSNodeComponent::ms_Type))
		{
			T * Component = (T *)VSObject::GetInstance<T>();
			Component->PostCreate();
			return Component;
		}
		else
		{
			return NULL;
		}
	}
}

