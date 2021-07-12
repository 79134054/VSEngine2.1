//չʾ�˰�5�� Mesh�ϲ���һ������LODЧ����Mesh�����洢����Mesh��
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
		VSOutputDebugString("Load VSEngine Model MonsterLOD0\n");
		printf("Load VSEngine Model MonsterLOD0\n");
		VSStaticMeshNodeRPtr pMonsterLOD0Model = VSResourceManager::LoadASYNResource<VSStaticMeshNode>(_T("NewMonsterLOD0.STMODEL"), false);

		VSOutputDebugString("Load VSEngine Model MonsterLOD1\n");
		printf("Load VSEngine Model MonsterLOD1\n");
		VSStaticMeshNodeRPtr pMonsterLOD1Model = VSResourceManager::LoadASYNResource<VSStaticMeshNode>(_T("NewMonsterLOD1.STMODEL"), false);

		VSOutputDebugString("Load VSEngine Model MonsterLOD2\n");
		printf("Load VSEngine Model MonsterLOD2\n");
		VSStaticMeshNodeRPtr pMonsterLOD2Model = VSResourceManager::LoadASYNResource<VSStaticMeshNode>(_T("NewMonsterLOD2.STMODEL"), false);

		VSOutputDebugString("Load VSEngine Model MonsterLOD3\n");
		printf("Load VSEngine Model MonsterLOD3\n");
		VSStaticMeshNodeRPtr pMonsterLOD3Model = VSResourceManager::LoadASYNResource<VSStaticMeshNode>(_T("NewMonsterLOD3.STMODEL"), false);

		VSOutputDebugString("Load VSEngine Model MonsterLOD4\n");
		printf("Load VSEngine Model MonsterLOD4\n");
		VSStaticMeshNodeRPtr pMonsterLOD4Model = VSResourceManager::LoadASYNResource<VSStaticMeshNode>(_T("NewMonsterLOD4.STMODEL"), false);
	
		VSStaticMeshNode * pStaticMeshNodeLOD0 = pMonsterLOD0Model->GetResource();
		pStaticMeshNodeLOD0->AddLodMesh(pMonsterLOD1Model);
		pStaticMeshNodeLOD0->AddLodMesh(pMonsterLOD2Model);
		pStaticMeshNodeLOD0->AddLodMesh(pMonsterLOD3Model);
		pStaticMeshNodeLOD0->AddLodMesh(pMonsterLOD4Model);
		pStaticMeshNodeLOD0->GetGeometryNode(1)->m_fLODScreenSize = 0.5f;
		pStaticMeshNodeLOD0->GetGeometryNode(2)->m_fLODScreenSize = 0.1f;
		pStaticMeshNodeLOD0->GetGeometryNode(3)->m_fLODScreenSize = 0.05f;
		pStaticMeshNodeLOD0->GetGeometryNode(4)->m_fLODScreenSize = 0.01f;
 		VSOutputDebugString("Save VSEngine Model DLODMonster\n");
 		printf("Save VSEngine Model DLODMonster\n");
		VSResourceManager::SaveResouce(pStaticMeshNodeLOD0, _T("DLODMonster"), true);
		
		return true;
	}
}
#endif