#pragma once
#include "Character.h"

BEGIN(Engine)
class CStateMachine;
class CRigidbody;
END

BEGIN(Client)
enum ANIM_MA
{
	BATTLE_APPEAR_MA,
	BATTLE_ATTACK_MA,
	BATTLE_DISAPPEAR_MA,

	GIMMICK_APPEAR_MA,
	GIMMICK_TIMMING_MA,
	GIMMICK_SUCCESS_DISAPPEAR_MA,
	GIMMICK_FAILURE_DISAPPEAR_MA,

	ENDING_START_MA,
	ENDING_IDLE_MA,

	ANIM_MA_END,
};

static const string AnimNames_MA[ANIM_MA::ANIM_MA_END]
{
	"ch2000_atk_100",
	"ch2000_atk_101",
	"ch2000_atk_102",

	"ch2000_action_020",
	"ch2000_action_021",
	"ch2000_action_022",
	"ch2000_action_040",

	"ch2000_talk-idle_003",
	"ch2000_talk-idle_005"
};

enum STATE_MA
{
	STATE_BATTLE_MA,
	STATE_GIMMICK_MA,
	STATE_ENDING_MA,
	STATE_END_MA
};

static const wstring StateNames_MA[STATE_MA::STATE_END_MA]
{
	L"BATTLE",
	L"GIMMICK",
	L"ENDING"
};

static const Vec4 vMacaron_Gimmick_Relative_Pos{ -4.f, 0.f, 1.f, 1.f };

class CMacaron final : public CCharacter
{

private:
	CMacaron(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMacaron(const CMacaron& rhs);
	virtual ~CMacaron() = default;

public:
	virtual HRESULT		Initialize_Prototype();
	virtual HRESULT		Initialize(void* pArg);
	virtual void		Tick(_double fTimeDelta);
	virtual void		LateTick(_double fTimeDelta);
	virtual HRESULT		Render();

public:
	virtual void		Set_State(const OBJ_STATE& eState) override;
	virtual void		Damaged(CCharacter* pCharacter, const ATK_TYPE& eAtkType = ATK_TYPE::LIGHT) override {};

private:
	HRESULT				Ready_Components();
	virtual HRESULT		Ready_Chilren() override;
	virtual HRESULT		Ready_StateMachine() override;

private:
	virtual void		OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild = -1) override;
	virtual void		OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild = -1) override;
	virtual void		OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild = -1) override;

public:
	static CMacaron*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CMacaron*	Clone(void* pArg) override;
	virtual void		Free() override;

private:
	_bool					m_bActive = FALSE;
	_bool					m_bInit = FALSE;

private:
	friend class CState_Peppermint_Base;
	friend class CState_Peppermint_Battle;
	friend class CState_Peppermint_Gimmick;
};

END