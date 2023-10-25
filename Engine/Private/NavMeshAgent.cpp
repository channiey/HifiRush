#include "GameInstance.h"
#include "NavMesh.h"

#include "GameObject.h"

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

	NAVMESHAGENT_DESC tDesc;
	{
		memmove(&tDesc, pArg, sizeof(NAVMESHAGENT_DESC));
		if (nullptr == tDesc.pTransfrom)
			return E_FAIL;
	}

	m_iCurIndex = tDesc.iCurIndex;

	tDesc.pTransfrom->m_pNavMeshAgentCom = this;

	return S_OK;
}

HRESULT CNavMeshAgent::Render()
{
	if (!m_pNavMesh->Is_Render())
		return S_OK;

	if (FAILED(m_pNavMesh->Render_Cell(m_iCurIndex)))
		return E_FAIL;

	return S_OK;
}

const _bool CNavMeshAgent::Can_Move(_fvector vPoint)
{
	return m_pNavMesh->Can_Move(vPoint, m_iCurIndex);
}

const _float CNavMeshAgent::Get_Height(const Vec3& vPos)
{
	return m_pNavMesh->Get_AgentHeight(m_iCurIndex, vPos);
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
