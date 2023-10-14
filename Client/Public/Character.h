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

class CCharacter abstract  : public CGameObject
{
protected:
	CCharacter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCharacter(const CCharacter& rhs);
	virtual ~CCharacter() = default;

public:
	virtual HRESULT		Initialize_Prototype();
	virtual HRESULT		Initialize(void* pArg);
	virtual void		Tick(_float fTimeDelta);
	virtual void		LateTick(_float fTimeDelta);
	virtual HRESULT		Render();

protected:
	HRESULT				Ready_Components();
	HRESULT				Ready_ChildObjects();

protected:
	HRESULT				Bind_ShaderResources();

private:
	HRESULT				Update_RootMotion();

protected:
	CShader*			m_pShaderCom	= { nullptr };
	CRenderer*			m_pRendererCom	= { nullptr };
	CTransform*			m_pTransformCom = { nullptr };
	CModel*				m_pModelCom		= { nullptr };
	vector<CCollider*>  m_pColliderComs;

	TRANSFORM_DESC		m_tTransDesc = {};


public:
	virtual CCharacter* Clone(void* pArg)= 0;
	virtual void Free() override;

};

END