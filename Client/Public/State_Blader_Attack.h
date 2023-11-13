#pragma once
#include "State_Blader_Base.h"

BEGIN(Client)
class CState_Blader_Attack final : public CState_Blader_Base
{
private:
	CState_Blader_Attack();
	CState_Blader_Attack(const CState_Blader_Attack& rhs);
	virtual ~CState_Blader_Attack() = default;

public:
	virtual HRESULT Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);

public:
	HRESULT Enter() override;
	const wstring Tick(const _double& fTimeDelta) override;
	const wstring LateTick() override;
	void Exit() override;

public:
	const wstring Check_Transition() override;

private:
	virtual HRESULT			Set_Animation() override;
	virtual const wstring	Choose_NextState() override;

public:
	static CState_Blader_Attack* Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};
END
