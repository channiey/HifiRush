#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
class CShader;
class CRenderer;
class CTransform;
END

BEGIN(Client)

class CPlayer final : public CGameObject
{
protected:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CGameObject& rhs);

	virtual ~CPlayer() = default;

public:
	virtual HRESULT		Initialize_Prototype();
	virtual HRESULT		Initialize(void* pArg);
	virtual void		Tick(_float fTimeDelta);
	virtual void		LateTick(_float fTimeDelta);
	virtual HRESULT		Render();

private: 
	CShader*			m_pShaderCom = { nullptr };	
	CRenderer*			m_pRendererCom = { nullptr };
	CTransform*			m_pTransformCom = { nullptr };
	CModel*				m_pModelCom = { nullptr };

private:
	HRESULT				Ready_Components();
	HRESULT				Bind_ShaderResources();

public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override; 
	virtual void Free() override;
};

END