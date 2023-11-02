#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

#include "Model.h"

BEGIN(Engine)
class CModel;
class CShader;
class CCollider;
class CRenderer;
class CTransform;
END

BEGIN(Client)

class CWeapon abstract : public CGameObject
{
protected:
	CWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWeapon(const CWeapon& rhs);
	virtual ~CWeapon() = default;

public:
	virtual HRESULT		Initialize_Prototype();
	virtual HRESULT		Initialize(void* pArg);
	virtual void		Tick(_double fTimeDelta);
	virtual void		LateTick(_double fTimeDelta);
	virtual HRESULT		Render();

public:
	void				Set_Socket(const CModel::BONE_TYPE& eType);

protected:
	HRESULT				Ready_Components();

protected:
	HRESULT				Bind_ShaderResources();

protected:
	virtual void		OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild = -1) override;
	virtual void		OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild = -1) override;
	virtual void		OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild = -1) override;

protected:
	CShader*			m_pShaderCom		= { nullptr };
	CRenderer*			m_pRendererCom		= { nullptr };
	CTransform*			m_pTransformCom		= { nullptr };
	CModel*				m_pModelCom			= { nullptr };
	vector<CCollider*>  m_pColliderComs;

	CModel::BONE_TYPE	m_eSocketType = CModel::BONE_END;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;

};

END