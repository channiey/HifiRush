#include "Node_Sequence.h"

CNode_Sequence::CNode_Sequence()
{
}

CNode_Sequence::CNode_Sequence(const CNode_Sequence& rhs)
{
}

HRESULT CNode_Sequence::Initialize_Node(class CBlackboard* pBlackboard)
{
	if (FAILED(__super::Initialize_Node(pBlackboard)))
		return E_FAIL;

	m_eType = NODE_TYPE::SEQUENCE;

	return S_OK;
}

const NODE_STATE CNode_Sequence::Evaluate(const _float& fTimeDelta)
{
	if (!Is_ChildNode()) 
		return NODE_STATE::FAILURE;

	/* 단 하나의 자식노드라도 FAILURE 혹은 RUNNING을 반환할 때까지 실행한다. */
	for (auto iter : m_pChildNodes)
	{
		if (nullptr == iter) 
			continue;

		const NODE_STATE& eState = iter->Evaluate(fTimeDelta);

		if (NODE_STATE::SUCCESS != eState) 
			return eState;
	}

	/* 모든 자식 노드가 성공했다면 시퀀스도 성공한다.*/
	return NODE_STATE::SUCCESS;
}

HRESULT CNode_Sequence::Add_ChildNode(CNode* pChildNode)
{
	if (nullptr == pChildNode)
		return E_FAIL;

	m_pChildNodes.push_back(pChildNode);
	
	return S_OK;
}

CNode_Sequence* CNode_Sequence::Create(class CBlackboard* pBlackboard)
{
	CNode_Sequence* pInstance = new CNode_Sequence();

	if (FAILED(pInstance->Initialize_Node(pBlackboard)))
	{
		MSG_BOX("Failed to Created : CNode_Sequence");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNode_Sequence::Free()
{
	__super::Free();
}
