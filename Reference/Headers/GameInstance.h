#pragma once

#include "Component_Manager.h"

#include "Thread_Manager.h"

/* Ŭ���̾�Ʈ�����ڰ� ������ ����� �̿��ϰ����� �� �����ϴ� ��ü.  */
/* Ŭ���̾�Ʈ�� ��������� �Լ����� ��� �����ϰ� �ִ´�. */

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

public: /* For.Object_Manager */
	HRESULT Add_Prototype(const wstring& strPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_GameObject(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, void* pArg = nullptr);

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