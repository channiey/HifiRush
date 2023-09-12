#include "Node_Parallel.h"

CNode_Parallel::CNode_Parallel()
{
	m_eType = NODE_TYPE::PARALLEL;

}

CNode_Parallel::CNode_Parallel(const CNode_Parallel& rhs)
{
}

const NODE_STATE CNode_Parallel::Evaluate(const _float& fTimeDelta)
{
	/* Parallel은 반환되는 값에 상관없이 모든 자식 노드를 순차적으로 실행한다. */
	/* 모든 자식노드가 성공할 경우 성공 반환, 하나라도 실패할 경우 실패 반환, 이외에는 running을 반환한다. */

	if (!Is_ChildNode()) return NODE_STATE::FAILURE;

	_bool bAnyFailure = FALSE;
	_bool bAnyRunning = FALSE;

	for (auto iter : m_pChildNodes)
	{
		if (nullptr == iter) continue;

		const NODE_STATE& eChildState = iter->Evaluate(fTimeDelta);

		switch (eChildState)
		{
			case NODE_STATE::FAILURE: bAnyFailure = TRUE; break;
			case NODE_STATE::RUNNING: bAnyRunning = TRUE; break;
			default: break;	
		}
	}

	return bAnyFailure ? NODE_STATE::FAILURE : (bAnyRunning ? NODE_STATE::RUNNING : NODE_STATE::SUCCESS);
}

HRESULT CNode_Parallel::Add_ChildNode(CNode* pChildNode)
{
	NULL_CHECK_RETURN(pChildNode, E_FAIL);

	m_pChildNodes.push_back(pChildNode);

	return S_OK;
}

CNode* CNode_Parallel::Clone(void* pArg)
{
	return nullptr;
}

void CNode_Parallel::Free()
{
}
