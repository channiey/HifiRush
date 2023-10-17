#include "..\Default\stdafx.h"
#include "State_Chai_Jump.h"

CState_Chai_Jump::CState_Chai_Jump()
{
}

CState_Chai_Jump::CState_Chai_Jump(const CState_Chai_Jump& rhs)
{
}

HRESULT CState_Chai_Jump::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Chai_Jump::Enter()
{
	m_pChai->m_tMoveDesc.bGround = FALSE;
	m_pChai->m_tMoveDesc.bJump = TRUE;

	m_pChai->Get_Model()->Set_Animation(STATE_CH::JUMP_00, FALSE);

	return S_OK;
}

const wstring& CState_Chai_Jump::Tick(const _float& fTimeDelta)
{
	return Check_Transition();
}

const wstring& CState_Chai_Jump::LateTick()
{
	return Check_Transition();
}

void CState_Chai_Jump::Exit()
{
}

const wstring& CState_Chai_Jump::Check_Transition()
{
	__super::Check_Transition();

	return m_pChai->m_StateNames[STATE_CH::IDLE_00];
}

CState_Chai_Jump* CState_Chai_Jump::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Chai_Jump* pInstance = new CState_Chai_Jump();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Chai_Jump");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Chai_Jump::Clone(void* pArg)
{
	return nullptr;
}

void CState_Chai_Jump::Free()
{
}
