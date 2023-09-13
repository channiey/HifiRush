#include "..\Public\Renderer.h"
#include "GameObject.h"

CRenderer::CRenderer(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{
}

HRESULT CRenderer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRenderer::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup(RENDERGROUP eRenderGroup, CGameObject * pGameObject)
{
	if (eRenderGroup >= RG_END)
		return E_FAIL;

	m_RenderObjects[eRenderGroup].push_back(pGameObject);

	Safe_AddRef(pGameObject);

	return S_OK;
}

HRESULT CRenderer::Draw_RenderObjects()
{
	if (FAILED(Render_Priority()))
		return E_FAIL;
	if (FAILED(Render_NonLight()))
		return E_FAIL;
	if (FAILED(Render_NonBlend()))
		return E_FAIL;
	if (FAILED(Render_Blend()))
		return E_FAIL;
	if (FAILED(Render_UI()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Priority()
{
	for (auto& pGameObject : m_RenderObjects[RG_PRIORITY])
	{
		if (nullptr != pGameObject/* && pGameObject->Is_Active()*/)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RG_PRIORITY].clear();

	return S_OK;
}

HRESULT CRenderer::Render_NonLight()
{
	for (auto& pGameObject : m_RenderObjects[RG_NONLIGHT])
	{
		if (nullptr != pGameObject/* && pGameObject->Is_Active()*/)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RG_NONLIGHT].clear();
	
	return S_OK;
}

HRESULT CRenderer::Render_NonBlend()
{
	for (auto& pGameObject : m_RenderObjects[RG_NONBLEND])
	{
		if (nullptr != pGameObject/* && pGameObject->Is_Active()*/)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RG_NONBLEND].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Blend()
{
	for (auto& pGameObject : m_RenderObjects[RG_BLEND])
	{
		if (nullptr != pGameObject/* && pGameObject->Is_Active()*/)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RG_BLEND].clear();

	return S_OK;
}

HRESULT CRenderer::Render_UI()
{
	for (auto& pGameObject : m_RenderObjects[RG_UI])
	{
		if (nullptr != pGameObject/* && pGameObject->Is_Active()*/)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RG_UI].clear();


	return S_OK;
}

CRenderer * CRenderer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CRenderer*	pInstance = new CRenderer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CRenderer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CRenderer::Clone(void * pArg)
{
	AddRef();

	return this;
}

void CRenderer::Free()
{
	__super::Free();

}
