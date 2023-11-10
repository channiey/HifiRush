#include "..\Default\stdafx.h"
#include "State_Saber_Damaged.h"

CState_Saber_Damaged::CState_Saber_Damaged()
{
}

CState_Saber_Damaged::CState_Saber_Damaged(const CState_Saber_Damaged& rhs)
{
}

HRESULT CState_Saber_Damaged::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Saber_Damaged::Enter()
{
	Damaged();

	return S_OK;
}

const wstring CState_Saber_Damaged::Tick(const _double& fTimeDelta)
{
	if (m_pSaber->m_tFightDesc.bDamaged)
	{
		Damaged();
	}

	return m_strName;
}

const wstring CState_Saber_Damaged::LateTick()
{
	return Check_Transition();
}

void CState_Saber_Damaged::Exit()
{
	m_pSaber->m_tFightDesc.bDamaged = FALSE;
	m_pSaber->m_tFightDesc.pAttacker = nullptr;
}

const wstring CState_Saber_Damaged::Check_Transition()
{
	if(m_pModel->Is_Tween())	
		return m_strName;

	if (m_pModel->Is_ThreeFourths_Animation())
		return StateNames_SA[STATE_IDLE_SA];

	return m_strName;
}

void CState_Saber_Damaged::Damaged()
{
	if (m_pSaber->m_tStatDesc.bDead)
	{
		m_pStateMachine->Set_State(StateNames_SA[STATE_DEAD_SA]);
		return;
	}

	ANIM_SA		eAnimID = ANIM_SA_END;
	CAnimation* pAnimation = nullptr;
	_double		fTimePerFrame = 0.f;

	const _int iRand = rand() % 3;

	if (0 == iRand % 2)
	{
		switch (iRand)
		{
		case 0:
		{
			eAnimID = ANIM_SA::DAMAGED_LIGHT_00_SA;
			pAnimation = m_pModel->Get_Animation(eAnimID);
			fTimePerFrame = CBeatManager::CBeatManager::GetInstance()->Get_SPB(2) / (_double)35.f;
		}
		break;
		case 1:
		{
			eAnimID = ANIM_SA::DAMAGED_LIGHT_03_SA;
			pAnimation = m_pModel->Get_Animation(eAnimID);
			fTimePerFrame = CBeatManager::CBeatManager::GetInstance()->Get_SPB(2) / (_double)35.f;
		}
		break;
		default:
		{
			eAnimID = ANIM_SA::DAMAGED_LIGHT_04_SA;
			pAnimation = m_pModel->Get_Animation(eAnimID);
			fTimePerFrame = CBeatManager::CBeatManager::GetInstance()->Get_SPB(2) / (_double)30.f;
		}
		break;
		}
	}
	else
	{
		switch (iRand)
		{
		case 0:
		{
			eAnimID = ANIM_SA::DAMAGED_MIDDLE_00_SA;
			pAnimation = m_pModel->Get_Animation(eAnimID);
			fTimePerFrame = CBeatManager::CBeatManager::GetInstance()->Get_SPB(2) / (_double)40.f;
		}
		break;
		case 1:
		{
			eAnimID = ANIM_SA::DAMAGED_MIDDLE_01_SA;
			pAnimation = m_pModel->Get_Animation(eAnimID);
			fTimePerFrame = CBeatManager::CBeatManager::GetInstance()->Get_SPB(2) / (_double)40.f;
		}
		break;
		default:
		{
			eAnimID = ANIM_SA::DAMAGED_MIDDLE_02_SA;
			pAnimation = m_pModel->Get_Animation(eAnimID);
			fTimePerFrame = CBeatManager::CBeatManager::GetInstance()->Get_SPB(2) / (_double)40.f;
		}
		break;
		}
	}

	m_pModel->Set_Animation(eAnimID, fTimePerFrame, DF_TW_TIME);

	m_pSaber->KnockBack(m_pSaber->Get_FightDesc().pAttacker);


	ENGINE_INSTANCE->Play_Sound(EFC_ENEMY_DAMAGED_00, ENEMY_SABER, 0.4f);


	m_pSaber->m_tFightDesc.bDamaged = FALSE;
	m_pSaber->m_tFightDesc.pAttacker = nullptr;
}

CState_Saber_Damaged* CState_Saber_Damaged::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Saber_Damaged* pInstance = new CState_Saber_Damaged();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Saber_Damaged");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Saber_Damaged::Clone(void* pArg)
{
	return nullptr;
}

void CState_Saber_Damaged::Free()
{
}
