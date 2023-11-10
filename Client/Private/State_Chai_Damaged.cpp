#include "..\Default\stdafx.h"
#include "State_Chai_Damaged.h"

#include "Enemy.h"

CState_Chai_Damaged::CState_Chai_Damaged()
{
}

CState_Chai_Damaged::CState_Chai_Damaged(const CState_Chai_Damaged& rhs)
{
}

HRESULT CState_Chai_Damaged::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Chai_Damaged::Enter()
{
	cout << " Chai Damaged\n";
	CEnemy* pEnemy = dynamic_cast<CEnemy*>(m_pChai->m_tFightDesc.pAttacker);

	if (nullptr == pEnemy)
		return E_FAIL;

	CEnemy::ENEMY_TYPE eEnemyType = pEnemy->Get_EnemyType();


	ANIM_CH		eAnimID = ANIM_CH_END;
	CAnimation* pAnimation = nullptr;
	_double		fTimePerFrame = 0.f;

	switch (eEnemyType)
	{
	case CEnemy::ENEMY_TYPE::NORMAL: 
	{
		eAnimID = ANIM_CH::DMG_LIGHT;
		pAnimation = m_pChai->Get_Model()->Get_Animation(eAnimID);
		fTimePerFrame = 1 / pAnimation->Get_TickPerSecond() * 0.5f;

		ENGINE_INSTANCE->Play_Sound(EFC_ENEMY_DAMAGED_00, PLAYER_CHAI, 0.4f);

	}
		break;
	case CEnemy::ENEMY_TYPE::FLYING:
	{

	}
		break;
	case CEnemy::ENEMY_TYPE::BOSS:
	{
		if (0 == rand() % 2)
		{
			eAnimID = ANIM_CH::DMG_LIGHT;
			pAnimation = m_pChai->Get_Model()->Get_Animation(eAnimID);
			fTimePerFrame = 1 / pAnimation->Get_TickPerSecond();
		}
		else
		{
			eAnimID = ANIM_CH::DMG_HEAVY;
			pAnimation = m_pChai->Get_Model()->Get_Animation(eAnimID);
			fTimePerFrame = 1 / pAnimation->Get_TickPerSecond();
		}
	}
		break;
	default:
		break;
	}

	m_pChai->Get_Model()->Set_Animation(eAnimID, fTimePerFrame, DF_TW_TIME);

	return S_OK;
}

const wstring CState_Chai_Damaged::Tick(const _double& fTimeDelta)
{
	return Check_Transition();
}

const wstring CState_Chai_Damaged::LateTick()
{
	return Check_Transition();
}

void CState_Chai_Damaged::Exit()
{
	m_pChai->m_tFightDesc.bDamaged = FALSE;
	m_pChai->m_tFightDesc.pAttacker = nullptr;
}

const wstring CState_Chai_Damaged::Check_Transition()
{
	if (m_pModel->Is_Tween())
		return m_strName;

	if(m_pModel->Is_ThreeFourths_Animation())
		return StateNames_CH[STATE_IDLE_CH];

	return m_strName;
}

CState_Chai_Damaged* CState_Chai_Damaged::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Chai_Damaged* pInstance = new CState_Chai_Damaged();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Chai_Damaged");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Chai_Damaged::Clone(void* pArg)
{
	return nullptr;
}

void CState_Chai_Damaged::Free()
{
}
