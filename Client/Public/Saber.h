#pragma once
#include "Enemy.h"

BEGIN(Client)

enum ANIM_SA
{
	ACCENT_00_SA,
	ACCENT_01_SA,
	ACCENT_02_SA,

	APPEAR_00_SA,
	 
	ATK_FLYINNG_SA,
	ATK_NONEFLYING_SA,

	DIE_HEAVY_00_SA,
	DAMAGED_LIGHT_00_SA,
	DAMAGED_LIGHT_01_SA,
	DAMAGED_LIGHT_02_SA,
	DAMAGED_LIGHT_03_SA,
	DAMAGED_LIGHT_04_SA,
	DAMAGED_LIGHT_05_SA,
	DAMAGED_MIDDLE_00_SA,
	DAMAGED_MIDDLE_01_SA,
	DAMAGED_MIDDLE_02_SA,

	DIE_STAR_SA,
	DIE_HEAVY_01_SA,
	DIE_NORMAL_SA,

	STAND_UP_SA,

	ESC_BACKWARD_LOOK_FORWARD_00_SA,
	ESC_BACKWARD_LOOK_ORIGIN_00_SA,

	ESC_FORWARD_LOOK_FORWARD_00_SA,
	ESC_FORWARD_LOOK_ORIGIN_00_SA,
	ESC_FORWARD_LOOK_ORIGIN_01_SA,

	ESC_LEFT_LOOK_FORWARD_00_SA,
	ESC_LEFT_LOOK_ORIGIN_00_SA,
	ESC_LEFT_LOOK_ORIGIN_01_SA,

	ESC_RIGHT_LOOK_FORWARD_00_SA,
	ESC_RIGHT_LOOK_ORIGIN_00_SA,
	ESC_RIGHT_LOOK_ORIGIN_01_SA,

	IDLE_SA,

	RUN_ING_SA,
	RUN_START_SA,
	RUN_STOP_SA,

	IDLE_STAND_SLEEP_SA,
	IDLE_STAND_WAKE_SA,

	WALK_BACKWARD_LOOK_FORWARD_SA,
	WALK_FORWARD_LOOK_FORWARD_SA,
	WALK_LEFT_LOOK_FORWARD_SA,
	WALK_RIGHT_LOOK_FORWARD_SA,

	ANIM_SA_END

};

enum STATE_SA
{
	STATE_IDLE_SA,
	STATE_MOVE_SA,
	STATE_ATTACK_SA,
	STATE_APPEAR_SA,
	STATE_DAMAGED_SA,
	STATE_DEAD_SA,
	STATE_END_SA
};

static const wstring StateNames_SA[STATE_SA::STATE_END_SA]
{
	L"IDLE",
	L"MOVE",
	L"ATTACK",
	L"APPEAR",
	L"DAMAGED",
	L"DEAD"
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

	friend class CState_Saber_Idle;

	friend class CState_Saber_Move;

	friend class CState_Saber_Attack;
	friend class CState_Saber_Damaged;
	friend class CState_Saber_Die;

	friend class CState_Saber_Appear;
};
END
