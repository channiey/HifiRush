#pragma once
#include "State_Gunner_Base.h"

BEGIN(Client)
class CState_Gunner_Damaged final : public CState_Gunner_Base
{
private:
	CState_Gunner_Damaged();
	CState_Gunner_Damaged(const CState_Gunner_Damaged& rhs);
	virtual ~CState_Gunner_Damaged() = default;

public:
	virtual HRESULT Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);

public:
	HRESULT			Enter() override;
	const wstring	Tick(const _double& fTimeDelta) override;
	const wstring	LateTick() override;
	void			Exit() override;

public:
	const wstring	Check_Transition() override;

	void			Damaged();
	void PlayEffect();

private:
	_bool			m_bPlayEffect = FALSE;

public:
	static CState_Gunner_Damaged* Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};
END
