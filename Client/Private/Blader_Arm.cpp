#include "..\Default\stdafx.h"
#include "Blader_Arm.h"
#include "EngineInstance.h"

#include "Util_File.h"
#include "Util_String.h"


CBlader_Arm::CBlader_Arm(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CWeapon(pDevice, pContext)
{
}

CBlader_Arm::CBlader_Arm(const CBlader_Arm& rhs)
	: CWeapon(rhs)
{
}

HRESULT CBlader_Arm::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBlader_Arm::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CBlader_Arm::Tick(_double fTimeDelta)
{
	if (nullptr == m_pParent)
		return;

	__super::Tick(fTimeDelta);
}

void CBlader_Arm::LateTick(_double fTimeDelta)
{
	if (nullptr == m_pParent)
		return;

	__super::LateTick(fTimeDelta);
}

HRESULT CBlader_Arm::Render()
{
	if (nullptr == m_pParent)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBlader_Arm::Ready_Components()
{
	/* Com_Collider_Sphere */
	CCollider::COLLIDERDESC		ColliderDesc{ Vec3(0, 0, 0), 110.f };
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

void CBlader_Arm::OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild)
{
	m_pParent->OnCollision_Enter(pCollider, m_iIndexAsChild);
}

void CBlader_Arm::OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild)
{
	m_pParent->OnCollision_Stay(pCollider, m_iIndexAsChild);
}

void CBlader_Arm::OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild)
{
	m_pParent->OnCollision_Stay(pCollider, m_iIndexAsChild);
}

CBlader_Arm* CBlader_Arm::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBlader_Arm* pInstance = new CBlader_Arm(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CBlader_Arm");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBlader_Arm::Clone(void* pArg)
{
	CBlader_Arm* pInstance = new CBlader_Arm(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CBlader_Arm");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBlader_Arm::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
}
