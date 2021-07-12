#pragma once
#include "VSObject.h"
namespace VSEngine2
{
	struct PropertyReplaceType
	{
		VSArrayOrder<VSUsedName> ReplaceProperty;
	};
	struct ClassReplaceType
	{
		VSArrayOrder<VSUsedName> ReplaceClass;
		VSMapOrder<VSUsedName, PropertyReplaceType> ProperyReplaceMap;
	};
	class VSGRAPHIC_API VSConfig
	{
		//PRIORITY
		DECLARE_PRIORITY
	public:
		VSConfig();
		~VSConfig();
		DECLARE_INITIAL_ONLY
		typedef VSMapOrder < VSUsedName, ClassReplaceType> CLASS_REPLACE_TYPE;
	private:
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
		static void InitConfig();
		static void InitEnginePath();
		static void InitParamConfig();
		static void InitClassReplace();
		static void Inner_InitClassReplace();
		static CLASS_REPLACE_TYPE ms_ClassReplaceMap;
		static VSMapOrder<VSUsedName, VSUsedName> ms_ClassReplace;
		static VSMapOrder<VSUsedName, VSMapOrder<unsigned int, unsigned int>> ms_PropertyReplace;
	public:
		
		static bool ms_EnableAsynLoad;
		static bool ms_EnableAdvanceInstance;
		static VSString ms_OutputShaderCodePath;
		static VSString ms_OutputLogPath;
		static VSString ms_TerrainPath;
		static VSString ms_ResourcePath;
		static VSREAL ms_LODScreenScale;
		

		static bool GetPropertyReplace(VSString & ClassName, VSMapOrder<unsigned int, unsigned int> * &pPropertyReplace);
		static bool GetClassReplace(VSString & ClassName);
		static bool GetPropertyReplace(VSMapOrder<unsigned int, unsigned int> * pPropertyReplace, unsigned int &NameID);
	};
}
