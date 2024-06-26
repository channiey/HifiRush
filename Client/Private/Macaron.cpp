#include "..\Default\stdafx.h"
#include "..\Public\Macaron.h"

#include "EngineInstance.h"
#include "Animation.h"

#include "UiManager.h"
#include "PlayerController.h"

#include "State_Macaron_Battle.h"
#include "State_Macaron_Gimmick.h"

#include "ImGui_Manager.h"

#include "BattleManager.h"

CMacaron::CMacaron(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCharacter(pDevice, pContext)
{
}

CMacaron::CMacaron(const CMacaron& rhs)
	: CCharacter(rhs)
{
}

HRESULT CMacaron::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMacaron::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Chilren()))
		return E_FAIL;

	if (FAILED(Ready_StateMachine()))
		return E_FAIL;

	if (FAILED(CPlayerController::GetInstance()->Add_Player(this, PLAYER_TYPE::MACARON)))
		return E_FAIL;

	return S_OK;
}

void CMacaron::Tick(_double fTimeDelta)
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
	/*cout << "CMacaron Tick\t" << m_pModelCom->Get_CurAnimationIndex() << "\t" << m_pModelCom->Get_CurAnimationFrame()
		<< "\t" << m_pModelCom->Get_TweenDesc().next.iAnimIndex << "\t" << m_pModelCom->Get_TweenDesc().next.iCurFrame << "\t" << m_pModelCom->Get_TweenDesc().cur.fRatio << endl;*/
}

void CMacaron::LateTick(_double fTimeDelta)
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
	/*cout << "CMacaron Late\t" << m_pModelCom->Get_CurAnimationIndex() << "\t" << m_pModelCom->Get_CurAnimationFrame()
		<< "\t" << m_pModelCom->Get_TweenDesc().next.iAnimIndex << "\t" << m_pModelCom->Get_TweenDesc().next.iCurFrame << "\t" << m_pModelCom->Get_TweenDesc().cur.fRatio << endl;*/
}

HRESULT CMacaron::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	//cout << "CMacaron Render\n";

	return S_OK;
}

void CMacaron::Set_State(const OBJ_STATE& eState)
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

		if (FALSE == CBattleManager::GetInstance()->Is_In_Battle())
		{
			if (FAILED(CPlayerController::GetInstance()->Change_ControlPlayer(PLAYER_TYPE::MACARON)))
				return;

			m_pStateMachineCom->Set_State(StateNames_MA[STATE_MA::STATE_GIMMICK_MA]);
		}
		else
		{
			m_pStateMachineCom->Set_State(StateNames_MA[STATE_MA::STATE_BATTLE_MA]);
		}
		
		//cout << "CMacaron Set_State\n";
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

HRESULT CMacaron::Ready_Components()
{
	/* Com_Model */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Model_Macaron"),
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

HRESULT CMacaron::Ready_StateMachine()
{
	CState* pState = nullptr;

	pState = CState_Macaron_Battle::Create(m_pStateMachineCom, StateNames_MA[STATE_BATTLE_MA], this);
	if (FAILED(m_pStateMachineCom->Add_State(pState)))
		return E_FAIL;

	pState = CState_Macaron_Gimmick::Create(m_pStateMachineCom, StateNames_MA[STATE_GIMMICK_MA], this);
	if (FAILED(m_pStateMachineCom->Add_State(pState)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMacaron::Ready_Chilren()
{
	return S_OK;
}

void CMacaron::OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild)
{
	m_pStateMachineCom->Get_CurState()->OnCollision_Enter(pCollider, iIndexAsChild);
}

void CMacaron::OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild)
{
	m_pStateMachineCom->Get_CurState()->OnCollision_Stay(pCollider, iIndexAsChild);
}

void CMacaron::OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild)
{
	m_pStateMachineCom->Get_CurState()->OnCollision_Exit(pCollider, iIndexAsChild);
}

CMacaron* CMacaron::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMacaron* pInstance = new CMacaron(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMacaron");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CMacaron* CMacaron::Clone(void* pArg)
{
	CMacaron* pInstance = new CMacaron(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMacaron");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMacaron::Free()
{
	__super::Free();
	Safe_Release(m_pRigidbodyCom);
}
