#include "VSDx9Renderer.h"
#include "VS2DTexSampler.h"
#include "VSGeometry.h"
#include "VSString.h"
#include "VSResourceManager.h"
#include "VSShaderStringFactory.h"
#include "VSDirectionLight.h"
#include "VSPointLight.h"
#include "VSSpotLight.h"
#include "VSGraphicInclude.h"
#ifdef WINDOWS_PLATFORM	
using namespace VSEngine2;
void VSDX9Renderer::SetUserLightShadowMatrix(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID)
{
	VSLocalLight * pLocalLight = DynamicCast<VSLocalLight>(MSPara.pShadowLight);
	if (pLocalLight)
	{
		VSMatrix3X3W  * TempMatrix  = (VSMatrix3X3W  *)pShader->m_pUserConstant[ID]->GetData();
		*TempMatrix = pLocalLight->m_LightShadowMatrix;
		ID++;
	}
	else
	{
		VSMAC_ASSERT(0);
	}
}
void VSDX9Renderer::SetUserConstantSkin(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID)
{
	VSVertexBuffer * pVBuffer = MSPara.pGeometry->GetMeshData()->GetVertexBuffer();
	if(pVBuffer->HaveBlendIndicesInfo(0))
	{
		VSInstanceGeometry * pInstanceGeometry = DynamicCast<VSInstanceGeometry>(MSPara.pGeometry);
		if (pInstanceGeometry && pVBuffer->HaveBlendIndicesInfo(1))
		{
			VSVector3W  * TempVector = (VSVector3W  *)pShader->m_pUserConstant[ID]->GetData();
			*TempVector = VSVector3W(pInstanceGeometry->m_uiBoneNum * 1.0f, 
				pInstanceGeometry->m_AnimInstanceTexture->GetTexture()->GetWidth(0) * 1.0f, pInstanceGeometry->m_AnimInstanceTexture->GetTexture()->GetHeight(0) * 1.0f, 0.0f);
			ID++;
		}
		else
		{
			VSVector3W  * TempVector = (VSVector3W  *)pShader->m_pUserConstant[ID]->GetData();
			VSArray<VSVector3W> & Buffer = MSPara.pGeometry->GetSkinWeightBuffer();
			VSMemcpy(TempVector, Buffer.GetBuffer(), Buffer.GetNum() * sizeof(VSVector3W));
			ID++;
		}
		
	}
}
void VSDX9Renderer::SetUserConstantLightWorldPos(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID)
{
	VSVector3 * pLightWorldPos = (VSVector3 *) pShader->m_pUserConstant[ID]->GetData();
	*pLightWorldPos= MSPara.pShadowLight->GetWorldTranslate();
	ID++;
}
void VSDX9Renderer::SetUserConstantLightWorldDirection(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID)
{
	VSVector3 * pLightWorldDirection = (VSVector3 *) pShader->m_pUserConstant[ID]->GetData();

	const VSMatrix3X3 &Rotator = MSPara.pShadowLight->GetWorldRotate();
	VSVector3 U,V,N;
	Rotator.GetUVN(U,V,N);

	*pLightWorldDirection= N;
	ID++;
}
void VSDX9Renderer::SetUserConstantMorph(MaterialShaderPara &MSPara, VSShader * pShader, unsigned int& ID)
{
	if (MSPara.pGeometry->HasMorphTarget())
	{
		const MorphDataType & MDT = MSPara.pGeometry->GetMorphData();
		VSREAL* LayerAndNum = (VSREAL*)pShader->m_pUserConstant[ID]->GetData();
		LayerAndNum[0] = MDT.Layer * 1.0f;
		LayerAndNum[1] = MDT.ValueMap.GetNum() * 1.0f;
		LayerAndNum[2] = VSResourceManager::GetGpuMorphTextureSize() * 1.0f;
		LayerAndNum[3] = MDT.TextureSizeY * 1.0f;
		ID++;


		VSREAL* Value = (VSREAL*)pShader->m_pUserConstant[ID]->GetData();
		VSREAL TempValue[100] = { 0.0f };
		for (unsigned int i = 0; i < MDT.ValueMap.GetNum(); i++)
		{
			TempValue[i] = MDT.ValueMap[i].Value;
		}
		VSMemcpy(Value, TempValue, sizeof(VSREAL) * VSResourceManager::GetGpuMorphTargetNum());
		ID++;
	}
}
void VSDX9Renderer::SetUserConstantSimpleAnimInstanceTexture(MaterialShaderPara &MSPara, VSShader * pShader, unsigned int& ID)
{
	VSInstanceGeometry * pInstanceGeometry = DynamicCast<VSInstanceGeometry>(MSPara.pGeometry);
	if (!pInstanceGeometry)
	{
		return;
	}
	if (!pInstanceGeometry->HaveSkinInstance())
	{
		return;
	}
	pShader->m_pUserSampler[ID]->SetTex(pInstanceGeometry->m_AnimInstanceTexture, 0);
	ID++;
}
void VSDX9Renderer::SetUserConstantMorphTexture(MaterialShaderPara &MSPara, VSShader * pShader, unsigned int& ID)
{
	if (!MSPara.pGeometry->HasMorphTarget())
	{
		return;
	}
	const MorphDataType & MorphData = MSPara.pGeometry->GetMorphData();
	pShader->m_pUserSampler[ID]->SetTex(MorphData.MorphVDataTexture, 0);
	ID++;
	if (!MorphData.MorphNDataTexture)
	{
		return;
	}
	pShader->m_pUserSampler[ID]->SetTex(MorphData.MorphNDataTexture, 0);
	ID++;
	if (!MorphData.MorphTDataTexture)
	{
		return;
	}
	pShader->m_pUserSampler[ID]->SetTex(MorphData.MorphTDataTexture, 0);
	ID++;
	if (!MorphData.MorphBDataTexture)
	{
		return;
	}
	pShader->m_pUserSampler[ID]->SetTex(MorphData.MorphBDataTexture, 0);
	ID++;

}
void VSDX9Renderer::SetMaterialVShaderConstant(MaterialShaderPara &MSPara,VSVShader *pVShader)
{
	unsigned int ID = 0;
	unsigned int uiSamplerID = 0;
	unsigned int uiPassType = MSPara.uiPassType;
	if (uiPassType == VSPass::PT_MATERIAL 
		|| uiPassType == VSPass::PT_PREZ 
		|| uiPassType == VSPass::PT_NORMALDEPTH 
		|| uiPassType == VSPass::PT_INDIRECT
		|| uiPassType == VSPass::PT_POINT_CUBE_SHADOW)
	{	
		SetUserConstantWorldMatrix(MSPara, pVShader, ID);
		SetUserConstantViewProjectMatrix(MSPara,pVShader,ID);
		SetUserConstantCameraPos(MSPara, pVShader, ID);
		SetUserConstantSkin(MSPara,pVShader,ID);
		SetUserConstantMorph(MSPara, pVShader, ID);
		SetUserConstantMorphTexture(MSPara, pVShader, uiSamplerID);
		SetUserConstantSimpleAnimInstanceTexture(MSPara, pVShader, uiSamplerID);
	}
	else if (uiPassType == VSPass::PT_POINT_VOLUME_SHADOW)
	{
		SetUserConstantWorldMatrix(MSPara,pVShader,ID);
		SetUserConstantViewProjectMatrix(MSPara,pVShader,ID);
		SetUserConstantCameraPos(MSPara, pVShader, ID);
		SetUserConstantLightWorldPos(MSPara,pVShader,ID);
		SetUserConstantSkin(MSPara,pVShader,ID);
		SetUserConstantSimpleAnimInstanceTexture(MSPara, pVShader, uiSamplerID);
	}
	else if (uiPassType == VSPass::PT_DIRECT_VOLUME_SHADOW)
	{
		SetUserConstantWorldMatrix(MSPara,pVShader,ID);
		SetUserConstantViewProjectMatrix(MSPara,pVShader,ID);
		SetUserConstantCameraPos(MSPara, pVShader, ID);
		SetUserConstantLightWorldDirection(MSPara,pVShader,ID);
		SetUserConstantSkin(MSPara,pVShader,ID);
		SetUserConstantSimpleAnimInstanceTexture(MSPara, pVShader, uiSamplerID);
	}
	else if (uiPassType == VSPass::PT_SHADOW)
	{
		SetUserConstantWorldMatrix(MSPara, pVShader, ID);
		SetUserLightShadowMatrix(MSPara,pVShader,ID);
		SetUserConstantCameraPos(MSPara, pVShader, ID);
		SetUserConstantSkin(MSPara,pVShader,ID);
		SetUserConstantMorph(MSPara, pVShader, ID);
		SetUserConstantMorphTexture(MSPara, pVShader, uiSamplerID);
		SetUserConstantSimpleAnimInstanceTexture(MSPara, pVShader, uiSamplerID);
	}
	else if (uiPassType == VSPass::PT_DUAL_PARABOLOID_SHADOW)
	{
		SetUserConstantWorldMatrix(MSPara, pVShader, ID);
		SetUserConstantViewProjectMatrix(MSPara,pVShader,ID);
		SetUserConstantCameraPos(MSPara, pVShader, ID);
		SetUserConstantPointLightRange(MSPara,pVShader,ID);
		SetUserConstantSkin(MSPara,pVShader,ID);
		SetUserConstantMorph(MSPara, pVShader, ID);
		SetUserConstantMorphTexture(MSPara, pVShader, uiSamplerID);
		SetUserConstantSimpleAnimInstanceTexture(MSPara, pVShader, uiSamplerID);
	}

	else if (uiPassType == VSPass::PT_LIGHT_FUNCTION)
	{
		SetUserConstantInvRTWidth(MSPara,pVShader,ID);
	}
	MSPara.pMaterialInstance->SetShaderValue(pVShader);
}
void VSDX9Renderer::SetUserConstantWorldMatrix(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID)
{
	VSTransform World = MSPara.pGeometry->GetWorldTransform();
	VSMatrix3X3W  *TempMatrix  = (VSMatrix3X3W  *)pShader->m_pUserConstant[ID]->GetData();
	*TempMatrix = World.GetCombine();
	ID++;
}
void VSDX9Renderer::SetUserConstantCameraPos(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID)
{
	VSVector3 * pCameraPos = (VSVector3 *) pShader->m_pUserConstant[ID]->GetData();
	*pCameraPos= MSPara.pCamera->GetWorldTranslate();
	ID++;
}
void VSDX9Renderer::SetUserConstantSkyUpColor(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID)
{
	VSColorRGBA * pColor = (VSColorRGBA *) pShader->m_pUserConstant[ID]->GetData();
	*pColor= MSPara.SkyLightUpColor;
	ID++;
}
void VSDX9Renderer::SetUserConstantSkyDownColor(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID)
{
	VSColorRGBA * pColor = (VSColorRGBA *) pShader->m_pUserConstant[ID]->GetData();
	*pColor= MSPara.SkyLightDownColor;
	ID++;	
}
void VSDX9Renderer::SetUserConstantLight(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID)
{
	VSArray<VSLight*> & LightArray = MSPara.LightArray;

	int iLightNum[VSLight::LT_MAX] = { 0 };	
	VSVector3W * pLightBuffer[VSLight::LT_MAX] = { NULL };
	for (unsigned int i = 0 ; i < LightArray.GetNum() ; i++)
	{
		if (LightArray[i])
		{
			for (unsigned int j = 0 ; j < VSLight::LT_MAX ; j++)
			{
				if (LightArray[i]->GetLightType() == j)
				{

					iLightNum[j]++;
				}
			}
		}
	}

	for(unsigned int i = 0 ; i < VSLight::LT_MAX ; i++)
	{
		if (!iLightNum[i])
		{
			continue;
		}
		pLightBuffer[i] = (VSVector3W *) pShader->m_pUserConstant[ID]->GetData();
		ID++;
	}
	for (unsigned int i = 0 ; i < LightArray.GetNum() ; i++)
	{
		if (LightArray[i])
		{
			VSLight* pLight = LightArray[i];
			if (LightArray[i]->GetLightType() == VSLight::LT_DIRECTION)
			{

				*pLightBuffer[VSLight::LT_DIRECTION] = ((VSDirectionLight *)pLight)->m_Diffuse;
				pLightBuffer[VSLight::LT_DIRECTION]++;
				*pLightBuffer[VSLight::LT_DIRECTION] = ((VSDirectionLight *)pLight)->m_Specular;
				pLightBuffer[VSLight::LT_DIRECTION]++;
				const VSMatrix3X3 &Rotator = ((VSDirectionLight *)pLight)->GetWorldRotate();
				VSVector3 U,V,N;
				Rotator.GetUVN(U,V,N);

				pLightBuffer[VSLight::LT_DIRECTION]->x = N.x;
				pLightBuffer[VSLight::LT_DIRECTION]->y = N.y;
				pLightBuffer[VSLight::LT_DIRECTION]->z = N.z;
				pLightBuffer[VSLight::LT_DIRECTION]->w = ((VSDirectionLight *)pLight)->GetShadowResolution() * 1.0f;
				pLightBuffer[VSLight::LT_DIRECTION]++;

				pLightBuffer[VSLight::LT_DIRECTION]->x = ((VSDirectionLight *)pLight)->m_ZBias;

				pLightBuffer[VSLight::LT_DIRECTION]++;

				pLightBuffer[VSLight::LT_DIRECTION]->x = ((VSDirectionLight *)pLight)->m_LightFunScale.x;
				pLightBuffer[VSLight::LT_DIRECTION]->y = ((VSDirectionLight *)pLight)->m_LightFunScale.y;
				pLightBuffer[VSLight::LT_DIRECTION]->z = ((VSDirectionLight *)pLight)->m_LightFunOffset.x;
				pLightBuffer[VSLight::LT_DIRECTION]->w = ((VSDirectionLight *)pLight)->m_LightFunOffset.y;
				pLightBuffer[VSLight::LT_DIRECTION]++;

				VSMatrix3X3W WVP = ((VSLocalLight *)pLight)->GetWVP();
				VSVector3W WVPRow[4];
				WVP.GetRowVector(WVPRow[0],WVPRow[1],WVPRow[2],WVPRow[3]);
				*pLightBuffer[VSLight::LT_DIRECTION] = WVPRow[0];
				pLightBuffer[VSLight::LT_DIRECTION]++;
				*pLightBuffer[VSLight::LT_DIRECTION] = WVPRow[1];
				pLightBuffer[VSLight::LT_DIRECTION]++;
				*pLightBuffer[VSLight::LT_DIRECTION] = WVPRow[2];
				pLightBuffer[VSLight::LT_DIRECTION]++;
				*pLightBuffer[VSLight::LT_DIRECTION] = WVPRow[3];
				pLightBuffer[VSLight::LT_DIRECTION]++;
				if (((VSDirectionLight *)pLight)->GetShadowType() == VSDirectionLight::ST_VOLUME)
				{
					unsigned int uiRTWidth = m_uiCurRTWidth;
					unsigned int uiRTHeight = m_uiCurRTHeight;
					VSREAL fOffsetX = 0.5f + (0.5f / (VSREAL)(uiRTWidth));
					VSREAL fOffsetY = 0.5f + (0.5f / (VSREAL)(uiRTHeight));
					VSMatrix3X3W texScaleBiasMat(	0.5f,     0.0f,     0.0f,       0.0f,
						0.0f,    -0.5f,     0.0f,       0.0f,
						0.0f,     0.0f,     1.0f,		0.0f,
						fOffsetX, fOffsetY, 0.0f,       1.0f );
					VSMatrix3X3W Mat = MSPara.pCamera->GetViewMatrix() * MSPara.pCamera->GetProjMatrix() * texScaleBiasMat;

					VSVector3W Row[4];
					Mat.GetRowVector(Row[0],Row[1],Row[2],Row[3]);
					*pLightBuffer[VSLight::LT_DIRECTION] = Row[0];
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = Row[1];
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = Row[2];
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = Row[3];
					pLightBuffer[VSLight::LT_DIRECTION]++;

					*pLightBuffer[VSLight::LT_DIRECTION] = VSVector3W();
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = VSVector3W();
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = VSVector3W();
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = VSVector3W();
					pLightBuffer[VSLight::LT_DIRECTION]++;

					*pLightBuffer[VSLight::LT_DIRECTION] = VSVector3W();
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = VSVector3W();
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = VSVector3W();
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = VSVector3W();
					pLightBuffer[VSLight::LT_DIRECTION]++;
				}
				else if (((VSDirectionLight *)pLight)->GetShadowType() == VSDirectionLight::ST_CSM)
				{
					//shadow
					unsigned int uiRTWidth = ((VSDirectionLight *)pLight)->GetShadowResolution();

					VSREAL fOffsetX = 0.5f + (0.5f / (VSREAL)(uiRTWidth));
					VSREAL fOffsetY = 0.5f + (0.5f / (VSREAL)(uiRTWidth));
					VSMatrix3X3W texScaleBiasMat(	0.5f,     0.0f,     0.0f,       0.0f,
						0.0f,    -0.5f,     0.0f,       0.0f,
						0.0f,     0.0f,     1.0f,		0.0f,
						fOffsetX, fOffsetY, 0.0f,       1.0f );
					VSMatrix3X3W Mat = ((VSDirectionLight *)pLight)->m_CSMLightShadowMatrix[0] * texScaleBiasMat;

					VSVector3W Row[4];
					Mat.GetRowVector(Row[0],Row[1],Row[2],Row[3]);
					*pLightBuffer[VSLight::LT_DIRECTION] = Row[0];
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = Row[1];
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = Row[2];
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = Row[3];
					pLightBuffer[VSLight::LT_DIRECTION]++;

					Mat = ((VSDirectionLight *)pLight)->m_CSMLightShadowMatrix[1] * texScaleBiasMat;

					Mat.GetRowVector(Row[0],Row[1],Row[2],Row[3]);
					*pLightBuffer[VSLight::LT_DIRECTION] = Row[0];
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = Row[1];
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = Row[2];
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = Row[3];
					pLightBuffer[VSLight::LT_DIRECTION]++;

					Mat = ((VSDirectionLight *)pLight)->m_CSMLightShadowMatrix[2] * texScaleBiasMat;

					Mat.GetRowVector(Row[0],Row[1],Row[2],Row[3]);
					*pLightBuffer[VSLight::LT_DIRECTION] = Row[0];
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = Row[1];
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = Row[2];
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = Row[3];
					pLightBuffer[VSLight::LT_DIRECTION]++;
					
				}
				else
				{
					//shadow
					unsigned int uiRTWidth = ((VSDirectionLight *)pLight)->GetShadowResolution();

					VSREAL fOffsetX = 0.5f + (0.5f / (VSREAL)(uiRTWidth));
					VSREAL fOffsetY = 0.5f + (0.5f / (VSREAL)(uiRTWidth));
					VSMatrix3X3W texScaleBiasMat(	0.5f,     0.0f,     0.0f,       0.0f,
						0.0f,    -0.5f,     0.0f,       0.0f,
						0.0f,     0.0f,     1.0f,		0.0f,
						fOffsetX, fOffsetY, 0.0f,       1.0f );
					VSMatrix3X3W Mat = ((VSDirectionLight *)pLight)->m_LightShadowMatrix * texScaleBiasMat;

					VSVector3W Row[4];
					Mat.GetRowVector(Row[0],Row[1],Row[2],Row[3]);
					*pLightBuffer[VSLight::LT_DIRECTION] = Row[0];
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = Row[1];
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = Row[2];
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = Row[3];
					pLightBuffer[VSLight::LT_DIRECTION]++;

					*pLightBuffer[VSLight::LT_DIRECTION] = VSVector3W();
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = VSVector3W();
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = VSVector3W();
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = VSVector3W();
					pLightBuffer[VSLight::LT_DIRECTION]++;

					*pLightBuffer[VSLight::LT_DIRECTION] = VSVector3W();
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = VSVector3W();
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = VSVector3W();
					pLightBuffer[VSLight::LT_DIRECTION]++;
					*pLightBuffer[VSLight::LT_DIRECTION] = VSVector3W();
					pLightBuffer[VSLight::LT_DIRECTION]++;	
				}
			}
			else if (LightArray[i]->GetLightType() == VSLight::LT_POINT)
			{
				*pLightBuffer[VSLight::LT_POINT] = ((VSPointLight *)pLight)->m_Diffuse;
				pLightBuffer[VSLight::LT_POINT]++;
				*pLightBuffer[VSLight::LT_POINT] = ((VSPointLight *)pLight)->m_Specular;
				pLightBuffer[VSLight::LT_POINT]++;
				pLightBuffer[VSLight::LT_POINT]->x = ((VSPointLight *)pLight)->GetWorldTranslate().x;
				pLightBuffer[VSLight::LT_POINT]->y = ((VSPointLight *)pLight)->GetWorldTranslate().y;
				pLightBuffer[VSLight::LT_POINT]->z = ((VSPointLight *)pLight)->GetWorldTranslate().z;
				pLightBuffer[VSLight::LT_POINT]->w = ((VSPointLight *)pLight)->GetRange();
				pLightBuffer[VSLight::LT_POINT]++;
				pLightBuffer[VSLight::LT_POINT]->x = ((VSPointLight *)pLight)->GetShadowResolution() * 1.0f;
				pLightBuffer[VSLight::LT_POINT]->y = ((VSPointLight *)pLight)->m_ZBias;
				pLightBuffer[VSLight::LT_POINT]++;

				pLightBuffer[VSLight::LT_POINT]->x = ((VSPointLight *)pLight)->m_LightFunScale.x;
				pLightBuffer[VSLight::LT_POINT]->y = ((VSPointLight *)pLight)->m_LightFunScale.y;
				pLightBuffer[VSLight::LT_POINT]->z = ((VSPointLight *)pLight)->m_LightFunOffset.x;
				pLightBuffer[VSLight::LT_POINT]->w = ((VSPointLight *)pLight)->m_LightFunOffset.y;
				pLightBuffer[VSLight::LT_POINT]++;

				VSMatrix3X3W WVP = ((VSLocalLight *)pLight)->GetWVP();
				VSVector3W WVPRow[4];
				WVP.GetRowVector(WVPRow[0],WVPRow[1],WVPRow[2],WVPRow[3]);
				*pLightBuffer[VSLight::LT_POINT] = WVPRow[0];
				pLightBuffer[VSLight::LT_POINT]++;
				*pLightBuffer[VSLight::LT_POINT] = WVPRow[1];
				pLightBuffer[VSLight::LT_POINT]++;
				*pLightBuffer[VSLight::LT_POINT] = WVPRow[2];
				pLightBuffer[VSLight::LT_POINT]++;
				*pLightBuffer[VSLight::LT_POINT] = WVPRow[3];
				pLightBuffer[VSLight::LT_POINT]++;

				if (((VSPointLight *)pLight)->GetShadowType() == VSPointLight::ST_VOLUME)
				{

					unsigned int uiRTWidth = m_uiCurRTWidth;
					unsigned int uiRTHeight = m_uiCurRTHeight;
					VSREAL fOffsetX = 0.5f + (0.5f / (VSREAL)(uiRTWidth));
					VSREAL fOffsetY = 0.5f + (0.5f / (VSREAL)(uiRTHeight));
					VSMatrix3X3W texScaleBiasMat(	0.5f,     0.0f,     0.0f,       0.0f,
													0.0f,    -0.5f,     0.0f,       0.0f,
													0.0f,     0.0f,     1.0f,		0.0f,
													fOffsetX, fOffsetY, 0.0f,       1.0f );
					VSMatrix3X3W Mat = MSPara.pCamera->GetViewMatrix() * MSPara.pCamera->GetProjMatrix() * texScaleBiasMat;

					VSVector3W Row[4];
					Mat.GetRowVector(Row[0],Row[1],Row[2],Row[3]);
					*pLightBuffer[VSLight::LT_POINT] = Row[0];
					pLightBuffer[VSLight::LT_POINT]++;
					*pLightBuffer[VSLight::LT_POINT] = Row[1];
					pLightBuffer[VSLight::LT_POINT]++;
					*pLightBuffer[VSLight::LT_POINT] = Row[2];
					pLightBuffer[VSLight::LT_POINT]++;
					*pLightBuffer[VSLight::LT_POINT] = Row[3];
					pLightBuffer[VSLight::LT_POINT]++;


				}
				else
				{
					//shadow
					*pLightBuffer[VSLight::LT_POINT] = VSVector3W();
					pLightBuffer[VSLight::LT_POINT]++;
					*pLightBuffer[VSLight::LT_POINT] = VSVector3W();
					pLightBuffer[VSLight::LT_POINT]++;
					*pLightBuffer[VSLight::LT_POINT] = VSVector3W();
					pLightBuffer[VSLight::LT_POINT]++;
					*pLightBuffer[VSLight::LT_POINT] = VSVector3W();
					pLightBuffer[VSLight::LT_POINT]++;
				}
				
			}
			else if (LightArray[i]->GetLightType() == VSLight::LT_SPOT)
			{
				*pLightBuffer[VSLight::LT_SPOT] = ((VSSpotLight *)pLight)->m_Diffuse;
				pLightBuffer[VSLight::LT_SPOT]++;
				*pLightBuffer[VSLight::LT_SPOT] = ((VSSpotLight *)pLight)->m_Specular;
				pLightBuffer[VSLight::LT_SPOT]++;
				pLightBuffer[VSLight::LT_SPOT]->x = ((VSSpotLight *)pLight)->GetWorldTranslate().x;
				pLightBuffer[VSLight::LT_SPOT]->y = ((VSSpotLight *)pLight)->GetWorldTranslate().y;
				pLightBuffer[VSLight::LT_SPOT]->z = ((VSSpotLight *)pLight)->GetWorldTranslate().z;
				pLightBuffer[VSLight::LT_SPOT]->w = ((VSSpotLight *)pLight)->GetRange();
				pLightBuffer[VSLight::LT_SPOT]++;
				const VSMatrix3X3 &Rotator = ((VSSpotLight *)pLight)->GetWorldRotate();
				VSVector3 U,V,N;
				Rotator.GetUVN(U,V,N);
				pLightBuffer[VSLight::LT_SPOT]->x = N.x;
				pLightBuffer[VSLight::LT_SPOT]->y = N.y;
				pLightBuffer[VSLight::LT_SPOT]->z = N.z;
				pLightBuffer[VSLight::LT_SPOT]->w = ((VSDirectionLight *)pLight)->GetShadowResolution() * 1.0f;
				pLightBuffer[VSLight::LT_SPOT]++;

				pLightBuffer[VSLight::LT_SPOT]->x = ((VSSpotLight *)pLight)->GetFalloff();
				pLightBuffer[VSLight::LT_SPOT]->y = ((VSSpotLight *)pLight)->GetTheta();
				pLightBuffer[VSLight::LT_SPOT]->z = ((VSSpotLight *)pLight)->GetPhi();
				pLightBuffer[VSLight::LT_SPOT]->w = ((VSSpotLight *)pLight)->m_ZBias;
				pLightBuffer[VSLight::LT_SPOT]++;

				pLightBuffer[VSLight::LT_SPOT]->x = ((VSSpotLight *)pLight)->m_LightFunScale.x;
				pLightBuffer[VSLight::LT_SPOT]->y = ((VSSpotLight *)pLight)->m_LightFunScale.y;
				pLightBuffer[VSLight::LT_SPOT]->z = ((VSSpotLight *)pLight)->m_LightFunOffset.x;
				pLightBuffer[VSLight::LT_SPOT]->w = ((VSSpotLight *)pLight)->m_LightFunOffset.y;
				pLightBuffer[VSLight::LT_SPOT]++;

				VSMatrix3X3W WVP = ((VSLocalLight *)pLight)->GetWVP();
				VSVector3W WVPRow[4];
				WVP.GetRowVector(WVPRow[0],WVPRow[1],WVPRow[2],WVPRow[3]);
				*pLightBuffer[VSLight::LT_SPOT] = WVPRow[0];
				pLightBuffer[VSLight::LT_SPOT]++;
				*pLightBuffer[VSLight::LT_SPOT] = WVPRow[1];
				pLightBuffer[VSLight::LT_SPOT]++;
				*pLightBuffer[VSLight::LT_SPOT] = WVPRow[2];
				pLightBuffer[VSLight::LT_SPOT]++;
				*pLightBuffer[VSLight::LT_SPOT] = WVPRow[3];
				pLightBuffer[VSLight::LT_SPOT]++;


				//shadow
				unsigned int uiRTWidth = ((VSSpotLight *)pLight)->GetShadowResolution();

				VSREAL fOffsetX = 0.5f + (0.5f / (VSREAL)(uiRTWidth));
				VSREAL fOffsetY = 0.5f + (0.5f / (VSREAL)(uiRTWidth));
				VSMatrix3X3W texScaleBiasMat(	0.5f,     0.0f,     0.0f,       0.0f,
					0.0f,    -0.5f,     0.0f,       0.0f,
					0.0f,     0.0f,     1.0f,		0.0f,
					fOffsetX, fOffsetY, 0.0f,       1.0f );
				VSMatrix3X3W Mat = ((VSSpotLight *)pLight)->m_LightShadowMatrix * texScaleBiasMat;

				VSVector3W Row[4];
				Mat.GetRowVector(Row[0],Row[1],Row[2],Row[3]);
				*pLightBuffer[VSLight::LT_SPOT] = Row[0];
				pLightBuffer[VSLight::LT_SPOT]++;
				*pLightBuffer[VSLight::LT_SPOT] = Row[1];
				pLightBuffer[VSLight::LT_SPOT]++;
				*pLightBuffer[VSLight::LT_SPOT] = Row[2];
				pLightBuffer[VSLight::LT_SPOT]++;
				*pLightBuffer[VSLight::LT_SPOT] = Row[3];
				pLightBuffer[VSLight::LT_SPOT]++;

			}
		}
	}
}
void VSDX9Renderer::SetUserConstantLightFunctionSampler(MaterialShaderPara &MSPara, VSShader * pShader)
{
	unsigned int ID = 0;
	VSArray<VSLight*> & LightArray = MSPara.LightArray;
	for (unsigned int i = 0; i < LightArray.GetNum(); i++)
	{
		VSLocalLight * pLocalLight = DynamicCast<VSLocalLight>(LightArray[i]);
		if (pLocalLight && pLocalLight->HaveLightFun())
		{

			pShader->SetParam(VSShaderStringFactory::ms_PSConstantLightFunResource[ID], pLocalLight->GetLightFunDiffuseTexture(), 0);
			ID++;

		}
	}
}
void VSDX9Renderer::SetUserConstantShadowSampler(MaterialShaderPara &MSPara, VSShader * pShader)
{
	VSArray<VSLight*> & LightArray = MSPara.LightArray;
	unsigned int ID = 0;
	for (unsigned int i = 0; i < LightArray.GetNum(); i++)
	{
		VSLocalLight * pLocalLight = DynamicCast<VSLocalLight>(LightArray[i]);
		if (pLocalLight && pLocalLight->GetCastShadow())
		{
			for (unsigned int j = 0; j < pLocalLight->GetShadowTextureNum(); j++)
			{
				pShader->SetParam(VSShaderStringFactory::ms_PSConstantShadowResource[ID], pLocalLight->GetShadowTexture(j), j);
			}
			ID++;

		}
	}
}
void VSDX9Renderer::SetUserConstantFarZ(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID)
{
	VSREAL  * Temp  = (VSREAL *)pShader->m_pUserConstant[ID]->GetData();
	*Temp = MSPara.pCamera->GetZFar();	
	ID++;
}
void VSDX9Renderer::SetUserConstantPointLightRange(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID)
{
	VSREAL  * Temp  = (VSREAL *)pShader->m_pUserConstant[ID]->GetData();
	VSPointLight * pPointLight = DynamicCast<VSPointLight>(MSPara.pShadowLight);
	VSMAC_ASSERT(pPointLight);
	*Temp = pPointLight->GetRange();
	ID++;
}
void VSDX9Renderer::SetUserConstantInvRTWidth(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID)
{
	VSREAL  * Temp  = (VSREAL *)pShader->m_pUserConstant[ID]->GetData();
	VSLocalLight * pLight = DynamicCast<VSLocalLight>(MSPara.pShadowLight);
	VSMAC_ASSERT(pLight);
	*Temp = 1.0f / pLight->GetLightMaterialRtWidth();	
	ID++;
}
void VSDX9Renderer::SetUserConstantProjectMatrix(MaterialShaderPara &MSPara, VSShader * pShader, unsigned int& ID)
{
	VSMatrix3X3W  *TempMatrix = (VSMatrix3X3W  *)pShader->m_pUserConstant[ID]->GetData();
	*TempMatrix = MSPara.pCamera->GetProjMatrix();
	ID++;
}
void VSDX9Renderer::SetUserConstantViewMatrix(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID)
{
	VSMatrix3X3W  *TempMatrix  = (VSMatrix3X3W  *)pShader->m_pUserConstant[ID]->GetData();
	*TempMatrix = MSPara.pCamera->GetViewMatrix();
	ID++;
}
void VSDX9Renderer::SetUserConstantViewProjectMatrix(MaterialShaderPara &MSPara,VSShader * pShader,unsigned int& ID)
{
	VSMatrix3X3W  *TempMatrix  = (VSMatrix3X3W  *)pShader->m_pUserConstant[ID]->GetData();
	*TempMatrix = MSPara.pCamera->GetViewMatrix() * MSPara.pCamera->GetProjMatrix();
	ID++;
}
void VSDX9Renderer::SetMaterialPShaderConstant(MaterialShaderPara &MSPara,VSPShader *pPShader)
{
	unsigned int ID = 0;
	unsigned int uiPassType = MSPara.uiPassType;
	if (uiPassType == VSPass::PT_MATERIAL)
	{

		SetUserConstantWorldMatrix(MSPara,pPShader,ID);
		SetUserConstantViewMatrix(MSPara,pPShader,ID);
		SetUserConstantProjectMatrix(MSPara, pPShader, ID);
		SetUserConstantCameraPos(MSPara,pPShader,ID);
		SetUserConstantFarZ(MSPara,pPShader,ID);
		if (MSPara.LightArray.GetNum() > 0)
		{
			SetUserConstantLight(MSPara,pPShader,ID);	
		}
		
		SetUserConstantShadowSampler(MSPara,pPShader);	
		SetUserConstantLightFunctionSampler(MSPara,pPShader);

	}
	else if (uiPassType == VSPass::PT_INDIRECT)
	{
		SetUserConstantWorldMatrix(MSPara,pPShader,ID);
		SetUserConstantViewMatrix(MSPara,pPShader,ID);
		SetUserConstantProjectMatrix(MSPara, pPShader, ID);
		SetUserConstantCameraPos(MSPara,pPShader,ID);
		SetUserConstantSkyUpColor(MSPara,pPShader,ID);
		SetUserConstantSkyDownColor(MSPara,pPShader,ID);
	}
	else if (uiPassType == VSPass::PT_NORMALDEPTH)
	{
		SetUserConstantWorldMatrix(MSPara, pPShader, ID);
		SetUserConstantViewMatrix(MSPara, pPShader, ID);
		SetUserConstantProjectMatrix(MSPara, pPShader, ID);
		SetUserConstantCameraPos(MSPara, pPShader, ID);
		SetUserConstantFarZ(MSPara,pPShader,ID);
	}
	else if (uiPassType == VSPass::PT_POINT_CUBE_SHADOW)
	{
		SetUserConstantWorldMatrix(MSPara,pPShader,ID);
		SetUserConstantCameraPos(MSPara,pPShader,ID);

		SetUserConstantPointLightRange(MSPara,pPShader,ID);
	}
	else if (uiPassType == VSPass::PT_SHADOW)
	{
		SetUserLightShadowMatrix(MSPara, pPShader, ID);
		SetUserConstantWorldMatrix(MSPara, pPShader, ID);
		SetUserConstantCameraPos(MSPara, pPShader, ID);
	}
	MSPara.pMaterialInstance->SetShaderValue(pPShader);
}

void VSDX9Renderer::SetMaterialGShaderConstant(MaterialShaderPara &MSPara,VSGShader *pGShader)
{
	VSMAC_ASSERT(0); return;
}
void VSDX9Renderer::SetMaterialHShaderConstant(MaterialShaderPara &MSPara, VSHShader * pHShader) { VSMAC_ASSERT(0); return; }
void VSDX9Renderer::SetMaterialDShaderConstant(MaterialShaderPara &MSPara, VSDShader *pDShader) { VSMAC_ASSERT(0); return; }
#endif