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
	ANIM_CH			eAnimID = ANIM_CH::ANIM_END;
	_double			dAnimSpeed = 0.f;

	if (Input::LBtn())
	{
		m_eAttackType			= ATTACK_TYPE::LIGHT;
		eAnimID					= ANIM_CH::ATK_LIGHT_00;
		CAnimation* pAnimation	= m_pChai->Get_Model()->Get_Animation(eAnimID);
		dAnimSpeed = CBeatManager::GetInstance()->Get_SPB(1) / (_double)pAnimation->Get_MaxFrameCount();
	}
	else if (Input::RBtn())
	{
		m_eAttackType			= ATTACK_TYPE::STRONG;
		eAnimID					= ANIM_CH::ATK_SINGLE_00;
		CAnimation* pAnimation	= m_pChai->Get_Model()->Get_Animation(eAnimID);
		dAnimSpeed = CBeatManager::GetInstance()->Get_SPB(2) / (_double)pAnimation->Get_MaxFrameCount();

	}
	else if (Input::MBtn())
	{
		m_eAttackType			= ATTACK_TYPE::THROW;
		eAnimID					= ANIM_CH::ATK_SINGLE_02;
		CAnimation* pAnimation	= m_pChai->Get_Model()->Get_Animation(eAnimID);
		dAnimSpeed = CBeatManager::GetInstance()->Get_SPB(2) / (_double)pAnimation->Get_MaxFrameCount();

		m_pChai->Get_Transform()->Set_Look(ENGINE_INSTANCE->Get_CurCamera_State(CTransform::STATE_LOOK).ZeroY().Normalized());
	}

	m_pChai->Get_Model()->Set_Animation(eAnimID, dAnimSpeed, 0.05f);
	m_pChai->Get_Model()->Set_RootAnimation(TRUE);

	m_pChai->Get_Child(CChai::CH_WEAPON_RIGHT)->Get_Collider_Sphere()->Set_Active(TRUE);

	return S_OK;
}

const wstring& CState_Chai_Attack::Tick(const _double& fTimeDelta)
{
	CModel* pModel = m_pChai->Get_Model();
	
	// 어차피 공격 애니메이션은 비트 시간 단위로 실행 
	if (Input::Attack() && !pModel->Is_Tween() && pModel->Is_Finish_Animation()) // CBeatManager::GetInstance()->Is_HalfBeat()
	{
		++m_pChai->m_tFightDesc.iStep;
		
		ANIM_CH			eAnimID = ANIM_CH::ANIM_END;
		_double			dAnimSpeed = 0.f;

		switch (m_pChai->m_tFightDesc.iStep)
		{
		case 0: 
		{
			if (ATTACK_TYPE::LIGHT == m_eAttackType && Input::LBtn())
			{
				eAnimID					= ANIM_CH::ATK_LIGHT_01;
				CAnimation* pAnimation	= m_pChai->Get_Model()->Get_Animation(eAnimID);
				dAnimSpeed				= CBeatManager::GetInstance()->Get_SPB(1) / (_double)pAnimation->Get_MaxFrameCount();

			}
			else if (ATTACK_TYPE::STRONG == m_eAttackType && Input::RBtn())
			{
				eAnimID					= ANIM_CH::ATK_SINGLE_01;
				CAnimation* pAnimation	= m_pChai->Get_Model()->Get_Animation(eAnimID);
				dAnimSpeed				= CBeatManager::GetInstance()->Get_SPB(1) / (_double)pAnimation->Get_MaxFrameCount();
			}
		}
			break;
		case 1:
		{
			if (ATTACK_TYPE::LIGHT == m_eAttackType && Input::LBtn())
			{
				eAnimID					= ANIM_CH::ATK_LIGHT_02;
				CAnimation* pAnimation	= m_pChai->Get_Model()->Get_Animation(eAnimID);
				dAnimSpeed				= CBeatManager::GetInstance()->Get_SPB(1) / (_double)pAnimation->Get_MaxFrameCount();
			}
			else if (ATTACK_TYPE::STRONG == m_eAttackType && Input::RBtn())
			{
				eAnimID					= ANIM_CH::ATK_SPC_2;
				CAnimation* pAnimation	= m_pChai->Get_Model()->Get_Animation(eAnimID);
				dAnimSpeed				= CBeatManager::GetInstance()->Get_SPB(1) / (_double)pAnimation->Get_MaxFrameCount();

				m_pChai->m_tFightDesc.iStep = -1;
			}
		}
			break;
		case 2:
		{
			if (ATTACK_TYPE::LIGHT == m_eAttackType && Input::LBtn())
			{
				eAnimID					= ANIM_CH::ATK_LIGHT_03;
				CAnimation* pAnimation	= m_pChai->Get_Model()->Get_Animation(eAnimID);
				dAnimSpeed				= CBeatManager::GetInstance()->Get_SPB(1) / (_double)pAnimation->Get_MaxFrameCount();

				m_pChai->m_tFightDesc.iStep = -1;
			}
		}
			break;
		default:
			break;
		}

		m_pChai->Get_Model()->Set_Animation(eAnimID, dAnimSpeed, 0.05f);
	}

	return m_strName;
}

const wstring& CState_Chai_Attack::LateTick()
{
	return Check_Transition();
}

void CState_Chai_Attack::Exit()
{
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
	if (!CBeatManager::GetInstance()->Is_HalfBeat())
		return m_strName;

	if (m_pChai->Get_Model()->Is_Tween())
		return m_strName;

	if(m_pChai->Get_Model()->Is_TwoThirds_Animation())
	{
		return StateNames[STATE_IDLE];
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
