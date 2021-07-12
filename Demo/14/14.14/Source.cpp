//��Demo14.13�����AnimTree��Դ�����õ�����ģ���У������档
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


		VSAnimTreeRPtr pAnimTree = VSResourceManager::LoadASYNResource<VSAnimTree>(_T("AnimTreeTwoImmediateAnim.ANIMTREE"), false);



		VSSkeletonMeshNode * pSkeletonMeshNode = pMonsterModel->GetResource();


		pSkeletonMeshNode->SetAnimTree(pAnimTree);

		VSOutputDebugString("Save VSEngine Model NewMonsterWithTwoImmediateAnim\n");
		printf("Save VSEngine Model NewMonsterWithTwoImmediateAnim\n");
		VSResourceManager::SaveResouce(pSkeletonMeshNode, _T("NewMonsterWithTwoImmediateAnim"), true);

		return true;
	}
}
#endif