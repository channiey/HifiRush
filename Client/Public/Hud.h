#pragma once

#include "Ui.h"

BEGIN(Client)

class CCharacter;

class CHud final : public CUi
{
	enum TEX_TYPE { DROP_LOGO, DROP_HEALTH, LOGO, EMPTY_HEALTH, FULL_HEALTH, TYPEEND };

private:
	CHud(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHud(const CGameObject& rhs);
	virtual ~CHud() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void	Tick(_double fTimeDelta);
	virtual void	LateTick(_double fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT			Ready_Components();
	HRESULT			Bind_ShaderResources();

private:
	void			Drop(_double fTimeDelta);
	void			Set_Player();

private:
	LERP_FLOAT_DESC	m_tLerpDesc;
	_bool			m_bMax = FALSE;

	CCharacter*		m_pPlayer = nullptr;


public:
	static CHud*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CHud*	Clone(void* pArg) override;
	virtual void	Free() override;
};

END