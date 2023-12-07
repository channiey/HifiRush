#include "..\Default\stdafx.h"
#include "..\Public\TriggerSection_B.h"

#include "EngineInstance.h"
#include "Util_String.h"

#include "BattleManager.h"

#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG

#include "UiManager.h"

#include "Enemy.h"


CTriggerSection_B::CTriggerSection_B(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTriggerBattle(pDevice, pContext)
{
}

CTriggerSection_B::CTriggerSection_B(const CTriggerSection_B& rhs)
	: CTriggerBattle(rhs)
{
}

HRESULT CTriggerSection_B::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTriggerSection_B::Initialize(void* pArg)
{
	m_strTriggerTag = BattelTriggerNames[BATTLE_TRIGGER_TYPE::SECTION_B];

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	CBattleManager::GetInstance()->Add_Trigger(m_strTriggerTag, this);

	if (FAILED(Pop_Enemy()))
		return E_FAIL;

	return S_OK;
}

void CTriggerSection_B::Tick(_double fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bBattle)
		Update_Battle(fTimeDelta);
}

void CTriggerSection_B::LateTick(_double fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}


HRESULT CTriggerSection_B::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CTriggerSection_B::Start_Battle()
{
	/*const _uint iCurLevel = ENGINE_INSTANCE->Get_CurLevelIndex();


	for (auto Pair : m_Flows)
	{
		vector<CGameObject*> Clones;
		for (auto desc : Pair.second)
		{
			CGameObject* pClone = ENGINE_INSTANCE->Pop_Pool(iCurLevel, desc.strPrototypeTag);

			if (nullptr != pClone)
			{
				pClone->Get_Transform()->Set_WorldMat(desc.matWorld);
				pClone->Get_NavMeshAgent()->Set_CurIndex(desc.iCellIndex);
				Clones.push_back(pClone);

				static_cast<CEnemy*>(pClone)->Set_EnemyActive(TRUE);

			}
		}
		m_Clones.push_back(Clones);
	}*/

	for (auto& pEnemy : m_Clones.front())
	{
		static_cast<CEnemy*>(pEnemy)->Set_EnemyActive(TRUE);
	}

	return S_OK;
}

HRESULT CTriggerSection_B::Update_Battle(_double fTimeDelta)
{

	return S_OK;
}

HRESULT CTriggerSection_B::Finish_Battle()
{
	// Start 원상복귀

	return S_OK;
}

void CTriggerSection_B::OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild)
{
	if (nullptr != pCollider->Get_Owner())
	{
		if (L"Player_Chai_000" == pCollider->Get_Owner()->Get_Name())
		{
			CBattleManager::GetInstance()->OnTrigger_Enter(m_strTriggerTag);
			CUiManager::GetInstance()->On_Dialouge(0, L"이 녀석들인가 보군");

		}
	}
}

void CTriggerSection_B::OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild)
{
}

void CTriggerSection_B::OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild)
{
}

HRESULT CTriggerSection_B::Ready_Components()
{
	return S_OK;
}

CTriggerSection_B* CTriggerSection_B::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTriggerSection_B* pInstance = new CTriggerSection_B(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTriggerSection_B");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTriggerSection_B::Clone(void* pArg)
{
	CTriggerSection_B* pInstance = new CTriggerSection_B(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTriggerSection_B");
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CTriggerSection_B::Free()
{
	__super::Free();

}