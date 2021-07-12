#pragma once
#include"VSRtti.marc"
#include "VSString.h"
#include "VSGraphic.h"
#include "VSMemManager.h"
#include "VSArray.h"
#include "VSName.h"
namespace VSEngine2
{
class VSObject;
typedef VSObject *(*CreateObjectFun)();
class VSProperty;
class VSFunction;
class VSGRAPHIC_API VSRtti : public VSMemObject
{
	//PRIORITY
	DECLARE_PRIORITY
	DECLARE_INITIAL_ONLY
public:
	VSRtti(const TCHAR * pcRttiName,VSRtti *pBase,CreateObjectFun COF);
	~VSRtti();


	inline const VSString &GetName()const;
	inline bool IsSameType(const VSRtti &Type) const;
	inline bool IsDerived(const VSRtti &Type) const;
	inline VSRtti* GetBase()const
	{
		return m_pBase;
	}
	VSProperty * GetProperty(unsigned int uiIndex)const;
	VSProperty *GetProperty(const VSString & PropertyName)const;
	unsigned int GetPropertyNum()const;
	void AddProperty(VSProperty * pProperty);
	void AddProperty(VSRtti & Rtti);
	friend class VSObject;
	typedef VSObject *(*CreateObjectFun)();
	void ClearProperty();

	VSFunction * GetFunction(unsigned int uiIndex)const;
	unsigned int GetFunctionNum()const;
	void AddFunction(VSFunction * pProperty);
	void AddFunction(VSRtti & Rtti);
	void ClearFunction();
	static const VSRtti * GetRttiByName(const VSUsedName & RttiName);
private:
	VSString		m_cRttiName;
	VSRtti*	m_pBase;
	VSArray<VSProperty *> m_PropertyArray;
	VSArray<VSFunction *> m_FunctionArray;
	CreateObjectFun m_CreateFun;
	static VSMapOrder<VSUsedName, VSRtti *> ms_RttiMap;
	static VSArray<VSRtti *> &GetRttiArray()
	{
		static VSArray<VSRtti *> ms_RttiArray;
		return ms_RttiArray;
	}
	static bool InitialDefaultState();
	static bool TerminalDefaultState();
};

#include"VSRtti.inl"
}
