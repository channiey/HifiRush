#pragma once
#include "State_Macaron_Base.h"

BEGIN(Client)
class CState_Macaron_Battle final : public CState_Macaron_Base
{
private:
	CState_Macaron_Battle();
	CState_Macaron_Battle(const CState_Macaron_Battle& rhs);
	virtual ~CState_Macaron_Battle() = default;

public:
	virtual HRESULT		Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);

public:
	HRESULT				Enter() override;
	const wstring		Tick(const _double& fTimeDelta) override;
	const wstring		LateTick() override;
	void				Exit() override;

public:
	const wstring		Check_Transition() override;

public:
	static CState_Macaron_Battle* Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};
END
