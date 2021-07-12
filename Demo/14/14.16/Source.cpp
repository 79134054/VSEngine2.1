//创建叠加动画和创建Pos动画，取攻击动画的第600毫秒和待机动画的第0毫秒做差值，
//然后叠加到待机动画上去播放，用攻击动画的第600毫秒做Pos动画，Demo14.4中第6个和第7个位这两个动画的播放。
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