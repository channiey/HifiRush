#include "..\Default\stdafx.h"
#include "Enemy.h"

#include "GameInstance.h"
#include "Animation.h"

#include "Weapon.h"
#include "TriggerDummy.h"

#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG

CEnemy::CEnemy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCharacter(pDevice, pContext)
{
}

CEnemy::CEnemy(const CEnemy& rhs)
	: CCharacter(rhs)
{
}

HRESULT CEnemy::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnemy::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Chilren()))
		return E_FAIL;

	if (FAILED(Ready_BehavoiurTree()))
		return E_FAIL;

	return S_OK;
}

void CEnemy::Tick(_float fTimeDelta)
{
	if (!CImGui_Manager::GetInstance()->Is_DebugCam())
	{
		if (nullptr != m_pRigidbodyCom)
			m_pRigidbodyCom->Tick(fTimeDelta);
	}
	__super::Tick(fTimeDelta);
}

void CEnemy::LateTick(_float fTimeDelta)
{
	if (!CImGui_Manager::GetInstance()->Is_DebugCam())
	{
		if (FAILED(m_pModelCom->Update(fTimeDelta)))
			return;

		if (FAILED(m_pBehaviourTreeCom->LateTick(fTimeDelta)))
			return;
	}
	__super::LateTick(fTimeDelta);
}

HRESULT CEnemy::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CEnemy::Set_State(const OBJ_STATE& eState)
{
	__super::Set_State(eState);

	if (OBJ_STATE::STATE_ACTIVE == eState)
	{
		CGameObject* pTarget = GAME_INSTNACE->Get_GameObject_InCurLevel_InLayerFirst(LayerNames[LAYER_PLAYER]);

		if (nullptr != pTarget)
			m_tFightDesc.pTarget = dynamic_cast<CCharacter*>(pTarget);
		else
			Set_State(OBJ_STATE::STATE_UNACTIVE);
	}
}

HRESULT CEnemy::Ready_Components()
{
	/* Com_BehaviourTree*/
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_BehaviourTree"),
		ComponentNames[COM_BEHAVIOURTREE], (CComponent**)&m_pBehaviourTreeCom)))
		return E_FAIL;

	/* Com_Rigidbody*/
	CRigidbody::RIGIDBODY_TYPE eType = CRigidbody::RIGIDBODY_TYPE::DYNAMIC;
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Rigidbody"),
		ComponentNames[COM_RIGIDBODY], (CComponent**)&m_pRigidbodyCom, &eType)))
		return E_FAIL;

	///* Com_NavMeshAgent*/
	//const _int iIndex = 0;// CNavMesh::GetInstance()->Find_Cell(m_pTransformCom->Get_FinalPosition().xyz());
	//{
	//	if (iIndex < 0)
	//		return E_FAIL;

	//	CNavMeshAgent::NAVMESHAGENT_DESC tDesc(iIndex, m_pTransformCom);

	//	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_NavMeshAgent"),
	//		ComponentNames[COM_NAVMESHAGENT], (CComponent**)&m_pNavMeshAgentCom, &tDesc)))
	//		return E_FAIL;
	//}

	return S_OK;
}

void CEnemy::OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild)
{
	CGameObject*	pGameObject = pCollider->Get_Owner();

	if (LayerNames[LAYER_WEAPON] == pGameObject->Get_LayerTag())
	{
		if (LayerNames[LAYER_PLAYER] == pGameObject->Get_Parent()->Get_LayerTag())
		{
			m_tFightDesc.bDamaged = TRUE;
			m_tFightDesc.pAttacker = dynamic_cast<CCharacter*>(pGameObject->Get_Parent());
		}
	}
}

void CEnemy::OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild)
{
}

void CEnemy::OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild)
{
}

void CEnemy::Free()
{
	__super::Free();

	Safe_Release(m_pBehaviourTreeCom);
	Safe_Release(m_pRigidbodyCom);
}
