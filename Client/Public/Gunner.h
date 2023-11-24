#pragma once
#include "Enemy.h"

BEGIN(Client)

enum ANIM_GU
{
	APPEAR_GU,

	ANIM_END_GU
};

static const string AnimNames_GU[ANIM_GU::ANIM_END_GU]
{
	"APPEAR",
};

enum STATE_GU
{
	STATE_IDLE_GU,
	STATE_MOVE_GU,

	STATE_APPEAR_GU,
	STATE_ATTACK_GU,
	STATE_DAMAGED_GU,
	STATE_DEAD_GU,

	STATE_END_GU
};

static const wstring StateNames_GU[STATE_GU::STATE_END_GU]
{
	L"IDLE",
	L"MOVE",
	
	L"APPEAR",
	L"ATTACK",
	L"DAMAGED",
	L"DEAD"	
};

class CGunner final : public CEnemy
{

private:
	CGunner(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGunner(const CGunner& rhs);
	virtual ~CGunner() = default;

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
	static CGunner*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGunner*	Clone(void* pArg) override;
	virtual void		Free() override;

private:
};
END
