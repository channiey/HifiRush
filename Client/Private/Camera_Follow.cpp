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

	if (FAILED(GAME_INSTNACE->Add_Camera(CAM_FOLLOW, this)))
		return E_FAIL;

	m_tTransDesc.fSpeedPerSec = 30.f;
	m_tTransDesc.fRotRadPerSec = XMConvertToRadians(45.f);

	/* Set Camera */
	{

	}
	return S_OK;
}

void CCamera_Follow::Tick(_float fTimeDelta)
{	
	if (nullptr == m_pCameraCom->Get_TargetObj())
	{
		CGameObject* pObj = GAME_INSTNACE->Get_GameObject(LV_PROTO, g_StrLayerID[LAYER_PLAYER], L"Player_Chai_000");

		if (nullptr != pObj)
		{
			m_pCameraCom->Set_TargetObj(pObj);
			m_pCameraCom->Set_LookAtObj(pObj);
		}
	}


	if (!m_pCameraCom->Is_TargetObj() || !m_pCameraCom->Is_LookAtObj())
		return;

	__super::Tick(fTimeDelta);

	Move(fTimeDelta);
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

void CCamera_Follow::Move(const _float& fTimeDelta)
{
	/* Position */
	_long	MouseMove = 0l;

	if (MouseMove = GAME_INSTNACE->Get_DIMMoveState(CInput_Device::MMS_X))
	{
		m_fAzimuth += MouseMove * -m_fSpeed * fTimeDelta;
		/*m_pTransformCom->Rotate(Vec4::UnitY, 
			MouseMove * m_pCameraCom->Get_MouseSensitive() * m_tTransDesc.fRotRadPerSec * fTimeDelta)*/;
	}

	if (MouseMove = GAME_INSTNACE->Get_DIMMoveState(CInput_Device::MMS_Y))
	{
		m_fEvelvation += MouseMove * m_fSpeed * fTimeDelta;

		/*m_pTransformCom->Rotate(m_pTransformCom->Get_Right(), 
			MouseMove * m_pCameraCom->Get_MouseSensitive() * m_tTransDesc.fRotRadPerSec * fTimeDelta);*/
	}


	_float t = m_fRadius * sinf(m_fEvelvation);
	_float x = t * cosf(m_fAzimuth);
	_float y = m_fRadius * cosf(m_fEvelvation);
	_float z = t * sinf(m_fAzimuth);

	Vec4 vPos = Vec4(x, y, z, 1.f) + (Vec4)m_pCameraCom->Get_TargetObj()->Get_Transform()->Get_FinalMat().m[3];
	vPos.w = 1.f;

	m_pTransformCom->Set_Position(vPos);

	/* Rotation */
	vPos = (Vec4)m_pCameraCom->Get_TargetObj()->Get_Transform()->Get_FinalMat().m[3] + m_pCameraCom->Get_LookAtOffSet();
	vPos.w = 1.f;
	m_pTransformCom->LookAt(vPos);
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
