#pragma once

#include "Base.h"

/* 레이어의 사본 객체들을 리스트에 보관한다. */
/* Ex. 몬스터 레이어라면 몬스터 종류 객체들이 리스트에 보관되어 있다. */

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

	/* 이름이 중복일 경우 앞에 위치한 오브젝트가 반환된다. */
	class CGameObject* Get_GameObject(const wstring& strPrototypeTag);
	const _int		Get_ObjectIndex(const wstring& strName);
	const wstring&		Get_Name() const { return m_strName; }

	/* 클론의 고유 번호를 세팅한 이름을 리턴한다. */
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