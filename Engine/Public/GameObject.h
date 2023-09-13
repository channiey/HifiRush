#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	/* ������ ������ �� */
	CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* �纻�� ������ �� (���� ������) */
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
	/* Active �̺�Ʈ ó���� ���� �����Լ�ȭ, �������̵�� __super::Active()���� ȣ�� �ʿ� */
	virtual void Set_Active(const _bool& bActive) { m_bActive = bActive; }
	void Set_Name(const wstring& strName) { m_strName = strName; }

protected:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

	/* Ư�� ������Ʈ�� �˻��� �����ϰ� �ϱ� ���ؼ�. */
	/* �˻��� ����ϴ� ����?! : Ÿ ��ü�� ���� ��ü�� ����(������Ʈ)�� �����ϰ��� �ϴ� ��찡 ����ϴ�. */
	map<const wstring, class CComponent*>		m_Components;

	_bool					m_bActive = { TRUE };
	wstring					m_strLayer = { };
	wstring					m_strName = { };

	/* ��ŷ ó���� ���� ���� ���̵� */
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