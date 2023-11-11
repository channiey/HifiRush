#include "../Default/stdafx.h"

#include "BattleManager.h"
#include "EngineInstance.h"

#include "TriggerBattle.h"

IMPLEMENT_SINGLETON(CBattleManager)

CBattleManager::CBattleManager()
{
}

HRESULT CBattleManager::Initialize()
{
	if (FAILED(Ready_Pool()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBattleManager::Update(const _double fTimedelta)
{
	return S_OK;
}

HRESULT CBattleManager::LateUpdate(const _double fTimedelta)
{
	return S_OK;
}

HRESULT CBattleManager::Add_Trigger(const wstring strTriggerTag, CTriggerBattle* pTrigger)
{
	if (nullptr == pTrigger)
		return E_FAIL;

	m_BattelTriggers.emplace(strTriggerTag, pTrigger);

	return S_OK;
}

void CBattleManager::OnTrigger_Enter(const wstring strTriggerTag)
{
	CTriggerBattle* pTriggerBattle = Find_Trigger(strTriggerTag);
	
	if (nullptr == pTriggerBattle || pTriggerBattle->Is_StartBattle())
		return;

	m_pCurBattleTriggers = pTriggerBattle;
	m_pCurBattleTriggers->Start_Battle();
	m_bBattle = TRUE;
}

void CBattleManager::OnTrigger_Stay(const wstring strTriggerTag)
{
}

void CBattleManager::OnTrigger_Exit(const wstring strTriggerTag)
{
}

CTriggerBattle* CBattleManager::Find_Trigger(const wstring strTriggerTag)
{
	auto iter = m_BattelTriggers.find(strTriggerTag);

	if(iter == m_BattelTriggers.end())
		return nullptr;

	return iter->second;
}

HRESULT CBattleManager::Remove_Trigger(const wstring strTriggerTag)
{
	return S_OK;
}

HRESULT CBattleManager::Ready_Pool()
{
 	if (FAILED(ENGINE_INSTANCE->Reserve_Pool(LEVEL_ID::LV_STAGE_01, LayerNames[LAYER_ENEMY], L"Enemy_Saber", 5)))
		return E_FAIL;

	if (FAILED(ENGINE_INSTANCE->Reserve_Pool(LEVEL_ID::LV_STAGE_01, LayerNames[LAYER_ENEMY], L"Enemy_Blader", 1)))
		return E_FAIL;
	
	return S_OK;
}


void CBattleManager::Free()
{
	__super::Free();
}

