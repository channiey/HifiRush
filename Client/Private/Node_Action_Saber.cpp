#include "..\Default\stdafx.h"

#include "EngineInstance.h"
#include "Node_Action_Saber.h"


CNode_Action_Saber::CNode_Action_Saber()
{
}

CNode_Action_Saber::CNode_Action_Saber(const CNode_Action_Saber& rhs)
{
}

HRESULT CNode_Action_Saber::Initialize_Node(CBlackboard* pBlackboard)
{
	if(FAILED(__super::Initialize_Node()))
		return E_FAIL;

	m_pBlackboard_Saber = dynamic_cast<CBlackboard_Saber*>(pBlackboard);

	if (nullptr == m_pBlackboard_Saber)
		return E_FAIL;

	return S_OK;
}

const _bool CNode_Action_Saber::Is_Playing_Animation()
{
	/* 현재 애니메이션으로 재생중(2/3까지)이거나, 다음 애니메이션으로 예약 되어있을 경우 TRUE */

	return m_pBlackboard_Saber->m_pSaber->Get_Model()->Is_Playing_Animation(m_eCurAnim);

}

void CNode_Action_Saber::Set_LookAtTarget(const _double& fTimeDelta)
{
	/* Set Look */
	if (!m_pBlackboard_Saber->m_pSaber->Get_Model()->Is_Tween())
	{
		Vec4 vRotDir, vLook, vTargetDir;

		vLook = m_pBlackboard_Saber->m_pSaber->Get_Transform()->Get_Forward();
		vTargetDir = Vec4(m_pBlackboard_Saber->m_pSaber->m_tFightDesc.pTarget->Get_Transform()->Get_FinalPosition()
			- m_pBlackboard_Saber->m_pSaber->Get_Transform()->Get_FinalPosition()).Normalized();

		if (3.f < acos(vTargetDir.Dot((vLook))))
			vRotDir = Vec4::Lerp(vLook, vTargetDir, 0.66f);
		else
			vRotDir = Vec4::Lerp(vLook, vTargetDir, fTimeDelta * 20.f);

		m_pBlackboard_Saber->m_pSaber->Get_Transform()->Set_Look(vRotDir);
	}
}

void CNode_Action_Saber::Free()
{
	__super::Free();
	
	Safe_Release(m_pBlackboard_Saber);
}
