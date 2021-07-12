#define COMMON_POST_EFFECT_SCENE_RENDER_DECLARE_BEGIN(PostEffectName,PostEffectFunction) \
	DECLARE_Ptr(VSPE##PostEffectName##SceneRender);\
	class VSGRAPHIC_API VSPE##PostEffectName##SceneRender : public VSCommonPESceneRender \
	{ \
	protected: \
		virtual void OnUpdate(double dAppTime); \
	public: \
		VSPE##PostEffectName##SceneRender::VSPE##PostEffectName##SceneRender() \
		{ \
			m_pCustomMaterial = VSCustomMaterial::##PostEffectFunction##(); \
		}

#define POST_EFFECT_SCENE_RENDER_END \
	}; 
	
#define COMMON_POST_EFFECT_SCENE_RENDER_IMPLEMENT_BEGIN(PostEffectName) \
	void VSPE##PostEffectName##SceneRender::OnUpdate(double dAppTime) \
	{ \
		VSCommonPESceneRender::OnUpdate(dAppTime);

#define POST_EFFECT_SCENE_RENDER_SCALE(PostEffectName,ParentSceneRender,Scale) \
	DECLARE_Ptr(VSPE##PostEffectName##SceneRender);\
	class VSGRAPHIC_API VSPE##PostEffectName##SceneRender : public ParentSceneRender \
	{ \
	public: \
		VSPE##PostEffectName##SceneRender::VSPE##PostEffectName##SceneRender() \
		{ \
			m_fGetRTScale = Scale;\
		}



#define COMMON_POST_EFFECT_SCENE_RENDER_IMPLEMENT_VALUE(ValueName) \
	static VSUsedName Temp##ValueName = _T(#ValueName); \
	m_pCustomMaterial->SetPShaderValue(Temp##ValueName,&ValueName);

#define GET_COMMON_POST_EFFECT_SCENE_RENDER(PESceneRender,i) \
	PESceneRender * Get##PESceneRender##()const \
	{ \
		return (PESceneRender *)(m_CPEArray[i].GetObject()); \
	}

#define COMBINE_POST_EFFECT_SCENE_RENDER_DECLARE_TWO_BEGIN(PostEffectName,PESceneRender1,PESceneRender2) \
	DECLARE_Ptr(VSPE##PostEffectName##SceneRender); \
	class VSGRAPHIC_API VSPE##PostEffectName##SceneRender : public VSCombinePostEffectSceneRender \
	{ \
	public: \
		virtual void OnUpdate(unsigned int uiPassID,double dAppTime); \
		VSPE##PostEffectName##SceneRender() \
			:VSCombinePostEffectSceneRender(2) \
		{ \
			m_CPEArray[0] = VS_NEW PESceneRender1(); \
			m_CPEArray[1] = VS_NEW PESceneRender2(); \
		} \
		GET_COMMON_POST_EFFECT_SCENE_RENDER(PESceneRender1,0) \
		GET_COMMON_POST_EFFECT_SCENE_RENDER(PESceneRender2,1)

#define COMBINE_POST_EFFECT_SCENE_RENDER_DECLARE_THREE_BEGIN(PostEffectName,PESceneRender1,PESceneRender2,PESceneRender3) \
	DECLARE_Ptr(VSPE##PostEffectName##SceneRender); \
	class VSGRAPHIC_API VSPE##PostEffectName##SceneRender : public VSCombinePostEffectSceneRender \
	{ \
	public: \
		virtual void OnUpdate(unsigned int uiPassID,double dAppTime); \
		VSPE##PostEffectName##SceneRender() \
			:VSCombinePostEffectSceneRender(3) \
		{ \
			m_CPEArray[0] = VS_NEW PESceneRender1(); \
			m_CPEArray[1] = VS_NEW PESceneRender2(); \
			m_CPEArray[2] = VS_NEW PESceneRender3(); \
		} \
		GET_COMMON_POST_EFFECT_SCENE_RENDER(PESceneRender1,0) \
		GET_COMMON_POST_EFFECT_SCENE_RENDER(PESceneRender2,1) \
		GET_COMMON_POST_EFFECT_SCENE_RENDER(PESceneRender3,2)

#define COMBINE_POST_EFFECT_SCENE_RENDER_DECLARE_FOUR_BEGIN(PostEffectName,PESceneRender1,PESceneRender2,PESceneRender3,PESceneRender4) \
	DECLARE_Ptr(VSPE##PostEffectName##SceneRender); \
	class VSGRAPHIC_API VSPE##PostEffectName##SceneRender : public VSCombinePostEffectSceneRender \
	{ \
	public: \
		virtual void OnUpdate(unsigned int uiPassID,double dAppTime); \
		VSPE##PostEffectName##SceneRender() \
			:VSCombinePostEffectSceneRender(4) \
		{ \
			m_CPEArray[0] = VS_NEW PESceneRender1(); \
			m_CPEArray[1] = VS_NEW PESceneRender2(); \
			m_CPEArray[2] = VS_NEW PESceneRender3(); \
			m_CPEArray[3] = VS_NEW PESceneRender4(); \
		} \
		GET_COMMON_POST_EFFECT_SCENE_RENDER(PESceneRender1,0) \
		GET_COMMON_POST_EFFECT_SCENE_RENDER(PESceneRender2,1) \
		GET_COMMON_POST_EFFECT_SCENE_RENDER(PESceneRender3,2) \
		GET_COMMON_POST_EFFECT_SCENE_RENDER(PESceneRender4,3)

#define COMBINE_POST_EFFECT_SCENE_RENDER_DECLARE_FIVE_BEGIN(PostEffectName,PESceneRender1,PESceneRender2,PESceneRender3,PESceneRender4,PESceneRender5) \
	DECLARE_Ptr(VSPE##PostEffectName##SceneRender); \
	class VSGRAPHIC_API VSPE##PostEffectName##SceneRender : public VSCombinePostEffectSceneRender \
	{ \
	public: \
		virtual void OnUpdate(unsigned int uiPassID,double dAppTime); \
		VSPE##PostEffectName##SceneRender() \
			:VSCombinePostEffectSceneRender(5) \
		{ \
			m_CPEArray[0] = VS_NEW PESceneRender1(); \
			m_CPEArray[1] = VS_NEW PESceneRender2(); \
			m_CPEArray[2] = VS_NEW PESceneRender3(); \
			m_CPEArray[3] = VS_NEW PESceneRender4(); \
			m_CPEArray[4] = VS_NEW PESceneRender5(); \
		} \
		GET_COMMON_POST_EFFECT_SCENE_RENDER(PESceneRender1,0) \
		GET_COMMON_POST_EFFECT_SCENE_RENDER(PESceneRender2,1) \
		GET_COMMON_POST_EFFECT_SCENE_RENDER(PESceneRender3,2) \
		GET_COMMON_POST_EFFECT_SCENE_RENDER(PESceneRender4,3) \
		GET_COMMON_POST_EFFECT_SCENE_RENDER(PESceneRender5,4)

#define COMBINE_POST_EFFECT_SCENE_RENDER_IMPLEMENT_BEGIN(PostEffectName) \
	void VSPE##PostEffectName##SceneRender::OnUpdate(unsigned int uiPassID,double dAppTime) \
	{\
		VSCombinePostEffectSceneRender::OnUpdate(uiPassID,dAppTime);