#include "../Default/stdafx.h"

#include "GameManager.h"
#include "EngineInstance.h"

IMPLEMENT_SINGLETON(CGameManager)

CGameManager::CGameManager()
{
}

HRESULT CGameManager::Initialize()
{
	return S_OK;
}


void CGameManager::Free()
{
	__super::Free();
}

