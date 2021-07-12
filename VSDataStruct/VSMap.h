#pragma once
#include "VSContainer.h"
namespace VSEngine2
{
	
	template<class KEY,class VALUE, class VSMemManagerClass = VSDefaultContainerMem>
	class VSMap : public VSContainer<MapElement<KEY,VALUE>, VSMemManagerClass>
	{
	public:
		enum
		{
			DEFAULT_GROWBY = 10
		};
		
		VSMap(unsigned int uiGrowBy = DEFAULT_GROWBY);
		~VSMap();

		void SetBufferNum(unsigned int uiBufferNum);
		
		void operator= (const VSMap<KEY,VALUE, VSMemManagerClass>& Map);


		inline unsigned int GetNum()const;

		inline unsigned int GetBufferNum()const;

		inline MapElement<KEY,VALUE> * GetBuffer()const;
		inline void SetGrowBy(unsigned int uiGrowBy);
		
		template<class KEY1,class VALUE1>
		unsigned int AddElement(const MapElement<KEY1,VALUE1> & Element);
		unsigned int AddElement(const KEY& Key , const VALUE &Value);

		template<class KEY1,class VALUE1, class VSMemManagerClass1>
		void AddElement(const VSMap<KEY1,VALUE1, VSMemManagerClass1> & Map,unsigned int uiBegin,unsigned int uiEnd);

		MapElement<KEY,VALUE>& operator[] (unsigned int i)const;

		void Clear();

		inline unsigned int GetSize()const;

		void Erase(unsigned int i);

		void Erase(unsigned int uiBegin,unsigned int uiEnd);

		template <class N>
		void Sort(unsigned int uiBegin,unsigned int uiEnd,N& Compare);
		template <class N>
		void SortAll(N& Compare);
		void Sort(unsigned int uiBegin,unsigned int uiEnd);
		void SortAll();
		unsigned int Find(const KEY& Key)const ;

		unsigned int FindValueIndex(const VALUE &Value)const;

		void Destroy();


		class VSMapIterator
		{
		public:
			VSMapIterator(MapElement<KEY, VALUE> * pNode = NULL) :m_pNode(pNode)
			{
			}
			VSMapIterator(const VSMapIterator& Iterator)
			{
				m_pNode = Iterator.m_pNode;
			}
			~VSMapIterator() = default;
			inline void operator= (const VSMapIterator& Iterator)
			{
				m_pNode = Iterator.m_pNode;
			}
			inline bool operator!= (const VSMapIterator& Iterator)
			{
				return (m_pNode != Iterator.m_pNode);
			}
			inline bool operator== (const VSMapIterator& Iterator)
			{
				return (m_pNode == Iterator.m_pNode);
			}

			inline VSMapIterator operator++()
			{
				m_pNode++;
				return (*this);
			}
			inline VSMapIterator operator++(int)
			{
				VSMapIterator _Tmp = *this;
				++*this;
				return (_Tmp);
			}
			inline VSMapIterator operator--()
			{
				m_pNode--;
				return (*this);
			}
			inline VSMapIterator operator--(int)
			{
				VSMapIterator _Tmp = *this;
				--*this;
				return (_Tmp);
			}
			inline MapElement<KEY, VALUE>& operator*()const
			{
				return *m_pNode;
			}
			inline MapElement<KEY, VALUE>* operator->()const
			{
				return m_pNode;
			}
		protected:
			MapElement<KEY, VALUE> * m_pNode;

		};
		inline VSMapIterator Begin() const
		{
			return VSMapIterator(m_pBuffer);
		}
		inline VSMapIterator End() const
		{
			return VSMapIterator(m_pBuffer + GetNum());
		}
		inline VSMapIterator begin() const
		{
			return Begin();
		}
		inline VSMapIterator end() const
		{
			return End();
		}
	protected:
		void AddBufferNum(unsigned int uiBufferNum);
		MapElement<KEY,VALUE> * m_pBuffer;
		unsigned int m_uiGrowBy;
		unsigned int m_uiCurUse;
		unsigned int m_uiBufferNum;
		unsigned int m_uiAllocNum;
	};

	template<class KEY,class VALUE, class VSMemManagerClass = VSDefaultContainerMem>
	class VSMapOrder : public VSMap<KEY,VALUE, VSMemManagerClass>
	{
	public:
		VSMapOrder(unsigned int uiGrowBy = DEFAULT_GROWBY);
		~VSMapOrder();

		template<class KEY1,class VALUE1>
		unsigned int AddElement(const MapElement<KEY1,VALUE1> & Element);

		unsigned int AddElement(const KEY& Key , const VALUE &Value);

		template<class KEY1, class VALUE1, class VSMemManagerClass1>
		void AddElement(const VSMap<KEY1, VALUE1, VSMemManagerClass1> & Map, unsigned int uiBegin, unsigned int uiEnd);

		unsigned int Find(const KEY& Key)const ;

	protected:
		template<class KEY1,class VALUE1>
		unsigned int Process(unsigned int uiIndex0,unsigned int uiIndex1,const MapElement<KEY1,VALUE1> & Element);

		unsigned int FindElement(unsigned int uiIndex0,unsigned int uiIndex1,const KEY& Key)const;
	};
	#include "VSMap.inl"
}
