#include "Node_Selector.h"

CNode_Selector::CNode_Selector()
{
}

CNode_Selector::CNode_Selector(const CNode_Selector& rhs)
{
}

HRESULT CNode_Selector::Initialize_Node()
{
	m_eType = NODE_TYPE::SELECTOR;

	return S_OK;
}

const NODE_STATE CNode_Selector::Evaluate(const _float& fTimeDelta)
{
	if (!Is_ChildNode()) return NODE_STATE::FAILURE;

	/* 단 하나의 자식노드라도 성공 혹은 Running을 반환할 때까지 실행한다. */
	for (auto iter : m_pChildNodes)
	{
		if (nullptr == iter) continue;

		const NODE_STATE& eState = iter->Evaluate(fTimeDelta);

		if (NODE_STATE::FAILURE != eState) return eState;
	}

	/* 모든 자식 노드가 실패했다면 셀렉터도 실패한다.*/
	return NODE_STATE::FAILURE;
}

HRESULT CNode_Selector::Add_ChildNode(CNode* pChildNode)
{
	NULL_CHECK_RETURN(pChildNode, E_FAIL);

	m_pChildNodes.push_back(pChildNode);

	return S_OK;
}

CNode_Selector* CNode_Selector::Create(void* pArg)
{
	CNode_Selector* pInstance = new CNode_Selector();

	if (FAILED(pInstance->Initialize_Node()))
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
