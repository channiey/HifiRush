#include "Node.h"

CNode::CNode()
{

}

CNode::CNode(const CNode& rhs)
{
}

HRESULT CNode::Initialize_Node(class CBlackboard* pBlackboard)
{
	if (nullptr == pBlackboard)
		return E_FAIL;

	m_pBlackboard = pBlackboard;

	Safe_AddRef(m_pBlackboard);

	return S_OK;
}

void CNode::Free()
{
	for (auto iter : m_pChildNodes)
	{
		if (nullptr == iter) continue;

		iter->Free();
	}

	Safe_Release(m_pBlackboard);
}
