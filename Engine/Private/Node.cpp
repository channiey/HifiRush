#include "Node.h"

CNode::CNode()
{

}

CNode::CNode(const CNode& rhs)
{
}

void CNode::Free()
{
	for (auto iter : m_pChildNodes)
	{
		if (nullptr == iter) continue;

		iter->Free();
	}
}
