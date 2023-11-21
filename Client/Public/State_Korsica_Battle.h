#pragma once
#include "State_Korsica_Base.h"

BEGIN(Client)
class CState_Korsica_Battle final : public CState_Korsica_Base
{
	enum PROGRESS_ID { APPEAR, ATTACK, DISAPPEAR, PROGRESS_END };

private:
	CState_Korsica_Battle();
	CState_Korsica_Battle(const CState_Korsica_Battle& rhs);
	virtual ~CState_Korsica_Battle() = default;

public:
	virtual HRESULT	Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);

public:
	HRESULT			Enter() override;
	const wstring	Tick(const _double& fTimeDelta) override;
	const wstring	LateTick() override;
	void			Exit() override;

protected:
	virtual void	Check_Progress(const _double& fTimeDelta) override;
	virtual void	Set_Transform() override;
	virtual void	Play_Sound() override;

private:
	HRESULT			Attack();

private:
	const wstring	Check_Transition() override;

private:
	PROGRESS_ID		m_eProgressID = PROGRESS_ID::PROGRESS_END;
	_bool			m_bAtttack = FALSE;

public:
	static CState_Korsica_Battle* Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};
END
