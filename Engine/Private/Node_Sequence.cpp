#include "Node_Sequence.h"

CNode_Sequence::CNode_Sequence()
{
	m_eType = NODE_TYPE::SEQUENCE;

}

CNode_Sequence::CNode_Sequence(const CNode_Sequence& rhs)
{
}

const NODE_STATE CNode_Sequence::Evaluate(const _float& fTimeDelta)
{
	if (!Is_ChildNode()) return NODE_STATE::FAILURE;

	/* �� �ϳ��� �ڽĳ��� ���� Ȥ�� Running�� ��ȯ�� ������ �����Ѵ�. */
	for (auto iter : m_pChildNodes)
	{
		if (nullptr == iter) continue;

		const NODE_STATE& eState = iter->Evaluate(fTimeDelta);

		if (NODE_STATE::SUCCESS != eState) return eState;
	}

	/* ��� �ڽ� ��尡 �����ߴٸ� �������� �����Ѵ�.*/
	return NODE_STATE::SUCCESS;
}

HRESULT CNode_Sequence::Add_ChildNode(CNode* pChildNode)
{
	NULL_CHECK_RETURN(pChildNode, E_FAIL);

	m_pChildNodes.push_back(pChildNode);
	
	return S_OK;
}

CNode* CNode_Sequence::Clone(void* pArg)
{
	return nullptr;
}

void CNode_Sequence::Free()
{
}
