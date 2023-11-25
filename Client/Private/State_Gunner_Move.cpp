#include "..\Default\stdafx.h"
#include "State_Gunner_Move.h"

CState_Gunner_Move::CState_Gunner_Move()
{
}

CState_Gunner_Move::CState_Gunner_Move(const CState_Gunner_Move& rhs)
{
}

HRESULT CState_Gunner_Move::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Gunner_Move::Enter()
{
	Set_NextAnimation();

	return S_OK;
}

const wstring CState_Gunner_Move::Tick(const _double& fTimeDelta)
{
	return m_strName;
}

const wstring CState_Gunner_Move::LateTick()
{
	return Check_Transition();
}

void CState_Gunner_Move::Exit()
{
	m_eDirType = DIR_TYPE::TYPEEND;
}

const wstring CState_Gunner_Move::Check_Transition()
{
	if (m_pGunner->m_tFightDesc.bDamaged)
		return StateNames_GU[STATE_DAMAGED_GU];

	if (!m_pModel->Is_Tween() && m_pModel->Is_Quater_Animation())
		return StateNames_GU[STATE_IDLE_GU];

	return m_strName;
}

void CState_Gunner_Move::Set_Direction()
{
	const _float fTargetDistance = Get_Distance();

	if (fTargetDistance <= m_fTargetMinDist)
	{
		m_eDirType = DIR_TYPE::BACKWARD;
	}
	else if (fTargetDistance >= m_fTargetMaxDist)
	{
		m_eDirType = DIR_TYPE::FORWARD;
	}
	else
	{
		const _int iRand = rand() % 2;
		if (0 == iRand)
		{
			m_eDirType = DIR_TYPE::LEFT; 
		}
		else
		{
			m_eDirType = DIR_TYPE::RIGHT;
		}
	}
}

void CState_Gunner_Move::Set_NextAnimation()
{
	Set_Direction();

	CAnimation*		pAnimation = nullptr;

	switch (m_eDirType)
	{
	case DIR_TYPE::FORWARD:
	{
		pAnimation = m_pModel->Get_Animation(AnimNames_GU[ANIM_GU::ESCAPE_FORWARD]);
	}
		break;
	case DIR_TYPE::BACKWARD:
	{
		pAnimation = m_pModel->Get_Animation(AnimNames_GU[ANIM_GU::ESCAPE_BACKWARD]);
	}
		break;
	case DIR_TYPE::LEFT:
	{
		pAnimation = m_pModel->Get_Animation(AnimNames_GU[ANIM_GU::ESCAPE_LEFT]);
	}
		break;
	case DIR_TYPE::RIGHT:
	{
		pAnimation = m_pModel->Get_Animation(AnimNames_GU[ANIM_GU::ESCAPE_RIGHT]);
	}
		break;
	default:
	{
		pAnimation = m_pModel->Get_Animation(AnimNames_GU[ANIM_GU::ESCAPE_BACKWARD]);
	}
		break;
	}

	m_bLookTarget = FALSE;

	m_pModel->Set_Animation(pAnimation, pAnimation->Get_TickPerFrame(), DF_TW_TIME);

	//ENGINE_INSTANCE->Play_Sound(EFC_SABER_ESCAPING, ENEMY_SABER, 0.4f);
}


CState_Gunner_Move* CState_Gunner_Move::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Gunner_Move* pInstance = new CState_Gunner_Move();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Gunner_Move");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Gunner_Move::Clone(void* pArg)
{
	return nullptr;
}

void CState_Gunner_Move::Free()
{
}
