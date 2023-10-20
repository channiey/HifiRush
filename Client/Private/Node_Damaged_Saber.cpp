#include "..\Default\stdafx.h"

#include "GameInstance.h"
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
	if (nullptr == m_pBlackboard_Saber->m_pSaber)
		return NODE_STATE::FAILURE;

	if (m_pBlackboard_Saber->m_pSaber->Get_FightDesc().bDamaged)
	{
		/* 체력 깍기도 필요 */

		/* 사망 처리 */

		/* 맞은 방향에 따른 회전, 넉백 */

		m_pBlackboard_Saber->m_pSaber->Get_Model()->Set_Animation(ANIM_SA::DMG_05, FALSE, DF_PL_TIME, DF_TW_TIME);

		m_pBlackboard_Saber->m_pSaber->m_tFightDesc.bDamaged = FALSE;
		
		return NODE_STATE::SUCCESS;
	}
	
	return NODE_STATE::FAILURE;
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
