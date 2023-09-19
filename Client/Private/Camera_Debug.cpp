#include "..\Default\stdafx.h"
#include "..\Public\Camera_Debug.h"

#include "GameInstance.h"
CCamera_Debug::CCamera_Debug(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCamera(pDevice, pContext)
{

}

CCamera_Debug::CCamera_Debug(const CCamera_Debug & rhs)
	: CCamera(rhs)
{

}

HRESULT CCamera_Debug::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_Debug::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	/* 파생 클래스의 단독 데이터를 세팅한다. */
	m_fMouseSensitive = 1.f;

	/* 카메라 베이스 클래스의 초기 구조체 데이터를 세팅한다. */
	ZeroMemory(&m_tCamDesc, sizeof(CAMERA_DESC));

	m_tCamDesc.vEye = _float4(0.f, 10.f, -8.f, 1.f);
	m_tCamDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
	m_tCamDesc.fFovy = XMConvertToRadians(60.0f);
	m_tCamDesc.fAspect = g_iWinSizeX / (_float)g_iWinSizeY;
	m_tCamDesc.fNear = 0.2f;
	m_tCamDesc.fFar = 300.0f;

	/* 카메라 베이스 클래스의 트랜스폼 정보를 세팅한다. */

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_tCamDesc.vEye);
	m_pTransformCom->LookAt(m_tCamDesc.vAt);

	m_tTransDesc.fSpeedPerSec = 10.f;
	m_tTransDesc.fRotRadPerSec = 90.f;
	
	return S_OK;
}

void CCamera_Debug::Tick(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Key_Pressing('W'))
	{
		m_pTransformCom->Translate(m_pTransformCom->Get_Forward() * m_tTransDesc.fSpeedPerSec * fTimeDelta);
	}
	if (pGameInstance->Key_Pressing('S'))
	{
		m_pTransformCom->Translate(m_pTransformCom->Get_Backward() * m_tTransDesc.fSpeedPerSec * fTimeDelta);
	}
	if (pGameInstance->Key_Pressing('A'))
	{
		m_pTransformCom->Translate(m_pTransformCom->Get_Left() * m_tTransDesc.fSpeedPerSec * fTimeDelta);
	}
	if (pGameInstance->Key_Pressing('D'))
	{
		m_pTransformCom->Translate(m_pTransformCom->Get_Right() * m_tTransDesc.fSpeedPerSec * fTimeDelta);
	}

	__super::Tick(fTimeDelta);

	Safe_Release(pGameInstance);
}

void CCamera_Debug::LateTick(_float fTimeDelta)
{
}

HRESULT CCamera_Debug::Ready_Components()
{
	/* Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}

CCamera_Debug * CCamera_Debug::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CCamera_Debug*	pInstance = new CCamera_Debug(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Debug");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Debug::Clone(void* pArg)
{
	CCamera_Debug*	pInstance = new CCamera_Debug(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_Debug");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Debug::Free()
{
	__super::Free();

}
