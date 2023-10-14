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

class CWeapon final : public CGameObject
{
private:
	CWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWeapon(const CWeapon& rhs);
	virtual ~CWeapon() = default;

public:
	virtual HRESULT		Initialize_Prototype();
	virtual HRESULT		Initialize(void* pArg);
	virtual void		Tick(_float fTimeDelta);
	virtual void		LateTick(_float fTimeDelta);
	virtual HRESULT		Render();

public:
	void				Set_Socket(const CModel::BONE_TYPE& eType);

private:
	HRESULT				Ready_Components();

private:
	HRESULT				Update_BoneTransform();

private:
	HRESULT				Bind_ShaderResources();

private:
	CShader*			m_pShaderCom		= { nullptr };
	CRenderer*			m_pRendererCom		= { nullptr };
	CTransform*			m_pTransformCom		= { nullptr };
	CModel*				m_pModelCom			= { nullptr };
	vector<CCollider*>  m_pColliderComs;

	CModel::BONE_TYPE	m_eSocketType = CModel::BONE_END;

public:
	static CWeapon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END