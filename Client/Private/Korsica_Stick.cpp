#include "..\Default\stdafx.h"
#include "..\Public\Korsica_Stick.h"
#include "EngineInstance.h"

#include "Util_File.h"
#include "Util_String.h"


CKorsica_Stick::CKorsica_Stick(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CWeapon(pDevice, pContext)
{
}

CKorsica_Stick::CKorsica_Stick(const CKorsica_Stick& rhs)
	: CWeapon(rhs)
{
}

HRESULT CKorsica_Stick::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CKorsica_Stick::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CKorsica_Stick::Tick(_double fTimeDelta)
{
	if (nullptr == m_pParent)
		return;

	__super::Tick(fTimeDelta);
}

void CKorsica_Stick::LateTick(_double fTimeDelta)
{
	if (nullptr == m_pParent)
		return;

	__super::LateTick(fTimeDelta);
}

HRESULT CKorsica_Stick::Render()
{
	if (nullptr == m_pParent)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CKorsica_Stick::Ready_Components()
{
	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Model_Weapon_Korsica_Stick"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* Com_Collider_Sphere */
	/*CCollider::COLLIDERDESC		ColliderDesc{ Vec3(0, 0, -75), 30.f };
	{
		CCollider_Sphere* pCollider = nullptr;
		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
			TEXT("Com_Collider_Sphere"), (CComponent**)&pCollider, &ColliderDesc)))
			return E_FAIL;

		m_pColliderComs.push_back(pCollider);

		pCollider->Set_Active(FALSE);
	}*/

	return S_OK;
}

void CKorsica_Stick::OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild)
{
	m_pParent->OnCollision_Enter(pCollider, m_iIndexAsChild);
}

void CKorsica_Stick::OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild)
{
	m_pParent->OnCollision_Stay(pCollider, m_iIndexAsChild);
}

void CKorsica_Stick::OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild)
{
	m_pParent->OnCollision_Stay(pCollider, m_iIndexAsChild);
}

CKorsica_Stick* CKorsica_Stick::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CKorsica_Stick* pInstance = new CKorsica_Stick(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CKorsica_Stick");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CKorsica_Stick::Clone(void* pArg)
{
	CKorsica_Stick* pInstance = new CKorsica_Stick(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CKorsica_Stick");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CKorsica_Stick::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
}
