#include "..\Public\Layer.h"
#include "GameObject.h"

CLayer::CLayer()
{
}

HRESULT CLayer::Initialize(const wstring& strLayerTag)
{
	m_strName = strLayerTag;

	return S_OK;
}

HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_GameObjects.push_back(pGameObject);

	return S_OK;
}

HRESULT CLayer::Delete_GameObject(CGameObject* pGameObject)
{
	auto iter = find(m_GameObjects.begin(), m_GameObjects.end(), pGameObject);

	if (m_GameObjects.end() == iter)
		return E_FAIL;

	m_GameObjects.erase(iter);
	Safe_Release(pGameObject); /* TODO : iter는 Safe_Release()가 안되네? */

	return S_OK;
}

HRESULT CLayer::Erase_GameObject(CGameObject* pGameObject)
{
	auto iter = find(m_GameObjects.begin(), m_GameObjects.end(), pGameObject);

	if (m_GameObjects.end() == iter)
		return E_FAIL;

	m_GameObjects.erase(iter);

	return S_OK;
}

HRESULT CLayer::Push_GameObject(CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_GameObjects.push_back(pGameObject);

	return S_OK;
}

void CLayer::Tick(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject && pGameObject->Is_Active())
			pGameObject->Tick(fTimeDelta);
	}
}

void CLayer::LateTick(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{		
		if (nullptr != pGameObject && pGameObject->Is_Active())
			pGameObject->LateTick(fTimeDelta);
	}
}

CLayer * CLayer::Create(const wstring& strLayerTag)
{
	CLayer*	pInstance = new CLayer();

	if (FAILED(pInstance->Initialize(strLayerTag)))
	{
		MSG_BOX("Failed to Created : CLayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLayer::Free()
{
	__super::Free();

	for (auto& pGameObject : m_GameObjects)
		Safe_Release(pGameObject);

	m_GameObjects.clear();
}
