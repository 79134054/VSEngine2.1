#include "VSStreamingType.h"
#include "VSGraphicInclude.h"
#include "VSRenderThread.h"
using namespace VSEngine2;
void VSStreamingType::ReleaseStreamObject()
{
	VSMAC_DELETE(m_pToStreamObject);
}