#include "..\Default\stdafx.h"
#include "State_Chai_Idle.h"

CState_Chai_Idle::CState_Chai_Idle()
{
}

CState_Chai_Idle::CState_Chai_Idle(const CState_Chai_Idle& rhs)
{
}

HRESULT CState_Chai_Idle::Initialize()
{
	return S_OK;
}

HRESULT CState_Chai_Idle::Enter()
{
	m_pChai->Get_Model()->Set_Animation(4, TRUE); /* TODO:: 애니메이션 다 적용하고 CChai에 있는 enum으로 사용 */

	return S_OK;
}

const wstring& CState_Chai_Idle::Tick(const _float& fimeDelta)
{
	// ... 

	return Check_Transition();
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
	/* First Condition */
	if (m_pChai->m_tFightDesc.bDamaged)
	{
		m_pChai->m_tFightDesc.bDamaged = FALSE;

		return StateNames_CH[STATE_CH::DAMAGED];
	}

	/* Second Conditon */
	if (Input::Move())
	{
		return StateNames_CH[STATE_CH::RUN]; 
	}
	else if (Input::Attack())
	{
		m_pChai->m_tFightDesc.bInAttack = TRUE;

		return StateNames_CH[STATE_CH::ATTACK_1];
	}
	else if (Input::Shift())
	{
		return StateNames_CH[STATE_CH::DASH];
	}
	else if (Input::Space && m_pChai->m_tMoveDesc.bGround)
	{
		m_pChai->m_tMoveDesc.bGround = FALSE;
		m_pChai->m_tMoveDesc.bJump = TRUE;
	}

	return StateNames_CH[STATE_CH::IDLE];
}

CState_Chai_Idle* CState_Chai_Idle::Create()
{
	CState_Chai_Idle* pInstance = new CState_Chai_Idle();

	if (FAILED(pInstance->Initialize()))
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
