#include "..\Default\stdafx.h"
#include "State_Saber_Move.h"

CState_Saber_Move::CState_Saber_Move()
{
}

CState_Saber_Move::CState_Saber_Move(const CState_Saber_Move& rhs)
{
}

HRESULT CState_Saber_Move::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Saber_Move::Enter()
{
	Set_NextAnimation();

	return S_OK;
}

const wstring CState_Saber_Move::Tick(const _double& fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	return m_strName;
}

const wstring CState_Saber_Move::LateTick()
{
	return Check_Transition();
}

void CState_Saber_Move::Exit()
{
	m_fTimeAcc = 0.f;
	m_fTimeLimit = 0.f;

	m_eDirType = DIR_TYPE::TYPEEND;
}

const wstring CState_Saber_Move::Check_Transition()
{
	if (m_pSaber->m_tFightDesc.bDamaged)
	{
		return StateNames_SA[STATE_DAMAGED_SA];
	}

	/* 최소 거리 필터링 먼저 */
	if (Get_Distance() <= m_fTargetMinDist)
	{
		const wstring strNextState = Choice_NextState();

		if (strNextState == m_strName)
		{
			Set_NextAnimation();

			return m_strName;
		}
		else
			return strNextState;
	}

	/* 일반 필터링 시작 */
	if (m_pModel->Is_Tween())
		return m_strName;

	if (m_fTimeLimit <= m_fTimeAcc)
	{
		return StateNames_SA[STATE_IDLE_SA];
	}

	return m_strName;
}

void CState_Saber_Move::Set_Direction()
{
	const _float fTargetDistance = Get_Distance();

	if (fTargetDistance <= m_fTargetMinDist)
	{
		if (0 == rand())
			m_eDirType = DIR_TYPE::LEFT;
		else
			m_eDirType = DIR_TYPE::RIGHT;
	}
	else if (fTargetDistance >= m_fTargetMaxDist)
	{
		m_eDirType = DIR_TYPE::FORWARD;
	}
	else
	{
		_int iDir = rand() % 4;

		switch (iDir)
		{
			case 0: m_eDirType = DIR_TYPE::FORWARD; break;
			case 1: m_eDirType = DIR_TYPE::BACKWARD; break;
			case 2: m_eDirType = DIR_TYPE::LEFT; break;
			case 3: m_eDirType = DIR_TYPE::RIGHT; break;
			default : m_eDirType = DIR_TYPE::FORWARD; break;
		}
	}
}

void CState_Saber_Move::Set_NextAnimation()
{
	Set_Direction();

	ANIM_SA			eAnimID = ANIM_SA::ANIM_SA_END;
	CAnimation*		pAnimation = nullptr;
	_double			fDuration = 0.f;
	_double			fTimePerFrame = 0.f;

	switch (m_eDirType)
	{
	case DIR_TYPE::FORWARD:
	{
		/*if (m_fTargetMaxDist * 0.8f <= Get_Distance())
		{
			eAnimID = ANIM_SA::RUN_ING_SA;
			pAnimation = m_pModel->Get_Animation(eAnimID);
			fDuration = CBeatManager::GetInstance()->Get_SPB(2);
			m_fTimeLimit = fDuration;
			fTimePerFrame = fDuration / pAnimation->Get_MaxFrameCount();
		}
		else*/
		{
			//if (0 == rand() % 2)
			{
				eAnimID = ANIM_SA::ESC_FORWARD_LOOK_FORWARD_00_SA;
				fDuration = CBeatManager::GetInstance()->Get_SPB(2);
				m_fTimeLimit = fDuration;
			}
		/*	else
			{
				eAnimID = ANIM_SA::WALK_FORWARD_LOOK_FORWARD_SA;
				fDuration = CBeatManager::GetInstance()->Get_SPB(2);
				m_fTimeLimit = fDuration * (1 + (rand() % 10 * 0.1f));
			}*/

			pAnimation = m_pModel->Get_Animation(eAnimID);
			fTimePerFrame = fDuration / pAnimation->Get_MaxFrameCount();
		}

		m_bLookTarget = FALSE;
	}
		break;
	case DIR_TYPE::BACKWARD:
	{
		//if (0 == rand() % 2)
		{
			eAnimID = ANIM_SA::ESC_BACKWARD_LOOK_FORWARD_00_SA;
			fDuration = CBeatManager::GetInstance()->Get_SPB(2);
			m_fTimeLimit = fDuration;
		}
		/*else
		{
			eAnimID = ANIM_SA::WALK_BACKWARD_LOOK_FORWARD_SA;
			fDuration = CBeatManager::GetInstance()->Get_SPB(2);
			m_fTimeLimit = fDuration * (1 + (rand() % 10 * 0.1f));
		}*/

		m_bLookTarget = FALSE;

		pAnimation = m_pModel->Get_Animation(eAnimID);
		
		fTimePerFrame = fDuration / pAnimation->Get_MaxFrameCount();
	}
		break;
	case DIR_TYPE::LEFT:
	{
		//if (0 == rand() % 2)
		{
			eAnimID = ANIM_SA::ESC_LEFT_LOOK_FORWARD_00_SA;
			fDuration = CBeatManager::GetInstance()->Get_SPB(2);
			m_fTimeLimit = fDuration;
			m_bLookTarget = FALSE;
		}
		/*else
		{
			eAnimID = ANIM_SA::WALK_LEFT_LOOK_FORWARD_SA;
			fDuration = CBeatManager::GetInstance()->Get_SPB(2);
			m_fTimeLimit = fDuration * (1 + (rand() % 10 * 0.1f));
			m_bLookTarget = TRUE;
		}*/

		pAnimation = m_pModel->Get_Animation(eAnimID);
		fTimePerFrame = fDuration / pAnimation->Get_MaxFrameCount();
	}
		break;
	case DIR_TYPE::RIGHT:
	{
		//if (0 == rand() % 2)
		{
			eAnimID = ANIM_SA::ESC_RIGHT_LOOK_FORWARD_00_SA;
			fDuration = CBeatManager::GetInstance()->Get_SPB(2);
			m_fTimeLimit = fDuration;
			m_bLookTarget = FALSE;
		}
		/*else
		{
			eAnimID = ANIM_SA::WALK_RIGHT_LOOK_FORWARD_SA;
			fDuration = CBeatManager::GetInstance()->Get_SPB(2);
			m_fTimeLimit = fDuration * (1 + (rand() % 10 * 0.1f));
			m_bLookTarget = TRUE;
		}*/

		pAnimation = m_pModel->Get_Animation(eAnimID);
		fTimePerFrame = fDuration / pAnimation->Get_MaxFrameCount();
	}
		break;

	default:
		break;
	}

	m_fTimeAcc = 0.f;
	
	m_pModel->Set_Animation(eAnimID, fTimePerFrame, DF_TW_TIME);

	ENGINE_INSTANCE->Play_Sound(EFC_SABER_ESCAPING, ENEMY_SABER, 0.4f);

}


CState_Saber_Move* CState_Saber_Move::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Saber_Move* pInstance = new CState_Saber_Move();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Saber_Move");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Saber_Move::Clone(void* pArg)
{
	return nullptr;
}

void CState_Saber_Move::Free()
{
}
