#pragma once
#include "Enemy.h"

BEGIN(Client)

enum ANIM_GU
{
	/* Attack */
	ATK_GROUND_INTRO_GU,
	ATK_GROUND_WAIT_GU,
	ATK_GROUND_SHOOT_GU,

	ATK_JUMP_INTRO_GU,
	ATK_JUMP_WAIT_GU,
	ATK_JUMP_SHOOT_GU,
	
	/* Dead */
	DEAD_GU,

	/* Idle */
	IDLE_GU,

	/* Move */
	ESCAPE_LEFT,
	ESCAPE_RIGHT,
	ESCAPE_FORWARD,
	ESCAPE_BACKWARD,

	/* Non Combat*/
	NON_COMBAT_00_GU,

	NON_COMBAT_01_00_GU,
	NON_COMBAT_01_01_GU,

	NON_COMBAT_TO_COMBAT_GU,

	/* Dmg */
	DMG_LIGHT_00_GU,
	DMG_LIGHT_01_GU,
	DMG_LIGHT_02_GU,

	DMG_MIDDLE_00_GU,
	DMG_MIDDLE_01_GU,
	DMG_MIDDLE_02_GU,

	DMG_KNOCK_UP_00_GU,

	DMG_IN_AIR_00_GU,
	DMG_IN_AIR_01_GU,
	DMG_IN_AIR_02_GU,

	DMG_FALL_GU,
	DMG_DOWN_GU, // еп╫Б
	DMG_DOWN_TO_STAND_GU,

	ANIM_END_GU
};

static const string AnimNames_GU[ANIM_GU::ANIM_END_GU]
{
	"em0200_atk-gun_000",
	"em0200_atk-gun_001",
	"em0200_atk-gun_002",

	"em0200_atk-gun_020",
	"em0200_atk-gun_021",
	"em0200_atk-gun_022",

	"em0200_dmg-high_500",

	"em0200_idle_000",

	"em0200_mvnt-escape_000",
	"em0200_mvnt-escape_001",
	"em0200_mvnt-escape_002",
	"em0200_mvnt-escape_003",

	"em0200_idle-noncombat_001",

	"em0200_idle-noncombat_004",
	"em0200_idle-noncombat_005",

	"em0200_idle-noncombat_002",

	"em0200_dmg-low_010",
	"em0200_dmg-low_020",
	"em0200_dmg-low_030",

	"em0200_dmg-mid_000",
	"em0200_dmg-mid_010",
	"em0200_dmg-mid_020",

	"em0200_dmg-air_000",

	"em0200_dmg-air_200",
	"em0200_dmg-air_210",
	"em0200_dmg-air_220",

	"em0200_dmg-air_050",
	"em0200_dmg-down_210", // еп╫Б
	"em0200_dmg-down_150",
};

enum STATE_GU
{
	STATE_IDLE_GU,
	STATE_MOVE_GU,

	STATE_NONECOMBAT,
	STATE_ATTACK_GU,
	STATE_DAMAGED_GU,
	STATE_DEAD_GU,

	STATE_END_GU
};

static const wstring StateNames_GU[STATE_GU::STATE_END_GU]
{
	L"IDLE",
	L"MOVE",
	
	L"NONECOMBAT",
	L"ATTACK",
	L"DAMAGED",
	L"DEAD"	
};

class CGunner final : public CEnemy
{

private:
	CGunner(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGunner(const CGunner& rhs);
	virtual ~CGunner() = default;

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
	static CGunner*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGunner*	Clone(void* pArg) override;
	virtual void		Free() override;

private:
	friend class CState_Gunner_Base;
	friend class CState_Gunner_Idle;
	friend class CState_Gunner_Move;
	friend class CState_Gunner_NonCombat;
	friend class CState_Gunner_Attack;
	friend class CState_Gunner_Damaged;
	friend class CState_Gunner_Dead;
};
END
