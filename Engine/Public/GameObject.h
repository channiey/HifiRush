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
	const _bool& Is_Render() const { return m_bRender; }

public:
	const wstring& Get_Name() const { return m_strName; }
	const wstring& Get_LayerTag() const { return m_strLayer; }
	const _uint& Get_PickID() const { return m_iPickID; }

	class CTransform* const Get_Transform();
	
	class CComponent* const Get_Component(const wstring& strComponentTag) { return Find_Component(strComponentTag); };
	map<const wstring, class CComponent*>& Get_Components() { return m_Components; }
	class CMonoBehaviour* const Get_MonoBehaviour(const _uint& iIndex);
	vector<class CMonoBehaviour*>& Get_MonoBehaviours() { return m_MonoBehaviours; }

public:
	/* Active �̺�Ʈ ó���� ���� �����Լ�ȭ, �������̵�� __super::Active()���� ȣ�� �ʿ� */
	virtual void Set_Active(const _bool& bActive) { m_bActive = bActive; }
	/* ���� ���θ� ��Ÿ����. �ش� ������ false��� ���� ���� ȣ������ �ʴ´�. */
	virtual void Set_Render(const _bool& bRender) { m_bRender = bRender; }
	void Set_Name(const wstring& strName) { m_strName = strName; }
	void Set_LayerTag(const wstring& strLayer) { m_strLayer = strLayer; }

protected:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

	/* Ư�� ������Ʈ�� �˻��� �����ϰ� �ϱ� ���ؼ�. */
	/* �˻��� ����ϴ� ����?! : Ÿ ��ü�� ���� ��ü�� ����(������Ʈ)�� �����ϰ��� �ϴ� ��찡 ����ϴ�. */
	map<const wstring, class CComponent*>		m_Components;
	vector<class CMonoBehaviour*>				m_MonoBehaviours;

	/* UnActive ���¶�� ������ ����Ŭ �Լ��� Ÿ�� �ʴ´�. */
	_bool					m_bActive = { TRUE };
	/* ���� ���θ� ��Ÿ����, �ش� ������ false��� �ƿ� �������� ȣ������ �ʴ´�. */
	_bool					m_bRender = { TRUE };	
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