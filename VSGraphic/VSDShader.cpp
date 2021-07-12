#include "VSDShader.h"
#include "VSGraphicInclude.h"
#include "VSStream.h"
using namespace VSEngine2;
IMPLEMENT_RTTI(VSDShader,VSShader)
BEGIN_ADD_PROPERTY(VSDShader,VSShader)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSDShader)
IMPLEMENT_INITIAL_END
VSPointer<VSDShader> VSDShader::ms_Default;
VSDShader::VSDShader()
{

}
VSDShader::~VSDShader()
{

}
VSDShader::VSDShader(const TCHAR * pBuffer,const VSString & MainFunName,bool IsFromFile)
:VSShader(pBuffer,MainFunName,IsFromFile)
{

}
VSDShader::VSDShader(const VSString &Buffer,const VSString & MainFunName,bool IsFromFile)
:VSShader(Buffer,MainFunName,IsFromFile)
{

}
bool VSDShader::OnLoadResource(VSResourceIdentifier *&pID)
{
	VSMAC_ASSERT(m_pUser);

	if(!m_pUser->OnLoadDShaderProgram(this,pID))
		return 0;

	return 1;
}