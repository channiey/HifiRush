#include "..\Default\stdafx.h"
#include "State_Chai_Base.h"

CState_Chai_Base::CState_Chai_Base()
{
}

CState_Chai_Base::CState_Chai_Base(const CState_Chai_Base& rhs)
{
}

HRESULT CState_Chai_Base::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName)))
		return E_FAIL;

	m_pChai = dynamic_cast<CChai*>(pOwner);

	if (nullptr == m_pChai)
		return E_FAIL;

	return S_OK;
}

const wstring& CState_Chai_Base::Check_Transition()
{
	if (m_pChai->m_tFightDesc.bDamaged)
	{
		return StateNames_CH[STATE_CH::DAMAGED];
	}

	/* Movement */
	if (Input::Move()) 
	{
		if (m_pChai->m_tMoveDesc.bGround) /* Run */
		{
			return StateNames_CH[STATE_CH::RUN];
		}
	}
	else if (Input::Shift()) 
	{
		if (!m_pChai->m_tMoveDesc.bDash) /* Dash */
		{
			return StateNames_CH[STATE_CH::DASH];
		}
	}
	else if (Input::Jump())
	{
		if (m_pChai->m_tMoveDesc.bGround) /* Jump */
		{
			return StateNames_CH[STATE_CH::JUMP];
		}
		else if (m_pChai->m_tMoveDesc.bJump) /* Double Jump */
		{
			return StateNames_CH[STATE_CH::DOUBLEJUMP];
		}
	}
	else if (!m_pChai->m_tMoveDesc.bGround && !m_pChai->m_tMoveDesc.bJump) /* Fall */
	{
		return StateNames_CH[STATE_CH::FALL];
	}

	/* Action */
	if (Input::Attack())
	{
		if (!m_pChai->m_tFightDesc.bAttack)
		{
			return StateNames_CH[STATE_CH::ATTACK_1];
		}
		else
		{
			if (1 == m_pChai->m_tFightDesc.iCombo)
			{
				return StateNames_CH[STATE_CH::ATTACK_2];
			}
			else if (2 == m_pChai->m_tFightDesc.iCombo)
			{
				return StateNames_CH[STATE_CH::ATTACK_3];
			}
		}
	}
	else if (Input::Interact()) /* Interaction */
	{
		/* 근처에 NPC 있는지 등 조건 확인 후 리턴 */
	}
	else if (Input::Parry()) /* Parry */
	{
		if (!m_pChai->m_tFightDesc.bParry)
			return StateNames_CH[STATE_CH::PARRY];
	}
	else if (Input::Magnet()) /* Magnet */
	{
		/* 근처에 자석 있는지 등 조건 확인 후 리턴 */
	}

	return StateNames_CH[STATE_CH::IDLE];
}

CState* CState_Chai_Base::Clone(void* pArg)
{
	return nullptr;
}

void CState_Chai_Base::Free()
{
	__super::Free();
}
