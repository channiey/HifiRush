#include "..\Default\stdafx.h"
#include "State_Chai_DoubleJump.h"

CState_Chai_DoubleJump::CState_Chai_DoubleJump()
{
}

CState_Chai_DoubleJump::CState_Chai_DoubleJump(const CState_Chai_DoubleJump& rhs)
{
}

HRESULT CState_Chai_DoubleJump::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Chai_DoubleJump::Enter()
{
	m_pChai->m_tMoveDesc.bDoubleJump = TRUE;

	m_pChai->Get_Model()->Set_Animation(STATE_CH::DOUBLE_JUMP_00, FALSE);

	return S_OK;
}

const wstring& CState_Chai_DoubleJump::Tick(const _float& fTimeDelta)
{
	return m_strName;
}

const wstring& CState_Chai_DoubleJump::LateTick()
{
	return Check_Transition();
}

void CState_Chai_DoubleJump::Exit()
{
	m_pChai->m_tMoveDesc.bDoubleJump = FALSE;
}

const wstring& CState_Chai_DoubleJump::Check_Transition()
{
	if (m_pChai->Get_Model()->Is_FinishAnimation())
	{
		return m_pChai->m_StateNames[STATE_CH::IDLE_00];
	}

	return m_strName;
}

CState_Chai_DoubleJump* CState_Chai_DoubleJump::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Chai_DoubleJump* pInstance = new CState_Chai_DoubleJump();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Chai_DoubleJump");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Chai_DoubleJump::Clone(void* pArg)
{
	return nullptr;
}

void CState_Chai_DoubleJump::Free()
{
}