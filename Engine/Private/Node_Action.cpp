#include "Node_Action.h"

CNode_Action::CNode_Action()
{
	m_eType = NODE_TYPE::ACTION;
}

CNode_Action::CNode_Action(const CNode_Action& rhs)
{
}


CNode* CNode_Action::Clone(void* pArg)
{
	return nullptr;
}

void CNode_Action::Free()
{
}
