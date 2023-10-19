#include "..\Default\stdafx.h"
#include "State_Chai_Parry.h"

CState_Chai_Parry::CState_Chai_Parry()
{
}

CState_Chai_Parry::CState_Chai_Parry(const CState_Chai_Parry& rhs)
{
}

HRESULT CState_Chai_Parry::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Chai_Parry::Enter()
{
	m_pChai->m_tFightDesc.bParry = TRUE;

	switch (rand() % 3)
	{
	case 0:
		m_pChai->Get_Model()->Set_Animation(ANIM_CH::PARRY_00, FALSE, 2.2f);
		break;
	case 1 :
		m_pChai->Get_Model()->Set_Animation(ANIM_CH::PARRY_01, FALSE, 2.2f);
		break;
	case 2:
		m_pChai->Get_Model()->Set_Animation(ANIM_CH::PARRY_02, FALSE, 2.2f);
		break;
	default:
		m_pChai->Get_Model()->Set_Animation(ANIM_CH::PARRY_02, FALSE, 2.2f);
		break;
	}

	return S_OK;
}

const wstring& CState_Chai_Parry::Tick(const _float& fTimeDelta)
{
	return m_strName;
}

const wstring& CState_Chai_Parry::LateTick()
{
	return Check_Transition();
}

void CState_Chai_Parry::Exit()
{
	m_pChai->m_tFightDesc.bParry = FALSE;
}

const wstring& CState_Chai_Parry::Check_Transition()
{
	if (m_pChai->Get_Model()->Is_Tween())
		return m_strName;

	if (m_pChai->Get_Model()->Is_TwoThirds_Animation())
	{
		return StateNames[STATE_IDLE];
	}

	return m_strName;
}

CState_Chai_Parry* CState_Chai_Parry::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Chai_Parry* pInstance = new CState_Chai_Parry();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Chai_Parry");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Chai_Parry::Clone(void* pArg)
{
	return nullptr;
}

void CState_Chai_Parry::Free()
{
}
