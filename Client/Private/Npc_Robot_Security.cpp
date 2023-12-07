#include "..\Default\stdafx.h"
#include "..\Public\Npc_Robot_Security.h"

#include "EngineInstance.h"
#include "Animation.h"

#include "UiManager.h"
#include "PlayerController.h"


#include "ImGui_Manager.h"


CNpc_Robot_Security::CNpc_Robot_Security(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CNpc(pDevice, pContext)
{
}

CNpc_Robot_Security::CNpc_Robot_Security(const CNpc_Robot_Security& rhs)
	: CNpc(rhs)
{
}

HRESULT CNpc_Robot_Security::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CNpc_Robot_Security::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	CAnimation* pAnim = m_pModelCom->Get_Animation("np3050_npc-st07-n018_000");

	if (nullptr == pAnim) return E_FAIL;

	m_pModelCom->Set_Animation(pAnim, pAnim->Get_TickPerFrame() * 2.f, DF_TW_TIME, FALSE);

	return S_OK;
}

void CNpc_Robot_Security::Tick(_double fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CNpc_Robot_Security::LateTick(_double fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CNpc_Robot_Security::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CNpc_Robot_Security::Set_State(const OBJ_STATE& eState)
{
	__super::Set_State(eState);
}

HRESULT CNpc_Robot_Security::Ready_Components()
{
	/* Com_Model */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Model_Robot_Security"),
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

HRESULT CNpc_Robot_Security::Ready_Chilren()
{
	return S_OK;
}

HRESULT CNpc_Robot_Security::Ready_StateMachine()
{
	return S_OK;
}


void CNpc_Robot_Security::OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild)
{
	
}

void CNpc_Robot_Security::OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild)
{
	CGameObject* pGameObject = pCollider->Get_Owner();

	if (L"Player_Chai_000" == pGameObject->Get_Name())
	{
		if (!m_bSetDialouge && ENGINE_INSTANCE->Key_Down('M'))
		{
			m_bSetDialouge = TRUE;

			CUiManager::GetInstance()->On_Dialouge(11, L"소장님이 오기 전에 얼른 작업 계획을 마련해야지");
			for (auto& pCollider : m_pColliderComs)
				if (nullptr != pCollider)
					pCollider->Set_Active(FALSE);
		}
	}
}

void CNpc_Robot_Security::OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild)
{
}

CNpc_Robot_Security* CNpc_Robot_Security::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CNpc_Robot_Security* pInstance = new CNpc_Robot_Security(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CNpc_Robot_Security");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CNpc_Robot_Security* CNpc_Robot_Security::Clone(void* pArg)
{
	CNpc_Robot_Security* pInstance = new CNpc_Robot_Security(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CNpc_Robot_Security");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNpc_Robot_Security::Free()
{
	__super::Free();
}
