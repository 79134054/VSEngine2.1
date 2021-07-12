//给Skin Instance Mesh 加上材质
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
		




		
		VSOutputDebugString("Load VSEngine texture Monster.Monster_d\n");
		printf("Load VSEngine texture Monster.Monster_d\n");
		VSTexAllStateRPtr  pDiffuseR =  VSResourceManager::LoadASYNResource<VSTexAllState>(_T("Monster/Monster_d.TEXTURE"), false);

		VSOutputDebugString("Load VSEngine texture Monster.Monster_n\n");
		printf("Load VSEngine texture Monster.Monster_n\n");
		VSTexAllStateRPtr  pNormalR = VSResourceManager::LoadASYNResource<VSTexAllState>(_T("Monster/Monster_n.TEXTURE"), false);

		VSOutputDebugString("Load VSEngine texture Monster.Monster_s\n");
		printf("Load VSEngine texture Monster.Monster_s\n");
		VSTexAllStateRPtr  pSpecularR = VSResourceManager::LoadASYNResource<VSTexAllState>(_T("Monster/Monster_s.TEXTURE"), false);

		VSOutputDebugString("Load VSEngine texture Monster.Monster_e\n");
		printf("Load VSEngine texture Monster.Monster_e\n");
		VSTexAllStateRPtr  pEmissiveR = VSResourceManager::LoadASYNResource<VSTexAllState>(_T("Monster/Monster_e.TEXTURE"), false);

		VSOutputDebugString("Load VSEngine texture Monster.Monster_d\n");
		printf("Load VSEngine texture Monster.Monster_d\n");
		VSTexAllStateRPtr  pDiffuse_wR = VSResourceManager::LoadASYNResource<VSTexAllState>(_T("Monster/Monster_w_d.TEXTURE"), false);

		VSOutputDebugString("Load VSEngine texture Monster.Monster_n\n");
		printf("Load VSEngine texture Monster.Monster_n\n");
		VSTexAllStateRPtr  pNormal_wR = VSResourceManager::LoadASYNResource<VSTexAllState>(_T("Monster/Monster_w_n.TEXTURE"), false);

		VSOutputDebugString("Load VSEngine texture Monster.Monster_s\n");
		printf("Load VSEngine texture Monster.Monster_s\n");
		VSTexAllStateRPtr  pSpecular_wR = VSResourceManager::LoadASYNResource<VSTexAllState>(_T("Monster/Monster_w_s.TEXTURE"), false);

		VSOutputDebugString("Load VSEngine texture Monster.Monster_e\n");
		printf("Load VSEngine texture Monster.Monster_e\n");
		VSTexAllStateRPtr  pEmissive_wR = VSResourceManager::LoadASYNResource<VSTexAllState>(_T("Monster/Monster_w_e.TEXTURE"), false);


		//material


		VSOutputDebugString("Load VSEngine Material Phone\n");
		printf("Load VSEngine Material Phone\n");
		VSMaterialRPtr  pPhoneMaterialR = VSResourceManager::LoadASYNResource<VSMaterial>(_T("Phone.MATERIAL"), false);



		//Model
		VSOutputDebugString("Load VSEngine Model MonsterInstance\n");
		printf("Load VSEngine Model MonsterInstance\n");
		VSSkeletonMeshNodeRPtr pMonsterModel = VSResourceManager::LoadASYNResource<VSSkeletonMeshNode>(_T("MonsterInstance.SKMODEL"), false);





		VSOutputDebugString("Set Material\n");
		printf("Set Material\n");
		VSSkeletonMeshNode * pSkeletonMeshNode = pMonsterModel->GetResource();
		VSGeometryNode * pGeometryNode = pSkeletonMeshNode->GetGeometryNode(0);
		for (unsigned int i = 0; i < pGeometryNode->GetNormalGeometryNum(); i++)
		{
			VSGeometry * pGeometry = pGeometryNode->GetNormalGeometry(i);
			pGeometry->SetMaterialInstance(pPhoneMaterialR,0);
			if (i == 0)
			{
				pGeometry->GetMaterialInstance(0)->SetShaderTexture(_T("DiffuseTexture"), pDiffuseR);
				pGeometry->GetMaterialInstance(0)->SetShaderTexture(_T("NormalTexture"), pNormalR);
				pGeometry->GetMaterialInstance(0)->SetShaderTexture(_T("SpecularTexture"), pSpecularR);
				pGeometry->GetMaterialInstance(0)->SetShaderTexture(_T("EmissiveTexture"), pEmissiveR);
			}
			else if (i == 1)
			{
				pGeometry->GetMaterialInstance(0)->SetShaderTexture(_T("DiffuseTexture"), pDiffuse_wR);
				pGeometry->GetMaterialInstance(0)->SetShaderTexture(_T("NormalTexture"), pNormal_wR);
				pGeometry->GetMaterialInstance(0)->SetShaderTexture(_T("SpecularTexture"), pSpecular_wR);
				pGeometry->GetMaterialInstance(0)->SetShaderTexture(_T("EmissiveTexture"), pEmissive_wR);
			}
			
		}
		

	
		VSOutputDebugString("Save VSEngine Model MonsterInstance\n");
		printf("Save VSEngine Model MonsterInstance\n");
		VSResourceManager::SaveResouce(pSkeletonMeshNode, _T("NewMonsterInstance"), true);

		
		return true;
	}
}
#endif