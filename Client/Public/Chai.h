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
	EVT_THINKING_00_02,

	PARRY_00,
	PARRY_01,
	PARRY_02,

	EVT_BATTLE_START_00,
	EVT_BATTLE_START_01,
	
	EVT_GAME_CLEAR_00,
	EVT_GAME_CLEAR_01,

	ELECTRIC_SHOCK_00,

	EVT_APPEAR_JUMP_00,
	EVT_APPEAR_JUMP_01, 

	ATK_LIGHT_00,
	ATK_LIGHT_01,
	ATK_LIGHT_02,
	ATK_LIGHT_03,

	ATK_HEAVY_01,
	ATK_HEAVY_02,
	ATK_HEAVY_NONE_00,
	ATK_HEAVY_00,

	ATK_THROW_GUITAR_00, // ch0000_atk-guitar_031

	ATK_DOUBLE_00,
	ATK_UPPER,
	ATK_HIGHJUMP,
	ATK_SPECIAL_00, // ch0000_atk-guitar_100 // HIBIKI
	ATK_BACKSPIN_00,
	ATK_WAIT,
	ATK_NONE_00,
	ATK_VERTICAL_TOPBLADE_00,
	ATK_FINAL,
	ATK_HORIZONTAL_TOPBLADE_03, // ch0000_atk-sp_020 // POWER CHORD
	
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
	ANNOY_01, 

	DMG_HEAVY,
	DMG_LIGHT,
	DMG_DIE,
	
	ELECTRIC_SHOCK_01,

	DMG_POOP_00,
	DMG_POOP_01,
	DMG_POOP_02, 

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
	EVT_PLAY_GUITAR_05, 

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

	ANIM_CH_END ,
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
	STATE_PARRYEVENT_CH,
	STATE_SPECIALATTACK_CH,
	STATE_ENDING_CH,
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
	L"PARRY",
	L"PARRYEVENT",
	L"SPECIALATTACK",
	L"ENDING"
};

class CChai final : public CCharacter
{
	enum CHILD_TYPE	{ CH_WEAPON_RIGHT, CH_END };
	
public:
	typedef struct tagChaiDesc
	{
		/* Reverb Guage */
		const _float	fMaxReverbGuage		= 100.f;
		const _float	fDeltaReverbGuage	= 10.f;
		_float			fCurReverbGuage		= 0.f;
		_bool			bFullReverbGuage	= FALSE;

		void Add_ReverbGuage(const _float& fMul = 1.f)
		{
			if (bFullReverbGuage)
				return;

			fCurReverbGuage += fDeltaReverbGuage * fMul;
			if (fMaxReverbGuage < fCurReverbGuage)
			{
				fCurReverbGuage = fMaxReverbGuage;
				bFullReverbGuage = TRUE;
			}

		}
		void Clear_ReverbGuage()
		{
			fCurReverbGuage = 0.f;
			bFullReverbGuage = FALSE;
		}
		const _float Get_ReverbGuage_Percent()
		{
			return fCurReverbGuage / fMaxReverbGuage;
		}
		const _bool& Is_Full_ReverbGuage() const { return bFullReverbGuage; }

	}CHAI_DESC;

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

public:
	virtual void		OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild = -1) override;
	virtual void		OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild = -1) override;
	virtual void		OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild = -1) override;

public:
	virtual void		Damaged(CCharacter* pCharacter, const ATK_TYPE& eAtkType) override;
	CHAI_DESC			Get_ChaiDesc() const { return m_tChaiDesc; }

public:
	static CChai*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CChai*		Clone(void* pArg) override;
	virtual void		Free() override;

private:
	HRESULT				Set_OtherPlayer();
	void				Quick_Test();

private:
	CHAI_DESC			m_tChaiDesc;

private:
	friend class CState_Chai_Base;

	friend class CState_Chai_Idle;

	friend class CState_Chai_Run;
	friend class CState_Chai_Dash;
	friend class CState_Chai_Jump;
	
	friend class CState_Chai_Attack;
	friend class CState_Chai_Damaged;
	friend class CState_Chai_Parry;
	friend class CState_Chai_ParryEvent;
	friend class CState_Chai_SpecialAttack;
};

END