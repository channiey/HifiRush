#include "..\Default\stdafx.h"

#include "EngineInstance.h"
#include "Node_Damaged_Saber.h"

CNode_Damaged_Saber::CNode_Damaged_Saber()
{
}

CNode_Damaged_Saber::CNode_Damaged_Saber(const CNode_Damaged_Saber& rhs)
{
}

HRESULT CNode_Damaged_Saber::Initialize_Node(CBlackboard* pBlackboard)
{
	if (FAILED(__super::Initialize_Node(pBlackboard)))
		return E_FAIL;

	return S_OK;
}
const NODE_STATE CNode_Damaged_Saber::Evaluate(const _float& fTimeDelta)
{
	if (nullptr == m_pBlackboard_Saber->m_pSaber->m_tFightDesc.pAttacker)
		return NODE_STATE::FAILURE;

	if (Check_Condition(fTimeDelta))
	{
		Damaged();
		return NODE_STATE::SUCCESS;
	}
	else
	{
		if (Is_Playing_Animation())
		{
			return NODE_STATE::RUNNING;
		}
		else
		{
			m_pBlackboard_Saber->m_pSaber->m_tFightDesc.pAttacker = nullptr;
			return NODE_STATE::FAILURE;
		}
	}
}


const _bool CNode_Damaged_Saber::Check_Condition(const _float& fTimeDelta)
{
	if (m_pBlackboard_Saber->m_pSaber->Get_FightDesc().bDamaged)
		return TRUE;

	return FALSE;
}

void CNode_Damaged_Saber::Damaged()
{
	/* 룩 변경 */
	{
		Vec4 vAttackerDirInverse = m_pBlackboard_Saber->m_pSaber->m_tFightDesc.pAttacker->Get_Transform()->Get_Backward();

		m_pBlackboard_Saber->m_pSaber->Get_Transform()->Set_Look(vAttackerDirInverse);
	}

	/* 스탯 및 사망 처리*/
	{
		m_pBlackboard_Saber->m_pSaber->m_tStatDesc.fCurHp -= m_pBlackboard_Saber->m_pSaber->m_tFightDesc.pAttacker->Get_StatDesc().fAd;

		/* 죽었다면 */
		//if (0 >= m_pBlackboard_Saber->m_pSaber->m_tStatDesc.fCurHp)
		//{
		//	m_pBlackboard_Saber->m_pSaber->m_tStatDesc.fCurHp = 0.f;
		//	m_pBlackboard_Saber->m_pSaber->m_tStatDesc.bDead = TRUE;

		//	/* TODO:: 애니메이션 재생 후 고정 필요 */
		//}
		//else
		{
			m_eCurAnim = ANIM_SA::DMG_05;
			m_pBlackboard_Saber->m_pSaber->Get_Model()->Set_Animation(m_eCurAnim);
		}
	}


	m_pBlackboard_Saber->m_fResetTime = 0.5f;
	m_pBlackboard_Saber->m_fAttackAccTime = 0.5f;

	m_pBlackboard_Saber->m_pSaber->m_tFightDesc.bDamaged = FALSE;
}

CNode_Damaged_Saber* CNode_Damaged_Saber::Create(CBlackboard* pBlackboard)
{
	CNode_Damaged_Saber* pInstance = new CNode_Damaged_Saber();

	if (FAILED(pInstance->Initialize_Node(pBlackboard)))
	{
		MSG_BOX("Failed to Created : CNode_Damaged_Saber");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNode_Damaged_Saber::Free()
{
	__super::Free();
}
