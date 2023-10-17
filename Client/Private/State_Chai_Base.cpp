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
	//if (m_pChai->m_tFightDesc.bDamaged)
	//{
	//	return m_pChai->m_StateNames[STATE_CH::DMG_LIGHT];
	//}

	///* Movement */
	//if (Input::Move()) 
	//{
	//	if (m_pChai->m_tMoveDesc.bGround) /* Run */
	//	{
	//		return m_pChai->m_StateNames[STATE_CH::RUN_00];
	//	}
	//}
	//else if (Input::Shift()) 
	//{
	//	if (!m_pChai->m_tMoveDesc.bDash) /* Dash */
	//	{
	//		return m_pChai->m_StateNames[STATE_CH::DASH_FRONT_00];
	//	}
	//}
	//else if (Input::Jump())
	//{
	//	if (m_pChai->m_tMoveDesc.bGround) /* Jump */
	//	{
	//		return m_pChai->m_StateNames[STATE_CH::JUMP_00];
	//	}
	//	else if (m_pChai->m_tMoveDesc.bJump) /* Double Jump */
	//	{
	//		return m_pChai->m_StateNames[STATE_CH::DOUBLE_JUMP_00];
	//	}
	//}
	//else if (!m_pChai->m_tMoveDesc.bGround && !m_pChai->m_tMoveDesc.bJump) /* Fall */
	//{
	//	return m_pChai->m_StateNames[STATE_CH::FALL_00];
	//}

	///* Action */
	//if (Input::Attack())
	//{
	//	if (!m_pChai->m_tFightDesc.bAttack)
	//	{
	//		return m_pChai->m_StateNames[STATE_CH::ATK_LIGHT_00];
	//	}
	//	else
	//	{
	//		if (1 == m_pChai->m_tFightDesc.iCombo)
	//		{
	//			return m_pChai->m_StateNames[STATE_CH::ATK_LIGHT_01];
	//		}
	//		else if (2 == m_pChai->m_tFightDesc.iCombo)
	//		{
	//			return m_pChai->m_StateNames[STATE_CH::ATK_LIGHT_02];
	//		}
	//	}
	//}
	//else if (Input::Interact()) /* Interaction */
	//{
	//	/* 근처에 NPC 있는지 등 조건 확인 후 리턴 */
	//}
	//else if (Input::Parry()) /* Parry */
	//{
	//	if (!m_pChai->m_tFightDesc.bParry)
	//		return m_pChai->m_StateNames[STATE_CH::PARRY_00];
	//}
	//else if (Input::Magnet()) /* Magnet */
	//{
	//	/* 근처에 자석 있는지 등 조건 확인 후 리턴 */
	//}

	return m_pChai->m_StateNames[STATE_CH::IDLE_00];
}

CState* CState_Chai_Base::Clone(void* pArg)
{
	return nullptr;
}

void CState_Chai_Base::Free()
{
	__super::Free();
}
