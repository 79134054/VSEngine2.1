#pragma once
#include "VSActor.h"
#include "VSCamera.h"
#include "VSSkeletonMeshComponent.h"
#include "VSViewFamily.h"
namespace VSEngine2
{
	class VSRootMotionActor : public VSActor
	{
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSRootMotionActor();
		~VSRootMotionActor();
		virtual void CreateDefaultComponentNode();
		virtual void Update(double dAppTime);
		virtual void ProcessInput(unsigned int uiInputType, unsigned int uiEvent, unsigned int uiKey, int x, int y, int z);
	protected:
		VSSkeletonMeshComponent *	m_pMonsterMC;
	};
	DECLARE_Ptr(VSRootMotionActor);
	VSTYPE_MARCO(VSRootMotionActor);
}
