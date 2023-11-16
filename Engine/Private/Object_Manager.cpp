#include "..\Public\Object_Manager.h"
#include "Layer.h"

#include "EngineInstance.h"
#include "Util_String.h"

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

	/* 이름을 설정한다. */
	pPrototype->Set_Name(strPrototypeTag);

	m_Prototypes.emplace(strPrototypeTag, pPrototype);

	return S_OK;
}

class CGameObject* CObject_Manager::Add_GameObject(_uint iLevelIndex, const wstring & strLayerTag, const wstring & strPrototypeTag, void * pArg)
{
	CGameObject* pPrototype = nullptr;

	/* 복제할 원본 찾는다. */
	/* 클론 고유 번호가 존재하는 경우, 원형을 찾기 위해 클론 고유 번호를 지운다. */
	if (Util_String::Has_ClonePin(strPrototypeTag))
		pPrototype = Find_Prototype(Util_String::Remove_LastNumChar(strPrototypeTag, CLONE_PIN_MAX_DIGIT));
	else
		pPrototype = Find_Prototype(strPrototypeTag);

	if (nullptr == pPrototype)
		return nullptr;

	/* 원형을 복제하여 사본을 만든다. */
	CGameObject*		pGameObject = pPrototype->Clone(pArg);
	if (nullptr == pGameObject)
		return nullptr;

	/* 클론 고유 번호를 지운 이름을 세팅한다. */
	if (Util_String::Has_ClonePin(strPrototypeTag))
		pGameObject->Set_Name(Util_String::Remove_LastNumChar(strPrototypeTag, CLONE_PIN_MAX_DIGIT));
	else
		pGameObject->Set_Name(strPrototypeTag);

	pGameObject->Set_LayerTag(strLayerTag);

	/* 이벤트를 설정한 뒤 리스트에 추가한다. */
	EVENT_DESC tEvent(iLevelIndex, pGameObject);
	m_Events[OBJ_ADD].push_back(tEvent);

	return pGameObject;
}

HRESULT CObject_Manager::Delete_GameObject(_uint iLevelIndex, CGameObject* pObj)
{
	NULL_CHECK_RETURN(pObj, E_FAIL);

	/* 이벤트를 설정한 뒤 리스트에 추가한다. */
	EVENT_DESC tEvent(iLevelIndex, pObj);
	m_Events[OBJ_DEL].push_back(tEvent);

	return S_OK;
}

HRESULT CObject_Manager::Reserve_Pool(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, const _uint& iNumObj, void* pArg)
{
	/* 복제할 사본을 찾는다. */
	CGameObject* pPrototype = Find_Prototype(strPrototypeTag);
	if (nullptr == pPrototype)
		return E_FAIL;

	/* 해당 오브젝트 이름으로 풀을 생성한다. */
	POOL* pPool = new POOL;
	if (nullptr == pPool)
		return E_FAIL;

	/* 레벨에 해당하는 맵에 풀을 추가한다. */
	m_pPools[iLevelIndex].emplace(strPrototypeTag, pPool);

	/* 풀에 오브젝트를 클론하여 추가한다. */
	for (size_t i = 0; i < iNumObj; i++)
	{
		/* 원형을 복제하여 사본을 만든다. */
		CGameObject* pGameObject = pPrototype->Clone(pArg);
		if (nullptr == pGameObject)
			return E_FAIL;

		/* 이름과 레이어를 세팅한다. */
		pGameObject->Set_Name(strPrototypeTag);
		pGameObject->Set_LayerTag(strLayerTag);

		pPool->push(pGameObject);
	}

	return S_OK;
}

CGameObject* CObject_Manager::Pop_Pool(_uint iLevelIndex, const wstring& strPrototypeTag)
{
	/* 풀을 가져온다. */
	POOL* pPool = Find_Pool(iLevelIndex, strPrototypeTag);

	if (nullptr == pPool || pPool->empty())
		return nullptr;

	/* 풀에서 오브젝트를 꺼낸다. */
	CGameObject* pObj = pPool->front();

	if (nullptr == pObj) 
		return nullptr;
	
	Safe_AddRef(pObj);

	pPool->pop();

	/* 이벤트를 추가한다. */
	EVENT_DESC tEvent(iLevelIndex, pObj);
	m_Events[POP_FROM_POOL].push_back(tEvent);

	Safe_Release(pObj);

	/* 해당 오브젝트는 이번 프레임 마지막에 이벤트가 처리될 때 활성화 된다. */

	return pObj;
}

HRESULT CObject_Manager::Return_Pool(_uint iLevelIndex, CGameObject* pObj)
{
	NULL_CHECK_RETURN(pObj, E_FAIL);

	pObj->Set_Name(Util_String::Remove_LastNumChar(pObj->Get_Name(), CLONE_PIN_MAX_DIGIT));

	POOL* pPool = Find_Pool(iLevelIndex, pObj->Get_Name());

	if (nullptr == pPool)
		return E_FAIL;

	pObj->Set_State(CGameObject::STATE_UNACTIVE);

	pPool->push(pObj);

	/* 이벤트를 추가한다. */
	EVENT_DESC tEvent(iLevelIndex, pObj);
	m_Events[RETURN_TO_POOL].push_back(tEvent);

	return S_OK;
}

void CObject_Manager::Tick(_double fTimeDelta)
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

void CObject_Manager::LateTick(_double fTimeDelta)
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
	/* 0-1 레이어 삭제 이벤트 */
	for (auto& iterEvent : m_Events[LAYER_DEL])
	{
		m_pLayers[iterEvent.iLevelIndex].erase(iterEvent.pLayer->Get_Name());

		Safe_Release(iterEvent.pLayer);
	}
	m_Events[LAYER_DEL].clear();


	/* 0-2 레이어 추가 이벤트 */
	for (auto& iterEvent : m_Events[LAYER_ADD])
	{
		/* 혹시라도 그 전에 추가된 레이어가 있는지 한번 더 검사한다. */
		if (nullptr == Find_Layer(iterEvent.iLevelIndex, iterEvent.pLayer->Get_Name()))
		{
			m_pLayers[iterEvent.iLevelIndex].emplace(iterEvent.pLayer->Get_Name(), iterEvent.pLayer);
		}
	}
	m_Events[LAYER_ADD].clear();


	/* 1-1. 오브젝트 삭제 이벤트 */
	for (auto& iter : m_Events[OBJ_DEL])
	{
		CLayer* pLayer = Find_Layer(iter.iLevelIndex, iter.pObj->Get_LayerTag());

		if (nullptr != pLayer)
			pLayer->Delete_GameObject(iter.pObj);

		Safe_Release(iter.pObj);
	}
	m_Events[OBJ_DEL].clear();


	/* 1-2. 오브젝트 Return 이벤트 (layer To pool) */
	for (auto& iter : m_Events[RETURN_TO_POOL])
	{
		CLayer* pLayer = Find_Layer(iter.iLevelIndex, iter.pObj->Get_LayerTag());

		if (nullptr != pLayer)
			pLayer->Erase_GameObject(iter.pObj);

		/* 클론 고유 번호를 지운다. (이름만 남겨둔다) */
		//iter.pObj->Set_Name(Util_String::Remove_LastNumChar(iter.pObj->Get_Name(), CLONE_PIN_MAX_DIGIT));

		Safe_Release(iter.pObj);
	}
	m_Events[RETURN_TO_POOL].clear();


	/* 2-1. 오브젝트 Pop 이벤트 (pool To Layer) */
	for (auto& iter : m_Events[POP_FROM_POOL])
	{
		CLayer* pLayer = Find_Layer(iter.iLevelIndex, iter.pObj->Get_LayerTag());

		if (nullptr == pLayer)
		{
			pLayer = CLayer::Create(iter.pObj->Get_LayerTag());
			m_pLayers[iter.iLevelIndex].emplace(iter.pObj->Get_LayerTag(), pLayer);
		}
		/* 클론 고유번호를 부여한다.*/
		iter.pObj->Set_Name(pLayer->Get_CloneNameWithPin(iter.pObj->Get_Name()));
		iter.pObj->Set_State(CGameObject::STATE_ACTIVE);
		pLayer->Push_GameObject(iter.pObj);
		
		Safe_Release(iter.pObj);
	}
	m_Events[POP_FROM_POOL].clear();


	/* 2-2. 오브젝트 추가 이벤트 */
	for (auto& iter : m_Events[OBJ_ADD])
	{
		CLayer* pLayer = Find_Layer(iter.iLevelIndex, iter.pObj->Get_LayerTag());

		if (nullptr == pLayer)
		{
			pLayer = CLayer::Create(iter.pObj->Get_LayerTag());

			/* 클론 고유번호를 부여한다.*/
			iter.pObj->Set_Name(pLayer->Get_CloneNameWithPin(iter.pObj->Get_Name()));

			pLayer->Add_GameObject(iter.pObj);

			m_pLayers[iter.iLevelIndex].emplace(iter.pObj->Get_LayerTag(), pLayer);
		}
		else
		{
			/* 클론 고유번호를 부여한다.*/
			iter.pObj->Set_Name(pLayer->Get_CloneNameWithPin(iter.pObj->Get_Name()));
			pLayer->Add_GameObject(iter.pObj);
		}

		Safe_Release(iter.pObj);
	}

	m_Events[OBJ_ADD].clear();
}

map<const wstring, class CLayer*>* CObject_Manager::Get_All_Layer(_uint iLevelIndex)
{
	if (iLevelIndex >= m_iNumLevels)
		return nullptr;

	return &m_pLayers[iLevelIndex];
}

list<class CGameObject*>* CObject_Manager::Get_Layer(_uint iLevelIndex, const wstring& strLayerTag)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);

	if (nullptr == pLayer)
		return nullptr;

	return pLayer->Get_Objects();
}

CLayer* CObject_Manager::Get_LayerClass(_uint iLevelIndex, const wstring& strLayerTag)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);

	if (nullptr == pLayer)
		return nullptr;

	return pLayer;
}

CGameObject* CObject_Manager::Get_Player()
{
	return nullptr;
}

CGameObject* CObject_Manager::Get_GameObject(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);

	if (nullptr == pLayer)
		return nullptr;

	return pLayer->Get_GameObject(strPrototypeTag);
}

const _int CObject_Manager::Get_ObjectIndex(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strName)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);

	if (nullptr == pLayer)
		return -1;

	return pLayer->Get_ObjectIndex(strName);
}

HRESULT CObject_Manager::Add_Layer(_uint iLevelIndex, const wstring& strLayerTag)
{
	if (iLevelIndex >= m_iNumLevels)
		return E_FAIL;

	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);

	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create(strLayerTag);
		
		/* 이벤트를 추가한다. */
		EVENT_DESC tEvent(iLevelIndex, pLayer);
		m_Events[LAYER_ADD].push_back(tEvent);

		return S_OK;
	}
	else
		return E_FAIL;
}

HRESULT CObject_Manager::Delete_Layer(_uint iLevelIndex, const wstring& strLayerTag)
{
	if (iLevelIndex >= m_iNumLevels)
		return E_FAIL;

	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);

	if (nullptr == pLayer)
		return E_FAIL;
	else
	{
		/* 이벤트를 추가한다. */
		EVENT_DESC tEvent(iLevelIndex, pLayer);
		m_Events[LAYER_DEL].push_back(tEvent);
	}

	return S_OK;
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

	/* 사본의 삭제 */
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Safe_Release(Pair.second);
		}
		m_pLayers[i].clear();
	}
	Safe_Delete_Array(m_pLayers);


	/* 풀의 삭제 */
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


	/* 원형의 삭제 */
	for (auto& Pair : m_Prototypes)
		Safe_Release(Pair.second);

	m_Prototypes.clear();
}
