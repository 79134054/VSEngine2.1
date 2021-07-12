#pragma once
#include "VSShader.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSVShader : public VSShader
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSVShader();
		VSVShader(const TCHAR * pBuffer,const VSString & MainFunName,bool IsFromFile = false);
		VSVShader(const VSString &Buffer,const VSString & MainFunName,bool IsFromFile = false);
		virtual ~VSVShader();
		virtual unsigned int GetShaderType()const
		{
			return VSEngineFlag::ST_VERTEX;
		}
	public:
		static const VSVShader *GetDefault()
		{
			return ms_Default;
		}
	protected:
		virtual bool OnLoadResource(VSResourceIdentifier *&pID);		
		static VSPointer<VSVShader> ms_Default;
	};
	DECLARE_Ptr(VSVShader);
	VSTYPE_MARCO(VSVShader);
}
