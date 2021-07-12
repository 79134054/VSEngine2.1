#pragma once
#include "VSGraphic.h"
#include "VSArray.h"
namespace VSEngine2
{
	class VSGRAPHIC_API VSSlot 
	{
	public:
		VSSlot();
		~VSSlot();
		
		VSArray<unsigned char> m_VSTexSlot;
		VSArray<unsigned char> m_PSTexSlot;
		VSArray<unsigned char> m_GSTexSlot;
		VSArray<unsigned char> m_DSTexSlot;
		VSArray<unsigned char> m_HSTexSlot;
		VSArray<unsigned char> m_CSTexSlot;
		void ClearAllSlot();
		bool HasAnySlot();
		bool m_bBindResourceUse;
		inline unsigned int GetOutputType()const
		{
			return m_uiOutputType;
		}
	protected:
		unsigned int m_uiOutputType;
	};
}