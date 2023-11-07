#pragma once

#include "TriggerBattle.h"

BEGIN(Client)

class CTriggerSection_A final : public CTriggerBattle
{

private:
	CTriggerSection_A(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTriggerSection_A(const CTriggerSection_A& rhs);
	virtual ~CTriggerSection_A() = default;

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

private:
	virtual void		OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild = -1) override;
	virtual void		OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild = -1) override;
	virtual void		OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild = -1) override;

protected:
	HRESULT				Ready_Components();
	virtual HRESULT		Ready_Pool() override;

private:
	_float				m_fOriginFov = 0.f;
	_float				m_fBattleFov = XMConvertToRadians(70.0f);

public:
	static CTriggerSection_A* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};

END