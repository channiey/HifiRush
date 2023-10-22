#include "..\Default\stdafx.h"

#include "GameInstance.h"
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
	/* ���� �ִϸ��̼����� �����(2/3����)�̰ų�, ���� �ִϸ��̼����� ���� �Ǿ����� ��� TRUE */

	return m_pBlackboard_Saber->m_pSaber->Get_Model()->Is_Playing_Animation(m_eCurAnim);

}

void CNode_Action_Saber::Free()
{
	__super::Free();
	
	Safe_Release(m_pBlackboard_Saber);
}
