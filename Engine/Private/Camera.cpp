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

	/* �ڽ� ī�޶� Ŭ�������� �ʿ��� ���º�ȯ(ex ���� ��ȯ���)�� ���� �� �Ŀ� �ش� Tick()���� ���´�. */

	/* ���ŵ� ������ �������� ���������ο� ��� ��������� �����Ѵ�. */
	
	/* �� �����̽� ��ȯ ����� ī�޶� ���� ��ȯ ������̴�. */

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
