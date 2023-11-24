#pragma once
#include "State_Gunner_Base.h"

BEGIN(Client)
class CState_Gunner_Attack final : public CState_Gunner_Base
{
	enum class ATTACK_TYPE 
	{
		GROUND, 
		AIR, 
		TYPEEND 
	};

private:
	CState_Gunner_Attack();
	CState_Gunner_Attack(const CState_Gunner_Attack& rhs);
	virtual ~CState_Gunner_Attack() = default;

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
	ATTACK_TYPE		m_eAttackType = ATTACK_TYPE::TYPEEND;

public:
	static CState_Gunner_Attack* Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};
END
