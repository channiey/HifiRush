#pragma once

#include "Dynamic.h"

BEGIN(Client)

class CDynamic_AutoDoor final : public CDynamic
{
	enum MODEL_TYPE { LEFT_DOOR, RIGHT_DOOR, MODEL_TYPEEND };
	enum STATE_TYPE { OPEN, CLOSED, STATEEND };

protected:
	CDynamic_AutoDoor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDynamic_AutoDoor(const CDynamic_AutoDoor& rhs);
	virtual ~CDynamic_AutoDoor() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Tick(_double fTimeDelta);
	virtual void			LateTick(_double fTimeDelta);
	virtual HRESULT			Render();

public:
	virtual void			OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild = -1) override;
	virtual void			OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild = -1) override;
	virtual void			OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild = -1) override;

protected:
	virtual HRESULT			Set_On() override;
	virtual HRESULT			Set_Off() override;
	virtual void			Check_Progress(_double fTimeDelta) override;

protected:
	HRESULT					Ready_Components();
	virtual HRESULT			Ready_Chilren() { return S_OK; }

protected:
	HRESULT					Bind_ShaderResources();

private:
	STATE_TYPE				m_eStateType = CDynamic_AutoDoor::STATE_TYPE::CLOSED;
	CModel*					m_pModelComs[CDynamic_AutoDoor::MODEL_TYPE::MODEL_TYPEEND];

	const _float			m_fOriginX		= 0.f;
	const _float			m_fTargetX		= 6.5f;
	const _float			m_fLerpTime		= 0.2f;
	const _float			m_fColliderSize = 8.f;
	LERP_FLOAT_DESC			m_tLerpDesc;

public:
	static CDynamic_AutoDoor*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CDynamic_AutoDoor*	Clone(void* pArg) override;
	virtual void				Free() override;
};

END