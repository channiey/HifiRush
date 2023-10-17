#include "..\Default\stdafx.h"
#include "State_Chai_Attack_1.h"

CState_Chai_Attack_1::CState_Chai_Attack_1()
{
}

CState_Chai_Attack_1::CState_Chai_Attack_1(const CState_Chai_Attack_1& rhs)
{
}

HRESULT CState_Chai_Attack_1::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Chai_Attack_1::Enter()
{
	m_pChai->m_tFightDesc.bAttack = TRUE;
	m_pChai->m_tFightDesc.iCombo = 1;

	m_pChai->Get_Model()->Set_Animation(STATE_CH::ATK_LIGHT_00, FALSE);

	return S_OK;
}

const wstring& CState_Chai_Attack_1::Tick(const _float& fTimeDelta)
{
	return Check_Transition();
}

const wstring& CState_Chai_Attack_1::LateTick()
{
	return Check_Transition();
}

void CState_Chai_Attack_1::Exit()
{
}

const wstring& CState_Chai_Attack_1::Check_Transition()
{
	__super::Check_Transition();

	//if(m_pChai->Get_Model()->Is_FinishAnimation())
	//	return m_pChai->m_StateNames[STATE_CH::IDLE];

	return m_pChai->m_StateNames[STATE_CH::IDLE_00];
}

CState_Chai_Attack_1* CState_Chai_Attack_1::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Chai_Attack_1* pInstance = new CState_Chai_Attack_1();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Chai_Attack_1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Chai_Attack_1::Clone(void* pArg)
{
	return nullptr;
}

void CState_Chai_Attack_1::Free()
{
}
