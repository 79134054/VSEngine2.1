//这个demo给导出的Stone.STMODEL创建一个材质资源，并赋予它，然后导出新的模型叫做NewInstanceTestStone.STMODEL。
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
	protected:
		VSMaterialPtr m_SaveMaterial;
	
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
		m_SaveMaterial = NULL;
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
		m_SaveMaterial = NULL;
		return true;
	}
	bool MaterialSaverDemo::OnInitial()
	{
		if (!VSConsoleApplication::OnInitial())
		{
			return false;
		}	
		m_bIsRunning = false;

		VSOutputDebugString("Load VSEngine texture Stone.Diffuse\n");
		printf("Load VSEngine texture Stone.Diffuse\n");
		VSTexAllStateRPtr  pDiffuseR = VSResourceManager::LoadASYNResource<VSTexAllState>(_T("Stone/Diffuse.TEXTURE"), false);

		VSOutputDebugString("Load VSEngine texture Stone.Normal\n");
		printf("Load VSEngine texture Stone.Normal\n");
		VSTexAllStateRPtr  pNormalR = VSResourceManager::LoadASYNResource<VSTexAllState>(_T("Stone/Normal.TEXTURE"), false);

		VSOutputDebugString("Save VSEngine Material InstanceTest\n");
		printf("Save VSEngine Material InstanceTest\n");
		m_SaveMaterial = VS_NEW VSMaterialInstanceTest(_T("InstanceTest"), pDiffuseR, pNormalR);
		VSResourceManager::SaveResouce(m_SaveMaterial, _T("InstanceTest"), true);

		VSOutputDebugString("Load VSEngine Material InstanceTest\n");
		printf("Load VSEngine Material InstanceTest\n");
		VSMaterialRPtr  pMaterialR = VSResourceManager::LoadASYNResource<VSMaterial>(_T("InstanceTest.MATERIAL"), false);

		VSOutputDebugString("Load VSEngine Model Stone\n");
		printf("Load VSEngine Model Stone\n");
		VSStaticMeshNodeRPtr pModel = VSResourceManager::LoadASYNResource<VSStaticMeshNode>(_T("Stone.STMODEL"), false);

		VSOutputDebugString("Set Material\n");
		printf("Set Material\n");
		VSStaticMeshNode * pStaticMeshNode = pModel->GetResource();
		pStaticMeshNode->SetDrawInstance(true);
		VSGeometryNode * pGeometryNode = pStaticMeshNode->GetGeometryNode(0);
		for (unsigned int i = 0; i < pGeometryNode->GetNormalGeometryNum(); i++)
		{
			VSGeometry * pGeometry = pGeometryNode->GetNormalGeometry(i);
			pGeometry->SetMaterialInstance(pMaterialR,0);
		}
		VSOutputDebugString("Save VSEngine Model NewInstanceTestStone\n");
		printf("Save VSEngine Model NewInstanceTestStone\n");
		VSResourceManager::SaveResouce(pStaticMeshNode, _T("NewInstanceTestStone"), true);
		return true;
	}
}
#endif