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

	if (FAILED(ENGINE_INSTANCE->Add_Camera(CAM_FOLLOW, this)))
		return E_FAIL;

	/* Set Camera */
	{
		m_pCameraCom->Set_Distance(CamDist_Follow_Default);
		m_pCameraCom->Set_MouseSensitiveX(0.3f);
		m_pCameraCom->Set_MouseSensitiveY(0.1f);
		m_pCameraCom->Set_LookAtOffSet(Vec4{ 0.f, 1.8f, 0.f, 1.f });
	}

	m_eState = CGameObject::STATE_ACTIVE;

	return S_OK;
}

void CCamera_Parry::Tick(_double fTimeDelta)
{

}

void CCamera_Parry::LateTick(_double fTimeDelta)
{
	if (!m_pCameraCom->Is_TargetObj() || !m_pCameraCom->Is_LookAtObj())
		return;

	__super::LateTick(fTimeDelta);
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
