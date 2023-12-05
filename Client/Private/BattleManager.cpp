#include "../Default/stdafx.h"

#include "BattleManager.h"
#include "EngineInstance.h"

#include "TriggerBattle.h"
#include "UiManager.h"
#include "Ui.h"

#include "Enemy.h"

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

const _bool CBattleManager::Is_In_Battle()
{
	list<CGameObject*>* pEnemies = ENGINE_INSTANCE->Get_Layer(ENGINE_INSTANCE->Get_CurLevelIndex(), LayerNames[LAYER_ID::LAYER_ENEMY]);

	if (nullptr != pEnemies && !pEnemies->empty())
	{
		for (auto& pEnemy : *pEnemies)
		{
			if (static_cast<CEnemy*>(pEnemy)->Is_EnemyActive())
				return TRUE;
		}
	}

	return FALSE;
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

	CUiManager::GetInstance()->Get_UI(UI_ID::UI_FIGHT)->Set_State(CGameObject::OBJ_STATE::STATE_ACTIVE);

	m_pCurBattleTriggers = pTriggerBattle;
	m_pCurBattleTriggers->Start_Battle();
	m_pCurBattleTriggers->Get_Collider_Sphere()->Set_Active(FALSE);
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
 	if (FAILED(ENGINE_INSTANCE->Reserve_Pool(LEVEL_ID::LV_STAGE_01, LayerNames[LAYER_ENEMY], L"Enemy_Saber", 3)))
		return E_FAIL;

	if (FAILED(ENGINE_INSTANCE->Reserve_Pool(LEVEL_ID::LV_STAGE_01, LayerNames[LAYER_ENEMY], L"Enemy_Blader", 1)))
		return E_FAIL;

	if (FAILED(ENGINE_INSTANCE->Reserve_Pool(LEVEL_ID::LV_STAGE_01, LayerNames[LAYER_ENEMY], L"Enemy_Gunner", 3)))
		return E_FAIL;
	
	return S_OK;
}


void CBattleManager::Free()
{
	__super::Free();
}

