#include "..\Default\stdafx.h"
#include "..\Public\Peppermint_Gun.h"
#include "EngineInstance.h"

#include "Util_File.h"
#include "Util_String.h"


CPeppermint_Gun::CPeppermint_Gun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CWeapon(pDevice, pContext)
{
}

CPeppermint_Gun::CPeppermint_Gun(const CPeppermint_Gun& rhs)
	: CWeapon(rhs)
{
}

HRESULT CPeppermint_Gun::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPeppermint_Gun::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CPeppermint_Gun::Tick(_double fTimeDelta)
{
	if (nullptr == m_pParent)
		return;

	__super::Tick(fTimeDelta);
}

void CPeppermint_Gun::LateTick(_double fTimeDelta)
{
	if (nullptr == m_pParent)
		return;

	__super::LateTick(fTimeDelta);
}

HRESULT CPeppermint_Gun::Render()
{
	if (nullptr == m_pParent)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPeppermint_Gun::Ready_Components()
{
	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Model_Weapon_Peppermint_Gun"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

void CPeppermint_Gun::OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild)
{
	m_pParent->OnCollision_Enter(pCollider, m_iIndexAsChild);
}

void CPeppermint_Gun::OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild)
{
	m_pParent->OnCollision_Stay(pCollider, m_iIndexAsChild);
}

void CPeppermint_Gun::OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild)
{
	m_pParent->OnCollision_Stay(pCollider, m_iIndexAsChild);
}

CPeppermint_Gun* CPeppermint_Gun::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPeppermint_Gun* pInstance = new CPeppermint_Gun(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CPeppermint_Gun");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPeppermint_Gun::Clone(void* pArg)
{
	CPeppermint_Gun* pInstance = new CPeppermint_Gun(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CPeppermint_Gun");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPeppermint_Gun::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
}
