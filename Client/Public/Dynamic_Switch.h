#pragma once

#include "Dynamic.h"

BEGIN(Client)

class CDynamic_Switch final : public CDynamic
{
	enum STATE_TYPE
	{
		OPEN,
		CLOSED,
		OPENING,
		CLOSING,
		TYPEEND
	};

	const string m_AnimNames[STATE_TYPE::TYPEEND]
	{
		"bg0440_idle_001",
		"bg0440_idle_000",
		"bg0440_action_000",
		"bg0440_action_002",
	};

protected:
	CDynamic_Switch(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDynamic_Switch(const CDynamic_Switch& rhs);
	virtual ~CDynamic_Switch() = default;

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
	STATE_TYPE				m_eStateType = STATE_TYPE::OPEN;

	_bool					m_bShot = FALSE;

	_float					m_fTimeAcc = 0.f;
	_float					m_fTimeLimit = 0.8f;

	CDynamic*				m_pTarget = nullptr;

public:
	static CDynamic_Switch* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CDynamic_Switch* Clone(void* pArg) override;
	virtual void			Free() override;
};

END