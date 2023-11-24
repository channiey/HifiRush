#include "..\Default\stdafx.h"
#include "Saber.h"

#include "EngineInstance.h"
#include "Animation.h"

#include "Weapon.h"

#include "State_Saber_Base.h"
#include "State_Saber_Idle.h"
#include "State_Saber_Move.h"
#include "State_Saber_Attack.h"
#include "State_Saber_Damaged.h"
#include "State_Saber_Dead.h"
#include "State_Saber_Appear.h"

CSaber::CSaber(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEnemy(pDevice, pContext)
{
}

CSaber::CSaber(const CSaber& rhs)
	: CEnemy(rhs)
{
}

HRESULT CSaber::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSaber::Initialize(void* pArg)
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

void CSaber::Tick(_double fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CSaber::LateTick(_double fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CSaber::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CSaber::Set_State(const OBJ_STATE& eState)
{
	__super::Set_State(eState);

	if (OBJ_STATE::STATE_ACTIVE == eState)
	{
		m_pStateMachineCom->Set_State(StateNames_SA[STATE_APPEAR_SA]);

		m_tStatDesc.fCurHp = m_tStatDesc.fMaxHp = 150.f;
	}
}

HRESULT CSaber::Ready_Components()
{
	/* Com_Model */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Model_Saber"),
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

HRESULT CSaber::Ready_Chilren()
{
	CWeapon* pWeapon = nullptr;
	{
		pWeapon = dynamic_cast<CWeapon*>(ENGINE_INSTANCE->Add_GameObject(ENGINE_INSTANCE->Get_CurLoadingLevel(), LayerNames[LAYER_WEAPON], L"Weapon_Saber_Sword"));
	
		if (FAILED(Add_Child(pWeapon)))
			return E_FAIL;

		pWeapon->Set_Socket(CModel::BONE_SOCKET_RIGHT);
		pWeapon->Set_IndexAsChild(CHILD_TYPE::SA_WEAPON_RIGHT);
	}

	return S_OK;
}

HRESULT CSaber::Ready_StateMachine()
{
	CState* pState = nullptr;

	/* General */
	{
		pState = CState_Saber_Idle::Create(m_pStateMachineCom, StateNames_SA[STATE_IDLE_SA], this);
		if (FAILED(m_pStateMachineCom->Add_State(pState)))
			return E_FAIL;
	}
	/* Movement */
	{
		pState = CState_Saber_Move::Create(m_pStateMachineCom, StateNames_SA[STATE_MOVE_SA], this);
		if (FAILED(m_pStateMachineCom->Add_State(pState)))
			return E_FAIL;
	}
	/* Action */
	{
		pState = CState_Saber_Attack::Create(m_pStateMachineCom, StateNames_SA[STATE_ATTACK_SA], this);
		if (FAILED(m_pStateMachineCom->Add_State(pState)))
			return E_FAIL;

		pState = CState_Saber_Damaged::Create(m_pStateMachineCom, StateNames_SA[STATE_DAMAGED_SA], this);
		if (FAILED(m_pStateMachineCom->Add_State(pState)))
			return E_FAIL;

		pState = CState_Saber_Dead::Create(m_pStateMachineCom, StateNames_SA[STATE_DEAD_SA], this);
		if (FAILED(m_pStateMachineCom->Add_State(pState)))
			return E_FAIL;

		pState = CState_Saber_Appear::Create(m_pStateMachineCom, StateNames_SA[STATE_APPEAR_SA], this);
		if (FAILED(m_pStateMachineCom->Add_State(pState)))
			return E_FAIL;
	}

	return S_OK;
}

void CSaber::OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild)
{
	__super::OnCollision_Enter(pCollider, iIndexAsChild);

	CGameObject* pGameObject = pCollider->Get_Owner();
}

void CSaber::OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild)
{
	__super::OnCollision_Stay(pCollider, iIndexAsChild);
}

void CSaber::OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild)
{
	__super::OnCollision_Exit(pCollider, iIndexAsChild);
}

CSaber* CSaber::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSaber* pInstance = new CSaber(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSaber");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CSaber* CSaber::Clone(void* pArg)
{
	CSaber* pInstance = new CSaber(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSaber");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSaber::Free()
{
	__super::Free();
}
