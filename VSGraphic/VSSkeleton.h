#pragma once
#include "VSNode.h"
#include "VSGeometry.h"
namespace VSEngine2
{
	DECLARE_Ptr(VSBoneNode);
	class VSStream;
	class VSGRAPHIC_API VSSkeleton : public VSNode
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSSkeleton();
		virtual ~VSSkeleton();

	protected:
		VSArray<VSBoneNode *> m_pBoneArray;

		void LinkBoneArray();
		virtual void UpdateNoChild(double dAppTime);
		virtual void ComputeNodeVisibleSet(VSCuller & Culler,bool bNoCull,double dAppTime);
		virtual void UpdateWorldBound(double dAppTime);
		static VSREAL ms_fBoneAxisLength;
		void Draw(VSCamera * pCamera);
		VSAABB3	m_LocalBV;
		VSAABB3 m_OriginLocalBV;
	public:
		friend class VSSkeletonMeshNode;
		virtual void CreateLocalAABB();
		inline void SetLocalBV(const VSAABB3 & BV)
		{
			m_LocalBV = BV;
		}
		inline VSAABB3 GetLocalBV()const
		{
			return m_LocalBV;
		}
		void CreateBoneArray();
		
		

		unsigned int GetBoneNum()const;
		VSBoneNode *GetBoneNode(const VSUsedName & Name)const;
		VSBoneNode * GetBoneNode(unsigned int i)const;

		int GetBoneIndex(const VSUsedName &Name)const;
		bool m_bIsDrawSkeleton;
	

		virtual void UpdateView(VSCuller & Culler,double dAppTime);
		const VSTransform &GetRootTransform();
	};
	DECLARE_Ptr(VSSkeleton);
	VSTYPE_MARCO(VSSkeleton);
}
