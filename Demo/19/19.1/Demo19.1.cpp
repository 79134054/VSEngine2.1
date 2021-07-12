//输出一个地面模型，测试Tessellation。
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

		VSOutputDebugString("Load Displacement tga texture\n");
		printf("Load Displacement tga texture\n");
		VSFileName DisplacementDFileName = VSTexAllState::ms_ResourcePath;
		DisplacementDFileName += _T("Displacement.tga");
		VSTexAllStatePtr pDisplacement = VSResourceManager::Load2DTexture(DisplacementDFileName.GetBuffer(), pTriLineSamplerState, VSResourceManager::CT_BC3, false, false,true);

		VSOutputDebugString("Save VSEngine texture Displacement.Displacement\n");
		printf("Save VSEngine texture Displacement.Displacement\n");
		VSResourceManager::SaveResouce(pDisplacement, _T("Displacement/Displacement"), true);

		VSOutputDebugString("Load VSEngine texture Displacement.Displacement\n");
		printf("Load VSEngine texture Displacement.Displacement\n");
		VSTexAllStateRPtr  pDisplacementR = VSResourceManager::LoadASYNResource<VSTexAllState>(_T("Displacement/Displacement.TEXTURE"), false);

		VSOutputDebugString("Load VSEngine texture Wall.Wall_D\n");
		printf("Load VSEngine texture Wall.Wall_D\n");
		VSTexAllStateRPtr  pWall_DR = VSResourceManager::LoadASYNResource<VSTexAllState>(_T("Wall/Wall_D.TEXTURE"), false);

		VSOutputDebugString("Load VSEngine texture Wall.Wall_N\n");
		printf("Load VSEngine texture Wall.Wall_N\n");
		VSTexAllStateRPtr  pWall_NR = VSResourceManager::LoadASYNResource<VSTexAllState>(_T("Wall/Wall_N.TEXTURE"), false);

		VSOutputDebugString("Save VSEngine Material TessellationTest\n");
		printf("Save VSEngine Material TessellationTest\n");
		m_SaveMaterial = VS_NEW VSMaterialTessellation(_T("TessellationTest"), pWall_DR, pWall_NR, pDisplacementR);
		VSResourceManager::SaveResouce(m_SaveMaterial, _T("TessellationTest"), true);

		VSOutputDebugString("Load VSEngine Material TessellationTest\n");
		printf("Load VSEngine Material TessellationTest\n");
		VSMaterialRPtr  pMaterialR = VSResourceManager::LoadASYNResource<VSMaterial>(_T("TessellationTest.MATERIAL"), false);

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
		}
		printf("Save VSEngine Model TessellationOceanPlane\n");
		VSResourceManager::SaveResouce(pStaticMeshNode, _T("TessellationOceanPlane"), true);
		return true;
	}
}
#endif