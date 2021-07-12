//这个Demo演示了加载5个层级LOD Mesh并赋予材质，存储成新的Mesh。这个Demo可以看见Material Instance的使用，具体的会在后面章节介绍。
#ifndef VSSAMPLE_H
#define VSSAMPLE_H
#include "VSApplication.h"
#include "VSMaterial.h"
#include "VSConfig.h"
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
		VSSamplerDesc SamplerDesc;
		SamplerDesc.m_uiMag = VSSamplerDesc::FM_LINE;
		SamplerDesc.m_uiMin = VSSamplerDesc::FM_LINE;
		SamplerDesc.m_uiMip = VSSamplerDesc::FM_LINE;
		VSSamplerStatePtr pTriLineSamplerState = VSResourceManager::CreateSamplerState(SamplerDesc);
		// texture 
		VSOutputDebugString("Load diffuse tga texture\n");
		printf("Load diffuse tga texture\n");
		VSFileName DiffuseFileName = VSTexAllState::ms_ResourcePath;
		DiffuseFileName += _T("Monster_d.tga");
		VSTexAllStatePtr pDiffuse = VSResourceManager::Load2DTexture(DiffuseFileName.GetBuffer(), pTriLineSamplerState,
			VSResourceManager::CT_BC1, false, true, true);

		VSOutputDebugString("Save VSEngine texture Monster.Monster_d\n");
		printf("Save VSEngine texture Monster.Monster_d\n");
		VSResourceManager::SaveResouce(pDiffuse, _T("Monster/Monster_d"),true);

		VSOutputDebugString("Load normal tga texture\n");
		printf("Load normal tga texture\n");
		VSFileName NormalFileName = VSTexAllState::ms_ResourcePath;
		NormalFileName += _T("Monster_n.tga");
		VSTexAllStatePtr pNormal = VSResourceManager::Load2DTexture(NormalFileName.GetBuffer(), pTriLineSamplerState, 
			VSResourceManager::CT_BC1, true, false, true);

		VSOutputDebugString("Save VSEngine texture Monster.Monster_n\n");
		printf("Save VSEngine texture Monster.Monster_n\n");
		VSResourceManager::SaveResouce(pNormal, _T("Monster/Monster_n"), true);


		VSOutputDebugString("Load specular tga texture\n");
		printf("Load specular tga texture\n");
		VSFileName SpecularFileName = VSTexAllState::ms_ResourcePath;
		SpecularFileName += _T("Monster_s.tga");
		VSTexAllStatePtr pSpecular = VSResourceManager::Load2DTexture(SpecularFileName.GetBuffer(), pTriLineSamplerState,
			VSResourceManager::CT_BC1, false, false, true);

		VSOutputDebugString("Save VSEngine texture Monster.Monster_s\n");
		printf("Save VSEngine texture Monster.Monster_s\n");
		VSResourceManager::SaveResouce(pSpecular, _T("Monster/Monster_s"), true);


		VSOutputDebugString("Load emissive tga texture\n");
		printf("Load emissive tga texture\n");
		VSFileName EmissiveFileName = VSTexAllState::ms_ResourcePath;
		EmissiveFileName += _T("Monster_e.tga");
		VSTexAllStatePtr pEmissive = VSResourceManager::Load2DTexture(EmissiveFileName.GetBuffer(), pTriLineSamplerState,
			VSResourceManager::CT_BC1, false, false, true);

		VSOutputDebugString("Save VSEngine texture Monster.Monster_e\n");
		printf("Save VSEngine texture Monster.Monster_e\n");
		VSResourceManager::SaveResouce(pEmissive, _T("Monster/Monster_e"), true);



		VSOutputDebugString("Load diffuse tga texture\n");
		printf("Load diffuse tga texture\n");
		VSFileName DiffuseWFileName = VSTexAllState::ms_ResourcePath;
		DiffuseWFileName += _T("Monster_w_d.tga");
		VSTexAllStatePtr pDiffuseW = VSResourceManager::Load2DTexture(DiffuseWFileName.GetBuffer(), pTriLineSamplerState,
			VSResourceManager::CT_BC1, false, true, true);

		VSOutputDebugString("Save VSEngine texture Monster.Monster_w_d\n");
		printf("Save VSEngine texture Monster.Monster_w_d\n");
		VSResourceManager::SaveResouce(pDiffuseW, _T("Monster/Monster_w_d"), true);

		VSOutputDebugString("Load normal tga texture\n");
		printf("Load normal tga texture\n");
		VSFileName NormalWFileName = VSTexAllState::ms_ResourcePath;
		NormalWFileName += _T("Monster_w_n.tga");
		VSTexAllStatePtr pNormalW = VSResourceManager::Load2DTexture(NormalWFileName.GetBuffer(), pTriLineSamplerState,
			VSResourceManager::CT_BC1, true, false, true);

		VSOutputDebugString("Save VSEngine texture Monster.Monster_w_n\n");
		printf("Save VSEngine texture Monster.Monster_w_n\n");
		VSResourceManager::SaveResouce(pNormalW, _T("Monster/Monster_w_n"), true);


		VSOutputDebugString("Load specular tga texture\n");
		printf("Load specular tga texture\n");
		VSFileName SpecularWFileName = VSTexAllState::ms_ResourcePath;
		SpecularWFileName +=_T("Monster_w_s.tga");
		VSTexAllStatePtr pSpecularW = VSResourceManager::Load2DTexture(SpecularWFileName.GetBuffer(), pTriLineSamplerState, 
			VSResourceManager::CT_BC1, false, false, true);

		VSOutputDebugString("Save VSEngine texture Monster.Monster_w_s\n");
		printf("Save VSEngine texture Monster.Monster_w_s\n");
		VSResourceManager::SaveResouce(pSpecularW, _T("Monster/Monster_w_s"), true);


		VSOutputDebugString("Load emissive tga texture\n");
		printf("Load emissive tga texture\n");
		VSFileName EmissiveWFileName = VSTexAllState::ms_ResourcePath;
		EmissiveWFileName += _T("Monster_w_e.tga");
		VSTexAllStatePtr pEmissiveW = VSResourceManager::Load2DTexture(EmissiveWFileName.GetBuffer(), pTriLineSamplerState, 
			VSResourceManager::CT_BC1, false, false, true);

		VSOutputDebugString("Save VSEngine texture Monster.Monster_w_e\n");
		printf("Save VSEngine texture Monster.Monster_w_e\n");
		VSResourceManager::SaveResouce(pEmissiveW, _T("Monster/Monster_w_e"), true);




		
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
		VSOutputDebugString("Save VSEngine Material OnlyTexture\n");
		printf("Save VSEngine Material OnlyTexture\n");
		VSMaterialPtr pOnlyTextureMaterial = VS_NEW VSMaterialOnlyTexture(_T("OnlyTexture"), pDiffuseR);
		VSResourceManager::SaveResouce(pOnlyTextureMaterial, _T("OnlyTexture"), true);

		VSOutputDebugString("Save VSEngine Material Phone\n");
		printf("Save VSEngine Material Phone\n");
		VSMaterialPtr pPhoneMaterial = VS_NEW VSMaterialPhone(_T("Phone"), pDiffuseR,pNormalR,pSpecularR,pEmissiveR,true);
		VSResourceManager::SaveResouce(pPhoneMaterial, _T("Phone"), true);

		VSOutputDebugString("Load VSEngine Material OnlyTexture\n");
		printf("Load VSEngine Material OnlyTexture\n");
		VSMaterialRPtr  pOnlyTextureMaterialR = VSResourceManager::LoadASYNResource<VSMaterial>(_T("OnlyTexture.MATERIAL"), false);

		VSOutputDebugString("Load VSEngine Material TextureAndNormal\n");
		printf("Load VSEngine Material TextureAndNormal\n");
		VSMaterialRPtr  pTextureAndNormalMaterialR = VSResourceManager::LoadASYNResource<VSMaterial>(_T("TextureAndNormal.MATERIAL"), false);

		VSOutputDebugString("Load VSEngine Material Phone\n");
		printf("Load VSEngine Material Phone\n");
		VSMaterialRPtr  pPhoneMaterialR = VSResourceManager::LoadASYNResource<VSMaterial>(_T("Phone.MATERIAL"), false);



		//Model
		VSOutputDebugString("Load VSEngine Model MonsterLOD0\n");
		printf("Load VSEngine Model MonsterLOD0\n");
		VSStaticMeshNodeRPtr pMonsterLOD0Model = VSResourceManager::LoadASYNResource<VSStaticMeshNode>(_T("MonsterLOD0.STMODEL"), false);

		VSOutputDebugString("Load VSEngine Model MonsterLOD1\n");
		printf("Load VSEngine Model MonsterLOD1\n");
		VSStaticMeshNodeRPtr pMonsterLOD1Model = VSResourceManager::LoadASYNResource<VSStaticMeshNode>(_T("MonsterLOD1.STMODEL"), false);

		VSOutputDebugString("Load VSEngine Model MonsterLOD0\n");
		printf("Load VSEngine Model MonsterLOD0\n");
		VSStaticMeshNodeRPtr pMonsterLOD2Model = VSResourceManager::LoadASYNResource<VSStaticMeshNode>(_T("MonsterLOD2.STMODEL"), false);

		VSOutputDebugString("Load VSEngine Model MonsterLOD0\n");
		printf("Load VSEngine Model MonsterLOD0\n");
		VSStaticMeshNodeRPtr pMonsterLOD3Model = VSResourceManager::LoadASYNResource<VSStaticMeshNode>(_T("MonsterLOD3.STMODEL"), false);

		VSOutputDebugString("Load VSEngine Model MonsterLOD0\n");
		printf("Load VSEngine Model MonsterLOD0\n");
		VSStaticMeshNodeRPtr pMonsterLOD4Model = VSResourceManager::LoadASYNResource<VSStaticMeshNode>(_T("MonsterLOD4.STMODEL"), false);




		VSOutputDebugString("Set Material\n");
		printf("Set Material\n");
		VSStaticMeshNode * pStaticMeshNodeLOD0 = pMonsterLOD0Model->GetResource();
		VSGeometryNode * pGeometryNodeLOD0 = pStaticMeshNodeLOD0->GetGeometryNode(0);
		for (unsigned int i = 0; i < pGeometryNodeLOD0->GetNormalGeometryNum(); i++)
		{
			VSGeometry * pGeometry = pGeometryNodeLOD0->GetNormalGeometry(i);
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
		

		VSStaticMeshNode * pStaticMeshNodeLOD1 = pMonsterLOD1Model->GetResource();
		VSGeometryNode * pGeometryNodeLOD1 = pStaticMeshNodeLOD1->GetGeometryNode(0);
		for (unsigned int i = 0; i < pGeometryNodeLOD1->GetNormalGeometryNum(); i++)
		{
			VSGeometry * pGeometry = pGeometryNodeLOD1->GetNormalGeometry(i);
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

		VSStaticMeshNode * pStaticMeshNodeLOD2 = pMonsterLOD2Model->GetResource();
		VSGeometryNode * pGeometryNodeLOD2 = pStaticMeshNodeLOD2->GetGeometryNode(0);
		for (unsigned int i = 0; i < pGeometryNodeLOD2->GetNormalGeometryNum(); i++)
		{
			VSGeometry * pGeometry = pGeometryNodeLOD2->GetNormalGeometry(i);
			pGeometry->SetMaterialInstance(pTextureAndNormalMaterialR,0);
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

		VSStaticMeshNode * pStaticMeshNodeLOD3 = pMonsterLOD3Model->GetResource();
		VSGeometryNode * pGeometryNodeLOD3 = pStaticMeshNodeLOD3->GetGeometryNode(0);
		for (unsigned int i = 0; i < pGeometryNodeLOD3->GetNormalGeometryNum(); i++)
		{
			VSGeometry * pGeometry = pGeometryNodeLOD3->GetNormalGeometry(i);
			pGeometry->SetMaterialInstance(pTextureAndNormalMaterialR,0);
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

		VSStaticMeshNode * pStaticMeshNodeLOD4 = pMonsterLOD4Model->GetResource();
		VSGeometryNode * pGeometryNodeLOD4 = pStaticMeshNodeLOD4->GetGeometryNode(0);
		for (unsigned int i = 0; i < pGeometryNodeLOD4->GetNormalGeometryNum(); i++)
		{
			VSGeometry * pGeometry = pGeometryNodeLOD4->GetNormalGeometry(i);
			pGeometry->SetMaterialInstance(pOnlyTextureMaterialR,0);
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
		VSOutputDebugString("Save VSEngine Model MonsterLOD0\n");
		printf("Save VSEngine Model MonsterLOD0\n");
		VSResourceManager::SaveResouce(pStaticMeshNodeLOD0, _T("NewMonsterLOD0"), true);

		VSOutputDebugString("Save VSEngine Model MonsterLOD1\n");
		printf("Save VSEngine Model MonsterLOD1\n");
		VSResourceManager::SaveResouce(pStaticMeshNodeLOD1, _T("NewMonsterLOD1"), true);

		VSOutputDebugString("Save VSEngine Model MonsterLOD2\n");
		printf("Save VSEngine Model MonsterLOD2\n");
		VSResourceManager::SaveResouce(pStaticMeshNodeLOD2, _T("NewMonsterLOD2"), true);

		VSOutputDebugString("Save VSEngine Model MonsterLOD3\n");
		printf("Save VSEngine Model MonsterLOD3\n");
		VSResourceManager::SaveResouce(pStaticMeshNodeLOD3, _T("NewMonsterLOD3"), true);

		VSOutputDebugString("Save VSEngine Model MonsterLOD4\n");
		printf("Save VSEngine Model MonsterLOD4\n");
		VSResourceManager::SaveResouce(pStaticMeshNodeLOD4, _T("NewMonsterLOD4"), true);
		return true;
	}
}
#endif