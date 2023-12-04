#pragma once

#include "Effect.h"

BEGIN(Client)

class CEffect_Projectile_Peppermint final : public CEffect
{
	enum TEXTURE_TYPE { DIFFUSE, TEXTURE_TYPEEND };

protected:
	CEffect_Projectile_Peppermint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Projectile_Peppermint(const CEffect_Projectile_Peppermint& rhs);
	virtual ~CEffect_Projectile_Peppermint() = default;

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
	CGameObject*			m_pBullet	= nullptr;

	_float					m_fAcc		= 0.f;

	Matrix					m_matWorldOrigin;

public:
	static CEffect_Projectile_Peppermint* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CEffect_Projectile_Peppermint* Clone(void* pArg) override;
	virtual void					Free() override;
};

END