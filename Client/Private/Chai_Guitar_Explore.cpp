#include "..\Default\stdafx.h"
#include "Chai_Guitar_Explore.h"
#include "EngineInstance.h"

#include "Util_File.h"
#include "Util_String.h"


CChai_Guitar_Explore::CChai_Guitar_Explore(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CWeapon(pDevice, pContext)
{
}

CChai_Guitar_Explore::CChai_Guitar_Explore(const CChai_Guitar_Explore& rhs)
	: CWeapon(rhs)
{
}

HRESULT CChai_Guitar_Explore::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CChai_Guitar_Explore::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CChai_Guitar_Explore::Tick(_float fTimeDelta)
{
	if (nullptr == m_pParent)
		return;

	__super::Tick(fTimeDelta);
}

void CChai_Guitar_Explore::LateTick(_float fTimeDelta)
{
	if (nullptr == m_pParent)
		return;

	__super::LateTick(fTimeDelta);
}

HRESULT CChai_Guitar_Explore::Render()
{
	if (nullptr == m_pParent)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CChai_Guitar_Explore::Ready_Components()
{
	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Model_Weapon_Chai_Guitar_Explore"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* Com_Collider_Sphere */
	CCollider::COLLIDERDESC		ColliderDesc{ Vec3(0, 0, -75), 30.f };
	{
		CCollider_Sphere* pCollider = nullptr;
		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
			TEXT("Com_Collider_Sphere"), (CComponent**)&pCollider, &ColliderDesc)))
			return E_FAIL;

		m_pColliderComs.push_back(pCollider);

		pCollider->Set_Active(FALSE);
	}

	return S_OK;
}

void CChai_Guitar_Explore::OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild)
{
	m_pParent->OnCollision_Enter(pCollider, m_iIndexAsChild);
}

void CChai_Guitar_Explore::OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild)
{
	m_pParent->OnCollision_Stay(pCollider, m_iIndexAsChild);
}

void CChai_Guitar_Explore::OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild)
{
	m_pParent->OnCollision_Stay(pCollider, m_iIndexAsChild);
}

CChai_Guitar_Explore* CChai_Guitar_Explore::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CChai_Guitar_Explore* pInstance = new CChai_Guitar_Explore(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CChai_Guitar_Explore");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CChai_Guitar_Explore::Clone(void* pArg)
{
	CChai_Guitar_Explore* pInstance = new CChai_Guitar_Explore(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CChai_Guitar_Explore");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChai_Guitar_Explore::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
}
