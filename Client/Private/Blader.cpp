#include "..\Default\stdafx.h"
#include "Blader.h"

#include "ImGui_Manager.h"

#include "EngineInstance.h"
#include "Animation.h"

#include "State_Blader_Idle.h"

#include "State_Blader_Move.h"

#include "State_Blader_Appear.h"
#include "State_Blader_Attack.h"
#include "State_Blader_Damaged.h"
#include "State_Blader_ParryEvent.h"
#include "State_Blader_Dead.h"

#include "Blader_Arm.h"

CBlader::CBlader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEnemy(pDevice, pContext)
{
}

CBlader::CBlader(const CBlader& rhs)
	: CEnemy(rhs)
{
}

HRESULT CBlader::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBlader::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Chilren()))
		return E_FAIL;

	if (FAILED(Ready_StateMachine()))
		return E_FAIL;

	m_tStatDesc.fCurHp = m_tStatDesc.fMaxHp = 400.f;
	return S_OK;
}

void CBlader::Tick(_double fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CBlader::LateTick(_double fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	//Debug_Animation();
}

HRESULT CBlader::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CBlader::Set_State(const OBJ_STATE& eState)
{
	__super::Set_State(eState);

	if (OBJ_STATE::STATE_ACTIVE == eState)
	{
		if (!m_bActive)
		{
			m_bActive = TRUE;
			return;
		}
		m_pStateMachineCom->Set_State(StateNames_BL[STATE_APPEAR_BL]);
	}
}

HRESULT CBlader::Ready_Components()
{
	/* Com_Model */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Model_Blader"),
		ComponentNames[COM_MODEL], (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* Com_Collider */
	CCollider_Sphere* pCollider = nullptr;
	{
		CCollider::COLLIDERDESC	ColliderDesc(Vec3{ 0.f, 2.3f, 0.f }, 2.3f);

		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
			ComponentNames[COM_COLLIDER_SPHERE], (CComponent**)&pCollider, &ColliderDesc)))
			return E_FAIL;

		m_pColliderComs.push_back(pCollider);
	}

	return S_OK;
}

HRESULT CBlader::Ready_Chilren()
{
	CWeapon* pChild = nullptr;

	pChild = dynamic_cast<CWeapon*>(ENGINE_INSTANCE->Add_GameObject(ENGINE_INSTANCE->Get_CurLoadingLevel(), LayerNames[LAYER_WEAPON], L"Weapon_Blader_Arm"));
	{
		if (FAILED(Add_Child(pChild)))
			return E_FAIL;

		pChild->Set_Socket(CModel::BONE_SOCKET_LEFT);
		pChild->Set_IndexAsChild(CHILD_TYPE::ARM_LEFT_BL);
	}

	pChild = dynamic_cast<CWeapon*>(ENGINE_INSTANCE->Add_GameObject(ENGINE_INSTANCE->Get_CurLoadingLevel(), LayerNames[LAYER_WEAPON], L"Weapon_Blader_Arm"));
	{
		if (FAILED(Add_Child(pChild)))
			return E_FAIL;

		pChild->Set_Socket(CModel::BONE_SOCKET_RIGHT);
		pChild->Set_IndexAsChild(CHILD_TYPE::ARM_RIGHT_BL);
	}

	return S_OK;
}

HRESULT CBlader::Ready_StateMachine()
{
	CState* pState = nullptr;

	/* Idle */
	{
		pState = CState_Blader_Idle::Create(m_pStateMachineCom, StateNames_BL[STATE_IDLE_BL], this);
		if (FAILED(m_pStateMachineCom->Add_State(pState)))
			return E_FAIL;
	}

	/* Move */
	{
		pState = CState_Blader_Move::Create(m_pStateMachineCom, StateNames_BL[STATE_MOVE_BL], this);
		if (FAILED(m_pStateMachineCom->Add_State(pState)))
			return E_FAIL;
	}

	/* Act */
	{
		pState = CState_Blader_Appear::Create(m_pStateMachineCom, StateNames_BL[STATE_APPEAR_BL], this);
		if (FAILED(m_pStateMachineCom->Add_State(pState)))
			return E_FAIL;

		pState = CState_Blader_Attack::Create(m_pStateMachineCom, StateNames_BL[STATE_ATTACK_BL], this);
		if (FAILED(m_pStateMachineCom->Add_State(pState)))
			return E_FAIL;

		pState = CState_Blader_Damaged::Create(m_pStateMachineCom, StateNames_BL[STATE_DAMAGED_BL], this);
		if (FAILED(m_pStateMachineCom->Add_State(pState)))
			return E_FAIL;

		pState = CState_Blader_ParryEvent::Create(m_pStateMachineCom, StateNames_BL[STATE_PARRYEVENT_BL], this);
		if (FAILED(m_pStateMachineCom->Add_State(pState)))
			return E_FAIL;

		pState = CState_Blader_Dead::Create(m_pStateMachineCom, StateNames_BL[STATE_DEAD_BL], this);
		if (FAILED(m_pStateMachineCom->Add_State(pState)))
			return E_FAIL;
	}

	return S_OK;
}

void CBlader::OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild)
{
	m_pStateMachineCom->Get_CurState()->OnCollision_Enter(pCollider, iIndexAsChild);
}

void CBlader::OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild)
{
	m_pStateMachineCom->Get_CurState()->OnCollision_Stay(pCollider, iIndexAsChild);
}

void CBlader::OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild)
{
	m_pStateMachineCom->Get_CurState()->OnCollision_Exit(pCollider, iIndexAsChild);
}

CBlader* CBlader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBlader* pInstance = new CBlader(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBlader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CBlader* CBlader::Clone(void* pArg)
{
	CBlader* pInstance = new CBlader(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBlader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBlader::Free()
{
	__super::Free();
}
