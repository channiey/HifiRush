#include "..\Default\stdafx.h"
#include "..\Public\Camera_Follow.h"

#include "GameInstance.h"


CCamera_Follow::CCamera_Follow(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{

}

CCamera_Follow::CCamera_Follow(const CCamera_Follow & rhs)
	: CGameObject(rhs)
{

}

HRESULT CCamera_Follow::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_Follow::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;


	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pCameraCom->Get_CameraDesc().vEye);
	m_pTransformCom->LookAt(m_pCameraCom->Get_CameraDesc().vAt);

	if (FAILED(GAME_INSTNACE->Add_Camera(CAM_DEBUG, this)))
		return E_FAIL;

	return S_OK;
}

void CCamera_Follow::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	Move(fTimeDelta);
}

void CCamera_Follow::LateTick(_float fTimeDelta)
{
}

HRESULT CCamera_Follow::Ready_Components()
{
	/* Com_Transform */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* Com_Camera */
	CCamera::CAMERA_DESC desc;
	{
		desc.vEye = _float4(0.f, 10.f, -8.f, 1.f);
		desc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
		desc.fFovy = XMConvertToRadians(60.0f);
		desc.fAspect = g_iWinSizeX / (_float)g_iWinSizeY;
		desc.fNear = 0.2f;
		desc.fFar = 1000.0f;
	}
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Camera"),
		TEXT("Com_Camera"), (CComponent**)&m_pCameraCom, &desc)))
		return E_FAIL;


	return S_OK;
}

void CCamera_Follow::Move(const _float& fTimeDelta)
{
	
}

CCamera_Follow * CCamera_Follow::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CCamera_Follow*	pInstance = new CCamera_Follow(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Follow");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Follow::Clone(void* pArg)
{
	CCamera_Follow*	pInstance = new CCamera_Follow(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_Follow");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Follow::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pCameraCom);
	Safe_Release(m_pColliderCom);
}
