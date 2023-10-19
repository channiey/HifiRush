#include "..\Default\stdafx.h"
#include "State_Chai_Idle.h"

CState_Chai_Idle::CState_Chai_Idle()
{
}

CState_Chai_Idle::CState_Chai_Idle(const CState_Chai_Idle& rhs)
{
}

HRESULT CState_Chai_Idle::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Chai_Idle::Enter()
{
	m_pChai->Get_Model()->Set_Animation(ANIM_CH::IDLE, TRUE, 1.5f); 

	return S_OK;
}

const wstring& CState_Chai_Idle::Tick(const _float& fTimeDelta)
{
	// ... 

	return m_strName;
}

const wstring& CState_Chai_Idle::LateTick()
{
	// ... 

	return Check_Transition();
}

void CState_Chai_Idle::Exit()
{
}

const wstring& CState_Chai_Idle::Check_Transition()
{
	if (m_pChai->Get_Model()->Is_Tween())
		return m_strName;

	if (m_pChai->m_tFightDesc.bDamaged)
	{
		return StateNames[STATE_DAMAGED];
	}

	if (Input::Move())
	{
		if (m_pChai->m_tMoveDesc.bGround) 
		{
			return StateNames[STATE_RUN];
		}
	}
	else if (Input::Shift())
	{
		if (!m_pChai->m_tMoveDesc.bDash)
		{
			return StateNames[STATE_DASH];
		}
	}
	else if (Input::Attack()) 
	{
		return StateNames[STATE_ATTACK];
	}
	else if (Input::Parry())
	{
		return StateNames[STATE_PARRY];
	}

	return m_strName;
}

CState_Chai_Idle* CState_Chai_Idle::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Chai_Idle* pInstance = new CState_Chai_Idle();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Chai_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Chai_Idle::Clone(void* pArg)
{
	return nullptr;
}

void CState_Chai_Idle::Free()
{
}
