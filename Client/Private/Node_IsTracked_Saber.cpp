#include "..\Default\stdafx.h"

#include "GameInstance.h"
#include "Node_IsTracked_Saber.h"


CNode_IsTracked_Saber::CNode_IsTracked_Saber()
{
}

CNode_IsTracked_Saber::CNode_IsTracked_Saber(const CNode_IsTracked_Saber& rhs)
{
}

HRESULT CNode_IsTracked_Saber::Initialize_Node(CBlackboard* pBlackboard)
{
	if (FAILED(__super::Initialize_Node(pBlackboard)))
		return E_FAIL;

	return S_OK;
}

const NODE_STATE CNode_IsTracked_Saber::Evaluate(const _float& fTimeDelta)
{
	if (nullptr == m_pBlackboard_Saber->m_pSaber || nullptr == m_pBlackboard_Saber->m_pSaber->m_tFightDesc.pTarget)
		return NODE_STATE::FAILURE;

	if (nullptr != m_pBlackboard_Saber->m_pSaber && nullptr != m_pBlackboard_Saber->m_pSaber->m_tFightDesc.pTarget)
	{
		m_pBlackboard_Saber->m_fCurTargetDist = Vec4::Distance(m_pBlackboard_Saber->m_pSaber->m_tFightDesc.pTarget->Get_Transform()->Get_FinalPosition(),
											m_pBlackboard_Saber->m_pSaber->Get_Transform()->Get_FinalPosition());
	
		if (m_pBlackboard_Saber->m_fCurTargetDist <= m_pBlackboard_Saber->m_fCanBattleDist)
		{
			return NODE_STATE::SUCCESS;
		}
	}
		
	if (!m_pBlackboard_Saber->m_pSaber->Get_Model()->Is_Contain_InTween(ANIM_SA::IDLE_ATTACK))
	{
		if (m_pBlackboard_Saber->m_pSaber->Get_Model()->Is_TwoThirds_Animation())
		{
			m_pBlackboard_Saber->m_pSaber->Get_Model()->Set_Animation(ANIM_SA::IDLE_ATTACK, DF_PL_TIME, DF_TW_TIME);
			return NODE_STATE::FAILURE;
		}
	}

	return NODE_STATE::FAILURE;
}

CNode_IsTracked_Saber* CNode_IsTracked_Saber::Create(CBlackboard* pBlackboard)
{
	CNode_IsTracked_Saber* pInstance = new CNode_IsTracked_Saber();

	if (FAILED(pInstance->Initialize_Node(pBlackboard)))
	{
		MSG_BOX("Failed to Created : CNode_IsTracked_Saber");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNode_IsTracked_Saber::Free()
{
	__super::Free();
}
