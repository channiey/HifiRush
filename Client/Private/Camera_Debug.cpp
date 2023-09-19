#include "..\Default\stdafx.h"
#include "..\Public\Camera_Debug.h"

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
	m_pTransformCom->Set_Speed(10.f);
	m_pTransformCom->Set_RotRad(XMConvertToRadians(90.0f));

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_tCamDesc.vEye);
	m_pTransformCom->LookAt(XMLoadFloat4(&m_tCamDesc.vAt));
	
	return S_OK;
}

void CCamera_Debug::Tick(_float fTimeDelta)
{
	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		m_pTransformCom->Move_Left(fTimeDelta);
	}
	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		m_pTransformCom->Move_Right(fTimeDelta);
	}
	if (GetKeyState(VK_UP) & 0x8000)
	{
		m_pTransformCom->Move_Forward(fTimeDelta);
	}
	if (GetKeyState(VK_DOWN) & 0x8000)
	{
		m_pTransformCom->Move_Backward(fTimeDelta);
	}

	__super::Tick(fTimeDelta);
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
