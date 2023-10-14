#pragma once

#include "Component_Manager.h"

#include "Input_Device.h"
#include "PipeLine.h"

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public: /* For.GameInstance */
	HRESULT Initialize_Engine(_uint iNumLevels, HINSTANCE hInst, const GRAPHIC_DESC& GraphicDesc, _Inout_ ID3D11Device** ppDevice, _Inout_ ID3D11DeviceContext** ppContext);
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);
	void FinishTick();
	void Clear(_uint iLevelIndex);

public: /* For.Timer_Manager */
	_float	Compute_TimeDelta(const wstring& strTimerTag);
	HRESULT	Add_Timer(const wstring& strTimerTag);

public: /* For.Graphic_Device */
	HRESULT			Clear_BackBuffer_View(_float4 vClearColor);	
	HRESULT			Clear_DepthStencil_View();	
	HRESULT			Present();
	ID3D11Device*	Get_Device();
	GRAPHIC_DESC	Get_GraphicDesc();
	const Viewport	Get_ViewPort();


public: /* For.Level_Manager */
	HRESULT		Open_Level(_uint iLevelIndex, class CLevel* pNewLevel);
	const _uint Get_CurLevelIndex();

public: /* For.Object_Manager */
	map<const wstring, class CGameObject*>*	Get_Prototypes();
	map<const wstring, class CLayer*>* Get_All_Layer(_uint iLevelIndex);
	map<const wstring, class CLayer*>* Get_All_Layer_CurLevel();
	list<class CGameObject*>* Get_Layer(_uint iLevelIndex, const wstring& strLayerTag);
	class CLayer* Get_LayerClass(_uint iLevelIndex, const wstring& strLayerTag);
	class CGameObject* Get_Player();
	class CGameObject* Get_GameObject(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag);
	const _int Get_ObjectIndex(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strName);

	HRESULT	Add_Prototype(const wstring& strPrototypeTag, class CGameObject* pPrototype);
	class CGameObject* Add_GameObject(const _uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, void* pArg = nullptr);
	HRESULT Delete_GameObject(const _uint iLevelIndex, class CGameObject* pObj);

	HRESULT Reserve_Pool(const _uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, const _uint& iNumObj, void* pArg);
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
	void		Check_Collision_Layer(const wstring& strLayerTag1, const wstring& strLayerTag2, const CCollider::TYPE& eType1, const CCollider::TYPE& eType2);
	const _bool Check_Collision_Ray(Ray& ray, class CCollider* pCollider, OUT RAYHIT_DESC& hitDesc);
	const _bool Check_Collision_PickingRay(class CCollider* pCollider, const Matrix& matWorld, OUT RAYHIT_DESC& hitDesc);
	const _bool Check_Collision_PickingRay(class CModel* pModel, const Matrix& matWorld, OUT RAYHIT_DESC& hitDesc, const _bool& bPreInterSphere);
	Ray			Create_PickingRay(const Matrix& matWorld);

private:
	class CTimer_Manager*			m_pTimer_Manager = { nullptr };
	class CGraphic_Device*			m_pGraphic_Device = { nullptr };
	class CLevel_Manager*			m_pLevel_Manager = { nullptr };
	class CObject_Manager*			m_pObject_Manager = { nullptr };
	class CComponent_Manager*		m_pComponent_Manager = { nullptr };
	class CProfiler_Manager*		m_pProfiler_Manager = { nullptr };
	class CInput_Device*			m_pInput_Device = { nullptr };
	class CPipeLine*				m_pPipeLine = { nullptr };
	class CCollision_Manager*		m_pCollision_Manager = { nullptr };

public:
	static void Release_Engine();
	virtual void Free() override;
};

END