#include "Node_Action.h"

CNode_Action::CNode_Action()
{
}

CNode_Action::CNode_Action(const CNode_Action& rhs)
{
}

HRESULT CNode_Action::Initialize_Node()
{
	if (FAILED(__super::Initialize_Node()))
		return E_FAIL;

	m_eType = NODE_TYPE::ACTION;

	return S_OK;
}

HRESULT CNode_Action::Reset_Node()
{
	__super::Reset_Node();

	return S_OK;
}

void CNode_Action::Free()
{
	__super::Free();
}
