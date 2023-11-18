#pragma once

#include "Client_Defines.h"
#include "Weapon.h"

BEGIN(Client)

class CKorsica_Stick final : public CWeapon
{
protected:
	CKorsica_Stick(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CKorsica_Stick(const CKorsica_Stick& rhs);
	virtual ~CKorsica_Stick() = default;

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
	static CKorsica_Stick* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END