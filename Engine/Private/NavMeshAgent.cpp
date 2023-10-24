#include "GameInstance.h"
#include "NavMesh.h"
CNavMeshAgent::CNavMeshAgent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{
}

CNavMeshAgent::CNavMeshAgent(const CNavMeshAgent& rhs)
	: CComponent(rhs)
	, m_pNavMesh(CNavMesh::GetInstance())
{
}

HRESULT CNavMeshAgent::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CNavMeshAgent::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	memmove(&m_iCurIndex, pArg, sizeof NAVMESHAGENT_DESC);

	return S_OK;
}

const _bool CNavMeshAgent::Can_Move(_fvector vPoint)
{
	if (m_pNavMesh->Can_Move(vPoint, m_iCurIndex))
		return FALSE;

	return TRUE;
}

CNavMeshAgent* CNavMeshAgent::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CNavMeshAgent* pInstance = new CNavMeshAgent(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CNavMeshAgent");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CNavMeshAgent::Clone(void* pArg)
{
	CNavMeshAgent* pInstance = new CNavMeshAgent(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CNavMeshAgent");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNavMeshAgent::Free()
{
	__super::Free();

	Safe_Release(m_pNavMesh);
}
