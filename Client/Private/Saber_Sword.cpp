#include "..\Default\stdafx.h"
#include "Saber_Sword.h"
#include "EngineInstance.h"

#include "Util_File.h"
#include "Util_String.h"


CSaber_Sword::CSaber_Sword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CWeapon(pDevice, pContext)
{
}

CSaber_Sword::CSaber_Sword(const CSaber_Sword& rhs)
	: CWeapon(rhs)
{
}

HRESULT CSaber_Sword::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSaber_Sword::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CSaber_Sword::Tick(_double fTimeDelta)
{
	if (nullptr == m_pParent)
		return;

	__super::Tick(fTimeDelta);
}

void CSaber_Sword::LateTick(_double fTimeDelta)
{
	if (nullptr == m_pParent)
		return;

	__super::LateTick(fTimeDelta);
}

HRESULT CSaber_Sword::Render()
{
	if (nullptr == m_pParent)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSaber_Sword::Ready_Components()
{
	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Model_Weapon_Saber_Sword"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* Com_Collider_Sphere */
	CCollider::COLLIDERDESC		ColliderDesc{ Vec3(0, 0, 90), 40.f };
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

void CSaber_Sword::OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild)
{
	m_pParent->OnCollision_Enter(pCollider, m_iIndexAsChild);
}

void CSaber_Sword::OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild)
{
	m_pParent->OnCollision_Stay(pCollider, m_iIndexAsChild);
}

void CSaber_Sword::OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild)
{
	m_pParent->OnCollision_Stay(pCollider, m_iIndexAsChild);
}

CSaber_Sword* CSaber_Sword::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSaber_Sword* pInstance = new CSaber_Sword(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CSaber_Sword");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSaber_Sword::Clone(void* pArg)
{
	CSaber_Sword* pInstance = new CSaber_Sword(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CSaber_Sword");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSaber_Sword::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
}
