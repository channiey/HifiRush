#include "..\Public\Light_Manager.h"
#include "Light.h"

#include "EngineInstance.h"

IMPLEMENT_SINGLETON(CLight_Manager)

CLight_Manager::CLight_Manager()
{

}

const LIGHT_DESC* CLight_Manager::Get_LightDesc(_uint iLightIndex)
{
	if (iLightIndex >= m_Lights.size())
		return nullptr;

	auto	iter = m_Lights.begin();

	for (size_t i = 0; i < iLightIndex; i++)
		++iter;

	return (*iter)->Get_LightDesc();	
}

HRESULT CLight_Manager::Add_Light(const LIGHT_DESC & LightDesc)
{
	CLight*		pLight = CLight::Create(LightDesc);

	if (nullptr == pLight)
		return E_FAIL;

	m_Lights.push_back(pLight);

	return S_OK;
}

HRESULT CLight_Manager::Render(CShader * pShader, CVIBuffer_Rect * pVIBuffer)
{
	for (auto& pLight : m_Lights)
	{
		pLight->Render(pShader, pVIBuffer);
	}

	return S_OK;
}

Matrix CLight_Manager::Get_ShadowLight_MatView() const
{
	Matrix matView;

	if (m_ShadowLight_OriginPos == m_ShadowLight_TargetPos)
		return Matrix::Identity;

	XMStoreFloat4x4(&matView, XMMatrixLookAtLH(m_ShadowLight_OriginPos, m_ShadowLight_TargetPos, Vec4::UnitY));

	return matView;
}

void CLight_Manager::Set_ShadowLight_MatProj()
{
	D3D11_VIEWPORT		ViewportDesc;
	_uint				iNumViewports = 1;

	ENGINE_INSTANCE->Get_Context()->RSGetViewports(&iNumViewports, &ViewportDesc);

	XMStoreFloat4x4(&m_ShadowLight_MatProj, XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), (_float)ViewportDesc.Width / (_float)ViewportDesc.Height, 0.1f, 1000.f));
}

void CLight_Manager::Free()
{
	for (auto& pLight : m_Lights)
		Safe_Release(pLight);

	m_Lights.clear();
}
