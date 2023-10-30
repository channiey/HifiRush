#include "../Default/stdafx.h"

#include "BeatManager.h"
#include "EngineInstance.h"

IMPLEMENT_SINGLETON(CBeatManager)

CBeatManager::CBeatManager()
{
}

HRESULT CBeatManager::Initialize()
{
	return S_OK;
}


void CBeatManager::Free()
{
	__super::Free();
}

