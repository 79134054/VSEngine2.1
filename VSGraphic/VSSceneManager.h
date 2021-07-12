#pragma once
#include "VSSceneRender.h"
#include "VSViewFamily.h"
namespace VSEngine2
{
	class VSGRAPHIC_API VSSceneManager : public VSMemObject
	{
	public:
		VSSceneManager();
		~VSSceneManager();
		
		void Update(double dAppTime);
		void Draw(double dAppTime);
		static VSSceneManager * ms_pSceneManager;
		inline void AddViewFamily(VSViewFamily* pViewFamily);
		inline VSViewFamily * GetViewFamily(unsigned int i)const;
		inline VSViewFamily * GetViewFamily(const VSString & ViewFamilyName)const;
		inline void ClearAllViewFamily();
		inline void DeleteViewFamily(unsigned int i);
		inline void DeleteViewFamily(VSViewFamily * pViewFamily);
		inline unsigned int GetViewFamilyNum()const;
		
		inline void AddScene(VSScene* pScene);
		inline VSScene * GetScene(unsigned int i)const;
		inline void ClearAllScene();
		inline void DeleteScene(unsigned int i);
		inline void DeleteScene(VSScene * pScene);
		inline unsigned int GetSceneNum()const;
	protected:

		VSArray<VSViewFamily*> m_pViewFamily;
		VSArray<VSScene *> m_pScene;
		
	
	};
	
	inline void VSSceneManager::AddScene(VSScene * pSecen)
	{
		if(pSecen)
		{
			for (unsigned int i = 0 ; i < m_pScene.GetNum() ;i++)
			{
				if (m_pScene[i] == pSecen)
				{
					return ;
				}
			}
			m_pScene.AddElement(pSecen);
		}
	}
	inline VSScene * VSSceneManager::GetScene(unsigned int i)const
	{
		if(i >= m_pScene.GetNum())
			return NULL;
		return m_pScene[i];
	}
	inline void VSSceneManager::ClearAllScene()
	{
		m_pScene.Clear();
	}
	inline void VSSceneManager::DeleteScene(unsigned int i)
	{	
		if(i >= m_pScene.GetNum())
			return ;
		m_pScene.Erase(i);

	}
	inline void VSSceneManager::DeleteScene(VSScene * pScene)
	{
		if (!pScene)
		{
			return ;
		}
		for (unsigned int i= 0 ; i <m_pScene.GetNum() ; i++)
		{
			if(pScene == m_pScene[i])
			{
				m_pScene.Erase(i);
			}
		}
	}

	inline unsigned int VSSceneManager::GetSceneNum()const
	{
		return m_pScene.GetNum();
	}
	inline void VSSceneManager::AddViewFamily(VSViewFamily * pViewFamily)
	{
		if(pViewFamily)
		{
			for (unsigned int i = 0 ; i < m_pViewFamily.GetNum() ;i++)
			{
				if (m_pViewFamily[i] == pViewFamily)
				{
					return ;
				}
			}
			m_pViewFamily.AddElement(pViewFamily);
		}
	}
	inline VSViewFamily * VSSceneManager::GetViewFamily(unsigned int i)const
	{
		if(i >= m_pViewFamily.GetNum())
			return NULL;
		return m_pViewFamily[i];
	}
	inline void VSSceneManager::ClearAllViewFamily()
	{
		m_pViewFamily.Clear();
	}
	inline void VSSceneManager::DeleteViewFamily(unsigned int i)
	{	
		if(i >= m_pViewFamily.GetNum())
			return ;
		m_pViewFamily.Erase(i);

	}
	inline void VSSceneManager::DeleteViewFamily(VSViewFamily * pViewFamily)
	{
		if (!pViewFamily)
		{
			return ;
		}
		for (unsigned int i= 0 ; i <m_pViewFamily.GetNum() ; i++)
		{
			if(pViewFamily == m_pViewFamily[i])
			{
				m_pViewFamily.Erase(i);
			}
		}
	}

	inline unsigned int VSSceneManager::GetViewFamilyNum()const
	{
		return m_pViewFamily.GetNum();
	}

}