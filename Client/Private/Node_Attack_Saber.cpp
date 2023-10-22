#include "..\Default\stdafx.h"

#include "GameInstance.h"
#include "Node_Attack_Saber.h"


CNode_Attack_Saber::CNode_Attack_Saber()
{
}

CNode_Attack_Saber::CNode_Attack_Saber(const CNode_Attack_Saber& rhs)
{
}

HRESULT CNode_Attack_Saber::Initialize_Node(CBlackboard* pBlackboard)
{
	if (FAILED(__super::Initialize_Node(pBlackboard)))
		return E_FAIL;

	return S_OK;
}

const NODE_STATE CNode_Attack_Saber::Evaluate(const _float& fTimeDelta)
{
	if (nullptr == m_pBlackboard_Saber->m_pSaber->m_tFightDesc.pTarget)
		return NODE_STATE::FAILURE;

	CModel* pModel = m_pBlackboard_Saber->m_pSaber->Get_Model();
	
	if (m_eCurAnim == pModel->Get_CurAnimationIndex() && pModel->Is_TwoThirds_Animation())
	{
		/* 임시 조치 */
		m_pBlackboard_Saber->m_pSaber->Get_Model()->Set_Animation(ANIM_SA::IDLE_ATTACK, DF_PL_TIME, DF_TW_TIME);
		return NODE_STATE::SUCCESS;
	}
	else if (Is_Playing_Animation())
	{
		return NODE_STATE::RUNNING;
	}

	Attack();
	return NODE_STATE::RUNNING;
}

const _bool CNode_Attack_Saber::Check_Condition(const _float& fTimeDelta)
{

	return TRUE;
}

void CNode_Attack_Saber::Attack()
{
	/* Set Look */
	{
		Vec4 vDir = Vec4(m_pBlackboard_Saber->m_pSaber->m_tFightDesc.pTarget->Get_Transform()->Get_FinalPosition()
							- m_pBlackboard_Saber->m_pSaber->Get_Transform()->Get_FinalPosition()).Normalized();

		m_pBlackboard_Saber->m_pSaber->Get_Transform()->Set_Look(vDir);
	}

	/* Play Animation */
	m_eCurAnim = ANIM_SA::ATTACK_JUMP_CUT;

	if (!m_pBlackboard_Saber->m_pSaber->Get_Model()->Is_Contain_InTween(m_eCurAnim))
		m_pBlackboard_Saber->m_pSaber->Get_Model()->Set_Animation(m_eCurAnim, DF_PL_TIME, DF_TW_TIME);
}

CNode_Attack_Saber* CNode_Attack_Saber::Create(CBlackboard* pBlackboard)
{
	CNode_Attack_Saber* pInstance = new CNode_Attack_Saber();

	if (FAILED(pInstance->Initialize_Node(pBlackboard)))
	{
		MSG_BOX("Failed to Created : CNode_Attack_Saber");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNode_Attack_Saber::Free()
{
	__super::Free();
}
