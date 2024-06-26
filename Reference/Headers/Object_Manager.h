#pragma once

#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CObject_Manager final : public CBase
{
	DECLARE_SINGLETON(CObject_Manager)

private:
	typedef map<const wstring, class CLayer*>				LAYERS;
	typedef map<const wstring, queue<class CGameObject*>*>	POOLS;
	typedef queue<class CGameObject*>						POOL;

	enum EVENT_TYPE { OBJ_ADD, OBJ_DEL, POP_FROM_POOL, RETURN_TO_POOL, LAYER_ADD, LAYER_DEL, END_EVT };

	typedef struct EventDesc
	{
		_uint				iLevelIndex	= 0;
		class CGameObject*	pObj		= { nullptr };
		class CLayer*		pLayer = { nullptr };

		EventDesc(const _uint& _iLevelIndex, class CGameObject* _pObj)
			: iLevelIndex(_iLevelIndex), pObj(_pObj), pLayer(nullptr)
		{
			Safe_AddRef(pObj);
		}

		EventDesc(const _uint& _iLevelIndex, class CLayer* _pLayer)
			: iLevelIndex(_iLevelIndex), pLayer(_pLayer), pObj(nullptr)
		{
			//Safe_AddRef(pLayer); /*이거 왜 오류 뜨지? 게임오브젝트는 안 뜨는데?*/
		}

	}EVENT_DESC;

private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	HRESULT Reserve_Manager(_uint iNumLevels);
	void	Tick(_double fTimeDelta);
	void	LateTick(_double fTimeDelta);
	void	FinishTick(); 

public: 
	map<const wstring, class CGameObject*>* Get_Prototypes() { return &m_Prototypes; } 
	LAYERS*									Get_All_Layer(_uint iLevelIndex);													/* 특정 레벨의 모든 레이어를 가져온다. */
	list<class CGameObject*>*				Get_Layer(_uint iLevelIndex, const wstring& strLayerTag);			/* 특정 레벨의 특정 레이어를 가져온다. */
	class CLayer*							Get_LayerClass(_uint iLevelIndex, const wstring& strLayerTag);
	class CGameObject*						Get_Player();
	class CGameObject*						Get_GameObject(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag);
	const _int								Get_ObjectIndex(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strName);
public:
	/* 원형 오브젝트 생성 */
	HRESULT				Add_Prototype(const wstring& strPrototypeTag, class CGameObject* pPrototype);	/* 원형을 생성하여 원형 벡터에 추가한다. (클라 로딩에서 생성하여 받아온다) */

	/* Obj Event (Clone) */
	class CGameObject*	Add_GameObject(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, void* pArg); /* 사본을 생성하여 레이어에 추가한다. - 원형을 제외한 모든 게임오브젝트는 이 함수를 통해 생성(복제)된다. */
	HRESULT				Delete_GameObject(_uint iLevelIndex, class CGameObject* pObj);					/* TODO 고유 아이디로 해도 괜찮을 듯 */

	/* Pool Event (Clone) */
	HRESULT				Reserve_Pool(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, const _uint& iNumObj, void* pArg); /* 해당 레벨의 원형이 생성된 뒤에 호출된다. */
	class CGameObject*	Pop_Pool(_uint iLevelIndex, const wstring& strPrototypeTag);
	HRESULT				Return_Pool(_uint iLevelIndex, class CGameObject* pObj);

	/* Layer Event */
	HRESULT				Add_Layer(_uint iLevelIndex, const wstring& strLayerTag);
	HRESULT				Delete_Layer(_uint iLevelIndex, const wstring& strLayerTag);

	void Clear(_uint iLevelIndex);

private:
	_uint										m_iNumLevels = { 0 };
	
	/* 원형(레벨별로 생성하되 하나의 컨테이너) 및 클론 컨테이너(레벨별 컨테이너) */
	map<const wstring, class CGameObject*>		m_Prototypes;				/* 원형 객체를 레벨별로 보관 <레벨 이름, 객체 이름> */
	LAYERS*										m_pLayers = { nullptr };	/* 사본 객체들을 레벨별로 보관. -맵 배열(레벨별)이다. 맵은 <레이어 이름(오브젝트 종류 이름) - 해당 종류 오브젝트 리스트> 구성되어 있다.*/

	/* Obj Event */
	list<EVENT_DESC>							m_Events[END_EVT];

	/* Pool Event */
	POOLS*										m_pPools = { nullptr }; /* 레벨 별, 오브젝트별 풀을 생성한다. 맵 배열 구성이며, 하나의 맵이 하나의 레벨을 뜻하며, 키는 오브젝트 이름, 밸류는 오브젝트 벡터이다. */

private:
	class CGameObject* Find_Prototype(const wstring& strPrototypeTag);
	class CLayer* Find_Layer(_uint iLevelIndex, const wstring& strLayerTag);
	queue<class CGameObject*>* Find_Pool(_uint iLevelIndex, const wstring& strPrototypeTag);

public:
	virtual void Free() override;
};

END