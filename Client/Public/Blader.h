#pragma once
#include "Enemy.h"

BEGIN(Client)

enum ANIM_BL
{
	ANIM_IDLE_BL,
	ANIM_END_BL
};

enum STATE_BL
{
	STATE_IDLE_BL,
	STATE_END_BL
};

static const wstring StateNames_BL[STATE_BL::STATE_END_BL]
{
	L"IDLE"
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
};
END
