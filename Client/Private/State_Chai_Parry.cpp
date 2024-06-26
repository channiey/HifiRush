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
	ANIM_CH	eAnimID	= ANIM_CH::ANIM_CH_END;

	const _int iRand = rand() % 3;

	switch (iRand)
	{
	case 0 :
		eAnimID = ANIM_CH::PARRY_00;
		break;
	case 2:
		eAnimID = ANIM_CH::PARRY_01;
		break;
	default:
		eAnimID = ANIM_CH::PARRY_02;
		break;
	}
	CAnimation*		pAnimation		= m_pChai->Get_Model()->Get_Animation(eAnimID);
	const _double	fTimePerFrame	= 1 / pAnimation->Get_TickPerSecond() * 0.6f;

	m_pChai->Get_Model()->Set_Animation(eAnimID, fTimePerFrame, 0.05f);

	m_pChai->m_tFightDesc.bParry = TRUE;

	return S_OK;
}

const wstring CState_Chai_Parry::Tick(const _double& fTimeDelta)
{
	return m_strName;
}

const wstring CState_Chai_Parry::LateTick()
{
	return Check_Transition();
}

void CState_Chai_Parry::Exit()
{
	m_pChai->m_tFightDesc.bParry = FALSE;
}

const wstring CState_Chai_Parry::Check_Transition()
{
	if (m_pChai->Get_Model()->Is_Tween())
		return m_strName;

	if (m_pChai->Get_Model()->Is_TwoThirds_Animation())
	{
		return StateNames_CH[STATE_IDLE_CH];		
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
