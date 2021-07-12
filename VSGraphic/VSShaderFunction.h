#pragma once
#include "VSObject.h"
#include "VSPutNode.h"
#include "VSName.h"
#include "VSBitArray.h"
#include "VSShaderKey.h"
namespace VSEngine2
{
	class VSMaterial;
	class VSStream;
	class VSShaderMainFunction;
	class VSGRAPHIC_API VSShaderFunction : public VSObject
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY;
	public:
		virtual ~VSShaderFunction() = 0;
	protected:
		VSUsedName m_ShowName;
		VSShaderFunction(const VSUsedName & ShowName,VSMaterial * pMaterial);
		VSShaderFunction();
		VSArray<VSInputNode *> m_pInput;
		VSArray<VSOutputNode *> m_pOutput;
		VSMaterial * m_pOwner;
		bool m_bIsVisited;
		inline void SetOwner(VSMaterial * pOwner)
		{
			VSMAC_ASSERT(pOwner);
			m_pOwner = pOwner;
		}

	public:
		friend class VSMaterial;
		virtual bool IsHaveOutput()
		{
			if (m_pOutput.GetNum() > 0)
			{
				return m_pOutput[0]->IsConnection();
			}
			return false;
		}
		virtual void ResetInShaderName(MaterialShaderPara &MSPara) = 0;
		virtual bool GetInputValueString(VSString &OutString, MaterialShaderPara &MSPara)const;
		virtual bool GetOutputValueString(VSString &OutString, MaterialShaderPara &MSPara)const;
		virtual bool GetFunctionString(VSString &OutString, MaterialShaderPara &MSPara)const = 0;
		bool GetShaderTreeString(VSString &OutString, MaterialShaderPara &MSPara);
		bool ClearShaderTreeStringFlag();
		

		VSInputNode * GetInputNode(unsigned int uiNodeID)const;
		VSInputNode * GetInputNode(const VSString & NodeName)const;
		inline unsigned int GetInputNodeNum()const
		{
			return m_pInput.GetNum();
		}
		VSOutputNode * GetOutputNode(unsigned int uiNodeID)const;
		VSOutputNode * GetOutputNode(const VSString & NodeName)const;
		inline unsigned int GetOutputNodeNum()const
		{
			return m_pOutput.GetNum();
		}
		inline const VSUsedName & GetShowName()const
		{
			return m_ShowName;
		}

		VSString GetValueEqualString(const VSOutputNode * pOutputNode,const VSInputNode * pInputNode)const;


		virtual bool IsValidNodeToThis(VSShaderFunction * pShaderFunction, MaterialShaderPara &MSPara);

		virtual bool CheckChildNodeValidToThis(VSArray<VSShaderFunction *> & NoValidShaderFunctionArray, MaterialShaderPara &MSPara);

		virtual bool GetAllChildNode(VSArray<VSShaderFunction *> & ChildNodeArray, MaterialShaderPara &MSPara);

		virtual bool CheckChildNodeValidAll(VSMap<VSShaderFunction *, VSArray<VSShaderFunction *>> & NoValidMap, MaterialShaderPara &MSPara);

		virtual bool HaveThisChild(VSShaderFunction * pShaderFunction, MaterialShaderPara &MSPara);

		virtual void GetNoLightFunctionParentNode(VSArray<VSShaderFunction *> & ChildNodeArray, MaterialShaderPara &MSPara);

		virtual bool ResetValueType(MaterialShaderPara &MSPara);


	};
	DECLARE_Ptr(VSShaderFunction);
	VSTYPE_MARCO(VSShaderFunction);
}
