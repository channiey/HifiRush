#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CCamera_Debug final : public CGameObject
{

private:
	CCamera_Debug(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Debug(const CCamera_Debug& rhs);
	virtual ~CCamera_Debug() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double fTimeDelta) override;
	virtual void LateTick(_double fTimeDelta) override;

public:
	_float Get_SpeedPerSec() const { return m_fSpeedPerSec; }
	void Set_SpeedPerSec(const _float& fSpeed) { m_fSpeedPerSec = fSpeed; }

private:
	HRESULT Ready_Components();

private:
	void Move(const _double& fTimeDelta);

private:
	CTransform*		m_pTransformCom = { nullptr };
	CCamera*		m_pCameraCom = { nullptr };

	_float			m_fSpeedPerSec = 0.f;


public:
	static CCamera_Debug* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END