#include "VSOutputResource.h"
#include "VSGraphicInclude.h"
using namespace VSEngine2;
IMPLEMENT_RTTI_NoCreateFun(VSOutputResource, VSBind)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSOutputResource)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(VSOutputResource, VSBind)
END_ADD_PROPERTY
VSOutputResource::VSOutputResource()
{
	m_pCreateBy = NULL;
}
VSOutputResource::~VSOutputResource()
{
	m_pCreateBy = NULL;
}
