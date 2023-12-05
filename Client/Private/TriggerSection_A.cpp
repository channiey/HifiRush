#include "..\Default\stdafx.h"
#include "..\Public\TriggerSection_A.h"

#include "EngineInstance.h"
#include "Util_String.h"

#include "BattleManager.h"

#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG

#include "Enemy.h"

CTriggerSection_A::CTriggerSection_A(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTriggerBattle(pDevice, pContext)
{
}

CTriggerSection_A::CTriggerSection_A(const CTriggerSection_A& rhs)
	: CTriggerBattle(rhs)
{
}

HRESULT CTriggerSection_A::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTriggerSection_A::Initialize(void* pArg)
{
	m_strTriggerTag = BattelTriggerNames[BATTLE_TRIGGER_TYPE::SECTION_A];

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

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
	__super::Render();

	return S_OK;
}

HRESULT CTriggerSection_A::Start_Battle()
{
	m_bStartBattle = TRUE;

	/* 01. 카메라 변경 */

	/* 02. 세이버 애니메이션 재생 */

	/* 03. 카메라 복귀 */

	/* 04. 플레이어 애니메이션 재생 */

	/* 05. 아유 레디 ? */

	/* 06. 파이트 */

	/* Set Camera */
	/*CCamera* pCameraCom = ENGINE_INSTANCE->Get_CurCamera()->Get_Camera();
	if (nullptr != pCameraCom && CAMERA_ID::CAM_FOLLOW == (CAMERA_ID)pCameraCom->Get_Key())
	{
		pCameraCom->Lerp_Fov(CamFov_Follow_Battle, 1.5f, LERP_MODE::SMOOTHER_STEP);
		pCameraCom->Lerp_Dist(CamDist_Follow_Battle, 1.5f, LERP_MODE::SMOOTHER_STEP);
	}*/

	/* Set Volume */
	//ENGINE_INSTANCE->Lerp_BGMSound(BgmVolumeInBattle, 2.f, LERP_MODE::SMOOTHER_STEP);

	/* Pop from Pool */

	const _uint iCurLevel = ENGINE_INSTANCE->Get_CurLevelIndex();

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
	}
	
	
	return S_OK;
}

HRESULT CTriggerSection_A::Update_Battle(_double fTimeDelta)
{
	if (Is_Finish_Battle())
		Finish_Battle();

	return S_OK;
}

HRESULT CTriggerSection_A::Finish_Battle()
{
	

	return S_OK;
}

void CTriggerSection_A::OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild)
{
	if (nullptr != pCollider->Get_Owner())
	{
		if (L"Player_Chai_000" == pCollider->Get_Owner()->Get_Name())
		{
			CBattleManager::GetInstance()->OnTrigger_Enter(m_strTriggerTag);
		}
	}
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