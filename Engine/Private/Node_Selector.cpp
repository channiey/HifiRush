#include "Node_Selector.h"

CNode_Selector::CNode_Selector()
{
}

CNode_Selector::CNode_Selector(const CNode_Selector& rhs)
{
}

HRESULT CNode_Selector::Initialize_Node(class CBlackboard* pBlackboard)
{
	if (FAILED(__super::Initialize_Node(pBlackboard)))
		return E_FAIL;

	m_eType = NODE_TYPE::SELECTOR;

	return S_OK;
}

const NODE_STATE CNode_Selector::Evaluate(const _float& fTimeDelta)
{
	if (!Is_ChildNode())
		return NODE_STATE::FAILURE;

	/* �� �ϳ��� �ڽĳ��� SUCCESS Ȥ�� RUNNING�� ��ȯ�� ������ �����Ѵ�. */
	for (auto iter : m_pChildNodes)
	{
		if (nullptr == iter) 
			continue;

		const NODE_STATE& eState = iter->Evaluate(fTimeDelta);

		if (NODE_STATE::FAILURE != eState) 
			return eState;
	}

	/* ��� �ڽ� ��尡 �����ߴٸ� �����͵� �����Ѵ�.*/
	return NODE_STATE::FAILURE;
}

HRESULT CNode_Selector::Add_ChildNode(CNode* pChildNode)
{
	if (nullptr == pChildNode)
		return E_FAIL;

	m_pChildNodes.push_back(pChildNode);

	return S_OK;
}

CNode_Selector* CNode_Selector::Create(class CBlackboard* pBlackboard)
{
	CNode_Selector* pInstance = new CNode_Selector();

	if (FAILED(pInstance->Initialize_Node(pBlackboard)))
	{
		MSG_BOX("Failed to Created : CNode_Selector");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNode_Selector::Free()
{
	__super::Free();
}
