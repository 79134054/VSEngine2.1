//��Monster�����ֹ�ͷ����һ��Socket��ͨ����3DMax�鿴FBX�ļ��Ǽܣ�����֪�����ֹ�ͷ���ֽ�Bip01 R Hand������Socket���ֽ�HandSocket��
#ifndef VSSAMPLE_H
#define VSSAMPLE_H
#include "VSApplication.h"
#include "VSMaterial.h"
namespace VSEngine2
{

//SourcePath DestPath -s(static mesh)/-d(dynamic mesh)/-a(action) -c(compress) -v(shadow volume)
	class MaterialSaverDemo : public VSConsoleApplication
	{
	public:
		MaterialSaverDemo();
		virtual ~MaterialSaverDemo();
		DLCARE_APPLICATION(MaterialSaverDemo);
	public:
		virtual bool PreInitial();
		virtual bool OnInitial();	
		virtual bool OnTerminal();

	
	};
	IMPLEMENT_APPLICATION(MaterialSaverDemo);

	MaterialSaverDemo::MaterialSaverDemo()
	{

	}

	MaterialSaverDemo::~MaterialSaverDemo()
	{
		
		
	}
	bool MaterialSaverDemo::OnTerminal()
	{
		if (!VSConsoleApplication::OnTerminal())
		{
			return false;
		}
		return true;
	}
	bool MaterialSaverDemo::PreInitial()
	{
		if (!VSConsoleApplication::PreInitial())
		{
			return false;
		}
		VSResourceManager::ms_bUpdateThread = false;
		VSResourceManager::ms_bRenderThread = false;
		return true;
	}
	bool MaterialSaverDemo::OnInitial()
	{
		if (!VSConsoleApplication::OnInitial())
		{
			return false;
		}	
		m_bIsRunning = false;
		




		//Model
		VSOutputDebugString("Load VSEngine Model NewMonsterWithAnim\n");
		printf("Load VSEngine Model NewMonsterWithAnim\n");
		VSSkeletonMeshNodeRPtr pMonsterModel = VSResourceManager::LoadASYNResource<VSSkeletonMeshNode>(_T("NewMonsterWithAnim.SKMODEL"), false);




		
		
		VSSkeletonMeshNode * pSkeletonMeshNode = pMonsterModel->GetResource();

		
		pSkeletonMeshNode->CreateSocket(_T("Bip01 R Hand"), _T("HandSocket"));

		VSOutputDebugString("Save VSEngine Model NewMonsterWithSocket\n");
		printf("Save VSEngine Model NewMonsterWithSocket\n");
		VSResourceManager::SaveResouce(pSkeletonMeshNode, _T("NewMonsterWithSocket"), true);

		
		return true;
	}
}
#endif