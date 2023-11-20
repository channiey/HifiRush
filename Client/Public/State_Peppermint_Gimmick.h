#pragma once
#include "State_Peppermint_Base.h"

BEGIN(Client)
class CState_Peppermint_Gimmick final : public CState_Peppermint_Base
{
	enum PROGRESS_ID { APPEAR, AIM, SHOOT, DISAPPEAR, PROGRESS_END };

private:
	CState_Peppermint_Gimmick();
	CState_Peppermint_Gimmick(const CState_Peppermint_Gimmick& rhs);
	virtual ~CState_Peppermint_Gimmick() = default;

public:
	virtual HRESULT		Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);

public:
	HRESULT				Enter() override;
	const wstring		Tick(const _double& fTimeDelta) override;
	const wstring		LateTick() override;
	void				Exit() override;

public:
	const wstring		Check_Transition() override;

protected:
	virtual void		Check_Progress(const _double& fTimeDelta);
	virtual void		Set_Transform();

private:
	PROGRESS_ID			m_eProgressID = PROGRESS_ID::PROGRESS_END;

public:
	static CState_Peppermint_Gimmick* Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};
END
