#pragma once
#include "VSResource.h"
#include "VSVertexFormat.h"
#include "VSShaderMap.h"
#include "VS1DTexture.h"
#include "VSCubeTexture.h"
#include "VS2DTexture.h"
#include "VS3DTexture.h"
#include "VSBufferResource.h"
#include "VSMeshData.h"
#include "VSConfig.h"
#include "VSASYNLoader.h"
namespace VSEngine2
{
	class VSDataBuffer;
	class VSFont;
	class VSShaderProgram;
	class VSVertexBuffer;
	class VSDynamicBufferGeometry;
	class VSGeometry;
	class VSName;
	class VSCaptureTexAllState;
	class VSActor;
	class VSSceneMap;

	DECLARE_Ptr(VSTexAllState);
	DECLARE_Ptr(VSName);
	DECLARE_Ptr(VSVShader);
	DECLARE_Ptr(VSPShader);
	DECLARE_Ptr(VSGShader);
	DECLARE_Ptr(VSHShader);
	DECLARE_Ptr(VSDShader);
	DECLARE_Ptr(VSSkeletonMeshNode);
	DECLARE_Ptr(VSStaticMeshNode);
	DECLARE_Ptr(VSAnimSet);
	DECLARE_Ptr(VSMaterial);
	DECLARE_Ptr(VSFont);
	DECLARE_Ptr(VSVertexFormat);
	DECLARE_Ptr(VSAnimTree);
	DECLARE_Ptr(VSAnim);
	DECLARE_Ptr(VSBufferResource);

	


	DECLARE_Ptr(VSPostEffectSet);

	DECLARE_Proxy(VSTexAllState);
	DECLARE_Proxy(VSMaterial);
	DECLARE_Proxy(VSAnim);
	DECLARE_Proxy(VSFont);
	DECLARE_Proxy(VSStaticMeshNode);
	DECLARE_Proxy(VSSkeletonMeshNode);
	DECLARE_Proxy(VSPostEffectSet);
	DECLARE_Proxy(VSAnimTree);
	DECLARE_Proxy(VSActor);
	DECLARE_Proxy(VSSceneMap);


	DECLARE_Ptr(VSRenderTarget);
	DECLARE_Ptr(VSDepthStencil);
	DECLARE_Ptr(VSUnorderAccess);
	DECLARE_Ptr(VSBlendState);
	DECLARE_Ptr(VSDepthStencilState);
	DECLARE_Ptr(VSRasterizerState);
	DECLARE_Ptr(VSSamplerState);
	DECLARE_Ptr(VSDynamicBufferGeometry);
	DECLARE_Ptr(VSInstanceGeometry);
	class VSBlendDesc;
	class VSDepthStencilDesc;
	class VSRasterizerDesc;
	class VSSamplerDesc;
	class VSGCTask;
	struct DynamicBufferIndex
	{
		VSVertexFormatPtr pVertexFormat;
		unsigned int uiMeshDataType;
		DynamicBufferIndex()
		{
			pVertexFormat = NULL; 
			uiMeshDataType = 0;
		}	
		DynamicBufferIndex& operator= (const DynamicBufferIndex& DBI)
		{
			pVertexFormat = DBI.pVertexFormat;
			uiMeshDataType = DBI.uiMeshDataType;
			return *this;
		}
	};
	bool operator ==(const DynamicBufferIndex & DBI1, const DynamicBufferIndex & DBI2);
	bool operator >(const DynamicBufferIndex & DBI1, const DynamicBufferIndex & DBI2);
	bool operator <(const DynamicBufferIndex & DBI1, const DynamicBufferIndex & DBI2);


	typedef VSDelegateEvent<void(void)> DelayUpdateObjectOneFrame;
	typedef VSDelegateEvent<void(VSREAL)> DelayUpdateObject;
	typedef VSDelegateEvent<void(void)> DelayUpdateNoTimeObject;

	class VSGRAPHIC_API VSResourceManager 
	{
		//PRIORITY
		DECLARE_PRIORITY
	public:
		VSResourceManager();
		~VSResourceManager();
	
#define GET_INNER_RESOURCE_SET(ResourceName)\
		static VSResourceSet<unsigned int, VS##ResourceName##Ptr> & Get##ResourceName##Set()\
		{\
			static VSResourceSet<unsigned int, VS##ResourceName##Ptr> s_##ResourceName##Set; \
			return s_##ResourceName##Set; \
		}

		//内部管理resource
		GET_INNER_RESOURCE_SET(VertexFormat);
		GET_INNER_RESOURCE_SET(Name);
		GET_INNER_RESOURCE_SET(BlendState);
		GET_INNER_RESOURCE_SET(DepthStencilState);
		GET_INNER_RESOURCE_SET(RasterizerState);
		GET_INNER_RESOURCE_SET(SamplerState);

#define GET_SHADER_MAP_INNER(ShaderMapName)\
		static VSShaderMap & Get##ShaderMapName()\
		{\
			static VSShaderMap s_##ShaderMapName(_T(#ShaderMapName)); \
			return s_##ShaderMapName; \
		}

#define GET_SHADER_MAP(Name)\
		GET_SHADER_MAP_INNER(Name##VertexShaderMap);\
		GET_SHADER_MAP_INNER(Name##PixelShaderMap);\
		GET_SHADER_MAP_INNER(Name##GeometryShaderMap);\
		GET_SHADER_MAP_INNER(Name##HullShaderMap);\
		GET_SHADER_MAP_INNER(Name##DomainShaderMap);\
		GET_SHADER_MAP_INNER(Name##ComputeShaderMap);

		//for custom material
		GET_SHADER_MAP(Custom);

		//  one shader map to one material tree
		GET_SHADER_MAP(Material);

		GET_SHADER_MAP(Indirect);

		GET_SHADER_MAP(NormalDepth);

		GET_SHADER_MAP(CubShadow);

		GET_SHADER_MAP(VolumeShadow);

		GET_SHADER_MAP(Shadow);

		GET_SHADER_MAP(DualParaboloidShadow);

		GET_SHADER_MAP(PreZ);

		// one shader map all material tree
		GET_SHADER_MAP(Inner);


		static VSDVGeometry *GetDVGeometry(VSVertexFormat * pVertexFormat, unsigned int MeshDataType, unsigned int VertexNum);

		
		static void LoadDefaultDeviceResource(unsigned int RenderTypeAPI);
		static bool IsReleaseAll();
		static void CacheResource();
		static void GC();
		template <class T>
		static VSTexAllState * Create2DTexture(unsigned int uiWidth, unsigned int uiHeight,unsigned int uiFormatType,
			unsigned int uiMipLevel,T *pBuffer, bool bSRGB = false);

		template <class T>
		static VSTexAllState * Create1DTexture(unsigned int uiWidth,unsigned int uiFormatType,
			unsigned int uiMipLevel,T *pBuffer, bool bSRGB = false);

		template <class T>
		static VSTexAllState * CreateCubTexture(unsigned int uiWidth,unsigned int uiFormatType,
			unsigned int uiMipLevel,T *pBuffer, bool bSRGB = false);

		template <class T>
		static VSTexAllState * Create3DTexture(unsigned int uiWidth,unsigned int uiHeight, unsigned int uiLength,unsigned int uiFormatType,
			unsigned int uiMipLevel,T *pBuffer, bool bSRGB = false);

		static VSCaptureTexAllState * CreateCaptureTexture(const VSString & ViewFamilyName, unsigned int uiMipLevel = 1);

		enum CompressType
		{
			CT_NONE,
			CT_BC1,
			CT_BC2,
			CT_BC3,
			CT_BC4,
			CT_BC5,
			CT_MAX
		};
		static VSTexAllState * Load2DTexture(const TCHAR *pFileName,VSSamplerStatePtr pSamplerState,
			CompressType uiCompressType,bool bIsNormal,bool bSRGB,bool bMip);

		static VS2DTexture * CreateTextureCache(void* SourceData,unsigned int uiWidth, unsigned int uiHeight,
			unsigned int uiFormatType, bool bIsNormal, bool bSRGB, bool bMip);

		template<typename T>
		static bool SaveResouce(T * pResource, const TCHAR * PathName, bool bSaveResourcePath = false)
		{
			if (!pResource)
			{
				return false;
			}
			if (!pResource->IsCanSave())
			{
				return false;
			}
			VSStream SaveStream;
			SaveStream.SetStreamFlag(VSStream::AT_REGISTER);
			SaveStream.ArchiveAll(pResource);
			VSString ResourceName = PathName;
			ResourceName += T::ms_PointFileSuffix;
			VSString FileName = T::ms_ResourcePath + ResourceName;
			if (!TIsVSCacheResourceType<T>::Value)
			{
				VSResource * pTemp = (VSResource *)pResource;
				pTemp->SetResourceName(ResourceName);
			}
			return SaveStream.Save(bSaveResourcePath ? FileName.GetBuffer() : PathName);
		}
		template<typename T>
		static bool SaveResouce(VSPointer<T> & pResource, const TCHAR * PathName, bool bSaveResourcePath = false)
		{
			return SaveResouce(pResource.GetObject(), PathName, bSaveResourcePath);
		}

		static VSVertexFormat *LoadVertexFormat(VSVertexBuffer * pVertexBuffer,VSArray<VSVertexFormat::VERTEXFORMAT_TYPE> *pFormatArray = NULL);
		static VSName * CreateName(const TCHAR * pChar);
		static VSName * CreateName(const VSString & String);
		static VSVShader * CreateVShader(MaterialShaderPara &MSPara,unsigned int uiShaderID);
		static VSPShader * CreatePShader(MaterialShaderPara &MSPara,unsigned int uiShaderID);
		static VSGShader * CreateGShader(MaterialShaderPara &MSPara,unsigned int uiShaderID);
		static VSHShader * CreateHShader(MaterialShaderPara &MSPara, unsigned int uiShaderID);
		static VSDShader * CreateDShader(MaterialShaderPara &MSPara, unsigned int uiShaderID);

		static VSBlendState * CreateBlendState(const VSBlendDesc & BlendDesc);
		static VSDepthStencilState * CreateDepthStencilState(const VSDepthStencilDesc & DepthStencilDesc);
		static VSRasterizerState * CreateRasterizerState(const VSRasterizerDesc &RasterizerDesc);
		static VSSamplerState * CreateSamplerState(const VSSamplerDesc &SamplerDesc);

		template<typename T>
		static VSResourceProxy<T> * LoadASYNResource(const TCHAR *pFileName, bool IsAsyn)
		{
			if (!pFileName)
			{
				return NULL;
			}
			VSFileName FileName = pFileName;
			VSString Extension;
			VSUsedName ResourceName;
			if (FileName.GetExtension(Extension))
			{
				if (Extension != T::ms_FileSuffix)
				{
					return NULL;
				}
				else
				{
					ResourceName = FileName;
					FileName = T::ms_ResourcePath + FileName;

				}
			}
			else
			{
				ResourceName = FileName + T::ms_PointFileSuffix;
				FileName = T::ms_ResourcePath + FileName + T::ms_PointFileSuffix;

			}
			VSCriticalSection::Locker Temp(T::ms_LoadResourceCriticalSection);
			VSPointer<VSResourceProxy<T>> pResourceR = NULL;
			pResourceR = (VSResourceProxy<T> *)T::GetASYNResourceSet().CheckIsHaveTheResource(ResourceName);
			if (pResourceR)
			{
				return pResourceR;
			}
			pResourceR = VS_NEW VSResourceProxy<T>();

			pResourceR->SetResourceName(ResourceName);


			T::GetASYNResourceSet().AddResource(ResourceName, pResourceR);


			if (IsAsyn && VSConfig::ms_EnableAsynLoad && (pResourceR->GetResourceAbility() & RA_ASYN_LOAD))
			{
				VSMAC_ASSERT(VSASYNLoadManager::ms_pASYNLoadManager);
				VSASYNLoadManager::ms_pASYNLoadManager->AddResource(pResourceR, FileName);
			}
			else
			{
				T * pResource = LoadResource<T>(FileName.GetBuffer());
				if (pResource)
				{
					pResourceR->SetNewResource(pResource);
					if (pResourceR->GetResourceAbility() & RA_NEED_CACHE)
					{
						T::CacheType * pCacheResouce = LoadResource<T::CacheType>(pResource->GetCacheFilePath().GetBuffer());
						VSMAC_ASSERT(pCacheResouce);
						pCacheResouce->SetCacheResource(pResource);
						VSMAC_DELETE(pCacheResouce);
					}					
					pResourceR->Loaded();
				}
				else
				{
					VSMAC_ASSERT(0);

				}

			}
			return pResourceR;
		}

		

		static void DeleteMapResource(const TCHAR *  pFileName);
		static void DeleteAllMapResource();

		//RenderTarget
		static VSRenderTarget * CreateRenderTarget(unsigned int uiWidth, unsigned int uiHeight,unsigned int uiFormatType,
			unsigned int uiMulSample,bool CPURead = false);
		static VSRenderTarget * CreateRenderTarget(VS2DTexture * pCreateBy,unsigned int uiMulSample = 0
			,unsigned int uiLevel = 0, bool CPURead = false);
		static VSRenderTarget * CreateRenderTarget(VS2DTextureArray * pCreateBy, unsigned int uiMulSample = 0
			, unsigned int uiLevel = 0, unsigned int First = 0, unsigned int uiArraySize = 1, bool CPURead = false);
		static VSRenderTarget * CreateRenderTarget(VS3DTexture * pCreateBy, unsigned int uiMulSample = 0
			, unsigned int uiLevel = 0, unsigned int First = 0, unsigned int uiArraySize = 1, bool CPURead = false);

		struct TextureBindInfoType
		{
			unsigned int uiFirst;
			unsigned int uiArraySize;
		};
		//取得满足条件的，并且没有再使用的RT，为了节省显存空间，一般这种RT和texture绑定的，并且贴图miplevel都是1
		static VSRenderTarget * GetRenderTarget(unsigned int uiWidth, unsigned int uiHeight,
			unsigned int uiFormatType,unsigned int uiMulSample,bool TextureUse = true, bool CPURead = false);		
		static bool GetCubRenderTarget(unsigned int uiWidth, unsigned int uiFormatType,
			unsigned int uiMulSample,VSRenderTarget * OutRT[VSCubeTexture::F_MAX], bool CPURead = false);
		static bool Get2DTextureArrayRenderTarget(unsigned int uiWidth, unsigned int uiHeight,unsigned int uiFormatType,
			unsigned int uiMulSample, VSArray<TextureBindInfoType> & TextureBindInfo,VSArray<VSRenderTarget *> &RTArray, bool CPURead = false);
		static bool Get3DTextureRenderTarget(unsigned int uiWidth, unsigned int uiHeight, unsigned int uiFormatType,
			unsigned int uiMulSample, VSArray<TextureBindInfoType> & TextureBindInfo, VSArray<VSRenderTarget *> &RTArray, bool CPURead = false);

		

		// DepthStencil
		static VSDepthStencil * CreateDepthStencil(unsigned int uiWidth, unsigned int uiHeight,
			unsigned int uiMulSample, unsigned int uiFormatType, bool CPURead = false);
		static VSDepthStencil * CreateDepthStencil(VS2DTexture * pCreateBy, unsigned int uiMulSample = 0
			, unsigned int uiLevel = 0, bool CPURead = false);
		static VSDepthStencil * CreateDepthStencil(VS2DTextureArray * pCreateBy, unsigned int uiMulSample = 0
			, unsigned int uiLevel = 0, unsigned int First = 0, unsigned int uiArraySize = 1, bool CPURead = false);
		static VSDepthStencil * CreateDepthStencil(VS3DTexture * pCreateBy, unsigned int uiMulSample = 0
			, unsigned int uiLevel = 0, unsigned int First = 0, unsigned int uiArraySize = 1, bool CPURead = false);

		static VSDepthStencil * GetDepthStencil(unsigned int uiWidth,unsigned int uiHeight,
			unsigned int uiFormatType,unsigned int uiMulSample,bool TextureUse = false, bool CPURead = false);		
		static bool GetCubDepthStencil(unsigned int uiWidth, unsigned int uiFormatType,
			unsigned int uiMulSample, VSDepthStencil * OutDS[VSCubeTexture::F_MAX], bool CPURead = false);	
		static bool Get2DTextureArrayDepthStencil(unsigned int uiWidth, unsigned int uiHeight, unsigned int uiFormatType,
			unsigned int uiMulSample, VSArray<TextureBindInfoType> & TextureBindInfo, VSArray<VSDepthStencil *> &DSArray, bool CPURead = false);
		static bool Get3DTextureDepthStencil(unsigned int uiWidth, unsigned int uiHeight, unsigned int uiFormatType,
			unsigned int uiMulSample, VSArray<TextureBindInfoType> & TextureBindInfo, VSArray<VSDepthStencil *> &DSArray, bool CPURead = false);


		// UnorderAccess Buffer
		static VSUnorderAccess * CreateBufferUnorderAccess(unsigned int uiNum, unsigned int uiDT,
			unsigned int uiStructStride = 0, bool CPURead = false);
		static VSUnorderAccess * CreateBufferUnorderAccess(VSBufferResource * pCreateBy, bool CPURead = false);

		static VSUnorderAccess * GetBufferUnorderAccess(unsigned int uiNum, unsigned int uiDT,
			unsigned int uiStructStride = 0, bool BufferResource = true, bool CPURead = false);

		// UnorderAccess Texture
		static VSUnorderAccess * CreateTextureUnorderAccess(unsigned int uiWidth, unsigned int uiHeight,
			unsigned int uiFormatType, bool CPURead = false);
		static VSUnorderAccess * CreateTextureUnorderAccess(VS2DTexture * pCreateBy,
			unsigned int uiLevel = 0, bool CPURead = false);
		static VSUnorderAccess * CreateTextureUnorderAccess(VS2DTextureArray * pCreateBy
			, unsigned int uiLevel = 0, unsigned int First = 0, unsigned int uiArraySize = 1, bool CPURead = false);
		static VSUnorderAccess * CreateTextureUnorderAccess(VS3DTexture * pCreateBy
			, unsigned int uiLevel = 0, unsigned int First = 0, unsigned int uiArraySize = 1, bool CPURead = false);
		
		static VSUnorderAccess * GetTextureUnorderAccess(unsigned int uiWidth, unsigned int uiHeight,
			unsigned int uiFormatType, bool TextureUse = false, bool CPURead = false);		
		static bool GetCubeUnorderAccess(unsigned int uiWidth, unsigned int uiFormatType,
			 VSUnorderAccess * OutUA[VSCubeTexture::F_MAX], bool CPURead = false);
		static bool Get2DTextureArrayUnorderAccess(unsigned int uiWidth, unsigned int uiHeight, unsigned int uiFormatType,
			VSArray<TextureBindInfoType> & TextureBindInfo, VSArray<VSUnorderAccess *> &UAArray, bool CPURead = false);
		static bool Get3DTextureUnorderAccess(unsigned int uiWidth, unsigned int uiHeight, unsigned int uiFormatType,
			VSArray<TextureBindInfoType> & TextureBindInfo, VSArray<VSUnorderAccess *> &UAArray, bool CPURead = false);


		template <class T>
		static void DisableOutputResource(T * &pOutputResource)
		{
			ReleaseOutputResource(pOutputResource);
			if (pOutputResource->GetOutputType() == VSOutputResource::OT_BUFFER_UNORDER_ACCESS)
			{
				VSBufferResource * pBufferResource = (VSBufferResource *)pOutputResource->GetCreateBy();
				if (pBufferResource)
				{
					pBufferResource->ClearAllSlot();
				}
			}
			else 
			{
				VSTexture * pTexture = (VSTexture *)pOutputResource->GetCreateBy();
				if (pTexture)
				{
					pTexture->ClearAllSlot();
				}
			}
			
			pOutputResource = NULL;
		}
		template <class T>
		static void DisableOutputResource(VSPointer<T> &pOutputResource)
		{
			T * pTempOutputResource = pOutputResource;
			DisableOutputResource(pTempOutputResource);
			pOutputResource = NULL;
		}
		template <class T>
		static void DisableOutputReousrceArray(VSArray<T *> &OutputReousrceArray)
		{
#if _DEBUG 
			if (!CheckOutputResourceArrayIsValid(OutputReousrceArray))
			{
				VSMAC_ASSERT(0);
			}
#endif
			ReleaseOutputResourceArray(OutputReousrceArray);
			VSTexture * pTexture = (VSTexture *)OutputReousrceArray[0]->GetCreateBy();
			if (pTexture)
			{
				pTexture->ClearAllSlot();
			}
			for (unsigned int i = 0; i < RTArray.GetNum(); i++)
			{
				OutputReousrceArray[i] = NULL;
			}
		}
		template <class T>
		static void DisableOutputReousrceArray(VSArray<VSPointer<T>>& OutputReousrceArray)
		{
			VSArray<T *> OutputReousrceArrayTemp;
			for (unsigned int i = 0; i < OutputReousrceArray.GetNum(); i++)
			{
				OutputReousrceArrayTemp[i] = OutputReousrceArray[i];
			}
			DisableOutputReousrceArray(OutputReousrceArrayTemp);
			for (unsigned int i = 0; i < OutputReousrceArray.GetNum(); i++)
			{
				OutputReousrceArray[i] = NULL;
			}
		}

		//Source - Target
		static VSAnim * CreateAdditiveAnim(VSAnim * pSourceAnim, VSAnim * pTargetAnim, VSAnimR * pBlendAnimR);

		static VSAnim * CreateAdditiveAnim(VSAnim * pSourceAnim, VSAnim * pTargetAnim, VSAnimR * pBlendAnimR, VSREAL fTargetTime = 0.0f);

		static VSAnim * CreateAdditiveAnim(VSAnim * pSourceAnim, VSAnim * pTargetAnim, VSAnimR * pBlendAnimR, VSREAL fSourceTime = 0.0f, VSREAL fTargetTime = 0.0f);

		static VSAnim * CreateAnim(VSAnim * pSourceAnim, VSREAL fSourceTime = 0.0f);

		static class VSInstanceGeometry *  GetDynamicInstanceGeometry(VSMeshData * pSourceMeshData, VSMaterialR * pMaterial);
		DECLARE_INITIAL_ONLY
	private:
		static void InitCacheShader(unsigned int RenderTypeAPI);
		static bool InitialDefaultState();
		static bool TerminalDefaultState();

		static bool CacheShader();


		static bool CheckTextureBindInfo(VSArray<TextureBindInfoType> & TextureBindInfo);
		static bool CheckTextureBindInfo(VSArray<TextureBindInfoType> & TextureBindInfo, VSTextureOutputInfo * pTextureOutputInfo);

		template <class T>
		static void ReleaseOutputResource(T * pOutputResource)
		{
			if (pOutputResource)
			{
				VSMAC_ASSERT(pOutputResource->m_bUsed == true);
				pOutputResource->m_bUsed = false;
			}
		}

		template <class T>
		static bool CheckOutputResourceArrayIsValid(VSArray<T *> &OutputResourceArray)
		{
			if (OutputResourceArray.GetNum() == 0 || OutputResourceArray[0])
			{
				return false;
			}
			VSTexture* pCreatBy = (VSTexture *)OutputResourceArray[0]->GetCreateBy();
			if (!pCreatBy)
			{
				return false;
			}
			unsigned int uiMaxRT = OutputResourceArray[0]->GetFirst() + OutputResourceArray[0]->GetArraySize();
			unsigned int uiArrayNum = 0;
			unsigned int uiUsedNum = 0;
			for (unsigned int i = 0; i < OutputResourceArray.GetNum(); i++)
			{
				if (!OutputResourceArray[i])
				{
					return false;
				}
				if (pCreatBy != OutputResourceArray[i]->GetCreateBy())
				{
					return false;
				}
				if (OutputResourceArray[i]->m_bUsed)
				{
					uiUsedNum++;
				}
				unsigned int ArrayNumIndex = OutputResourceArray[i]->GetArraySize();
				unsigned int MaxRTIndex = OutputResourceArray[i]->GetFirst() + ArrayNumIndex;
				uiArrayNum += ArrayNumIndex;
				if (MaxRTIndex > uiMaxRT)
				{
					uiMaxRT = MaxRTIndex;
				}
			}
			if (uiMaxRT != uiArrayNum || uiUsedNum != OutputResourceArray.GetNum())
			{
				return false;
			}
			if (pCreatBy->GetTexType() == VSTexture::TT_2DARRAY)
			{
				if (uiArrayNum != pCreatBy->GetArraySize())
				{
					return false;
				}
			}
			else if (pCreatBy->GetTexType() == VSTexture::TT_3D)
			{
				if (uiArrayNum != pCreatBy->GetLength(0))
				{
					return false;
				}
			}
			return true;
		}


		template <class T>
		static void ReleaseOutputResourceArray(VSArray<T*> &OutputResourceArray)
		{
			for (unsigned int i = 0; i < OutputResourceArray.GetNum(); i++)
			{
				OutputResourceArray[i]->m_bUsed = false;
			}
		}

		static VSRenderTarget * GetRenderTargetTexture(unsigned int uiWidth, unsigned int uiHeight,
			unsigned int uiFormatType, unsigned int uiMulSample, bool CPURead = false);
		static VSRenderTarget * GetRenderTargetNoTexture(unsigned int uiWidth, unsigned int uiHeight,
			unsigned int uiFormatType, unsigned int uiMulSample, bool CPURead = false);

		static VSDepthStencil * GetDepthStencilNoTexture(unsigned int uiWidth, unsigned int uiHeight,
			unsigned int uiFormatType, unsigned int uiMulSample, bool CPURead = false);
		static VSDepthStencil * GetDepthStencilTexture(unsigned int uiWidth, unsigned int uiHeight,
			unsigned int uiFormatType, unsigned int uiMulSample, bool CPURead = false);

		static VSUnorderAccess * GetUnorderAccessNoBuffer(unsigned int uiNum, unsigned int uiDT,
			unsigned int uiStructStride = 0, bool CPURead = false);
		static VSUnorderAccess * GetUnorderAccessBuffer(unsigned int uiNum, unsigned int uiDT,
			unsigned int uiStructStride = 0, bool CPURead = false);

		static VSUnorderAccess * GetUnorderAccessNoTexture(unsigned int uiWidth, unsigned int uiHeight,
			unsigned int uiFormatType, bool CPURead = false);
		static VSUnorderAccess * GetUnorderAccessTexture(unsigned int uiWidth, unsigned int uiHeight,
			unsigned int uiFormatType, bool CPURead = false);

		

		static VSResourceArrayControl<VSInstanceGeometryPtr> &GetDynamicInstanceGeometryArray()
		{
			static VSResourceArrayControl<VSInstanceGeometryPtr> s_DynamicInstanceGeometryArray;
			return s_DynamicInstanceGeometryArray;
		}
		static VSResourceSet<DynamicBufferIndex, VSDVGeometryPtr> &GetDVGeometryArray()
		{
			static VSResourceSet<DynamicBufferIndex, VSDVGeometryPtr> s_DVGeometryArray;
			return s_DVGeometryArray;
		}
		static VSArray<VSDynamicBufferGeometryPtr> ms_SaveDelete;

		static VSResourceArrayControl<VSRenderTargetPtr> &GetRenderTargetArray()
		{
			static VSResourceArrayControl<VSRenderTargetPtr> s_RenderTargetArray;
			return s_RenderTargetArray;
		}
		static VSResourceArrayControl<VSDepthStencilPtr> &GetDepthStencilArray()
		{
			static VSResourceArrayControl<VSDepthStencilPtr> s_RenderTargetArray;
			return s_RenderTargetArray;
		}
		static VSResourceArrayControl<VSUnorderAccessPtr> & GetUnorderAccessArray()
		{
			static VSResourceArrayControl<VSUnorderAccessPtr> s_UnorderAccessArray;
			return s_UnorderAccessArray;
		}
		static VSResourceArrayControl<VSRenderTargetPtr> &GetRenderTargetBufferArray()
		{
			static VSResourceArrayControl<VSRenderTargetPtr> s_RenderTargetBufferArray;
			return s_RenderTargetBufferArray;
		}
		static VSResourceArrayControl<VSDepthStencilPtr> &GetDepthStencilBufferArray()
		{
			static VSResourceArrayControl<VSDepthStencilPtr> s_RenderTargetBufferArray;
			return s_RenderTargetBufferArray;
		}
		static VSResourceArrayControl<VSUnorderAccessPtr> & GetUnorderAccessBufferArray()
		{
			static VSResourceArrayControl<VSUnorderAccessPtr> s_UnorderAccessBufferArray;
			return s_UnorderAccessBufferArray;
		}
	protected:
		static VSCriticalSection ms_NameCri;
		static VSResourceProxyBase * LoadResource(const TCHAR *pFileName, bool IsAsyn);
		template<typename T>
		static T * LoadResource(const TCHAR *pFileName)
		{
			if (!pFileName)
			{

				return NULL;
			}
			VSStream LoadStream;
			LoadStream.Load(pFileName);
			T * pResource = (T *)LoadStream.GetObjectByRtti(T::ms_Type);
			if (!pResource)
			{
				return NULL;
			}
			return pResource;
		}
	public:
		//help function
		template <class T>
		static bool GetNextMipData(const T * pInData,unsigned int uiInDataWidth,unsigned int uiInDataHeight,
			T * pOutData,unsigned int uiChannel);
		template <class T>
		static bool GetNextMipData(const T * pInData,unsigned int uiInDataWidth,unsigned int uiInDataHeight,unsigned int uiInDataLenght,
			T * pOutData,unsigned int uiChannel);
	public:
		static unsigned int GetGpuSkinBoneNum()
		{
			return ms_uiGpuSkinBoneNum;
		}
		static unsigned int GetGpuMorphTargetNum()
		{
			return ms_uiGpuMorphTargetNum;
		}
		static unsigned int GetGpuMorphTextureSize()
		{
			return ms_uiGpuMorphTextureSize;
		}
		static unsigned int GetSimpleInstanceAnimFloat4Num()
		{
			return ms_uiSimpleInstanceAnimFloat4Num;
		}
		static unsigned int GetLerpSimpleInstanceAnim()
		{
			return ms_bLerpSimpleInstanceAnim;
		}
		static bool DrawSkeleton()
		{
			return ms_bDrawSkeleton;
		}
		static bool GetActiveDebugDraw()
		{
			return ms_bActiveDebugDraw;
		}

		static bool IsCacheShader()
		{
			return ms_bIsCacheShader;
		}


		static bool ms_bRenderThread;
		static bool ms_bUpdateThread;

		static unsigned int ms_CurRenderAPIType;
		
	protected:
		static unsigned int ms_uiGpuSkinBoneNum;
		static bool ms_bDrawSkeleton;
		static bool ms_bActiveDebugDraw;
		static unsigned int ms_uiGpuMorphTargetNum;
		static unsigned int ms_uiGpuMorphTextureSize;
		static unsigned int ms_uiSimpleInstanceAnimFloat4Num;
		static bool ms_bLerpSimpleInstanceAnim;
		static bool ms_bIsCacheShader;


		static VSArray<VSObject *> ms_pRootObject;
		static VSArrayOrder<VSObject *> ms_pGCObject;
	public:
		static DelayUpdateObjectOneFrame & GetDelayUpdateObjectOneFrame()
		{
			static DelayUpdateObjectOneFrame s_DelayUpdateObjectOneFrame;
			return s_DelayUpdateObjectOneFrame;
		}
		static DelayUpdateObject & GetDelayUpdateObject()
		{
			static DelayUpdateObject s_DelayUpdateObject;
			return s_DelayUpdateObject;
		}
		static DelayUpdateNoTimeObject & GetDelayUpdateNoTimeObject()
		{
			static DelayUpdateNoTimeObject s_DelayUpdateNoTimeObject;
			return s_DelayUpdateNoTimeObject;
		}
		static void DelayUpdate(VSREAL fTime)
		{
			GetDelayUpdateObjectOneFrame()();
			GetDelayUpdateObjectOneFrame().Reset();
			GetDelayUpdateObject()(fTime);
			GetDelayUpdateNoTimeObject()();
			ms_SaveDelete.Clear();
		}
		/*
		GC规则：
		默认从VS_NEW创建的OBJECT 不参与GC，所以不会被GC掉，要自己管理。
		只有从Object GetInstance 创建出来的才参与GC，如果想要不参与GC请调用AddRootObject。
		把指针加入属性表，保证要参与GC的Object可以访问的到，否则就会被GC掉。

		*/
		static void AddRootObject(VSObject * p);
		static void DeleteRootObject(VSObject * p);
		static void AddGCObject(VSObject * p);
		static void GCObject();
		
		protected:
			friend class VSStream;
			friend class VSAsynStream;
			friend class VSWorld;
			static VSGCTask * ms_pCurGCTask;
			static VSGCTask * ms_pEndGCTask;
			static void RunGCTask();
			static void AddCanGCObject(VSArray<VSObject *>& CanGCObject);
		public:
			static void RunAllGCTask();

	};
	template <class T>
	bool VSResourceManager::GetNextMipData(const T * pInData,unsigned int uiInDataWidth,unsigned int uiInDataHeight,
		T * pOutData , unsigned int uiChannel)
	{
		VSMAC_ASSERT(pInData && pOutData);
		if (!pInData || !pOutData)
		{
			return false;
		}

		unsigned int uiOutDataWidth = uiInDataWidth >> 1;
		if (!uiOutDataWidth)
		{
			uiOutDataWidth = 1;
		}
		unsigned int uiOutDataHeight = uiInDataHeight >> 1;
		if (!uiOutDataHeight)
		{
			uiOutDataHeight = 1;
		}
		for (unsigned int i = 0 ;i < uiOutDataWidth ; i++)
		{
			unsigned int uiRow0 = i * 2;
			unsigned int uiRow1 = i * 2 + 1;

			if (uiRow1 >= uiInDataWidth)
			{
				uiRow1 = uiInDataWidth - 1;
			}
			for (unsigned int j = 0 ; j < uiOutDataHeight ; j++)
			{

				unsigned int uiCol0 = j * 2;
				unsigned int uiCol1 = j * 2 + 1;
				if (uiCol1 >= uiInDataHeight)
				{
					uiCol1 = uiInDataHeight - 1;
				}

				for (unsigned int k = 0 ; k < uiChannel ; k++)
				{
					pOutData[(j + i * uiOutDataHeight) * uiChannel + k] 
					= (T)((pInData[(uiCol0 + uiRow0 * uiInDataHeight) * uiChannel + k] + pInData[(uiCol1 + uiRow0 * uiInDataHeight) * uiChannel + k]
					+ pInData[(uiCol0 + uiRow1 * uiInDataHeight) * uiChannel + k] + pInData[(uiCol1 + uiRow1 * uiInDataHeight) * uiChannel + k]) * 0.25f);
				}

			}
		}
		return true;
	}
	template <class T>
	bool VSResourceManager::GetNextMipData(const T * pInData,unsigned int uiInDataWidth,unsigned int uiInDataHeight,unsigned int uiInDataLength,
		T * pOutData , unsigned int uiChannel)
	{
		VSMAC_ASSERT(pInData && pOutData);
		if (!pInData || !pOutData)
		{
			return false;
		}

		unsigned int uiOutDataWidth = uiInDataWidth >> 1;
		if (!uiOutDataWidth)
		{
			uiOutDataWidth = 1;
		}
		unsigned int uiOutDataHeight = uiInDataHeight >> 1;
		if (!uiOutDataHeight)
		{
			uiOutDataHeight = 1;
		}
		unsigned int uiOutDataLength = uiInDataLength >> 1;
		if (!uiOutDataLength)
		{
			uiOutDataLength = 1;
		}
		for (unsigned int s = 0 ; s < uiOutDataLength ; s++)
		{
			unsigned int uiL0 = s * 2;
			unsigned int uiL1 = s * 2 + 1;

			if (uiL1 >= uiOutDataLength)
			{
				uiL1 = uiOutDataLength - 1;
			}

			for (unsigned int i = 0 ;i < uiOutDataWidth ; i++)
			{
				unsigned int uiRow0 = i * 2;
				unsigned int uiRow1 = i * 2 + 1;

				if (uiRow1 >= uiInDataWidth)
				{
					uiRow1 = uiInDataWidth - 1;
				}
				for (unsigned int j = 0 ; j < uiOutDataHeight ; j++)
				{

					unsigned int uiCol0 = j * 2;
					unsigned int uiCol1 = j * 2 + 1;
					if (uiCol1 >= uiInDataHeight)
					{
						uiCol1 = uiInDataHeight - 1;
					}

					for (unsigned int k = 0 ; k < uiChannel ; k++)
					{
						pOutData[(j + i * uiOutDataHeight + s * uiOutDataHeight * uiOutDataWidth) * uiChannel + k] 
						= (T)((pInData[(uiCol0 + uiRow0 * uiInDataHeight + uiL0 * uiInDataHeight * uiInDataWidth) * uiChannel + k] + 
							pInData[(uiCol1 + uiRow0 * uiInDataHeight + uiL0 * uiInDataHeight * uiInDataWidth) * uiChannel + k] + 
							pInData[(uiCol0 + uiRow1 * uiInDataHeight + uiL0 * uiInDataHeight * uiInDataWidth) * uiChannel + k] + 
							pInData[(uiCol1 + uiRow1 * uiInDataHeight + uiL0 * uiInDataHeight * uiInDataWidth) * uiChannel + k] +
							pInData[(uiCol0 + uiRow0 * uiInDataHeight + uiL1 * uiInDataHeight * uiInDataWidth) * uiChannel + k] + 
							pInData[(uiCol1 + uiRow0 * uiInDataHeight + uiL1 * uiInDataHeight * uiInDataWidth) * uiChannel + k] + 
							pInData[(uiCol0 + uiRow1 * uiInDataHeight + uiL1 * uiInDataHeight * uiInDataWidth) * uiChannel + k] + 
							pInData[(uiCol1 + uiRow1 * uiInDataHeight + uiL1 * uiInDataHeight * uiInDataWidth) * uiChannel + k]
							) * 0.125f);
					}

				}
			}
		}
		
		return true;
	}
	template <class T>
	VSTexAllState * VSResourceManager::Create2DTexture(unsigned int uiWidth, unsigned int uiHeight,unsigned int uiFormatType,
		unsigned int uiMipLevel,T *pBuffer,bool bSRGB)
	{
		if (!uiWidth || !uiHeight)
		{
			return NULL;
		}
		if ((!IsTwoPower(uiWidth) || !IsTwoPower(uiHeight)) && !VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_NoPow2Texture))
		{
			VSMAC_ASSERT(0);
			return NULL;
		}
		VSTexAllState * pTexAllState = VS_NEW VSTexAllState();
		VS2DTexture * pTexture = VS_NEW VS2DTexture(uiWidth,uiHeight,uiFormatType,uiMipLevel,1, bSRGB);
		if(!pTexture)
		{
			return NULL;
		}
		if (pBuffer)
		{
			pTexture->CreateRAMData();		
			T *pDestBuffer = (T *)pTexture->GetBuffer(0);
			VSMemcpy(pDestBuffer,pBuffer,pTexture->GetByteSize(0));

			T * pLast = (T *)pTexture->GetBuffer(0);
			for (unsigned int i = 1 ; i < pTexture->GetMipLevel() ; i++)
			{
				T *pNow = (T *)pTexture->GetBuffer(i);

				if(!VSResourceManager::GetNextMipData(pLast,pTexture->GetWidth(i - 1),pTexture->GetHeight(i - 1),pNow,pTexture->GetChannelPerPixel()))
				{
					VSMAC_DELETE(pTexture);
					return NULL;
				}
				pLast = (T *)pTexture->GetBuffer(i);
			}
		}
		
		pTexAllState->SetTexture(pTexture);

		return pTexAllState;
	}
	
	template <class T>
	VSTexAllState * VSResourceManager::Create1DTexture(unsigned int uiWidth,unsigned int uiFormatType,
		unsigned int uiMipLevel,T *pBuffer, bool bSRGB)
	{
		if (!uiWidth)
		{
			return NULL;
		}
		if (!IsTwoPower(uiWidth) && !VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_NoPow2Texture))
		{
			VSMAC_ASSERT(0);
			return NULL;
		}
		VSTexAllState * pTexAllState = VS_NEW VSTexAllState();
		VS1DTexture * pTexture = VS_NEW VS1DTexture(uiWidth,uiFormatType,uiMipLevel,1, bSRGB);
		if(!pTexture)
		{
			return NULL;
		}
		if (pBuffer)
		{
			pTexture->CreateRAMData();
		
			T *pDestBuffer = (T *)pTexture->GetBuffer(0);
			VSMemcpy(pDestBuffer,pBuffer,pTexture->GetByteSize(0));

			T * pLast = (T *)pTexture->GetBuffer(0);
			for (unsigned int i = 1 ; i < pTexture->GetMipLevel() ; i++)
			{
				T *pNow = (T *)pTexture->GetBuffer(i);

				if(!VSResourceManager::GetNextMipData(pLast,pTexture->GetWidth(i - 1),pTexture->GetHeight(i - 1),pNow,pTexture->GetChannelPerPixel()))
				{
					VSMAC_DELETE(pTexture);
					return NULL;
				}
				pLast = (T *)pTexture->GetBuffer(i);
			}
		}

		pTexAllState->SetTexture(pTexture);

		return pTexAllState;
	}
	template <class T>
	VSTexAllState * VSResourceManager::CreateCubTexture(unsigned int uiWidth,unsigned int uiFormatType,
		unsigned int uiMipLevel,T *pBuffer, bool bSRGB)
	{
		if (!uiWidth)
		{
			return NULL;
		}
		if (!IsTwoPower(uiWidth))
		{
			VSMAC_ASSERT(0);
			return NULL;
		}
		VSTexAllState * pTexAllState = VS_NEW VSTexAllState();
		VSCubeTexture * pTexture = VS_NEW VSCubeTexture(uiWidth,uiFormatType,uiMipLevel,1, bSRGB);
		if(!pTexture)
		{
			return NULL;
		}
		if (pBuffer)
		{
			pTexture->CreateRAMData();
		
			T *pDestBuffer = (T *)pTexture->GetBuffer(0);
			VSMemcpy(pDestBuffer,pBuffer,pTexture->GetByteSize(0));

			for (unsigned int s = 0 ; s < VSCubeTexture::F_MAX ; s++)
			{
				T * pLast = (T *)pTexture->GetBuffer(0,s);
				for (unsigned int i = 1 ; i < pTexture->GetMipLevel() ; i++)
				{
					T *pNow = (T *)pTexture->GetBuffer(i,s);

					if(!VSResourceManager::GetNextMipData(pLast,pTexture->GetWidth(i - 1),pTexture->GetHeight(i - 1),pNow,pTexture->GetChannelPerPixel()))
					{
						VSMAC_DELETE(pTexture);
						return NULL;
					}
					pLast = (T *)pTexture->GetBuffer(i,s);
				}
			}
		}

		pTexAllState->SetTexture(pTexture);

		return pTexAllState;
	}
	template <class T>
	VSTexAllState * VSResourceManager::Create3DTexture(unsigned int uiWidth,unsigned int uiHeight, unsigned int uiLength,unsigned int uiFormatType,
		unsigned int uiMipLevel,T *pBuffer, bool bSRGB)
	{
		if (!uiWidth || !uiHeight || !uiLength)
		{
			return NULL;
		}
		if (!IsTwoPower(uiWidth) || !IsTwoPower(uiHeight) || !IsTwoPower(uiLength))
		{
			VSMAC_ASSERT(0);
			return NULL;
		}
		VSTexAllState * pTexAllState = VS_NEW VSTexAllState();
		VS3DTexture * pTexture = VS_NEW VS3DTexture(uiWidth,uiHeight,uiLength,uiFormatType,uiMipLevel,1, bSRGB);
		if(!pTexture)
		{
			return NULL;
		}
		if (pBuffer)
		{
			pTexture->CreateRAMData();
		
			T *pDestBuffer = (T *)pTexture->GetBuffer(0);
			VSMemcpy(pDestBuffer,pBuffer,pTexture->GetByteSize(0));

			T * pLast = (T *)pTexture->GetBuffer(0);
			for (unsigned int i = 1 ; i < pTexture->GetMipLevel() ; i++)
			{
				T *pNow = (T *)pTexture->GetBuffer(i);

				if(!VSResourceManager::GetNextMipData(pLast,pTexture->GetWidth(i - 1),pTexture->GetHeight(i - 1),pTexture->GetLength(i - 1),pNow,pTexture->GetChannelPerPixel()))
				{
					VSMAC_DELETE(pTexture);
					return NULL;
				}
				pLast = (T *)pTexture->GetBuffer(i);
			}
		}

		pTexAllState->SetTexture(pTexture);

		return pTexAllState;
	}
}
