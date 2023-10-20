#include "Node.h"

CNode::CNode()
{

}

CNode::CNode(const CNode& rhs)
{
}

HRESULT CNode::Initialize_Node()
{
	return S_OK;
}

HRESULT CNode::Reset_Node()
{
	m_eState = NODE_STATE::TYPEEND;

	return S_OK;
}

void CNode::Free()
{
	for (auto iter : m_ChildNodes)
	{
		if (nullptr == iter) continue;

		iter->Free();
	}
}
