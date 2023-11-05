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
	m_tAttackDesc.Reset();

	if (Input::LBtn())
	{
		m_tAttackDesc.iAnimIndex		= ANIM_CH::ATK_LIGHT_00;
		m_tAttackDesc.fTweenTime		= DF_TW_TIME * 0.5f;
		m_tAttackDesc.dTimePerFrame		= CBeatManager::GetInstance()->Get_SPB(4) 
											/ (_double)m_pChai->Get_Model()->Get_Animation(m_tAttackDesc.iAnimIndex)->Get_MaxFrameCount();

		m_tAttackDesc.eAttackType		= ATTACK_TYPE::LIGHT;
		m_tAttackDesc.eAnimCheckType	= CModel::ANIM_PROGRESS::QUATER;
	}
	else if (Input::RBtn())
	{
		m_tAttackDesc.iAnimIndex		= ANIM_CH::ATK_SINGLE_00;
		m_tAttackDesc.fTweenTime		= DF_TW_TIME * 0.5f;
		m_tAttackDesc.dTimePerFrame		= CBeatManager::GetInstance()->Get_SPB(8)
											/ (_double)m_pChai->Get_Model()->Get_Animation(m_tAttackDesc.iAnimIndex)->Get_MaxFrameCount();

		m_tAttackDesc.eAttackType		= ATTACK_TYPE::STRONG;
		m_tAttackDesc.eAnimCheckType	= CModel::ANIM_PROGRESS::HALF;
	}

	m_pChai->m_tFightDesc.iStep++;
	m_tAttackDesc.bSet = TRUE;

	cout << "\n\nATTACK ENTER\n\n\n";
	return S_OK;
}

const wstring& CState_Chai_Attack::Tick(const _double& fTimeDelta)
{
	Set_AttackDesc();

	if (m_tAttackDesc.bSet)
	{
		CModel* pModel = m_pChai->Get_Model();

		/* 첫 공격 */
		if (0 == m_pChai->m_tFightDesc.iStep && CBeatManager::GetInstance()->Is_SteadyBeat())
		{
			ENGINE_INSTANCE->Play_Sound(EFC_CHAI_ATTACK_SWING_NONCOLLISION, PLAYER_CHAI, EfcVolumeChai);
			pModel->Set_Animation(m_tAttackDesc.iAnimIndex, m_tAttackDesc.dTimePerFrame, m_tAttackDesc.fTweenTime);
			m_tAttackDesc.bSet = FALSE;

			m_tAttackDesc.bFirstAttack = TRUE;

		}/* 콤보 공격 */
		else if(1 <= m_pChai->m_tFightDesc.iStep && !pModel->Is_Tween())
		{
			switch (m_tAttackDesc.eAnimCheckType)
			{
			case Engine::CModel::QUATER:
				if (pModel->Is_Quater_Animation())
				{
					ENGINE_INSTANCE->Play_Sound(EFC_CHAI_ATTACK_SWING_NONCOLLISION, PLAYER_CHAI, EfcVolumeChai);
					pModel->Set_Animation(m_tAttackDesc.iAnimIndex, m_tAttackDesc.dTimePerFrame, m_tAttackDesc.fTweenTime);
					m_tAttackDesc.bSet = FALSE;
				}
				break;
			case Engine::CModel::ONE_THIRDS:
				if (pModel->Is_OneThirds_Animation())
				{
					ENGINE_INSTANCE->Play_Sound(EFC_CHAI_ATTACK_SWING_NONCOLLISION, PLAYER_CHAI, EfcVolumeChai);
					pModel->Set_Animation(m_tAttackDesc.iAnimIndex, m_tAttackDesc.dTimePerFrame, m_tAttackDesc.fTweenTime);
					m_tAttackDesc.bSet = FALSE;
				}
				break;
			case Engine::CModel::HALF:
				if (pModel->Is_Half_Animation())
				{
					ENGINE_INSTANCE->Play_Sound(EFC_CHAI_ATTACK_SWING_NONCOLLISION, PLAYER_CHAI, EfcVolumeChai);
					pModel->Set_Animation(m_tAttackDesc.iAnimIndex, m_tAttackDesc.dTimePerFrame, m_tAttackDesc.fTweenTime);
					m_tAttackDesc.bSet = FALSE;
				}
				break;
			case Engine::CModel::TWO_THIRDS:
				if (pModel->Is_TwoThirds_Animation())
				{
					ENGINE_INSTANCE->Play_Sound(EFC_CHAI_ATTACK_SWING_NONCOLLISION, PLAYER_CHAI, EfcVolumeChai);
					pModel->Set_Animation(m_tAttackDesc.iAnimIndex, m_tAttackDesc.dTimePerFrame, m_tAttackDesc.fTweenTime);
					m_tAttackDesc.bSet = FALSE;
				}
				break;
			case Engine::CModel::FINISH:
				if (pModel->Is_Finish_Animation())
				{
					ENGINE_INSTANCE->Play_Sound(EFC_CHAI_ATTACK_SWING_NONCOLLISION, PLAYER_CHAI, EfcVolumeChai);
					pModel->Set_Animation(m_tAttackDesc.iAnimIndex, m_tAttackDesc.dTimePerFrame, m_tAttackDesc.fTweenTime);
					m_tAttackDesc.bSet = FALSE;
				}
				break;
			default:
				break;
			}
		}
	}
	return m_strName;
}

const wstring& CState_Chai_Attack::LateTick()
{
	return Check_Transition();
}

void CState_Chai_Attack::Exit()
{
	cout << "\n\nATTACK Exit\n\n\n";

	m_tAttackDesc.Reset();

	m_pChai->m_tFightDesc.iStep = -1;

	m_pChai->Get_Child(CChai::CH_WEAPON_RIGHT)->Get_Collider_Sphere()->Set_Active(FALSE);
}

void CState_Chai_Attack::OnCollision_Enter(CGameObject* pGameObject)
{
	if (nullptr == pGameObject) return;
}

void CState_Chai_Attack::OnCollision_Stay(CGameObject* pGameObject)
{
}

void CState_Chai_Attack::OnCollision_Exit(CGameObject* pGameObject)
{
}

const wstring& CState_Chai_Attack::Check_Transition()
{
	if (!m_tAttackDesc.bFirstAttack)
		return m_strName;

	if (!m_tAttackDesc.bSet)
	{
		CModel*	pModel = m_pChai->Get_Model();
		CModel::TweenDesc desc = pModel->Get_TweenDesc();

		if (ANIM_CH::ATK_LIGHT_02 == m_tAttackDesc.iAnimIndex)
		{
			if (!pModel->Is_Tween() && 40 <= desc.cur.iCurFrame)
				return StateNames[STATE_IDLE];
		}
		else if (ANIM_CH::ATK_LIGHT_03 == m_tAttackDesc.iAnimIndex)
		{
			if(!pModel->Is_Tween() && 70 <= desc.cur.iCurFrame )
				return StateNames[STATE_IDLE];
		}
		else
		{
			if(!pModel->Is_Tween() && pModel->Is_TwoThirds_Animation())
				return StateNames[STATE_IDLE];
		}
	}

	return m_strName;
}

void CState_Chai_Attack::Set_AttackDesc()
{
	/* 이미 세팅되어 있는 경우 리턴 */
	if (m_tAttackDesc.bSet)
		return;

	/* 현재 애니메이션에 해당하는 타이밍을 지났다면 리턴 */
	CModel* pModel = m_pChai->Get_Model();

	switch (m_tAttackDesc.eAnimCheckType)
	{
	case CModel::QUATER:
		if (pModel->Is_Quater_Animation()) return; break;
	case CModel::ONE_THIRDS:
		if (pModel->Is_OneThirds_Animation()) return; break;
	case CModel::HALF:
		if (pModel->Is_Half_Animation()) return; break;
	case CModel::TWO_THIRDS:
		if (pModel->Is_TwoThirds_Animation()) return; break;
	case CModel::FINISH:
		if (pModel->Is_Finish_Animation()) return; break;
	default: return; break;
	}

	/* 다음 공격 세팅 */
	if (Input::Attack() && Input::LBtn())
	{
		if (ATTACK_TYPE::STRONG == m_tAttackDesc.eAttackType)
		{

		}
		
		if (0 == m_pChai->m_tFightDesc.iStep)
		{
			m_tAttackDesc.iAnimIndex = ANIM_CH::ATK_LIGHT_01;
			m_tAttackDesc.dTimePerFrame = CBeatManager::GetInstance()->Get_SPB(4)
				/ (_double)m_pChai->Get_Model()->Get_Animation(m_tAttackDesc.iAnimIndex)->Get_MaxFrameCount();

			m_tAttackDesc.eAnimCheckType = CModel::ANIM_PROGRESS::QUATER;
		}
		else if (1 == m_pChai->m_tFightDesc.iStep)
		{
			m_tAttackDesc.iAnimIndex = ANIM_CH::ATK_LIGHT_02;
			m_tAttackDesc.dTimePerFrame = CBeatManager::GetInstance()->Get_SPB(4)
				/ (_double)m_pChai->Get_Model()->Get_Animation(m_tAttackDesc.iAnimIndex)->Get_MaxFrameCount();

			m_tAttackDesc.eAnimCheckType = CModel::ANIM_PROGRESS::QUATER;
		}
		else if (2 == m_pChai->m_tFightDesc.iStep)
		{
			m_tAttackDesc.iAnimIndex = ANIM_CH::ATK_LIGHT_03;
			m_tAttackDesc.dTimePerFrame = CBeatManager::GetInstance()->Get_SPB(4)
				/ (_double)m_pChai->Get_Model()->Get_Animation(m_tAttackDesc.iAnimIndex)->Get_MaxFrameCount();
			
			m_tAttackDesc.eAnimCheckType = CModel::ANIM_PROGRESS::QUATER;
		}
		else if (3 <= m_pChai->m_tFightDesc.iStep)
		{
			return;
		}


		m_tAttackDesc.eAttackType		= ATTACK_TYPE::LIGHT;
		m_tAttackDesc.fTweenTime		= DF_TW_TIME * 0.5f;
		m_tAttackDesc.bSet = TRUE;

		m_pChai->m_tFightDesc.iStep++;



	}
	else if (Input::Attack() && Input::RBtn())
	{
		if (ATTACK_TYPE::LIGHT == m_tAttackDesc.eAttackType)
		{

		}
		


		m_tAttackDesc.eAttackType = ATTACK_TYPE::STRONG;
		m_tAttackDesc.bSet = TRUE;

		m_pChai->m_tFightDesc.iStep++;


	}
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
