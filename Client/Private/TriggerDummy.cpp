#include "..\Default\stdafx.h"
#include "..\Public\TriggerDummy.h"
#include "EngineInstance.h"

#include "Util_String.h"

#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG

CTriggerDummy::CTriggerDummy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CTriggerDummy::CTriggerDummy(const CTriggerDummy& rhs)
	: CGameObject(rhs)
{
}

HRESULT CTriggerDummy::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTriggerDummy::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	memmove(&m_tDesc, pArg, sizeof(TRIGGER_DESC));
	{
		m_iIndexAsChild = m_tDesc.iIndexAsChild;
	}

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CTriggerDummy::Tick(_double fTimeDelta)
{
	__super::Tick(fTimeDelta);

	/* Update Colliders */
	if (nullptr != m_pCollider && m_pCollider->Is_Active())
		m_pCollider->Update(m_pTransformCom->Get_WorldMat());
}

void CTriggerDummy::LateTick(_double fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifdef _DEBUG
	if (nullptr != m_pCollider && m_pCollider->Is_Active() && CImGui_Manager::GetInstance()->Is_Render_Collider())
		m_pRendererCom->Add_DebugGroup(m_pCollider);
#endif // _DEBUG
}


HRESULT CTriggerDummy::Render()
{
	return S_OK;
}

HRESULT CTriggerDummy::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Renderer"),
		ComponentNames[COM_RENDERER], (CComponent**)&m_pRendererCom)))
		return E_FAIL;
	
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Transform"), 
		ComponentNames[COM_TRANSFORM], (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Collider */
	switch (m_tDesc.eColType)
	{
	case Engine::CCollider::AABB:
	{
		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Collider_AABB"),
			ComponentNames[COM_COLLIDER_AABB], (CComponent**)&m_pCollider, &m_tDesc.tColDesc)))
			return E_FAIL;
	}
		break;
	case Engine::CCollider::OBB:
	{
		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Collider_OBB"),
			ComponentNames[COM_COLLIDER_OBB], (CComponent**)&m_pCollider, &m_tDesc.tColDesc)))
			return E_FAIL;
	}
		break;
	case Engine::CCollider::SPHERE:
	{
		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
			ComponentNames[COM_COLLIDER_SPHERE], (CComponent**)&m_pCollider, &m_tDesc.tColDesc)))
			return E_FAIL;
	}
		break;
	default:
		break;
	}

	return S_OK;
}

CTriggerDummy* CTriggerDummy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTriggerDummy* pInstance = new CTriggerDummy(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CTriggerDummy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTriggerDummy::Clone(void* pArg)
{
	CTriggerDummy* pInstance = new CTriggerDummy(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CTriggerDummy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTriggerDummy::Free()
{
	__super::Free();

	Safe_Release(m_pCollider);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);

}
