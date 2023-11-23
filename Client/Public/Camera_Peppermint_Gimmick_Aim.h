#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CCamera_Peppermint_Gimmick_Aim final : public CGameObject
{

private:
	CCamera_Peppermint_Gimmick_Aim(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Peppermint_Gimmick_Aim(const CCamera_Peppermint_Gimmick_Aim& rhs);
	virtual ~CCamera_Peppermint_Gimmick_Aim() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double fTimeDelta) override;
	virtual void LateTick(_double fTimeDelta) override;

public:
	virtual void		Set_State(const OBJ_STATE& eState) override;

private:
	HRESULT				Ready_Components();

private:
	HRESULT				Find_Target();
	void				Rotate(const _double& fTimeDelta);

private:
	CTransform*			m_pTransformCom = { nullptr };
	CCamera*			m_pCameraCom = { nullptr };

public:
	static CCamera_Peppermint_Gimmick_Aim* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END