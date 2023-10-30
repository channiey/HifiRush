#include "..\Public\Renderer.h"
#include "GameObject.h"

#include "EngineInstance.h"

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

HRESULT CRenderer::Add_DebugGroup(CComponent* pDebugCom)
{
	m_DebugObject.push_back(pDebugCom);

	Safe_AddRef(pDebugCom);

	return S_OK;
}


HRESULT CRenderer::Draw_RenderObjects()
{
	if (FAILED(Render_Priority()))
		return S_OK;// E_FAIL;
	if (FAILED(Render_NonLight()))
		return S_OK;// E_FAIL;
	if (FAILED(Render_NonBlend()))
		return S_OK;// E_FAIL;
	if (FAILED(Render_Blend()))
		return S_OK;// E_FAIL;
	if (FAILED(Render_UI()))
		return S_OK;// E_FAIL;

#ifdef _DEBUG
	if (FAILED(Render_Debug()))
		return S_OK;// E_FAIL;
#endif

	return S_OK;
}

HRESULT CRenderer::Render_Priority()
{
	for (auto& pGameObject : m_RenderObjects[RG_PRIORITY])
	{
		if (nullptr != pGameObject && pGameObject->Is_Active() && pGameObject->Is_Render())
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
		if (nullptr != pGameObject && pGameObject->Is_Active() && pGameObject->Is_Render())
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
		if (nullptr != pGameObject && pGameObject->Is_Active() && pGameObject->Is_Render())
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
		if (nullptr != pGameObject && pGameObject->Is_Active() && pGameObject->Is_Render())
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
		if (nullptr != pGameObject && pGameObject->Is_Active() && pGameObject->Is_Render())
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RG_UI].clear();


	return S_OK;
}

HRESULT CRenderer::Render_Debug()
{
	/*m_pShader->Bind_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pShader->Bind_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));*/

	//m_pTarget_Manager->Render_Debug(TEXT("MRT_Deferred"), m_pVIBuffer, m_pShader);
	//m_pTarget_Manager->Render_Debug(TEXT("MRT_LightAcc"), m_pVIBuffer, m_pShader);

	for (auto& pDebugCom : m_DebugObject)
	{
		pDebugCom->Render();
		Safe_Release(pDebugCom);
	}

	m_DebugObject.clear();

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

#ifdef _DEBUG
	//Safe_Release(m_pShader);
	//Safe_Release(m_pVIBuffer);
#endif // _DEBUG
}
