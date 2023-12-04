#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
class CShader;
class CCollider;
class CRenderer;
class CTransform;
END

BEGIN(Client)
class CCharacter;
class CEffect;

class CProjectile abstract : public CGameObject
{

public:
	typedef struct tagProjectileDesc
	{
		CCharacter* pOwner			= nullptr;
		Vec4		vDir			= {};
		_float		fSpeedPerSec	= 0.f;
		_uint		iBoneType		= 0;
		Vec4		vOrigin			= {};
		void Clear()
		{
			pOwner			= nullptr;
			vDir			= {};
			vOrigin			= {};
			fSpeedPerSec	= 0.f;
		}
	}PROJECTILE_DESC;

protected:
	CProjectile(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CProjectile(const CProjectile& rhs);
	virtual ~CProjectile() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Tick(_double fTimeDelta);
	virtual void			LateTick(_double fTimeDelta);
	virtual HRESULT			Render();

public:
	CCharacter*				Get_Owner() { return m_tDesc.pOwner; }

public:
	virtual HRESULT			Shoot(PROJECTILE_DESC tDesc) PURE;
	HRESULT					Return_To_Pool();

protected:
	HRESULT					Ready_Components();
	void					Translate(_double fTimeDelta);

protected:
	HRESULT					Bind_ShaderResources();

	virtual HRESULT			Set_Effect() { return S_OK; }

private:
	void					Check_LifeTime(_double fTimeDelta);

protected:
	CShader*				m_pShaderCom	= { nullptr };
	CRenderer*				m_pRendererCom	= { nullptr };
	CTransform*				m_pTransformCom = { nullptr };
	CModel*					m_pModelCom		= { nullptr };
	CCollider*				m_pCollider		= { nullptr };

protected:
	PROJECTILE_DESC			m_tDesc			= {};

protected:
	_float					m_fLimit		= 1.f;
	_float					m_fAcc			= 0.f;

	CEffect*				m_pEffect		= nullptr;

public:
	virtual CProjectile*	Clone(void* pArg) = 0;
	virtual void			Free() override;
};

END