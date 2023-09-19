#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CCamera_Debug final : public CCamera
{

private:
	CCamera_Debug(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Debug(const CCamera_Debug& rhs);
	virtual ~CCamera_Debug() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;

private:
	_float	m_fMouseSensitive = { 0.0f };
	TRANSFORM_DESC m_tTransDesc = {};

private:
	HRESULT Ready_Components();

public:
	static CCamera_Debug* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END