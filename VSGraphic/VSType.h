#pragma once
#include "VSPointer.h"
#include "VSString.h"
#include "VSArray.h"
#include "VSMap.h"
#include "VSVector2.h"
#include "VSVector3.h"
#include "VSVector3W.h"
#include "VSMatrix3X3.h"
#include "VSMatrix3X3W.h"
#include "VSQuat.h"
#include "VSAABB3.h"
#include "VSSphere3.h"
#include "VSOBB3.h"
#include "VSRect2.h"
#include "VSLine3.h"
#include "VSRay3.h"
#include "VSSegment3.h"
#include "VSRectangle3.h"
#include "VSTriangle3.h"
#include "VSPlane3.h"
#include "VSTransform.h"
//一般内部使用的从object类继承的，是要区分属性的，所以声明rtti来区分属性，VSTYPE_MARCO
//来区分是内部从object类继承的，支持以属性为单位来实现序列化

//自定义类型，如果想要支持自定义的序列化，要用CUSTOMTYPE_MARCO  声明，不区分属性，
//里面类的属性版本兼容自己要制定。
namespace VSEngine2
{
#define DEFINE_TYPE(TypeName)\
	template<typename T> struct TIs##TypeName##Type { enum { Value = false }; };\
	template<typename T> struct TIs##TypeName##PointerType { enum { Value = false }; };\
	template<typename T> struct TIs##TypeName##SmartPointerType { enum { Value = false }; };
#define DEFINE_TYPE_INSTANCE(TypeName,ClassName)\
	class ClassName;\
	template<> struct TIs##TypeName##Type<ClassName> { enum { Value = true }; }; \
	template<> struct TIs##TypeName##PointerType<ClassName *> { enum { Value = true }; }; \
	template<> struct TIs##TypeName##PointerType<const ClassName *> { enum { Value = true }; }; \
	template<> struct TIs##TypeName##SmartPointerType<VSPointer<ClassName>> { enum { Value = true }; }; \
	template<> struct TIs##TypeName##SmartPointerType<const VSPointer<ClassName>> { enum { Value = true }; };

	//custom type
	DEFINE_TYPE(Custom)
#define CUSTOMTYPE_MARCO(ClassName) \
	DEFINE_TYPE_INSTANCE(Custom,ClassName)

	// vs type
	DEFINE_TYPE(VS)
#define VSTYPE_MARCO(ClassName) \
	DEFINE_TYPE_INSTANCE(VS,ClassName)

	template<typename T> struct TIsVSEnumType									{ enum { Value = false }; };
#define VSENUMTYPE_MARCO(ClassName) \
	template<>	struct TIsVSEnumType<ClassName>									{ enum { Value = true }; }; 
	
	//cache resource
	DEFINE_TYPE(VSCacheResource)
#define DECLARE_CACHE_RESOURCE_TYPE(ClassName) \
	DEFINE_TYPE_INSTANCE(VSCacheResource,ClassName)

	DECLARE_CACHE_RESOURCE_TYPE(VSTextureCache)
	DECLARE_CACHE_RESOURCE_TYPE(VSShaderMapCache)

	template<typename T> struct TIsVSResourceProxyType							{ enum { Value = false}; };
	template<typename T> struct TIsVSResourceProxyPointerType					{ enum { Value = false}; };
	template<class T>
	class VSResourceProxy;
	template<typename T> struct TIsVSResourceProxyType<VSResourceProxy<T>>							{ enum { Value = true }; };
	template<typename T> struct TIsVSResourceProxyPointerType<VSPointer<VSResourceProxy<T>>>		{ enum { Value = true }; };


	
	// string
	template<typename T> struct TIsVSStringType									{ enum { Value = false}; };
	template<>	struct TIsVSStringType<VSString>								{ enum { Value = true }; };

	// name
	template<typename T> struct TIsVSUsedNameType								{ enum { Value = false }; };
	class VSUsedName;
	template<>	struct TIsVSUsedNameType<VSUsedName>							{ enum { Value = true }; };

	// Archive on need Loop VSArray 
	template<typename T> struct TIsNoNeedLoop
	{
		enum { Value = IS_ENUM(T) || TIsArithmeticType<T>::Value };	
	};
	template<>	struct TIsNoNeedLoop<VSVector3W>								{ enum { Value = true }; };
	template<>	struct TIsNoNeedLoop<VSVector3>									{ enum { Value = true }; };
	template<>	struct TIsNoNeedLoop<VSVector2>									{ enum { Value = true }; };
	template<>	struct TIsNoNeedLoop<VSMatrix3X3W>								{ enum { Value = true }; };
	template<>	struct TIsNoNeedLoop<VSMatrix3X3>								{ enum { Value = true }; };
	template<>	struct TIsNoNeedLoop <VSQuat>									{ enum { Value = true }; };
	template<>	struct TIsNoNeedLoop <VSAABB3>									{ enum { Value = true }; };
	template<>	struct TIsNoNeedLoop <VSSphere3>								{ enum { Value = true }; };
	template<>	struct TIsNoNeedLoop <VSOBB3>									{ enum { Value = true }; };
	template<>	struct TIsNoNeedLoop <VSRect2>									{ enum { Value = true }; };
	template<>	struct TIsNoNeedLoop <VSLine3>									{ enum { Value = true }; };
	template<>	struct TIsNoNeedLoop <VSRay3>									{ enum { Value = true }; };
	template<>	struct TIsNoNeedLoop <VSSegment3>								{ enum { Value = true }; };
	template<>	struct TIsNoNeedLoop <VSRectangle3>								{ enum { Value = true }; };
	template<>	struct TIsNoNeedLoop <VSTriangle3>								{ enum { Value = true }; };
	template<>	struct TIsNoNeedLoop <VSPlane3>									{ enum { Value = true }; };
	template<typename T> struct TIsNeedGC										
	{ 
		enum 
		{
			Value = TIsVSPointerType<T>::Value || 
			TIsVSSmartPointerType<T>::Value || 
			TIsCustomType<T>::Value || 
			TIsVSType<T>::Value
		};
	};
	template<typename T> struct TIsNeedGC<VSArray<T>>
	{
		enum
		{
			Value = TIsNeedGC<T>::Value
		};
	};
	template<typename T> struct TIsNeedGC<VSArrayOrder<T>>
	{
		enum
		{
			Value = TIsNeedGC<T>::Value
		};
	};
	template<typename Key, typename Value> struct TIsNeedGC<VSMapOrder<Key, Value>> 
	{
		enum 
		{
			Value = TIsNeedGC<Key>::Value || TIsNeedGC<Value>::Value
		};
	};
	template<typename Key, typename Value> struct TIsNeedGC<VSMap<Key, Value>>
	{
		enum
		{
			Value = TIsNeedGC<Key>::Value || TIsNeedGC<Value>::Value
		};
	};
}
