#include "..\Public\Camera.h"
#include "PipeLine.h"

CCamera::CCamera(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{
}

CCamera::CCamera(const CCamera & rhs)
	: CComponent(rhs)
{
}

HRESULT CCamera::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera::Initialize(void * pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	memmove(&m_tProjDesc, pArg, sizeof PROJ_DESC);

	return S_OK;
}

HRESULT CCamera::Update(const _double fDeltaTime)
{
	Update_Lerp(fDeltaTime);

	return S_OK;
}

void CCamera::Lerp_Fov(const _float fStartValue, const _float& fTargetValue, const _double& fTime, const LERP_MODE& eMode)
{
	m_tLerpFov.Start(fStartValue, fTargetValue, fTime, eMode);
}

void CCamera::Lerp_Fov(const _float& fTargetValue, const _double& fTime, const LERP_MODE& eMode)
{
	m_tLerpFov.Start(m_tProjDesc.fFovy, fTargetValue, fTime, eMode);
}

void CCamera::Lerp_Dist(const _float fStartValue, const _float& fTargetValue, const _double& fTime, const LERP_MODE& eMode)
{
	m_tLerpDist.Start(fStartValue, fTargetValue, fTime, eMode);
}

void CCamera::Lerp_Dist(const _float& fTargetValue, const _double& fTime, const LERP_MODE& eMode)
{
	m_tLerpDist.Start(m_fDistance, fTargetValue, fTime, eMode);
}

void CCamera::Change_TargetObj(CGameObject* pObj)
{
}

void CCamera::Change_LookAtObj(CGameObject* pObj)
{
}

void CCamera::Update_Lerp(const _double fDeltaTime)
{
	if (m_tLerpFov.bActive)
	{
		m_tLerpFov.Update(fDeltaTime);
		m_tProjDesc.fFovy = m_tLerpFov.fCurValue;
		cout << "FOV : " << m_tLerpFov.fCurValue << endl;
	}

	if (m_tLerpDist.bActive)
	{
		m_tLerpDist.Update(fDeltaTime);
		m_fDistance = m_tLerpDist.fCurValue;

		cout << "DIST : " << m_tLerpDist.fCurValue << endl;
	}
}

CCamera* CCamera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera* pInstance = new CCamera(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CCamera::Clone(void* pArg)
{
	CCamera* pInstance = new CCamera(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera::Free()
{
	__super::Free();

}
