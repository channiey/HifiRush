#include "../Default/stdafx.h"

#include "UiManager.h"
#include "EngineInstance.h"

IMPLEMENT_SINGLETON(CUiManager)

CUiManager::CUiManager()
{
}

HRESULT CUiManager::Initialize()
{
	return S_OK;
}

CUi* CUiManager::Get_UI(const UI_ID& eID)
{
	return Find_UI(eID);
}

HRESULT CUiManager::Add_UI(const UI_ID& eID, CUi* pUi)
{
	if (nullptr != Find_UI(eID))
		return E_FAIL;

	m_UIs.emplace(eID, pUi);

	return S_OK;
}

CUi* CUiManager::Find_UI(const UI_ID& eID)
{
	auto iter = m_UIs.find(eID);

	if (iter == m_UIs.end())
		return nullptr;

	return iter->second;
}


void CUiManager::Free()
{
	__super::Free();
}

