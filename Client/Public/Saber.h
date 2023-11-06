#pragma once
#include "Enemy.h"

BEGIN(Client)

enum ANIM_SA
{
	
};

enum STATE_SA
{
	STATE_IDLE_SA,
	STATE_RUN_SA,
	STATE_DASH_SA,
	STATE_JUMP_SA,
	STATE_ATTACK_SA,
	STATE_DAMAGED_SA,
	STATE_PARRY_SA,
	STATE_END_SA
};

static const wstring StateNames_SA[STATE_SA::STATE_END_SA]
{
	L"IDLE",
	L"RUN",
	L"DASH",
	L"JUMP",
	L"ATTACK",
	L"DAMAGED",
	L"PARRY"
};

class CSaber final : public CEnemy
{
	enum CHILD_TYPE { SA_WEAPON_RIGHT, SA_TRIGGER_TRACKED, SA_END };

private:
	CSaber(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSaber(const CSaber& rhs);
	virtual ~CSaber() = default;

public:
	virtual HRESULT		Initialize_Prototype();
	virtual HRESULT		Initialize(void* pArg);
	virtual void		Tick(_double fTimeDelta);
	virtual void		LateTick(_double fTimeDelta);
	virtual HRESULT		Render();

public:
	virtual void		Set_State(const OBJ_STATE& eState) override;

private:
	HRESULT				Ready_Components();
	virtual HRESULT		Ready_Chilren() override;
	virtual HRESULT		Ready_StateMachine() override;

private:
	virtual void		OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild = -1) override;
	virtual void		OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild = -1) override;
	virtual void		OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild = -1) override;

public:
	static CSaber*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CSaber*		Clone(void* pArg) override;
	virtual void		Free() override;

private:
	friend class CState_Saber_Base;
};
END
