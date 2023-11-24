#pragma once
#include "State_Gunner_Base.h"

BEGIN(Client)
class CState_Gunner_NonCombat final : public CState_Gunner_Base
{
private:
	CState_Gunner_NonCombat();
	CState_Gunner_NonCombat(const CState_Gunner_NonCombat& rhs);
	virtual ~CState_Gunner_NonCombat() = default;

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
	ANIM_GU			m_eAnimType = ANIM_GU::ANIM_END_GU;

public:
	static CState_Gunner_NonCombat* Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};
END
