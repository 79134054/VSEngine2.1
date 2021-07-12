#pragma once
#include "VSGraphic.h"
#include "VSTexAllState.h"
#include "VSMeshComponent.h"
#include "VSMath.h"
namespace VSEngine2
{
	struct VSGRAPHIC_API StreamElementType
	{
		VSArray<VSResourceProxyBase*> pResourceProxy;
		bool bFree = true;
		bool operator ==(const StreamElementType& SET)
		{
			return true;
		}
	};
	class VSGRAPHIC_API VSStreamingManager : public VSMemObject
	{
	public:
		VSStreamingManager();
		~VSStreamingManager();
		static VSStreamingManager* ms_pStreamingManager;
		void Update(double dAppTime);
		static void GetStreamInfo(const VSAABB3& WorldAABB, StreamInformation_TYPE& StreamInformation);
		StreamElementType& GetFreeSlot();
		void ReleaseAllStream();
	protected:
		void GetStreamInfo();
		void DispathStream();
		VSList<StreamElementType> m_StreamObjectList;
	};
}
