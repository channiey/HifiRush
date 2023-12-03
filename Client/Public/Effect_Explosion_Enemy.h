#pragma once

#include "Effect.h"

BEGIN(Client)

class CEffect_Explosion_Enemy final : public CEffect
{
	enum TEXTURE_TYPE  { DIFFUSE, SCRATCH, EMISSIVE, NOISE, TEXTURE_TYPEEND };

protected:
	CEffect_Explosion_Enemy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Explosion_Enemy(const CEffect_Explosion_Enemy& rhs);
	virtual ~CEffect_Explosion_Enemy() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Tick(_double fTimeDelta);
	virtual void			LateTick(_double fTimeDelta);
	virtual HRESULT			Render();

public:
	virtual void			Set_State(const OBJ_STATE& eState) override;
	virtual HRESULT			Return_Pool() override;

public:
	virtual HRESULT			Start_Effect() override;
	virtual HRESULT			Finish_Effect() override;

protected:
	HRESULT					Ready_Components();

protected:
	HRESULT					Bind_ShaderResources();

private:
	HRESULT					Render_Explosion(const _uint& iPassIndex);

private:
	Matrix							m_matWorldOrigin;

	/* Explosion */
	const _float					m_fLerpTimeExplosion	= 0.1f;
	const _float					m_fMinScaleExplosion	= 0.5f;
	const _float					m_fMaxScaleExplosion	= 4.f;
	LERP_FLOAT_DESC					m_tLerpScaleExplosion;

	/* Dissolve */
	const _float					m_fDissolveTime			= 0.1f;
	LERP_FLOAT_DESC					m_tLerpDissoveAmount;

public:
	static CEffect_Explosion_Enemy* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CEffect_Explosion_Enemy* Clone(void* pArg) override;
	virtual void					Free() override;
};

END