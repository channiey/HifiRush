#pragma once
#include "State_Blader_Base.h"

BEGIN(Client)
class CState_Blader_Damaged final : public CState_Blader_Base
{
private:
	CState_Blader_Damaged();
	CState_Blader_Damaged(const CState_Blader_Damaged& rhs);
	virtual ~CState_Blader_Damaged() = default;

public:
	virtual HRESULT Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);

public:
	HRESULT Enter() override;
	const wstring Tick(const _double& fTimeDelta) override;
	const wstring LateTick() override;
	void Exit() override;

public:
	const wstring Check_Transition() override;

	void Damaged();

	_bool	m_bParriedEvent = FALSE;

private:
	void PlayEffect();

	_bool			m_bPlayEffect = FALSE;

public:
	static CState_Blader_Damaged* Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};
END
