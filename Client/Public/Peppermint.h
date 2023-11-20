#pragma once
#include "Character.h"

BEGIN(Engine)
class CStateMachine;
class CRigidbody;
END

BEGIN(Client)
enum ANIM_PE
{
	BATTLE_APPEAR,
	BATTLE_SHOOT,
	BATTLE_DISAPPEAR,

	GIMMICK_IDLE,
	GIMMICK_SHOOT,
	GIMMICK_DISAPPEAR,

	ANIM_PE_END,
};

static const string AnimNames_PE[ANIM_PE::ANIM_PE_END]
{
	"ch1000_atk_200",
	"ch1000_atk_201",
	"ch1000_atk_204",

	"ch1000_gun_000",
	"ch1000_gun_100",
	"ch1000_gun_030"
};

enum STATE_PE
{
	STATE_BATTLE_PE,
	STATE_GIMMICK_PE,
	STATE_END_PE
};

static const wstring StateNames_PE[STATE_PE::STATE_END_PE]
{
	L"BATTLE",
	L"GIMMICK",
};

class CPeppermint final : public CCharacter
{
public:
	enum CHILD_TYPE { WP_LEFT, WP_RIGHT, PROJECTILE, CH_END };

private:
	CPeppermint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPeppermint(const CPeppermint& rhs);
	virtual ~CPeppermint() = default;

public:
	virtual HRESULT		Initialize_Prototype();
	virtual HRESULT		Initialize(void* pArg);
	virtual void		Tick(_double fTimeDelta);
	virtual void		LateTick(_double fTimeDelta);
	virtual HRESULT		Render();

public:
	virtual void		Set_State(const OBJ_STATE& eState) override;
	virtual void		Damaged(CCharacter* pCharacter, const ATK_TYPE& eAtkType = ATK_TYPE::LIGHT) override;

private:
	HRESULT				Ready_Components();
	virtual HRESULT		Ready_Chilren() override;
	virtual HRESULT		Ready_StateMachine() override;
	virtual HRESULT		Ready_Pool();

private:
	virtual void		OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild = -1) override;
	virtual void		OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild = -1) override;
	virtual void		OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild = -1) override;

public:
	static CPeppermint*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CPeppermint*	Clone(void* pArg) override;
	virtual void			Free() override;

private:
	friend class CState_Peppermint_Base;
	friend class CState_Peppermint_Battle;
	friend class CState_Peppermint_Gimmick;
};

END