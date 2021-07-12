//输出一个地面模型，给后面Demo用。
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
		VSSamplerDesc SamplerDesc;
		SamplerDesc.m_uiMag = VSSamplerDesc::FM_LINE;
		SamplerDesc.m_uiMin = VSSamplerDesc::FM_LINE;
		SamplerDesc.m_uiMip = VSSamplerDesc::FM_LINE;
		VSSamplerStatePtr pTriLineSamplerState = VSResourceManager::CreateSamplerState(SamplerDesc);

		VSOutputDebugString("Load Wall_D tga texture\n");
		printf("Load Wall_D tga texture\n");
		VSFileName Wall_DFileName = VSTexAllState::ms_ResourcePath;
		Wall_DFileName  += _T("Wall_D.tga");
		VSTexAllStatePtr pWall_D = VSResourceManager::Load2DTexture(Wall_DFileName.GetBuffer(), pTriLineSamplerState,
			VSResourceManager::CT_BC1, false, true,true);

		VSOutputDebugString("Save VSEngine texture Wall.Wall_D\n");
		printf("Save VSEngine texture Wall.Wall_D\n");
		VSResourceManager::SaveResouce(pWall_D, _T("Wall/Wall_D"), true);

		VSOutputDebugString("Load Wall_N tga texture\n");
		printf("Load Wall_N tga texture\n");
		VSFileName Wall_NFileName = VSTexAllState::ms_ResourcePath;
		Wall_NFileName += _T("Wall_N.tga");
		VSTexAllStatePtr pWall_N = VSResourceManager::Load2DTexture(Wall_NFileName.GetBuffer(), pTriLineSamplerState, 
			VSResourceManager::CT_BC1, true, false, true);

		VSOutputDebugString("Save VSEngine texture Wall.Wall_N\n");
		printf("Save VSEngine texture Wall.Wall_N\n");
		VSResourceManager::SaveResouce(pWall_N, _T("Wall/Wall_N"), true);


		VSOutputDebugString("Load VSEngine texture Wall.Wall_D\n");
		printf("Load VSEngine texture Wall.Wall_D\n");
		VSTexAllStateRPtr  pWall_DR = VSResourceManager::LoadASYNResource<VSTexAllState>(_T("Wall/Wall_D.TEXTURE"), false);

		VSOutputDebugString("Load VSEngine texture Wall.Wall_N\n");
		printf("Load VSEngine texture Wall.Wall_N\n");
		VSTexAllStateRPtr  pWall_NR = VSResourceManager::LoadASYNResource<VSTexAllState>(_T("Wall/Wall_N.TEXTURE"), false);




		VSOutputDebugString("Load VSEngine Material TextureAndNormal\n");
		printf("Load VSEngine Material TextureAndNormal\n");
		VSMaterialRPtr  pMaterialR = VSResourceManager::LoadASYNResource<VSMaterial>(_T("TextureAndNormal.MATERIAL"), false);

		VSOutputDebugString("Load VSEngine Model OceanPlane\n");
		printf("Load VSEngine Model OceanPlane\n");
		VSStaticMeshNodeRPtr pModel = VSResourceManager::LoadASYNResource<VSStaticMeshNode>(_T("OceanPlane.STMODEL"), false);

		VSOutputDebugString("Set Material\n");
		printf("Set Material\n");
		VSStaticMeshNode * pStaticMeshNode = pModel->GetResource();
		VSGeometryNode * pGeometryNode = pStaticMeshNode->GetGeometryNode(0);
		for (unsigned int i = 0; i < pGeometryNode->GetNormalGeometryNum(); i++)
		{
			VSGeometry * pGeometry = pGeometryNode->GetNormalGeometry(i);
			pGeometry->SetMaterialInstance(pMaterialR,0);
			pGeometry->GetMaterialInstance(0)->SetShaderTexture(_T("DiffuseTexture"), pWall_DR);
			pGeometry->GetMaterialInstance(0)->SetShaderTexture(_T("NormalTexture"), pWall_NR);
		}
		printf("Save VSEngine Model NewOceanPlane\n");
		VSResourceManager::SaveResouce(pStaticMeshNode, _T("NewOceanPlane"), true);
		return true;
	}
}
#endif