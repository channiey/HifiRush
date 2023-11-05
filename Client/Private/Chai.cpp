#include "..\Default\stdafx.h"
#include "..\Public\Chai.h"

#include "EngineInstance.h"
#include "Animation.h"

#include "Weapon.h"

/* Genral */
#include "State_Chai_Idle.h"

/* Movement */
#include "State_Chai_Run.h"
#include "State_Chai_Dash.h"
#include "State_Chai_Jump.h"

/* Attack */
#include "State_Chai_Attack.h"
#include "State_Chai_Damaged.h"
#include "State_Chai_Parry.h"

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

	return S_OK;
}

void CChai::Tick(_double fTimeDelta)
{
	if (!CImGui_Manager::GetInstance()->Is_DebugCam())
	{
		if (FAILED(m_pStateMachineCom->Tick(fTimeDelta)))
			return;

		if(nullptr != m_pRigidbodyCom)
			m_pRigidbodyCom->Tick(fTimeDelta);
	}

	__super::Tick(fTimeDelta);
}

void CChai::LateTick(_double fTimeDelta)
{
	if (!CImGui_Manager::GetInstance()->Is_DebugCam())
	{
		if (FAILED(m_pModelCom->Update(fTimeDelta)))
			return;

		if (FAILED(m_pStateMachineCom->LateTick(fTimeDelta)))
			return;
	}

	//CModel::TweenDesc desc = m_pModelCom->Get_TweenDesc();
	//cout << desc.cur.iAnimIndex << "\t" << desc.next.iAnimIndex << "\t" << desc.cur.iCurFrame << "\t" << desc.next.iCurFrame << "\t" << desc.cur.tSoundEventDesc.eSoundID << endl;

	cout << m_pModelCom->Is_RootMotion() << endl;

	__super::LateTick(fTimeDelta);
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
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Model_Chai"),
		ComponentNames[COM_MODEL], (CComponent**)&m_pModelCom)))
		return E_FAIL;
	
	/* Com_StateMachine */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_StateMachine"),
		ComponentNames[COM_STATEMACHINE], (CComponent**)&m_pStateMachineCom)))
		return E_FAIL;

	/* Com_Rigidbody*/
	CRigidbody::RIGIDBODY_TYPE eType = CRigidbody::RIGIDBODY_TYPE::DYNAMIC;
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Rigidbody"),
		ComponentNames[COM_RIGIDBODY], (CComponent**)&m_pRigidbodyCom, &eType)))
		return E_FAIL;

	/* Com_NavMeshAgent*/
	const _int iIndex = 0;// CNavMesh::GetInstance()->Find_Cell(m_pTransformCom->Get_FinalPosition().xyz());
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
		CCollider::COLLIDERDESC	ColliderDesc(Vec3{ 0.f, 0.9f, 0.f }, 0.9f);

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
		pState = CState_Chai_Idle::Create(m_pStateMachineCom, StateNames[STATE_IDLE], this);
		if (FAILED(m_pStateMachineCom->Add_State(pState)))
			return E_FAIL;
	}
	/* Movement */
	{
		pState = CState_Chai_Run::Create(m_pStateMachineCom, StateNames[STATE_RUN], this);
		if (FAILED(m_pStateMachineCom->Add_State(pState)))
			return E_FAIL;

		pState = CState_Chai_Dash::Create(m_pStateMachineCom, StateNames[STATE_DASH], this);
		if (FAILED(m_pStateMachineCom->Add_State(pState)))
			return E_FAIL;

		pState = CState_Chai_Jump::Create(m_pStateMachineCom, StateNames[STATE_JUMP], this);
		if (FAILED(m_pStateMachineCom->Add_State(pState)))
			return E_FAIL;
	}
	/* Action */
	{
		pState = CState_Chai_Attack::Create(m_pStateMachineCom, StateNames[STATE_ATTACK], this);
		if (FAILED(m_pStateMachineCom->Add_State(pState)))
			return E_FAIL;

		pState = CState_Chai_Damaged::Create(m_pStateMachineCom, StateNames[STATE_DAMAGED], this);
		if (FAILED(m_pStateMachineCom->Add_State(pState)))
			return E_FAIL;

		pState = CState_Chai_Parry::Create(m_pStateMachineCom, StateNames[STATE_PARRY], this);
		if (FAILED(m_pStateMachineCom->Add_State(pState)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CChai::Ready_Chilren()
{
	CWeapon* pChild = nullptr;
	
	pChild = dynamic_cast<CWeapon*>(ENGINE_INSTANCE->Add_GameObject(ENGINE_INSTANCE->Get_CurLevelIndex(), LayerNames[LAYER_WEAPON], L"Weapon_Chai_Guitar_Explore"));
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
	CGameObject* pGameObject = pCollider->Get_Owner();
	
	/* 플레이어 무기의 충돌처리(공격 관련)은 스테이트에서 처리한다. */
	if (iIndexAsChild == CH_WEAPON_RIGHT)
	{
		CState_Chai_Base* pState = dynamic_cast<CState_Chai_Base*>(m_pStateMachineCom->Get_CurState());

		if (nullptr != pState)
			pState->OnCollision_Enter(pGameObject);
	
	}
}

void CChai::OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild)
{
}

void CChai::OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild)
{
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

	Safe_Release(m_pStateMachineCom);
	Safe_Release(m_pRigidbodyCom);
}
