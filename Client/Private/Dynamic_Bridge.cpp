#include "..\Default\stdafx.h"
#include "Dynamic_Bridge.h"

#include "EngineInstance.h"
#include "Animation.h"

#include "Character.h"
#include "Projectile.h"

#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG

#include "UiManager.h"

CDynamic_Bridge::CDynamic_Bridge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CDynamic(pDevice, pContext)
{
}

CDynamic_Bridge::CDynamic_Bridge(const CDynamic_Bridge& rhs)
	: CDynamic(rhs)
{
}

HRESULT CDynamic_Bridge::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDynamic_Bridge::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	CAnimation* pAnim = m_pModelCom->Get_Animation(m_strAnimName);

	if (nullptr == pAnim) return E_FAIL;

	m_pModelCom->Set_Animation(pAnim, pAnim->Get_TickPerFrame(), DF_TW_TIME);

	m_pModelCom->Stop_Animation(TRUE);

	return S_OK;
}

void CDynamic_Bridge::Tick(_double fTimeDelta)
{
	if (m_iCol == 2 && !m_bStart)
	{
		m_fAcc += fTimeDelta;
		if (3.f <= m_fAcc)
		{
			Set_On();
			m_bStart = TRUE;

		}
	}
	__super::Tick(fTimeDelta);

	if(!m_pModelCom->Is_StopAnimation())
		Update_Sound();
}


void CDynamic_Bridge::LateTick(_double fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (!m_pModelCom->Is_Tween() && 100 == m_pModelCom->Get_TweenDesc().cur.iCurFrame && !bSet)
	{
		m_pModelCom->Stop_Animation(TRUE);
		CUiManager::GetInstance()->On_Dialouge(0, L"오? 신기한데?");
		ENGINE_INSTANCE->Play_Sound(SOUND_FILE_ID::EFC_CHAI_AMAZE_WITH_CRANE, CHANNEL_ID::TALK_CHAI, 0.9f);

		bSet = TRUE;
	}
}

HRESULT CDynamic_Bridge::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CDynamic_Bridge::OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild)
{
	if (L"Player_Chai_000" == pCollider->Get_Owner()->Get_Name())
	{
		++m_iCol;

		if (1 == m_iCol)
		{
			ENGINE_INSTANCE->Get_CurCamera()->Get_Camera()->Lerp_Dist(CamDist_Follow_NPC_TALK, 0.7f, LERP_MODE::SMOOTHER_STEP);
			CUiManager::GetInstance()->On_Dialouge(14, L"다리를 지나가려면 옆 빌딩의 적들을 해치워야 합니다!");
			CUiManager::GetInstance()->On_Dialouge(0, L"이런, 그냥 지나갈 수가 없군");
		}
		else if (2 == m_iCol)
		{
			ENGINE_INSTANCE->Get_CurCamera()->Get_Camera()->Lerp_Dist(CamDist_Follow_NPC_TALK, 0.7f, LERP_MODE::SMOOTHER_STEP);

			CUiManager::GetInstance()->On_Dialouge(14, L"고생하셨습니다 다리를 열어드리겠습니다!");
			CUiManager::GetInstance()->On_Dialouge(0, L"오케이 좋았어!");

			//m_pCollider->Set_Active(FALSE);

			CGameObject* pNpc = ENGINE_INSTANCE->Get_GameObject_InCurLevel(LayerNames[LAYER_ID::LAYER_NPC], L"Npc_Robot_Joy");
			if (nullptr != pNpc)
				pNpc->Set_State(CGameObject::STATE_UNACTIVE);

			pNpc = nullptr;
			pNpc = ENGINE_INSTANCE->Get_GameObject_InCurLevel(LayerNames[LAYER_ID::LAYER_NPC], L"Npc_Robot_Clean");
			if (nullptr != pNpc)
				pNpc->Set_State(CGameObject::STATE_UNACTIVE);

		}

		//Set_On();
	}
}

void CDynamic_Bridge::OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild)
{
}

void CDynamic_Bridge::OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild)
{
	ENGINE_INSTANCE->Get_CurCamera()->Get_Camera()->Lerp_Dist(CamDist_Follow_Default, 0.7f, LERP_MODE::SMOOTHER_STEP);

	if (2 == m_iCol)
	{
		m_pCollider->Set_Active(FALSE);
	}
}

HRESULT CDynamic_Bridge::Set_On()
{
	m_pModelCom->Stop_Animation(FALSE);

	return S_OK;
}

HRESULT CDynamic_Bridge::Set_Off()
{
	m_pModelCom->Clear_Animation();

	CAnimation* pAnim = m_pModelCom->Get_Animation(m_strAnimName);

	if (nullptr == pAnim) return E_FAIL;

	m_pModelCom->Set_Animation(pAnim, pAnim->Get_TickPerFrame(), DF_TW_TIME);

	m_pModelCom->Stop_Animation(TRUE);

	m_bPlaySound = FALSE;

	return S_OK;
}

void CDynamic_Bridge::Check_Progress(_double fTimeDelta)
{
}

HRESULT CDynamic_Bridge::Ready_Components()
{
	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Model_Dynamic_Bridge"),
		ComponentNames[COM_MODEL], (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* Com_Collider */
	CCollider::COLLIDERDESC	ColliderDesc(Vec3{ 0.f, 6.f, 0.f }, 6.f);
	{
		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
			ComponentNames[COM_COLLIDER_SPHERE], (CComponent**)&m_pCollider, &ColliderDesc)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CDynamic_Bridge::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(ENGINE_INSTANCE->Bind_TransformToShader(m_pShaderCom, "g_ViewMatrix", CPipeLine::STATE_VIEW)))
		return E_FAIL;

	if (FAILED(ENGINE_INSTANCE->Bind_TransformToShader(m_pShaderCom, "g_ProjMatrix", CPipeLine::STATE_PROJ)))
		return E_FAIL;

	return S_OK;
}

void CDynamic_Bridge::Update_Sound()
{
	const _int iCurFrame = m_pModelCom->Get_TweenDesc().cur.iCurFrame;

	if (!m_bPlaySound && (0 == iCurFrame || 14 == iCurFrame))
	{
		ENGINE_INSTANCE->Play_Sound(SOUND_FILE_ID::EFC_OBJ_BRIDGE_FRAME_DOWN, CHANNEL_ID::ETC_OBJ, 0.9f);
		m_bPlaySound = TRUE;
	}
	else if (!m_bPlaySound && (27 == iCurFrame))
	{
		ENGINE_INSTANCE->Play_Sound(SOUND_FILE_ID::EFC_OBJ_BRIDGE_CREATE_BODY, CHANNEL_ID::ETC_OBJ, 0.9f);
		m_bPlaySound = TRUE;
	}
	else if (!m_bPlaySound && (56 == iCurFrame || 62 == iCurFrame || 66 == iCurFrame || 69 == iCurFrame || 72 == iCurFrame || 76 == iCurFrame || 79 == iCurFrame || 82 == iCurFrame || 85 == iCurFrame || 89 == iCurFrame || 93 == iCurFrame))
	{
		ENGINE_INSTANCE->Play_Sound(SOUND_FILE_ID::EFC_OBJ_BRIDGE_DOWN_BODY, CHANNEL_ID::ETC_OBJ, 0.7f);
		m_bPlaySound = TRUE;
	}
	else
		m_bPlaySound = FALSE;
}

CDynamic_Bridge* CDynamic_Bridge::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDynamic_Bridge* pInstance = new CDynamic_Bridge(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDynamic_Bridge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CDynamic_Bridge* CDynamic_Bridge::Clone(void* pArg)
{
	CDynamic_Bridge* pInstance = new CDynamic_Bridge(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CDynamic_Bridge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDynamic_Bridge::Free()
{
	__super::Free();
}
