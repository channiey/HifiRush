#include "..\Default\stdafx.h"

#include "GameInstance.h"
#include "Node_Move_Saber.h"


CNode_Move_Saber::CNode_Move_Saber()
{
}

CNode_Move_Saber::CNode_Move_Saber(const CNode_Move_Saber& rhs)
{
}

HRESULT CNode_Move_Saber::Initialize_Node(CBlackboard* pBlackboard)
{
	if (FAILED(__super::Initialize_Node(pBlackboard)))
		return E_FAIL;

	return S_OK;
}

const NODE_STATE CNode_Move_Saber::Evaluate(const _float& fTimeDelta)
{
	if (nullptr == m_pBlackboard_Saber->m_pSaber->m_tFightDesc.pTarget)
		return NODE_STATE::FAILURE;

	if (Check_Condition(fTimeDelta))
	{
		if (Is_ReachTarget())
		{
			// << : 임시 코드 
			if (!m_pBlackboard_Saber->m_pSaber->Get_Model()->Is_Contain_InTween(ANIM_SA::IDLE_ATTACK))
				m_pBlackboard_Saber->m_pSaber->Get_Model()->Set_Animation(ANIM_SA::IDLE_ATTACK, DF_PL_TIME, DF_TW_TIME);
			// >> : 
			return NODE_STATE::SUCCESS;
		}
		else
		{
			Move_ToTarget();
			return NODE_STATE::RUNNING;
		}

	}
	else /* 지금은 여기 들어올 일 없음 */
	{
		Hover_Target();
		return NODE_STATE::FAILURE;
	}
}

const _bool CNode_Move_Saber::Check_Condition(const _float& fTimeDelta)
{
	return TRUE;
}

const _bool CNode_Move_Saber::Is_ReachTarget()
{
	const _float fCurDist = Vec4::Distance(m_pBlackboard_Saber->m_pSaber->m_tFightDesc.pTarget->Get_Transform()->Get_FinalPosition(),
											m_pBlackboard_Saber->m_pSaber->Get_Transform()->Get_FinalPosition());

	if (fCurDist <= m_pBlackboard_Saber->m_fCanAttackDist)
		return TRUE;

	return FALSE;
}

void CNode_Move_Saber::Move_ToTarget()
{
	/* Set Look */
	{
		Vec4 vDir = Vec4(m_pBlackboard_Saber->m_pSaber->m_tFightDesc.pTarget->Get_Transform()->Get_FinalPosition()
						- m_pBlackboard_Saber->m_pSaber->Get_Transform()->Get_FinalPosition()).Normalized();

		m_pBlackboard_Saber->m_pSaber->Get_Transform()->Set_Look(vDir);
	}

	/* Translate */
	if (!m_pBlackboard_Saber->m_pSaber->Get_Model()->Is_Contain_InTween(ANIM_SA::DOUBLE_STEP_FRONT_00))
		m_pBlackboard_Saber->m_pSaber->Get_Model()->Set_Animation(ANIM_SA::DOUBLE_STEP_FRONT_00, DF_PL_TIME, DF_TW_TIME);
}


void CNode_Move_Saber::Hover_Target() 
{
	/* Set Look */
	{
		Vec4 vDir = Vec4(m_pBlackboard_Saber->m_pSaber->m_tFightDesc.pTarget->Get_Transform()->Get_FinalPosition()
			- m_pBlackboard_Saber->m_pSaber->Get_Transform()->Get_FinalPosition()).Normalized();

		m_pBlackboard_Saber->m_pSaber->Get_Transform()->Set_Look(vDir);
	}

	/* Translate */
	if (!m_pBlackboard_Saber->m_pSaber->Get_Model()->Is_Contain_InTween(ANIM_SA::DOUBLE_STEP_RIGHT_BACK_00))// && m_pBlackboard_Saber->m_pSaber->Get_Model()->Is_TwoThirds_Animation())
		m_pBlackboard_Saber->m_pSaber->Get_Model()->Set_Animation(ANIM_SA::DOUBLE_STEP_RIGHT_BACK_00, DF_PL_TIME, DF_TW_TIME);
}


CNode_Move_Saber* CNode_Move_Saber::Create(CBlackboard* pBlackboard)
{
	CNode_Move_Saber* pInstance = new CNode_Move_Saber();

	if (FAILED(pInstance->Initialize_Node(pBlackboard)))
	{
		MSG_BOX("Failed to Created : CNode_Move_Saber");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNode_Move_Saber::Free()
{
	__super::Free();
}
