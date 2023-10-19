#pragma once

#include "Client_Defines.h"

#include "Base.h"

BEGIN(Engine)
class CRenderer;
class CGameInstance;
END

BEGIN(Client)

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT Initialize();
	void Tick(_float fTimeDelta);
	HRESULT Render();
	HRESULT FinishTick();

private:
	CGameInstance*			m_pGameInstance = { nullptr };

	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

	CRenderer*				m_pRenderer = { nullptr };

#ifdef _DEBUG
	_float					m_fTimeAcc = 0.f;
	_uint					m_iNumDraw = 0;
	_tchar					m_szFPS[MAX_PATH] = TEXT("");

	class CImGui_Manager*  m_pImGui_Manager = { nullptr };
#endif // _DEBUG

private:
	HRESULT Open_Level(LEVEL_ID eLEVEL_ID);

	/* 모든 레벨에서 사용되는 컴포넌트 원형을 생성한다. */
	HRESULT Ready_Prototype_Components(); 


public:
	static CMainApp* Create();
	virtual void Free() override;
};

END

