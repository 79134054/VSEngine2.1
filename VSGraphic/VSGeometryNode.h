#pragma once
#include "VSNode.h"
#include "VSMorphSet.h"
#include "VSGeometry.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSGeometryNode : public VSNode
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSGeometryNode();
		virtual ~VSGeometryNode();
		void SetMorphSet(VSMorphSet * pMorphSet);
		inline const VSMorphSet *GetMorphSet()const
		{
			return m_pMorphSet;
		}
		VSGeometry * GetGeometry(unsigned int i)const;
		VSGeometry * GetNormalGeometry(unsigned int index)const;
		unsigned int GetNormalGeometryNum()const;
		virtual bool PostLoad(VSStream* pStream);
		virtual bool PostClone(VSObject * pObjectSrc);
		void SetMorphData(const VSUsedName & ShowName, float fData);
		void SetMorphData(unsigned int Index, float fData);
		VSREAL m_fLODScreenSize = 1.0f;
		virtual void GetStreamResource(VSArray<VSResourceProxyBase*>& pResourceProxy, StreamInformation_TYPE& StreamInformation)const;
	protected:
		VSMorphSetPtr m_pMorphSet;
		void UpdateLocalAABB();
	};
	DECLARE_Ptr(VSGeometryNode);
	VSTYPE_MARCO(VSGeometryNode);
}
