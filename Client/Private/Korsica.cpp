#include "..\Default\stdafx.h"
#include "..\Public\Korsica.h"

#include "EngineInstance.h"
#include "Animation.h"

#include "Weapon.h"

/* UI */
#include "UiManager.h"

CKorsica::CKorsica(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCharacter(pDevice, pContext)
{
}

CKorsica::CKorsica(const CKorsica& rhs)
	: CCharacter(rhs)
{
}

HRESULT CKorsica::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CKorsica::Initialize(void* pArg)
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

void CKorsica::Tick(_double fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CKorsica::LateTick(_double fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CKorsica::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CKorsica::Ready_Components()
{
	/* Com_Model */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Model_Korsica"),
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

HRESULT CKorsica::Ready_StateMachine()
{
	CState* pState = nullptr;

	/* General */
	{
		/*pState = CState_Chai_Idle::Create(m_pStateMachineCom, StateNames_CH[STATE_IDLE_CH], this);
		if (FAILED(m_pStateMachineCom->Add_State(pState)))
			return E_FAIL;*/
	}

	return S_OK;
}

HRESULT CKorsica::Ready_Chilren()
{
	/*CWeapon* pChild = nullptr;

	pChild = dynamic_cast<CWeapon*>(ENGINE_INSTANCE->Add_GameObject(ENGINE_INSTANCE->Get_CurLoadingLevel(), LayerNames[LAYER_WEAPON], L"Weapon_Chai_Guitar_Explore"));
	{
		if (FAILED(Add_Child(pChild)))
			return E_FAIL;

		pChild->Set_Socket(CModel::BONE_SOCKET_RIGHT);
		pChild->Set_IndexAsChild(CHILD_TYPE::CH_WEAPON_RIGHT);
	}*/

	return S_OK;
}

void CKorsica::OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild)
{
	m_pStateMachineCom->Get_CurState()->OnCollision_Enter(pCollider, iIndexAsChild);
}

void CKorsica::OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild)
{
	m_pStateMachineCom->Get_CurState()->OnCollision_Stay(pCollider, iIndexAsChild);
}

void CKorsica::OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild)
{
	m_pStateMachineCom->Get_CurState()->OnCollision_Exit(pCollider, iIndexAsChild);
}

CKorsica* CKorsica::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CKorsica* pInstance = new CKorsica(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CKorsica");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CKorsica* CKorsica::Clone(void* pArg)
{
	CKorsica* pInstance = new CKorsica(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CKorsica");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CKorsica::Free()
{
	__super::Free();
	Safe_Release(m_pRigidbodyCom);
}
