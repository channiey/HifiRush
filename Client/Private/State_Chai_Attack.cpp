#include "..\Default\stdafx.h"
#include "State_Chai_Attack.h"

CState_Chai_Attack::CState_Chai_Attack()
{
}

CState_Chai_Attack::CState_Chai_Attack(const CState_Chai_Attack& rhs)
{
}

HRESULT CState_Chai_Attack::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Chai_Attack::Enter()
{
	++m_pChai->m_tFightDesc.iStep;

	if(0 == m_pChai->m_tFightDesc.iStep)
		m_pChai->Get_Model()->Set_Animation(ANIM_CH::ATK_LIGHT_00, FALSE);
	else if (1 == m_pChai->m_tFightDesc.iStep)
		m_pChai->Get_Model()->Set_Animation(ANIM_CH::ATK_LIGHT_01, FALSE);
	else if (2 == m_pChai->m_tFightDesc.iStep)
		m_pChai->Get_Model()->Set_Animation(ANIM_CH::ATK_LIGHT_02, FALSE);
	else if (3 == m_pChai->m_tFightDesc.iStep)
	{
		m_pChai->Get_Model()->Set_Animation(ANIM_CH::ATK_LIGHT_03, FALSE);
		m_pChai->m_tFightDesc.iStep = -1;
	}

	return S_OK;
}

const wstring& CState_Chai_Attack::Tick(const _float& fTimeDelta)
{
	return m_strName;
}

const wstring& CState_Chai_Attack::LateTick()
{

	return Check_Transition();
}

void CState_Chai_Attack::Exit()
{
	//Set_LastFramePos();
}

const wstring& CState_Chai_Attack::Check_Transition()
{
	if(m_pChai->Get_Model()->Is_Finish_Animation())
	{
		if (Input::Move())
		{
			return StateNames[STATE_RUN];
		}
		else
		{
			return StateNames[STATE_IDLE];
		}
	}

	return m_strName;
}

CState_Chai_Attack* CState_Chai_Attack::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Chai_Attack* pInstance = new CState_Chai_Attack();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Chai_Attack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Chai_Attack::Clone(void* pArg)
{
	return nullptr;
}

void CState_Chai_Attack::Free()
{
}
