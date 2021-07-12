//加载引擎格式的根骨动画到动画集合中，并一起保存在Mesh里面。
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
		VSOutputDebugString("Load VSEngine Model Mannequin\n");
		printf("Load VSEngine Model Mannequin\n");
		VSSkeletonMeshNodeRPtr pMonsterModel = VSResourceManager::LoadASYNResource<VSSkeletonMeshNode>(_T("Mannequin.SKMODEL"), false);




		
		
		VSSkeletonMeshNode * pSkeletonMeshNode = pMonsterModel->GetResource();
		VSAnimSet* pAnimSet = pSkeletonMeshNode->GetAnimSet();
		if (pAnimSet == NULL)
		{
			pAnimSet = VS_NEW VSAnimSet();
		}
		VSOutputDebugString("Load VSEngine Action RootMotion\n");
		printf("Load VSEngine Action RootMotion\n");
		VSAnimRPtr  pAnim0 = VSResourceManager::LoadASYNResource<VSAnim>(_T("RootMotion"), false);



		pAnimSet->AddAnim(_T("RootMotion"), pAnim0);

		pSkeletonMeshNode->SetAnimSet(pAnimSet);
		

		VSOutputDebugString("Save VSEngine Model MannequinWithAnim\n");
		printf("Save VSEngine Model MannequinWithAnim\n");
		VSResourceManager::SaveResouce(pSkeletonMeshNode, _T("MannequinWithAnim"), true);

		
		return true;
	}
}
#endif