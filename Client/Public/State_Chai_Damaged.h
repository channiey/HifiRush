#pragma once
#include "State_Chai_Base.h"

BEGIN(Client)
class CState_Chai_Damaged final : public CState_Chai_Base
{
private:
	CState_Chai_Damaged();
	CState_Chai_Damaged(const CState_Chai_Damaged& rhs);
	virtual ~CState_Chai_Damaged() = default;

public:
	virtual HRESULT Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);

public:
	HRESULT Enter() override;
	const wstring& Tick(const _double& fTimeDelta) override;
	const wstring& LateTick() override;
	void Exit() override;

public:
	const wstring& Check_Transition() override;

public:
	static CState_Chai_Damaged* Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};
END
