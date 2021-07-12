#pragma once
#include "VSObject.h"
#include "VSVertexBuffer.h"
#include "VSName.h"
#include <VSDelegate.h>
namespace VSEngine2
{
	/*
		ÿ��Morph���ݶ��ж�Ӧ��GeometryNode��GeometryNode����N���ڵ㣨N��Geometry��Mesh��,��Ӧ��Morph
		Ҳ����N��Buffer��Morph��ÿ��Buffer�Ǻ�GeometryNode�Ľڵ�һһ��Ӧ�ġ��ڰ�Morph���뵽MorphSetʱ�����е�Morph��Buffer��Ҫһ����
		���Ҷ�ӦBuffer��������ݣ���Ӧ�Ķ������ҲҪһ����
		��ΪGeometryNode����MorphSet��ʱ��Ҫ���GeometryNode�Ľڵ��MorphSetBuffer�Ƿ�һ������Ҫ���ÿ��Mesh�Ķ������
		�Ƿ��ÿ��Morph��Buffer�Ķ���һ���������Buffer��Ϊ�յ�ʱ��

	*/
	class VSStream;

	class VSGRAPHIC_API VSMorph : public VSObject
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
		
	public:
		VSMorph();
		virtual ~VSMorph();
		inline unsigned int GetBufferNum()const
		{
			return m_pVertexBufferArray.GetNum();
		}
		inline unsigned int GetVertexNum(unsigned int uiBufferID)const
		{
			if (uiBufferID >= m_pVertexBufferArray.GetNum())
			{
				return 0;
			}
			if (m_pVertexBufferArray[uiBufferID] == NULL)
			{
				return 0;
			}
			return m_pVertexBufferArray[uiBufferID]->GetVertexNum();
		}
		inline VSVertexBuffer * GetBuffer(unsigned int uiBufferID)const
		{
			if (uiBufferID >= m_pVertexBufferArray.GetNum())
			{
				return NULL;
			}

			return m_pVertexBufferArray[uiBufferID];
		}
		void ReSizeBuffer(unsigned int uiBufferSize)
		{
			m_pVertexBufferArray.SetBufferNum(uiBufferSize);
		}
		void SetVertexBuffer(unsigned int uiBufferID,VSVertexBuffer * pBuffer)
		{
			if (uiBufferID >= m_pVertexBufferArray.GetNum())
			{
				return;
			}
			m_pVertexBufferArray[uiBufferID] = pBuffer;

		}
		VSUsedName m_cName;
	protected:
		//element is null that is this geometry no morph
		VSArray<VSVertexBufferPtr> m_pVertexBufferArray;
	};
	DECLARE_Ptr(VSMorph);
	VSTYPE_MARCO(VSMorph);


	typedef VSDelegateEvent<void(void)> AddMorphEventType;

	class VSGRAPHIC_API VSMorphSet: public VSObject
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
		
	public:
		VSMorphSet();
		virtual ~VSMorphSet();
		bool AddMorph(VSMorph * pMorph);
		VSMorph * GetMorph(const VSUsedName & MorphName)const;
		VSMorph * GetMorph(unsigned int i)const;
		unsigned int GetMorphIndex(const VSUsedName & MorphName)const;
		inline unsigned int GetMorphNum()const
		{
			return m_pMorphArray.GetNum();
		}
		inline unsigned int GetBufferNum()const
		{
			if (m_pMorphArray.GetNum())
			{
				return m_pMorphArray[0]->GetBufferNum();
			}
			else
				return 0;
		}
		AddMorphEventType m_AddMorphEvent;
	protected:
		VSArray<VSMorphPtr> m_pMorphArray;
	};
	DECLARE_Ptr(VSMorphSet);
	VSTYPE_MARCO(VSMorphSet);

}
