#include "..\Default\stdafx.h"

#include "GameInstance.h"
#include "Node_IsDamaged_Saber.h"


CNode_IsDamaged_Saber::CNode_IsDamaged_Saber()
{
}

CNode_IsDamaged_Saber::CNode_IsDamaged_Saber(const CNode_IsDamaged_Saber& rhs)
{
}

HRESULT CNode_IsDamaged_Saber::Initialize_Node(CBlackboard* pBlackboard)
{
	if (FAILED(__super::Initialize_Node(pBlackboard)))
		return E_FAIL;

	return S_OK;
}

const NODE_STATE CNode_IsDamaged_Saber::Evaluate(const _float& fTimeDelta)
{
	if (nullptr == m_pBlackboard_Saber->m_pSaber || nullptr == m_pBlackboard_Saber->m_pSaber->m_tFightDesc.pAttacker)
		return NODE_STATE::FAILURE;

	if (m_pBlackboard_Saber->m_pSaber->Get_FightDesc().bDamaged && !m_pBlackboard_Saber->m_pSaber->Get_Model()->Is_Contain_InTween(ANIM_SA::DMG_05))
	{
		/* 회전 및 넉백 */
		{
			Vec4 vAttackerDirInverse = m_pBlackboard_Saber->m_pSaber->m_tFightDesc.pAttacker->Get_Transform()->Get_Backward();

			m_pBlackboard_Saber->m_pSaber->Get_Transform()->Set_Look(vAttackerDirInverse);
		}

		/* 스탯 및 사망 처리*/
		{
			m_pBlackboard_Saber->m_pSaber->m_tStatDesc.fCurHp -= m_pBlackboard_Saber->m_pSaber->m_tFightDesc.pAttacker->Get_StatDesc().fAd;

			/* 죽었다면 */
			if (0 <= m_pBlackboard_Saber->m_pSaber->m_tStatDesc.fCurHp)
			{
				m_pBlackboard_Saber->m_pSaber->m_tStatDesc.fCurHp = 0.f;
				m_pBlackboard_Saber->m_pSaber->m_tStatDesc.bDead = TRUE;

				/* 애니메이션 재생  */
			}
			else
			{

				m_pBlackboard_Saber->m_pSaber->Get_Model()->Set_Animation(ANIM_SA::DMG_05, DF_PL_TIME, DF_TW_TIME);
			}
		} 

		m_pBlackboard_Saber->m_pSaber->m_tFightDesc.bDamaged = FALSE;
		return NODE_STATE::SUCCESS;
	}
	
	return NODE_STATE::FAILURE;
}

CNode_IsDamaged_Saber* CNode_IsDamaged_Saber::Create(CBlackboard* pBlackboard)
{
	CNode_IsDamaged_Saber* pInstance = new CNode_IsDamaged_Saber();

	if (FAILED(pInstance->Initialize_Node(pBlackboard)))
	{
		MSG_BOX("Failed to Created : CNode_IsDamaged_Saber");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNode_IsDamaged_Saber::Free()
{
	__super::Free();
}
