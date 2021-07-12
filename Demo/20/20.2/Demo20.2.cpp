//²âÊÔ°üº¬TextureµÄCS Shader
#ifndef VSSAMPLE_H
#define VSSAMPLE_H
#include "VSApplication.h"
#include "VSMaterial.h"
namespace VSEngine2
{

	const UINT NUM_ELEMENTS = 32;
	class VSTestCSDemo : public VSConsoleApplication
	{
	public:
		VSTestCSDemo();
		virtual ~VSTestCSDemo();
		DLCARE_APPLICATION(VSTestCSDemo);
	public:
		virtual bool PreInitial();
		virtual bool OnInitial();
		virtual bool OnTerminal();
		
		
		VSREAL g_vBuf0[NUM_ELEMENTS * NUM_ELEMENTS];
		VSREAL g_vBuf1[NUM_ELEMENTS * NUM_ELEMENTS];

		VSTexAllStatePtr m_pBuf0;
		VSTexAllStatePtr m_pBuf1;
	};
	IMPLEMENT_APPLICATION(VSTestCSDemo);

	VSTestCSDemo::VSTestCSDemo()
	{

	}

	VSTestCSDemo::~VSTestCSDemo()
	{
		
	}
	bool VSTestCSDemo::OnTerminal()
	{
		if (!VSConsoleApplication::OnTerminal())
		{
			return false;
		}
		m_pBuf0 = NULL;
		m_pBuf1 = NULL;
		
		return true;
	}
	bool VSTestCSDemo::PreInitial()
	{
		if (!VSConsoleApplication::PreInitial())
		{
			return false;
		}
		m_uiRenderAPIType = VSRenderer::RAT_DIRECTX11;
		VSResourceManager::ms_bUpdateThread = false;
		VSResourceManager::ms_bRenderThread = false;
		return true;
	}
	bool VSTestCSDemo::OnInitial()
	{
		if (!VSConsoleApplication::OnInitial())
		{
			return false;
		}
		m_bIsRunning = false;

		for (int i = 0; i < NUM_ELEMENTS; ++i)
		{
			for (int j = 0 ; j < NUM_ELEMENTS ; j++)
			{
				g_vBuf0[i * NUM_ELEMENTS + j] = VSREAL(i * NUM_ELEMENTS + j);
				g_vBuf1[i * NUM_ELEMENTS + j] = VSREAL(i * NUM_ELEMENTS + j);
			}
		}

		m_pBuf0 = VSResourceManager::Create2DTexture(NUM_ELEMENTS, NUM_ELEMENTS, VSRenderer::SFT_R32F, 1, g_vBuf0);
		m_pBuf1 = VSResourceManager::Create2DTexture(NUM_ELEMENTS, NUM_ELEMENTS, VSRenderer::SFT_R32F, 1, g_vBuf1);


		VSUnorderAccess * pUAV = NULL;
		VSCustomCSMaterial * pTestBufferCS = NULL;

		pUAV = VSResourceManager::GetTextureUnorderAccess(NUM_ELEMENTS, NUM_ELEMENTS, VSRenderer::SFT_R32F,false,true);
		pTestBufferCS = VSCustomCSMaterial::GetTestTexture();


		

		 

		VSUsedName Buffer0 = _T("Buffer0");
		VSUsedName Buffer1 = _T("Buffer1");
		pTestBufferCS->SetCShaderTexture(Buffer0, m_pBuf0);
		pTestBufferCS->SetCShaderTexture(Buffer1, m_pBuf1);
		pTestBufferCS->AddUnorderAccess(pUAV);
		pTestBufferCS->UseMaterial(2, 2, 1);

		VSREAL * p = (VSREAL *)pUAV->Lock();
		BOOL bSuccess = TRUE;
		if (p)
		{
			for (int i = 0; i < NUM_ELEMENTS; ++i)
			{
				for (int j = 0; j < NUM_ELEMENTS; j++)
				{
					if ((p[i * NUM_ELEMENTS + j] != g_vBuf0[i * NUM_ELEMENTS + j] + g_vBuf1[i * NUM_ELEMENTS + j]))
					{
						VSOutputDebugString(_T("failure\n"));
						printf(_T("failure\n"));
						bSuccess = FALSE;

						break;
					}
					VSOutputDebugString(_T("%f \n"), p[i * NUM_ELEMENTS + j]);
					printf(_T("%f \n"), p[i * NUM_ELEMENTS + j]);
				}				
			}			
			if (bSuccess)
			{
				VSOutputDebugString(_T("succeeded\n"));
				printf(_T("succeeded\n"));
			}
		}
		pUAV->UnLock();
		VSResourceManager::DisableOutputResource(pUAV);
		return true;
	}
}
#endif