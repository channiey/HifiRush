#pragma once
#include "State_Saber_Base.h"

BEGIN(Client)
class CState_Saber_Idle final : public CState_Saber_Base
{
private:
	CState_Saber_Idle();
	CState_Saber_Idle(const CState_Saber_Idle& rhs);
	virtual ~CState_Saber_Idle() = default;

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
	static CState_Saber_Idle* Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};
END
