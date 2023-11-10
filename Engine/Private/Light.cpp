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
	if (FAILED(pShader->Bind_RawValue("g_vLightDir", &m_LightDesc.vLightDir, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(pShader->Begin(1)))
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
