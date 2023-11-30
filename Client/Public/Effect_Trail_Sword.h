#pragma once

#include "Effect.h"

BEGIN(Client)

class CEffect_Trail_Sword final : public CEffect
{
	enum TEXTURE_TYPE { DIFFUESE, TEXTURE_TYPEEND };

protected:
	CEffect_Trail_Sword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Trail_Sword(const CEffect_Trail_Sword& rhs);
	virtual ~CEffect_Trail_Sword() = default;

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

public:
	static CEffect_Trail_Sword*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CEffect_Trail_Sword*	Clone(void* pArg) override;
	virtual void					Free() override;
};

END