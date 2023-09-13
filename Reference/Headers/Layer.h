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