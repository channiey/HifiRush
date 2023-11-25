#include "..\Default\stdafx.h"
#include "Gunner.h"

#include "ImGui_Manager.h"

#include "EngineInstance.h"
#include "Animation.h"

#include "State_Gunner_Idle.h"
#include "State_Gunner_Move.h"
#include "State_Gunner_NonCombat.h"
#include "State_Gunner_Move.h"
#include "State_Gunner_Attack.h"
#include "State_Gunner_Damaged.h"
#include "State_Gunner_Dead.h"

#include "Util_String.h"
CGunner::CGunner(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEnemy(pDevice, pContext)
{
}

CGunner::CGunner(const CGunner& rhs)
	: CEnemy(rhs)
{
}

HRESULT CGunner::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGunner::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Chilren()))
		return E_FAIL;

	if (FAILED(Ready_StateMachine()))
		return E_FAIL;

	m_tStatDesc.fCurHp = m_tStatDesc.fMaxHp = 200.f;

	return S_OK;
}

void CGunner::Tick(_double fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CGunner::LateTick(_double fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Debug_Animation();
}

HRESULT CGunner::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CGunner::Set_State(const OBJ_STATE& eState)
{
	__super::Set_State(eState);

	if (OBJ_STATE::STATE_ACTIVE == eState)
		m_pStateMachineCom->Set_State(StateNames_GU[STATE_NONECOMBAT]);
}

void CGunner::Damaged(CCharacter* pCharacter, const ATK_TYPE& eAtkType)
{
	if (m_tPhysicsDesc.bJump)
		return;

	__super::Damaged(pCharacter, eAtkType);
}

HRESULT CGunner::Ready_Components()
{
	/* Com_Model */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Model_Gunner"),
		ComponentNames[COM_MODEL], (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* Com_Collider */
	CCollider_Sphere* pCollider = nullptr;
	{
		CCollider::COLLIDERDESC	ColliderDesc(Vec3{ 0.f, 1.3f, 0.f }, 1.3f);

		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
			ComponentNames[COM_COLLIDER_SPHERE], (CComponent**)&pCollider, &ColliderDesc)))
			return E_FAIL;

		m_pColliderComs.push_back(pCollider);
	}

	return S_OK;
}

HRESULT CGunner::Ready_Chilren()
{

	return S_OK;
}

HRESULT CGunner::Ready_StateMachine()
{
	CState* pState = nullptr;

	/* Idle */
	{
		pState = CState_Gunner_Idle::Create(m_pStateMachineCom, StateNames_GU[STATE_IDLE_GU], this);
		if (FAILED(m_pStateMachineCom->Add_State(pState)))
			return E_FAIL;
	}

	/* Move */
	{
		pState = CState_Gunner_Move::Create(m_pStateMachineCom, StateNames_GU[STATE_MOVE_GU], this);
		if (FAILED(m_pStateMachineCom->Add_State(pState)))
			return E_FAIL;
	}

	/* Act */
	{
		pState = CState_Gunner_NonCombat::Create(m_pStateMachineCom, StateNames_GU[STATE_NONECOMBAT], this);
		if (FAILED(m_pStateMachineCom->Add_State(pState)))
			return E_FAIL;

		pState = CState_Gunner_Attack::Create(m_pStateMachineCom, StateNames_GU[STATE_ATTACK_GU], this);
		if (FAILED(m_pStateMachineCom->Add_State(pState)))
			return E_FAIL;

		pState = CState_Gunner_Damaged::Create(m_pStateMachineCom, StateNames_GU[STATE_DAMAGED_GU], this);
		if (FAILED(m_pStateMachineCom->Add_State(pState)))
			return E_FAIL;

		pState = CState_Gunner_Dead::Create(m_pStateMachineCom, StateNames_GU[STATE_DEAD_GU], this);
		if (FAILED(m_pStateMachineCom->Add_State(pState)))
			return E_FAIL;
	}

	return S_OK;
}

void CGunner::OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild)
{
	__super::OnCollision_Enter(pCollider, iIndexAsChild);
}

void CGunner::OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild)
{
	__super::OnCollision_Stay(pCollider, iIndexAsChild);
}

void CGunner::OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild)
{
	__super::OnCollision_Exit(pCollider, iIndexAsChild);
}

CGunner* CGunner::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGunner* pInstance = new CGunner(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CGunner");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGunner* CGunner::Clone(void* pArg)
{
	CGunner* pInstance = new CGunner(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CGunner");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGunner::Free()
{
	__super::Free();
}
