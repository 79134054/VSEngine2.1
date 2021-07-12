//测试包含了Buffer和StructBuffer的CS Shader 
#ifndef VSSAMPLE_H
#define VSSAMPLE_H
#include "VSApplication.h"
#include "VSMaterial.h"
namespace VSEngine2
{

	struct BufType
	{
		int i;
		VSREAL f;

	};
	const UINT NUM_ELEMENTS = 1024;
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
		
		
		BufType g_vBuf0[NUM_ELEMENTS];
		BufType g_vBuf1[NUM_ELEMENTS];

		VSBufferResourcePtr m_pBuf0;
		VSBufferResourcePtr m_pBuf1;

		enum 
		{
			STRUCT_BUFFER,
			BYTE_BUFFER,
		};
		unsigned int BufferType;
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
		BufferType = STRUCT_BUFFER;
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
			g_vBuf0[i].i = i;
			g_vBuf0[i].f = (VSREAL)i;


			g_vBuf1[i].i = i;
			g_vBuf1[i].f = (VSREAL)i;
		}

		VSDataBufferPtr pDataBuffer0 = VS_NEW VSDataBuffer();
		m_pBuf0 = VS_NEW VSBufferResource();
		VSDataBufferPtr pDataBuffer1 = VS_NEW VSDataBuffer();
		m_pBuf1 = VS_NEW VSBufferResource();

		if (BufferType == STRUCT_BUFFER)
		{
			pDataBuffer0->SetData(g_vBuf0, NUM_ELEMENTS, VSDataBuffer::DT_STRUCT, sizeof(BufType));
			pDataBuffer1->SetData(g_vBuf1, NUM_ELEMENTS, VSDataBuffer::DT_STRUCT, sizeof(BufType));
		}	
		else
		{
			pDataBuffer0->SetData(g_vBuf0, NUM_ELEMENTS * sizeof(BufType), VSDataBuffer::DT_UBYTE);
			pDataBuffer1->SetData(g_vBuf1, NUM_ELEMENTS * sizeof(BufType), VSDataBuffer::DT_UBYTE);
		}
		m_pBuf0->SetData(pDataBuffer0);		
		m_pBuf1->SetData(pDataBuffer1);

		VSUnorderAccess * pUAV = NULL;
		VSCustomCSMaterial * pTestBufferCS = NULL;
		if (BufferType == STRUCT_BUFFER)
		{
			pUAV = VSResourceManager::GetBufferUnorderAccess(NUM_ELEMENTS, VSDataBuffer::DT_STRUCT, sizeof(BufType), false, true);
			pTestBufferCS = VSCustomCSMaterial::GetTestStruct();
		}
		else
		{
			pUAV = VSResourceManager::GetBufferUnorderAccess(NUM_ELEMENTS * sizeof(BufType), VSDataBuffer::DT_UBYTE, 0, false, true);
			pTestBufferCS = VSCustomCSMaterial::GetTestByteBuffer();
		}

		

		 

		VSUsedName Buffer0 = _T("Buffer0");
		VSUsedName Buffer1 = _T("Buffer1");
		pTestBufferCS->SetCShaderBuffer(Buffer0, m_pBuf0);
		pTestBufferCS->SetCShaderBuffer(Buffer1, m_pBuf1);
		pTestBufferCS->AddUnorderAccess(pUAV);
		pTestBufferCS->UseMaterial(NUM_ELEMENTS, 1, 1);

		BufType * p = (BufType *)pUAV->Lock();
		BOOL bSuccess = TRUE;
		if (p)
		{
			for (int i = 0; i < NUM_ELEMENTS; ++i)
			{
				if ((p[i].i != g_vBuf0[i].i + g_vBuf1[i].i))
				{
					VSOutputDebugString(_T("failure\n"));
					printf(_T("failure\n"));
					bSuccess = FALSE;

					break;
				}
				VSOutputDebugString(_T("%d  %f \n"), p[i].i, p[i].f);
				printf(_T("%d  %f \n"), p[i].i, p[i].f);
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