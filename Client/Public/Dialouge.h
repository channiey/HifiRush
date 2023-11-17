#pragma once

#include "Ui.h"

BEGIN(Client)

class CCharacter;

class CDialouge final : public CUi
{
	enum TEX_TYPE { LOGO, TYPEEND };

private:
	CDialouge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDialouge(const CGameObject& rhs);
	virtual ~CDialouge() = default;

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
	LERP_FLOAT_DESC	m_tLerpDescHeight;
	_float			m_fLimit = 1.5f;
	_float			m_fAcc = 0.f;
	_float			m_fAlpha = 0.6f;
	_bool			m_bFix = FALSE;
	_bool			m_bSound = FALSE;

public:
	static CDialouge* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CDialouge* Clone(void* pArg) override;
	virtual void	Free() override;
};

END