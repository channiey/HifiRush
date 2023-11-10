#include "..\Public\Light.h"
#include "Shader.h"
#include "VIBuffer_Rect.h"

CLight::CLight()
{
}

HRESULT CLight::Initialize(const LIGHT_DESC & LightDesc)
{
	memmove(&m_LightDesc, &LightDesc, sizeof LightDesc);

	return S_OK;
}

HRESULT CLight::Render(CShader * pShader, CVIBuffer_Rect * pVIBuffer)
{
	_uint		iPassIndex = 0;

	if (LIGHT_DESC::LIGHT_DIRECTIONAL == m_LightDesc.eLightType)
	{
		if (FAILED(pShader->Bind_RawValue("g_vLightDir", &m_LightDesc.vLightDir, sizeof(_float4))))
			return E_FAIL;
		iPassIndex = 1;
	}
	else if (LIGHT_DESC::LIGHT_POINT == m_LightDesc.eLightType)
	{
		if (FAILED(pShader->Bind_RawValue("g_vLightPos", &m_LightDesc.vLightPos, sizeof(_float4))))
			return E_FAIL;
		if (FAILED(pShader->Bind_RawValue("g_fLightRange", &m_LightDesc.fLightRange, sizeof(_float))))
			return E_FAIL;
		iPassIndex = 2;
	}

	if (FAILED(pShader->Bind_RawValue("g_vLightDiffuse", &m_LightDesc.vDiffuse, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(pShader->Bind_RawValue("g_vLightAmbient", &m_LightDesc.vAmbient, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(pShader->Bind_RawValue("g_vLightSpecular", &m_LightDesc.vSpecular, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(pShader->Begin(iPassIndex)))
		return E_FAIL;

	if (FAILED(pVIBuffer->Render()))
		return E_FAIL;

	return S_OK;
}

CLight * CLight::Create(const LIGHT_DESC & LightDesc)
{
	CLight*		pInstance = new CLight;

	if (FAILED(pInstance->Initialize(LightDesc)))
	{
		MSG_BOX("Failed to Created : CLight");
		Safe_Release(pInstance);		
	}

	return pInstance;
}

void CLight::Free()
{
}
