#include "..\Default\stdafx.h"
#include "State_Saber_Idle.h"

#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG

CState_Saber_Idle::CState_Saber_Idle()
{
}

CState_Saber_Idle::CState_Saber_Idle(const CState_Saber_Idle& rhs)
{
}

HRESULT CState_Saber_Idle::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Saber_Idle::Enter()
{
	ANIM_SA			eAnimID = ANIM_SA::IDLE_SA;
	CAnimation*		pAnimation = m_pModel->Get_Animation(eAnimID);
	const _double	fTimePerFrame = CBeatManager::GetInstance()->Get_AnimTimePerFrame(pAnimation);
	
	m_pModel->Set_Animation(eAnimID, fTimePerFrame * (_double)2.f, DF_TW_TIME);
	
	return S_OK;
}

const wstring& CState_Saber_Idle::Tick(const _double& fTimeDelta)
{
	return m_strName;
}

const wstring& CState_Saber_Idle::LateTick()
{
	return Check_Transition();
}

void CState_Saber_Idle::Exit()
{
}

const wstring& CState_Saber_Idle::Check_Transition()
{
	/*if (m_pChai->Get_Model()->Is_Tween())
		return m_strName;

	if (m_pChai->m_tFightDesc.bDamaged)
	{
		return StateNames_CH[STATE_DAMAGED_CH];
	}

	if (Input::Move() && CBeatManager::GetInstance()->Is_HalfBeat())
	{
		if (m_pChai->m_tPhysicsDesc.bGround)
		{
			return StateNames_CH[STATE_RUN_CH];
		}
	}
	else if (Input::Shift() && CBeatManager::GetInstance()->Is_HalfBeat())
	{
		if (!m_pChai->m_tPhysicsDesc.bDash)
		{
			return StateNames_CH[STATE_DASH_CH];
		}
	}
	else if (Input::Attack())
	{
		if (!CImGui_Manager::GetInstance()->Is_ClickedWindow())
			return StateNames_CH[STATE_ATTACK_CH];
	}
	else if (Input::Parry() && CBeatManager::GetInstance()->Is_HalfBeat())
	{
		return StateNames_CH[STATE_PARRY_CH];
	}
	else if (Input::Jump())
	{
		return StateNames_CH[STATE_JUMP_CH];
	}*/

	return m_strName;
}

CState_Saber_Idle* CState_Saber_Idle::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Saber_Idle* pInstance = new CState_Saber_Idle();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Saber_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Saber_Idle::Clone(void* pArg)
{
	return nullptr;
}

void CState_Saber_Idle::Free()
{
}
