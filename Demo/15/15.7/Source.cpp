//创建和存储两个ShaderFunction混合材质，3种光照模型材质，1种自定义光照材质，LightFunction材质
#ifndef VSSAMPLE_H
#define VSSAMPLE_H
#include "VSApplication.h"
#include "VSPostEffectSet.h"
#include "VSCaptureTexAllState.h"
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
		VSTexAllStateRPtr  pDiffuseR = VSResourceManager::LoadASYNResource<VSTexAllState>(_T("Monster/Monster_d.TEXTURE"), false);

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
		
		VSOutputDebugString("Load VSEngine Model Monster\n");
		printf("Load VSEngine Model Monster\n");
		VSSkeletonMeshNodeRPtr pMonsterModel = VSResourceManager::LoadASYNResource<VSSkeletonMeshNode>(_T("Monster.SKMODEL"), false);

		// PhoneTwoPass
		VSOutputDebugString("Save VSEngine Material MaterialPhoneTwoPass\n");
		printf("Save VSEngine Material MaterialPhoneTwoPass\n");
		VSMaterialPhoneTwoPassPtr  pMaterial = VS_NEW VSMaterialPhoneTwoPass(_T("MaterialPhoneTwoPass"),pDiffuseR, pNormalR, pSpecularR, pEmissiveR,true);
		VSResourceManager::SaveResouce(pMaterial, _T("MaterialPhoneTwoPass"), true);

		VSOutputDebugString("Load VSEngine Material MaterialPhoneTwoPass\n");
		printf("Load VSEngine Material MaterialPhoneTwoPass\n");
		VSMaterialRPtr  pMaterialPhoneTwoPassR = VSResourceManager::LoadASYNResource<VSMaterial>(_T("MaterialPhoneTwoPass.MATERIAL"), false);


		VSSkeletonMeshNode * pSkeletonMeshNode = pMonsterModel->GetResource();
		VSGeometryNode * pGeometryNode = pSkeletonMeshNode->GetGeometryNode(0);
		for (unsigned int i = 0; i < pGeometryNode->GetNormalGeometryNum(); i++)
		{
			VSGeometry * pGeometry = pGeometryNode->GetNormalGeometry(i);
			pGeometry->SetMaterialInstance(pMaterialPhoneTwoPassR,0);
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



		VSOutputDebugString("Save VSEngine Model NewMonsterPhoneTwoPass\n");
		printf("Save VSEngine Model NewMonsterPhoneTwoPass\n");
		VSResourceManager::SaveResouce(pSkeletonMeshNode, _T("NewMonsterPhoneTwoPass"), true);

		// Phone
		VSOutputDebugString("Save VSEngine Material MaterialPhone\n");
		printf("Save VSEngine Material MaterialPhone\n");
		VSMaterialPhonePtr  pMaterialPhone = VS_NEW VSMaterialPhone(_T("MaterialPhone"), pDiffuseR, pNormalR, pSpecularR, pEmissiveR, true);
		VSResourceManager::SaveResouce(pMaterialPhone, _T("MaterialPhone"), true);

		VSOutputDebugString("Load VSEngine Material MaterialPhone\n");
		printf("Load VSEngine Material MaterialPhone\n");
		VSMaterialRPtr  pMaterialPhoneR = VSResourceManager::LoadASYNResource<VSMaterial>(_T("MaterialPhone.MATERIAL"), false);


		for (unsigned int i = 0; i < pGeometryNode->GetNormalGeometryNum(); i++)
		{
			VSGeometry * pGeometry = pGeometryNode->GetNormalGeometry(i);
			pGeometry->SetMaterialInstance(pMaterialPhoneR, 0);
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



		VSOutputDebugString("Save VSEngine Model NewMonsterPhone\n");
		printf("Save VSEngine Model NewMonsterPhone\n");
		VSResourceManager::SaveResouce(pSkeletonMeshNode, _T("NewMonsterPhone"), true);

		// BlinnPhone
		VSOutputDebugString("Save VSEngine Material MaterialBlinnPhone\n");
		printf("Save VSEngine Material MaterialBlinnPhone\n");
		VSMaterialPhonePtr  pMaterialBlinnPhone = VS_NEW VSMaterialPhone(_T("MaterialBlinnPhone"), pDiffuseR, pNormalR, pSpecularR, pEmissiveR, false);
		VSResourceManager::SaveResouce(pMaterialBlinnPhone, _T("MaterialBlinnPhone"), true);

		VSOutputDebugString("Load VSEngine Material MaterialBlinnPhone\n");
		printf("Load VSEngine Material MaterialBlinnPhone\n");
		VSMaterialRPtr  pMaterialBlinnPhoneR = VSResourceManager::LoadASYNResource<VSMaterial>(_T("MaterialBlinnPhone.MATERIAL"), false);


		for (unsigned int i = 0; i < pGeometryNode->GetNormalGeometryNum(); i++)
		{
			VSGeometry * pGeometry = pGeometryNode->GetNormalGeometry(i);
			pGeometry->SetMaterialInstance(pMaterialBlinnPhoneR, 0);
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



		VSOutputDebugString("Save VSEngine Model NewMonsterBlinnPhone\n");
		printf("Save VSEngine Model NewMonsterBlinnPhone\n");
		VSResourceManager::SaveResouce(pSkeletonMeshNode, _T("NewMonsterBlinnPhone"), true);



		//OrenNayar

		VSOutputDebugString("Save VSEngine Material MaterialOrenNayar\n");
		printf("Save VSEngine Material MaterialOrenNayar\n");
		VSMaterialOrenNayarPtr  pMaterialOrenNayar = VS_NEW VSMaterialOrenNayar(_T("MaterialOrenNayar"), pDiffuseR, pNormalR, pEmissiveR);
		VSResourceManager::SaveResouce(pMaterialOrenNayar, _T("MaterialOrenNayar"), true);

		VSOutputDebugString("Load VSEngine Material MaterialOrenNayar\n");
		printf("Load VSEngine Material MaterialOrenNayar\n");
		VSMaterialRPtr  pMaterialOrenNayarR = VSResourceManager::LoadASYNResource<VSMaterial>(_T("MaterialOrenNayar.MATERIAL"), false);


		for (unsigned int i = 0; i < pGeometryNode->GetNormalGeometryNum(); i++)
		{
			VSGeometry * pGeometry = pGeometryNode->GetNormalGeometry(i);
			pGeometry->SetMaterialInstance(pMaterialOrenNayarR, 0);
			if (i == 0)
			{
				pGeometry->GetMaterialInstance(0)->SetShaderTexture(_T("DiffuseTexture"), pDiffuseR);
				pGeometry->GetMaterialInstance(0)->SetShaderTexture(_T("NormalTexture"), pNormalR);
				pGeometry->GetMaterialInstance(0)->SetShaderTexture(_T("EmissiveTexture"), pEmissiveR);
			}
			else if (i == 1)
			{
				pGeometry->GetMaterialInstance(0)->SetShaderTexture(_T("DiffuseTexture"), pDiffuse_wR);
				pGeometry->GetMaterialInstance(0)->SetShaderTexture(_T("NormalTexture"), pNormal_wR);
				pGeometry->GetMaterialInstance(0)->SetShaderTexture(_T("EmissiveTexture"), pEmissive_wR);
			}

		}



		VSOutputDebugString("Save VSEngine Model NewMonsterOrenNayar\n");
		printf("Save VSEngine Model NewMonsterOrenNayar\n");
		VSResourceManager::SaveResouce(pSkeletonMeshNode, _T("NewMonsterOrenNayar"), true);
	
		//Custom
		VSOutputDebugString("Save VSEngine Material MaterialCustom\n");
		printf("Save VSEngine Material MaterialCustom\n");
		VSMaterialCustomPtr  pMaterialCustom = VS_NEW VSMaterialCustom(_T("MaterialCustom"), pDiffuseR, pNormalR, pEmissiveR);
		VSResourceManager::SaveResouce(pMaterialCustom, _T("MaterialCustom"), true);

		VSOutputDebugString("Load VSEngine Material MaterialCustom\n");
		printf("Load VSEngine Material MaterialCustom\n");
		VSMaterialRPtr  pMaterialCustomR = VSResourceManager::LoadASYNResource<VSMaterial>(_T("MaterialCustom.MATERIAL"), false);


		for (unsigned int i = 0; i < pGeometryNode->GetNormalGeometryNum(); i++)
		{
			VSGeometry * pGeometry = pGeometryNode->GetNormalGeometry(i);
			pGeometry->SetMaterialInstance(pMaterialCustomR, 0);
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



		VSOutputDebugString("Save VSEngine Model NewMonsterCustom\n");
		printf("Save VSEngine Model NewMonsterCustom\n");
		VSResourceManager::SaveResouce(pSkeletonMeshNode, _T("NewMonsterCustom"), true);

		//LightFunction

		// texture 
		VSOutputDebugString("Load LightFunction texture\n");
		printf("Load LightFunction texture\n");
		VSFileName LightFunctionFileName = VSTexAllState::ms_ResourcePath;
		LightFunctionFileName += _T("LightFunction.tga");
		VSTexAllStatePtr pLightFunction = VSResourceManager::Load2DTexture(LightFunctionFileName.GetBuffer(), (VSSamplerState *)VSSamplerState::GetTriLine(),
			VSResourceManager::CT_BC1, false, true,true);

		VSOutputDebugString("Save VSEngine texture LightFunction.LightFunction\n");
		printf("Save VSEngine texture LightFunction.LightFunction\n");
		VSResourceManager::SaveResouce(pLightFunction, _T("LightFunction/LightFunction"), true);

		VSOutputDebugString("Load VSEngine texture LightFunction.LightFunction\n");
		printf("Load VSEngine texture LightFunction.LightFunction\n");
		VSTexAllStateRPtr  pLightFunctionR = VSResourceManager::LoadASYNResource<VSTexAllState>(_T("LightFunction/LightFunction.TEXTURE"), false);

		VSOutputDebugString("Save VSEngine Material LightFunction\n");
		printf("Save VSEngine Material LightFunction\n");
		VSLightMaterialPtr  pLightMaterial = VS_NEW VSLightMaterial(_T("LightFunction"), pLightFunctionR);
		VSResourceManager::SaveResouce(pLightMaterial, _T("LightFunction"), true);

		return true;
	}
}
#endif