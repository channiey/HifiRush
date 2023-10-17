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
	m_pChai->Get_Model()->Set_Animation(STATE_CH::IDLE_00, TRUE); /* TODO:: 애니메이션 다 적용하고 CChai에 있는 enum으로 사용 */

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
	if (m_pChai->m_tFightDesc.bDamaged)
	{
		return m_pChai->m_StateNames[STATE_CH::DMG_LIGHT];
	}

	/* Movement */
	if (Input::Move())
	{
		if (m_pChai->m_tMoveDesc.bGround) /* Run */
		{
			return m_pChai->m_StateNames[STATE_CH::RUN_00];
		}
	}
	else if (Input::Shift())
	{
		if (!m_pChai->m_tMoveDesc.bDash) /* Dash */
		{
			return m_pChai->m_StateNames[STATE_CH::DASH_FRONT_00];
		}
	}
	//else if (Input::Jump())
	//{
	//	if (m_pChai->m_tMoveDesc.bGround && !m_pChai->m_tMoveDesc.bJump) /* Jump */
	//	{
	//		return m_pChai->m_StateNames[STATE_CH::JUMP_00];
	//	}
	//	if (m_pChai->m_tMoveDesc.bJump && !m_pChai->m_tMoveDesc.bDoubleJump) /* Double Jump */
	//	{
	//		return m_pChai->m_StateNames[STATE_CH::DOUBLE_JUMP_00];
	//	}
	//}
	else if (Input::Attack())
	{
		return m_pChai->m_StateNames[STATE_CH::ATK_LIGHT_00];
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
