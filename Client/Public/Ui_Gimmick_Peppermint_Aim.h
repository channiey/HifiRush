#pragma once

#include "Ui.h"

BEGIN(Client)

class CCharacter;

class CUi_Peppermint_Aim final : public CUi
{
	enum TEX_TYPE 
	{ 
		SIDE_LEFT,
		SIDE_RIGHT,

		ARROW_TOP,
		ARROW_RIGHT,
		ARROW_DOWN,
		ARROW_LEFT,

		DOT_OUT,
		DOT_IN,
		
		TYPEEND 
	};

private:
	CUi_Peppermint_Aim(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUi_Peppermint_Aim(const CGameObject& rhs);
	virtual ~CUi_Peppermint_Aim() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void	Tick(_double fTimeDelta);
	virtual void	LateTick(_double fTimeDelta);
	virtual HRESULT Render();

public:
	virtual void	Set_State(const OBJ_STATE& eState) override;

private:
	HRESULT			Ready_Components();
	HRESULT			Bind_ShaderResources();

private:
	_float			m_fAlpha = 1.f;

public:
	static CUi_Peppermint_Aim* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CUi_Peppermint_Aim* Clone(void* pArg) override;
	virtual void	Free() override;
};

END