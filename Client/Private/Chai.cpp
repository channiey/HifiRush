#include "..\Default\stdafx.h"
#include "..\Public\Chai.h"

#include "EngineInstance.h"
#include "Animation.h"

#include "Weapon.h"
#include "PlayerController.h"

/* Effect */
#include "EffectManager.h"
#include "Effect.h"

/* UI */
#include "UiManager.h"
#include "Ui_Dialouge.h"

/* Genral */
#include "State_Chai_Idle.h"

/* Movement */
#include "State_Chai_Run.h"
#include "State_Chai_Dash.h"
#include "State_Chai_Jump.h"

/* Act */
#include "State_Chai_Attack.h"
#include "State_Chai_Damaged.h"
#include "State_Chai_Parry.h"
#include "State_Chai_ParryEvent.h"
#include "State_Chai_SpecialAttack.h"

#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG


CChai::CChai(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCharacter(pDevice, pContext)
{
}

CChai::CChai(const CChai& rhs)
	: CCharacter(rhs)
{
}

HRESULT CChai::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CChai::Initialize(void* pArg)
{
  	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Chilren()))
		return E_FAIL;
	
	if (FAILED(Ready_StateMachine()))
		return E_FAIL;

	if (FAILED(CPlayerController::GetInstance()->Add_Player(this, PLAYER_TYPE::CHAI)))
		return E_FAIL;

	return S_OK;
}

void CChai::Tick(_double fTimeDelta)
{
	Quick_Test();

	Set_OtherPlayer();

	__super::Tick(fTimeDelta);
}

void CChai::LateTick(_double fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	/* Set Shadow Data */
	{
		/* 이 정보를 가지고 모든 다이나믹 캐릭터의 그림자를 생성한다. */
		Vec4 vFinalPos = m_pTransformCom->Get_FinalPosition();

		ENGINE_INSTANCE->Set_ShadowLight_OriginPos(vFinalPos + g_WorldShadowLightOffset);
		ENGINE_INSTANCE->Set_ShadowLight_TargetPos(vFinalPos);
	}
}

HRESULT CChai::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CChai::Ready_Components()
{
	/* Com_Model */
	{
		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Model_Chai"),
		ComponentNames[COM_MODEL], (CComponent**)&m_pModelCom)))
		return E_FAIL;

		/* 루트 포지션 원점으로 초기화 */
		{
			if (FAILED(m_pModelCom->Set_RootPositon_StartFromZero((_uint)ATK_SPECIAL_00)))
				return E_FAIL;
			if (FAILED(m_pModelCom->Set_RootPositon_StartFromZero((_uint)ATK_HORIZONTAL_TOPBLADE_03)))
				return E_FAIL;
		}
	}

	/* Com_Rigidbody*/
	CRigidbody::RIGIDBODY_TYPE eType = CRigidbody::RIGIDBODY_TYPE::DYNAMIC;
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Rigidbody"),
		ComponentNames[COM_RIGIDBODY], (CComponent**)&m_pRigidbodyCom, &eType)))
		return E_FAIL;

	/* Com_NavMeshAgent*/
	const _int iIndex = 0;
	{
		if(iIndex < 0 ) 
			return E_FAIL;

		CNavMeshAgent::NAVMESHAGENT_DESC tDesc(iIndex, m_pTransformCom);
		
		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_NavMeshAgent"),
			ComponentNames[COM_NAVMESHAGENT], (CComponent**)&m_pNavMeshAgentCom, &tDesc)))
			return E_FAIL;
	}

	/* Com_Collider */
	CCollider_Sphere* pCollider = nullptr;
	{
		CCollider::COLLIDERDESC	ColliderDesc(Vec3{ 0.f, 1.f, 0.f }, 1.f);

		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
			ComponentNames[COM_COLLIDER_SPHERE], (CComponent**)&pCollider, &ColliderDesc)))
			return E_FAIL;

		m_pColliderComs.push_back(pCollider);
	}

	return S_OK;
}

HRESULT CChai::Ready_StateMachine()
{
	CState* pState = nullptr;

	/* General */
	{
		pState = CState_Chai_Idle::Create(m_pStateMachineCom, StateNames_CH[STATE_IDLE_CH], this);
		if (FAILED(m_pStateMachineCom->Add_State(pState)))
			return E_FAIL;
	}
	/* Movement */
	{
		pState = CState_Chai_Run::Create(m_pStateMachineCom, StateNames_CH[STATE_RUN_CH], this);
		if (FAILED(m_pStateMachineCom->Add_State(pState)))
			return E_FAIL;

		pState = CState_Chai_Dash::Create(m_pStateMachineCom, StateNames_CH[STATE_DASH_CH], this);
		if (FAILED(m_pStateMachineCom->Add_State(pState)))
			return E_FAIL;

		pState = CState_Chai_Jump::Create(m_pStateMachineCom, StateNames_CH[STATE_JUMP_CH], this);
		if (FAILED(m_pStateMachineCom->Add_State(pState)))
			return E_FAIL;
	}
	/* Action */
	{
		pState = CState_Chai_Attack::Create(m_pStateMachineCom, StateNames_CH[STATE_ATTACK_CH], this);
		if (FAILED(m_pStateMachineCom->Add_State(pState)))
			return E_FAIL;

		pState = CState_Chai_Damaged::Create(m_pStateMachineCom, StateNames_CH[STATE_DAMAGED_CH], this);
		if (FAILED(m_pStateMachineCom->Add_State(pState)))
			return E_FAIL;

		pState = CState_Chai_Parry::Create(m_pStateMachineCom, StateNames_CH[STATE_PARRY_CH], this);
		if (FAILED(m_pStateMachineCom->Add_State(pState)))
			return E_FAIL;

		pState = CState_Chai_ParryEvent::Create(m_pStateMachineCom, StateNames_CH[STATE_PARRYEVENT_CH], this);
		if (FAILED(m_pStateMachineCom->Add_State(pState)))
			return E_FAIL;

		pState = CState_Chai_SpecialAttack::Create(m_pStateMachineCom, StateNames_CH[STATE_SPECIALATTACK_CH], this);
		if (FAILED(m_pStateMachineCom->Add_State(pState)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CChai::Ready_Chilren()
{
	CWeapon* pChild = nullptr;
	
	pChild = dynamic_cast<CWeapon*>(ENGINE_INSTANCE->Add_GameObject(ENGINE_INSTANCE->Get_CurLoadingLevel(), LayerNames[LAYER_WEAPON], L"Weapon_Chai_Guitar_Explore"));
	{
		if (FAILED(Add_Child(pChild)))
			return E_FAIL;

		pChild->Set_Socket(CModel::BONE_SOCKET_RIGHT); 
		pChild->Set_IndexAsChild(CHILD_TYPE::CH_WEAPON_RIGHT);
	}

	return S_OK;
}

void CChai::OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild)
{
	m_pStateMachineCom->Get_CurState()->OnCollision_Enter(pCollider, iIndexAsChild);
}

void CChai::OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild)
{
	m_pStateMachineCom->Get_CurState()->OnCollision_Stay(pCollider, iIndexAsChild);
}

void CChai::OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild)
{
	m_pStateMachineCom->Get_CurState()->OnCollision_Exit(pCollider, iIndexAsChild);
}

void CChai::Damaged(CCharacter* pCharacter, const ATK_TYPE& eAtkType)
{
	if (nullptr == pCharacter || m_tStatDesc.bDead)
		return;

	/* 패링 여부 검사 */
	if (m_tFightDesc.bParry) 
	{
		pCharacter->Damaged(this);
		ENGINE_INSTANCE->Play_Sound(EFC_CHAI_PARRY, PLAYER_CHAI, EfcVolumeChai);
		return;
	}

	/* 상대방 및 공격 타입 검사*/
	m_tFightDesc.pAttacker = pCharacter;
	m_tFightDesc.eAtkType = eAtkType;

	/* 체력 및 사망 여부 검사 */
	m_tFightDesc.bDamaged = TRUE;

	m_tStatDesc.fCurHp -= (pCharacter->Get_StatDesc().fAd * 0.25f);

	if (m_tStatDesc.fCurHp <= 0)
		m_tStatDesc.bDead = TRUE;
}

CChai * CChai::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CChai*	pInstance = new CChai(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CChai");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CChai* CChai::Clone(void* pArg)
{
	CChai*	pInstance = new CChai(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CChai");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChai::Free()
{
	__super::Free();

	Safe_Release(m_pRigidbodyCom);
}

HRESULT CChai::Set_OtherPlayer()
{
	/*if (!CImGui_Manager::GetInstance()->Is_DebugCam())
	{
		if (ENGINE_INSTANCE->Key_Down(0x31))
			CPlayerController::GetInstance()->SetOn_Player(PLAYER_TYPE::PEPPERMINT);
		else if (ENGINE_INSTANCE->Key_Down(0x32))
			CPlayerController::GetInstance()->SetOn_Player(PLAYER_TYPE::MACARON);
		else if (ENGINE_INSTANCE->Key_Down(0x33))
			CPlayerController::GetInstance()->SetOn_Player(PLAYER_TYPE::KORSICA);
	}*/
	return S_OK;
}

void CChai::Quick_Test()
{
	if (ENGINE_INSTANCE->Key_Down('I'))
	{
		const Vec3 vPos = { 96.f, -4.5f, 116.f };
		const _int iIndex = CNavMesh::GetInstance()->Find_Cell(vPos);

		if (-1 != iIndex)
		{
			m_pTransformCom->Set_Position(vPos, TRUE);
			m_pNavMeshAgentCom->Set_CurIndex(iIndex);
		}
	}
	else if (ENGINE_INSTANCE->Key_Down('O'))
	{
		const Vec3 vPos = { 16.f, -3.5f, 58.f };
		const _int iIndex = CNavMesh::GetInstance()->Find_Cell(vPos);

		if (-1 != iIndex)
		{
			m_pTransformCom->Set_Position(vPos, TRUE);
			m_pNavMeshAgentCom->Set_CurIndex(iIndex);
		}
	}
	else if (ENGINE_INSTANCE->Key_Down('P'))
	{
		const Vec3 vPos = { 93.f, -4.7f, 64.f };
		const _int iIndex = CNavMesh::GetInstance()->Find_Cell(vPos);

		if (-1 != iIndex)
		{
			m_pTransformCom->Set_Position(vPos, TRUE);
			m_pNavMeshAgentCom->Set_CurIndex(iIndex);
		}
	}
	else if (ENGINE_INSTANCE->Key_Down('Y'))
	{
		CUiManager::GetInstance()->On_Dialouge(0, L"다들 준비 됐어?");
	}
	else if (ENGINE_INSTANCE->Key_Down('U'))
	{
		CUiManager::GetInstance()->On_Dialouge(4, L"음료수를 마시면 체력을 회복할 수 있습니다.");
	}
	else if (ENGINE_INSTANCE->Key_Down('L'))
	{
		CGameObject* pClone = ENGINE_INSTANCE->Pop_Pool(ENGINE_INSTANCE->Get_CurLevelIndex(), L"Effect_Explosion_Enemy");
		if (nullptr != pClone)
		{
			CEffect* pEffect = dynamic_cast<CEffect*>(pClone);
			if (nullptr != pEffect)
			{

				Vec4 vPos = Get_Transform()->Get_FinalPosition();
				vPos.y += 1.5f;
				pEffect->Get_Transform()->Set_Position(vPos);
				pEffect->Start_Effect();
			}
		}
	}
}

