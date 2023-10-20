#include "..\Default\stdafx.h"

#include "GameInstance.h"
#include "Node_Tracked_Saber.h"


CNode_Tracked_Saber::CNode_Tracked_Saber()
{
}

CNode_Tracked_Saber::CNode_Tracked_Saber(const CNode_Tracked_Saber& rhs)
{
}

HRESULT CNode_Tracked_Saber::Initialize_Node(CBlackboard* pBlackboard)
{
	if (FAILED(__super::Initialize_Node(pBlackboard)))
		return E_FAIL;

	return S_OK;
}

const NODE_STATE CNode_Tracked_Saber::Evaluate(const _float& fTimeDelta)
{
	/* 타겟을 언제 어디서 어떻게 설정하지 ? */

	if (nullptr != m_pBlackboard_Saber->m_pSaber && nullptr != m_pBlackboard_Saber->m_pTarget)
	{
		_float fDistance = Vec4::Distance(m_pBlackboard_Saber->m_pTarget->Get_Transform()->Get_FinalPosition(),
											m_pBlackboard_Saber->m_pSaber->Get_Transform()->Get_FinalPosition());
	
		if (m_pBlackboard_Saber->m_fCanBattleDist >= fDistance)
		{

			return NODE_STATE::SUCCESS;
		}
	}
		
	m_pBlackboard_Saber->m_pSaber->Get_Model()->Set_Animation(ANIM_SA::IDLE_ATTACK, TRUE, DF_PL_TIME, DF_TW_TIME);
	
	return NODE_STATE::FAILURE;
}

CNode_Tracked_Saber* CNode_Tracked_Saber::Create(CBlackboard* pBlackboard)
{
	CNode_Tracked_Saber* pInstance = new CNode_Tracked_Saber();

	if (FAILED(pInstance->Initialize_Node(pBlackboard)))
	{
		MSG_BOX("Failed to Created : CNode_Tracked_Saber");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNode_Tracked_Saber::Free()
{
	__super::Free();
}
