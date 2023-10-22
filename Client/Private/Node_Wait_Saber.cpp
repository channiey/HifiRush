#include "..\Default\stdafx.h"

#include "GameInstance.h"
#include "Node_Wait_Saber.h"


CNode_Wait_Saber::CNode_Wait_Saber()
{
}

CNode_Wait_Saber::CNode_Wait_Saber(const CNode_Wait_Saber& rhs)
{
}

HRESULT CNode_Wait_Saber::Initialize_Node(CBlackboard* pBlackboard)
{
	if (FAILED(__super::Initialize_Node(pBlackboard)))
		return E_FAIL;

	return S_OK;
}

const NODE_STATE CNode_Wait_Saber::Evaluate(const _float& fTimeDelta)
{
	if (nullptr == m_pBlackboard_Saber->m_pSaber->m_tFightDesc.pTarget)
		return NODE_STATE::FAILURE;

	if (Check_Condition(fTimeDelta))
	{
		return NODE_STATE::SUCCESS;
	}
	else
	{
		Wait();	
		return NODE_STATE::FAILURE;
	}
}

const _bool CNode_Wait_Saber::Check_Condition(const _float& fTimeDelta)
{
	m_pBlackboard_Saber->m_fAttackAccTime += fTimeDelta;

	if (m_pBlackboard_Saber->m_fAttackFreqTime <= m_pBlackboard_Saber->m_fAttackAccTime)
	{
		m_pBlackboard_Saber->m_fAttackAccTime = 0.f;
		return TRUE;
	}

	return FALSE;
}


void CNode_Wait_Saber::Wait()
{
	/* Set Look */
	if(!m_pBlackboard_Saber->m_pSaber->Get_Model()->Is_Tween())
	{
		Vec4 vDir = Vec4(m_pBlackboard_Saber->m_pSaber->m_tFightDesc.pTarget->Get_Transform()->Get_FinalPosition()
			- m_pBlackboard_Saber->m_pSaber->Get_Transform()->Get_FinalPosition()).Normalized();

		m_pBlackboard_Saber->m_pSaber->Get_Transform()->Set_Look(vDir);
	}

	m_eCurAnim = ANIM_SA::IDLE_ATTACK;

	if (!m_pBlackboard_Saber->m_pSaber->Get_Model()->Is_Contain_InTween(m_eCurAnim))
		m_pBlackboard_Saber->m_pSaber->Get_Model()->Set_Animation(m_eCurAnim, DF_PL_TIME, DF_TW_TIME);
}

CNode_Wait_Saber* CNode_Wait_Saber::Create(CBlackboard* pBlackboard)
{
	CNode_Wait_Saber* pInstance = new CNode_Wait_Saber();

	if (FAILED(pInstance->Initialize_Node(pBlackboard)))
	{
		MSG_BOX("Failed to Created : CNode_Wait_Saber");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNode_Wait_Saber::Free()
{
	__super::Free();
}
