#pragma once

#include "Effect.h"

BEGIN(Client)

class CEffect_Damaged_Enemy final : public CEffect
{
	enum TEXTURE_TYPE { CIRCLE_DIFFUSE, CIRCLE_DISSOLVE, TEXTURE_TYPEEND };

protected:
	CEffect_Damaged_Enemy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Damaged_Enemy(const CEffect_Damaged_Enemy& rhs);
	virtual ~CEffect_Damaged_Enemy() = default;

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
	const _float					m_fMinScale = 1.f;
	const _float					m_fMaxScale = 4.f;


	const _float					m_fLifeTime = 0.08f;
	
	LERP_FLOAT_DESC					m_tLerpDescScale;

public:
	static CEffect_Damaged_Enemy* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CEffect_Damaged_Enemy* Clone(void* pArg) override;
	virtual void					Free() override;
};

END