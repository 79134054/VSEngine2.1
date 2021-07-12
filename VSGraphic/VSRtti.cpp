#include"VSRtti.h"
#include "VSProperty.h"
#include "VSMain.h"
using namespace VSEngine2;
IMPLEMENT_PRIORITY(VSRtti)
IMPLEMENT_INITIAL_ONLY_BEGIN(VSRtti);
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_ONLY_END
VSMapOrder<VSUsedName, VSRtti *> VSRtti::ms_RttiMap;
bool VSRtti::TerminalDefaultState()
{
	return true;
}
bool VSRtti::InitialDefaultState()
{
	for (unsigned int i = 0 ; i < GetRttiArray().GetNum() ; i++)
	{
		VSRtti * pRtti = GetRttiArray()[i];
		ms_RttiMap.AddElement(pRtti->GetName(), pRtti);
	}
	return 1;
}
/********************************************************************************/
VSRtti::VSRtti(const TCHAR* pcRttiName,VSRtti *pBase,CreateObjectFun COF):m_cRttiName(pcRttiName)
{
	m_pBase = pBase;
	m_CreateFun = COF;
	GetRttiArray().AddElement(this);
}//end construct
/********************************************************************************/
VSRtti::~VSRtti()
{

	m_pBase = 0;
	for(unsigned int i = 0 ; i < m_PropertyArray.GetNum() ;i++) 
	{ 
		if(m_PropertyArray[i]) 
		{ 
			VSMAC_DELETE(m_PropertyArray[i]) 
		} 
	} 
	for (unsigned int i = 0; i < m_FunctionArray.GetNum(); i++)
	{
		if (m_FunctionArray[i])
		{
			VSMAC_DELETE(m_FunctionArray[i])
		}
	}

}//end destruct
/********************************************************************************/
VSProperty * VSRtti::GetProperty(unsigned int uiIndex)const
{
	if (uiIndex >= m_PropertyArray.GetNum())
	{
		return NULL;
	}
	return m_PropertyArray[uiIndex];
}
VSProperty *VSRtti::GetProperty(const VSString & PropertyName)const
{
	for (unsigned int i = 0; i < m_PropertyArray.GetNum(); i++)
	{
		if (m_PropertyArray[i]->GetName() == PropertyName)
		{
			return m_PropertyArray[i];
		}
	}
	VSMAC_ASSERT(0);
	return NULL;
}
unsigned int VSRtti::GetPropertyNum()const
{
	return m_PropertyArray.GetNum();
}
void VSRtti::AddProperty(VSProperty * pProperty)
{	
	if (pProperty)
	{
		for (unsigned int i = 0 ; i < m_PropertyArray.GetNum() ; i++)
		{
			VSMAC_ASSERT(m_PropertyArray[i]->GetName() != pProperty->GetName());
		}
		m_PropertyArray.AddElement(pProperty);
	}

}
void VSRtti::AddProperty(VSRtti & Rtti)
{
	for (unsigned int i = 0 ; i < Rtti.m_PropertyArray.GetNum() ; i++)
	{
		VSProperty * pProperty = Rtti.m_PropertyArray[i];
		VSProperty *pNewProperty = pProperty->GetInstance();
		pNewProperty->Clone(pProperty);
		pNewProperty->SetOwner(*this);
		m_PropertyArray.AddElement(pNewProperty);
	}
}
void VSRtti::ClearProperty()
{
	for(unsigned int i = 0 ; i < m_PropertyArray.GetNum() ;i++) 
	{ 
		if(m_PropertyArray[i]) 
		{ 
			VSMAC_DELETE(m_PropertyArray[i]) 
		} 
	} 
	m_PropertyArray.Clear();	
	for (unsigned int i = 0; i < m_FunctionArray.GetNum(); i++)
	{
		if (m_FunctionArray[i])
		{
			VSMAC_DELETE(m_FunctionArray[i])
		}
	}
	m_FunctionArray.Clear();
}
VSFunction * VSRtti::GetFunction(unsigned int uiIndex)const
{
	if (uiIndex >= m_FunctionArray.GetNum())
	{
		return NULL;
	}
	return m_FunctionArray[uiIndex];
}
unsigned int VSRtti::GetFunctionNum()const
{
	return m_FunctionArray.GetNum();
}
void VSRtti::AddFunction(VSFunction * pFunction)
{
	if (pFunction)
	{
		for (unsigned int i = 0; i < m_FunctionArray.GetNum(); i++)
		{
			if (m_FunctionArray[i]->IsSame(pFunction))
			{
				VSMAC_DELETE(m_FunctionArray[i]);
				m_FunctionArray[i] = pFunction;
				return;
			}
		}
		m_FunctionArray.AddElement(pFunction);
	}

}
void VSRtti::AddFunction(VSRtti & Rtti)
{
	for (unsigned int i = 0; i < Rtti.m_FunctionArray.GetNum(); i++)
	{
		VSFunction * pFunction = Rtti.m_FunctionArray[i];
		VSFunction *pNewFunction = pFunction->GetInstance();
		pNewFunction->SetOwner(*this);
		pNewFunction->Clone(pFunction);
		m_FunctionArray.AddElement(pNewFunction);
	}
}
void VSRtti::ClearFunction()
{
	for (unsigned int i = 0; i < m_FunctionArray.GetNum(); i++)
	{
		if (m_FunctionArray[i])
		{
			VSMAC_DELETE(m_FunctionArray[i])
		}
	}
	m_FunctionArray.Clear();
}
const VSRtti * VSRtti::GetRttiByName(const VSUsedName & RttiName)
{
	unsigned int i = ms_RttiMap.Find(RttiName);
	if (i != ms_RttiMap.GetNum())
	{
		return ms_RttiMap[i].Value;
	}
	return NULL;
}