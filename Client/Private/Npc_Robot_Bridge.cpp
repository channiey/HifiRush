#include "..\Default\stdafx.h"
#include "..\Public\Npc_Robot_Bridge.h"

#include "EngineInstance.h"
#include "Animation.h"

#include "UiManager.h"
#include "PlayerController.h"


#include "ImGui_Manager.h"


CNpc_Robot_Bridge::CNpc_Robot_Bridge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CNpc(pDevice, pContext)
{
}

CNpc_Robot_Bridge::CNpc_Robot_Bridge(const CNpc_Robot_Bridge& rhs)
	: CNpc(rhs)
{
}

HRESULT CNpc_Robot_Bridge::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CNpc_Robot_Bridge::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	CAnimation* pAnim = m_pModelCom->Get_Animation("np0500_idle_000");

	if (nullptr == pAnim) return E_FAIL;

	m_pModelCom->Set_Animation(pAnim, pAnim->Get_TickPerFrame() * 2.f, DF_TW_TIME, FALSE);

	return S_OK;
}

void CNpc_Robot_Bridge::Tick(_double fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CNpc_Robot_Bridge::LateTick(_double fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CNpc_Robot_Bridge::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CNpc_Robot_Bridge::Set_State(const OBJ_STATE& eState)
{
	__super::Set_State(eState);
}

HRESULT CNpc_Robot_Bridge::Ready_Components()
{
	/* Com_Model */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Model_Robot_Bridge"),
		ComponentNames[COM_MODEL], (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* Com_Collider */
	CCollider_Sphere* pCollider = nullptr;
	{
		CCollider::COLLIDERDESC	ColliderDesc(Vec3{ 0.f, 3.f, 0.f }, 1.5f);

		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
			ComponentNames[COM_COLLIDER_SPHERE], (CComponent**)&pCollider, &ColliderDesc)))
			return E_FAIL;

		m_pColliderComs.push_back(pCollider);
	}

	return S_OK;
}

HRESULT CNpc_Robot_Bridge::Ready_Chilren()
{
	return S_OK;
}

HRESULT CNpc_Robot_Bridge::Ready_StateMachine()
{
	return S_OK;
}


void CNpc_Robot_Bridge::OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild)
{
	__super::OnCollision_Enter(pCollider, iIndexAsChild);


	/*for (auto& pCollider : m_pColliderComs)
		if (nullptr != pCollider)
			pCollider->Set_Active(FALSE);*/
}

void CNpc_Robot_Bridge::OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild)
{
}

void CNpc_Robot_Bridge::OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild)
{
}

CNpc_Robot_Bridge* CNpc_Robot_Bridge::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CNpc_Robot_Bridge* pInstance = new CNpc_Robot_Bridge(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CNpc_Robot_Bridge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CNpc_Robot_Bridge* CNpc_Robot_Bridge::Clone(void* pArg)
{
	CNpc_Robot_Bridge* pInstance = new CNpc_Robot_Bridge(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CNpc_Robot_Bridge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNpc_Robot_Bridge::Free()
{
	__super::Free();
}
