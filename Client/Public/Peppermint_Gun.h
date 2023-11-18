#pragma once

#include "Client_Defines.h"
#include "Weapon.h"

BEGIN(Client)

class CPeppermint_Gun final : public CWeapon
{
protected:
	CPeppermint_Gun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPeppermint_Gun(const CPeppermint_Gun& rhs);
	virtual ~CPeppermint_Gun() = default;

public:
	virtual HRESULT		Initialize_Prototype();
	virtual HRESULT		Initialize(void* pArg);
	virtual void		Tick(_double fTimeDelta);
	virtual void		LateTick(_double fTimeDelta);
	virtual HRESULT		Render();

protected:
	HRESULT				Ready_Components();

protected:
	virtual void		OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild = -1) override;
	virtual void		OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild = -1) override;
	virtual void		OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild = -1) override;

public:
	static CPeppermint_Gun* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END