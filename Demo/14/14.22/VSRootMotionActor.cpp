#include "VSRootMotionActor.h"
#include "VSGraphicInclude.h"
#include "VSViewFamily.h"
#include "VSWorld.h"
#include "VSEngineInput.h"
#include "VSTimer.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSRootMotionActor, VSActor)
BEGIN_ADD_PROPERTY(VSRootMotionActor, VSActor)
REGISTER_PROPERTY(m_pMonsterMC, MonsterMC, VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSRootMotionActor)
IMPLEMENT_INITIAL_END
VSRootMotionActor::VSRootMotionActor()
{
	m_pMonsterMC = NULL;

}
VSRootMotionActor::~VSRootMotionActor()
{
	m_pMonsterMC = NULL;

	
}
void VSRootMotionActor::CreateDefaultComponentNode()
{
	VSActor::CreateDefaultComponentNode();

	m_pMonsterMC = AddComponentNode<VSSkeletonMeshComponent>();
	VSSkeletonMeshNodeRPtr  pResource = VSResourceManager::LoadASYNResource<VSSkeletonMeshNode>(_T("MannequinWithAnim.SKMODEL"), false);
	if (pResource)
	{

		m_pMonsterMC->SetSkeletonMeshResource(pResource);
		m_pMonsterMC->SetIsDrawBoundVolume(true);
		m_pMonsterMC->SetIsDrawSkeleton(true);
		m_pMonsterMC->PlayAnim(_T("RootMotion"), 1.0f, VSController::RT_CLAMP);

	}

}
void VSRootMotionActor::Update(double dAppTime)
{
	const VSAnimAtom & Delta = m_pMonsterMC->GetRootDelta();
 	VSTransform DeltaT = VSAnimAtom::VSEngineFrom3DMax(Delta);
 	const VSTransform& T = GetWorldTransform();
 	
	VSVector3 Scale = T.GetScale() * DeltaT.GetScale();
	VSMatrix3X3 Rotator = T.GetRotate() * DeltaT.GetRotate();
	VSVector3 Trans = DeltaT.GetTranslate() * Rotator + T.GetTranslate();

	SetWorldScale(Scale);
	SetWorldRotate(Rotator);
	SetWorldPos(Trans);
}
void VSRootMotionActor::ProcessInput(unsigned int uiInputType, unsigned int uiEvent, unsigned int uiKey, int x, int y, int z)
{
	VSActor::ProcessInput(uiInputType, uiEvent, uiKey, x, y, z);

	if (uiInputType == VSEngineInput::IT_KEYBOARD)
	{
		if (uiEvent == VSEngineInput::IE_DOWN)
		{
			if (uiKey == VSEngineInput::BK_P)
			{
				m_pMonsterMC->PlayAnim(_T("RootMotion"), 1.0f, VSController::RT_CLAMP);
			}
		}
	}
	return;

}

