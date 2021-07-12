//加载引擎格式的动作到动画集合中，一共导入6个动画，一起保存在Mesh里面。
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
		VSOutputDebugString("Load VSEngine Model NewMonster\n");
		printf("Load VSEngine Model NewMonster\n");
		VSSkeletonMeshNodeRPtr pMonsterModel = VSResourceManager::LoadASYNResource<VSSkeletonMeshNode>(_T("NewMonster.SKMODEL"), false);




		
		
		VSSkeletonMeshNode * pSkeletonMeshNode = pMonsterModel->GetResource();
		VSAnimSet* pAnimSet = pSkeletonMeshNode->GetAnimSet();
		if (pAnimSet == NULL)
		{
			pAnimSet = VS_NEW VSAnimSet();
		}
		VSOutputDebugString("Load VSEngine Action Idle\n");
		printf("Load VSEngine Action Idle\n");
		VSAnimRPtr  pAnim0 = VSResourceManager::LoadASYNResource<VSAnim>(_T("Idle"), false);

		VSOutputDebugString("Load VSEngine Action Walk\n");
		printf("Load VSEngine Action Walk\n");
 		VSAnimRPtr  pAnim1 = VSResourceManager::LoadASYNResource<VSAnim>("Walk", false);

		VSOutputDebugString("Load VSEngine Action Attack\n");
		printf("Load VSEngine Action Attack\n");
 		VSAnimRPtr  pAnim2 = VSResourceManager::LoadASYNResource<VSAnim>("Attack", false);


		VSOutputDebugString("Load VSEngine Action Attack2\n");
		printf("Load VSEngine Action Attack2\n");
		VSAnimRPtr  pAnim3 = VSResourceManager::LoadASYNResource<VSAnim>("Attack2", false);
		
		VSOutputDebugString("Load VSEngine Action AddtiveAnim\n");
		printf("Load VSEngine Action AddtiveAnim\n");
		VSAnimRPtr  pAnim4 = VSResourceManager::LoadASYNResource<VSAnim>("AddtiveAnim", false);

		VSOutputDebugString("Load VSEngine Action Pos\n");
		printf("Load VSEngine Action Pos\n");
		VSAnimRPtr  pAnim5 = VSResourceManager::LoadASYNResource<VSAnim>("Pos", false);

		pAnimSet->AddAnim(_T("Idle"), pAnim0);
		pAnimSet->AddAnim(_T("Walk"), pAnim1);
		pAnimSet->AddAnim(_T("Attack"), pAnim2);
		pAnimSet->AddAnim(_T("Attack2"), pAnim3);
		pAnimSet->AddAnim(_T("AddtiveAnim"), pAnim4);
		pAnimSet->AddAnim(_T("Pos"), pAnim5);

		pSkeletonMeshNode->SetAnimSet(pAnimSet);
		

		VSOutputDebugString("Save VSEngine Model NewMonsterWithAnim\n");
		printf("Save VSEngine Model NewMonsterWithAnim\n");
		VSResourceManager::SaveResouce(pSkeletonMeshNode, _T("NewMonsterWithAnim"), true);

		
		return true;
	}
}
#endif