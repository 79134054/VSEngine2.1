#pragma once
#include "VSObject.h"
#include "VSMemBind.h"
#include "VSBindLockInterface.h"
#include "VSDataBuffer.h"
#include "VSMath.h"
#include "VSVertexFormat.h"
namespace VSEngine2
{
	class VSStream;
	class VSGRAPHIC_API VSVertexBuffer : public VSMemBind , public VSBindLockInterface
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		friend class VSVertexFormat;
		friend class VSResourceManager;
		VSVertexBuffer(bool bIsStatic);
		VSVertexBuffer(VSArray<VSVertexFormat::VERTEXFORMAT_TYPE>& FormatArray,unsigned int uiNum);
		virtual ~VSVertexBuffer();
		bool SetData(VSDataBuffer * pData,unsigned int uiVF);
		inline VSDataBuffer * GetData(unsigned int uiVF,unsigned int uiLevel)const;
		inline unsigned int GetLevel(unsigned int uiVF)const;

		inline VSDataBuffer * GetPositionData(unsigned int uiLevel)const;
		inline VSDataBuffer * GetNormalData(unsigned int uiLevel)const;
		inline VSDataBuffer * GetPSizeData()const;
		inline VSDataBuffer * GetColorData(unsigned int uiLevel)const;
		inline VSDataBuffer * GetBlendWeightData()const;
		inline VSDataBuffer * GetBlendIndicesData(unsigned int uiLevel)const;
		inline VSDataBuffer * GetTangentData()const;
		inline VSDataBuffer * GetBinormalData()const;
		inline VSDataBuffer * GetFogData()const;
		inline VSDataBuffer * GetVertexIDData()const;
		inline VSDataBuffer * GetTexCoordData(unsigned int uiLevel)const;
		inline VSDataBuffer * GetMaterialInstanceData(unsigned int uiLevel)const;

		inline unsigned int GetPositionLevel()const;
		inline unsigned int GetColorLevel()const;
		inline unsigned int GetNormalLevel()const;
		inline unsigned int GetTexCoordLevel()const;
		inline unsigned int GetMaterialInstanceLevel()const;
		inline unsigned int GetBlendIndicesLevel()const;

		inline unsigned int GetVertexNum()const;

		bool GetVertexFormat(VSArray<VSVertexFormat::VERTEXFORMAT_TYPE> &FormatArray);

		inline VSVertexFormat * GetVertexFormat()const;

		inline unsigned int GetOneVertexSize()const;

		inline bool GetLockDataOffset(unsigned int uiVF,unsigned int uiLevel,unsigned int &uiOffset)const;
		inline void *GetLockData(unsigned int uiVF,unsigned int uiLevel)const;

		inline void *GetLockPositionData(unsigned int uiLevel)const;
		inline void *GetLockNormalData(unsigned int uiLevel)const;
		inline void *GetLockPSizeData()const;
		inline void *GetLockColorData(unsigned int uiLevel)const;
		inline void *GetLockBlendWeightData()const;
		inline void *GetLockBlendIndicesData(unsigned int uiLevel)const;
		inline void *GetLockTangentData()const;
		inline void *GetLockBinormalData()const;
		inline void *GetLockFogData()const;
		inline void *GetLockVertexIDData()const;
		inline void *GetLockTexCoordData(unsigned int uiLevel)const;
		inline void *GetLockMaterialInstanceData(unsigned int uiLevel)const;
		
		virtual unsigned int GetByteSize()const;

		unsigned int GetSemanticsNum(unsigned int uiSemantics)const;

		inline bool HavePositionInfo(unsigned int uiLevel)const;
		inline bool HaveNormalInfo(unsigned int uiLevel)const;
		inline bool HavePSizeInfo()const;
		inline bool HaveColorInfo(unsigned int uiLevel)const;
		inline bool HaveBlendWeightInfo()const;
		inline bool HaveBlendIndicesInfo(unsigned int uiLevel)const;
		inline bool HaveTangentInfo()const;
		inline bool HaveBinormalInfo()const;
		inline bool HaveFogInfo()const;
		inline bool HaveVertexIDInfo()const;
		inline bool HaveTexCoordInfo(unsigned int uiLevel)const;
		inline bool HaveMaterialInstanceInfo(unsigned int uiLevel)const;

		unsigned int GetSemanticsChannel(unsigned int uiSemantics,unsigned int uiLevel)const;

		inline unsigned int PositionChannel(unsigned int uiLevel)const;
		inline unsigned int NormalChannel(unsigned int uiLevel)const;
		inline unsigned int PSizeChannel()const;
		inline unsigned int ColorChannel(unsigned int uiLevel)const;
		inline unsigned int BlendWeightChannel()const;
		inline unsigned int BlendIndicesChannel(unsigned int uiLevel)const;
		inline unsigned int TangentChannel()const;
		inline unsigned int BinormalChannel()const;
		inline unsigned int FogChannel()const;
		inline unsigned int VertexIDChannel()const;
		inline unsigned int TexCoordChannel(unsigned int uiLevel)const;
		inline unsigned int MaterialInstanceChannel(unsigned int uiLevel)const;

		unsigned int GetSemanticsDataType(unsigned int uiSemantics,unsigned int uiLevel)const;

		inline unsigned int NormalDataType(unsigned int uiLevel)const;
		inline unsigned int TangentDataType()const;
		inline unsigned int BinormalDataType()const;

	protected:
		friend class VSVertexFormat;
		friend class VSGeometry;
		VSVertexBuffer();
		VSArray<VSDataBufferPtr> m_pData[VSVertexFormat::VF_MAX];
		unsigned int m_uiVertexNum;
		unsigned int m_uiOneVertexSize;

		VSVertexFormatPtr m_pVertexFormat;
		
		void * m_pLockData;
		//都要重新实现
	public:
	

		virtual	bool LoadResource(VSRenderer * pRender);

		void *Lock();
		void UnLock();
		inline void * GetLockDataPtr()const
		{
			return m_pLockData;
		}
		virtual void ClearInfo();
	protected:
		virtual bool OnLoadResource(VSResourceIdentifier *&pID);		
	};
	DECLARE_Ptr(VSVertexBuffer);
	VSTYPE_MARCO(VSVertexBuffer);
	#include "VSVertexBuffer.inl"
	
}
