#pragma once
#include "VSContainer.h"
#include "VSBinaryTree.h"
#include "VSArray.h"
#include "VSMath.h"
#include "VSList.h"
namespace VSEngine2
{
	template <class HashType, class VSMemManagerClass = VSDefaultContainerMem>
	class VSHash : public VSContainer<HashType, VSMemManagerClass>
	{
	public:
		VSHash()
		{		
		}
		template <typename T>
		void AddElement(const T & Element)
		{
			unsigned int HashID = GetTypeHash(Element) % m_Hash.GetNum();
			m_Hash[HashID].AddElement(Element);
		}
		template <typename T>
		bool Erase(const T & Element)
		{
			unsigned int HashID = GetTypeHash(Element) % m_Hash.GetNum();
			return m_Hash[HashID].Erase(Element);
		}
		template <typename T>
		bool Has(const T & Element)const
		{
			unsigned int HashID = GetTypeHash(Element) % m_Hash.GetNum();
			return m_Hash[HashID].Has(Element);
		}
		template <typename T>
		const T * FindElement(const T & Element)const
		{
			unsigned int HashID = GetTypeHash(Element) % m_Hash.GetNum();
			return m_Hash[HashID].FindElement(Element);
		}
		HashType * GetHashTarget(unsigned int HashID)
		{
			if (HashID < m_Hash.GetNum())
			{
				return &m_Hash[HashID];
			}
			else
			{
				return NULL;
			}
			
		}
		inline void SetHashNum(unsigned int uiHashNum)
		{
			VSMAC_ASSERT(uiHashNum);
			m_Hash.SetBufferNum(uiHashNum);
		}
	protected:
		VSArray<HashType, VSMemManagerClass> m_Hash;
	};
	template <class T, class VSMemManagerClass = VSDefaultContainerMem>
	class VSHashTree : public VSHash<VSBinaryTree<T, VSMemManagerClass>, VSMemManagerClass>
	{
	public:
		VSHashTree():VSHash<VSBinaryTree<T, VSMemManagerClass>, VSMemManagerClass>()
		{
		}
		const VSBinaryTreeNode<T> * Find(const T & Element)const
		{
			unsigned int HashID = GetTypeHash(Element) % m_Hash.GetNum();
			return &m_Hash[HashID].Find(Element);
		}
		template<class N>
		void PreProcess(N & Process)
		{
			for (unsigned int i = 0 ; i < m_Hash.GetNum() ; i++)
			{
				m_Hash[i].PreProcess(Process);
			}
		}
		template<class N>
		void PostProcess(N &Process)
		{
			for (unsigned int i = 0; i < m_Hash.GetNum(); i++)
			{
				m_Hash[i].PostProcess(Process);
			}
		}
		template<class N>
		void MiddleProcess(N & Process)
		{
			for (unsigned int i = 0; i < m_Hash.GetNum(); i++)
			{
				m_Hash[i].MiddleProcess(Process);
			}
		}
	};
	template <class T, class VSMemManagerClass = VSDefaultContainerMem>
	class VSHashList : public VSHash<VSList<T, VSMemManagerClass>, VSMemManagerClass>
	{
	public:
		VSHashList():VSHash<VSList<T, VSMemManagerClass>, VSMemManagerClass>()
		{
			
		}
		const ListElement<T> * Find(const T & Element)const
		{
			unsigned int HashID = GetTypeHash(Element) % m_Hash.GetNum();
			return &m_Hash[HashID].Find(Element);
		}
		inline void SetHashNum(unsigned int uiHashNum)
		{
			VSMAC_ASSERT(uiHashNum);
			m_Hash.SetBufferNum(uiHashNum);
			for (unsigned int i = 0; i < m_Hash.GetNum(); i++)
			{
				m_Hash[i].SetUnique(false);
			}
		}
		class VSHashListIterator 
		{
		public:
			VSHashListIterator()
			{
				m_pOwner = NULL;
				m_iCurHashID = -1;
				m_pNode = NULL;
			}
			VSHashListIterator(VSHashList * InOwner)
			{
				m_pOwner = InOwner;				
				if (InOwner)
				{
					m_iCurHashID = 0;
					m_pNode = InOwner->m_Hash[m_iCurHashID].Begin().GetNode();
				}
				else
				{
					Invald();
				}
				
			}
			VSHashListIterator(const VSHashListIterator& Iterator)
			{
				m_pOwner = Iterator.m_pOwner;
				m_pNode = Iterator.m_pNode;
				m_iCurHashID = Iterator.m_iCurHashID;
			}
			~VSHashListIterator()
			{
				Invald();
			}
			void operator= (const VSHashListIterator& Iterator)
			{
				m_pNode = Iterator.m_pNode;
				m_pOwner = Iterator.m_pOwner;
				m_iCurHashID = Iterator.m_iCurHashID;
			}
			bool operator!= (const VSHashListIterator& Iterator)
			{
				return (m_pNode != Iterator.m_pNode || m_pOwner != Iterator.m_pOwner || m_iCurHashID != Iterator.m_iCurHashID);
			}
			bool operator== (const VSHashListIterator& Iterator)
			{
				return (m_pNode == Iterator.m_pNode && m_pOwner == Iterator.m_pOwner && m_iCurHashID == Iterator.m_iCurHashID);
			}

			void operator= (ListElement<T> * pNode)
			{
				m_pNode = pNode;
			}
			bool operator!= (ListElement<T> * pNode)
			{
				return (m_pNode != pNode);
			}
			bool operator== (ListElement<T> * pNode)
			{
				return (m_pNode == pNode);
			}

			VSHashListIterator operator++()
			{
				if (m_pNode)
				{
					m_pNode = m_pNode->m_pNext;
					while (!m_pNode)
					{						
						m_iCurHashID++;
						if (m_iCurHashID == m_pOwner->m_Hash.GetNum())
						{
							Invald();
							break;
						}
						m_pNode = m_pOwner->m_Hash[m_iCurHashID].Begin().GetNode();
					}
				}
				return (*this);
			}
			VSHashListIterator operator++(int)
			{
				VSHashListIterator Temp = *this;
				if (m_pNode)
				{
					m_pNode = m_pNode->m_pNext;
					while (!m_pNode)
					{
						m_iCurHashID++;
						if (m_iCurHashID == m_pOwner->m_Hash.GetNum())
						{
							Invald();
							break;
						}
						m_pNode = m_pOwner->m_Hash[m_iCurHashID].Begin().GetNode();
					}
				}
				return Temp;
			}
			VSHashListIterator operator--()
			{
				VSHashListIterator Temp = *this;
				if (m_pNode)
				{
					m_pNode = m_pNode->m_pFront;
					while (!m_pNode)
					{						
						m_iCurHashID--;
						if (m_iCurHashID == -1)
						{
							Invald();
							break;
						}
						m_pNode = m_pOwner->m_Hash[m_iCurHashID].Begin().GetNode();
					}
				}
				return Temp;
			}
			VSHashListIterator operator--(int)
			{
				if (m_pNode)
				{
					m_pNode = m_pNode->m_pFront;
					while (!m_pNode)
					{
						m_iCurHashID--;
						if (m_iCurHashID == -1)
						{
							Invald();
							break;
						}
						m_pNode = m_pOwner->m_Hash[m_iCurHashID].Begin().GetNode();
					}
				}
				return (*this);
			}
			bool IsValid()
			{
				return (m_pNode != NULL);
			}
			T &operator*()
			{
				if (m_pNode)
				{
					return m_pNode->Element;
				}
				else
				{
					VSMAC_ASSERT(0);
					return m_pNode->Element;
				}
			}
		protected:
			void Invald()
			{
				m_pOwner = NULL;
				m_iCurHashID = -1;
				m_pNode = NULL;
			}
			VSHashList * m_pOwner;
			int m_iCurHashID;
			ListElement<T> * m_pNode;
		};
		VSHashListIterator Begin()
		{
			return VSHashListIterator(this);
		}
		VSHashListIterator End()
		{
			return VSHashListIterator();
		}
	};
}
