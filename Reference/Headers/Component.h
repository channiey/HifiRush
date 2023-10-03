#pragma once

#include "Base.h"

BEGIN(Engine)

enum COMPONTENT_TYPE
{
	COM_RENDERER,
	COM_VIBUFFER,
	COM_SHADER,
	COM_TEXTURE,
	COM_TRANSFORM,

	COM_STATEMACHINE,
	COM_BEHAVIOURTREE,

	//COM_MODEL,
	//COM_COLLIDER,
	//COM_NAVIMESH,
	//COM_CAMERA,

	//COM_LOD,
	//COM_BT,
	//COM_WATER,
	//COM_MONOBEHAVIOUR,

	COM_END
};
static wstring gStrComponentType[COM_END]
{
	L"Com_Renderer",
	L"Com_VIBuffer",
	L"Com_Shader",
	L"Com_Texture"
	L"Com_Transform",

	L"Com_StateMachine",
	L"Com_BehaviourTree"

	//L"Com_Model",
	//L"Com_Collider",
	//L"Com_Navimesh",
	//L"Com_Camera"

	//L"Com_Lod",
	//L"Com_BT",
	//L"Com_Water",
	//L"Com_Monobehaviour",
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
	const _bool& Is_Active() const { return m_bActive; }

public:
	const class CGameObject* const Get_Owner() const { return m_pOwner; }

public:
	/* Active 이벤트 처리를 위한 가상함수화, 오버라이드시 __super::Active()먼저 호출 필요 */
	virtual HRESULT Set_Active(const _bool& bActive) { m_bActive = bActive; return S_OK; }

protected:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

	_bool					m_bActive = { TRUE };

	_bool					m_bClone = { FALSE };
	/* 아직 미사용 */
	class CGameObject*		m_pOwner = { nullptr };

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END