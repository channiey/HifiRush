#pragma once

#include "Component_Manager.h"

#include "Thread_Manager.h"

/* 클라이언트개발자가 엔진의 기능을 이용하고자할 때 접촉하는 객체.  */
/* 클라이언트에 보여줘야할 함수들을 모두 정의하고 있는다. */

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public: /* For.GameInstance */
	HRESULT Initialize_Engine(_uint iNumLevels, const GRAPHIC_DESC& GraphicDesc, _Inout_ ID3D11Device** ppDevice, _Inout_ ID3D11DeviceContext** ppContext);
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);
	void FinishTick();
	void Clear(_uint iLevelIndex);

public: /* For.Timer_Manager */
	_float Compute_TimeDelta(const wstring& strTimerTag);
	HRESULT	Add_Timer(const wstring& strTimerTag);

public: /* For.Graphic_Device */
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);	
	HRESULT Clear_DepthStencil_View();	
	HRESULT Present();

public: /* For.Level_Manager */
	HRESULT Open_Level(_uint iLevelIndex, class CLevel* pNewLevel);
	const _uint& Get_CurLevelIndex();

public: /* For.Object_Manager */

	map<const wstring, class CGameObject*>*		Get_Prototypes();
	map<const wstring, class CLayer*>*			Get_Layers(_uint iLevelIndex);
	list<class CGameObject*>*					Get_Objects(_uint iLevelIndex, const wstring& strLayerTag);
	class CGameObject*							Get_Player();

	HRESULT										Add_Prototype(const wstring& strPrototypeTag, class CGameObject* pPrototype);
	HRESULT										Add_GameObject(const _uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, void* pArg = nullptr);
	HRESULT										Delete_GameObject(const _uint iLevelIndex, class CGameObject* pObj);

	HRESULT										Reserve_Pool(const _uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, const _uint& iNumObj, void* pArg);
	class CGameObject*							Pop_Pool(const _uint iLevelIndex, const wstring& strPrototypeTag);
	HRESULT										Return_Pool(const _uint iLevelIndex, class CGameObject* pObj);

public: /* For.Component_Manager */
	HRESULT Add_Prototype(_uint iLevelIndex, const wstring& strPrototypeTag, class CComponent* pPrototype);
	class CComponent* Clone_Component(_uint iLevelIndex, const wstring& strPrototypeTag, void* pArg = nullptr);

public:	/* For.Thread_Manager */
	HRESULT	Set_MultiThreading(const _uint& iNumThread);
	void Finish_MultiThreading();
	template <class F, class... Args>
	HRESULT	Add_Command(F&& f, Args&&... args) { return (m_pThread_Manager->Add_Command(f, args...)); }

private:
	class CTimer_Manager*			m_pTimer_Manager = { nullptr };
	class CGraphic_Device*			m_pGraphic_Device = { nullptr };
	class CLevel_Manager*			m_pLevel_Manager = { nullptr };
	class CObject_Manager*			m_pObject_Manager = { nullptr };
	class CComponent_Manager*		m_pComponent_Manager = { nullptr };
	class CThread_Manager*			m_pThread_Manager = { nullptr };

public:
	static void Release_Engine();
	virtual void Free() override;
};

END