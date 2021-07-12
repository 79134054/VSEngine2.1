#pragma once
#include "VSGraphic.h"
#include "VSVector3.h"
#include "VSVector3W.h"
namespace VSEngine2
{
	enum STREAM_STATE
	{
		SS_NONE,
		SS_READY_STREAM,
		SS_STREAMING,
		SS_END_STREAM,
		SS_MAX
	};
	struct StreamInformation_TYPE
	{
		StreamInformation_TYPE()
		{
			m_fStreamInfo = 0.0f;
		};
		VSVector3 CameraPos; 
		VSREAL m_fStreamInfo;
		VSVector3W ProjectInfo;
	};
	class VSGRAPHIC_API VSStreamingType
	{
	public:
		VSStreamingType()
		{
			m_StreamState = SS_NONE;
			m_pToStreamObject = NULL;
			m_uiWantStreamLevel = 0;
		}
		~VSStreamingType()
		{
			
		}
		void ReleaseStreamObject();
		STREAM_STATE m_StreamState;
		bool CanWriteStream(const StreamInformation_TYPE& StreamInformation)
		{
			if (m_uiMaxLevel == 1)
			{
				return false;
			}
			if (m_StreamState == SS_READY_STREAM)
			{
				
				unsigned int uiWantSteamLevel = unsigned int(StreamInformation.m_fStreamInfo * m_uiMaxLevel);
				uiWantSteamLevel = Clamp(uiWantSteamLevel, m_uiMaxLevel, (unsigned int)5);

				unsigned int uiLastWantSteamLevel = VSlockedCompareExchange((long*)&m_uiWantStreamLevel, uiWantSteamLevel, 0);
				MaxMipTable[uiWantSteamLevel] = true;
				return !uiLastWantSteamLevel;

			}

			return false;
		}
		inline unsigned int GetMaxLevel() const
		{
			return m_uiMaxLevel;
		}
		virtual unsigned int GetCurStreamLevel() const = 0;
		virtual const class VSObject* CreateToStreamObject(const class VSCacheResource* pCacheResouce) = 0;		
		virtual void SetStreamJob()
		{
			GetWantStreamLevel();
			unsigned int uiCurStreamLevel = GetCurStreamLevel();
			if (uiCurStreamLevel != m_uiWantStreamLevel)
			{
				ToStreaming();
			}
			else
			{
				ToEndStream();
			}
		}
		virtual void StreamEnd()
		{
			m_pToStreamObject = NULL;
			m_uiWantStreamLevel = 0;
			VSMemset(MaxMipTable, 0, sizeof(MaxMipTable));
			ToReadyStream();
		}
		void GetWantStreamLevel()
		{
			for (int i = m_uiMaxLevel; i >= 0 ; i--)
			{
				if (MaxMipTable[i])
				{
					m_uiWantStreamLevel = i;
					break;
				}
			}

		}
	protected:
		inline void ToReadyStream()
		{
			m_StreamState = SS_READY_STREAM;
		}
		inline void ToStreaming()
		{
			m_StreamState = SS_STREAMING;
		}
		inline void ToEndStream()
		{
			m_StreamState = SS_END_STREAM;
		}
		class VSObject* m_pToStreamObject = NULL;
		unsigned int m_uiWantStreamLevel = 0;
		unsigned int m_uiMaxLevel = 1;
		bool MaxMipTable[MAX_MIP_LEVEL] = {false};
	};
}
