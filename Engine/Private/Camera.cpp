#include "..\Public\Camera.h"
#include "PipeLine.h"

CCamera::CCamera(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
	, m_pPipeLine(CPipeLine::GetInstance())
{
	Safe_AddRef(m_pPipeLine);
}

CCamera::CCamera(const CCamera & rhs)
	: CGameObject(rhs)
	, m_pPipeLine(rhs.m_pPipeLine)
{
	Safe_AddRef(m_pPipeLine);
}

HRESULT CCamera::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera::Initialize(void * pArg)
{
	return S_OK;
}

void CCamera::Tick(_float fTimeDelta)
{
	if (nullptr == m_pPipeLine)
		return;

	/* 자식 카메라 클래스에서 필요한 상태변환(ex 월드 변환행렬)을 진행 한 후에 해당 Tick()으로 들어온다. */

	/* 갱신된 정보를 바탕으로 파이프라인에 뷰와 투영행렬을 저장한다. */
	
	/* 뷰 스페이스 변환 행렬은 카메라 월드 변환 역행렬이다. */

	m_pPipeLine->Set_Transform(CPipeLine::D3DTS_VIEW, m_pTransformCom->Get_WorldMat().Invert());
	m_pPipeLine->Set_Transform(CPipeLine::D3DTS_PROJ, XMMatrixPerspectiveFovLH(m_tCamDesc.fFovy, m_tCamDesc.fAspect, m_tCamDesc.fNear, m_tCamDesc.fFar));
}

void CCamera::LateTick(_float fTimeDelta)
{
}

void CCamera::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pPipeLine);
}
