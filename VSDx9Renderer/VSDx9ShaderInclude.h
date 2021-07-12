#pragma once
#include "VSMap.h"
#include "VSString.h"
#ifdef WINDOWS_PLATFORM
#include <d3dx9shader.h>
#endif
namespace VSEngine2
{
#ifdef WINDOWS_PLATFORM	
	class VSDx9ShaderInclude : public ID3DXInclude
	{
	public:
		VSDx9ShaderInclude();
		virtual ~VSDx9ShaderInclude();
		STDMETHOD(Open)(THIS_ D3DXINCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes);
		STDMETHOD(Close)(THIS_ LPCVOID pData);	
	private:
		VSMap<VSString,VSString> m_FileMap;
	};
#endif
};
