#pragma once
#include "Character.h"

BEGIN(Engine)
class CStateMachine;
class CRigidbody;
END

BEGIN(Client)
enum ANIM_KO
{
	BATTLE_APPEAR_KO,
	BATTLE_ATTACK_KO,
	BATTLE_DISAPPEAR_KO,

	ANIM_KO_END,
};

static const string AnimNames_KO[ANIM_KO::ANIM_KO_END]
{
	"ch4000_atk_000",
	"ch4000_atk_002",
	"ch4000_atk_003"
};

enum STATE_KO
{
	STATE_BATTLE_KO,
	STATE_GIMMICK_KO,
	STATE_END_KO
};

static const wstring StateNames_KO[STATE_KO::STATE_END_KO]
{ 
	L"BATTLE",
	L"GIMMICK"
};

class CKorsica final : public CCharacter
{
public:
	enum CHILD_TYPE { WP_RIGHT, PROJECTILE, CH_END };

private:
	CKorsica(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CKorsica(const CKorsica& rhs);
	virtual ~CKorsica() = default;

public:
	virtual HRESULT		Initialize_Prototype();
	virtual HRESULT		Initialize(void* pArg);
	virtual void		Tick(_double fTimeDelta);
	virtual void		LateTick(_double fTimeDelta);
	virtual HRESULT		Render();

public:
	virtual void		Set_State(const OBJ_STATE& eState) override;
	virtual void		Damaged(CCharacter* pCharacter, const ATK_TYPE& eAtkType = ATK_TYPE::LIGHT) override {};
	virtual HRESULT		Ready_Pool();

private:
	HRESULT				Ready_Components();
	virtual HRESULT		Ready_Chilren() override;
	virtual HRESULT		Ready_StateMachine() override;

private:
	virtual void		OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild = -1) override;
	virtual void		OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild = -1) override;
	virtual void		OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild = -1) override;

public:
	static CKorsica*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CKorsica*	Clone(void* pArg) override;
	virtual void		Free() override;

private:
	friend class CState_Peppermint_Base;
	friend class CState_Peppermint_Battle;
	friend class CState_Peppermint_Gimmick;
};

END