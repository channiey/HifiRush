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
	list<class CGameObject*>* Get_Objects() { return &m_GameObjects; }

public:
	HRESULT Initialize();
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);

public:
	HRESULT Add_GameObject(class CGameObject* pGameObject);
	HRESULT Delete_GameObject(class CGameObject* pGameObject);

	HRESULT Erase_GameObject(class CGameObject* pGameObject); /* Layer to Pool*/
	HRESULT Push_GameObject(class CGameObject* pGameObject); /* Pool to Layer */

private:
	list<class CGameObject*>	m_GameObjects;

public:
	static CLayer* Create();
	virtual void Free() override;
};

END