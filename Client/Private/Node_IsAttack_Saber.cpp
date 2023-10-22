#include "..\Default\stdafx.h"

#include "GameInstance.h"
#include "Node_IsAttack_Saber.h"


CNode_IsAttack_Saber::CNode_IsAttack_Saber()
{
}

CNode_IsAttack_Saber::CNode_IsAttack_Saber(const CNode_IsAttack_Saber& rhs)
{
}

HRESULT CNode_IsAttack_Saber::Initialize_Node(CBlackboard* pBlackboard)
{
	if (FAILED(__super::Initialize_Node(pBlackboard)))
		return E_FAIL;

	return S_OK;
}

const NODE_STATE CNode_IsAttack_Saber::Evaluate(const _float& fTimeDelta)
{
	/*if (!m_pBlackboard_Saber->m_pSaber->Get_Model()->Is_Contain_InTween(m_pBlackboard_Saber->m_eCurAttackAnim))
		m_pBlackboard_Saber->m_fAttackAccTime += fTimeDelta;*/

	if (m_pBlackboard_Saber->m_fAttackAccTime <= m_pBlackboard_Saber->m_fAttackFreqTime)
	{
		m_pBlackboard_Saber->m_fAttackAccTime = 0.f;
		Attack();
		return NODE_STATE::SUCCESS;
	}
	else
	{
		Escape();
		return NODE_STATE::RUNNING;
	}
}

const _bool CNode_IsAttack_Saber::Check_Condition(const _float& fTimeDelta)
{
	return _bool();
}

void CNode_IsAttack_Saber::Attack()
{
	/*m_pBlackboard_Saber->m_ePrevAttackAnim = m_pBlackboard_Saber->m_eCurAttackAnim;
	m_pBlackboard_Saber->m_eCurAttackAnim = Change_AttackAnim();

	if (!m_pBlackboard_Saber->m_pSaber->Get_Model()->Is_Contain_InTween(m_pBlackboard_Saber->m_eCurAttackAnim))
	{
		if (m_pBlackboard_Saber->m_pSaber->Get_Model()->Is_TwoThirds_Animation())
		{
			m_pBlackboard_Saber->m_pSaber->Get_Model()->Set_Animation(m_pBlackboard_Saber->m_eCurAttackAnim, DF_PL_TIME, DF_TW_TIME);
		}
	}*/
}

void CNode_IsAttack_Saber::Escape()
{
	//m_pBlackboard_Saber->m_ePrevEscapeAnim = m_pBlackboard_Saber->m_eCurEscapeAnim;
	//m_pBlackboard_Saber->m_eCurEscapeAnim = Change_EscapeAnim();

	///* Set Look */
	//{
	//	Vec4 vDir = Vec4(m_pBlackboard_Saber->m_pSaber->m_tFightDesc.pTarget->Get_Transform()->Get_FinalPosition()
	//		- m_pBlackboard_Saber->m_pSaber->Get_Transform()->Get_FinalPosition()).Normalized();

	//	m_pBlackboard_Saber->m_pSaber->Get_Transform()->Set_Look(vDir);
	//}

	///* Translate */
	//if (!m_pBlackboard_Saber->m_pSaber->Get_Model()->Is_Contain_InTween(m_pBlackboard_Saber->m_eCurEscapeAnim))
	//{
	//	if (m_pBlackboard_Saber->m_pSaber->Get_Model()->Is_TwoThirds_Animation())
	//	{
	//		m_pBlackboard_Saber->m_pSaber->Get_Model()->Set_Animation(m_pBlackboard_Saber->m_eCurEscapeAnim, DF_PL_TIME, DF_TW_TIME);
	//	}
	//}
}

const ANIM_SA CNode_IsAttack_Saber::Change_AttackAnim()
{
	return ANIM_SA::ATTACK_JUMP_CUT;
}

const ANIM_SA CNode_IsAttack_Saber::Change_EscapeAnim()
{
	return ANIM_SA::DOUBLE_STEP_LEFT_00;
}

CNode_IsAttack_Saber* CNode_IsAttack_Saber::Create(CBlackboard* pBlackboard)
{
	CNode_IsAttack_Saber* pInstance = new CNode_IsAttack_Saber();

	if (FAILED(pInstance->Initialize_Node(pBlackboard)))
	{
		MSG_BOX("Failed to Created : CNode_IsAttack_Saber");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNode_IsAttack_Saber::Free()
{
	__super::Free();
}
