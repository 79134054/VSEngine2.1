//�������Ӷ����ʹ���Pos������ȡ���������ĵ�600����ʹ��������ĵ�0��������ֵ��
//Ȼ����ӵ�����������ȥ���ţ��ù��������ĵ�600������Pos������Demo14.4�е�6���͵�7��λ�����������Ĳ��š�
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


		VSOutputDebugString("Load VSEngine Action Idle\n");
		printf("Load VSEngine Action Idle\n");
		VSAnimRPtr  pAnim1 = VSResourceManager::LoadASYNResource<VSAnim>("Idle", false);


		VSOutputDebugString("Load VSEngine Action Attack2\n");
		printf("Load VSEngine Action Attack2\n");
		VSAnimRPtr  pAnim2 = VSResourceManager::LoadASYNResource<VSAnim>("Attack2", false);


		VSOutputDebugString("Create Additive Action\n");
		printf("Create Additive Action\n");
		VSAnimPtr pAddtiveAnim = VSResourceManager::CreateAdditiveAnim(pAnim2->GetResource(), pAnim1->GetResource(), pAnim1, 600.0f, 0.0f);

	
		pAddtiveAnim->ComputeAnimLength();
		pAddtiveAnim->Compress();

		VSOutputDebugString("Save Additive Action\n");
		printf("Save Additive Action\n");

		VSResourceManager::SaveResouce(pAddtiveAnim, _T("AddtiveAnim"),true);

		VSOutputDebugString("Create Pos Action\n");
		printf("Create Pos Action\n");
		VSAnimPtr pPosAnim = VSResourceManager::CreateAnim(pAnim2->GetResource(), 600.0f);

		pPosAnim->ComputeAnimLength();
		pPosAnim->Compress();

		VSOutputDebugString("Save Pos Action\n");
		printf("Save Additive Pos\n");

		VSResourceManager::SaveResouce(pPosAnim, _T("Pos"),true);
		return true;
	}
}
#endif