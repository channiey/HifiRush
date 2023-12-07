#pragma once
#include "Character.h"

BEGIN(Client)

class CNpc abstract : public CCharacter
{
protected:
	CNpc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNpc(const CNpc& rhs);
	virtual ~CNpc() = default;

public:
	virtual HRESULT		Initialize_Prototype();
	virtual HRESULT		Initialize(void* pArg);
	virtual void		Tick(_double fTimeDelta);
	virtual void		LateTick(_double fTimeDelta);
	virtual HRESULT		Render();

public:
	virtual void		Set_State(const OBJ_STATE& eState) override;

protected:
	HRESULT				Ready_Components();

protected:
	virtual HRESULT		Set_Dialogue() { return S_OK; }

protected:
	virtual void		OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild = -1) override;
	virtual void		OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild = -1) override;
	virtual void		OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild = -1) override;

protected:

	_bool				m_bSetDialouge = FALSE;

public:
	virtual CNpc* Clone(void* pArg) PURE;
	virtual void		Free() override;
};
END
