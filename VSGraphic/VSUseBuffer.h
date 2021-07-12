#pragma once
#include "VSObject.h"
namespace VSEngine2
{
	class VSGRAPHIC_API VSUseBuffer : public VSReference,public VSMemObject
	{
	public:
		
		VSUseBuffer(bool IsStatic,unsigned int uiBufferSize = 512);
		virtual ~VSUseBuffer();

	public:

		void * NewGet(unsigned int uiDataSize);
		bool Add(const void * pData,unsigned int uiDataSize);

		void Clear();

		inline unsigned char * GetBuffer()const{return m_pBuffer;}
		inline unsigned int GetBufferSize()const {return m_uiBufferSize;}
		inline unsigned int GetCurSize()const {return m_uiCurSize;}
		

		unsigned int GetSizeByElementIndex(unsigned int i0,unsigned int i1);
		unsigned int GetSizeByElementIndex(unsigned int i);

		inline unsigned int GetOneAddLimitSize()const {return m_uiOneAddLimitSize;}
		inline void SetOneAddLimitSize(unsigned int uiOneAddLimitSize) {m_uiOneAddLimitSize = uiOneAddLimitSize;}

		
		class VSBufferElementInfo
		{
		public:
			VSBufferElementInfo()
			{
				m_uiStart = 0;
				m_uiEnd = 0;
			}
			static VSBufferElementInfo ElementNULL;
			unsigned int m_uiStart;
			unsigned int m_uiEnd;
			unsigned int GetSize()const
			{
				if (m_uiEnd > m_uiStart)
				{
					return m_uiEnd - m_uiStart;
				}
				else
				{
					return 0;
				}
			}
		
		};

		inline const VSBufferElementInfo *GetElementInfo(unsigned int uiElementIndex)const
		{
			if (uiElementIndex < m_BufferElemetnArray.GetNum())
			{
				return &m_BufferElemetnArray[uiElementIndex];
			}
			else
			{
				return NULL;
			}
		}
		inline unsigned int GetElementNum()const
		{
			return m_BufferElemetnArray.GetNum();
		}
	protected:
		unsigned int m_uiBufferSize;
		bool m_bIsStatic;
		unsigned char * m_pBuffer;
		unsigned int m_uiCurSize;
		VSArray<VSBufferElementInfo> m_BufferElemetnArray;
		unsigned int m_uiOneAddLimitSize;
	};
	DECLARE_Ptr(VSUseBuffer);
}
