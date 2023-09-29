#include "..\Public\HierarchyNode.h"

CHierarchyNode::CHierarchyNode()
{

}

HRESULT CHierarchyNode::Initialize(string strName, Matrix transformMatrix, Matrix offsetMatrix, _uint iBoneIndex, _uint iParentIndex, _uint iDepth)
{
	strcpy_s(m_szName, strName.c_str());

	memcpy(&m_Transformation, &transformMatrix, sizeof(_float4x4));
	memcpy(&m_OffsetMatrix, &offsetMatrix, sizeof(_float4x4));
	XMStoreFloat4x4(&m_CombinedTransformation, XMMatrixIdentity());

	m_iIndex = iBoneIndex;
	m_iParentIndex = iParentIndex;
	m_iDepth = iDepth;

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

HRESULT CHierarchyNode::Set_Parent(CHierarchyNode* pParent)
{
	if (nullptr == pParent)
		return E_FAIL;

	m_pParent = pParent;

	Safe_AddRef(m_pParent);

	return S_OK;
}

CHierarchyNode * CHierarchyNode::Create(string strName, Matrix transformMatrix, Matrix offsetMatrix, _uint iBoneIndex, _uint iParentIndex, _uint iDepth)
{
	CHierarchyNode*			pInstance = new CHierarchyNode();

	if (FAILED(pInstance->Initialize(strName, transformMatrix, offsetMatrix, iBoneIndex, iParentIndex, iDepth)))
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
