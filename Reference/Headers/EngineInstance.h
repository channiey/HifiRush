#pragma once

#include "Component_Manager.h"

#include "Input_Device.h"
#include "PipeLine.h"

#include "Util_File.h"
#include "Util_String.h"

BEGIN(Engine)

class ENGINE_DLL CEngineInstance final : public CBase
{
	DECLARE_SINGLETON(CEngineInstance)
private:
	CEngineInstance();
	virtual ~CEngineInstance() = default;

public: 
	HRESULT Initialize_Engine(_uint iNumLevels, HINSTANCE hInst, const GRAPHIC_DESC& GraphicDesc, _Inout_ ID3D11Device** ppDevice, _Inout_ ID3D11DeviceContext** ppContext, const char* strSoundFilePath);
	void	Tick(_double fTimeDelta);
	void	LateTick(_double fTimeDelta);
	void	FinishTick();
	void	Clear(_uint iLevelIndex);

public:
	ID3D11Device* Get_Device() const { return m_pDevice; }
	ID3D11DeviceContext* Get_Context() const { return m_pContext; }

public: /* For.Timer_Manager */
	_double	Compute_TimeDelta(const wstring& strTimerTag);
	HRESULT	Add_Timer(const wstring& strTimerTag);

public: /* For.Graphic_Device */
	HRESULT			Clear_BackBuffer_View(_float4 vClearColor);	
	HRESULT			Clear_DepthStencil_View();	
	HRESULT			Present();
	ID3D11Device*	Get_Device();
	GRAPHIC_DESC	Get_GraphicDesc();
	const Viewport	Get_ViewPort();

public: /* For.Level_Manager */
	HRESULT			Open_Level(_uint iLevelIndex, class CLevel* pNewLevel);
	const _uint		Get_CurLevelIndex();
	const _uint Get_CurLoadingLevel();
	void Set_CurLoadingLevel(const _uint iLevel);



public: /* For.Object_Manager */
	map<const wstring, class CGameObject*>*	Get_Prototypes();
	map<const wstring, class CLayer*>* Get_All_Layer(_uint iLevelIndex);
	map<const wstring, class CLayer*>* Get_All_Layer_CurLevel();
	list<class CGameObject*>* Get_Layer(_uint iLevelIndex, const wstring& strLayerTag);
	class CLayer* Get_LayerClass(_uint iLevelIndex, const wstring& strLayerTag);
	class CGameObject* Get_Player();
	class CGameObject* Get_GameObject(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag);
	class CGameObject* Get_GameObject_InCurLevel(const wstring& strLayerTag, const wstring& strPrototypeTag);
	class CGameObject* Get_GameObject_InCurLevel_InLayerFirst(const wstring& strLayerTag);
	const _int Get_ObjectIndex(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strName);

	HRESULT	Add_Prototype(const wstring& strPrototypeTag, class CGameObject* pPrototype);
	class CGameObject* Add_GameObject(const _uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, void* pArg = nullptr);
	HRESULT Delete_GameObject(const _uint iLevelIndex, class CGameObject* pObj);

	HRESULT Reserve_Pool(const _uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, const _uint& iNumObj, void* pArg = nullptr);
	class CGameObject* Pop_Pool(const _uint iLevelIndex, const wstring& strPrototypeTag);
	HRESULT Return_Pool(const _uint iLevelIndex, class CGameObject* pObj);

	HRESULT Add_Layer(_uint iLevelIndex, const wstring& strLayerTag);
	HRESULT Delete_Layer(_uint iLevelIndex, const wstring& strLayerTag);

public: /* For.Component_Manager */
	HRESULT				Add_PrototypeCom(_uint iLevelIndex, const wstring& strPrototypeTag, class CComponent* pPrototype);
	HRESULT				Bind_Add_PrototypeCom(function<_bool(_uint iLevelIndex, const wstring& strPrototypeTag, class CComponent* pPrototype)>& other);
	class CComponent*	Clone_Component(_uint iLevelIndex, const wstring& strPrototypeTag, void* pArg = nullptr);

public: /* For.Profiler_Manager */
	const PROFILER_DESC Get_ProfillingData() const;

public:  /* For.Input_Device */
	_char		Get_DIKState(_uchar eKeyID);
	_char		Get_DIMKeyState(CInput_Device::MOUSEKEYSTATE eMouseKeyID);
	_long		Get_DIMMoveState(CInput_Device::MOUSEMOVESTATE eMouseMoveID);
	const _bool Key_Up(const _int& _iKey);
	const _bool Key_Down(const _int& _iKey);
	const _bool Key_Pressing(const _int& _iKey);
	//const _bool Get_PickPos_Window(_Inout_ Vec2* vPickPos);
	const _bool Is_Focus();

public: /* For.PipeLine */
	HRESULT Bind_TransformToShader(class CShader* pShader, const char* pConstantName, const Matrix& matTransform);
	HRESULT Bind_TransformToShader(class CShader* pShader, const char* pConstantName, CPipeLine::TRANSFORM_STATE eState);
	Matrix	Get_Transform(const CPipeLine::TRANSFORM_STATE& eState) const;
	Matrix	Get_Transform_Inverse(const CPipeLine::TRANSFORM_STATE& eState) const;
	Vec4	Get_CamPosition() const;

public: /* For.Collision_Manager */
	void				Check_Collision_Layer(const wstring& strLayerTag1, const wstring& strLayerTag2, const CCollider::TYPE& eType1, const CCollider::TYPE& eType2);
	RAYHIT_DESC			Check_ScreenRay(const wstring& strLayerTag, const _bool& bSnap = FALSE);
	CCell*				Check_ScreenRay();
	const Ray			Create_ScreenRayLocal(Matrix matWorld);

public: /* For.Camera_Manager */
	CGameObject*	Get_Camera(const _uint& iKey);
	CGameObject*	Get_CurCamera();
	HRESULT			Set_CurCamera(const _uint& iKey);
	HRESULT			Add_Camera(const _uint& iKey, CGameObject* pCamera);
	Vec4			Get_CurCamera_State(const _uint iState);
	HRESULT			Change_Camera(const _uint& iKey);
	HRESULT			Change_Camera_Inverse();
	const map<_uint, CGameObject*>* Get_Cameras();

public: /* For.Sound_Manager */
	void			Play_Sound(_uint eSoundID, _uint eChannelID, float fVolume);
	HRESULT			Register_BGM(_uint eSoundID, _uint eChannelID, float fVolume);
	HRESULT			Play_BGM();
	const _uint		Get_BPM();
	const _bool		Is_PlayBGM();
	void			Lerp_BGMSound(const _float& fTargetValue, const _double& fTime, const LERP_MODE& eMode = LERP_MODE::DEFAULT);

public: /* For.Light_Manager */
	const LIGHT_DESC*	Get_LightDesc(_uint iLightIndex);
	HRESULT				Add_Light(const LIGHT_DESC& LightDesc);

public: /* For.Target_Manager */
	const _bool& Is_DebugRTV() const;
	void Set_DebugRTV(const _bool& bSet);
	void Set_DebugRTV();


private:
	class CTimer_Manager*			m_pTimer_Manager		= { nullptr };
	class CInput_Device*			m_pInput_Device			= { nullptr };
	class CSound_Manager*			m_pSound_Manager		= { nullptr };
	
	class CGraphic_Device*			m_pGraphic_Device		= { nullptr };
	class CPipeLine*				m_pPipeLine				= { nullptr };
	class CLight_Manager*			m_pLight_Manager		= { nullptr };
	class CTarget_Manager*			m_pTarget_Manager		= { nullptr };


	class CLevel_Manager*			m_pLevel_Manager		= { nullptr };
	class CObject_Manager*			m_pObject_Manager		= { nullptr };
	class CCamera_Manager*			m_pCamera_Manager		= { nullptr };
	class CComponent_Manager*		m_pComponent_Manager	= { nullptr };

	class CCollision_Manager*		m_pCollision_Manager	= { nullptr };
	class CNavMesh*					m_pNavMesh				= { nullptr };
	class CProfiler_Manager*		m_pProfiler_Manager		= { nullptr };

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

public:
	static void Release_Engine();
	virtual void Free() override;
};

END