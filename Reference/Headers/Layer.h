#pragma once

#include "Base.h"

/* ���̾��� �纻 ��ü���� ����Ʈ�� �����Ѵ�. */
/* Ex. ���� ���̾��� ���� ���� ��ü���� ����Ʈ�� �����Ǿ� �ִ�. */

BEGIN(Engine)

class CLayer final : public CBase
{
private:
	CLayer();
	virtual ~CLayer() = default;

public:
	HRESULT Initialize(const wstring& strLayerTag);
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);

public:
	list<class CGameObject*>* Get_Objects() { return &m_GameObjects; }

	/* �̸��� �ߺ��� ��� �տ� ��ġ�� ������Ʈ�� ��ȯ�ȴ�. */
	class CGameObject* Get_GameObject(const wstring& strPrototypeTag);
	const _int		Get_ObjectIndex(const wstring& strName);
	const wstring&		Get_Name() const { return m_strName; }

	/* Ŭ���� ���� ��ȣ�� ������ �̸��� �����Ѵ�. */
	const wstring Get_CloneNameWithPin(const wstring& strName);

public:
	void Set_Name(const wstring& strName) { m_strName = strName; }

public:
	HRESULT Add_GameObject(class CGameObject* pGameObject);
	HRESULT Delete_GameObject(class CGameObject* pGameObject);

	HRESULT Erase_GameObject(class CGameObject* pGameObject); /* Layer to Pool*/
	HRESULT Push_GameObject(class CGameObject* pGameObject); /* Pool to Layer */

private:
	list<class CGameObject*>	m_GameObjects;
	wstring						m_strName;

	class CGameInstance*		m_pGameInstance = { nullptr };

public:
	static CLayer* Create(const wstring& strLayerTag);
	virtual void Free() override;
};

END