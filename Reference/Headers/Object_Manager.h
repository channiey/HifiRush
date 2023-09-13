#pragma once

#include "Base.h"

/* ���� ���� ���� ��ü���� ���������� ������ü���� �����Ѵ�.(����) */
/* ���� ���� ���� ��ü���� �����Ѵ�.(�纻) */

BEGIN(Engine)

class CObject_Manager final : public CBase
{
	DECLARE_SINGLETON(CObject_Manager)

private:
	enum EVENT_TYPE { OBJ_ADD, OBJ_DEL, POP_FROM_POOL, RETURN_TO_POOL, END_EVT };

	typedef struct EventDesc
	{
		_uint				iLevelIndex	= 0;
		class CGameObject*	pObj		= { nullptr };

		EventDesc(const _uint& _iLevelIndex, class CGameObject* _pObj)
			: iLevelIndex(_iLevelIndex), pObj(_pObj) 
		{
			Safe_AddRef(pObj);
		}

	}EVENT_DESC;

private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	HRESULT Reserve_Manager(_uint iNumLevels);
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);
	void FinishTick(); 

public: 
	map<const wstring, class CGameObject*>* Get_Prototypes() { return &m_Prototypes; } 
	map<const wstring, class CLayer*>* Get_Layers(_uint iLevelIndex);						/* Ư�� ������ ��� ���̾ �����´�. */
	list<class CGameObject*>* Get_Objects(_uint iLevelIndex, const wstring& strLayerTag);	/* Ư�� ������ Ư�� ���̾ �����´�. */
	class CGameObject* Get_Player();

public:
	/* ���� ������Ʈ ���� */
	HRESULT	Add_Prototype(const wstring& strPrototypeTag, class CGameObject* pPrototype);	/* ������ �����Ͽ� ���� ���Ϳ� �߰��Ѵ�. (Ŭ�� �ε����� �����Ͽ� �޾ƿ´�) */

	/* Ŭ�� ������Ʈ�� �߰�, ���� �̺�Ʈ */
	HRESULT	Add_GameObject(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, void* pArg); /* �纻�� �����Ͽ� ���̾ �߰��Ѵ�. - ������ ������ ��� ���ӿ�����Ʈ�� �� �Լ��� ���� ����(����)�ȴ�. */
	HRESULT	Delete_GameObject(_uint iLevelIndex, class CGameObject* pObj);					/* TODO ���� ���̵�� �ص� ������ �� */

	/* Ŭ�� ������Ʈ�� Ǯ�� �̺�Ʈ */
	HRESULT	Reserve_Pool(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, const _uint& iNumObj, void* pArg); /* �ش� ������ ������ ������ �ڿ� ȣ��ȴ�. */
	class CGameObject* Pop_Pool(_uint iLevelIndex, const wstring& strPrototypeTag);
	HRESULT	Return_Pool(_uint iLevelIndex, class CGameObject* pObj);

	void Clear(_uint iLevelIndex);

private:
	_uint													m_iNumLevels = { 0 };
	
	/* ���� �� Ŭ�� �����̳� */
	map<const wstring, class CGameObject*>					m_Prototypes;				/* ���� ��ü�� �������� ���� <���� �̸�, ��ü �̸�> */
	map<const wstring, class CLayer*>*						m_pLayers = { nullptr };	/* �纻 ��ü���� �������� ����. -�� �迭(������)�̴�. ���� <���̾� �̸�(������Ʈ ���� �̸�) - �ش� ���� ������Ʈ ����Ʈ> �����Ǿ� �ִ�.*/
	typedef map<const wstring, class CLayer*>				LAYERS;

	/* �߰�, ���� �̺�Ʈ �����̳� */
	list<EVENT_DESC>										m_Events[END_EVT];

	/* Ǯ�� �̺�Ʈ �����̳� */
	map<const wstring, queue<class CGameObject*>*>*			m_pPools = { nullptr }; /* ���� ��, ������Ʈ�� Ǯ�� �����Ѵ�. �� �迭 �����̸�, �ϳ��� ���� �ϳ��� ������ ���ϸ�, Ű�� ������Ʈ �̸�, ����� ������Ʈ �����̴�. */
	typedef map<const wstring, queue<class CGameObject*>*>	POOLS;
	typedef queue<class CGameObject*>						POOL;

private:
	class CGameObject* Find_Prototype(const wstring& strPrototypeTag);
	class CLayer* Find_Layer(_uint iLevelIndex, const wstring& strLayerTag);
	queue<class CGameObject*>* Find_Pool(_uint iLevelIndex, const wstring& strPrototypeTag);

public:
	virtual void Free() override;
};

END