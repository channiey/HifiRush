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

class CUi abstract : public CGameObject
{
public:
	typedef struct tagUiDesc
	{
		Vec2 vPos; 
		Vec2 vSize; 

		tagUiDesc() {};
		tagUiDesc(const Vec2& _vPos, const Vec2& _vSize)
			: vPos(_vPos), vSize(_vSize) {};

	}UI_DESC;

protected:
	CUi(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUi(const CGameObject& rhs);
	virtual ~CUi() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void	Tick(_double fTimeDelta);
	virtual void	LateTick(_double fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT			Ready_Components();
	HRESULT			Bind_ShaderResources();

protected:
	CRenderer*		m_pRendererCom = { nullptr };
	CShader*		m_pShaderCom = { nullptr };
	CTexture*		m_pTextureCom = { nullptr };
	CTransform*		m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

protected:
	UI_DESC			m_eDesc;
	Matrix			m_ViewMatrix; 
	Matrix			m_ProjMatrix;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END