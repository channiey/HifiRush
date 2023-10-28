#include "..\Public\BehaviourTree.h"

#include "Node.h" /* TODO 상호 참조 가능 위험*/
#include "Blackboard.h"

CBehaviourTree::CBehaviourTree(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{

}

CBehaviourTree::CBehaviourTree(const CBehaviourTree & rhs)
	: CComponent(rhs)
	, m_eState(rhs.m_eState)
	, m_pRootNode(rhs.m_pRootNode)
	, m_pBlackboard(rhs.m_pBlackboard)
{
	Safe_AddRef(m_pRootNode);
	Safe_AddRef(m_pBlackboard);

	/* 노드 타고 내려가면서 레퍼런스 카운트 다 늘려줘야 한다. */
}

HRESULT CBehaviourTree::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBehaviourTree::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CBehaviourTree::Tick(const _float& fTimeDelta)
{
	if (nullptr == m_pRootNode || !m_bActive)
		return E_FAIL;

	/* 만약 이전 프레임 실행 결과가 success였다면, 모든 노드를 재귀적으로 리셋하고 다시 평가한다. */
	if (NODE_STATE::SUCCESS == m_eState)
		m_pRootNode->Reset_Node();

	m_eState = m_pRootNode->Evaluate(fTimeDelta);
	
	return S_OK;
}

HRESULT CBehaviourTree::LateTick(const _float& fTimeDelta)
{	
	if (nullptr == m_pRootNode || !m_bActive)
		return E_FAIL;

	m_eState = m_pRootNode->Evaluate(fTimeDelta);

	return S_OK;
}

HRESULT CBehaviourTree::Set_RootNode(CNode* const pRootNode)
{
	if (nullptr == pRootNode)
		return E_FAIL;
	
	m_pRootNode = pRootNode;

	Safe_AddRef(m_pRootNode);

	return S_OK;
}

HRESULT CBehaviourTree::Set_Blackboard(CBlackboard* const pBlackboard)
{
	if (nullptr == pBlackboard)
		return E_FAIL;

	m_pBlackboard = pBlackboard;

	Safe_AddRef(m_pBlackboard);

	return S_OK;
}

HRESULT CBehaviourTree::Set_Active(const _bool& bActive)
{
	if (m_bActive == bActive) return E_FAIL;

	FAILED_CHECK_RETURN(__super::Set_Active(bActive), E_FAIL);
	
	return S_OK;
}

CBehaviourTree* CBehaviourTree::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBehaviourTree* pInstance = new CBehaviourTree(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBehaviourTree");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CBehaviourTree::Clone(void * pArg)
{
	CBehaviourTree*	pInstance = new CBehaviourTree(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBehaviourTree");
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CBehaviourTree::Free()
{
	Safe_Release(m_pRootNode);
	Safe_Release(m_pRootNode);

	Safe_Release(m_pBlackboard);

	__super::Free();
}
