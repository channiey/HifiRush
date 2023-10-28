#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
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
	virtual void	Tick(_float fTimeDelta) override;
	virtual void	LateTick(_float fTimeDelta) override;

private:
	HRESULT			Ready_Components();

private:
	HRESULT			Find_Target();

private:
	void			Move(const _float& fTimeDelta);
	const Vec4		Calculate_Position(const _float& fTimeDelta);
	const Vec4		Calculate_Look(const _float& fTimeDelta);

private:
	CTransform*		m_pTransformCom = { nullptr };
	CCamera*		m_pCameraCom	= { nullptr };

private:
	Vec2			m_vAngle		= { 0.f, 1.f };
	_float			m_fMinAngleY = 0.7f;
	_float			m_fMaxAngleY = 1.f;

public:
	static CCamera_Follow* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END