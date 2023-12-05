#include "..\Default\stdafx.h"
#include "..\Public\TriggerSection_C.h"

#include "EngineInstance.h"
#include "Util_String.h"

#include "BattleManager.h"

#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG

#include "Enemy.h"


CTriggerSection_C::CTriggerSection_C(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTriggerBattle(pDevice, pContext)
{
}

CTriggerSection_C::CTriggerSection_C(const CTriggerSection_C& rhs)
	: CTriggerBattle(rhs)
{
}

HRESULT CTriggerSection_C::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTriggerSection_C::Initialize(void* pArg)
{
	m_strTriggerTag = BattelTriggerNames[BATTLE_TRIGGER_TYPE::SECTION_C];
	
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	CBattleManager::GetInstance()->Add_Trigger(m_strTriggerTag, this);

	if (FAILED(Pop_Enemy()))
		return E_FAIL;

	return S_OK;
}

void CTriggerSection_C::Tick(_double fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bBattle)
		Update_Battle(fTimeDelta);
}

void CTriggerSection_C::LateTick(_double fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}


HRESULT CTriggerSection_C::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CTriggerSection_C::Start_Battle()
{
	m_bStartBattle = TRUE;

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
		pEnemy->Set_State(CGameObject::STATE_ACTIVE);
	}

	return S_OK;
}

HRESULT CTriggerSection_C::Update_Battle(_double fTimeDelta)
{
	if (Is_Finish_Battle())
		Finish_Battle();

	return S_OK;
}

HRESULT CTriggerSection_C::Finish_Battle()
{

	return S_OK;

}

void CTriggerSection_C::OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild)
{
	if (nullptr != pCollider->Get_Owner())
	{
		if (L"Player_Chai_000" == pCollider->Get_Owner()->Get_Name())
		{
			CBattleManager::GetInstance()->OnTrigger_Enter(m_strTriggerTag);
		}
	}
}

void CTriggerSection_C::OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild)
{
}

void CTriggerSection_C::OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild)
{
}

HRESULT CTriggerSection_C::Ready_Components()
{
	return S_OK;
}

CTriggerSection_C* CTriggerSection_C::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTriggerSection_C* pInstance = new CTriggerSection_C(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTriggerSection_C");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTriggerSection_C::Clone(void* pArg)
{
	CTriggerSection_C* pInstance = new CTriggerSection_C(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTriggerSection_C");
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CTriggerSection_C::Free()
{
	__super::Free();

}