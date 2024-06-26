#include "..\Default\stdafx.h"
#include "..\Public\Camera_Debug.h"

#include "EngineInstance.h"
#include "Input.h"

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
	 
	m_pCameraCom->Set_MouseSensitiveX(0.5f);
	m_pCameraCom->Set_MouseSensitiveY(0.5f);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float4(0.f, 10.f, -8.f, 1.f));
	m_pTransformCom->LookAt(_float4(0.f, 0.f, 0.f, 1.f));

	m_fSpeedPerSec = 60.f;

	m_eState = CGameObject::STATE_UNACTIVE;

	if (FAILED(ENGINE_INSTANCE->Add_Camera(CAM_DEBUG, this)))
		return E_FAIL;

	return S_OK;
}

void CCamera_Debug::Tick(_double fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pCameraCom->Update(fTimeDelta);

	/* Move */
#ifdef _DEBUG
	if (!CImGui_Manager::GetInstance()->Is_ClickedWindow() && !ENGINE_INSTANCE->Is_LerpCam())
		Move(fTimeDelta);
#else
	Move(fTimeDelta);
#endif // _DEBUG

}

void CCamera_Debug::LateTick(_double fTimeDelta)
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

void CCamera_Debug::Move(const _double& fTimeDelta)
{
	CEngineInstance* pGameInstance = GET_INSTANCE(CEngineInstance);

	/* Translation */
	if (Input::Up())
	{
		m_pTransformCom->Translate(m_pTransformCom->Get_Forward() * m_fSpeedPerSec * fTimeDelta);
	}
	if (Input::Down())
	{
		m_pTransformCom->Translate(m_pTransformCom->Get_Backward() * m_fSpeedPerSec * fTimeDelta);
	}
	if (Input::Left())
	{
		m_pTransformCom->Translate(m_pTransformCom->Get_Left() * m_fSpeedPerSec * fTimeDelta);
	}
	if (Input::Right())
	{
		m_pTransformCom->Translate(m_pTransformCom->Get_Right() * m_fSpeedPerSec * fTimeDelta);
	}

	/* Rotation */

	_long	MouseMove = 0l;

	if (pGameInstance->Key_Pressing(VK_LBUTTON))
	{
		if (MouseMove = pGameInstance->Get_DIMMoveState(CInput_Device::MMS_X))
		{
			m_pTransformCom->Rotate(Vec4{ 0, 1, 0, 0}, _float(MouseMove * m_pCameraCom->Get_MouseSensitiveX() * fTimeDelta));
		}

		if (MouseMove = pGameInstance->Get_DIMMoveState(CInput_Device::MMS_Y))
		{
			m_pTransformCom->Rotate(m_pTransformCom->Get_Right(), _float(MouseMove * m_pCameraCom->Get_MouseSensitiveY() * fTimeDelta));
		}
	}

	RELEASE_INSTANCE(CEngineInstance);
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
