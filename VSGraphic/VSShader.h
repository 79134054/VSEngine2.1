#pragma once
#include "VSObject.h"
#include "VSUserConstant.h"
#include "VSController.h"
#include "VSUserSampler.h"
#include "VSUserBuffer.h"
#include "VSResourceManager.h"
#include "VSBind.h"
#include "VSResource.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSShader : public VSBind
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY
	public:
		VSShader(const TCHAR * pBuffer,const VSString & MainFunName,bool IsFromFile = false);
		VSShader(const VSString &Buffer,const VSString & MainFunName,bool IsFromFile = false);
		VSShader();
		virtual ~VSShader() = 0;
		virtual unsigned int GetShaderType()const = 0;
	public:
		virtual bool SetParam(const VSUsedName &Name,void * pData);
		virtual bool SetParam(const VSUsedName &Name,VSTexAllState * pTexture,unsigned int uiIndex = 0);
		virtual bool SetParam(const VSUsedName &Name, VSBufferResource * pBufferResource, unsigned int uiIndex = 0);
		inline void ClearBuffer()
		{
			m_Buffer.Clear();
		}
		bool HasBindResource();
		
		inline const VSString & GetBuffer()const{ return m_Buffer;}
		inline const void *GetCacheBuffer()const
		{
			return m_pCacheBuffer;
		}
		inline unsigned int GetCacheBufferSize()const
		{
			return m_uiCacheBufferSize;
		}
		bool SetCacheBuffer(void * pBuffer, unsigned int uiSize);
		const VSString &GetMainFunName()const
		{
			return m_MainFunName;
		}
		unsigned int m_uiConstBufferSize;

		inline const VSUsedName & GetFileName()const
		{
			return m_FileName;
		}

		inline void SetFileName(const VSUsedName & FileName)
		{
			m_FileName = FileName;
		}
	protected:
		VSString m_Buffer;
		unsigned char * m_pCacheBuffer;
		unsigned int m_uiCacheBufferSize;
		VSString	m_MainFunName;
		VSUsedName	m_FileName;
	public:	
		void SetShaderString(const TCHAR * pBuffer,const VSString & MainFunName,bool IsFromFile = false);
		void SetShaderString(const VSString &Buffer,const VSString & MainFunName,bool IsFromFile = false);
		inline unsigned int GetMinShareSampler()const
		{
			return m_uiShareSamplerRegister.GetNum() > 0 ? m_uiShareSamplerRegister[0].Key : INVALID_SIMPLAR_REGISTER;
		}
		inline unsigned int GetMaxShareSampler()const
		{
			return m_uiShareSamplerRegister.GetNum() > 0 ? m_uiShareSamplerRegister[m_uiShareSamplerRegister.GetNum() - 1].Key : INVALID_SIMPLAR_REGISTER;
		}
		void AddShareSampler(unsigned int ShareSamplerRegister, unsigned int ShareSamplerType);
	public:
		VSArray<VSUserConstantPtr>		m_pUserConstant;
		VSArray<VSUserSamplerPtr>		m_pUserSampler;
		VSArray<VSUserBufferPtr>		m_pUserBuffer;
		VSShaderKey						m_ShaderKey;
		bool		m_bCreatePara;
		unsigned int m_uiArithmeticInstructionSlots;
		unsigned int m_uiTextureInstructionSlots;
		VSMapOrder<unsigned int, unsigned int> m_uiShareSamplerRegister;
	};
	DECLARE_Ptr(VSShader);
	VSTYPE_MARCO(VSShader);

}
