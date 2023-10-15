#pragma once
#include "State_Chai_Base.h"

BEGIN(Client)
class CState_Chai_Attack_1 final : public CState_Chai_Base
{
private:
	CState_Chai_Attack_1();
	CState_Chai_Attack_1(const CState_Chai_Attack_1& rhs);
	virtual ~CState_Chai_Attack_1() = default;

public:
	virtual HRESULT Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);

public:
	HRESULT Enter() override;
	const wstring& Tick(const _float& fTimeDelta) override;
	const wstring& LateTick() override;
	void Exit() override;

public:
	const wstring& Check_Transition() override;

public:
	static CState_Chai_Attack_1* Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};
END
