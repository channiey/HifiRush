#pragma once
#include "State_Blader_Base.h"

BEGIN(Client)
class CState_Blader_Appear final : public CState_Blader_Base
{
private:
	CState_Blader_Appear();
	CState_Blader_Appear(const CState_Blader_Appear& rhs);
	virtual ~CState_Blader_Appear() = default;

public:
	virtual HRESULT Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);

public:
	HRESULT			Enter() override;
	const wstring	Tick(const _double& fTimeDelta) override;
	const wstring	LateTick() override;
	void			Exit() override;

public:
	const wstring Check_Transition() override;

	void		Play_SecondAnimation();

private:
	_bool		m_bPlaySound = FALSE;

public:
	static CState_Blader_Appear* Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};
END
