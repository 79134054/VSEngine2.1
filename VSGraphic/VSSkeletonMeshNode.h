#pragma once
#include "VSModelMeshNode.h"
#include "VSSkeleton.h"
#include "VSAnimSet.h"
#include "VSAnimSequenceFunc.h"
#include "VSSocketNode.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSSkeletonMeshNode : public VSModelMeshNode, public VSResource
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSSkeletonMeshNode();
		virtual ~VSSkeletonMeshNode();
	protected:
		VSSkeletonPtr m_pSkeleton;
		VSAnimSetPtr   m_pAnimSet;
		VSAnimSequenceFuncPtr m_pAnimSequence;

	public:
		void SetSkeleton(VSSkeleton * pSkeleton);
		void SetAnimSet(VSAnimSet * pAnimSet);
		void SetAnimTree(VSAnimTreeR * pAnimTree);
		virtual void LoadedEvent(VSResourceProxyBase * pResourceProxy, void * Data = NULL);
		inline VSSkeleton * GetSkeleton()const
		{
			return m_pSkeleton;
		}
		inline VSAnimSet * GetAnimSet()const
		{
			return m_pAnimSet;
		}
		inline void SetIsDrawSkeleton(bool bIsDrawSkeleton)
		{
			if (m_pSkeleton)
			{
				m_pSkeleton->m_bIsDrawSkeleton = bIsDrawSkeleton;
			}
		}

		inline bool IsDrawSkeleton()const
		{
			if (m_pSkeleton)
			{
				return m_pSkeleton->m_bIsDrawSkeleton;
			}
			return 0;
		}
		virtual void UpdateController(double dAppTime);
		//can not use with anim tree
		bool PlayAnim(const VSString & AnimName,VSREAL fRatio = 1.0f,unsigned int uiRepeatType = VSController::RT_NONE);
		void StopAnim();

		virtual bool PostLoad(VSStream* pStream);
		virtual bool PostClone(VSObject * pObjectSrc);
		virtual void UpdateAll(double dAppTime);
		VSSocketNode * CreateSocket(const VSUsedName & BoneName,const VSUsedName &SocketName);
		void DeleteSocket(const VSUsedName &SocketName);
		VSSocketNode * GetSocket(const VSUsedName &SocketName);
		void SetAnimTreeNodePara(const VSUsedName & ShowName, void * pPara);
		inline const VSAnimTree * GetAnimTree()const
		{
			return m_pAnimTreeInstance;
		}
	protected:
		virtual void UpdateNodeAll(double dAppTime);
		virtual void ComputeNodeVisibleSet(VSCuller & Culler,bool bNoCull,double dAppTime);
		virtual void UpdateWorldBound(double dAppTime);
	protected:
		VSAnimTreeRPtr m_pAnimTree;
		
		VSAnimTreePtr m_pAnimTreeInstance;
		
		void UpdateLocalAABB();
		VSArray<VSSocketNodePtr> m_pSocketArray;
		void UpdateInstanceTexture();
		void CreateAnimInstanceData();
		VSArray<VSVector3W> m_AnimData;
		VSArray<VSHalfVector3W> m_HalfAnimData;
		unsigned int m_uiAnimInstanceTextureHeight;
		unsigned int m_uiAnimInstanceTextureWidth;
		VSArray<unsigned int> m_uiKeyFrameLengthArrray;
		VSTexAllStatePtr m_AnimInstanceTexture;
	public:
		DECLARE_RESOURCE(VSSkeletonMeshNode, RA_ASYN_LOAD | RA_ENABLE_GC, RT_SKELECTON_MODEL,SKMODEL, Resource/SkelectionMesh, VSCacheResource)
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
		const VSAnimAtom &GetRootDelta();
		virtual bool SetDrawInstance(bool bDrawInstance);
		VSArray<ANIM_INSTANCE_SHADER_DATA> m_AnimInstanceData;
		inline VSTexAllState * GetAnimInstanceTexture()const
		{
			return m_AnimInstanceTexture;
		}
	};
	DECLARE_Ptr(VSSkeletonMeshNode);
	VSTYPE_MARCO(VSSkeletonMeshNode);
}
