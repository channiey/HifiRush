#include "..\Default\stdafx.h"
#include "State_Chai_Damaged.h"

CState_Chai_Damaged::CState_Chai_Damaged()
{
}

CState_Chai_Damaged::CState_Chai_Damaged(const CState_Chai_Damaged& rhs)
{
}

HRESULT CState_Chai_Damaged::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Chai_Damaged::Enter()
{

	return S_OK;
}

const wstring& CState_Chai_Damaged::Tick(const _float& fTimeDelta)
{
	return Check_Transition();
}

const wstring& CState_Chai_Damaged::LateTick()
{
	return Check_Transition();
}

void CState_Chai_Damaged::Exit()
{
	m_pChai->m_tFightDesc.bDamaged = FALSE;
}

const wstring& CState_Chai_Damaged::Check_Transition()
{
	__super::Check_Transition();

	return StateNames_CH[STATE_CH::IDLE];
}

CState_Chai_Damaged* CState_Chai_Damaged::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Chai_Damaged* pInstance = new CState_Chai_Damaged();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Chai_Damaged");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Chai_Damaged::Clone(void* pArg)
{
	return nullptr;
}

void CState_Chai_Damaged::Free()
{
}
