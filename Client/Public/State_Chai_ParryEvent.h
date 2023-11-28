#pragma once
#include "State_Chai_Base.h"

BEGIN(Client)
class CState_Chai_ParryEvent final : public CState_Chai_Base
{
private:
	CState_Chai_ParryEvent();
	CState_Chai_ParryEvent(const CState_Chai_ParryEvent& rhs);
	virtual ~CState_Chai_ParryEvent() = default;

public:
	virtual HRESULT Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);

public:
	HRESULT			Enter() override;
	const wstring	Tick(const _double& fTimeDelta) override;
	const wstring	LateTick() override;
	void			Exit() override;

public:
	virtual void	OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild = -1) override;

public:
	const wstring	Check_Transition() override;
	const _bool&	Is_Success() const { return m_bSuccess; }
	void			Set_CanFinalAttack(const _bool& bCan) { m_bCanFinalAttack = bCan; }

private:
	void			Parry();
	void			Damaged();
	void			FinalAttack();
	void			Check_Animation();

private:
	_bool			m_bParried = FALSE;
	_bool			m_bDamaged = FALSE;
	_bool			m_bSuccess = TRUE;
	_bool			m_bCanFinalAttack = FALSE;
	_bool			m_bFinalAttack = FALSE;

	_bool			m_bPlaySound = FALSE;

public:
	static CState_Chai_ParryEvent* Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};
END
