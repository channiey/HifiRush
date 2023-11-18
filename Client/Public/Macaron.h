#pragma once
#include "Character.h"

BEGIN(Engine)
class CStateMachine;
class CRigidbody;
END

BEGIN(Client)
enum ANIM_MA
{
	IDLE_MA,

	ANIM_MA_END,
};

static const string AnimNames_MA[ANIM_MA::ANIM_MA_END]
{
	"ch2000_talk-idle_000"
};

enum STATE_MA
{
	STATE_IDLE_MA,
	STATE_END_MA
};

static const wstring StateNames_MA[STATE_MA::STATE_END_MA]
{
	L"IDLE"
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

};

END