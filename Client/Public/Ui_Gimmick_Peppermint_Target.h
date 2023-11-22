#pragma once

#include "Ui.h"

BEGIN(Client)

class CCharacter;

class CUi_Peppermint_Target final : public CUi
{
	enum TEX_TYPE
	{
		ARROW_TOP_1,
		ARROW_TOP_2,

		ARROW_RIGHT_1,
		ARROW_RIGHT_2,

		ARROW_DOWN_1,
		ARROW_DOWN_2,

		ARROW_LEFT_1,
		ARROW_LEFT_2,

		CIRCLE,
		DOT,

		TYPEEND
	};

private:
	CUi_Peppermint_Target(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUi_Peppermint_Target(const CGameObject& rhs);
	virtual ~CUi_Peppermint_Target() = default;

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

	HRESULT			Set_Target();

private:
	_float			m_fAlpha = 1.f;

	CGameObject*	m_pTarget = nullptr;

public:
	static CUi_Peppermint_Target* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CUi_Peppermint_Target* Clone(void* pArg) override;
	virtual void	Free() override;
};

END