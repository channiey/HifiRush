#pragma once
#include "State_Blader_Base.h"

BEGIN(Client)
class CState_Blader_ParryEvent final : public CState_Blader_Base
{
private:
	CState_Blader_ParryEvent();
	CState_Blader_ParryEvent(const CState_Blader_ParryEvent& rhs);
	virtual ~CState_Blader_ParryEvent() = default;

public:
	virtual HRESULT Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);

public:
	HRESULT			Enter() override;
	const wstring	Tick(const _double& fTimeDelta) override;
	const wstring	LateTick() override;
	void			Exit() override;

public:
	const wstring	Check_Transition() override;

private:
	void		WakeUp();
	void		Create_Note();
	void		Attack();

public:
	static CState_Blader_ParryEvent* Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};
END
