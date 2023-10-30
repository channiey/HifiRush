#include "..\Default\stdafx.h"
#include "..\Public\Peppermint.h"

#include "EngineInstance.h"

Peppermint::Peppermint(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCharacter(pDevice, pContext)
{
}

Peppermint::Peppermint(const CCharacter& rhs)
	: CCharacter(rhs)
{
}

HRESULT Peppermint::Initialize_Prototype()
{
	return S_OK;
}

HRESULT Peppermint::Initialize(void* pArg)
{
	if(FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void Peppermint::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void Peppermint::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT Peppermint::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT Peppermint::Ready_Components()
{
	return S_OK;
}

HRESULT Peppermint::Bind_ShaderResources()
{
	return S_OK;
}

Peppermint * Peppermint::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	Peppermint*	pInstance = new Peppermint(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : Peppermint");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CCharacter * Peppermint::Clone(void* pArg)
{
	Peppermint*	pInstance = new Peppermint(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : Peppermint");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Peppermint::Free()
{
	__super::Free();
}
