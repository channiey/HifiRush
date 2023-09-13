#include "Node_Parallel.h"

CNode_Parallel::CNode_Parallel()
{
}

CNode_Parallel::CNode_Parallel(const CNode_Parallel& rhs)
{
}

HRESULT CNode_Parallel::Initialize_Node()
{
	m_eType = NODE_TYPE::PARALLEL;

	return S_OK;
}

const NODE_STATE CNode_Parallel::Evaluate(const _float& fTimeDelta)
{
	/* Parallel�� ��ȯ�Ǵ� ���� ������� ��� �ڽ� ��带 ���������� �����Ѵ�. */
	/* ��� �ڽĳ�尡 ������ ��� ���� ��ȯ, �ϳ��� ������ ��� ���� ��ȯ, �̿ܿ��� running�� ��ȯ�Ѵ�. */

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

CNode_Parallel* CNode_Parallel::Create(void* pArg)
{
	CNode_Parallel* pInstance = new CNode_Parallel();

	if (FAILED(pInstance->Initialize_Node()))
	{
		MSG_BOX("Failed to Created : CNode_Parallel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNode_Parallel::Free()
{
	__super::Free();
}
