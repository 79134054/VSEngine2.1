#pragma once
#include "VSContainer.h"
namespace VSEngine2
{
	template<class T>
	class VSBit
	{
	public:
		enum 
		{
			BYTE_SIZE = sizeof(T) * 8
		};
		VSBit(const VSBit<T> &Bit)
		{
			m_Member = Bit.m_Member;
		}
		VSBit()
		{
			m_Member = 0;
		}
		bool operator[] (unsigned int uiIndex)
		{
			int bit = uiIndex % BYTE_SIZE;
			return (((m_Member & (1 << bit)) >> bit) == 1);
		}

		void operator= (const VSBit<T> &Bit)
		{
			m_Member = Bit.m_Member;
		}
		void Set(unsigned int uiIndex, bool bValue)
		{

			int bit = uiIndex % BYTE_SIZE;


			if (bValue == true)

				m_Member = (m_Member | (1 << bit));
			else

				m_Member = (m_Member & (~(1 << bit)));
		}


		void ClearAll()
		{

			m_Member = 0;
		}


		void SetAll()
		{
			m_Member = 0xFFFFFFFF;
		}

		bool HasValue()
		{
			return m_Member > 0;
		}
		unsigned int GetNum()
		{
			return BYTE_SIZE;
		}
	protected:
		T m_Member;
	};
	class VSBitArray
	{
	public:
		enum
		{
			BYTE_SIZE = sizeof(unsigned int) * 8
		};
		VSBitArray(const VSBitArray & BitArray)
		{
			VSMAC_DELETEA(m_pBuffer);
			m_uiSize = BitArray.m_uiSize;
			m_pBuffer = VS_NEW unsigned int[m_uiSize];
			VSMemcpy(m_pBuffer, BitArray.m_pBuffer, sizeof(unsigned int)* m_uiSize);
		}
		VSBitArray( unsigned int uiSize = 0)
		{
			m_pBuffer = 0;
			m_uiSize = 0;
			Resize( uiSize );
		}

		~VSBitArray()
		{
			VSMAC_DELETEA(m_pBuffer);
		}


		void Resize( unsigned int uiSize )
		{
			if (uiSize == 0)
			{
				return;
			}
			unsigned int* pNewVector = 0;

			if( uiSize % BYTE_SIZE == 0 )
				uiSize = uiSize / BYTE_SIZE;
			else
				uiSize = (uiSize / BYTE_SIZE) + 1;

			pNewVector = VS_NEW unsigned int[uiSize];
			
			VSMAC_ASSERT(pNewVector);

			VSMemset(pNewVector,0,uiSize * sizeof(unsigned int));
			unsigned int uiMin;
			if( uiSize < m_uiSize )
				uiMin = uiSize;
			else
				uiMin = m_uiSize;

			for( unsigned int uiIndex = 0; uiIndex < uiMin; uiIndex++ )
				pNewVector[uiIndex] = m_pBuffer[uiIndex];

			m_uiSize = uiSize;

			VSMAC_DELETEA(m_pBuffer);

			m_pBuffer = pNewVector;

		}


		bool operator[] ( unsigned int uiIndex )
		{
			int cell = uiIndex / BYTE_SIZE;
			int bit = uiIndex % BYTE_SIZE;
			return (((m_pBuffer[cell] & (1 << bit)) >> bit) == 1);
		}

		void operator= (const VSBitArray & BitArray)
		{
			m_uiSize = BitArray.m_uiSize;
			VSMAC_DELETEA(m_pBuffer);
			m_pBuffer = VS_NEW unsigned int[m_uiSize]; 
			VSMemcpy(m_pBuffer,BitArray.m_pBuffer,m_uiSize * sizeof(unsigned int));
		}
		void Set( unsigned int uiIndex, bool bValue )
		{
			int cell = uiIndex / BYTE_SIZE;

			int bit = uiIndex % BYTE_SIZE;


			if( bValue == true )

				m_pBuffer[cell] = (m_pBuffer[cell] | (1 << bit));
			else

				m_pBuffer[cell] = (m_pBuffer[cell] & (~(1 << bit)));
		}


		void ClearAll()
		{

			for( unsigned int uiIndex = 0; uiIndex < m_uiSize; uiIndex++ )
				m_pBuffer[uiIndex] = 0;
		}


		void SetAll()
		{

			for( unsigned int uiIndex = 0; uiIndex < m_uiSize; uiIndex++ )
				m_pBuffer[uiIndex] = 0xFFFFFFFF;
		}


		unsigned int Size()
		{
			return m_uiSize * BYTE_SIZE;
		}



		unsigned int GetCell( int uiIndex )
		{
			return m_pBuffer[uiIndex];
		}



		unsigned int* m_pBuffer;

		unsigned int m_uiSize;
	};
}


