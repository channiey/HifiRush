#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	/* 원형을 생성할 때 */
	CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* 사본을 생성할 때 (복사 생성자) */
	CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	const _bool& Is_Active() const { return m_bActive; }

public:
	const wstring& Get_Name() const { return m_strName; }
	const wstring& Get_Layer() const { return m_strLayer; }
	const _uint& Get_PickID() const { return m_iPickID; }

	class CComponent* Get_Transform() const { return nullptr; }
	class CComponent* Get_Collider() const { return nullptr; }
	const class CComponent* const Get_Component(const wstring& strComponentTag) { return Find_Component(strComponentTag); };

public:
	/* Active 이벤트 처리를 위한 가상함수화, 오버라이드시 __super::Active()먼저 호출 필요 */
	virtual void Set_Active(const _bool& bActive) { m_bActive = bActive; }
	void Set_Name(const wstring& strName) { m_strName = strName; }

protected:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

	/* 특정 컴포넌트의 검색을 용이하게 하기 위해서. */
	/* 검색을 고려하는 이유?! : 타 객체가 현재 객체의 정보(컴포넌트)를 참조하고자 하는 경우가 빈번하다. */
	map<const wstring, class CComponent*>		m_Components;

	_bool					m_bActive = { TRUE };
	wstring					m_strLayer = { };
	wstring					m_strName = { };

	/* 피킹 처리를 위한 고유 아이디 */
	_uint					m_iPickID = { 0 };

protected:
	HRESULT Add_Component(_uint iLevelIndex, const wstring& strPrototypeTag, const wstring& strComponentTag, _Inout_ CComponent** ppOut, void* pArg = nullptr);
	class CComponent* Find_Component(const wstring& strComponentTag);

private:
	HRESULT					Create_PickID() { return S_OK; }

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END