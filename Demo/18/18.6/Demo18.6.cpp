//给Skin Instance Mesh 创建动画树
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
		VSOutputDebugString("Load VSEngine Model NewMonsterInstance\n");
		printf("Load VSEngine Model NewMonsterInstance\n");
		VSSkeletonMeshNodeRPtr pMonsterModel = VSResourceManager::LoadASYNResource<VSSkeletonMeshNode>(_T("NewMonsterInstance.SKMODEL"), false);




		
		
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
		


		pAnimSet->AddAnim(_T("Idle"), pAnim0);
		pAnimSet->AddAnim(_T("Walk"), pAnim1);
		pAnimSet->AddAnim(_T("Attack"), pAnim2);
		pAnimSet->AddAnim(_T("Attack2"), pAnim3);


		pSkeletonMeshNode->SetAnimSet(pAnimSet);
		pSkeletonMeshNode->SetDrawInstance(true);
		VSAnimTreeRPtr pAnimTree = VSResourceManager::LoadASYNResource<VSAnimTree>(_T("AnimTreeTwoSmoothBlendAnim.ANIMTREE"), false);


		pSkeletonMeshNode->SetAnimTree(pAnimTree);

		VSOutputDebugString("Save VSEngine Model NewMonsterInstanceWithAnim\n");
		printf("Save VSEngine Model NewMonsterInstanceWithAnim\n");
		VSResourceManager::SaveResouce(pSkeletonMeshNode, _T("NewMonsterInstanceWithAnim"), true);

		
		return true;
	}
}
#endif