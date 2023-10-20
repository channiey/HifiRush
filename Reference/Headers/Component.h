#pragma once

#include "Base.h"

BEGIN(Engine)

enum COMPONENT_TYPE
{
	COM_TRANSFORM,
	COM_MODEL,
	COM_SHADER,
	COM_TEXTURE,
	COM_RENDERER,
	COM_COLLIDER_OBB,
	COM_COLLIDER_AABB,
	COM_COLLIDER_SPHERE,
	COM_VIBUFFER,
	COM_VIBUFFER_RECT,
	COM_VIBUFFER_CUBE,
	COM_VIBUFFER_TERRAIN,
	COM_MESH,
	COM_STATEMACHINE,
	COM_BEHAVIOURTREE,
	COM_CAMERA,
	COM_END
};

static const wstring ComponentNames[COM_END]
{
	L"Com_Transform",
	L"Com_Model",
	L"Com_Shader",
	L"Com_Texture",
	L"Com_Renderer",
	L"Com_Collider_OBB",
	L"Com_Collider_AABB",
	L"Com_Collider_Sphere",
	L"Com_VIBuffer",
	L"Com_VIBuffer_Rect",
	L"Com_VIBuffer_Cube",
	L"Com_VIBuffer_Terrain",
	L"Com_Mesh",
	L"Com_StateMachine",
	L"Com_BehaviourTree",
	L"Com_Camera",
};


class ENGINE_DLL CComponent abstract : public CBase
{
protected:
	CComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CComponent(const CComponent& rhs);
	virtual ~CComponent() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

#ifdef _DEBUG
public:
	virtual HRESULT Render() { return S_OK; }
#endif // _DEBUG

public:
	class CGameObject*	Get_Owner() const { return m_pOwner; }
	const wstring&		Get_Name() const { return m_strName; }
	
	HRESULT				Set_Owner(class CGameObject* pParent) { if (nullptr == pParent) return E_FAIL; else m_pOwner = pParent; return S_OK; }
	void				Set_Name(const wstring& strName) { m_strName = strName; }
	
	/* Active 이벤트 처리를 위한 가상함수화, 오버라이드시 __super::Active()먼저 호출 필요 */
	virtual HRESULT		Set_Active(const _bool& bActive) { m_bActive = bActive; return S_OK; }
	const _bool&		Is_Active() const { return m_bActive; }

protected:
	ID3D11Device*			m_pDevice	= { nullptr };
	ID3D11DeviceContext*	m_pContext	= { nullptr };

	_bool					m_bActive	= { TRUE };

	_bool					m_bClone	= { FALSE };
	class CGameObject*		m_pOwner	= { nullptr };

	wstring					m_strName	= {};

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END