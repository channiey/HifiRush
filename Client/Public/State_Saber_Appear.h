#pragma once
#include "State_Saber_Base.h"

BEGIN(Client)
class CState_Saber_Appear final : public CState_Saber_Base
{
private:
	CState_Saber_Appear();
	CState_Saber_Appear(const CState_Saber_Appear& rhs);
	virtual ~CState_Saber_Appear() = default;

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
	_bool	m_bLand = FALSE;

public:
	static CState_Saber_Appear* Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};
END
