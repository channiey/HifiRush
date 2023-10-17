#include "..\Default\stdafx.h"
#include "State_Chai_Dash.h"

CState_Chai_Dash::CState_Chai_Dash()
{
}

CState_Chai_Dash::CState_Chai_Dash(const CState_Chai_Dash& rhs)
{
}

HRESULT CState_Chai_Dash::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Chai_Dash::Enter()
{
	m_pChai->m_tMoveDesc.bDash = TRUE;

	m_pChai->Get_Model()->Set_Animation(STATE_CH::DASH_FRONT_00, FALSE);

	return S_OK;
}

const wstring& CState_Chai_Dash::Tick(const _float& fTimeDelta)
{

	return m_strName;
}

const wstring& CState_Chai_Dash::LateTick()
{
	
	return Check_Transition();
}

void CState_Chai_Dash::Exit()
{
	m_pChai->m_tMoveDesc.bDash = FALSE;

	Set_LastFramePos();
}

const wstring& CState_Chai_Dash::Check_Transition()
{
	if (m_pChai->Get_Model()->Is_FinishAnimation())
	{
		if (Input::Move())
		{
			return m_pChai->m_StateNames[STATE_CH::RUN_00];
		}
		else
		{
			return m_pChai->m_StateNames[STATE_CH::IDLE_00];
		}
	}

	return m_strName;
}

CState_Chai_Dash* CState_Chai_Dash::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Chai_Dash* pInstance = new CState_Chai_Dash();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Chai_Dash");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Chai_Dash::Clone(void* pArg)
{
	return nullptr;
}

void CState_Chai_Dash::Free()
{
}
