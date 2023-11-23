#include "..\Default\stdafx.h"
#include "..\Public\Camera_Macaron_Gimmick_Wall.h"

#include "EngineInstance.h"

#include "PlayerController.h"
#include "Character.h"

#include "Macaron.h"


#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG

CCamera_Macaron_Gimmick_Wall::CCamera_Macaron_Gimmick_Wall(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CCamera_Macaron_Gimmick_Wall::CCamera_Macaron_Gimmick_Wall(const CCamera_Macaron_Gimmick_Wall& rhs)
	: CGameObject(rhs)
{

}

HRESULT CCamera_Macaron_Gimmick_Wall::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_Macaron_Gimmick_Wall::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	/* Set Camera */
	{
		m_pCameraCom->Set_MouseSensitiveX(0.05f);
		m_pCameraCom->Set_MouseSensitiveY(0.05f);
		m_pCameraCom->Set_Distance(CamDist_Follow_MacaronGimmick);
		m_pCameraCom->Set_LookAtOffSet(Vec4{ vMacaron_Gimmick_Relative_Pos.x * -0.5f, 1.5f, 0.f, 1.f });
	}

	m_eState = CGameObject::STATE_UNACTIVE;

	if (FAILED(ENGINE_INSTANCE->Add_Camera(CAMERA_ID::CAM_MACARON_GIMMICK_WALL, this)))
		return E_FAIL;

	return S_OK;
}

void CCamera_Macaron_Gimmick_Wall::Tick(_double fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pCameraCom->Update(fTimeDelta);

}

void CCamera_Macaron_Gimmick_Wall::LateTick(_double fTimeDelta)
{
	if (nullptr == m_pCameraCom->Get_TargetObj())
		return;

	__super::LateTick(fTimeDelta);
}

void CCamera_Macaron_Gimmick_Wall::Set_State(const OBJ_STATE& eState)
{
	__super::Set_State(eState);

	if (OBJ_STATE::STATE_ACTIVE == eState)
	{
		if (nullptr == m_pCameraCom->Get_TargetObj())
		{
			if (FAILED(Find_Target()))
				return;
		}

		CTransform* pMacaronTransform = m_pCameraCom->Get_TargetObj()->Get_Transform();

		if (nullptr == pMacaronTransform) return;

		/* 포지션 세팅 (타겟의 x축 기준으로 +1, z축 기준으로 디스턴스만큼 떨어진다. */
		Vec4 vTargetPos		= pMacaronTransform->Get_FinalPosition();
		Vec4 vRelativePos	= Vec4{ vMacaron_Gimmick_Relative_Pos.x * -0.5f + 0.5f, 2.f, -m_pCameraCom->Get_Distance(), 0.f };
		vRelativePos		= pMacaronTransform->Get_RelativePosition(vRelativePos).ZeroW();


		m_pTransformCom->Clear();

		m_pTransformCom->Set_Position(vTargetPos + vRelativePos);

		m_pTransformCom->Set_Look(ENGINE_INSTANCE->Get_CurCamera_State(CTransform::STATE_LOOK).ZeroY().Normalized());

		m_pTransformCom->Rotate(m_pTransformCom->Get_State(CTransform::STATE_LOOK), DEG2RAD(-5.f));

		m_pCameraCom->Set_Fovy(CamFov_Follow_MacaronGimmick);
	}
	
}

HRESULT CCamera_Macaron_Gimmick_Wall::Ready_Components()
{
	/* Com_Transform */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* Com_Camera */
	CCamera::PROJ_DESC desc;
	{
		desc.fFovy = CamFov_Follow_MacaronGimmick;
		desc.fAspect = g_iWinSizeX / (_float)g_iWinSizeY;
		desc.fNear = 0.2f;
		desc.fFar = 1000.0f;
	}
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Camera"),
		TEXT("Com_Camera"), (CComponent**)&m_pCameraCom, &desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Macaron_Gimmick_Wall::Find_Target()
{
	CGameObject* pObject = CPlayerController::GetInstance()->Get_Player(PLAYER_TYPE::MACARON);

	if (nullptr == pObject) return E_FAIL;

	m_pCameraCom->Set_TargetObj(pObject);
	m_pCameraCom->Set_LookAtObj(pObject);

	return S_OK;
}


CCamera_Macaron_Gimmick_Wall* CCamera_Macaron_Gimmick_Wall::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Macaron_Gimmick_Wall* pInstance = new CCamera_Macaron_Gimmick_Wall(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Macaron_Gimmick_Wall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Macaron_Gimmick_Wall::Clone(void* pArg)
{
	CCamera_Macaron_Gimmick_Wall* pInstance = new CCamera_Macaron_Gimmick_Wall(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_Macaron_Gimmick_Wall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Macaron_Gimmick_Wall::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pCameraCom);

}
