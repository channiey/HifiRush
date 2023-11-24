#include "..\Default\stdafx.h"
#include "State_Gunner_NonCombat.h"

CState_Gunner_NonCombat::CState_Gunner_NonCombat()
{
}

CState_Gunner_NonCombat::CState_Gunner_NonCombat(const CState_Gunner_NonCombat& rhs)
{
}

HRESULT CState_Gunner_NonCombat::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Gunner_NonCombat::Enter()
{
	srand(time(NULL));

	m_pModel->Clear_Animation();

	const _int iRand = rand() % 2;
	{
		if (0 == iRand)
			m_eAnimType = NON_COMBAT_00_GU;
		else
			m_eAnimType = NON_COMBAT_01_00_GU;
	}

	CAnimation*		pAnimation = m_pModel->Get_Animation(AnimNames_GU[m_eAnimType]);

	m_pModel->Set_Animation(pAnimation, pAnimation->Get_TickPerFrame(), DF_TW_TIME);

	return S_OK;
}

const wstring CState_Gunner_NonCombat::Tick(const _double& fTimeDelta)
{
	return m_strName;
}

const wstring CState_Gunner_NonCombat::LateTick()
{
	return Check_Transition();
}

void CState_Gunner_NonCombat::Exit()
{
	m_eAnimType = ANIM_GU::ANIM_END_GU;
}

const wstring CState_Gunner_NonCombat::Check_Transition()
{
	if (m_pModel->Is_Tween())
		return m_strName;

	switch (m_eAnimType)
	{
	case ANIM_GU::NON_COMBAT_00_GU:
	{
		if (!m_pModel->Is_Tween() && m_pModel->Is_Quater_Animation())
		{
			m_eAnimType = NON_COMBAT_TO_COMBAT_GU;

			CAnimation* pAnimation = m_pModel->Get_Animation(AnimNames_GU[m_eAnimType]);

			m_pModel->Set_Animation(pAnimation, pAnimation->Get_TickPerFrame(), DF_TW_TIME);
		}
	}
		break;
	case ANIM_GU::NON_COMBAT_01_00_GU:
	{
		if (!m_pModel->Is_Tween() && m_pModel->Is_Quater_Animation())
		{
			m_eAnimType = NON_COMBAT_01_01_GU;

			CAnimation* pAnimation = m_pModel->Get_Animation(AnimNames_GU[m_eAnimType]);

			m_pModel->Set_Animation(pAnimation, pAnimation->Get_TickPerFrame(), DF_TW_TIME);
		}
	}
		break;
	case ANIM_GU::NON_COMBAT_01_01_GU:
	{
		if (!m_pModel->Is_Tween() && m_pModel->Is_Quater_Animation())
		{
			m_eAnimType = NON_COMBAT_TO_COMBAT_GU;

			CAnimation* pAnimation = m_pModel->Get_Animation(AnimNames_GU[m_eAnimType]);

			m_pModel->Set_Animation(pAnimation, pAnimation->Get_TickPerFrame(), DF_TW_TIME);
		}
	}
		break;
	case ANIM_GU::NON_COMBAT_TO_COMBAT_GU:
	{
		if (!m_pModel->Is_Tween())
		{
			Look_Target();
		}

		if (!m_pModel->Is_Tween() && m_pModel->Is_Quater_Animation())
		{
			return StateNames_GU[STATE_IDLE_GU];
		}
	}
		break;
	default:
	{
		return StateNames_GU[STATE_IDLE_GU];
	}
		break;
	}
	
	return m_strName;
}

CState_Gunner_NonCombat* CState_Gunner_NonCombat::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Gunner_NonCombat* pInstance = new CState_Gunner_NonCombat();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Gunner_NonCombat");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Gunner_NonCombat::Clone(void* pArg)
{
	return nullptr;
}

void CState_Gunner_NonCombat::Free()
{
}
