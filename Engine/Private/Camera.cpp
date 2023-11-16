#include "..\Public\Camera.h"
#include "PipeLine.h"

#include "GameObject.h"
#include "Transform.h"

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
	Update_Shake(fDeltaTime);

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

void CCamera::Shake_Camera(const _float& fShakeTime, const _int& iIntensity)
{
	m_tShakeDesc.bActive	= TRUE;
	m_tShakeDesc.fAcc		= 0.f;
	m_tShakeDesc.fShakeTime = fShakeTime;
	m_tShakeDesc.iIntensity = iIntensity;

	m_tShakeDesc.vOriginPos = m_pOwner->Get_Transform()->Get_Position();
}

void CCamera::Update_Lerp(const _double fDeltaTime)
{
	if (m_tLerpFov.bActive)
	{
		m_tLerpFov.Update(fDeltaTime);
		m_tProjDesc.fFovy = m_tLerpFov.fCurValue;
	}

	if (m_tLerpDist.bActive)
	{
		m_tLerpDist.Update(fDeltaTime);
		m_fDistance = m_tLerpDist.fCurValue;
	}
}

void CCamera::Update_Shake(const _double fDeltaTime)
{
	if (!m_tShakeDesc.bActive) return;

	m_tShakeDesc.fAcc += fDeltaTime;

	Vec4 vShakePos;
	
	vShakePos.x = rand() % m_tShakeDesc.iIntensity * 0.01f;
	vShakePos.y = rand() % m_tShakeDesc.iIntensity * 0.01f;
	vShakePos.z = rand() % m_tShakeDesc.iIntensity * 0.01f;

	vShakePos += m_tShakeDesc.vOriginPos;

	m_pOwner->Get_Transform()->Set_Position(vShakePos);
	
	if (m_tShakeDesc.fShakeTime <= m_tShakeDesc.fAcc)
	{
		m_tShakeDesc.Reset();
		m_pOwner->Get_Transform()->Set_Position(m_tShakeDesc.vOriginPos);
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
