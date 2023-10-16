#pragma once
#include "State_Chai_Base.h"

BEGIN(Client)
class CState_Chai_DoubleJump final : public CState_Chai_Base
{
private:
	CState_Chai_DoubleJump();
	CState_Chai_DoubleJump(const CState_Chai_DoubleJump& rhs);
	virtual ~CState_Chai_DoubleJump() = default;

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
	static CState_Chai_DoubleJump* Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};
END
