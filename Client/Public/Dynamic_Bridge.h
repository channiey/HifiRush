#pragma once

#include "Dynamic.h"

BEGIN(Client)

class CDynamic_Bridge final : public CDynamic
{
protected:
	CDynamic_Bridge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDynamic_Bridge(const CDynamic_Bridge& rhs);
	virtual ~CDynamic_Bridge() = default;

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

protected:
	virtual HRESULT			Set_On() override;
	virtual HRESULT			Set_Off() override;
	virtual void			Check_Progress(_double fTimeDelta) override;

protected:
	HRESULT					Ready_Components();
	virtual HRESULT			Ready_Chilren() { return S_OK; }

protected:
	HRESULT					Bind_ShaderResources();

private:	
	void					Update_Sound();

private:
	const string			m_strAnimName = "bg0210_event_000";
	_bool					m_bPlaySound = FALSE;
	

public:
	static CDynamic_Bridge* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CDynamic_Bridge* Clone(void* pArg) override;
	virtual void			Free() override;
};

END