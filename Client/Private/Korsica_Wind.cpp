#include "..\Default\stdafx.h"
#include "Korsica_Wind.h"

#include "EngineInstance.h"

#include "Korsica.h"

CKorsica_Wind::CKorsica_Wind(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CProjectile(pDevice, pContext)
{
}

CKorsica_Wind::CKorsica_Wind(const CKorsica_Wind& rhs)
	: CProjectile(rhs)
{
}

HRESULT CKorsica_Wind::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CKorsica_Wind::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Scale(Vec3{ 1.5f, 1.5f, 1.5f });

	return S_OK;
}

void CKorsica_Wind::Tick(_double fTimeDelta)
{
	if (nullptr == m_tDesc.pOwner) return;

	__super::Tick(fTimeDelta);
}

void CKorsica_Wind::LateTick(_double fTimeDelta)
{
	if (nullptr == m_tDesc.pOwner) return;

	__super::LateTick(fTimeDelta);
}

HRESULT CKorsica_Wind::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CKorsica_Wind::OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild)
{
	if (nullptr == m_tDesc.pOwner) return;

	if (LayerNames[LAYER_ID::LAYER_ENEMY] == pCollider->Get_Owner()->Get_LayerTag())
	{
		m_tDesc.pOwner->OnCollision_Enter(pCollider, CKorsica::CHILD_TYPE::PROJECTILE);
	}
}

void CKorsica_Wind::OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild)
{
	if (nullptr == m_tDesc.pOwner) return;

	m_tDesc.pOwner->OnCollision_Stay(pCollider, CKorsica::CHILD_TYPE::PROJECTILE);
}

void CKorsica_Wind::OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild)
{
	if (nullptr == m_tDesc.pOwner) return;

	m_tDesc.pOwner->OnCollision_Exit(pCollider, ::CKorsica::PROJECTILE);
}

HRESULT CKorsica_Wind::Shoot(PROJECTILE_DESC tDesc)
{
	memcpy(&m_tDesc, &tDesc, sizeof(PROJECTILE_DESC));

	if (nullptr == m_tDesc.pOwner)
		return E_FAIL;

	m_pTransformCom->Set_Position(m_tDesc.vOrigin);

	m_pCollider->Set_Active(TRUE);

	return S_OK;
}

HRESULT CKorsica_Wind::Ready_Components()
{
	return S_OK;
}

HRESULT CKorsica_Wind::Bind_ShaderResources()
{
	return S_OK;
}

CKorsica_Wind* CKorsica_Wind::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CKorsica_Wind* pInstance = new CKorsica_Wind(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CKorsica_Wind");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CKorsica_Wind* CKorsica_Wind::Clone(void* pArg)
{
	CKorsica_Wind* pInstance = new CKorsica_Wind(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CKorsica_Wind");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CKorsica_Wind::Free()
{
	__super::Free();
}
