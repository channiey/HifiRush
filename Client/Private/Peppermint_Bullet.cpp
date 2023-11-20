#include "..\Default\stdafx.h"
#include "Peppermint_Bullet.h"

#include "EngineInstance.h"

#include "Peppermint.h"

CPeppermint_Bullet::CPeppermint_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CProjectile(pDevice, pContext)
{
}

CPeppermint_Bullet::CPeppermint_Bullet(const CPeppermint_Bullet& rhs)
	: CProjectile(rhs)
{
}

HRESULT CPeppermint_Bullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPeppermint_Bullet::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CPeppermint_Bullet::Tick(_double fTimeDelta)
{
	if (nullptr == m_tDesc.pOwner) return;

	__super::Tick(fTimeDelta);
}

void CPeppermint_Bullet::LateTick(_double fTimeDelta)
{
	if (nullptr == m_tDesc.pOwner) return;

	__super::LateTick(fTimeDelta);
}

HRESULT CPeppermint_Bullet::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CPeppermint_Bullet::OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild)
{
	if (nullptr == m_tDesc.pOwner) return;

	m_tDesc.pOwner->OnCollision_Enter(pCollider, CPeppermint::CHILD_TYPE::PROJECTILE);

	Return_To_Pool();
}

void CPeppermint_Bullet::OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild)
{
	if (nullptr == m_tDesc.pOwner) return;

	m_tDesc.pOwner->OnCollision_Stay(pCollider, CPeppermint::CHILD_TYPE::PROJECTILE);
}

void CPeppermint_Bullet::OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild)
{
	if (nullptr == m_tDesc.pOwner) return;

	m_tDesc.pOwner->OnCollision_Exit(pCollider, CPeppermint::CHILD_TYPE::PROJECTILE);
}

HRESULT CPeppermint_Bullet::Ready_Components()
{
	return S_OK;
}

HRESULT CPeppermint_Bullet::Bind_ShaderResources()
{
	return S_OK;
}

CPeppermint_Bullet* CPeppermint_Bullet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPeppermint_Bullet* pInstance = new CPeppermint_Bullet(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPeppermint_Bullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CPeppermint_Bullet* CPeppermint_Bullet::Clone(void* pArg)
{
	CPeppermint_Bullet* pInstance = new CPeppermint_Bullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPeppermint_Bullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPeppermint_Bullet::Free()
{
	__super::Free();
}
