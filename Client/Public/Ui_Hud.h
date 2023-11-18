#pragma once

#include "Ui.h"

BEGIN(Client)

class CCharacter;

class CUi_Hud final : public CUi
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
	CUi_Hud(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUi_Hud(const CGameObject& rhs);
	virtual ~CUi_Hud() = default;

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
	static CUi_Hud*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CUi_Hud*Clone(void* pArg) override;
	virtual void	Free() override;
};

END