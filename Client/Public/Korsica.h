#pragma once
#include "Character.h"

BEGIN(Engine)
class CStateMachine;
class CRigidbody;
END

BEGIN(Client)
enum ANIM_KO
{
	IDLE_KO,

	ANIM_KO_END,
};

static const string AnimNames_KO[ANIM_KO::ANIM_KO_END]
{
	"IDLE"
};

enum STATE_KO
{
	STATE_IDLE_KO,
	STATE_END_KO
};

static const wstring StateNames_KO[STATE_KO::STATE_END_KO]
{
	L"IDLE"
};

class CKorsica final : public CCharacter
{
	enum CHILD_TYPE { CH_WEAPON_RIGHT, CH_END };

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

private:
	HRESULT				Ready_Components();
	virtual HRESULT		Ready_Chilren() override;
	virtual HRESULT		Ready_StateMachine() override;

private:
	virtual void		OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild = -1) override;
	virtual void		OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild = -1) override;
	virtual void		OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild = -1) override;

public:
	static CKorsica* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CKorsica* Clone(void* pArg) override;
	virtual void			Free() override;

private:

};

END