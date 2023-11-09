#include "..\Default\stdafx.h"
#include "State_Saber_Idle.h"

#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG

CState_Saber_Idle::CState_Saber_Idle()
{
}

CState_Saber_Idle::CState_Saber_Idle(const CState_Saber_Idle& rhs)
{
}

HRESULT CState_Saber_Idle::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Saber_Idle::Enter()
{
	if (StateNames_SA[STATE_APPEAR_SA] == m_pStateMachine->Get_PrevState()->Get_Name())
	{
		ANIM_SA			eAnimID = ANIM_SA::IDLE_SA;
		CAnimation*		pAnimation = m_pModel->Get_Animation(eAnimID);
		const _double	fTimePerFrame = CBeatManager::GetInstance()->Get_AnimTimePerFrame(pAnimation) * (_double)2.f;

		m_fTimeLimit = rand() % 2 + (rand() % 10) * 0.1f + 2;

		m_pModel->Set_Animation(eAnimID, fTimePerFrame, DF_TW_TIME);
	}
	else
	{
		Set_NextAnimation();
	}
	
	return S_OK;
}

const wstring CState_Saber_Idle::Tick(const _double& fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	Look_Target();

	return m_strName;
}

const wstring CState_Saber_Idle::LateTick()
{
	return Check_Transition();
}

void CState_Saber_Idle::Exit()
{
	m_fTimeAcc = 0.f;
	m_fTimeLimit = 0.f;
}

const wstring CState_Saber_Idle::Check_Transition()
{
	if (m_pSaber->m_tFightDesc.bDamaged)
	{
		return StateNames_SA[STATE_DAMAGED_SA];
	}

	if (m_pModel->Is_Tween() || m_fTimeLimit > m_fTimeAcc)
		return m_strName;

	const wstring strNextState = Choice_NextState();

	if (strNextState == m_strName)
	{
		Set_NextAnimation();

		return m_strName;
	}
	else
		return strNextState;

	return m_strName;
}

void CState_Saber_Idle::Set_NextAnimation()
{
	_int iNum = rand() % 4;

	ANIM_SA			eAnimID = ANIM_SA::ANIM_SA_END;
	CAnimation*		pAnimation = nullptr;
	_double			fTimePerFrame;
	_double			fDuration;

	switch (iNum)
	{
		case 0 : 
		{
			eAnimID = ANIM_SA::IDLE_SA;
			pAnimation = m_pModel->Get_Animation(eAnimID);
			fTimePerFrame = CBeatManager::GetInstance()->Get_AnimTimePerFrame(pAnimation) * (_double)2.f;

			m_fTimeLimit = rand() % 3 + 1;
		}
		break;
		case 1:
		{
			eAnimID = ANIM_SA::ACCENT_00_SA;
			pAnimation = m_pModel->Get_Animation(eAnimID);
			fDuration = CBeatManager::GetInstance()->Get_SPB(4);
			fTimePerFrame = fDuration / (_double)55.f;

			m_fTimeLimit = fDuration;
		}
		break;
		case 2:
		{
			eAnimID = ANIM_SA::ACCENT_01_SA;
			pAnimation = m_pModel->Get_Animation(eAnimID);
			fDuration = CBeatManager::GetInstance()->Get_SPB(4);
			fTimePerFrame = fDuration / (_double)55.f;

			m_fTimeLimit = fDuration;
		}
		break;
		case 3:
		{
			eAnimID = ANIM_SA::ACCENT_02_SA;
			pAnimation = m_pModel->Get_Animation(eAnimID);
			fDuration = CBeatManager::GetInstance()->Get_SPB(4);
			fTimePerFrame = fDuration / (_double)55.f;

			m_fTimeLimit = fDuration;
		}
		break;
		default:
		{
			eAnimID = ANIM_SA::IDLE_SA;
			pAnimation = m_pModel->Get_Animation(eAnimID);
			fTimePerFrame = CBeatManager::GetInstance()->Get_AnimTimePerFrame(pAnimation) * (_double)2.f;

			m_fTimeLimit = rand() % 3 + 2;
		}
		break;
	}
	
	m_fTimeAcc = 0.f;

	m_pModel->Set_Animation(eAnimID, fTimePerFrame, DF_TW_TIME);
}

CState_Saber_Idle* CState_Saber_Idle::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Saber_Idle* pInstance = new CState_Saber_Idle();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Saber_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Saber_Idle::Clone(void* pArg)
{
	return nullptr;
}

void CState_Saber_Idle::Free()
{
}
