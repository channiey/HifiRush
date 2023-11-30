#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CEffect abstract : public CGameObject
{
public:
	typedef struct tagEffectDesc /* 셰이더에 넘겨줄 구조체 */
	{
		_bool	bCW = FALSE;
		Vec2	vUVOffset;

	}EFFECT_DESC;

protected:
	CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect(const CEffect& rhs);
	virtual ~CEffect() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Tick(_double fTimeDelta);
	virtual void			LateTick(_double fTimeDelta);
	virtual HRESULT			Render();

public:
	virtual void			Set_State(const OBJ_STATE& eState) override;
	virtual HRESULT			Return_Pool();

public:
	virtual HRESULT			Start_Effect() { return S_OK; }
	virtual HRESULT			Finish_Effect() { return S_OK; }

public:
	EFFECT_DESC				Get_EffectDesc() const	{ return m_tEffectDesc; }
	_uint					Get_EffectID() const	{ return m_eEffectID; }

	void					Set_TargetObject(CGameObject* pTargetObject) { m_pTargetObject = pTargetObject; }
	void					Set_EffectDesc(EFFECT_DESC tDesc) { memcpy(& m_tEffectDesc, & tDesc, sizeof(EFFECT_DESC)); }
	
	void					Set_CW(const _bool& bCW) { m_tEffectDesc.bCW = bCW; }

protected:
	HRESULT					Ready_Components();

protected:
	HRESULT					Bind_ShaderResources();

protected:
	const _bool				Is_Finish_LifeTime(_double fTimeDelta);

protected:
	CShader*				m_pShaderCom	= { nullptr };
	CRenderer*				m_pRendererCom	= { nullptr };
	CTransform*				m_pTransformCom = { nullptr };

	CVIBuffer_Rect*			m_pVIBufferCom = { nullptr };

	vector<CModel*>			m_pModelComs;
	vector<CTexture*>		m_pTextureComs;
	
protected:
	CGameObject*			m_pTargetObject = { nullptr };

protected:
	EFFECT_DESC				m_tEffectDesc	= {};

	_uint					m_eEffectID		= 0;
	_float					m_fAccTime		= 0.f;
	_float					m_fLifeTime		= 0.3f;

public:
	virtual CEffect*		Clone(void* pArg) = 0;
	virtual void			Free() override;
};

END