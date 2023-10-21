#include "..\Default\stdafx.h"
#include "..\Public\Camera_Follow.h"

#include "GameInstance.h"
#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG

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

	if (FAILED(GAME_INSTNACE->Add_Camera(CAM_FOLLOW, this)))
		return E_FAIL;

	/* Set Camera */
	{
		m_pCameraCom->Set_Distance(5.5f);
		m_pCameraCom->Set_MouseSensitiveX(0.3f);
		m_pCameraCom->Set_MouseSensitiveY(0.1f);
		m_pCameraCom->Set_LookAtOffSet(Vec4{0.f, 1.8f, 0.f, 1.f });
	}
	return S_OK;
}

void CCamera_Follow::Tick(_float fTimeDelta)
{	
	/* TODO:: 고쳐야하느니라 */
	if (nullptr == m_pCameraCom->Get_TargetObj())
	{
		CGameObject* pObj = GAME_INSTNACE->Get_GameObject(LV_PROTO, LayerNames[LAYER_PLAYER], L"Player_Chai_000");

		if (nullptr != pObj)
		{
			m_pCameraCom->Set_TargetObj(pObj);
			m_pCameraCom->Set_LookAtObj(pObj);
			m_pTransformCom->Set_Position(pObj->Get_Transform()->Get_FinalPosition());
		}
	}

	if (!m_pCameraCom->Is_TargetObj() || !m_pCameraCom->Is_LookAtObj())
		return;

	__super::Tick(fTimeDelta);

#ifdef _DEBUG
	if (!CImGui_Manager::GetInstance()->Is_ClickedWindow())
		Move(fTimeDelta);
#else
	Move(fTimeDelta);
#endif // _DEBUG
}

void CCamera_Follow::LateTick(_float fTimeDelta)
{
	if (!m_pCameraCom->Is_TargetObj() || !m_pCameraCom->Is_LookAtObj())
		return;

	__super::LateTick(fTimeDelta);
}

HRESULT CCamera_Follow::Ready_Components()
{
	/* Com_Transform */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Transform"),
		ComponentNames[COM_TRANSFORM], (CComponent**)&m_pTransformCom)))
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
		ComponentNames[COM_CAMERA], (CComponent**)&m_pCameraCom, &desc)))
		return E_FAIL;


	return S_OK;
}

void CCamera_Follow::Move(const _float& fTimeDelta)
{
	/* Position */
	Vec4 vCamWorldPos;
	{
		_long	MouseMove = 0l;

		if (MouseMove = GAME_INSTNACE->Get_DIMMoveState(CInput_Device::MMS_X))
			m_fAzimuth += MouseMove * m_pCameraCom->Get_MouseSensitiveX() * fTimeDelta * -1.f;

		if (MouseMove = GAME_INSTNACE->Get_DIMMoveState(CInput_Device::MMS_Y))
		{
			m_fElevation += MouseMove * m_pCameraCom->Get_MouseSensitiveY() * fTimeDelta;

			/* 회전 방지 */
			if (m_fElevation <= 0.01f)
				m_fElevation = 0.01f;
			else if (3.13f < m_fElevation)
				m_fElevation = 3.13f;

			/* 임시 강제 고정 */
			//m_fElevation = 0.9f;
		}

		/* 구면 좌표계(극좌표계) -> 왼손 직교 좌표계 */
		Vec4 vCamLocal;
		vCamLocal.x = m_pCameraCom->Get_Distance() * sinf(m_fElevation) * cosf(m_fAzimuth);	// x = r * sin(위도 앙각) * cos(경도 방위각)
		vCamLocal.y = m_pCameraCom->Get_Distance() * cosf(m_fElevation);					// y = r * cos(위도 앙각)
		vCamLocal.z = m_pCameraCom->Get_Distance() * sinf(m_fElevation) * sinf(m_fAzimuth);	// z = r * sin(위도 앙각) * sin(경도 방위각)

		vCamWorldPos = m_pCameraCom->Get_TargetObj()->Get_Transform()->Get_FinalPosition() + vCamLocal;

		vCamWorldPos = Vec4::Lerp(m_pTransformCom->Get_Position(), vCamWorldPos, fTimeDelta * 7.5f);
	}

	/* Rotation */
	Vec4 vLookAt;
	{
		vLookAt = m_pCameraCom->Get_TargetObj()->Get_Transform()->Get_FinalPosition() + m_pCameraCom->Get_LookAtOffSet();
		vLookAt.w = 1.f;
	}

	/* Set */
	
	m_pTransformCom->Set_Position(vCamWorldPos.OneW());
	m_pTransformCom->LookAt(vLookAt);
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
