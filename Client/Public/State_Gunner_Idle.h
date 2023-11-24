#pragma once
#include "State_Gunner_Base.h"

BEGIN(Client)
class CState_Gunner_Idle final : public CState_Gunner_Base
{
private:
	CState_Gunner_Idle();
	CState_Gunner_Idle(const CState_Gunner_Idle& rhs);
	virtual ~CState_Gunner_Idle() = default;

public:
	virtual HRESULT Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);

public:
	HRESULT			Enter() override;
	const wstring	Tick(const _double& fTimeDelta) override;
	const wstring	LateTick() override;
	void			Exit() override;

public:
	const wstring	Check_Transition() override;

public:
	static CState_Gunner_Idle*	Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);
	virtual CState*				Clone(void* pArg) override;
	virtual void				Free() override;
};
END
