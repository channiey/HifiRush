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
		Vec2 vPos = Vec2::Zero;
		Vec2 vSize = Vec2{ 100.f, 100.f };

		tagUiDesc() {};
		tagUiDesc(const Vec2& _vPos, const Vec2& _vSize)
			: vPos(_vPos), vSize(_vSize) {};

		Vec2 Get_Pos() const { return vPos; }
		Vec2 Get_Size() const { return vSize; }
		void Set_Pos(const Vec2& _vPos) { memcpy(&vPos, &_vPos, sizeof(Vec2)); }
		void Set_Size(const Vec2& _vSize) { memcpy(&vSize, &_vSize, sizeof(Vec2)); }

	}UI_DESC;

protected:
	CUi(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUi(const CGameObject& rhs);
	virtual ~CUi() = default;

public:
	virtual HRESULT		Initialize_Prototype();
	virtual HRESULT		Initialize(void* pArg);
	virtual void		Tick(_double fTimeDelta);
	virtual void		LateTick(_double fTimeDelta);
	virtual HRESULT		Render();

public:
	vector<UI_DESC>		Get_TextureLocalDesc() const	{ return m_TextureLocalDesc; }
	const _uint			Get_TextureCount() 				{ return (_uint)m_TextureLocalDesc.size(); }
	vector<CTexture*>*  Get_TextureComs()				{ return &m_pTextureComs; }
	CTexture*			Get_Texture(const _uint iIndex) { if (m_pTextureComs.size() <= iIndex) return nullptr; return m_pTextureComs[iIndex]; }
	UI_DESC*			Get_Desc(const _uint iIndex)	{ if (m_TextureLocalDesc.size() <= iIndex) return nullptr; return &m_TextureLocalDesc[iIndex]; }

public:
	HRESULT				Save();
	HRESULT				Load();

private:
	HRESULT				Ready_Components();
	HRESULT				Bind_ShaderResources();

protected:
	CRenderer*			m_pRendererCom	= { nullptr };
	CShader*			m_pShaderCom	= { nullptr };
	CTransform*			m_pTransformCom = { nullptr };
	CVIBuffer_Rect*		m_pVIBufferCom	= { nullptr };

	Matrix				m_ViewMatrix; 
	Matrix				m_ProjMatrix;

	vector<CTexture*>	m_pTextureComs;
	vector<UI_DESC>		m_TextureLocalDesc;			/* �ؽ�ó ������ ���� ������ */
	vector<UI_DESC>		m_TextureLocalDescOrigin;	/* �ؽ�ó ������ ���� ������ ���� (�������� �����̴� �ֵ��� �ַ� ���) */

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END