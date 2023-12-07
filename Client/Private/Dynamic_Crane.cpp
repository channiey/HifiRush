#include "..\Default\stdafx.h"
#include "Dynamic_Crane.h"

#include "EngineInstance.h"
#include "Animation.h"

#include "UiManager.h"
#include "Camera_Follow.h"
#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG
 
CDynamic_Crane::CDynamic_Crane(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CDynamic(pDevice, pContext)
{
}

CDynamic_Crane::CDynamic_Crane(const CDynamic_Crane& rhs)
	: CDynamic(rhs)
{
}

HRESULT CDynamic_Crane::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDynamic_Crane::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	CAnimation* pAnim = m_pModelCom->Get_Animation(m_AnimNames[PROGRESS_TYPE::IDLE_CRANE]);

	if (nullptr == pAnim) return E_FAIL;

	m_pModelCom->Set_Animation(pAnim, pAnim->Get_TickPerFrame(), DF_TW_TIME);

	m_eProgress = CDynamic_Crane::PROGRESS_TYPE::IDLE_CRANE;

	return S_OK;
}

void CDynamic_Crane::Tick(_double fTimeDelta)
{
	Check_Progress(fTimeDelta);

	__super::Tick(fTimeDelta);
}

void CDynamic_Crane::LateTick(_double fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CDynamic_Crane::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDynamic_Crane::Set_On()
{
	if (CDynamic_Crane::PROGRESS_TYPE::ACTIVE_CRANE == m_eProgress)
		return E_FAIL;

	m_bWait = TRUE;

	ENGINE_INSTANCE->Change_Camera(CAMERA_ID::CAM_PEPPERMINT_GIMMICK_CRANE, 1.f, LERP_MODE::SMOOTH_STEP);

	m_eProgress = CDynamic_Crane::PROGRESS_TYPE::WAIT_CRANE;

	//CUiManager::GetInstance()->On_Dialouge(1, L"역시 명중이야!");

	//ENGINE_INSTANCE->Play_Sound(SOUND_FILE_ID::EFC_PEPPERMINT_RESPAWN_05, CHANNEL_ID::ETC_OTHER_CALL, EfcVolumeChai);

	ENGINE_INSTANCE->Play_Sound(SOUND_FILE_ID::EFC_OBJ_CRANE_ALERT, CHANNEL_ID::ETC_OBJ, 0.4f);

	return S_OK;
}

HRESULT CDynamic_Crane::Set_Off()
{
	CAnimation* pAnim = m_pModelCom->Get_Animation(m_AnimNames[PROGRESS_TYPE::IDLE_CRANE]);

	if (nullptr == pAnim) return E_FAIL;

	m_pModelCom->Set_Animation(pAnim, pAnim->Get_TickPerFrame(), DF_TW_TIME);

	m_eProgress = CDynamic_Crane::PROGRESS_TYPE::IDLE_CRANE;

	m_fWaitAcc	= 0.f;

	m_bWait		= FALSE;

	return S_OK;
}

void CDynamic_Crane::Check_Progress(_double fTimeDelta)
{
	if (CDynamic_Crane::PROGRESS_TYPE::WAIT_CRANE == m_eProgress)
	{
		if (m_bWait)
		{
			m_fWaitAcc += fTimeDelta;
			if (m_fWaitTime <= m_fWaitAcc)
			{
				CAnimation* pAnim = m_pModelCom->Get_Animation(m_AnimNames[PROGRESS_TYPE::ACTIVE_CRANE]);

				if (nullptr == pAnim) return;

				m_pModelCom->Set_Animation(pAnim, pAnim->Get_TickPerFrame(), DF_TW_TIME);

				m_eProgress = CDynamic_Crane::PROGRESS_TYPE::ACTIVE_CRANE;

				m_bWait = FALSE;

				//CUiManager::GetInstance()->On_Dialouge(0, L"잠시만 이게 무슨 소리지?");
	

				ENGINE_INSTANCE->Play_Sound(SOUND_FILE_ID::EFC_OBJ_CRANE_MOVE_2, CHANNEL_ID::ETC_OBJ_SUB_1, 0.9f);
			}
		}
	}
	else if (CDynamic_Crane::PROGRESS_TYPE::ACTIVE_CRANE == m_eProgress)
	{
		if (!m_pModelCom->Is_Tween() && 60 == m_pModelCom->Get_TweenDesc().cur.iCurFrame)
		{
			ENGINE_INSTANCE->Play_Sound(SOUND_FILE_ID::EFC_OBJ_CRANE_STOP, CHANNEL_ID::ETC_OBJ_SUB_2, 0.8f);
		}
		else if (!m_pModelCom->Is_Tween() && 100 == m_pModelCom->Get_TweenDesc().cur.iCurFrame && !bSet)
		{
			ENGINE_INSTANCE->Change_Camera(CAMERA_ID::CAM_FOLLOW);
			CCamera_Follow* pCam = dynamic_cast<CCamera_Follow*>(ENGINE_INSTANCE->Get_Camera(CAMERA_ID::CAM_FOLLOW));
			if (nullptr != pCam)
				pCam->Reset();

			ENGINE_INSTANCE->Play_Sound(SOUND_FILE_ID::EFC_CHAI_AMAZE_WITH_CRANE, CHANNEL_ID::TALK_CHAI, 0.9f);
			CUiManager::GetInstance()->On_Dialouge(0, L"크레인이 움직일줄은 생각도 못했어");

			bSet = TRUE;
		}
		else if (!m_pModelCom->Is_Tween() && 135 == m_pModelCom->Get_TweenDesc().cur.iCurFrame)
		{
			CAnimation* pAnim = m_pModelCom->Get_Animation(m_AnimNames[PROGRESS_TYPE::ACTIVATED_IDLE_CRANE]);

			if (nullptr == pAnim) return;

			m_pModelCom->Set_Animation(pAnim, pAnim->Get_TickPerFrame(), DF_TW_TIME);

			m_eProgress = CDynamic_Crane::PROGRESS_TYPE::ACTIVATED_IDLE_CRANE;

			m_fWaitAcc = 0.f;

			m_bWait = FALSE;
		}
	}
}

HRESULT CDynamic_Crane::Ready_Components()
{
	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Model_Crane"),
		ComponentNames[COM_MODEL], (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CDynamic_Crane::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(ENGINE_INSTANCE->Bind_TransformToShader(m_pShaderCom, "g_ViewMatrix", CPipeLine::STATE_VIEW)))
		return E_FAIL;

	if (FAILED(ENGINE_INSTANCE->Bind_TransformToShader(m_pShaderCom, "g_ProjMatrix", CPipeLine::STATE_PROJ)))
		return E_FAIL;

	return S_OK;
}

CDynamic_Crane* CDynamic_Crane::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDynamic_Crane* pInstance = new CDynamic_Crane(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDynamic_Crane");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CDynamic_Crane* CDynamic_Crane::Clone(void* pArg)
{
	CDynamic_Crane* pInstance = new CDynamic_Crane(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CDynamic_Crane");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDynamic_Crane::Free()
{
	Set_Off();

	__super::Free();
}
