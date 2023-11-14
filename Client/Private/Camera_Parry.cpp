#include "..\Default\stdafx.h"
#include "..\Public\Camera_Parry.h"

#include "EngineInstance.h"
#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG

#include "Character.h"

CCamera_Parry::CCamera_Parry(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CCamera_Parry::CCamera_Parry(const CCamera_Parry& rhs)
	: CGameObject(rhs)
{

}

HRESULT CCamera_Parry::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_Parry::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(ENGINE_INSTANCE->Add_Camera(CAM_PARRY, this)))
		return E_FAIL;

	m_eState = CGameObject::STATE_UNACTIVE;

	return S_OK;
}

void CCamera_Parry::Tick(_double fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CCamera_Parry::LateTick(_double fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

void CCamera_Parry::Set_CamTransform(CTransform* pTargetTransform)
{
	/* 카메라의 포지션, 룩, 회전 설정 */
	Vec4 vCamPos = pTargetTransform->Get_Position() 
		+ pTargetTransform->Get_State(CTransform::STATE_LOOK).ZeroY().Normalized() * 10.f;

	Vec4 fDir = pTargetTransform->Get_FinalPosition() - vCamPos;

	m_pTransformCom->Set_Position(vCamPos);
	m_pTransformCom->Set_Look(fDir.ZeroY().Normalized());
	m_pTransformCom->Rotate(Vec4{ 0.f, 0.f, 1.f, 0.f }, DEG2RAD(20.f));
}

void CCamera_Parry::Zoom_In()
{
}

void CCamera_Parry::Zoom_Out()
{
}

HRESULT CCamera_Parry::Ready_Components()
{
	/* Com_Transform */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Transform"),
		ComponentNames[COM_TRANSFORM], (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* Com_Camera */
	CCamera::PROJ_DESC desc;
	{
		desc.fFovy = CamFov_Follow_Default;
		desc.fAspect = g_iWinSizeX / (_float)g_iWinSizeY;
		desc.fNear = 0.2f;
		desc.fFar = 1000.0f;
	}
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Camera"),
		ComponentNames[COM_CAMERA], (CComponent**)&m_pCameraCom, &desc)))
		return E_FAIL;


	return S_OK;
}


CCamera_Parry* CCamera_Parry::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Parry* pInstance = new CCamera_Parry(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Parry");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Parry::Clone(void* pArg)
{
	CCamera_Parry* pInstance = new CCamera_Parry(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_Parry");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Parry::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pCameraCom);
}
