#pragma once

#include "Projectile.h"

BEGIN(Client)
class CPeppermint_Bullet final : public CProjectile
{
private:
	CPeppermint_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPeppermint_Bullet(const CPeppermint_Bullet& rhs);
	virtual ~CPeppermint_Bullet() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Tick(_double fTimeDelta);
	virtual void			LateTick(_double fTimeDelta);
	virtual HRESULT			Render();

public:
	virtual void			OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild = -1) override;
	virtual void			OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild = -1) override;
	virtual void			OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild = -1) override;

public:
	virtual HRESULT			Shoot(PROJECTILE_DESC tDesc) override;

private:
	HRESULT					Ready_Components();

	virtual HRESULT			Set_Effect() override;

private:
	HRESULT					Bind_ShaderResources();

public:
	static CPeppermint_Bullet*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CPeppermint_Bullet* Clone(void* pArg) override;
	virtual void				Free() override;
};
END
