#pragma once
#include "State_Gunner_Base.h"

BEGIN(Client)
class CState_Gunner_Dead final : public CState_Gunner_Base
{
private:
	CState_Gunner_Dead();
	CState_Gunner_Dead(const CState_Gunner_Dead& rhs);
	virtual ~CState_Gunner_Dead() = default;

public:
	virtual HRESULT Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);

public:
	HRESULT Enter() override;
	const wstring Tick(const _double& fTimeDelta) override;
	const wstring LateTick() override;
	void Exit() override;

public:
	const wstring Check_Transition() override;

public:
	static CState_Gunner_Dead* Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};
END
