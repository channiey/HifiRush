#include "..\Default\stdafx.h"
#include "..\Public\Peppermint.h"

#include "EngineInstance.h"
#include "Animation.h"

#include "UiManager.h"
#include "PlayerController.h"

#include "Weapon.h"
#include "Peppermint_Gun.h"
#include "Peppermint_Bullet.h"

#include "State_Peppermint_Battle.h"
#include "State_Peppermint_Gimmick.h"

CPeppermint::CPeppermint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCharacter(pDevice, pContext)
{
}

CPeppermint::CPeppermint(const CPeppermint& rhs)
	: CCharacter(rhs)
{
}

HRESULT CPeppermint::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPeppermint::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Chilren()))
		return E_FAIL;

	if (FAILED(Ready_StateMachine()))
		return E_FAIL;

	if (FAILED(Ready_Pool()))
		return E_FAIL;

	if (FAILED(CPlayerController::GetInstance()->Add_Player(this, PLAYER_TYPE::PEPPERMINT)))
		return E_FAIL;

	m_pNavMeshAgentCom->Set_Active(FALSE);

	return S_OK;
}

void CPeppermint::Tick(_double fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CPeppermint::LateTick(_double fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CPeppermint::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CPeppermint::Set_State(const OBJ_STATE& eState)
{
	__super::Set_State(eState);

	if (OBJ_STATE::STATE_ACTIVE == eState)
	{
		if (nullptr == ENGINE_INSTANCE->Get_GameObject_InCurLevel_InLayerFirst(LayerNames[LAYER_ID::LAYER_ENEMY]))
		{
			if (FAILED(CPlayerController::GetInstance()->Change_ControlPlayer(PLAYER_TYPE::PEPPERMINT)))
				return;

			m_pStateMachineCom->Set_State(StateNames_PE[STATE_PE::STATE_GIMMICK_PE]);
		}
		else
			m_pStateMachineCom->Set_State(StateNames_PE[STATE_PE::STATE_BATTLE_PE]);
	}
}

HRESULT CPeppermint::Ready_Components()
{
	/* Com_Model */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Model_Peppermint"),
		ComponentNames[COM_MODEL], (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* Com_Rigidbody*/
	CRigidbody::RIGIDBODY_TYPE eType = CRigidbody::RIGIDBODY_TYPE::DYNAMIC;
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Rigidbody"),
		ComponentNames[COM_RIGIDBODY], (CComponent**)&m_pRigidbodyCom, &eType)))
		return E_FAIL;

	/* Com_NavMeshAgent*/
	const _int iIndex = 0;
	{
		if (iIndex < 0)
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

HRESULT CPeppermint::Ready_StateMachine()
{
	CState* pState = nullptr;

	pState = CState_Peppermint_Battle::Create(m_pStateMachineCom, StateNames_PE[STATE_BATTLE_PE], this);
	if (FAILED(m_pStateMachineCom->Add_State(pState)))
		return E_FAIL;

	pState = CState_Peppermint_Gimmick::Create(m_pStateMachineCom, StateNames_PE[STATE_GIMMICK_PE], this);
	if (FAILED(m_pStateMachineCom->Add_State(pState)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPeppermint::Ready_Pool()
{
	if (FAILED(ENGINE_INSTANCE->Reserve_Pool(LEVEL_ID::LV_STAGE_01, LayerNames[LAYER_PROJECTILE], L"Projectile_Peppermint_Bullet", 10)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPeppermint::Ready_Chilren()
{
	CWeapon* pChild = nullptr;

	pChild = dynamic_cast<CWeapon*>(ENGINE_INSTANCE->Add_GameObject(ENGINE_INSTANCE->Get_CurLoadingLevel(), LayerNames[LAYER_WEAPON], L"Weapon_Peppermint_Gun"));
	{
		if (FAILED(Add_Child(pChild)))
			return E_FAIL;

		pChild->Set_Socket(CModel::BONE_SOCKET_LEFT);
		pChild->Set_IndexAsChild(CHILD_TYPE::WP_LEFT);
	}

	pChild = dynamic_cast<CWeapon*>(ENGINE_INSTANCE->Add_GameObject(ENGINE_INSTANCE->Get_CurLoadingLevel(), LayerNames[LAYER_WEAPON], L"Weapon_Peppermint_Gun"));
	{
		if (FAILED(Add_Child(pChild)))
			return E_FAIL;

		pChild->Set_Socket(CModel::BONE_SOCKET_RIGHT);
		pChild->Set_IndexAsChild(CHILD_TYPE::WP_RIGHT);
	}

	return S_OK;
}

void CPeppermint::OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild)
{
	CCharacter* pCharacter = dynamic_cast<CCharacter*>(pCollider->Get_Owner());

	if (CHILD_TYPE::PROJECTILE == iIndexAsChild && nullptr != pCharacter)
	{
		if (LayerNames[LAYER_ID::LAYER_ENEMY] == pCharacter->Get_LayerTag())
		{
			pCharacter->Damaged(this);
		}
	}
}

void CPeppermint::OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild)
{
	m_pStateMachineCom->Get_CurState()->OnCollision_Stay(pCollider, iIndexAsChild);
}

void CPeppermint::OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild)
{
	m_pStateMachineCom->Get_CurState()->OnCollision_Exit(pCollider, iIndexAsChild);
}

CPeppermint* CPeppermint::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPeppermint* pInstance = new CPeppermint(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPeppermint");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CPeppermint* CPeppermint::Clone(void* pArg)
{
	CPeppermint* pInstance = new CPeppermint(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPeppermint");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPeppermint::Free()
{
	__super::Free();
	Safe_Release(m_pRigidbodyCom);
}

