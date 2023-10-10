#include "..\Public\Bone.h"

CBone::CBone()
{
}

CBone::CBone(const CBone& rhs)
	: m_iIndex(rhs.m_iIndex)
	, m_iParentIndex(rhs.m_iParentIndex)
	, m_iDepth(rhs.m_iDepth)
{
	strcpy_s(m_szName, rhs.m_szName);

	memcpy(&m_Transformation, &rhs.m_Transformation, sizeof(_float4x4));
	memcpy(&m_OffsetMatrix, &rhs.m_OffsetMatrix, sizeof(_float4x4));
	memcpy(&m_CombinedTransformation, &rhs.m_CombinedTransformation, sizeof(_float4x4));
}

HRESULT CBone::Initialize_Prototype(string strName, Matrix transformMatrix, Matrix offsetMatrix, _int iBoneIndex, _int iParentIndex, _uint iDepth)
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

HRESULT CBone::Initialize(void* pArg)
{
	return S_OK;
}

void CBone::Set_CombinedTransformation()
{
	if (nullptr != m_pParent) /* To Global */
		XMStoreFloat4x4(&m_CombinedTransformation, 
			XMLoadFloat4x4(&m_Transformation) * XMLoadFloat4x4(&m_pParent->m_CombinedTransformation));
	else
		m_CombinedTransformation = m_Transformation;
}

HRESULT CBone::Set_Parent(CBone* pParent)
{
	if (nullptr == pParent)
		return E_FAIL;

	if (nullptr != m_pParent)
		Safe_Release(m_pParent);

	m_pParent = pParent;
	Safe_AddRef(m_pParent);

	return S_OK;
}

CBone * CBone::Create(string strName, Matrix transformMatrix, Matrix offsetMatrix, _int iBoneIndex, _int iParentIndex, _uint iDepth)
{
	CBone*			pInstance = new CBone();

	if (FAILED(pInstance->Initialize_Prototype(strName, transformMatrix, offsetMatrix, iBoneIndex, iParentIndex, iDepth)))
	{
		MSG_BOX("Failed To Created : CBone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CBone* CBone::Clone(void* pArg)
{
	CBone* pInstance = new CBone(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CBone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBone::Free()
{
	Safe_Release(m_pParent);
}
