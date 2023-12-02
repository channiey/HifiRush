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

	enum SHADER_TYPE { TEX, MESH, SHADER_END };

	typedef struct tagEffectDesc /* 셰이더에 넘겨줄 구조체 */
	{
		Vec2	vUVOffset1;
		Vec2	vUVOffset2;

		void Reset()
		{
			ZeroMemory(&vUVOffset1, sizeof(Vec2));
			ZeroMemory(&vUVOffset2, sizeof(Vec2));
		}
	}EFFECT_DESC;

	typedef struct tagTimeDesc
	{
		_float fTimeAcc;
		_float fTimeLimit = 0.3f;
		_float fPercentage = 0.f;
		_bool  bFull = FALSE;

		void Update(_double fTimeDelta)
		{
			if (bFull) return;

			fTimeAcc += fTimeDelta;

			fPercentage = fTimeAcc / fTimeLimit;
			if (1 < fPercentage)
			{
				fPercentage = 1.f;
				fTimeAcc = fTimeLimit;
				bFull = TRUE;
			}
		}
		void Reset()
		{
			fTimeAcc = 0.f;
			fPercentage = 0.f;
			bFull = FALSE;
		}

	}TIME_DESC;

	


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
	void					Set_TextureIndex(const _uint& iIndex);

	virtual HRESULT			Return_Pool();

public:
	virtual HRESULT			Start_Effect() { return S_OK; }
	virtual HRESULT			Finish_Effect() { return S_OK; }

public:
	EFFECT_DESC				Get_EffectDesc() const	{ return m_tEffectDesc; }
	_uint					Get_EffectID() const	{ return m_eEffectID; }

	void					Set_EffectDesc(EFFECT_DESC tDesc) { memcpy(& m_tEffectDesc, & tDesc, sizeof(EFFECT_DESC)); }
	
protected:
	HRESULT					Ready_Components();

protected:
	const _bool				Is_Finish_LifeTime(_double fTimeDelta);

	Matrix					Get_Matrix_By_Billboard();
	Matrix					Get_Matrix_Billboard();

protected:
	CShader*				m_pShaderComs[SHADER_TYPE::SHADER_END];
	CRenderer*				m_pRendererCom	= { nullptr };
	CTransform*				m_pTransformCom = { nullptr };

	CVIBuffer_Rect*			m_pVIBufferCom	= { nullptr };

	vector<CModel*>			m_pModelComs;
	vector<CTexture*>		m_pTextureComs;

protected:
	_uint					m_eEffectID		= 0;

	TIME_DESC				m_tTimeDesc		= {};
	EFFECT_DESC				m_tEffectDesc	= {};

	_uint					m_iTextureIndex;

public:
	virtual CEffect*		Clone(void* pArg) = 0;
	virtual void			Free() override;
};

END