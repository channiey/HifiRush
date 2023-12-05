#pragma once
#include "Character.h"

BEGIN(Engine)
class CBehaviourTree;
class CRigidbody;
END

BEGIN(Client)

class CTriggerBattle;

class CEnemy abstract : public CCharacter
{
protected:
	CEnemy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEnemy(const CEnemy& rhs);
	virtual ~CEnemy() = default;

public:
	virtual HRESULT		Initialize_Prototype();
	virtual HRESULT		Initialize(void* pArg);
	virtual void		Tick(_double fTimeDelta);
	virtual void		LateTick(_double fTimeDelta);
	virtual HRESULT		Render();

public:
	virtual void		Set_State(const OBJ_STATE& eState) override;
	void				Set_BattleTrigger(CTriggerBattle* pBattleTrigger) { m_pBattleTrigger = pBattleTrigger; }

	CTriggerBattle*		Get_BattleTrigger() const { return m_pBattleTrigger; }
	
	HRESULT				Return_To_Pool();

	const _bool			Is_EnemyActive() const { return m_bActive; }
	void				Set_EnemyActive(const _bool bActive) { m_bActive = bActive; }
protected:
	HRESULT				Ready_Components();

protected:
	virtual void		OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild = -1) override;
	virtual void		OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild = -1) override;
	virtual void		OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild = -1) override;

protected:
	CTriggerBattle*		m_pBattleTrigger = nullptr;

	_bool				m_bActive = FALSE;

public:
	virtual CEnemy*		Clone(void* pArg) PURE;
	virtual void		Free() override;
};
END
