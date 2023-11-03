#pragma once
#include "State_Chai_Base.h"

BEGIN(Client)
class CState_Chai_Jump final : public CState_Chai_Base
{
private:
	CState_Chai_Jump();
	CState_Chai_Jump(const CState_Chai_Jump& rhs);
	virtual ~CState_Chai_Jump() = default;

public:
	virtual HRESULT Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);

public:
	HRESULT			Enter() override;
	const wstring&	Tick(const _double& fTimeDelta) override;
	const wstring&	LateTick() override;
	void			Exit() override;

public:
	const wstring& Check_Transition() override;

private:
	void Move(const _double& fTimeDelta);
	void Jump();
	void Land();

private:
	const _bool Check_Land();
	const _bool	Check_Fall();
	void		Set_FallSpeed();

private:
	Vec3	m_vScale = {};

	_bool m_bSetFallSpeed_InJump = FALSE;
	_bool m_bSetFallSpeed_InDoubleJump = FALSE;

public:
	static CState_Chai_Jump* Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};
END
