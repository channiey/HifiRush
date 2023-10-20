#include "Node_Root.h"

CNode_Root::CNode_Root()
{
}

CNode_Root::CNode_Root(const CNode_Root& rhs)
{
}

HRESULT CNode_Root::Initialize_Node(class CBlackboard* pBlackboard)
{
	if (FAILED(__super::Initialize_Node(pBlackboard)))
		return E_FAIL;

	m_eType = NODE_TYPE::ROOT;

	return S_OK;
}

const NODE_STATE CNode_Root::Evaluate(const _float& fTimeDelta)
{
	if (!Is_ChildNode())
		return NODE_STATE::FAILURE; 

	return m_pChildNodes.front()->Evaluate(fTimeDelta);
}

HRESULT CNode_Root::Add_ChildNode(CNode* pChildNode)
{
	/* 루트 노드는 단 하나의 자식 노드만을 갖는다. */

	if (nullptr == pChildNode || Is_ChildNode())
		return E_FAIL;
		
	m_pChildNodes.push_back(pChildNode);

	return S_OK;
}

CNode_Root* CNode_Root::Create(class CBlackboard* pBlackboard)
{
	CNode_Root* pInstance = new CNode_Root();

	if (FAILED(pInstance->Initialize_Node(pBlackboard)))
	{
		MSG_BOX("Failed to Created : CNode_Root");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNode_Root::Free()
{
	__super::Free();
}

