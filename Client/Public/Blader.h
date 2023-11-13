#pragma once
#include "Enemy.h"

BEGIN(Client)

enum ANIM_BL
{
	APPEAR_BL,

	ATK_BLADE_BL,
	ATK_COMBO_BL,
	ATK_JUMP_STRKIE_00_BL,
	ATK_JUMP_STRKIE_01_BL,
	ATK_JUMP_STRKIE_02_BL,

	DIE_BL,

	DMG_AIR_BL,
	DMG_LIGHT_00_BL,
	DMG_LIGHT_01_BL,
	DMG_LIGHT_02_BL,

	ESCAPE_BACK_BL,
	ESCAPE_FRONT_BL,
	ESCAPE_LEFT_BL,
	ESCAPE_RIGHT_BL,

	IDLE_BL,
	IDLE_SHORT_00_BL,
	IDLE_SHORT_01_BL,

	PARRY_EVENT_DAMAGED_BL,
	PARRY_EVENT_FINISH_BL,
	PARRY_EVENT_ING_BL,
	PARRY_EVENT_START_BL,

	RUN_FORWARD,
	RUN_ING,
	RUN_LEFT_AND_FORWARD,
	RUN_RIGHT_AND_FORWARD,
	RUN_STAND_UP,


	WALK_BACK_BL,
	WALK_FRONT_BL,
	WALK_LEFT_BL,
	WALK_RIGHT_BL,

	TURN_BACK_TOLEFT_BL,
	TURN_BACK_TORIGHT_BL,
	TURN_LEFT_BL,
	TURN_LEFT_FORWARD_BL,
	TURN_RIGHT_BL,
	TURN_RUGHT_FORWARD_BL,


	ANIM_BL_END
};


static const string AnimNames_BL[ANIM_BL::ANIM_BL_END]
{
	"APPEAR",

	"ATK_BLADE",
	"ATK_COMBO",
	"ATK_JUMP_STRIKE_00",
	"ATK_JUMP_STRIKE_01",
	"ATK_JUMP_STRIKE_02",

	"DIE",

	"DMG_AIR_00",
	"DMG_LIGHT_00",
	"DMG_LIGHT_01",
	"DMG_LIGHT_02",

	"ESCAPE_BACK",
	"ESCAPE_FRONT",
	"ESCAPE_LEFT",
	"ESCAPE_RIGHT",

	"IDLE",
	"IDLE_SHORT_00",
	"IDLE_SHORT_01",
	
	"PARRYEVENT_DAMAGED",
	"PARRYEVENT_FINISH",
	"PARRYEVENT_ING",
	"PARRYEVENT_START",

	"RUN_FORWARD",
	"RUN_ING",
	"RUN_LEFT_AND_FORWARD",
	"RUN_RIGHT_AND_FORWARD",
	"RUN_STAND_UP",

	"WALK_BACK",
	"WALK_FRONT",
	"WALK_LEFT",
	"WALK_RIGHT"

	"TURN_BACK_TOLEFT",
	"TURN_BACK_TORIGHT",
	"TURN_LEFT",
	"TURN_LEFT_FORWARD",
	"TURN_RIGHT",
	"TURN_RUGHT_FORWARD"
};

enum STATE_BL
{
	STATE_IDLE_BL,
	STATE_MOVE_BL,
	STATE_APPEAR_BL,
	STATE_ATTACK_BL,
	STATE_DAMAGED_BL,
	STATE_DEAD_BL,
	STATE_PARRYEVENT_BL,
	STATE_END_BL
};

static const wstring StateNames_BL[STATE_BL::STATE_END_BL]
{
	L"IDLE",
	L"MOVE",
	L"APPEAR",
	L"ATTACK",
	L"DAMAGED",
	L"DEAD",
	L"PATRRYEVENT"
};

class CBlader final : public CEnemy
{
private:
	CBlader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBlader(const CBlader& rhs);
	virtual ~CBlader() = default;

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
	static CBlader*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CBlader*	Clone(void* pArg) override;
	virtual void		Free() override;

private:
	friend class CState_Blader_Base;
	friend class CState_Blader_Idle;
	friend class CState_Blader_Move;
	friend class CState_Blader_Appear;
	friend class CState_Blader_Attack;
	friend class CState_Blader_Damaged;
	friend class CState_Blader_ParryEvent;
	friend class CState_Blader_Dead;
};
END
