#pragma once
#include "VSObject.h"
#include "VSTransform.h"
#include "VSController.h"
#include "VSAABB3.h"
#include "VSResource.h"
namespace VSEngine2
{
class VSStream;
class VSCuller;
DECLARE_Ptr(VSMaterialInstance)
DECLARE_Ptr(VSLight);
DECLARE_Ptr(VSCamera);
DECLARE_Proxy(VSMaterial);
class VSGRAPHIC_API VSSpatial :public VSObject
{
	//RTTI
	DECLARE_RTTI;
	DECLARE_INITIAL_NO_CLASS_FACTORY;
public:
	virtual ~VSSpatial() = 0;

protected:

	VSAABB3 m_WorldBV;
	
public:
	virtual void ComputeVisibleSet(VSCuller & Culler,bool bNoCull,double dAppTime);

	enum //Cull Mode
	{
		CM_DYNAMIC,
		CM_ALAWAYS,
		CM_NEVER
	};


	virtual void UpdateView(VSCuller & Culler,double dAppTime);
	inline const VSAABB3 & GetWorldAABB()const;

	virtual void CreateLocalAABB(){}
	
protected:
	friend class VSNode;
	friend class VSSwitchNode;
	friend class VSScene;
	friend class VSGeometry;
	friend class VSMeshComponent;
	friend class VSSkeleton;
	friend class VSNodeComponent;
	friend class VSSkeletonMeshNode;
	bool		m_bIsVisibleUpdate;

	//bool m_bForceNoCull;
public:
	virtual void SetIsVisibleUpdate(bool bIsVisibleUpdate);

public:
	unsigned int m_uiCullMode;
protected:
	VSSpatial();

	virtual void UpdateWorldBound(double dAppTime) = 0;//更新边界体 
	virtual void UpdateNodeAll(double dAppTime) = 0;
	virtual void ComputeNodeVisibleSet(VSCuller & Culler,bool bNoCull,double dAppTime)= 0;
protected:
	VSTransform m_World;
	VSTransform m_Local;
	VSSpatial* m_pParent;
	VSArray<VSLight *> m_pAllLight;
	VSArray<VSCamera *> m_pAllCamera;
public:
	bool m_bInheritScale;
	bool m_bInheritRotate;
	bool m_bInheritTranslate;
	virtual void UpdateTransform(double dAppTime);//更新变换信息 
	virtual void UpdateAll(double dAppTime);
public:
	inline void SetParent(VSSpatial * pParent);
	inline VSSpatial *GetParent()const;
	inline const VSVector3 &GetLocalScale()const;
	inline const VSVector3 & GetLocalTranslate()const;
	inline const VSMatrix3X3 & GetLocalRotate()const;
	inline void GetLocalDir(VSVector3 &Dir, VSVector3 &Up, VSVector3 & Right)const;
	inline const VSTransform & GetLocalTransform();

	virtual const VSVector3 &GetWorldScale();
	virtual const VSVector3 & GetWorldTranslate();
	virtual const VSMatrix3X3 & GetWorldRotate();
	virtual void GetWorldDir(VSVector3 &Dir, VSVector3 &Up, VSVector3 & Right);
	virtual const VSTransform & GetWorldTransform();

	virtual void SetWorldScale(const VSVector3 & fScale);
	virtual void SetWorldTranslate(const VSVector3& Translate);
	virtual void SetWorldRotate(const VSMatrix3X3 & Rotate);
	virtual void SetWorldTransform(const VSTransform & LocalTransform);
	virtual void SetWorldMat(const VSMatrix3X3W VSMat);

	virtual void SetLocalScale(const VSVector3 & fScale);
	virtual void SetLocalTranslate(const VSVector3& Translate);
	virtual void SetLocalRotate(const VSMatrix3X3 & Rotate);
	virtual void SetLocalTransform(const VSTransform & LocalTransform);
	virtual void SetLocalMat(const VSMatrix3X3W VSMat);
public:
	VSArray<VSControllerPtr>	m_ControllerArray;
	inline unsigned int GetControllerNum()const { return m_ControllerArray.GetNum(); }
	virtual void UpdateController(double dAppTime);
	bool AddController(VSController * pController);
	VSController * DeleteController(unsigned int ID);
	unsigned int DeleteController(VSController *pController);
	VSController *GetController(unsigned int ID)const;
	void DeleteAllController();
	inline VSArray<VSPointer<VSController>> * GetControllerList();
public:
	inline void SetDynamic(bool bIsDynamic);
	inline bool IsDynamic()const { return !m_bIsStatic; }
	void ClearAllTime();
	protected:
		bool m_bIsStatic;
		bool m_bIsChanged;
	public:
		bool m_bEnable;

};
DECLARE_Ptr(VSSpatial);
VSTYPE_MARCO(VSSpatial);
}