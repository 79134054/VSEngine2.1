#pragma once
#include "VSSystem.h"
#include "VSGraphic.h"
namespace VSEngine2
{
	class VSGRAPHIC_API VSTextureOutputInfo
	{
	public:
		~VSTextureOutputInfo() {};
		inline unsigned int GetWidth()const
		{
			return m_uiWidth;
		}
		inline unsigned int GetHeight()const
		{

			return m_uiHeight;
		}
		inline unsigned int GetFormatType()const
		{
			return m_uiFormatType;
		}
		inline unsigned int GetMulSample()const
		{
			return m_uiMulSample;
		}
		inline unsigned int GetLevel()const
		{
			return m_uiLevel;
		}
		inline unsigned int GetFirst()const
		{
			return m_uiFirst;
		}
		inline unsigned int GetArraySize()const
		{
			return m_uiArraySize;
		}
	protected:
		VSTextureOutputInfo() {};
		unsigned int m_uiWidth;
		unsigned int m_uiHeight;
		unsigned int m_uiFormatType;
		unsigned int m_uiMulSample;
		unsigned int m_uiLevel;
		unsigned int m_uiFirst;
		unsigned int m_uiArraySize;
	};
}

