#define COMMON_POST_EFFECT_FUNCTION_DECLARE_BEGIN(PostEffectName) \
	DECLARE_Ptr(VSPE##PostEffectName); \
	VSTYPE_MARCO(VSPE##PostEffectName); \
	class VSPE##PostEffectName : public VSPostEffectFunction \
	{\
		DECLARE_RTTI \
		DECLARE_INITIAL \
	protected: \
		VSPE##PostEffectName(); \
	public: \
		VSPE##PostEffectName(const VSUsedName & ShowName, VSPostEffectSet * pPostEffectSet); \
		~VSPE##PostEffectName(); \
		virtual VSPostEffectSceneRender * CreateSceneRender(); \
		virtual void OnDraw(VSCuller & Culler, double dAppTime); \
		enum \
		{ \
			INPUT_COLOR \
		}; \
		enum \
		{ \
			OUT_COLOR \
		};
#define COMMON_POST_EFFECT_FUNCTION_DECLARE_END	};
	
#define COMMON_POST_EFFECT_FUNCTION_IMPLEMENT_MIDDLE(PostEffectName) \
	END_ADD_PROPERTY \
	IMPLEMENT_INITIAL_BEGIN(VSPE##PostEffectName) \
	IMPLEMENT_INITIAL_END \
	VSPE##PostEffectName::VSPE##PostEffectName(const VSUsedName & ShowName, VSPostEffectSet * pPostEffectSet) \
	:VSPostEffectFunction(ShowName, pPostEffectSet) \
	{ \
		VSInputNode * pInputNode = NULL; \
		pInputNode = VS_NEW VSInputNode(VSPutNode::PET_IN, _T("InputColor"), this); \
		VSMAC_ASSERT(pInputNode); \
		m_pInput.AddElement(pInputNode); \
		VSOutputNode * pOutNode = NULL; \
		pOutNode = VS_NEW VSOutputNode(VSPutNode::PET_OUT, _T("OutColor"), this); \
		VSMAC_ASSERT(pOutNode); \
		m_pOutput.AddElement(pOutNode); \
	} \
	VSPE##PostEffectName::~VSPE##PostEffectName() \
	{ \
	} \
	VSPE##PostEffectName::VSPE##PostEffectName() \
	{ \
	} \
	VSPostEffectSceneRender * VSPE##PostEffectName::CreateSceneRender() \
	{ \
		if (!m_pPostEffectRender) \
		{ \
			m_pPostEffectRender = VS_NEW VSPE##PostEffectName##SceneRender(); \
		} \
		return m_pPostEffectRender; \
	} \
	void VSPE##PostEffectName::OnDraw(VSCuller & Culler, double dAppTime) \
	{ \
		VSRenderTarget * pTarget = GetRenderTarget(m_pInput[INPUT_COLOR]); \
		VSPE##PostEffectName##SceneRender* pTemp = (VSPE##PostEffectName##SceneRender*)m_pPostEffectRender.GetObject(); \
		pTemp->SetSourceTarget((VSTexture *)pTarget->GetCreateBy());
#define COMMON_POST_EFFECT_FUNCTION_IMPLEMENT_END \
		m_pPostEffectRender->Draw(Culler, dAppTime); \
	}
#define COMMON_POST_EFFECT_FUNCTION_IMPLEMENT(PostEffectName) \
	IMPLEMENT_RTTI(VSPE##PostEffectName, VSPostEffectFunction) \
	BEGIN_ADD_PROPERTY(VSPE##PostEffectName, VSPostEffectFunction) \
	COMMON_POST_EFFECT_FUNCTION_IMPLEMENT_MIDDLE(PostEffectName) \
	COMMON_POST_EFFECT_FUNCTION_IMPLEMENT_END	

#define COMMON_POST_EFFECT_FUNCTION_IMPLEMENT_ONE(PostEffectName,ValueName) \
	IMPLEMENT_RTTI(VSPE##PostEffectName, VSPostEffectFunction) \
	BEGIN_ADD_PROPERTY(VSPE##PostEffectName, VSPostEffectFunction) \
	REGISTER_PROPERTY(ValueName,ValueName,VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME) \
	COMMON_POST_EFFECT_FUNCTION_IMPLEMENT_MIDDLE(PostEffectName) \
		pTemp->ValueName = ValueName;\
	COMMON_POST_EFFECT_FUNCTION_IMPLEMENT_END

#define COMMON_POST_EFFECT_FUNCTION_IMPLEMENT_TWO(PostEffectName,ValueName1,ValueName2) \
	IMPLEMENT_RTTI(VSPE##PostEffectName, VSPostEffectFunction) \
	BEGIN_ADD_PROPERTY(VSPE##PostEffectName, VSPostEffectFunction) \
	REGISTER_PROPERTY(ValueName1,ValueName1,VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME) \
	REGISTER_PROPERTY(ValueName2,ValueName2,VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME) \
	COMMON_POST_EFFECT_FUNCTION_IMPLEMENT_MIDDLE(PostEffectName) \
		pTemp->ValueName1 = ValueName1;\
		pTemp->ValueName2 = ValueName2;\
	COMMON_POST_EFFECT_FUNCTION_IMPLEMENT_END

#define COMMON_POST_EFFECT_FUNCTION_IMPLEMENT_THREE(PostEffectName,ValueName1,ValueName2,ValueName3) \
	IMPLEMENT_RTTI(VSPE##PostEffectName, VSPostEffectFunction) \
	BEGIN_ADD_PROPERTY(VSPE##PostEffectName, VSPostEffectFunction) \
	REGISTER_PROPERTY(ValueName1,ValueName1,VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME) \
	REGISTER_PROPERTY(ValueName2,ValueName2,VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME) \
	REGISTER_PROPERTY(ValueName3,ValueName3,VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME) \
	COMMON_POST_EFFECT_FUNCTION_IMPLEMENT_MIDDLE(PostEffectName) \
		pTemp->ValueName1 = ValueName1;\
		pTemp->ValueName2 = ValueName2;\
		pTemp->ValueName3 = ValueName3;\
	COMMON_POST_EFFECT_FUNCTION_IMPLEMENT_END



		