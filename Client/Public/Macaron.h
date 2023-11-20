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
	//BATTLE_
	//IDLE_MA,

	ANIM_MA_END,
};

static const string AnimNames_MA[ANIM_MA::ANIM_MA_END]
{
	"ch2000_talk-idle_000"
};

enum STATE_MA
{
	STATE_BATTLE_MA,
	STATE_GIMMICK_MA,
	STATE_END_MA
};

static const wstring StateNames_MA[STATE_MA::STATE_END_MA]
{
	L"BATTLE",
	L"GIMMICK"
};

class CMacaron final : public CCharacter
{
	enum CHILD_TYPE { CH_WEAPON_RIGHT, CH_END };

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
	virtual void		Damaged(CCharacter* pCharacter, const ATK_TYPE& eAtkType = ATK_TYPE::LIGHT) override;

private:
	HRESULT				Ready_Components();
	virtual HRESULT		Ready_Chilren() override;
	virtual HRESULT		Ready_StateMachine() override;

private:
	virtual void		OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild = -1) override;
	virtual void		OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild = -1) override;
	virtual void		OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild = -1) override;

public:
	static CMacaron* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CMacaron* Clone(void* pArg) override;
	virtual void			Free() override;

private:
	friend class CState_Peppermint_Base;
	friend class CState_Peppermint_Battle;
	friend class CState_Peppermint_Gimmick;
};

END