#pragma once
#include "VSPostEffectFunction.h"
namespace VSEngine2
{
	class VSPEBeginFunction : public VSPostEffectFunction
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
		
	public:
		enum
		{
			OUT_COLOR
		};
		VSPEBeginFunction(const VSUsedName & ShowName,VSPostEffectSet * pPostEffectSet);
		~VSPEBeginFunction();
		virtual VSRenderTarget * GetMainColorOutputRenderTarget()
		{
			if (m_pBeginTargetArray && m_pBeginTargetArray->GetNum() > 0)
			{
				return (*m_pBeginTargetArray)[0];
			}
			return NULL;
		}
		virtual VSRenderTarget * GetMainColorInputPutRenderTarget()
		{
			if (m_pBeginTargetArray && m_pBeginTargetArray->GetNum() > 0)
			{
				return (*m_pBeginTargetArray)[0];
			}
			return NULL;
		}
		virtual VSRenderTarget * GetRenderTarget(const VSOutputNode * pPutNode)
		{
			for (unsigned int i = 0 ; i < m_pOutput.GetNum() ;i++)
			{
				if (pPutNode == m_pOutput[i])
				{
					if (m_pBeginTargetArray && i < m_pBeginTargetArray->GetNum())
					{
						return (*m_pBeginTargetArray)[i];
					}
					else
					{
						return NULL;
					}
				}
			}
			return NULL;
		}
		inline void SetPara(VSArray<VSRenderTarget *> * pBeginTargetArray)
		{
			m_pBeginTargetArray = pBeginTargetArray;
		}
		virtual bool SetEndTarget(VSRenderTarget *pEndTarget)
		{
			return false;
		}
	protected:
		VSPEBeginFunction();
		VSArray<VSRenderTarget *> * m_pBeginTargetArray;
		virtual VSPostEffectSceneRender * CreateSceneRender();
		virtual void OnDraw(VSCuller & Culler,double dAppTime);
	};
	DECLARE_Ptr(VSPEBeginFunction);
	VSTYPE_MARCO(VSPEBeginFunction);
}
