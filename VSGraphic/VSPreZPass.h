#pragma once
#include "VSPass.h"
namespace VSEngine2
{

	class VSGRAPHIC_API VSPreZPass : public VSPass
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSPreZPass();
		virtual ~VSPreZPass();
		

	protected:
		static VSPointer<VSPreZPass>	ms_Default;
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
	protected:
		VSRenderState m_RenderState;
	public:
		virtual bool Draw(VSRenderer * pRenderer);
		
		static VSPreZPass * GetDefault()
		{
			return ms_Default;
		}
		virtual unsigned int GetPassType()
		{
			return PT_PREZ;
		}

	};
	DECLARE_Ptr(VSPreZPass);
	VSTYPE_MARCO(VSPreZPass);
}
