#pragma once

#include "Projectile.h"

BEGIN(Client)
class CKorsica_Wind final : public CProjectile
{
private:
	CKorsica_Wind(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CKorsica_Wind(const CKorsica_Wind& rhs);
	virtual ~CKorsica_Wind() = default;

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

private:
	HRESULT					Bind_ShaderResources();

public:
	static CKorsica_Wind*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CKorsica_Wind*	Clone(void* pArg) override;
	virtual void			Free() override;
};
END
