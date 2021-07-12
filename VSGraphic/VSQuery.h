#pragma once
#include "VSBind.h"
namespace VSEngine2
{
	class VSGRAPHIC_API VSQuery : public VSBind
	{

		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL

	public:
		enum //QueryType
		{
			QT_OCCLUSION,
			QT_MAX
		};
		virtual ~VSQuery();
		VSQuery(unsigned int uiQueryType);
		inline unsigned int GetQueryType()const
		{
			return m_uiQueryType;
		}
		bool GetQueryData(void * pData, unsigned int uiDataSize);
	protected:
		VSQuery();
		
		virtual bool OnLoadResource(VSResourceIdentifier *&pID);
	protected:
		unsigned int m_uiQueryType;
	};
	DECLARE_Ptr(VSQuery);
	VSTYPE_MARCO(VSQuery);
}