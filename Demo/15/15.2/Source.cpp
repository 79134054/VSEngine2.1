//创建和存储后期链，分别为屏幕灰色后期链、屏幕灰色加自定义材质链。
#ifndef VSSAMPLE_H
#define VSSAMPLE_H
#include "VSApplication.h"
#include "VSPostEffectSet.h"
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

		//Model
		VSOutputDebugString("Create PostEffect Gray\n");
		printf("Create PostEffect Gray\n");
		
		VSPESetGrayPtr pPESetGray = VS_NEW VSPESetGray(_T("PostEffect_Gray"));

		VSOutputDebugString("Save VSEngine PostEffect PostEffect_Gray\n");
		printf("Save VSEngine PostEffect PostEffect_Gray\n");
		VSResourceManager::SaveResouce(pPESetGray, _T("PostEffect_Gray"), true);


		VSOutputDebugString("Save VSEngine Material PostEffectMaterial\n");
		printf("Save VSEngine Material PostEffectMaterial\n");
		VSMaterialPtr pSaveMaterial = VS_NEW VSPostEffectMaterial(_T("PostEffectMaterial"));
		VSResourceManager::SaveResouce(pSaveMaterial, _T("PostEffectMaterial"), true);

		VSOutputDebugString("Load VSEngine Material PostEffectMaterial\n");
		printf("Load VSEngine Material PostEffectMaterial\n");
		VSMaterialRPtr  pMaterialR = VSResourceManager::LoadASYNResource<VSMaterial>(_T("PostEffectMaterial.MATERIAL"), false);

		VSOutputDebugString("Create PostEffect MaterialAndGray\n");
		printf("Create PostEffect MaterialAndGray\n");

		VSPESetMaterialAndGrayPtr pPESetMaterialAndGray = VS_NEW VSPESetMaterialAndGray(_T("PostEffect_MaterialAndGray"),pMaterialR);

		VSOutputDebugString("Save VSEngine PostEffect PostEffect_MaterialAndGray\n");
		printf("Save VSEngine PostEffect PostEffect_MaterialAndGray\n");
		VSResourceManager::SaveResouce(pPESetMaterialAndGray, _T("PostEffect_MaterialAndGray"), true);

		VSPESetMaterial_Gray_BloomPtr pPESetMaterial_Gray_Bloom = VS_NEW VSPESetMaterial_Gray_Bloom(_T("PostEffect_MaterialAndGray"), pMaterialR);
		VSOutputDebugString("Save VSEngine PostEffect PostEffect_Material_Gray_Bloom\n");
		printf("Save VSEngine PostEffect PostEffect_Material_Gray_Bloom\n");
		VSResourceManager::SaveResouce(pPESetMaterial_Gray_Bloom, _T("PostEffect_Material_Gray_Bloom"), true);
		return true;
	}
}
#endif