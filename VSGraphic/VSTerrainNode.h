#pragma once
#include "VSMeshNode.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSTerrainNode : public VSMeshNode
	{
		//RTTI
		DECLARE_RTTI;

		DECLARE_INITIAL_NO_CLASS_FACTORY;
	public:
		enum
		{
			MAX_TESSELLATION_LEVEL = 8,
			MIN_TESSELLATION_LEVEL = 1,
			MAX_GPU_TESSELLATION_LEVEL = 6,
			MAX_NUM = 10,
			MIN_NUM = 1,
			WIDTH_SCALE = 50
		};

		virtual ~VSTerrainNode() = 0;
	protected:
		VSTerrainNode();
		unsigned int m_uiTessellationLevel;		//每个儿子层级，儿子几点个数和网格密度关系看GetTileNumX函数
		unsigned int m_uiNumX;					//网格密度
		unsigned int m_uiNumZ;
		unsigned int m_uiTotalNum;

		unsigned char *m_pHeight;
		VSREAL m_fHeightScale;
		virtual bool CreateChild() = 0;
		void SetNum(unsigned int uiNumX,unsigned int uiNumZ);
		void SetTessellationLevel(unsigned int uiTessellationLevel, unsigned int uiMaxTessellationLevel = MAX_TESSELLATION_LEVEL, 
			unsigned int uiMinTessellationLevel = MIN_TESSELLATION_LEVEL);
	public:
		inline unsigned int GetTessellationLevel()const { return m_uiTessellationLevel ;}
		inline unsigned int GetNumX()const{return m_uiNumX;}
		inline unsigned int GetNumZ()const{return m_uiNumZ;}
		inline unsigned int GetTileNumX()const {return 1<<(m_uiNumX - m_uiTessellationLevel);}
		inline unsigned int GetTileNumZ()const {return 1<<(m_uiNumZ - m_uiTessellationLevel);}
		inline unsigned int GetSizeX()const{ return (1 << m_uiNumX) + 1; }
		inline unsigned int GetSizeZ()const{ return (1 << m_uiNumZ) + 1; }
		
		
		virtual bool CreateTerrain(unsigned int uiNumX,unsigned int uiNumZ,unsigned int uiTessellationLevel);
		bool CreateTerrainFromHeightMap(const TCHAR *pFileName,unsigned int uiTessellationLevel,VSREAL fHeightScale);
		
		inline VSREAL GetHeightScale()const
		{
			return m_fHeightScale;
		}
		unsigned char GetRawHeight(unsigned int uiX,unsigned int uiZ);
		VSREAL GetHeight(unsigned int uiX,unsigned int uiZ);
	};
	DECLARE_Ptr(VSTerrainNode);
	VSTYPE_MARCO(VSTerrainNode);
}
