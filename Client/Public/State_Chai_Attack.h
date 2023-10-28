#pragma once
#include "State_Chai_Base.h"

BEGIN(Client)
class CState_Chai_Attack final : public CState_Chai_Base
{ 
	enum class ATTACK_TYPE { LIGHT, STRONG, THROW, SPECIAL, TYPEEND };

private:
	CState_Chai_Attack();
	CState_Chai_Attack(const CState_Chai_Attack& rhs);
	virtual ~CState_Chai_Attack() = default;

public:
	virtual HRESULT		Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);

public:
	HRESULT				Enter() override;
	const wstring&		Tick(const _float& fTimeDelta) override;
	const wstring&		LateTick() override;
	void				Exit() override;

private:
	virtual void		OnCollision_Enter(CGameObject* pGameObject) override;
	virtual void		OnCollision_Stay(CGameObject* pGameObject) override;
	virtual void		OnCollision_Exit(CGameObject* pGameObject) override;

public:
	const wstring&		Check_Transition() override;

private:
	ATTACK_TYPE			m_eAttackType = ATTACK_TYPE::TYPEEND;

public:
	static CState_Chai_Attack* Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};
END
