#pragma once
#include "VSGraphic.h"
#include "VSType.h"
#include "VSMap.h"
namespace VSEngine2
{
	class VSObject;
	class VSStream;
	class VSEditorElement;
	//不支持指针序列化，不支持postload 和 postclone
	//类成员如果是VSType 是支持的
	class VSGRAPHIC_API VSCustomArchiveObject:public VSMemObject
	{
	public:
		VSCustomArchiveObject();
		virtual ~VSCustomArchiveObject();
		virtual void Archive(VSStream & Stream) = 0;
		virtual void CopyFrom(VSCustomArchiveObject *,VSMap<VSObject *,VSObject*>& CloneMap) = 0;
		virtual VSEditorElement *  CreateEElement(VSString& Name,VSObject * pOwner);
	};
	CUSTOMTYPE_MARCO(VSCustomArchiveObject)
}
