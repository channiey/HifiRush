#include "..\Public\Object_Manager.h"
#include "Layer.h"
#include "GameObject.h"

IMPLEMENT_SINGLETON(CObject_Manager)

CObject_Manager::CObject_Manager()
{
}

HRESULT CObject_Manager::Reserve_Manager(_uint iNumLevels)
{
	if (nullptr != m_pLayers)
		return E_FAIL;

	m_pLayers = new LAYERS[iNumLevels];

	m_pPools = new POOLS[iNumLevels];

	m_iNumLevels = iNumLevels;

	return S_OK;
}

HRESULT CObject_Manager::Add_Prototype(const wstring & strPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr != Find_Prototype(strPrototypeTag))
		return E_FAIL;

	/* �̸��� �����Ѵ�. */
	pPrototype->Set_Name(strPrototypeTag);

	m_Prototypes.emplace(strPrototypeTag, pPrototype);

	return S_OK;
}

HRESULT CObject_Manager::Add_GameObject(_uint iLevelIndex, const wstring & strLayerTag, const wstring & strPrototypeTag, void * pArg)
{
	/* ������ �纻�� ã�´�. */
	CGameObject*		pPrototype = Find_Prototype(strPrototypeTag);
	if (nullptr == pPrototype)
		return E_FAIL;

	/* ������ �����Ͽ� �纻�� �����. */
	CGameObject*		pGameObject = pPrototype->Clone(pArg);
	if (nullptr == pGameObject)
		return E_FAIL;

	/* �̸��� ���̾ �����Ѵ�. */
	pGameObject->Set_Name(strPrototypeTag);
	pGameObject->Set_Layer(strLayerTag);

	/* �̺�Ʈ�� ������ �� ����Ʈ�� �߰��Ѵ�. */
	EVENT_DESC tEvent(iLevelIndex, pGameObject);
	m_Events[OBJ_ADD].push_back(tEvent);

	return S_OK;
}

HRESULT CObject_Manager::Delete_GameObject(_uint iLevelIndex, CGameObject* pObj)
{
	NULL_CHECK_RETURN(pObj, E_FAIL);

	/* �̺�Ʈ�� ������ �� ����Ʈ�� �߰��Ѵ�. */
	EVENT_DESC tEvent(iLevelIndex, pObj);
	m_Events[OBJ_DEL].push_back(tEvent);

	return S_OK;
}

HRESULT CObject_Manager::Reserve_Pool(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, const _uint& iNumObj, void* pArg)
{
	/* ������ �纻�� ã�´�. */
	CGameObject* pPrototype = Find_Prototype(strPrototypeTag);
	if (nullptr == pPrototype)
		return E_FAIL;

	/* �ش� ������Ʈ �̸����� Ǯ�� �����Ѵ�. */
	POOL* pPool = new POOL;
	if (nullptr == pPool)
		return E_FAIL;

	/* ������ �ش��ϴ� �ʿ� Ǯ�� �߰��Ѵ�. */
	m_pPools[iLevelIndex].emplace(strPrototypeTag, pPool);

	/* Ǯ�� ������Ʈ�� Ŭ���Ͽ� �߰��Ѵ�. */
	for (size_t i = 0; i < iNumObj; i++)
	{
		/* ������ �����Ͽ� �纻�� �����. */
		CGameObject* pGameObject = pPrototype->Clone(pArg);
		if (nullptr == pGameObject)
			return E_FAIL;

		/* �̸��� ���̾ �����Ѵ�. */
		pGameObject->Set_Name(strPrototypeTag);
		pGameObject->Set_Layer(strLayerTag);

		pPool->push(pGameObject);
	}

	return S_OK;
}

CGameObject* CObject_Manager::Pop_Pool(_uint iLevelIndex, const wstring& strPrototypeTag)
{
	/* Ǯ�� �����´�. */
	POOL* pPool = Find_Pool(iLevelIndex, strPrototypeTag);

	if (nullptr == pPool || pPool->empty())
		return nullptr;

	/* Ǯ���� ������Ʈ�� ������. */
	CGameObject* pObj = pPool->front();

	if (nullptr == pObj) 
		return nullptr;
	
	Safe_AddRef(pObj);

	pPool->pop();

	/* �̺�Ʈ�� �߰��Ѵ�. */
	EVENT_DESC tEvent(iLevelIndex, pObj);
	m_Events[POP_FROM_POOL].push_back(tEvent);

	Safe_Release(pObj);

	/* �ش� ������Ʈ�� �̹� ������ �������� �̺�Ʈ�� ó���� �� Ȱ��ȭ �ȴ�. */

	return pObj;
}

HRESULT CObject_Manager::Return_Pool(_uint iLevelIndex, CGameObject* pObj)
{
	NULL_CHECK_RETURN(pObj, E_FAIL);

	POOL* pPool = Find_Pool(iLevelIndex, pObj->Get_Name());

	if (nullptr == pPool)
		return E_FAIL;

	pObj->Set_Active(FALSE);

	pPool->push(pObj);

	/* �̺�Ʈ�� �߰��Ѵ�. */
	EVENT_DESC tEvent(iLevelIndex, pObj);
	m_Events[RETURN_TO_POOL].push_back(tEvent);

	return S_OK;
}

void CObject_Manager::Tick(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
		{
			if(nullptr != Pair.second)
				Pair.second->Tick(fTimeDelta);		
		}
	}
}

void CObject_Manager::LateTick(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
		{
			if (nullptr != Pair.second)
				Pair.second->LateTick(fTimeDelta);
		}
	}
}

void CObject_Manager::FinishTick()
{
	/* 1-1. ���� �̺�Ʈ */
	for (auto& iter : m_Events[OBJ_DEL])
	{
		CLayer* pLayer = Find_Layer(iter.iLevelIndex, iter.pObj->Get_Layer());

		if (nullptr != pLayer)
			pLayer->Delete_GameObject(iter.pObj);

		Safe_Release(iter.pObj);
	}
	m_Events[OBJ_DEL].clear();


	/* 1-2. Retuen �̺�Ʈ (layer To pool) */
	for (auto& iter : m_Events[RETURN_TO_POOL])
	{
		CLayer* pLayer = Find_Layer(iter.iLevelIndex, iter.pObj->Get_Layer());

		if (nullptr != pLayer)
			pLayer->Erase_GameObject(iter.pObj);

		Safe_Release(iter.pObj);
	}
	m_Events[RETURN_TO_POOL].clear();


	/* 2-1. Pop �̺�Ʈ (pool To Layer) */
	for (auto& iter : m_Events[POP_FROM_POOL])
	{
		CLayer* pLayer = Find_Layer(iter.iLevelIndex, iter.pObj->Get_Layer());

		if (nullptr != pLayer)
			pLayer->Push_GameObject(iter.pObj);

		iter.pObj->Set_Active(TRUE);
		
		Safe_Release(iter.pObj);
	}
	m_Events[POP_FROM_POOL].clear();


	/* 2-2. �߰� �̺�Ʈ */
	for (auto& iter : m_Events[OBJ_ADD])
	{
		CLayer* pLayer = Find_Layer(iter.iLevelIndex, iter.pObj->Get_Layer());

		if (nullptr == pLayer)
		{
			pLayer = CLayer::Create();

			pLayer->Add_GameObject(iter.pObj);

			m_pLayers[iter.iLevelIndex].emplace(iter.pObj->Get_Layer(), pLayer);
		}
		else 
			pLayer->Add_GameObject(iter.pObj);

		Safe_Release(iter.pObj);
	}

	m_Events[OBJ_ADD].clear();
}

map<const wstring, class CLayer*>* CObject_Manager::Get_Layers(_uint iLevelIndex)
{
	if (iLevelIndex >= m_iNumLevels)
		return nullptr;

	return &m_pLayers[iLevelIndex];
}

list<class CGameObject*>* CObject_Manager::Get_Objects(_uint iLevelIndex, const wstring& strLayerTag)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);

	if (nullptr == pLayer)
		return nullptr;

	return pLayer->Get_Objects();
}

CGameObject* CObject_Manager::Get_Player()
{
	return nullptr;
}

void CObject_Manager::Clear(_uint iLevelIndex)
{
	for (auto& Pair : m_pLayers[iLevelIndex])
	{
		Safe_Release(Pair.second);
	}
	m_pLayers[iLevelIndex].clear();
}

CGameObject * CObject_Manager::Find_Prototype(const wstring & strPrototypeTag)
{
	auto iter = m_Prototypes.find(strPrototypeTag);

	if (iter == m_Prototypes.end())
		return nullptr;

	return iter->second;
}

CLayer * CObject_Manager::Find_Layer(_uint iLevelIndex, const wstring & strLayerTag)
{
	if (iLevelIndex >= m_iNumLevels)
		return nullptr;

	auto iter = m_pLayers[iLevelIndex].find(strLayerTag);

	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;

	return iter->second;
}

queue<class CGameObject*>* CObject_Manager::Find_Pool(_uint iLevelIndex, const wstring& strPrototypeTag)
{
	if (iLevelIndex >= m_iNumLevels)
		return nullptr;

	auto iter = m_pPools[iLevelIndex].find(strPrototypeTag);

	if (iter == m_pPools[iLevelIndex].end())
		return nullptr;

	return iter->second;
}

void CObject_Manager::Free()
{
	__super::Free();

	/* �纻�� ���� */
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Safe_Release(Pair.second);
		}
		m_pLayers[i].clear();
	}
	Safe_Delete_Array(m_pLayers);


	/* Ǯ�� ���� */
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for(auto& Pair : m_pPools[i])
		{
			while (!Pair.second->empty())
			{
				Safe_Release(Pair.second->front());
				Pair.second->pop();
			}
		}
		m_pPools[i].clear();
	}
	Safe_Delete_Array(m_pPools);


	/* ������ ���� */
	for (auto& Pair : m_Prototypes)
		Safe_Release(Pair.second);

	m_Prototypes.clear();
}
