#include "..\Default\stdafx.h"
#include "..\Public\TriggerSection_A.h"

#include "EngineInstance.h"
#include "Util_String.h"

#include "BattleManager.h"

#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG

CTriggerSection_A::CTriggerSection_A(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTriggerBattle(pDevice, pContext)
{
}

CTriggerSection_A::CTriggerSection_A(const CTriggerSection_A& rhs)
	: CTriggerBattle(rhs)
	, m_fOriginFov(rhs.m_fOriginFov)
	, m_fBattleFov(rhs.m_fBattleFov)
{
}

HRESULT CTriggerSection_A::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTriggerSection_A::Initialize(void* pArg)
{
	m_strTriggerTag = BattelTriggerNames[BATTLE_TRIGGER_TYPE::SECTION_A];

	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	CBattleManager::GetInstance()->Add_Trigger(m_strTriggerTag, this);

	return S_OK;
}

void CTriggerSection_A::Tick(_double fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bBattle)
		Update_Battle(fTimeDelta);
}

void CTriggerSection_A::LateTick(_double fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}


HRESULT CTriggerSection_A::Render()
{
	return S_OK;
}

HRESULT CTriggerSection_A::Start_Battle()
{
	m_bStartBattle = TRUE;

	/* Set Camera */
	{
		CCamera* pCameraCom = ENGINE_INSTANCE->Get_CurCamera()->Get_Camera();

		if (nullptr == pCameraCom)
			return E_FAIL;

		m_fOriginFov = pCameraCom->Get_ProjDesc().fFovy;
	
		pCameraCom->Lerp_Fov(m_fOriginFov, m_fBattleFov, 1.f, LERP_MODE::SMOOTHER_STEP);
	}

	/* Pop from Pool */
	for (auto Pair : m_Flows)
	{
		for (auto desc : Pair.second)
		{
			CGameObject* pClone = ENGINE_INSTANCE->Pop_Pool(ENGINE_INSTANCE->Get_CurLevelIndex(), desc.strPrototypeTag);

			if (nullptr != pClone)
			{
				pClone->Get_Transform()->Set_WorldMat(desc.matWorld);
				pClone->Get_NavMeshAgent()->Set_CurIndex(desc.iCellIndex);

				m_Clones.push_back(pClone);
			}
		}
	}
	
	
	return S_OK;
}

HRESULT CTriggerSection_A::Update_Battle(_double fTimeDelta)
{
	return S_OK;
}

HRESULT CTriggerSection_A::Finish_Battle()
{
	/* Set Camera */
	{
		CCamera* pCameraCom = ENGINE_INSTANCE->Get_CurCamera()->Get_Camera();

		if (nullptr == pCameraCom)
			return E_FAIL;

		pCameraCom->Lerp_Fov(pCameraCom->Get_ProjDesc().fFovy, m_fOriginFov, 1.f, LERP_MODE::SMOOTHER_STEP);
	}

	return S_OK;
}

void CTriggerSection_A::OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild)
{
	CBattleManager::GetInstance()->OnTrigger_Enter(m_strTriggerTag);
}

void CTriggerSection_A::OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild)
{
}

void CTriggerSection_A::OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild)
{
}

HRESULT CTriggerSection_A::Ready_Components()
{
	return S_OK;
}

CTriggerSection_A* CTriggerSection_A::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTriggerSection_A* pInstance = new CTriggerSection_A(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTriggerSection_A");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTriggerSection_A::Clone(void* pArg)
{
	CTriggerSection_A* pInstance = new CTriggerSection_A(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTriggerSection_A");
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CTriggerSection_A::Free()
{
	__super::Free();

}