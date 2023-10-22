#include "Node_Sequence.h"

CNode_Sequence::CNode_Sequence()
{
}

CNode_Sequence::CNode_Sequence(const CNode_Sequence& rhs)
{
}

HRESULT CNode_Sequence::Initialize_Node()
{
	if (FAILED(__super::Initialize_Node()))
		return E_FAIL;

	m_eType = NODE_TYPE::SEQUENCE;

	return S_OK;
}

const NODE_STATE CNode_Sequence::Evaluate(const _float& fTimeDelta)
{
	if (!Is_ChildNode()) 
		return NODE_STATE::FAILURE;

	/* 자식노드의 실행 결과가 성공일 경우, 다음 자식노드를 실행한다. 실패나 러닝일 경우 바로 반환 */
	
	list<CNode*>::iterator iter 
		= (m_ChildNodes.end() != m_iterRunning) ? m_iterRunning : m_ChildNodes.begin();

	for (; iter != m_ChildNodes.end(); iter++)
	{
		if (nullptr == *iter)
			continue;

		m_eState = (*iter)->Evaluate(fTimeDelta);

		/* 실행 결과가 running이라면 다음 업데이트때 이 노드부터 시작*/
		if (NODE_STATE::RUNNING == m_eState) 
		{
			m_iterRunning = iter;
			return m_eState;
		}

		if (NODE_STATE::FAILURE != m_eState)
		{
			m_iterRunning = m_ChildNodes.end();
			return m_eState;
		}
	}

	/* 모든 자식 노드가 성공했다면 시퀀스도 성공한다.*/
	return m_eState;
}

HRESULT CNode_Sequence::Add_ChildNode(CNode* pChildNode)
{
	if (nullptr == pChildNode)
		return E_FAIL;

	m_ChildNodes.push_back(pChildNode);
	
	pChildNode->Set_ParentNode(this);

	return S_OK;
}

HRESULT CNode_Sequence::Reset_Node()
{
	__super::Reset_Node();

	for (auto iter : m_ChildNodes)
	{
		if (nullptr != iter)
			iter->Reset_Node();
	}

	return S_OK;
}

CNode_Sequence* CNode_Sequence::Create()
{
	CNode_Sequence* pInstance = new CNode_Sequence();

	if (FAILED(pInstance->Initialize_Node()))
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
