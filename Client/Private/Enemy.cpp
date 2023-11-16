#include "..\Default\stdafx.h"
#include "Enemy.h"

#include "EngineInstance.h"
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

	/*if (FAILED(Ready_Chilren()))
		return E_FAIL;*/

	return S_OK;
}

void CEnemy::Tick(_double fTimeDelta)
{
	if (nullptr == m_tFightDesc.pTarget)
		return;

	__super::Tick(fTimeDelta);
}

void CEnemy::LateTick(_double fTimeDelta)
{
	if (nullptr == m_tFightDesc.pTarget)
		return;

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

	switch (eState)
	{
	case OBJ_STATE::STATE_ACTIVE:
	{
		CGameObject* pTarget = ENGINE_INSTANCE->Get_GameObject_InCurLevel_InLayerFirst(LayerNames[LAYER_PLAYER]);

		if (nullptr != pTarget)
			m_tFightDesc.pTarget = dynamic_cast<CCharacter*>(pTarget);
		else
			Set_State(OBJ_STATE::STATE_UNACTIVE);
	}
		break;
	case OBJ_STATE::STATE_UNACTIVE:
	{

	}
		break;
	case OBJ_STATE::STATE_WILLREMOVED:
	{

	}
		break;

	default:
		break;
	}
}

HRESULT CEnemy::Return_To_Pool()
{
	Reset_Desc();
	
	for(auto pChild : m_Children)
	{
		pChild->Set_State(OBJ_STATE::STATE_UNACTIVE);
	}

	return ENGINE_INSTANCE->Return_Pool(ENGINE_INSTANCE->Get_CurLevelIndex(), this);
}

HRESULT CEnemy::Ready_Components()
{
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

	return S_OK;
}

void CEnemy::OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild)
{
	CGameObject*	pGameObject = pCollider->Get_Owner();

	///* 플레이어 무기 맞음 */
	//if (LayerNames[LAYER_WEAPON] == pGameObject->Get_LayerTag())
	//{
	//	if (LayerNames[LAYER_PLAYER] == pGameObject->Get_Parent()->Get_LayerTag())
	//	{
	//		Damaged(dynamic_cast<CCharacter*>(pGameObject->Get_Parent()));
	//	}
	//}
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

	Safe_Release(m_pRigidbodyCom);
}
