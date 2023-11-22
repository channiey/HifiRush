#pragma once

#include "TriggerBattle.h"

BEGIN(Client)

class CTriggerSection_B final : public CTriggerBattle
{

private:
	CTriggerSection_B(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTriggerSection_B(const CTriggerSection_B& rhs);
	virtual ~CTriggerSection_B() = default;

public:
	virtual HRESULT		Initialize_Prototype();
	virtual HRESULT		Initialize(void* pArg);
	virtual void		Tick(_double fTimeDelta);
	virtual void		LateTick(_double fTimeDelta);
	virtual HRESULT		Render();

public:
	virtual HRESULT		Start_Battle() override;
	virtual HRESULT		Update_Battle(_double fTimeDelta) override;
	virtual HRESULT		Finish_Battle() override;

public:
	virtual void		OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild = -1) override;
	virtual void		OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild = -1) override;
	virtual void		OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild = -1) override;

private:
	HRESULT				Ready_Components();

public:
	static CTriggerSection_B* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void			Free() override;
};

END