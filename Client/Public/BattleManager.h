#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)

class CTriggerBattle;

class CBattleManager final : public CBase
{
	DECLARE_SINGLETON(CBattleManager)

private:
	CBattleManager();
	virtual ~CBattleManager() = default;

public:
	HRESULT			Initialize();
	HRESULT			Update(const _double fTimedelta);
	HRESULT			LateUpdate(const _double fTimedelta);

public:
	const _bool&	Is_Battle() const { return m_bBattle; }
	HRESULT			Add_Trigger(const wstring strTriggerTag, CTriggerBattle* pTrigger);

public:
	void			OnTrigger_Enter(const wstring strTriggerTag);
	void			OnTrigger_Stay(const wstring strTriggerTag);
	void			OnTrigger_Exit(const wstring strTriggerTag);

private:
	CTriggerBattle* Find_Trigger(const wstring strTriggerTag);
	HRESULT			Remove_Trigger(const wstring strTriggerTag);

private:
	HRESULT			Ready_Pool();

private:
	map<const wstring, CTriggerBattle*> m_BattelTriggers;
	CTriggerBattle*						m_pCurBattleTriggers = nullptr;
	_bool								m_bBattle = FALSE;

public:
	virtual void	Free() override;
};

END


