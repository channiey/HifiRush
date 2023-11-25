#pragma once

#include "Ui.h"

BEGIN(Client)

class CCharacter;

class CUi_SpecialAttack final : public CUi
{
	enum TEX_TYPE
	{
		BACKLOGO,
		CHAI,

		WORD_HIBIKI,
		WORD_POWERCHORD,

		TYPEEND
	};

private:
	CUi_SpecialAttack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUi_SpecialAttack(const CGameObject& rhs);
	virtual ~CUi_SpecialAttack() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void	Tick(_double fTimeDelta);
	virtual void	LateTick(_double fTimeDelta);
	virtual HRESULT Render();

public:
	void			Set_On(const _bool& bHibiki);
	void			Set_Off();


private:
	HRESULT			Ready_Components();
	HRESULT			Bind_ShaderResources();

private:
	LERP_FLOAT_DESC m_tLerpDesc_Alpha = {};

	TEX_TYPE		m_eSpecialAttackType = TEX_TYPE::TYPEEND;

public:
	static CUi_SpecialAttack*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CUi_SpecialAttack*	Clone(void* pArg) override;
	virtual void				Free() override;
};

END