#include "../Default/stdafx.h"

#include "BattleManager.h"
#include "EngineInstance.h"

IMPLEMENT_SINGLETON(CBattleManager)

CBattleManager::CBattleManager()
{
}

HRESULT CBattleManager::Initialize()
{
	return S_OK;
}


void CBattleManager::Free()
{
	__super::Free();
}

