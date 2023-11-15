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

	/* Set Camera */
	{
		m_fMaxDistance = 12.f;
		m_pCameraCom->Set_Distance(10.f);
	}

	m_eState = CGameObject::STATE_UNACTIVE;

	return S_OK;
}

void CCamera_Parry::Tick(_double fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pCameraCom->Update(fTimeDelta);

	if (m_pCameraCom->Is_Lerp_Dist())
	{
		Vec4 vCamPos = m_vPlayerStartPos + m_vRelativePos.ZeroW() + m_pCameraCom->Get_TargetOffSet().ZeroW();
		Vec4 vLook = m_pTargetTransformCom->Get_FinalPosition() - vCamPos;

		vCamPos += vLook.Normalized() * m_pCameraCom->Get_Distance();

		Vec4 vLookAtPos = m_pTargetTransformCom->Get_FinalPosition() + m_pCameraCom->Get_LookAtOffSet();

		m_pTransformCom->Set_Position(vCamPos);
		m_pTransformCom->LookAt(vLookAtPos);
	}
}

void CCamera_Parry::LateTick(_double fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

void CCamera_Parry::Set_CamTransform(CTransform* pTargetTransform, CTransform* pPlayerTransform, Vec4 vPlayerNewPos)
{
	m_pTargetTransformCom = pTargetTransform;

	/* Set Origin Position */
	{
		m_vPlayerStartPos = vPlayerNewPos;

		Matrix matWorld = pPlayerTransform->Get_WorldMat();
		memcpy(matWorld.m[3], &Vec4::Zero, sizeof(Vec3));

		m_vRelativePos		= XMVector3TransformCoord(Vec3{1.5f, 0.f ,-5.f}, matWorld);
	}

	/* Set Camera Component Data */
	{
		m_pCameraCom->Set_LookAtObj(m_pTargetTransformCom->Get_Owner());
		m_pCameraCom->Set_TargetObj(m_pTargetTransformCom->Get_Owner());
		m_pCameraCom->Set_LookAtOffSet(Vec4{ 0.f, 3.f, 0.f, 0.f });
		m_pCameraCom->Set_TargetOffSet(Vec4{ 0.f, 2.f, 0.f, 0.f });
	}

	/* Set Transform */
	{
		Vec4 vCamPos	= m_vPlayerStartPos + m_vRelativePos.ZeroW() + m_pCameraCom->Get_TargetOffSet().ZeroW();
		Vec4 vLook		= m_pTargetTransformCom->Get_FinalPosition() - vCamPos;

		vCamPos += vLook.Normalized() * m_pCameraCom->Get_Distance();

		Vec4 vLookAtPos = m_pTargetTransformCom->Get_FinalPosition() + m_pCameraCom->Get_LookAtOffSet();

		m_pTransformCom->Set_Position(vCamPos);
		m_pTransformCom->LookAt(vLookAtPos);
	}
}

void CCamera_Parry::Zoom_In()
{
	m_pCameraCom->Lerp_Dist(m_fMaxDistance, 0.5f, LERP_MODE::SMOOTHER_STEP);
}

void CCamera_Parry::Zoom_Out()
{
	m_pCameraCom->Lerp_Dist(0.f, 0.5f, LERP_MODE::SMOOTHER_STEP);
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
