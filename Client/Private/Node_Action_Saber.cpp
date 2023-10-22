#include "..\Default\stdafx.h"

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

void CNode_Action_Saber::Free()
{
	__super::Free();
	
	Safe_Release(m_pBlackboard_Saber);
}
