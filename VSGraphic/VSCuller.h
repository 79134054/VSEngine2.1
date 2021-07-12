#pragma once
#include "VSPlane3.h"
#include "VSObject.h"
#include "VSSphere3.h"
#include "VSStack.h"
namespace VSEngine2
{
	/*
		列表里存储所有可见节点，每个节点存储自己叶子对应的叶子几点在可以见列表的下标，现在去掉了这个功能，、
		列表里面只存储叶子节点。
	*/
	class VSSpatial;
	class VSMaterialInstance;
	class VSMaterial;
	class VSMeshNode;
	class VSGeometry;
	class VSCamera;
	class VSLight;
	class VSRenderContext
	{
	public:
		VSRenderContext()
		{
			m_pGeometry = NULL;
			m_pMaterialInstance = NULL;
			m_uiPassId = 0;
			m_pMaterial = NULL;
			m_pMeshNode = NULL;
			m_uiInstanceID = -1;
			bOcclusion = false;
		}
		~VSRenderContext()
		{

		}
		static VSRenderContext ms_RenderContextNULL;
		VSGeometry * m_pGeometry;
		VSMaterialInstance * m_pMaterialInstance;
		VSMaterial * m_pMaterial;
		unsigned int m_uiPassId;
		VSMeshNode * m_pMeshNode;
		VSArray<VSLight *> m_pIndirectLight;
		VSArray<VSLight *> m_pDirectLight;
		int m_uiInstanceID;
		bool bOcclusion;
		inline bool IsInstanceData()const
		{
			return m_pMeshNode == NULL && m_pMaterialInstance == NULL;
		}
		bool IsCanDraw()const;
		
	};
	class VSGRAPHIC_API VSCuller : public VSMemObject
	{
	public:
		enum //CULLER TYPE
		{
			CUT_MAIN,
			CUT_SHADOW,
			CUT_MAX
		};
		VSCuller();
		virtual ~VSCuller();
		enum
		{
			VS_MAX_PLANE_NUM = 32
		};
		enum //VisibleSet Type
		{
			VST_BASE,
			VST_ALPHABLEND,	
			VST_COMBINE,
			VST_MAX
		};
		enum	//Render Group
		{
			RG_BACK,
			RG_NORMAL,
			RG_FRONT,		
			RG_MAX
		};
		enum	// VisibleSet Flag
		{
			VSF_NONE,
			VSF_ALL,
			VSF_PARTIAL,
			VSF_MAX
			
		};
		bool PushPlane(const VSPlane3 & Plane);
		bool PushCameraPlane(VSCamera &Camera);
		inline void SetCamera(VSCamera &Camera);
		bool PopPlane(VSPlane3 &Plane);
		inline void ClearAllPlane();

		bool InsertObject(VSRenderContext &VisibleContext,unsigned int uiVisibleSetType = VST_BASE,unsigned int uiRenderGroup = RG_NORMAL);
		inline unsigned int GetPlaneNum()const;
		inline void SetPlaneState(unsigned int uiPlaneState);
		inline unsigned int GetPlaneState()const;
		inline unsigned int GetVisibleNum(unsigned int uiVisibleSetType = VST_BASE,unsigned int uiRenderGroup = RG_NORMAL)const;
		inline unsigned int GetRenderGroupVisibleNum(unsigned int uiRenderGroup = RG_NORMAL)const;
		inline unsigned int GetAllVisibleNum()const;
		inline void ClearVisibleSet();
		inline void ClearPlaneState();

		inline unsigned int GetLightNum()const;
		inline VSLight * GetLight(unsigned int i)const;
		inline void ClearLight();
		void InsertLight(VSLight * pLight);
		bool HasLight(VSLight * pLight);
		virtual unsigned int IsVisible(const VSAABB3 &BV,bool bClearState = false);
		virtual unsigned int IsVisible(const VSSphere3 &S,bool bClearState = false);
		virtual unsigned int IsVisible(const VSVector3 & Point,bool bClearState = false);
		inline void SetUseMaterialIndex(int iUseMaterialIndex)
		{
			m_iUseMaterialIndex = iUseMaterialIndex;
		}

		inline int GetUseMaterialIndex()const
		{
			return m_iUseMaterialIndex;
		}
		virtual unsigned int GetCullerType()const
		{
			return CUT_MAIN;
		}
		virtual bool ForceNoCull(const VSSpatial * pSpatial);
		virtual bool CullConditionNode(const VSMeshNode * pMeshNode);
		inline VSRenderContext& GetVisibleSpatial(unsigned int i,unsigned int uiVisibleSetType = VST_BASE,unsigned int uiRenderGroup = RG_NORMAL)const;
		inline VSCamera * GetCamera()const;
		class RenderPriority
		{
		public:
			RenderPriority(){}
			~RenderPriority(){}
			bool operator()(VSRenderContext & p1,VSRenderContext & p2);


		};
		class AlphaPriority
		{
		public:
			AlphaPriority(VSCamera * pCamera){m_pCamera = pCamera;}
			~AlphaPriority(){}
			bool operator()(VSRenderContext & p1,VSRenderContext & p2);
		protected:
			VSCamera *m_pCamera;
		};
		friend class VSShadowCuller;
		friend class VSVolumeShadowMapCuller;
		virtual void Sort();
		virtual void CollectDynamicInstance();
		virtual bool CullGeometry(VSGeometry * pGeometry);

		void GetAllVisibleAABB(unsigned int uiVisibleSetType, unsigned int uiRenderGroup, VSArray<VSAABB3> & AABBArray);

		void GetAndSortLight();
		inline  void SetBeignDynamicInstanceNum(unsigned int uiBeignDynamicInstanceNum)
		{
			m_uiBeignDynamicInstanceNum = uiBeignDynamicInstanceNum;
		}
		inline void ClearAll()
		{
			ClearPlaneState();
			ClearAllPlane();
			ClearVisibleSet();
			ClearLight();
		}
		void GetSceneContent(VSCamera & pCamera, VSArray<class VSScene *> & pSceneArray, double dAppTime,bool bSort = true);
	protected:
		void GetSceneGeometryContent(VSCamera & pCamera, VSArray<class VSScene *> & pSceneArray, double dAppTime,bool bSort);
		void GetSceneLightContent(VSCamera & pCamera, VSArray<class VSScene *> & pSceneArray, double dAppTime);
		virtual void CollectDynamicInstance(VSArray<VSRenderContext> & inContext);
		unsigned int m_uiPlaneNum;
		VSPlane3	m_Plane[VS_MAX_PLANE_NUM];
		unsigned int m_uiPlaneState;
		VSArray<VSRenderContext> m_VisibleSet[RG_MAX][VST_MAX];
		VSArray<VSLight *> m_LightSet;
		VSCamera *m_pCamera;
		int m_iUseMaterialIndex;	
		unsigned int m_uiBeignDynamicInstanceNum;
	};
#include "VSCuller.inl"
	class VSLocalLight;
	class VSGRAPHIC_API VSShadowCuller : public VSCuller
	{
	public:
		// use light Camera
		VSShadowCuller();
		virtual ~VSShadowCuller();
		virtual unsigned int GetCullerType()const
		{
			return CUT_SHADOW;
		}
		virtual bool CullConditionNode(const VSMeshNode * pMeshNode);
		virtual void Sort();
		class RenderPriority
		{
		public:
			RenderPriority(){}
			~RenderPriority(){}
			bool operator()(VSRenderContext & p1, VSRenderContext & p2);
		};
		bool TestObjectWithCameraIntersectOnLightDirPlane(const VSSpatial * pSpatial);
		void GetSceneContent(VSCamera & pCamera, VSArray<class VSScene *> & pSceneArray, VSLocalLight * pLocalLight,double dAppTime,bool bCollectDynamicInstance = true);
	protected:
		VSLocalLight * m_pLocalLight;
	};
	class VSGRAPHIC_API VSVolumeShadowMapCuller : public VSShadowCuller
	{
	public://use Culler Camera to Cull
		VSVolumeShadowMapCuller();
		virtual ~VSVolumeShadowMapCuller();
		virtual bool ForceNoCull(const VSSpatial * pSpatial);	
		virtual bool CullGeometry(VSGeometry * pGeometry);
	};

	class VSGRAPHIC_API VSDirShadowMapCuller : public VSShadowCuller
	{
	public:

		VSDirShadowMapCuller();
		virtual ~VSDirShadowMapCuller();
		virtual bool ForceNoCull(const VSSpatial * pSpatial);
	
	};
	class VSOcclusionQueryType
	{
	public:
		VSOcclusionQueryType()
		{
			pQuery = NULL;
		}
		~VSOcclusionQueryType()
		{
			pQuery = NULL;
		}
		bool GetQueryData(void * pData, unsigned int uiDataSize);
		class VSQuery * pQuery;
		friend class VSOcclusionQueryPool;
	private:
		int ID;
	};
	class VSGeometryOcclusionQueryData
	{
	public:
		VSGeometryOcclusionQueryData()
		{
			pGeometry = NULL;
			uiPixedNum = VSMAX_UINT32;
			bVisible = true;
			pCuller = NULL;
		}
		VSGeometryOcclusionQueryData(VSGeometry * pInGeometry,VSCuller * pInCuller)
		{
			pGeometry = pInGeometry;
			uiPixedNum = VSMAX_UINT32;
			bVisible = true;
			pCuller = pInCuller;
		}
		void GetOcclusionQuery(const VSOcclusionQueryType& InOcclusionQueryType)
		{
			OcclusionQueryType = InOcclusionQueryType;
		}
		bool operator > (const VSGeometryOcclusionQueryData& e)const
		{
			return pGeometry < e.pGeometry;
		}
		bool operator < (const VSGeometryOcclusionQueryData& e)const
		{
			return pGeometry > e.pGeometry;
		}
		bool operator == (const VSGeometryOcclusionQueryData& e)const
		{
			return pGeometry == e.pGeometry;
		}
		void ComputeVisible();
		VSGeometry * pGeometry;
		VSCuller * pCuller;
		VSOcclusionQueryType OcclusionQueryType;
		unsigned int uiPixedNum;
		bool bVisible;	
	};
	inline unsigned int GetTypeHash(const VSGeometryOcclusionQueryData & Element)
	{
		return GetTypeHash(Element.pGeometry);
	}
	class VSOcclusionQueryPool
	{
	public:
		VSOcclusionQueryPool();
		~VSOcclusionQueryPool();
		VSOcclusionQueryType GetOcclusionQuery();
		void ReleaseAllOcclusionQuery();
		void DeleteAllReleaseOcclusionQuery();
	private:
		VSArray<VSOcclusionQueryType> m_ElementArray;
		VSStack<unsigned int> m_FreeElement;
	};
	class VSGRAPHIC_API VSCullerManager
	{
	public: 
		VSCullerManager();
		~VSCullerManager();
		static VSCullerManager & GetCullerManager()
		{
			static VSCullerManager s_CullerManager;
			return s_CullerManager;
		}
		bool ProcessOcclusionCull(VSCuller * pCuller);

		void FillGeometryOcclusionQueryData();
		friend class VSOcclusionQuerySceneRender;
		void Exchange();
		void RegisterCuller(VSCuller * pCuller);
		void UnRegisterCuller(VSCuller * pCuller);
		bool IsRegister(VSCuller * pCuller);
		void ClearAllOcclusionQuery();
	protected:
		VSArray<VSGeometryOcclusionQueryData> & GetGroupGeometryOcclusionQueryData(VSCuller * pCuller, unsigned int uiGroup);
		
	
		VSArray<VSCuller *> m_CullerArray;
		VSArray<VSArrayOrder<VSGeometry *>> m_NoVisibleGeometry[VSCuller::RG_MAX];
		typedef VSArray<VSArray<VSGeometryOcclusionQueryData>> GeometryOcclusionQueryDataType;
		GeometryOcclusionQueryDataType m_GeometryOcclusionQueryBuffer[VSCuller::RG_MAX][3];


		GeometryOcclusionQueryDataType *m_GOQBufferPointer[VSCuller::RG_MAX][3];


		VSOcclusionQueryPool m_OcclusionQueryPool;

		void GetQueryData();
		VSArray<VSCuller * > m_PendingRegister;
		VSArray<VSCuller * > m_PendingDelete; 
		void OnRegisterCuller(VSCuller * pCuller);
		void OnUnRegisterCuller(VSCuller * pCuller);
	};
}