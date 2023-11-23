#include "..\Default\stdafx.h"
#include "..\Public\Camera_Peppermint_Gimmick_Crane.h"

#include "EngineInstance.h"
#include "Input.h"

#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG

CCamera_Peppermint_Gimmick_Crane::CCamera_Peppermint_Gimmick_Crane(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CCamera_Peppermint_Gimmick_Crane::CCamera_Peppermint_Gimmick_Crane(const CCamera_Peppermint_Gimmick_Crane& rhs)
	: CGameObject(rhs)
{

}

HRESULT CCamera_Peppermint_Gimmick_Crane::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_Peppermint_Gimmick_Crane::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pCameraCom->Set_MouseSensitiveX(0.5f);
	m_pCameraCom->Set_MouseSensitiveY(0.5f);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float4(0.f, 10.f, -8.f, 1.f));
	m_pTransformCom->LookAt(_float4(0.f, 0.f, 0.f, 1.f));

	m_fSpeedPerSec = 30.f;

	m_eState = CGameObject::STATE_UNACTIVE;

	if (FAILED(ENGINE_INSTANCE->Add_Camera(CAMERA_ID::CAM_PEPPERMINT_GIMMICK_CRANE, this)))
		return E_FAIL;

	return S_OK;
}

void CCamera_Peppermint_Gimmick_Crane::Tick(_double fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pCameraCom->Update(fTimeDelta);

	if (!CImGui_Manager::GetInstance()->Is_ClickedWindow() && !ENGINE_INSTANCE->Is_LerpCam() && m_bDebugMove)
		Debug_Move(fTimeDelta);
}

void CCamera_Peppermint_Gimmick_Crane::LateTick(_double fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

void CCamera_Peppermint_Gimmick_Crane::Set_State(const OBJ_STATE& eState)
{
	__super::Set_State(eState);

	if (OBJ_STATE::STATE_ACTIVE == eState)
	{
		CGameObject* pTarget = ENGINE_INSTANCE->Get_GameObject_InCurLevel(LayerNames[LAYER_ID::LAYER_ENV_INTERACTALBE], L"Env_Dynamic_Crane_000");
		
		if (nullptr == pTarget) return;

		m_pCameraCom->Set_LookAtObj(pTarget);

		Look_Target();

		m_bLookTarget = TRUE;
	}
}

HRESULT CCamera_Peppermint_Gimmick_Crane::Ready_Components()
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

void CCamera_Peppermint_Gimmick_Crane::Debug_Move(const _double& fTimeDelta)
{
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

	if (ENGINE_INSTANCE->Key_Pressing(VK_LBUTTON))
	{
		if (MouseMove = ENGINE_INSTANCE->Get_DIMMoveState(CInput_Device::MMS_X))
		{
			m_pTransformCom->Rotate(Vec4{ 0, 1, 0, 0 }, _float(MouseMove * m_pCameraCom->Get_MouseSensitiveX() * fTimeDelta));
		}

		if (MouseMove = ENGINE_INSTANCE->Get_DIMMoveState(CInput_Device::MMS_Y))
		{
			m_pTransformCom->Rotate(m_pTransformCom->Get_Right(), _float(MouseMove * m_pCameraCom->Get_MouseSensitiveY() * fTimeDelta));
		}
	}
}

void CCamera_Peppermint_Gimmick_Crane::Look_Target()
{
	if (nullptr == m_pCameraCom->Get_LookAtObj())
		return;

	Vec4 vTargetRootPos = m_pCameraCom->Get_LookAtObj()->Get_Transform()->Get_FinalPosition();

	Vec4 vTargetRelatePos = Vec4::Zero;

	Vec4 vLookAtPos = vTargetRootPos + vTargetRelatePos;

	m_pTransformCom->LookAt(vLookAtPos.OneW());
}

CCamera_Peppermint_Gimmick_Crane* CCamera_Peppermint_Gimmick_Crane::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Peppermint_Gimmick_Crane* pInstance = new CCamera_Peppermint_Gimmick_Crane(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Peppermint_Gimmick_Crane");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Peppermint_Gimmick_Crane::Clone(void* pArg)
{
	CCamera_Peppermint_Gimmick_Crane* pInstance = new CCamera_Peppermint_Gimmick_Crane(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_Peppermint_Gimmick_Crane");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Peppermint_Gimmick_Crane::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pCameraCom);
}
