#include "..\Default\stdafx.h"
#include "..\Public\Camera_Peppermint_Gimmick_Aim.h"

#include "EngineInstance.h"

#include "PlayerController.h"
#include "Character.h"

#include "Peppermint.h"


#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG

CCamera_Peppermint_Gimmick_Aim::CCamera_Peppermint_Gimmick_Aim(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CCamera_Peppermint_Gimmick_Aim::CCamera_Peppermint_Gimmick_Aim(const CCamera_Peppermint_Gimmick_Aim& rhs)
	: CGameObject(rhs)
{

}

HRESULT CCamera_Peppermint_Gimmick_Aim::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_Peppermint_Gimmick_Aim::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;
	
	/* Set Camera */
	{
		m_pCameraCom->Set_MouseSensitiveX(0.05f);
		m_pCameraCom->Set_MouseSensitiveY(0.05f);
		m_pCameraCom->Set_Distance(CamDist_Follow_PeppermintGimmick);
		m_pCameraCom->Set_LookAtOffSet(Vec4{ vPeppermint_Gimmick_Relative_Pos.x * -0.5f, 1.5f, 0.f, 1.f });
	}
	
	m_eState = CGameObject::STATE_UNACTIVE;

	if (FAILED(ENGINE_INSTANCE->Add_Camera(CAMERA_ID::CAM_PEPPERMINT_GIMMICK_AIM, this)))
		return E_FAIL;

	return S_OK;
}

void CCamera_Peppermint_Gimmick_Aim::Tick(_double fTimeDelta)
{
	if (nullptr == m_pCameraCom->Get_TargetObj())
		return;

	__super::Tick(fTimeDelta);

	m_pCameraCom->Update(fTimeDelta);

	/* Move */
#ifdef _DEBUG
	if (!CImGui_Manager::GetInstance()->Is_ClickedWindow() && !ENGINE_INSTANCE->Is_LerpCam())
		Rotate(fTimeDelta);
#else
	Move(fTimeDelta);
#endif // _DEBUG
}

void CCamera_Peppermint_Gimmick_Aim::LateTick(_double fTimeDelta)
{
	if (nullptr == m_pCameraCom->Get_TargetObj())
		return;

	__super::LateTick(fTimeDelta);
}

void CCamera_Peppermint_Gimmick_Aim::Set_State(const OBJ_STATE& eState)
{
	__super::Set_State(eState);

	if (OBJ_STATE::STATE_ACTIVE == eState)
	{
		if (nullptr == m_pCameraCom->Get_TargetObj())
		{
			if (FAILED(Find_Target()))
				return;
		}

		CTransform* pPeppermintTransform = m_pCameraCom->Get_TargetObj()->Get_Transform();

		if (nullptr == pPeppermintTransform) return;

		/* 포지션 세팅 (타겟의 x축 기준으로 +1, z축 기준으로 디스턴스만큼 떨어진다. */
		Vec4 vTargetPos		= pPeppermintTransform->Get_FinalPosition();
		Vec4 vRelativePos	= Vec4{ vPeppermint_Gimmick_Relative_Pos.x * -0.5f, 2.f, -m_pCameraCom->Get_Distance(), 0.f };
		vRelativePos		= pPeppermintTransform->Get_RelativePosition(vRelativePos).ZeroW();

		m_pTransformCom->Set_Position(vTargetPos + vRelativePos);
		
		/* 초기 룩 세팅 */
		m_pTransformCom->Set_Look(ENGINE_INSTANCE->Get_CurCamera_State(CTransform::STATE_LOOK).ZeroY().Normalized());
	}
}

HRESULT CCamera_Peppermint_Gimmick_Aim::Ready_Components()
{
	/* Com_Transform */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* Com_Camera */
	CCamera::PROJ_DESC desc;
	{
		desc.fFovy = CamFov_PeppermintGimmick;
		desc.fAspect = g_iWinSizeX / (_float)g_iWinSizeY;
		desc.fNear = 0.2f;
		desc.fFar = 1000.0f;
	}
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Camera"),
		TEXT("Com_Camera"), (CComponent**)&m_pCameraCom, &desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Peppermint_Gimmick_Aim::Find_Target()
{
	CGameObject* pObject = CPlayerController::GetInstance()->Get_Player(PLAYER_TYPE::PEPPERMINT);

	if (nullptr == pObject) return E_FAIL;

	m_pCameraCom->Set_TargetObj(pObject);
	m_pCameraCom->Set_LookAtObj(pObject);

	return S_OK;
}

void CCamera_Peppermint_Gimmick_Aim::Rotate(const _double& fTimeDelta)
{
	CEngineInstance* pGameInstance = GET_INSTANCE(CEngineInstance);

	/* Rotation */
	_long	MouseMove = 0l;

	if (MouseMove = pGameInstance->Get_DIMMoveState(CInput_Device::MMS_X))
	{
		m_pTransformCom->Rotate(Vec4{ 0, 1, 0, 0 }, _float(MouseMove * m_pCameraCom->Get_MouseSensitiveX() * fTimeDelta));
	}

	if (MouseMove = pGameInstance->Get_DIMMoveState(CInput_Device::MMS_Y))
	{
		m_pTransformCom->Rotate(m_pTransformCom->Get_Right(), _float(MouseMove * m_pCameraCom->Get_MouseSensitiveY() * fTimeDelta));
	}

	RELEASE_INSTANCE(CEngineInstance);
}

CCamera_Peppermint_Gimmick_Aim* CCamera_Peppermint_Gimmick_Aim::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Peppermint_Gimmick_Aim* pInstance = new CCamera_Peppermint_Gimmick_Aim(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Peppermint_Gimmick_Aim");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Peppermint_Gimmick_Aim::Clone(void* pArg)
{
	CCamera_Peppermint_Gimmick_Aim* pInstance = new CCamera_Peppermint_Gimmick_Aim(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_Peppermint_Gimmick_Aim");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Peppermint_Gimmick_Aim::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pCameraCom);

}
