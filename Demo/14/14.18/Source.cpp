//��Demo14.17�����AnimTree��Դ�����õ�����ģ���У������档
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
		VSOutputDebugString("Load VSEngine Model NewMonsterWithSocket\n");
		printf("Load VSEngine Model NewMonsterWithSocket\n");
		VSSkeletonMeshNodeRPtr pMonsterModel = VSResourceManager::LoadASYNResource<VSSkeletonMeshNode>(_T("NewMonsterWithSocket.SKMODEL"), false);


		VSAnimTreeRPtr pAnimTree1 = VSResourceManager::LoadASYNResource<VSAnimTree>(_T("AnimTreePartialAnim1.ANIMTREE"), false);



		VSSkeletonMeshNode * pSkeletonMeshNode = pMonsterModel->GetResource();


		pSkeletonMeshNode->SetAnimTree(pAnimTree1);

		VSOutputDebugString("Save VSEngine Model NewMonsterWithPartialAnim1\n");
		printf("Save VSEngine Model NewMonsterWithPartialAnim1\n");
		VSResourceManager::SaveResouce(pSkeletonMeshNode, _T("NewMonsterWithPartialAnim1"), true);


		VSAnimTreeRPtr pAnimTree2 = VSResourceManager::LoadASYNResource<VSAnimTree>(_T("AnimTreePartialAnim2.ANIMTREE"), false);
		pSkeletonMeshNode->SetAnimTree(pAnimTree2);

		VSOutputDebugString("Save VSEngine Model NewMonsterWithPartialAnim2\n");
		printf("Save VSEngine Model NewMonsterWithPartialAnim2\n");
		VSResourceManager::SaveResouce(pSkeletonMeshNode, _T("NewMonsterWithPartialAnim2"), true);
		return true;
	}
}
#endif