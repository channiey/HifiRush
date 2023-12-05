#include "..\Default\stdafx.h"
#include "..\Public\Korsica.h"

#include "EngineInstance.h"
#include "Animation.h"

#include "UiManager.h"
#include "PlayerController.h"

#include "Weapon.h"
#include "Korsica_Stick.h"
#include "Korsica_Wind.h"

#include "State_Korsica_Battle.h"
#include "State_Korsica_Gimmick.h"

#include "ImGui_Manager.h"

#include "BattleManager.h"

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

	if (FAILED(Ready_Pool()))
		return E_FAIL;

	if (FAILED(CPlayerController::GetInstance()->Add_Player(this, PLAYER_TYPE::KORSICA)))
		return E_FAIL;

	return S_OK;
}

void CKorsica::Tick(_double fTimeDelta)
{
	if (!CImGui_Manager::GetInstance()->Is_DebugCam() && m_bActive)
	{
		if (nullptr != m_pStateMachineCom)
		{
			if (FAILED(m_pStateMachineCom->Tick(fTimeDelta)))
				return;
		}

		if (nullptr != m_pRigidbodyCom)
			m_pRigidbodyCom->Tick(fTimeDelta);
	}

	for (auto& pCollider : m_pColliderComs)
	{
		if (nullptr != pCollider && pCollider->Is_Active())
			pCollider->Update(m_pTransformCom->Get_FinalMat());
	}
	/*cout << "CKorsica Tick\t" << m_pModelCom->Get_CurAnimationIndex() << "\t" << m_pModelCom->Get_CurAnimationFrame() 
		<< "\t" << m_pModelCom->Get_TweenDesc().next.iAnimIndex << "\t" << m_pModelCom->Get_TweenDesc().next.iCurFrame << "\t" << m_pModelCom->Get_TweenDesc().cur.fRatio << endl;*/
}

void CKorsica::LateTick(_double fTimeDelta)
{
	if (!CImGui_Manager::GetInstance()->Is_DebugCam() && m_bActive)
	{
		if (FAILED(m_pModelCom->Update(fTimeDelta)))
			return;

		if (nullptr != m_pStateMachineCom)
		{
			if (FAILED(m_pStateMachineCom->LateTick(fTimeDelta)))
				return;
		}
	}

	for (auto& pCollider : m_pColliderComs)
	{
		if (nullptr != pCollider && pCollider->Is_Active() && CImGui_Manager::GetInstance()->Is_Render_Collider())
			m_pRendererCom->Add_Debug(pCollider);
	}

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RG_SHADOW, this)))
		return;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;
	//cout << "CKorsica Late\t" << m_pModelCom->Get_CurAnimationIndex() << "\t" << m_pModelCom->Get_CurAnimationFrame()
	//	<< "\t" << m_pModelCom->Get_TweenDesc().next.iAnimIndex << "\t" << m_pModelCom->Get_TweenDesc().next.iCurFrame << "\t" << m_pModelCom->Get_TweenDesc().cur.fRatio << endl;
}

HRESULT CKorsica::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	//cout << "CKorsica Render\n";

	return S_OK;
}

void CKorsica::Set_State(const OBJ_STATE& eState)
{	
	if (OBJ_STATE::STATE_ACTIVE == eState)
	{
		if (!m_bInit)
		{
			m_bInit = TRUE;
			return;
		}

		m_bActive = TRUE;

		m_pModelCom->Stop_Animation(FALSE);

		for (auto& pCollider : m_pColliderComs)
			if (nullptr != pCollider)
				pCollider->Set_Active(TRUE);

		m_pStateMachineCom->Set_State(StateNames_KO[STATE_KO::STATE_BATTLE_KO]);
		
		//cout << "CKorsica Set_State\n";
	}
	else
	{
		m_bActive = FALSE;

		m_pTransformCom->Set_Position(Vec3{ 1000.f, -500.f, 1000.f });

		m_pModelCom->Stop_Animation(TRUE);

		for (auto& pCollider : m_pColliderComs)
			if (nullptr != pCollider)
				pCollider->Set_Active(FALSE);
	}
}

HRESULT CKorsica::Ready_Pool()
{
	if (FAILED(ENGINE_INSTANCE->Reserve_Pool(LEVEL_ID::LV_STAGE_01, LayerNames[LAYER_PROJECTILE], L"Projectile_Korsica_Wind", 4)))
		return E_FAIL;

	return S_OK;
}

HRESULT CKorsica::Ready_Components()
{
	/* Com_Model */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Model_Korsica"),
		ComponentNames[COM_MODEL], (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* Com_NavMeshAgent*/
	/*const _int iIndex = 0;
	{
		if (iIndex < 0)
			return E_FAIL;

		CNavMeshAgent::NAVMESHAGENT_DESC tDesc(iIndex, m_pTransformCom);

		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_NavMeshAgent"),
			ComponentNames[COM_NAVMESHAGENT], (CComponent**)&m_pNavMeshAgentCom, &tDesc)))
			return E_FAIL;
	}*/

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

	pState = CState_Korsica_Battle::Create(m_pStateMachineCom, StateNames_KO[STATE_BATTLE_KO], this);
	if (FAILED(m_pStateMachineCom->Add_State(pState)))
		return E_FAIL;

	/*pState = CState_Korsica_Gimmick::Create(m_pStateMachineCom, StateNames_KO[STATE_GIMMICK_KO], this);
	if (FAILED(m_pStateMachineCom->Add_State(pState)))
		return E_FAIL;*/

	return S_OK;
}

HRESULT CKorsica::Ready_Chilren()
{
	CWeapon* pChild = nullptr;

	pChild = dynamic_cast<CWeapon*>(ENGINE_INSTANCE->Add_GameObject(ENGINE_INSTANCE->Get_CurLoadingLevel(), LayerNames[LAYER_WEAPON], L"Weapon_Korsica_Stick"));
	{
		if (FAILED(Add_Child(pChild)))
			return E_FAIL;

		pChild->Set_Socket(CModel::BONE_SOCKET_RIGHT);
		pChild->Set_IndexAsChild(CHILD_TYPE::WP_RIGHT);
	}

	return S_OK;
}

void CKorsica::OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild)
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
