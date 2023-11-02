#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CHealthBar final : public CGameObject
{
protected:
	CHealthBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHealthBar(const CGameObject& rhs);

	virtual ~CHealthBar() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_double fTimeDelta);
	virtual void LateTick(_double fTimeDelta);
	virtual HRESULT Render();

private:
	CRenderer*		m_pRendererCom = { nullptr };
	CShader*		m_pShaderCom = { nullptr };
	CTexture*		m_pTextureCom = { nullptr };
	CTransform*		m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private: 
	_float					m_fX, m_fY, m_fSizeX, m_fSizeY;
	Matrix					m_ViewMatrix, m_ProjMatrix;
	LERP_FLOAT_DESC			m_LerpDesc;

	_bool					m_bMax = FALSE;
	Vec3					m_vOriginPos;

private:
	

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources(const _uint iTexIndex);

public:
	static CHealthBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END