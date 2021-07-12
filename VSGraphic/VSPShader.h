#pragma once
#include "VSShader.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSPShader : public VSShader
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSPShader();
		VSPShader(const TCHAR * pBuffer,const VSString & MainFunName,bool IsFromFile = false);
		VSPShader(const VSString &Buffer,const VSString & MainFunName,bool IsFromFile = false);
		virtual ~VSPShader();
		virtual unsigned int GetShaderType()const
		{
			return VSEngineFlag::ST_PIXEL;
		}
	public:
		static const VSPShader *GetDefault()
		{
			return ms_Default;
		}
	protected:
		virtual bool OnLoadResource(VSResourceIdentifier *&pID);		
		static VSPointer<VSPShader> ms_Default;
	};
	DECLARE_Ptr(VSPShader);
	VSTYPE_MARCO(VSPShader);

}
