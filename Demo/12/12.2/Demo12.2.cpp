//这个demo给导出的Stone.STMODEL创建一个材质资源，并赋予它，然后导出新的模型叫做NewStone.STMODEL。
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

		VSOutputDebugString("Load diffuse tga texture\n");
		printf("Load diffuse tga texture\n");
		VSFileName DiffuseFileName = VSTexAllState::ms_ResourcePath;
		DiffuseFileName += _T("stone_d.tga");
		VSTexAllStatePtr pDiffuse = VSResourceManager::Load2DTexture(DiffuseFileName.GetBuffer(), pTriLineSamplerState,
			VSResourceManager::CT_BC2, false, true,true);

		VSOutputDebugString("Save VSEngine texture Stone.Diffuse\n");
		printf("Save VSEngine texture Stone.Diffuse\n");
		VSResourceManager::SaveResouce(pDiffuse, _T("Stone/Diffuse"),true);

		VSOutputDebugString("Load normal tga texture\n");
		printf("Load normal tga texture\n");
		VSFileName NormalFileName = VSTexAllState::ms_ResourcePath;
		NormalFileName += _T("stone_n.tga");
		VSTexAllStatePtr pNormal = VSResourceManager::Load2DTexture(NormalFileName.GetBuffer(), pTriLineSamplerState, 
			VSResourceManager::CT_BC2, true, false,true);

		VSOutputDebugString("Save VSEngine texture Stone.Normal\n");
		printf("Save VSEngine texture Stone.Normal\n");
		VSResourceManager::SaveResouce(pNormal, _T("Stone/Normal"), true);

		VSOutputDebugString("Load VSEngine texture Stone.Diffuse\n");
		printf("Load VSEngine texture Stone.Diffuse\n");
		VSTexAllStateRPtr  pDiffuseR =  VSResourceManager::LoadASYNResource<VSTexAllState>(_T("Stone/Diffuse.TEXTURE"), false);

		VSOutputDebugString("Load VSEngine texture Stone.Normal\n");
		printf("Load VSEngine texture Stone.Normal\n");
		VSTexAllStateRPtr  pNormalR = VSResourceManager::LoadASYNResource<VSTexAllState>(_T("Stone/Normal.TEXTURE"), false);

		VSOutputDebugString("Save VSEngine Material TextureAndNormal\n");
		printf("Save VSEngine Material TextureAndNormal\n");
		m_SaveMaterial = VS_NEW VSMaterialTextureAndNormal(_T("TextureAndNormal"), pDiffuseR,pNormalR);
		VSResourceManager::SaveResouce(m_SaveMaterial, _T("TextureAndNormal"),true);


		VSOutputDebugString("Load VSEngine Material TextureAndNormal\n");
		printf("Load VSEngine Material TextureAndNormal\n");
		VSMaterialRPtr  pMaterialR = VSResourceManager::LoadASYNResource<VSMaterial>(_T("TextureAndNormal.MATERIAL"), false);

		VSOutputDebugString("Load VSEngine Model Stone\n");
		printf("Load VSEngine Model Stone\n");
		VSStaticMeshNodeRPtr pModel = VSResourceManager::LoadASYNResource<VSStaticMeshNode>(_T("Stone.STMODEL"), false);

		VSOutputDebugString("Set Material\n");
		printf("Set Material\n");
		VSStaticMeshNode * pStaticMeshNode = pModel->GetResource();
		VSGeometryNode * pGeometryNode = pStaticMeshNode->GetGeometryNode(0);
		for (unsigned int i = 0; i < pGeometryNode->GetNormalGeometryNum(); i++)
		{
			VSGeometry * pGeometry = pGeometryNode->GetNormalGeometry(i);
			pGeometry->SetMaterialInstance(pMaterialR,0);
		}
		VSOutputDebugString("Save VSEngine Model NewStone\n");
		printf("Save VSEngine Model NewStone\n");
		VSResourceManager::SaveResouce(pStaticMeshNode, _T("NewStone"), true);
		return true;
	}
}
#endif