#include "Node_Selector.h"

CNode_Selector::CNode_Selector()
{
}

CNode_Selector::CNode_Selector(const CNode_Selector& rhs)
{
}

HRESULT CNode_Selector::Initialize_Node()
{
	if (FAILED(__super::Initialize_Node()))
		return E_FAIL;

	m_eType = NODE_TYPE::SELECTOR;

	return S_OK;
}

const NODE_STATE CNode_Selector::Evaluate(const _float& fTimeDelta)
{
	if (!Is_ChildNode())
		return NODE_STATE::FAILURE;

	/* 단 하나의 자식노드라도 SUCCESS 혹은 RUNNING을 반환할 때까지 실행한다. */
	for (auto iter : m_ChildNodes)
	{
		if (nullptr == iter) 
			continue;

		m_eState = iter->Evaluate(fTimeDelta);

		if (NODE_STATE::FAILURE != m_eState)
			return m_eState;
	}

	/* 모든 자식 노드가 실패했다면 셀렉터도 실패한다.*/
	return m_eState;
}

HRESULT CNode_Selector::Add_ChildNode(CNode* pChildNode)
{
	if (nullptr == pChildNode)
		return E_FAIL;

	m_ChildNodes.push_back(pChildNode);

	pChildNode->Set_ParentNode(this);

	return S_OK;
}

HRESULT CNode_Selector::Reset_Node()
{
	__super::Reset_Node();

	for (auto iter : m_ChildNodes)
	{
		if (nullptr != iter)
			iter->Reset_Node();
	}

	return S_OK;
}

CNode_Selector* CNode_Selector::Create()
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
