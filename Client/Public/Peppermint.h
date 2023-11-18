#pragma once
#include "Character.h"

BEGIN(Engine)
class CStateMachine;
class CRigidbody;
END

BEGIN(Client)
enum ANIM_PE
{
	IDLE_PE,

	ANIM_PE_END,
};

static const string AnimNames_PE[ANIM_PE::ANIM_PE_END]
{
	"ch1000_talk-idle_000"
};

enum STATE_PE
{
	STATE_IDLE_PE,
	STATE_END_PE
};

static const wstring StateNames_PE[STATE_PE::STATE_END_PE]
{
	L"IDLE"
};

class CPeppermint final : public CCharacter
{
	enum CHILD_TYPE { WP_LEFT, WP_RIGHT, CH_END };

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

private:
	HRESULT				Ready_Components();
	virtual HRESULT		Ready_Chilren() override;
	virtual HRESULT		Ready_StateMachine() override;

private:
	virtual void		OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild = -1) override;
	virtual void		OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild = -1) override;
	virtual void		OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild = -1) override;

public:
	static CPeppermint*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CPeppermint*	Clone(void* pArg) override;
	virtual void			Free() override;

private:

};

END