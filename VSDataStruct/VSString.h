#pragma once
#include "VSContainer.h"
#include "VSMath.h"
namespace VSEngine2
{
class VSDATESTRUCT_API VSString : public VSContainer<TCHAR>
{
public:
	enum
	{
		BUFFER_SIZE = 2048
	};
	VSString();
	VSString(const TCHAR * String);
	VSString(const VSString &String);
	~VSString();
	static VSString ms_StringNULL;
	static TCHAR TempBuffer[BUFFER_SIZE];
	inline TCHAR * GetBuffer()const
	{
		return m_pBuffer;
	}
	inline unsigned int GetLength()const
	{
		return (unsigned int)VSStrLen(m_pBuffer);
	}
	void SetTCHARBufferNum(unsigned int uiLength);
	void Clear();
	//����=������
	VSString & operator =(const VSString &String);
	VSString & operator =(const TCHAR *String);

	//����[]������
	TCHAR &operator[](unsigned int i);

	//����+=������
	const VSString & operator +=(const VSString &String);
	const VSString & operator +=(const TCHAR *String);

	//ȡ��ָ���±��ǰ���ַ������ߺ���ַ���
	bool GetString(const VSString & String,unsigned int uiFind,bool bIsFront = true, bool bIsHaveFind = false);
	//���ҵ�iFindNum�γ��ֵ�ָ���ַ���Ȼ��ȡǰ���ַ������ߺ���ַ�����iFIndNum = -1��ʾ���һ�γ��ֵġ�
	bool GetString(const VSString & String,TCHAR cFind,int iFIndNum,bool bIsFront = true, bool bIsHaveFind = false);
	//���ҵ�ǰ�ַ����е�iFindNum�γ��ֵ�String���±꣬����-1���ʾ����ʧ��
	int GetSubStringIndex(const VSString &String, int iFindNum)const;
	bool RemoveSubString(const VSString &String, int iFindNum);
	void Remove(unsigned int i, unsigned int j);
	//ȥ��ָ�����ַ�
	bool StripChars(const VSString &String,const TCHAR * pStripChars);
	//�滻ָ�����ַ�
	bool ReplaceChars(const VSString &String ,const TCHAR * pReplaceChars,TCHAR UseChar);
	//ȥ���ַ�����������пո�
	void StringLtrim(const VSString &String);
	//ȥ���ַ������ұ����пո�
	void StringRtrim(const VSString &String);

	void Format(const TCHAR * pcString, ...);
protected:
	TCHAR * m_pBuffer;

};
	//����+������
	VSDATESTRUCT_API VSString operator +(const VSString & String1, const VSString &String2);
	VSDATESTRUCT_API VSString operator +(const VSString &String1, const TCHAR *String2);
	VSDATESTRUCT_API VSString operator +(const TCHAR *String1, const VSString &String2);

	//���ڷ��ش���0��С�ڷ���С��0�����ڷ���0 
	VSDATESTRUCT_API int CompareString(const VSString &String1,const VSString &String2);
	VSDATESTRUCT_API int CompareString(const VSString &String1,const TCHAR *String2);
	VSDATESTRUCT_API int CompareString(const TCHAR *String1,const VSString &String2);

	//����==������
	VSDATESTRUCT_API bool operator ==(const VSString &String1,const VSString &String2);
	VSDATESTRUCT_API bool operator ==(const VSString &String1,const TCHAR *String2);
	VSDATESTRUCT_API bool operator ==(const TCHAR *String1,VSString &String2);
	//����!=������
	VSDATESTRUCT_API bool operator !=(const VSString &String1,const VSString &String2);
	VSDATESTRUCT_API bool operator !=(const VSString &String1,const TCHAR *String2);
	VSDATESTRUCT_API bool operator !=(const TCHAR *String1,const VSString &String2);

	VSDATESTRUCT_API int StringToInt(const VSString & String);
	VSDATESTRUCT_API VSString IntToString(int i);

	VSDATESTRUCT_API unsigned int StringToUInt(const VSString & String);
	VSDATESTRUCT_API VSString UIntToString(unsigned int i);

	VSDATESTRUCT_API bool StringToBool(const VSString & String);
	VSDATESTRUCT_API VSString BoolToString(bool i);
	VSDATESTRUCT_API VSString BoolToString(unsigned int i);

	VSDATESTRUCT_API VSREAL StringToReal(const VSString & String);
	VSDATESTRUCT_API VSString RealToString(VSREAL Value);


	class VSDATESTRUCT_API VSFileName : public VSString
	{
	public:
		VSFileName()
		{

		}
		VSFileName(const TCHAR * String):VSString(String)
		{
		}
		VSFileName(const VSString &String):VSString(String)
		{

		}
		~VSFileName()
		{

		}
		bool GetExtension(VSString & Extension);
		bool GetBaseName(VSString & BaseName);
		bool GetBaseNameAndExtension(VSString & BaseNameAndExtension);
		bool GetPath(VSString & Path);
		bool GetPathAndName(VSString & PathAndName);
	};
}
