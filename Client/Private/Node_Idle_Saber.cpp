#include "..\Default\stdafx.h"

#include "GameInstance.h"
#include "Node_Idle_Saber.h"

CNode_Idle_Saber::CNode_Idle_Saber()
{
}

CNode_Idle_Saber::CNode_Idle_Saber(const CNode_Idle_Saber& rhs)
{
}

HRESULT CNode_Idle_Saber::Initialize_Node(CBlackboard* pBlackboard)
{
	if (FAILED(__super::Initialize_Node(pBlackboard)))
		return E_FAIL;

	m_eCurAnim = ANIM_SA::IDLE_ATTACK;

	return S_OK;
}
const NODE_STATE CNode_Idle_Saber::Evaluate(const _float& fTimeDelta)
{
	if (Check_Condition(fTimeDelta))
	{
		return NODE_STATE::SUCCESS;
	}
	else
	{
		Wait(fTimeDelta);
		return NODE_STATE::FAILURE;
	}
}


const _bool CNode_Idle_Saber::Check_Condition(const _float& fTimeDelta)
{
	if (m_pBlackboard_Saber->m_bResetIdle)
		return TRUE;
	else
	{
		m_pBlackboard_Saber->m_fResetAccTime += fTimeDelta;

		if (m_pBlackboard_Saber->m_fResetTime <= m_pBlackboard_Saber->m_fResetAccTime)
		{
			m_pBlackboard_Saber->m_bResetIdle = TRUE;
		}
	}

	return m_pBlackboard_Saber->m_bResetIdle;
}

void CNode_Idle_Saber::Wait(const _float& fTimeDelta)
{
	/* Set Look */
	Set_LookAtTarget(fTimeDelta);

	/* Play Animation */
	if (!m_pBlackboard_Saber->m_pSaber->Get_Model()->Is_Contain_InTween(m_eCurAnim))
		m_pBlackboard_Saber->m_pSaber->Get_Model()->Set_Animation(m_eCurAnim);
}

CNode_Idle_Saber* CNode_Idle_Saber::Create(CBlackboard* pBlackboard)
{
	CNode_Idle_Saber* pInstance = new CNode_Idle_Saber();

	if (FAILED(pInstance->Initialize_Node(pBlackboard)))
	{
		MSG_BOX("Failed to Created : CNode_Idle_Saber");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNode_Idle_Saber::Free()
{
	__super::Free();
}
