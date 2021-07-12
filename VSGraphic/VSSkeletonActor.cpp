#include "VSSkeletonActor.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSSkeletonActor,VSActor)
BEGIN_ADD_PROPERTY(VSSkeletonActor,VSActor)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSSkeletonActor)
IMPLEMENT_INITIAL_END
VSSkeletonActor::VSSkeletonActor()
{

}
VSSkeletonActor::~VSSkeletonActor()
{

}
void VSSkeletonActor::CreateDefaultComponentNode()
{
	m_pNode = VSNodeComponent::CreateComponent<VSSkeletonMeshComponent>();
}
