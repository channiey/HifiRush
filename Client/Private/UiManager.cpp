#include "../Default/stdafx.h"

#include "UiManager.h"
#include "EngineInstance.h"

#include "Ui_Dialouge.h"

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

HRESULT CUiManager::On_Dialouge(_uint eCharacterType, const wstring& strText, _bool bSound, _uint iSoundID, _uint iChannelID, _float fVolume)
{
	if (nullptr == m_pDialouge)
	{
		CUi* pUi = Find_UI(UI_ID::UI_DIALOUGE);
		if (nullptr == pUi)
			return E_FAIL;
		else
		{
			m_pDialouge = dynamic_cast<CUi_Dialouge*>(pUi);
			if (nullptr == m_pDialouge) 
				return E_FAIL;
		}
	}
	return m_pDialouge->On_Dialouge(eCharacterType, strText, bSound, iSoundID, iChannelID, fVolume);
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

