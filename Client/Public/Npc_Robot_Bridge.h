#pragma once
#include "Npc.h"

BEGIN(Client)

class CNpc_Robot_Bridge final : public CNpc
{
private:
	CNpc_Robot_Bridge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNpc_Robot_Bridge(const CNpc_Robot_Bridge& rhs);
	virtual ~CNpc_Robot_Bridge() = default;

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
	virtual HRESULT		Ready_Chilren();
	virtual HRESULT		Ready_StateMachine();

protected:
	virtual HRESULT		Set_Dialogue() { return S_OK; }

private:
	virtual void		OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild = -1) override;
	virtual void		OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild = -1) override;
	virtual void		OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild = -1) override;

public:
	static CNpc_Robot_Bridge* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CNpc_Robot_Bridge* Clone(void* pArg) override;
	virtual void		Free() override;
};

END