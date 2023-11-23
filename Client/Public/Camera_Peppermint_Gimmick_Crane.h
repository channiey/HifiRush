#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CCamera_Peppermint_Gimmick_Crane final : public CGameObject
{

private:
	CCamera_Peppermint_Gimmick_Crane(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Peppermint_Gimmick_Crane(const CCamera_Peppermint_Gimmick_Crane& rhs);
	virtual ~CCamera_Peppermint_Gimmick_Crane() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Tick(_double fTimeDelta) override;
	virtual void	LateTick(_double fTimeDelta) override;

public:
	const _bool&	Is_Debug_Move() const { return m_bDebugMove; }
	void			Set_Debug_Move(const _bool bMove) { m_bDebugMove = bMove; }

	const _bool&	Is_LookTarget() const { return m_bLookTarget; }
	void			Set_LookTarget(const _bool bLookTarget) { m_bLookTarget = bLookTarget; }

public:
	virtual void	Set_State(const OBJ_STATE& eState) override;

private:
	HRESULT			Ready_Components();

private:
	void			Debug_Move(const _double& fTimeDelta);
	void			Look_Target();

private:
	CTransform*		m_pTransformCom = { nullptr };
	CCamera*		m_pCameraCom = { nullptr };

	_float			m_fSpeedPerSec = 0.f;

	_bool			m_bDebugMove = FALSE;
	_bool			m_bLookTarget = FALSE;

public:
	static CCamera_Peppermint_Gimmick_Crane* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END