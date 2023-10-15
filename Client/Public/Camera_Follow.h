#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CCollider;
END

BEGIN(Client)

class CCamera_Follow final : public CGameObject
{
private:
	CCamera_Follow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Follow(const CCamera_Follow& rhs);
	virtual ~CCamera_Follow() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;

private:
	HRESULT Ready_Components();

private:
	void Move(const _float& fTimeDelta);

private:
	CTransform*		m_pTransformCom = { nullptr };
	CCamera*		m_pCameraCom = { nullptr };
	CCollider*		m_pColliderCom = { nullptr };

	TRANSFORM_DESC	m_tTransDesc = {};



	_float			m_fAzimuth = 0.f;
	_float			m_fEvelvation = 0.f;
	_float			m_fRadius = 10.f;
	_float			m_fSpeed = 0.5f;

public:
	static CCamera_Follow* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END