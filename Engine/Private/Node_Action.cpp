#include "Node_Action.h"

CNode_Action::CNode_Action()
{
}

CNode_Action::CNode_Action(const CNode_Action& rhs)
{
}

HRESULT CNode_Action::Initialize_Node(class CBlackboard* pBlackboard)
{
	if (FAILED(__super::Initialize_Node(pBlackboard)))
		return E_FAIL;

	m_eType = NODE_TYPE::ACTION;

	return S_OK;
}

void CNode_Action::Free()
{
	__super::Free();
}
