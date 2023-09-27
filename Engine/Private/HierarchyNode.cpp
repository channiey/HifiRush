#include "..\Public\HierarchyNode.h"

CHierarchyNode::CHierarchyNode()
{

}

HRESULT CHierarchyNode::Initialize(aiNode * pAINode, CHierarchyNode* pParent, _uint iDepth)
{
	/* 뼈 이름 저장 */
	strcpy_s(m_szName, pAINode->mName.data);

	/* 부모 기준 트랜스폼 저장 */
	memcpy(&m_Transformation, &pAINode->mTransformation, sizeof(_float4x4));
	XMStoreFloat4x4(&m_Transformation, XMMatrixTranspose(XMLoadFloat4x4(&m_Transformation))); /* 씬으로부터 행렬을 가져올 때는 반드시 전치! */

	/* 나머지 트랜스폼 초기화 */
	XMStoreFloat4x4(&m_OffsetMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_CombinedTransformation, XMMatrixIdentity());

	/* 노드 정보 저장 */
	m_iDepth = iDepth;
	m_pParent = pParent;

	Safe_AddRef(m_pParent);

	return S_OK;
}

void CHierarchyNode::Set_CombinedTransformation()
{
	if (nullptr != m_pParent) /* 글로벌로 가는 코드 같다. */
		XMStoreFloat4x4(&m_CombinedTransformation, XMLoadFloat4x4(&m_Transformation) * XMLoadFloat4x4(&m_pParent->m_CombinedTransformation));
	else
		m_CombinedTransformation = m_Transformation;
}

void CHierarchyNode::Set_OffsetMatrix(_fmatrix OffsetMatrix)
{
	XMStoreFloat4x4(&m_OffsetMatrix, OffsetMatrix);
}

CHierarchyNode * CHierarchyNode::Create(aiNode * pAINode, CHierarchyNode* pParent, _uint iDepth)
{
	CHierarchyNode*			pInstance = new CHierarchyNode();

	if (FAILED(pInstance->Initialize(pAINode, pParent, iDepth)))
	{
		MSG_BOX("Failed To Created : CHierarchyNode");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHierarchyNode::Free()
{
	Safe_Release(m_pParent);
}
