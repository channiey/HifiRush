#include "..\Default\stdafx.h"

#include "EngineInstance.h"
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

	m_eCurAnim = ANIM_SA::ATTACK_JUMP_CUT;

	return S_OK;
}

const NODE_STATE CNode_Attack_Saber::Evaluate(const _double& fTimeDelta)
{
	if (nullptr == m_pBlackboard_Saber->m_pSaber->m_tFightDesc.pTarget)
		return NODE_STATE::FAILURE;

	CModel* pModel = m_pBlackboard_Saber->m_pSaber->Get_Model();
	
	if (m_eCurAnim == pModel->Get_CurAnimationIndex() && pModel->Is_Finish_Animation())
	{
		Reset_BT();
		return NODE_STATE::SUCCESS;
	}
	else if (Is_Playing_Animation())
	{
		return NODE_STATE::RUNNING;
	}

	Attack();
	return NODE_STATE::RUNNING;
}

const _bool CNode_Attack_Saber::Check_Condition(const _double& fTimeDelta)
{

	return TRUE;
}

void CNode_Attack_Saber::Attack()
{
	/* Set Look */
	/*{
		Vec4 vDir = Vec4(m_pBlackboard_Saber->m_pSaber->m_tFightDesc.pTarget->Get_Transform()->Get_FinalPosition()
							- m_pBlackboard_Saber->m_pSaber->Get_Transform()->Get_FinalPosition()).Normalized();

		m_pBlackboard_Saber->m_pSaber->Get_Transform()->Set_Look(vDir);
	}*/

	/* Play Animation */
	if (!m_pBlackboard_Saber->m_pSaber->Get_Model()->Is_Contain_InTween(m_eCurAnim))
		m_pBlackboard_Saber->m_pSaber->Get_Model()->Set_Animation(m_eCurAnim);
}

void CNode_Attack_Saber::Reset_BT()
{
	m_pBlackboard_Saber->m_bResetIdle = FALSE;
	m_pBlackboard_Saber->m_fResetAccTime = 0.f;
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
