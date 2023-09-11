#pragma once

#include "Base.h"

/* 게임내에 사용될 객체들을 만들어내기위한 원형객체들을 보관한다.(원본) */
/* 게임내에 사용될 객체들을 보관한다.(사본) */


BEGIN(Engine)

class CObject_Manager final : public CBase
{
	DECLARE_SINGLETON(CObject_Manager)
private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public: /* Life Cycle*/
	HRESULT Reserve_Manager(_uint iNumLevels);
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);

public: /* Container Func */
	/* 원형을 추가한여 벡터에 추가한다. -> 클라 로딩에서 생성하여 받아온다. */
	HRESULT Add_Prototype(const wstring& strPrototypeTag, class CGameObject* pPrototype);
	/* 사본을 생성하여 레이어에 추가한다. - 원형을 제외한 모든 게임오브젝트는 이 함수를 통해 생성(복제)된다. */
	HRESULT Add_GameObject(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, void* pArg);
	void Clear(_uint iLevelIndex);

private:
	/* 원형객체들을 레벨별로 보관할까?! - <레벨 이름, 객체 이름>*/
	map<const wstring, class CGameObject*>			m_Prototypes;

private:
	/* 사본객체들을 레벨별로 그룹(CLayer)지어서 보관한다. */
	/* 맵 배열(레벨별)이다. 맵은 <레이어 이름(오브젝트 종류 이름) - 해당 종류 오브젝트 리스트> 구성되어 있다.*/
	_uint											m_iNumLevels = { 0 };
	map<const wstring, class CLayer*>*				m_pLayers = { nullptr };
	typedef map<const wstring, class CLayer*>		LAYERS;

private:
	class CGameObject* Find_Prototype(const wstring& strPrototypeTag);
	class CLayer* Find_Layer(_uint iLevelIndex, const wstring& strLayerTag);

public:
	virtual void Free() override;
};

END