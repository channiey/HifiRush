#include "..\Default\stdafx.h"
#include "..\Public\Camera_Follow.h"

#include "EngineInstance.h"
#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG

#include "Character.h"

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

	if (FAILED(ENGINE_INSTANCE->Add_Camera(CAM_FOLLOW, this)))
		return E_FAIL;

	/* Set Camera */
	{
		m_pCameraCom->Set_MouseSensitiveX(0.3f);
		m_pCameraCom->Set_MouseSensitiveY(0.1f);
		m_pCameraCom->Set_Distance(CamDist_Follow_Default);
		m_pCameraCom->Set_LookAtOffSet(Vec4{0.f, 1.8f, 0.f, 1.f });
	}

	m_eState = CGameObject::STATE_ACTIVE;

	return S_OK;
}

void CCamera_Follow::Tick(_double fTimeDelta)
{	
	if (nullptr == m_pCameraCom->Get_TargetObj())
	{
		if (FAILED(Find_Target()))
			return;
	}

	if (!m_pCameraCom->Is_TargetObj() || !m_pCameraCom->Is_LookAtObj())
		return;


	m_pCameraCom->Update(fTimeDelta);

	if (!CImGui_Manager::GetInstance()->Is_ClickedWindow() && m_bCanMove && !m_pCameraCom->Is_Shake())
		Move(fTimeDelta);

	__super::Tick(fTimeDelta);
}

void CCamera_Follow::LateTick(_double fTimeDelta)
{
	if (!m_pCameraCom->Is_TargetObj() || !m_pCameraCom->Is_LookAtObj())
		return;

	__super::LateTick(fTimeDelta);
}

void CCamera_Follow::Reset()
{
	CTransform* pTargetTransform = m_pCameraCom->Get_TargetObj()->Get_Transform();

	if (nullptr == pTargetTransform) return;

	Vec4 vTargetPos = pTargetTransform->Get_FinalPosition();
	Vec4 vRelativePos = pTargetTransform->Get_RelativePosition(Vec4{ -0.f, 0.f, -m_pCameraCom->Get_Distance(), 0.f });

	/* 포지션 세팅 */
	m_pTransformCom->Set_Position(vTargetPos + vRelativePos);

	/* 룩 세팅 */
	m_pTransformCom->LookAt(Calculate_Look());
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

HRESULT CCamera_Follow::Find_Target()
{
	CGameObject* pObject = ENGINE_INSTANCE->Get_GameObject(ENGINE_INSTANCE->Get_CurLevelIndex(), LayerNames[LAYER_PLAYER], L"Player_Chai_000");

	if (nullptr == pObject) return E_FAIL;

	m_pCameraCom->Set_TargetObj(pObject);
	m_pCameraCom->Set_LookAtObj(pObject);

	Reset();
	return S_OK;
}

void CCamera_Follow::Move(const _double& fTimeDelta)
{
	m_pTransformCom->Set_Position(Calculate_Position(fTimeDelta));
	m_pTransformCom->LookAt(Calculate_Look());
}

const Vec4 CCamera_Follow::Calculate_Position(const _double& fTimeDelta)
{
	_long	MouseMove = 0l;

	if (MouseMove = ENGINE_INSTANCE->Get_DIMMoveState(CInput_Device::MMS_X))
		m_vAngle.x += MouseMove * m_pCameraCom->Get_MouseSensitiveX() * fTimeDelta * -1.f;

	if (MouseMove = ENGINE_INSTANCE->Get_DIMMoveState(CInput_Device::MMS_Y))
	{
		m_vAngle.y += MouseMove * m_pCameraCom->Get_MouseSensitiveY() * fTimeDelta;

		if (m_vAngle.y <= 0.01f)//m_fMinAngleY) // 0.01f
		{
			m_vAngle.y = 0.01f;// m_fMinAngleY;
		}
		else if (3.13f < m_vAngle.y)//m_vAngle.y) // 3.13f
		{
			m_vAngle.y = 3.13f;// m_fMaxAngleY;
		}

		/* 임시 강제 고정 */
		//m_vAngle.y = 0.9f;
	}

	/* 구면 좌표계(극좌표계) -> 왼손 직교 좌표계 */
	Vec4 vCamLocal;
	{
		vCamLocal.x = m_pCameraCom->Get_Distance() * sinf(m_vAngle.y) * cosf(m_vAngle.x);	// x = r * sin(위도 앙각) * cos(경도 방위각)
		vCamLocal.y = m_pCameraCom->Get_Distance() * cosf(m_vAngle.y);						// y = r * cos(위도 앙각)
		vCamLocal.z = m_pCameraCom->Get_Distance() * sinf(m_vAngle.y) * sinf(m_vAngle.x);	// z = r * sin(위도 앙각) * sin(경도 방위각)
		vCamLocal.w = 0.f;
	}

	return vCamLocal + m_pCameraCom->Get_TargetObj()->Get_Transform()->Get_FinalPosition();
}

const Vec4 CCamera_Follow::Calculate_Look()
{
	return Vec4(m_pCameraCom->Get_TargetObj()->Get_Transform()->Get_FinalPosition() + m_pCameraCom->Get_LookAtOffSet()).OneW();
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
}
