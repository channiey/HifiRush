#pragma once
#include "Character.h"

BEGIN(Engine)
class CStateMachine;
class CRigidbody;
END

BEGIN(Client)
enum ANIM_CH 
{ 
	EVT_CHEERING_00,
	EVT_CHEERING_01,
	EVT_CHEERING_02,
	EVT_CHEER_FAILURE,

	EVT_THINKING_00_00,
	EVT_THINKING_00_01,
	EVT_THINKING_00_02, // 6

	PARRY_00,
	PARRY_01,
	PARRY_02,

	EVT_BATTLE_START_00,
	EVT_BATTLE_START_01,
	
	EVT_GAME_CLEAR_00,
	EVT_GAME_CLEAR_01,

	ELECTRIC_SHOCK_00, // 14

	EVT_APPEAR_JUMP_00,
	EVT_APPEAR_JUMP_01, // 16

	ATK_LIGHT_00,
	ATK_LIGHT_01,
	ATK_LIGHT_02,
	ATK_LIGHT_03,

	ATK_HEAVY_01,
	ATK_HEAVY_02,
	ATK_HEAVY_00_00,
	ATK_HEAVY_00_01,

	ATK_THROW_GUITAR_00,

	ATK_DOUBLE_00,
	ATK_UPPER,
	ATK_HIGHJUMP,
	ATK_SPECIAL_00,
	ATK_BACKSPIN_00,
	ATK_SPECIAL_01_00,
	ATK_SPECIAL_01_01,
	ATK_TOPBLADE_00,
	ATK_SPECIAL_02,
	ATK_SPECIAL_03,// 35

	
	RIDE_BOAD_00,
	RIDE_BOAD_01,
	RIDE_BOAD_02,
	RIDE_BOAD_03,
	RIDE_BOAD_04,
	RIDE_BOAD_05,
	RIDE_BOAD_06,
	RIDE_BOAD_07,
	RIDE_BOAD_08,

	EVT_CEREMONY_00,

	DRINK_00,

	ANNOY_00,
	ANNOY_01, //48

	DMG_HEAVY,
	DMG_LIGHT,
	DMG_DIE,
	
	ELECTRIC_SHOCK_01,

	DMG_POOP_00,
	DMG_POOP_01,
	DMG_POOP_02, // 55

	EVT_PUSH_SWITCH_00,
	EVT_PUSH_SWITCH_01,
	EVT_PUSH_SWITCH_02,
	EVT_PUSH_SWITCH_03,
	EVT_PUSH_SWITCH_04,
	EVT_PUSH_SWITCH_05,


	EVT_PLAY_GUITAR_00,
	EVT_PLAY_GUITAR_01,
	EVT_PLAY_GUITAR_02,
	EVT_PLAY_GUITAR_03,
	EVT_PLAY_GUITAR_04,
	EVT_PLAY_GUITAR_05, // 67

	IDLE,
	DASH,

	JUMP_NORMAL,
	FALL_NORMAL,
	LAND,
	JUMP_DOUBLE,
	FALL_DOUBLE,
	JUMP_NONE_00,

	RUN,

	
	EVT_TALK_HEADHAND_00,
	EVT_TALK_HEADHAND_01,
	EVT_TALK_HEADHAND_02,
	EVT_THINKING_01_00,
	EVT_THINKING_01_01,
	EVT_THINKING_01_02,

	EVT_TALK_IDLE,



	//ANNOY_00, 
	//ANNOY_01,

	//ATK_FINAL_B_00,
	//ATK_FINAL_A_00,
	//ATK_FINAL_A_01,

	//ATK_LIGHT_00,
	//ATK_LIGHT_01,
	//ATK_LIGHT_02,
	//ATK_LIGHT_03,

	//ATK_RUN_A_00,
	//ATK_RUN_A_01,

	//ATK_SINGLE_00,
	//ATK_SINGLE_01,
	//ATK_SINGLE_02,
	//ATK_SINGLE_03,
	//ATK_SINGLE_04,

	//ATK_SPC_0,
	//ATK_SPC_1,
	//ATK_SPC_2,

	//CEREMONY,

	//CLEAR_00,
	//CLEAR_01,

	//DMG_STRONG,
	//DMG_LIGHT,
	//DMG_ELEC,
	//DMG_HEAP_00,
	//DMG_HEAP_01,
	//DMG_HEAP_02,

	//DASH_BACK,
	//DASH_FRONT,
	//DASH_LEFT,
	//DASH_RIGHT,

	//DOUBLE_JUMP, // 32
	//FALL_DOUBLE_JUMP, // 33

	//FALL_JUMP, // 34

	//IDLE, // 35

	//JUMP, // 36
	//JUMP_DASH_00,
	//JUMP_DASH_01,

	//LAND, //39
	//
	//PARRY_00,
	//PARRY_01,
	//PARRY_02,

	//RUN,
	//TALK,

	ANIM_END ,
};

enum STATE_CH
{
	STATE_IDLE_CH,
	STATE_RUN_CH,
	STATE_DASH_CH,
	STATE_JUMP_CH,
	STATE_ATTACK_CH,
	STATE_DAMAGED_CH,
	STATE_PARRY_CH,
	STATE_END_CH
};

static const wstring StateNames_CH[STATE_CH::STATE_END_CH]
{
	L"IDLE",
	L"RUN",
	L"DASH",
	L"JUMP",
	L"ATTACK",
	L"DAMAGED",
	L"PARRY"
};

class CChai final : public CCharacter
{
	enum CHILD_TYPE	{ CH_WEAPON_RIGHT, CH_END };

private:
	CChai(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CChai(const CChai& rhs);
	virtual ~CChai() = default;

public:
	virtual HRESULT		Initialize_Prototype();
	virtual HRESULT		Initialize(void* pArg);
	virtual void		Tick(_double fTimeDelta);
	virtual void		LateTick(_double fTimeDelta);
	virtual HRESULT		Render();

private:
	HRESULT				Ready_Components();
	virtual HRESULT		Ready_Chilren() override;
	virtual HRESULT		Ready_StateMachine() override;

private:
	virtual void		OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild = -1) override;
	virtual void		OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild = -1) override;
	virtual void		OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild = -1) override;

public:
	static CChai*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CChai*		Clone(void* pArg) override;
	virtual void		Free() override;

private:
	friend class CState_Chai_Base;

	friend class CState_Chai_Idle;

	friend class CState_Chai_Run;
	friend class CState_Chai_Dash;
	friend class CState_Chai_Jump;
	
	friend class CState_Chai_Attack;
	friend class CState_Chai_Damaged;
	friend class CState_Chai_Parry;
};

END