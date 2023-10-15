#include "..\Default\stdafx.h"
#include "..\Public\Camera_Debug.h"

#include "GameInstance.h"

#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG

CCamera_Debug::CCamera_Debug(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{

}

CCamera_Debug::CCamera_Debug(const CCamera_Debug & rhs)
	: CGameObject(rhs)
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

	m_pCameraCom->Set_MouseSensitiveX(1.f);
	m_pCameraCom->Set_MouseSensitiveY(1.f);

	m_tTransDesc.fSpeedPerSec = 30.f;
	m_tTransDesc.fRotRadPerSec = XMConvertToRadians(45.f);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float4(0.f, 10.f, -8.f, 1.f));
	m_pTransformCom->LookAt(_float4(0.f, 0.f, 0.f, 1.f));

	if (FAILED(GAME_INSTNACE->Add_Camera(CAM_DEBUG, this)))
		return E_FAIL;

	return S_OK;
}

void CCamera_Debug::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	/* Move */
#ifdef _DEBUG
	if (!CImGui_Manager::GetInstance()->Is_ClickedWindow())
		Move(fTimeDelta);
#else
	Move(fTimeDelta);
#endif // _DEBUG

}

void CCamera_Debug::LateTick(_float fTimeDelta)
{
}

HRESULT CCamera_Debug::Ready_Components()
{
	/* Com_Transform */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* Com_Camera */
	CCamera::PROJ_DESC desc;
	{
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

void CCamera_Debug::Move(const _float& fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	/* Translation */
	if (pGameInstance->Key_Pressing(VK_UP))
	{
		m_pTransformCom->Translate(m_pTransformCom->Get_Forward() * m_tTransDesc.fSpeedPerSec * fTimeDelta);
	}
	if (pGameInstance->Key_Pressing(VK_DOWN))
	{
		m_pTransformCom->Translate(m_pTransformCom->Get_Backward() * m_tTransDesc.fSpeedPerSec * fTimeDelta);
	}
	if (pGameInstance->Key_Pressing(VK_LEFT))
	{
		m_pTransformCom->Translate(m_pTransformCom->Get_Left() * m_tTransDesc.fSpeedPerSec * fTimeDelta);
	}
	if (pGameInstance->Key_Pressing(VK_RIGHT))
	{
		m_pTransformCom->Translate(m_pTransformCom->Get_Right() * m_tTransDesc.fSpeedPerSec * fTimeDelta);
	}

	/* Rotation */

	_long	MouseMove = 0l;

	if (pGameInstance->Key_Pressing(VK_LBUTTON))
	{
		if (MouseMove = pGameInstance->Get_DIMMoveState(CInput_Device::MMS_X))
		{
			m_pTransformCom->Rotate(Vec4{ 0, 1, 0, 0}, MouseMove * m_pCameraCom->Get_MouseSensitiveX() * m_tTransDesc.fRotRadPerSec * fTimeDelta);
		}

		if (MouseMove = pGameInstance->Get_DIMMoveState(CInput_Device::MMS_Y))
		{
			m_pTransformCom->Rotate(m_pTransformCom->Get_Right(), MouseMove * m_pCameraCom->Get_MouseSensitiveY() * m_tTransDesc.fRotRadPerSec * fTimeDelta);
		}
	}

	RELEASE_INSTANCE(CGameInstance);
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

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pCameraCom);

}
