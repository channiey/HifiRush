#include "Node_Root.h"

CNode_Root::CNode_Root()
{
	m_eType = NODE_TYPE::ROOT;
}

CNode_Root::CNode_Root(const CNode_Root& rhs)
{

}


const NODE_STATE CNode_Root::Evaluate(const _float& fTimeDelta)
{
	if (Is_ChildNode())
		return NODE_STATE::FAILURE; /* TODO �̿� ���� ó�� �߰��� �ʿ� */

	return m_pChildNodes.front()->Evaluate(fTimeDelta);
}

HRESULT CNode_Root::Add_ChildNode(CNode* pChildNode)
{
	/* ��Ʈ ���� �� �ϳ��� �ڽ� ��常�� ���´�. */

	if (nullptr == pChildNode || Is_ChildNode())
		return E_FAIL;
		
	m_pChildNodes.push_back(pChildNode);

	return S_OK;
}

CNode* CNode_Root::Clone(void* pArg)
{
	return nullptr;
}

void CNode_Root::Free()
{
}

