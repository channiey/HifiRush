#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CCamera_Parry final : public CGameObject
{
private:
	CCamera_Parry(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Parry(const CCamera_Parry& rhs);
	virtual ~CCamera_Parry() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Tick(_double fTimeDelta) override;
	virtual void	LateTick(_double fTimeDelta) override;

public:
	void			Set_CamTransform(CTransform* pTargetTransform);
	void			Zoom_In();
	void			Zoom_Out();

private:
	HRESULT			Ready_Components();

private:
	CTransform*		m_pTransformCom = { nullptr };
	CCamera*		m_pCameraCom = { nullptr };


public:
	static CCamera_Parry* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END