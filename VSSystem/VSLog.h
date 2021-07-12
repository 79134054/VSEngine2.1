#pragma once
#include "VSFile.h"
namespace VSEngine2
{

	class VSSYSTEM_API VSLog : public VSFile
	{
	public:
		VSLog();
		~VSLog();
		bool Open(const TCHAR * pFileName);
		bool WriteInfo(const TCHAR *pcString);

	};
}
