#pragma once
#include "VSObject.h"
#include "VSName.h"
namespace VSEngine2
{
	class VSShaderFunction;
	class VSInputNode;
	class VSOutputNode;
	class VSStream;
	class VSGRAPHIC_API VSPutNode : public VSObject
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY;
	public:
		enum // Value Type
		{
			VT_1,
			VT_2,
			VT_3,
			VT_4,
			VT_MAX
		};
		enum		//ANIM VALUE TYPE
		{
			AVT_ANIM,
			AVT_IK,
			AVT_MAX
		};
		enum	//Post Effect Type
		{
			PET_OUT,
			PET_IN,
			PET_MAX
		};
		virtual ~VSPutNode() = 0;
	protected:
		VSPutNode(unsigned int uiValueType,const VSUsedName & NodeName,VSObject * pShaderFunction);
		VSPutNode();
		VSObject * m_pOwner;
		VSUsedName m_NodeName;
		unsigned int m_uiValueType;
		inline void SetOwner(VSObject *pOwner)
		{
			m_pOwner = pOwner;
		}
	public:
		friend class VSShaderFunction;
		friend class VSAnimBaseFunction;
		friend class VSPostEffectFunction;
	
		
		

		inline unsigned int GetValueType()const
		{
			return m_uiValueType;
		}
		inline void SetValueType(unsigned int uiValueType)
		{
			m_uiValueType = uiValueType;
		}
		inline const VSUsedName & GetNodeName()const
		{
			return m_NodeName;
		}
		inline void SetNodeName(const VSUsedName & NodeName)
		{
			m_NodeName = NodeName;
		}
		inline VSObject * GetOwner()const
		{
			return m_pOwner;
		}

	};
	DECLARE_Ptr(VSPutNode);
	VSTYPE_MARCO(VSPutNode);
	class VSGRAPHIC_API VSInputNode : public VSPutNode
	{
		//RTTI
		DECLARE_RTTI;

		DECLARE_INITIAL
	public:
		VSInputNode(unsigned int uiValueType,const VSUsedName &NodeName,VSObject * pObject);
		virtual ~VSInputNode();
	protected:
		VSInputNode();
		VSOutputNode * m_pOutputLink;
		

	public:
		friend class VSOutputNode;
		bool Connection(VSOutputNode * pOutputNode,bool bIsJudgeType = false);
		void UnConnection();
		inline const VSOutputNode *GetOutputLink()const
		{
			return m_pOutputLink;
		}
		bool IsConnection()
		{
			return m_pOutputLink != NULL;
		}
		enum //InputNodeIndex
		{
			INI_0 = 0,
			INI_A = INI_0,
			INI_TEXCOORD = INI_0,
			INI_VALUE = INI_0,
			INI_COLOR = INI_0,
			INI_1 = 1,
			INI_B = INI_1,
			INI_ALPHA = INI_1,
			INI_2 = 2,
			INI_C = INI_2,
			INI_3 = 3,
			INI_D = INI_3,
			INI_4 = 4,
			INI_E = INI_4,
			INI_MAX
		};
		
	};
	DECLARE_Ptr(VSInputNode);
	VSTYPE_MARCO(VSInputNode);
	class VSGRAPHIC_API VSOutputNode:public VSPutNode
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
		
	public:
		VSOutputNode(unsigned int uiValueType,const VSUsedName &NodeName,VSObject * pShaderFunction);
		virtual ~VSOutputNode();
	protected:
		VSOutputNode();
		VSArray<VSInputNode *> m_pInputLink;
		

	public:
		friend class VSInputNode;
		bool Connection(VSInputNode * pInputNode,bool bIsJudgeType = false);
		void UnConnection();
		bool IsConnection()
		{
			return m_pInputLink.GetNum() > 0;
		}
		VSInputNode * GetInputLink(unsigned int i)
		{
			return m_pInputLink[i];
		}
		unsigned int GetInputNum()const
		{
			return m_pInputLink.GetNum();
		}
		enum //OutputNodeIndex
		{
			ONI_OUT = 0,
			ONI_COLOR = ONI_OUT,
			ONI_VALUE = ONI_OUT,
			ONI_R = 1,
			ONI_X = ONI_R,
			ONI_G = 2,
			ONI_Y = ONI_G,
			ONI_B = 3,
			ONI_Z = ONI_B,
			ONI_A = 4,
			ONI_W = ONI_A,
			ONI_MAX
		};
	protected:
		bool CheckIsConnection(VSInputNode * pInputNode)const;
	
	};
	DECLARE_Ptr(VSOutputNode);
	VSTYPE_MARCO(VSOutputNode);
}
