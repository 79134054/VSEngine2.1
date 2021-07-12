#pragma once
#include "VSThread.h"
#include "VSSafeQueue.h"
#include "VSResource.h"
#include "VSAsynStream.h"
#include "VSRenderThread.h"
namespace VSEngine2
{
	class VSGRAPHIC_API VSAsynJob : public VSMemObject
	{
	public:
		VSAsynJob();
		virtual ~VSAsynJob() = 0;
		enum //JOB STATE
		{		
			JS_PENDING = 0, // you can custom pending state 0-9
			JS_MAIN_THREAD = 10,
			JS_SUCCESS = 245,
			JS_FAIL = 255,
		};
		enum // CONTROLL STATE
		{
			CS_NONE,
			CS_PENDING_JOB,
			CS_PENDING_HAVE_JOB,
			CS_DELETE,
		};
		enum // Job Type
		{
			JT_FILE,
			JT_RESOURCE,
		};
		unsigned int m_uiControllState;
		unsigned int m_uiJobState;
		virtual void MainThreadProcess() = 0;
		virtual void AsynThreadProcess() = 0;
		virtual unsigned int GetJobType()const = 0;
		virtual void Failed() = 0;
	};
	class VSGRAPHIC_API VSFileJob :public VSAsynJob
	{
	public:
		VSFileJob();
		virtual ~VSFileJob() = 0;
		virtual void MainThreadProcess() = 0;
		virtual void AsynThreadProcess();
		virtual unsigned int GetJobType()const
		{
			return JT_FILE;
		}
		VSFileName  m_FileName;
	protected:
		unsigned char * m_pBuffer;
		unsigned int m_uiSize;
	};
	class VSGRAPHIC_API VSResourceJob : public VSFileJob
	{
	public:
		enum 
		{
			JS_WaitLoadFromDisk_AnyThread = JS_PENDING,
			JS_CreateLoadObject_MainThread = JS_MAIN_THREAD,
			JS_LoadAndLinkObject_AnyThread = JS_WaitLoadFromDisk_AnyThread + 1,
			JS_PostLoadObject_AnyThread = JS_LoadAndLinkObject_AnyThread + 1,
			JS_GetResource_MainThread = JS_CreateLoadObject_MainThread + 1,
			JS_SetResource_MainThread = JS_GetResource_MainThread + 1,
			JS_CacheResourceReady_MainThread = JS_SetResource_MainThread + 1,
			JS_WaitingCacheResource_MainThread = JS_CacheResourceReady_MainThread + 1,
		};
		VSResourceJob();
		virtual ~VSResourceJob() = 0;
		virtual unsigned int GetResourceType()const = 0;
		VSAsynStream m_BreakLoadStream;
		VSStream m_LoadStream;
		virtual void AsynThreadProcess();
		virtual void MainThreadProcess();
		virtual unsigned int GetJobType()const
		{
			return JT_RESOURCE;
		}
		static bool ms_bEnableBreakLoadResouce;
	protected:
		virtual void Failed();
	};
	template<typename CacheType>
	class VSGRAPHIC_API VSCacheResourceJobBase : public VSResourceJob
	{
	public:
		virtual ~VSCacheResourceJobBase()
		{
			m_pCacheResource = NULL;
		}
		virtual void MainThreadProcess()
		{
			VSResourceJob::MainThreadProcess();
			if (!ms_bEnableBreakLoadResouce)
			{
				if (m_uiJobState == JS_MAIN_THREAD)
				{
					m_pCacheResource = (CacheType*)m_LoadStream.GetObjectByRtti(CacheType::ms_Type);
					if (!m_pCacheResource)
					{
						m_uiJobState = JS_FAIL;
						return;
					}
					GetCacheResource();
				}
				return;
			}
			if (m_uiJobState == JS_GetResource_MainThread)
			{
				m_pCacheResource = (CacheType*)m_BreakLoadStream.GetObjectByRtti(CacheType::ms_Type);
				if (CacheType::ms_bEnableAsynPostLoad)
				{
					m_uiJobState = JS_PostLoadObject_AnyThread;
					VSASYNLoadManager::ms_pASYNLoadManager->AddHaveJob(this);
				}
				else if (m_pCacheResource)
				{
					m_BreakLoadStream.PostLoadObject(m_pCacheResource);
					GetCacheResource();
				}
				else
				{
					m_uiJobState = JS_FAIL;
				}

			}
			else if (m_uiJobState == JS_SetResource_MainThread)
			{
				GetCacheResource();
			}
		}
		virtual void AsynThreadProcess()
		{
			VSResourceJob::AsynThreadProcess();
			if (m_uiJobState == JS_PostLoadObject_AnyThread)
			{
				m_BreakLoadStream.PostLoadObject(m_pCacheResource);
				m_uiJobState = JS_SetResource_MainThread;
			}
		}
		virtual void GetCacheResource() = 0;
	protected:
		CacheType* m_pCacheResource = NULL;
	};
	template<typename T,typename CacheType>
	class VSGRAPHIC_API VSCacheResourceJob : public VSCacheResourceJobBase<CacheType>
	{
	public:
		VSCacheResourceJob(T * pResource)
			:m_pResource(pResource)
		{

		}
		virtual ~VSCacheResourceJob()
		{
			m_pResource = NULL;
		}
		virtual unsigned int GetResourceType()const
		{
			return T::GetResourceType();
		}
	protected:
		virtual void GetCacheResource()
		{
			m_pCacheResource->SetCacheResource(m_pResource);
			VS_DELETE m_pCacheResource;
			m_uiJobState = JS_CacheResourceReady_MainThread;
		}
		T * m_pResource = NULL;
		
	};
	template<typename T, typename CacheType>
	class VSGRAPHIC_API VSStreamResourceJob : public VSCacheResourceJobBase<CacheType>
	{
	public:
		VSStreamResourceJob(VSResourceProxy<T>* pProxy)
		{
			m_pProxy = pProxy;
		}
		virtual ~VSStreamResourceJob()
		{
			m_pProxy = NULL;
		}
		virtual unsigned int GetResourceType()const
		{
			return T::GetResourceType();
		}
		/*virtual void AsynThreadProcess()
		{
			if (m_uiJobState == JS_WaitLoadFromDisk_AnyThread)
			{
				m_pProxy->GetWantStreamLevel();
			}
			VSCacheResourceJobBase<CacheType>::AsynThreadProcess();
		}*/
	protected:
		virtual void GetCacheResource()
		{
			m_pCacheResource->SetStreamResouce(m_pProxy);
			VS_DELETE m_pCacheResource;
			m_uiJobState = JS_SUCCESS;
		}
		VSResourceProxy<T>* m_pProxy;
	};
	template<typename T>
	class VSGRAPHIC_API VSTResourceJob : public VSResourceJob
	{	
	public:
		VSTResourceJob(VSPointer<VSResourceProxy<T>> &pProxy)
		{
			m_pProxy = pProxy;
		}
		virtual ~VSTResourceJob()
		{
			m_pProxy = NULL;
		}
		virtual void MainThreadProcess()
		{	
			VSResourceJob::MainThreadProcess();
			if (!ms_bEnableBreakLoadResouce)
			{
				if ( m_uiJobState == JS_MAIN_THREAD)
				{
					m_pResource = (T *)m_LoadStream.GetObjectByRtti(T::ms_Type);
					TryResourceSuccess();
				}
				else if (m_uiJobState == JS_WaitingCacheResource_MainThread)
				{
					WaitingCacheResource();
				}
				return;
			}
			if (m_uiJobState == JS_GetResource_MainThread)
			{
				m_pResource = (T *)m_BreakLoadStream.GetObjectByRtti(T::ms_Type);
				if (m_pResource && (m_pProxy->GetResourceAbility() & RA_ASYN_POSTLOAD))
				{
					m_uiJobState = JS_PostLoadObject_AnyThread;
					VSASYNLoadManager::ms_pASYNLoadManager->AddHaveJob(this);					
				}
				else if (m_pResource)
				{
					m_BreakLoadStream.PostLoadObject(m_pResource);
					TryResourceSuccess();
				}
				else
				{
					m_uiJobState = JS_FAIL;
				}
			}
			else if (m_uiJobState == JS_SetResource_MainThread)
			{
				TryResourceSuccess();
			}
			else if (m_uiJobState == JS_WaitingCacheResource_MainThread)
			{
				WaitingCacheResource();
			}
		}
		
		virtual void AsynThreadProcess()
		{
			VSResourceJob::AsynThreadProcess();
			if (m_uiJobState == JS_PostLoadObject_AnyThread)
			{
				m_BreakLoadStream.PostLoadObject(m_pResource);
				m_uiJobState = JS_SetResource_MainThread;
			}
		}
		virtual unsigned int GetResourceType()const
		{
			return T::GetResourceType();
		}
	protected:
		T * m_pResource = NULL;
		VSPointer<VSResourceProxy<T>> m_pProxy;
		VSCacheResourceJob<T, typename T::CacheType> * m_pCacheJob = NULL;
		void CreateCacheResourceJob()
		{
			if (!m_pCacheJob)
			{
				m_pCacheJob = VSASYNLoadManager::ms_pASYNLoadManager->AddCacheResourceJob(m_pResource);
			}
			if (m_pCacheJob)
			{
				m_uiJobState = JS_WaitingCacheResource_MainThread;
			}
		}
		void ResourceSuccess()
		{			
			m_pProxy->Loaded();
			m_uiJobState = JS_SUCCESS;
		}
		void TryResourceSuccess()
		{
			m_pProxy->SetNewResource(m_pResource);
			if (m_pProxy->GetResourceAbility() & RA_NEED_CACHE)
			{
				CreateCacheResourceJob();
			}
			else
			{
				ResourceSuccess();
			}
		}
		void WaitingCacheResource()
		{
			if (m_pCacheJob->m_uiJobState == JS_CacheResourceReady_MainThread)
			{
				ResourceSuccess();
				m_pCacheJob->m_uiJobState = JS_SUCCESS;
			}
			else if (m_pCacheJob->m_uiJobState == JS_FAIL)
			{
				m_uiJobState = JS_FAIL;
			}
		}
	};
		
	class VSGRAPHIC_API VSAsynJobThread : public VSThread
	{
	public:
		VSAsynJobThread();
		virtual ~VSAsynJobThread();
		bool AddJob(VSAsynJob* m_pJob);
		void Stop();
		inline bool IsEmpty()
		{
			return m_pResourceQueue.TryIsEmpty();
		}
	protected:
		virtual const TCHAR* GetThreadName()
		{
			return _T("VSAsynJobThread");
		}
		VSSafeQueue<VSAsynJob*> m_pResourceQueue;
		virtual void Run();	
	};
	
	class VSGRAPHIC_API VSASYNLoadManager
	{
	public:
		enum MyEnum
		{
			TASK_THREAD_NUM = 2
		};
		VSASYNLoadManager();
		~VSASYNLoadManager();
		void FlushAllJob();
		void Update(double AppTime);
		template<typename T>
		void AddResource(VSPointer<VSResourceProxy<T>>& ResourceRPTR, VSFileName & FileName)
		{
			VSFileJob* pResourceProxyJob = VS_NEW VSTResourceJob<T>(ResourceRPTR);
			pResourceProxyJob->m_FileName = FileName;
			AddJob(pResourceProxyJob);
		}
		void DeleteLoadResource(VSFileName & FileName);
		static VSASYNLoadManager * ms_pASYNLoadManager;
		
		
		friend class VSResourceJob;
		template <typename T> friend class VSTResourceJob;
		template<typename CacheType> friend class VSCacheResourceJobBase;
		template<typename T>
		void AddStreamResourceJob(VSResourceProxy<T>* pResourceR)
		{
			VSStreamResourceJob<T, typename T::CacheType>* pResourceCacheJob = VS_NEW VSStreamResourceJob<T, typename T::CacheType>(pResourceR);
			pResourceCacheJob->m_FileName = pResourceR->GetResource()->GetCacheFilePath();
			AddJob(pResourceCacheJob);
		}
	protected:
		void PendingResource(VSREAL fTime);
		template<typename T>
		VSCacheResourceJob<T,typename T::CacheType> * AddCacheResourceJob(T * pResource)
		{
			VSCacheResourceJob<T, typename T::CacheType>* pResourceCacheJob = VS_NEW VSCacheResourceJob<T, typename T::CacheType>(pResource);
			pResourceCacheJob->m_FileName = pResource->GetCacheFilePath();
			AddJob(pResourceCacheJob);
			return pResourceCacheJob;
		}
		VSAsynJobThread m_AsynJobThread[TASK_THREAD_NUM];
		VSArray<VSAsynJob * > m_JobArray;
		void AddJob(VSAsynJob* pJob);
		void AddHaveJob(VSAsynJob * pJob);
	};
}
