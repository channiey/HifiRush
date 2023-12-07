#include "..\Default\stdafx.h"
#include "Dynamic_Switch.h"

#include "EngineInstance.h"
#include "Animation.h"

#include "Character.h"
#include "Projectile.h"

#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG

#include "UiManager.h"
CDynamic_Switch::CDynamic_Switch(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CDynamic(pDevice, pContext)
{
}

CDynamic_Switch::CDynamic_Switch(const CDynamic_Switch& rhs)
	: CDynamic(rhs)
{
}

HRESULT CDynamic_Switch::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDynamic_Switch::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	CAnimation* pAnim = m_pModelCom->Get_Animation(m_AnimNames[CDynamic_Switch::STATE_TYPE::OPEN]);

	if (nullptr == pAnim) return E_FAIL;

	m_pModelCom->Set_Animation(pAnim, pAnim->Get_TickPerFrame(), DF_TW_TIME);

	m_eStateType = CDynamic_Switch::STATE_TYPE::OPEN;

	return S_OK;
}

void CDynamic_Switch::Tick(_double fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bShot)
	{
		m_fTimeAcc += fTimeDelta;
		if (m_fTimeLimit <= m_fTimeAcc)
		{
			m_bShot = FALSE;
			m_pTarget->Set_On();
		}
	}
}

void CDynamic_Switch::LateTick(_double fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_Progress(fTimeDelta);
}

HRESULT CDynamic_Switch::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CDynamic_Switch::OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild)
{
	if (LayerNames[LAYER_PROJECTILE] == pCollider->Get_Owner()->Get_LayerTag())
	{
		CProjectile* pProjectile = dynamic_cast<CProjectile*>(pCollider->Get_Owner());
		if (nullptr != pProjectile)
		{
			if (L"Player_Peppermint_000" == pProjectile->Get_Owner()->Get_Name())
			{
				CGameObject* pTarget = ENGINE_INSTANCE->Get_GameObject_InCurLevel(LayerNames[LAYER_ID::LAYER_ENV_INTERACTALBE], L"Env_Dynamic_Crane_000");
				if (nullptr != pTarget)
				{
					CDynamic* pDynamic = dynamic_cast<CDynamic*>(pTarget);
					if (nullptr != pDynamic)
					{
						m_pTarget	= pDynamic;
						m_bShot		= TRUE;

						m_eStateType = CDynamic_Switch::STATE_TYPE::CLOSING;

						CAnimation* pAnim = m_pModelCom->Get_Animation(m_AnimNames[CDynamic_Switch::STATE_TYPE::CLOSING]);

						if (nullptr == pAnim) return;

						m_pModelCom->Set_Animation(pAnim, pAnim->Get_TickPerFrame(), DF_TW_TIME);

						ENGINE_INSTANCE->Play_Sound(SOUND_FILE_ID::EFC_OBJ_SWITCH_SHOT, CHANNEL_ID::ETC_OBJ_SUB_1, 0.6f);
						
						CUiManager::GetInstance()->On_Dialouge(1, L"좋아 명중이야!");
						//CUiManager::GetInstance()->On_Dialouge(0, L"그걸 맞추다니 페퍼민트 대단한걸?");

					}
				}
			}
		}
	}
}

void CDynamic_Switch::OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild)
{
}

void CDynamic_Switch::OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild)
{
}

HRESULT CDynamic_Switch::Set_On()
{
	return S_OK;
}

HRESULT CDynamic_Switch::Set_Off()
{
	m_bShot = FALSE;

	m_fTimeAcc = 0.f;
	m_fTimeLimit = 0.f;

	m_eStateType = STATE_TYPE::OPEN;

	return S_OK;
}

void CDynamic_Switch::Check_Progress(_double fTimeDelta)
{
	if (m_pModelCom->Is_Tween())
		return;

	const string strCurAnimName = m_pModelCom->Get_CurAnimation()->Get_Name();

	if (CDynamic_Switch::STATE_TYPE::CLOSING == m_eStateType && m_AnimNames[CDynamic_Switch::STATE_TYPE::CLOSING] == strCurAnimName &&
		16 == m_pModelCom->Get_CurAnimationFrame())
	{
		m_pModelCom->Stop_Animation(TRUE);
		/*m_pModelCom->Clear_Animation();

		CAnimation* pAnim = m_pModelCom->Get_Animation(m_AnimNames[CDynamic_Switch::STATE_TYPE::CLOSED]);

		if (nullptr == pAnim) return;

		m_pModelCom->Set_Animation(pAnim, pAnim->Get_TickPerFrame(), DF_TW_TIME);*/

		ENGINE_INSTANCE->Play_Sound(SOUND_FILE_ID::EFC_OBJ_SWITCH_SHOT_CLOSED, CHANNEL_ID::ETC_OBJ_SUB_1, 0.6f);

		m_eStateType = CDynamic_Switch::STATE_TYPE::CLOSED;
	}
}

HRESULT CDynamic_Switch::Ready_Components()
{
	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Model_Switch"),
		ComponentNames[COM_MODEL], (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* Com_Collider */
	CCollider::COLLIDERDESC	ColliderDesc(Vec3{ 0.f, 0.f, 0.f }, 1.f);
	{
		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
			ComponentNames[COM_COLLIDER_SPHERE], (CComponent**)&m_pCollider, &ColliderDesc)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CDynamic_Switch::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(ENGINE_INSTANCE->Bind_TransformToShader(m_pShaderCom, "g_ViewMatrix", CPipeLine::STATE_VIEW)))
		return E_FAIL;

	if (FAILED(ENGINE_INSTANCE->Bind_TransformToShader(m_pShaderCom, "g_ProjMatrix", CPipeLine::STATE_PROJ)))
		return E_FAIL;

	return S_OK;
}

CDynamic_Switch* CDynamic_Switch::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDynamic_Switch* pInstance = new CDynamic_Switch(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDynamic_Switch");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CDynamic_Switch* CDynamic_Switch::Clone(void* pArg)
{
	CDynamic_Switch* pInstance = new CDynamic_Switch(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CDynamic_Switch");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDynamic_Switch::Free()
{
	__super::Free();
}
