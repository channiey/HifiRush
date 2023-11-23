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

class CDynamic abstract : public CGameObject
{
protected:
	CDynamic(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDynamic(const CDynamic& rhs);
	virtual ~CDynamic() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Tick(_double fTimeDelta);
	virtual void			LateTick(_double fTimeDelta);
	virtual HRESULT			Render();
	
public:
	virtual HRESULT			Set_On() { return S_OK; }
	virtual HRESULT			Set_Off() { return S_OK; }

protected:
	virtual void			Check_Progress(_double fTimeDelta) {};

protected:
	HRESULT					Ready_Components();
	virtual HRESULT			Ready_Chilren() { return S_OK; }
	
protected:
	HRESULT					Bind_ShaderResources();

protected:
	CShader*				m_pShaderCom	= { nullptr };
	CRenderer*				m_pRendererCom	= { nullptr };
	CTransform*				m_pTransformCom = { nullptr };
	CModel*					m_pModelCom		= { nullptr };
	CCollider*				m_pCollider		= { nullptr };

public:
	virtual CDynamic*		Clone(void* pArg) = 0;
	virtual void			Free() override;
};

END