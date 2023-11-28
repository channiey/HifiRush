#pragma once
#include "State_Blader_Base.h"

BEGIN(Client)
class CState_Blader_Idle final : public CState_Blader_Base
{
private:
	CState_Blader_Idle();
	CState_Blader_Idle(const CState_Blader_Idle& rhs);
	virtual ~CState_Blader_Idle() = default;

public:
	virtual HRESULT Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);

public:
	HRESULT			Enter() override;
	const wstring	Tick(const _double& fTimeDelta) override;
	const wstring	LateTick() override;
	void			Exit() override;

public:
	const wstring	Check_Transition() override;

	virtual const wstring Choose_NextState() override;

private:
	_bool	m_bPlaySound = FALSE;

public:
	static CState_Blader_Idle* Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};
END
