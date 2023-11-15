#pragma once
#include "State_Blader_Base.h"

BEGIN(Engine)
class CCollider_Sphere;
END

BEGIN(Client)
class CState_Blader_Attack final : public CState_Blader_Base
{
private:
	CState_Blader_Attack();
	CState_Blader_Attack(const CState_Blader_Attack& rhs);
	virtual ~CState_Blader_Attack() = default;

public:
	virtual HRESULT Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);

public:
	HRESULT			Enter() override;
	const wstring	Tick(const _double& fTimeDelta) override;
	const wstring	LateTick() override;
	void			Exit() override;

public:
	virtual void		OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild = -1) override;
	virtual void		OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild = -1) override;
	virtual void		OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild = -1) override;

public:
	const wstring			Check_Transition() override;

private:
	virtual HRESULT			Set_Animation() override;
	virtual const wstring	Choose_NextState() override;
	void					Activate_Collider();


private:
	CCollider_Sphere* m_pLeftArmCollider	= nullptr;
	CCollider_Sphere* m_pRightArmCollider	= nullptr;
	CCollider_Sphere* m_pBodyCollider		= nullptr;

public:
	static CState_Blader_Attack* Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};
END
