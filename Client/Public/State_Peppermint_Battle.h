#pragma once
#include "State_Peppermint_Base.h"

BEGIN(Client)
class CState_Peppermint_Battle final : public CState_Peppermint_Base
{
	enum PROGRESS_ID { APPEAR, SHOOT, DISAPPEAR, PROGRESS_END };

private:
	CState_Peppermint_Battle();
	CState_Peppermint_Battle(const CState_Peppermint_Battle& rhs);
	virtual ~CState_Peppermint_Battle() = default;

public:
	virtual HRESULT		Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);

public:
	HRESULT			Enter() override;
	const wstring	Tick(const _double& fTimeDelta) override;
	const wstring	LateTick() override;
	void			Exit() override;

protected:
	const wstring	Check_Transition() override;

protected:
	virtual void	Check_Progress(const _double& fTimeDelta) override;
	virtual void	Set_Transform() override;
	virtual void	Play_Sound() override;

private:
	HRESULT			Shoot(const _uint& iCurFrame);

private:
	PROGRESS_ID		m_eProgressID = PROGRESS_ID::PROGRESS_END;
	_bool			m_bShoot = FALSE;

public:
	static CState_Peppermint_Battle* Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};
END
