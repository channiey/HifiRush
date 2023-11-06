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

	return S_OK;
}

void CBattleManager::OnTrigger_Enter(const wstring strTriggerTag)
{
	CTriggerBattle* pTriggerBattle = Find_Trigger(strTriggerTag);
	
	if (nullptr == pTriggerBattle)
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
	return S_OK;
}


void CBattleManager::Free()
{
	__super::Free();
}

