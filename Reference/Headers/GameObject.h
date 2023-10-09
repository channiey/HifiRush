#pragma once

#include "Base.h"
BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
public:
	typedef map<const wstring, class CComponent*>				COMPONENTS;
	typedef vector<class CMonoBehaviour*>						MONOBEHAVIOURS;

	/* STATE_ACTIVE일 때만 라이프 사이클 함수를 타게 된다. */
	/* STATE_UNACTIVE는 컨테이너에는 포함되어 있지만 라이플 사이클 함수를 타진 않는다. */
	enum OBJ_STATE { STATE_ACTIVE, STATE_UNACTIVE, STATE_WILLREMOVED, STATE_END };

protected:
	CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public:
	virtual HRESULT		Initialize_Prototype();
	virtual HRESULT		Initialize(void* pArg);
	virtual void		Tick(_float fTimeDelta);
	virtual void		LateTick(_float fTimeDelta);
	virtual HRESULT		Render();

public: 
	virtual void		OnCollision_Enter(class CGameObject* pGameObject) {};
	virtual void		OnCollision_Stay(class CGameObject* pGameObject) {};
	virtual void		OnCollision_Exit(class CGameObject* pGameObject) {};

public: 
	const _bool 		Is_Active() const { return m_eState == STATE_ACTIVE ? TRUE : FALSE; }
	const _bool&		Is_Render() const { return m_bRender; }
	const _bool&		Is_Picked() const { return m_bPicked; }

public: 
	const wstring&		Get_Name() const { return m_strName; }
	const wstring&		Get_LayerTag() const { return m_strLayer; }
	const _uint&		Get_PickID() const { return m_iPickID; }
	const _float&		Get_CamDistance() const { return m_fCamDistance; }
	const OBJ_STATE&	Get_State() const { return m_eState; }

	class CTransform*			Get_Transform();
	class CVIBuffer*			Get_VIBuffer();
	class CModel*				Get_Model();
	class CCollider_Sphere*		Get_Collider_Sphere();
	class CCollider_AABB*		Get_Collider_AABB();
	class CCollider_OBB*		Get_Collider_OBB();

	
	class CComponent* const			Get_Component(const wstring& strComponentTag) { return Find_Component(strComponentTag); };
	COMPONENTS&						Get_Components() { return m_Components; }
	class CMonoBehaviour* const		Get_MonoBehaviour(const _uint& iIndex);
	vector<class CMonoBehaviour*>&	Get_MonoBehaviours() { return m_MonoBehaviours; }

public: 
	virtual void	Set_State(const OBJ_STATE& eState) { m_eState = eState; }
	virtual void	Set_Render(const _bool& bRender) { m_bRender = bRender; }
	void			Set_Name(const wstring& strName) { m_strName = strName; }
	void			Set_LayerTag(const wstring& strLayer) { m_strLayer = strLayer; }
	void			Set_Picked(const _bool& bPicked) { m_bPicked = bPicked; }

protected:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

	COMPONENTS				m_Components;
	MONOBEHAVIOURS			m_MonoBehaviours;

	/* UnActive 상태라면 라이프 사이클 함수를 타지 않는다. */
	OBJ_STATE				m_eState = { STATE_ACTIVE };
	_bool					m_bRender = { TRUE };	
	wstring					m_strLayer = { };
	wstring					m_strName = { };

	/* 피킹 처리를 위한 고유 아이디 */
	_uint					m_iPickID = { 0 };
	_bool					m_bPicked = FALSE;
	_float					m_fCamDistance = 0.f;

protected:
	HRESULT Add_Component(_uint iLevelIndex, const wstring& strPrototypeTag, const wstring& strComponentTag, _Inout_ CComponent** ppOut, void* pArg = nullptr);
	class CComponent* Find_Component(const wstring& strComponentTag);
	HRESULT Compute_CamZ(_fvector vWorldPos);


private:
	HRESULT					Create_PickID() { return S_OK; }

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;

	friend class CLayer;
};

END