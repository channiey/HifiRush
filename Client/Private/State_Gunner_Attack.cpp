#include "..\Default\stdafx.h"
#include "State_Gunner_Attack.h"

CState_Gunner_Attack::CState_Gunner_Attack()
{
}

CState_Gunner_Attack::CState_Gunner_Attack(const CState_Gunner_Attack& rhs)
{
}

HRESULT CState_Gunner_Attack::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Gunner_Attack::Enter()
{
	const _int	iRand = rand() % 2;
	CAnimation* pAnimation = nullptr;

	if (0 == iRand)
	{
		m_eAttackType = ATTACK_TYPE::GROUND;
		pAnimation = m_pModel->Get_Animation(AnimNames_GU[ATK_GROUND_INTRO_GU]);
		m_pModel->Set_Animation(pAnimation, pAnimation->Get_TickPerFrame(), DF_TW_TIME);
	}
	else
	{
		m_eAttackType = ATTACK_TYPE::AIR;
		pAnimation = m_pModel->Get_Animation(AnimNames_GU[ATK_JUMP_INTRO_GU]);
		m_pModel->Set_Animation(pAnimation, pAnimation->Get_TickPerFrame(), DF_TW_TIME, FALSE);
	}


	//ENGINE_INSTANCE->Play_Sound(EFC_SABER_CHARGE, ENEMY_SABER, 0.6f);

	return S_OK;
}

const wstring CState_Gunner_Attack::Tick(const _double& fTimeDelta)
{
	return m_strName;
}

const wstring CState_Gunner_Attack::LateTick()
{
	return Check_Transition();
}

void CState_Gunner_Attack::Exit()
{
	m_eAttackType = ATTACK_TYPE::TYPEEND;
}

const wstring CState_Gunner_Attack::Check_Transition()
{
	if(m_pModel->Is_Tween())
		return m_strName;

	const string	strCurAnimName = m_pModel->Get_CurAnimation()->Get_Name();
	const _int		iCurFrame = m_pModel->Get_TweenDesc().cur.iCurFrame;

	if (AnimNames_GU[ANIM_GU::ATK_GROUND_INTRO_GU] == strCurAnimName)
	{
		if (20 == iCurFrame)
		{
			//m_pModel->Clear_Animation();
			CAnimation* pAnimation = m_pModel->Get_Animation(AnimNames_GU[ATK_GROUND_WAIT_GU]);
			m_pModel->Set_Animation(pAnimation, pAnimation->Get_TickPerFrame(), DF_TW_TIME);
		}
	}
	else if (AnimNames_GU[ANIM_GU::ATK_GROUND_WAIT_GU] == strCurAnimName)
	{
		if (25 == iCurFrame)
		{
			//m_pModel->Clear_Animation();
			CAnimation* pAnimation = m_pModel->Get_Animation(AnimNames_GU[ATK_GROUND_SHOOT_GU]);
			m_pModel->Set_Animation(pAnimation, pAnimation->Get_TickPerFrame(), DF_TW_TIME);
		}

	}
	else if (AnimNames_GU[ANIM_GU::ATK_GROUND_SHOOT_GU] == strCurAnimName)
	{
		if (40 == iCurFrame)
		{
			return StateNames_GU[STATE_GU::STATE_IDLE_GU];
		}
	}
	else if (AnimNames_GU[ANIM_GU::ATK_JUMP_INTRO_GU] == strCurAnimName)
	{
		if (20 == iCurFrame)
		{
			m_pModel->Clear_Animation();
			CAnimation* pAnimation = m_pModel->Get_Animation(AnimNames_GU[ATK_JUMP_WAIT_GU]);
			m_pModel->Set_Animation(pAnimation, pAnimation->Get_TickPerFrame(), DF_TW_TIME, FALSE);
		}
	}
	else if (AnimNames_GU[ANIM_GU::ATK_JUMP_WAIT_GU] == strCurAnimName)
	{
		if (25 == iCurFrame)
		{
			m_pModel->Clear_Animation();
			CAnimation* pAnimation = m_pModel->Get_Animation(AnimNames_GU[ATK_JUMP_SHOOT_GU]);
			m_pModel->Set_Animation(pAnimation, pAnimation->Get_TickPerFrame(), DF_TW_TIME, FALSE);
		}
	}
	else if (AnimNames_GU[ANIM_GU::ATK_JUMP_SHOOT_GU] == strCurAnimName)
	{
		if (22 == iCurFrame)
		{
			return StateNames_GU[STATE_GU::STATE_IDLE_GU];
		}
	}

	return m_strName;
}

CState_Gunner_Attack* CState_Gunner_Attack::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Gunner_Attack* pInstance = new CState_Gunner_Attack();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Gunner_Attack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Gunner_Attack::Clone(void* pArg)
{
	return nullptr;
}

void CState_Gunner_Attack::Free()
{
}
