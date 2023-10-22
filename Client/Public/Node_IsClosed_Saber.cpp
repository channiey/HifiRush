//#include "..\Default\stdafx.h"
//
//#include "GameInstance.h"
//#include "Node_IsClosed_Saber.h"
//
//
//CNode_IsClosed_Saber::CNode_IsClosed_Saber()
//{
//}
//
//CNode_IsClosed_Saber::CNode_IsClosed_Saber(const CNode_IsClosed_Saber& rhs)
//{
//}
//
//HRESULT CNode_IsClosed_Saber::Initialize_Node(CBlackboard* pBlackboard)
//{
//	if (FAILED(__super::Initialize_Node(pBlackboard)))
//		return E_FAIL;
//
//	return S_OK;
//}
//
//const NODE_STATE CNode_IsClosed_Saber::Evaluate(const _float& fTimeDelta)
//{
//	/*if (m_pBlackboard_Saber->m_fCurTargetDist <= m_pBlackboard_Saber->m_fCanAttackDist)
//	{
//		return NODE_STATE::SUCCESS;
//	}
//	else if (m_pBlackboard_Saber->m_fCanAttackDist < m_pBlackboard_Saber->m_fCurTargetDist)
//	{
//		Move();
//
//		return NODE_STATE::RUNNING;
//	}*/
//	return NODE_STATE::FAILURE;
//}
//
//const _bool CNode_IsClosed_Saber::Check_Condition(const _float& fTimeDelta)
//{
//	return _bool();
//}
//
//void CNode_IsClosed_Saber::Move()
//{
//	/* Set Look */
//	{
//		Vec4 vDir = Vec4(m_pBlackboard_Saber->m_pSaber->m_tFightDesc.pTarget->Get_Transform()->Get_FinalPosition()
//			- m_pBlackboard_Saber->m_pSaber->Get_Transform()->Get_FinalPosition()).Normalized();
//
//		m_pBlackboard_Saber->m_pSaber->Get_Transform()->Set_Look(vDir);
//	}		
//
//	/* Translate */
//	if (!m_pBlackboard_Saber->m_pSaber->Get_Model()->Is_Contain_InTween(ANIM_SA::DOUBLE_STEP_FRONT_00))
//	{
//		if (m_pBlackboard_Saber->m_pSaber->Get_Model()->Is_TwoThirds_Animation())
//		{
//			m_pBlackboard_Saber->m_pSaber->Get_Model()->Set_Animation(ANIM_SA::DOUBLE_STEP_FRONT_00, DF_PL_TIME, DF_TW_TIME);
//		}
//	}
//}
//
//CNode_IsClosed_Saber* CNode_IsClosed_Saber::Create(CBlackboard* pBlackboard)
//{
//	CNode_IsClosed_Saber* pInstance = new CNode_IsClosed_Saber();
//
//	if (FAILED(pInstance->Initialize_Node(pBlackboard)))
//	{
//		MSG_BOX("Failed to Created : CNode_IsClosed_Saber");
//		Safe_Release(pInstance);
//	}
//
//	return pInstance;
//}
//
//void CNode_IsClosed_Saber::Free()
//{
//	__super::Free();
//}
