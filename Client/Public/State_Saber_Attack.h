#pragma once
#include "State_Saber_Base.h"

BEGIN(Client)
class CState_Saber_Attack final : public CState_Saber_Base
{
private:
	CState_Saber_Attack();
	CState_Saber_Attack(const CState_Saber_Attack& rhs);
	virtual ~CState_Saber_Attack() = default;

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
	static CState_Saber_Attack* Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};
END
