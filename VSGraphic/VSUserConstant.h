#pragma once
#include "VSMemManager.h"
#include "VSMath.h"
#include "VSObject.h"
#include "VSName.h"
namespace VSEngine2
{
class VSStream;
class VSGRAPHIC_API VSUserConstant : public VSObject
{
	//PRIORITY
	
	//RTTI
	DECLARE_RTTI;
	DECLARE_INITIAL
public:
	
	
	enum //Value Type
	{			
		VT_FLOAT,
		VT_BOOL,
		VT_INT,	
		VT_STRUCT,
		VT_MAX
	};
public:
	VSUserConstant(const VSUsedName & Name,unsigned int uiSize,
		unsigned int uiRegisterIndex,unsigned int uiRegisterNum,unsigned int uiValueType = VT_FLOAT);
	VSUserConstant(const VSUsedName & Name, const VSUsedName & NameInShader,unsigned int uiSize,
		unsigned int uiRegisterIndex, unsigned int uiRegisterNum, unsigned int uiValueType = VT_FLOAT);
	virtual ~VSUserConstant();
	bool SetData(void *pElementData);
	inline void* GetData()const;
	inline unsigned int GetValueType()const;
	inline unsigned int GetSize()const;
	inline unsigned int GetRegisterNum()const;
	inline unsigned int GetRegisterIndex()const;
	inline const VSUsedName & GetShowName()const
	{
		return m_Name;
	}
	inline const VSUsedName & GetNameInShader()const
	{
		return m_NameInShader;
	}

	virtual bool PostLoad(VSStream* pStream);
	virtual bool PostClone(VSObject * pObjectSrc);
	unsigned int m_uiSize;
	unsigned int m_uiValueType;	
	unsigned int m_uiRegisterIndex;
	unsigned int m_uiRegisterNum;
protected:
	friend class VSRenderer;
	VSUserConstant();

private:
	
	unsigned char * m_pData;
	VSUsedName m_Name;
	VSUsedName m_NameInShader;
};
#include "VSUserConstant.inl"
VSTYPE_MARCO(VSUserConstant);
DECLARE_Ptr(VSUserConstant);
}
