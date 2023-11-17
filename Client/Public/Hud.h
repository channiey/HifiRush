#pragma once

#include "Ui.h"

BEGIN(Client)

class CCharacter;

class CHud final : public CUi
{
	enum TEX_TYPE 
	{ 
		DROP_LOGO, 
		DROP_HEALTH,
		
		LOGO_NORMAL,  
		LOGO_SPECIAL,
		
		HEALTH_EMPTY, 
		HEALTH_FULL,
		HEALTH_DAMAGED,
		
		SPECIAL_EMPTY_0,
		SPECIAL_EMPTY_1,
		SPECIAL_EMPTY_2,

		SPECIAL_FULL_0,
		SPECIAL_FULL_1,
		SPECIAL_FULL_2,

		PARTNER_MACARON_ON,
		PARTNER_MACARON_OFF,

		PARTNER_KORSICA_ON,
		PARTNER_KORSICA_OFF,

		PARTNER_PEPPERMINT_ON,
		PARTNER_PEPPERMINT_OFF,
		
		TYPEEND 
	};

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