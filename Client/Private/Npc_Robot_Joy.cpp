#include "..\Default\stdafx.h"
#include "..\Public\Npc_Robot_Joy.h"

#include "EngineInstance.h"
#include "Animation.h"

#include "UiManager.h"
#include "PlayerController.h"


#include "ImGui_Manager.h"


CNpc_Robot_Joy::CNpc_Robot_Joy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CNpc(pDevice, pContext)
{
}

CNpc_Robot_Joy::CNpc_Robot_Joy(const CNpc_Robot_Joy& rhs)
	: CNpc(rhs)
{
}

HRESULT CNpc_Robot_Joy::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CNpc_Robot_Joy::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	CAnimation* pAnim = m_pModelCom->Get_Animation("np3000_npc-general_008");

	if (nullptr == pAnim) return E_FAIL;

	m_pModelCom->Set_Animation(pAnim, pAnim->Get_TickPerFrame() * 1.5f, DF_TW_TIME, FALSE);

	return S_OK;
}

void CNpc_Robot_Joy::Tick(_double fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CNpc_Robot_Joy::LateTick(_double fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CNpc_Robot_Joy::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CNpc_Robot_Joy::Set_State(const OBJ_STATE& eState)
{
	__super::Set_State(eState);
}

HRESULT CNpc_Robot_Joy::Ready_Components()
{
	/* Com_Model */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Model_Robot_Joy"),
		ComponentNames[COM_MODEL], (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* Com_Collider */
	CCollider_Sphere* pCollider = nullptr;
	{
		CCollider::COLLIDERDESC	ColliderDesc(Vec3{ 0.f, 4.f, 0.f }, 4.f);

		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
			ComponentNames[COM_COLLIDER_SPHERE], (CComponent**)&pCollider, &ColliderDesc)))
			return E_FAIL;

		m_pColliderComs.push_back(pCollider);
	}

	return S_OK;
}

HRESULT CNpc_Robot_Joy::Ready_Chilren()
{
	return S_OK;
}

HRESULT CNpc_Robot_Joy::Ready_StateMachine()
{
	return S_OK;
}


void CNpc_Robot_Joy::OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild)
{
	
}

void CNpc_Robot_Joy::OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild)
{
	CGameObject* pGameObject = pCollider->Get_Owner();

	if (L"Player_Chai_000" == pGameObject->Get_Name())
	{
		if (!m_bSetDialouge && ENGINE_INSTANCE->Key_Down('M'))
		{
			m_bSetDialouge = TRUE;

			CUiManager::GetInstance()->On_Dialouge(13, L"5ºÐ¸¸ ´õ ½¯°Ô.. ¸øº»Ã´ ÇØÁà");
			CUiManager::GetInstance()->On_Dialouge(0, L"(·Îº¿µµ ÈûµêÀ» ´À³¥ ¼ö ÀÖ´Â°Ç°¡..?)");

			for (auto& pCollider : m_pColliderComs)
				if (nullptr != pCollider)
					pCollider->Set_Active(FALSE);
		}
	}
}

void CNpc_Robot_Joy::OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild)
{
}

CNpc_Robot_Joy* CNpc_Robot_Joy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CNpc_Robot_Joy* pInstance = new CNpc_Robot_Joy(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CNpc_Robot_Joy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CNpc_Robot_Joy* CNpc_Robot_Joy::Clone(void* pArg)
{
	CNpc_Robot_Joy* pInstance = new CNpc_Robot_Joy(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CNpc_Robot_Joy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNpc_Robot_Joy::Free()
{
	__super::Free();
}
