#pragma once
#include "State_Chai_Base.h"

BEGIN(Client)
class CState_Chai_Idle final : public CState_Chai_Base
{
private:
	CState_Chai_Idle();
	CState_Chai_Idle(const CState_Chai_Idle& rhs);
	virtual ~CState_Chai_Idle() = default;

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
	static CState_Chai_Idle* Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};
END
