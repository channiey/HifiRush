#include "..\Public\Renderer.h"
#include "GameObject.h"

#include "Target_Manager.h"
#include "Light_Manager.h"

#include "EngineInstance.h"

CRenderer::CRenderer(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
	, m_pTarget_Manager(CTarget_Manager::GetInstance())
	, m_pLight_Manager(CLight_Manager::GetInstance())
{
	Safe_AddRef(m_pLight_Manager);
	Safe_AddRef(m_pTarget_Manager);
}

HRESULT CRenderer::Initialize_Prototype()
{

	_uint				iNumViewports = 1;

	m_pContext->RSGetViewports(&iNumViewports, &m_ViewportDesc);

	/* 렌더 타겟을 생성해서 매니저에 보관 */
	{
		/* For.Target_Diffuse */
		if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Diffuse"),
			m_ViewportDesc.Width, m_ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(1.f, 1.f, 1.f, 0.f))))
			return E_FAIL;

		/* For.Target_Normal */
		if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Normal"),
			m_ViewportDesc.Width, m_ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(1.f, 1.f, 1.f, 1.f))))
			return E_FAIL;

		/* For.Target_Depth */
		if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Depth"),
			m_ViewportDesc.Width, m_ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(1.f, 1.f, 1.f, 1.f))))
			return E_FAIL;

		/* For.Target_Shade */
		if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Shade"),
			m_ViewportDesc.Width, m_ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 1.f))))
			return E_FAIL;

		/* For.Target_LightDepth */
		if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_LightDepth"),
			m_ViewportDesc.Width * m_fShadowTargetMag, m_ViewportDesc.Height * m_fShadowTargetMag, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(1.f, 1.f, 1.f, 1.f))))
			return E_FAIL;

		/* For.Target_Outline */
		if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Outline"),
			m_ViewportDesc.Width, m_ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
			return E_FAIL;
	}

	/* 디버그용 미니 렌더 창 세팅 */
	{
		if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Diffuse"), 
			100.f, 100.f, 200.f, 200.f)))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Normal"), 
			100.f, 300.f, 200.f, 200.f)))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Depth"), 
			100.f, 500.f, 200.f, 200.f)))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Shade"), 
			300.f, 100.f, 200.f, 200.f)))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_LightDepth"),
			300.f, 300.f, 200.f, 200.f)))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Outline"), 
			300.f, 500.f, 200.f, 200.f)))

			return E_FAIL;
	}

	/* For.MRT_GameObjects (이 렌더타겟들은 그려지는 객체로부터 값을 저장받는다.)*/
	{
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Diffuse"))))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Normal"))))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Depth"))))
			return E_FAIL;
	}
	
	/* For.MRT_Lights (이 렌더타겟들은 게임내에 존재하는 빛으로부터 연산한 결과를 저장받는다.) */
	{
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Lights"), TEXT("Target_Shade"))))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_LightDepth"), TEXT("Target_LightDepth"))))
			return E_FAIL;
	}

	/* For.MRT_Outline */
	{
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Outline"), TEXT("Target_Outline"))))
			return E_FAIL;
	}


	/* 버퍼 생성 */
	m_pVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pContext);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	/* 셰이더 생성 */
	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Deferred.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;

	/* 행렬 세팅 (화면 꽉채움) */
	{
		XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
		m_WorldMatrix._11 = m_ViewportDesc.Width;
		m_WorldMatrix._22 = m_ViewportDesc.Height;

		XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
		XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(m_ViewportDesc.Width, m_ViewportDesc.Height, 0.f, 1.f));
	}

	if (FAILED(Create_ShadowDSV()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup(RENDERGROUP eRenderGroup, CGameObject* pGameObject)
{
	if (eRenderGroup >= RG_END || RG_PRIORITY > eRenderGroup)
		return E_FAIL;

	m_RenderObjects[eRenderGroup].push_back(pGameObject);

	Safe_AddRef(pGameObject);

	return S_OK;
}

HRESULT CRenderer::Add_Debug(CComponent* pDebug)
{
	m_RenderDebug.push_back(pDebug);

	Safe_AddRef(pDebug);

	return S_OK;
}

HRESULT CRenderer::Draw_RenderObjects()
{
	if (FAILED(Render_Priority()))
		return S_OK; 
	if (FAILED(Render_NonLight()))
		return S_OK; 
	if (FAILED(Render_LightDepth()))
		return S_OK;
	if (FAILED(Render_NonBlend())) /* 디퓨즈, 노말 */
		return S_OK; 
	if (FAILED(Render_OutLine()))  /* 아웃라인 (노말 참고)*/
		return S_OK;
	if (FAILED(Render_LightAcc()))
		return S_OK; 
	if (FAILED(Render_Deferred()))
		return S_OK; 
	if (FAILED(Render_Blend()))
		return S_OK; 
	if (FAILED(Render_UI()))
		return S_OK; 
	
	if (ENGINE_INSTANCE->Is_DebugRTV())
	{
		if (FAILED(Render_Debug()))
			return S_OK; // E_FAIL;
	}

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

HRESULT CRenderer::Render_LightDepth()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_LightDepth"), m_pShadowDSV)))
		return E_FAIL;

	m_pContext->ClearDepthStencilView(m_pShadowDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	/* 그림자를 그리기 위한 DSV를 세팅하기 위해 현재 DSV를 받아둔다. */
	D3D11_VIEWPORT		ViewportDesc;
	_float				fOriginalWidth, fOriginalHeight;
	_uint				iNumViewports = 1;
	{

		m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);

		fOriginalWidth		= ViewportDesc.Width;
		fOriginalHeight		= ViewportDesc.Height;

		ViewportDesc.Width	*= m_fShadowTargetMag;
		ViewportDesc.Height *= m_fShadowTargetMag;

		m_pContext->RSSetViewports(iNumViewports, &ViewportDesc);
	}

	/* 그림자 렌더 */
	{
		for (auto& pGameObject : m_RenderObjects[RG_SHADOW])
		{
			if (nullptr != pGameObject)
				pGameObject->Render_LightDepth();

			Safe_Release(pGameObject);
		}
		m_RenderObjects[RG_SHADOW].clear();

		if (FAILED(m_pTarget_Manager->End_MRT(m_pContext, m_pShadowDSV)))
			return E_FAIL;
	}

	/* DSV 원래 것으로 교체 */
	{
		m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);
		ViewportDesc.Width = fOriginalWidth;
		ViewportDesc.Height = fOriginalHeight;

		m_pContext->RSSetViewports(iNumViewports, &ViewportDesc);
	}

	return S_OK;
}

HRESULT CRenderer::Render_NonBlend()
{
	/* Diffuse + Normal */
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_GameObjects"))))
		return E_FAIL; /* 백버퍼를 빼고 MRT_GameObjects 렌더 타겟을 바인딩 */
	{
		for (auto& pGameObject : m_RenderObjects[RG_NONBLEND])
		{
			if (nullptr != pGameObject && pGameObject->Is_Active() && pGameObject->Is_Render())
				pGameObject->Render();

			Safe_Release(pGameObject);
		}
		m_RenderObjects[RG_NONBLEND].clear();

	}

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL; /* MRT_GameObjects 빼고 다시 백버퍼 바인딩 */

	return S_OK;
}

HRESULT CRenderer::Render_LightAcc()
{
	/* 빛연산의 결과를 그려놓을 Shade + Specular 타겟을 장치에 바인딩 한다. */
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Lights"))))
		return E_FAIL;

	/* 사각형 버퍼를 직교투영으로 Shade타겟의 사이즈만큼 꽉 채워서 그릴꺼야. */
	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	CPipeLine* pPipeLine = GET_INSTANCE(CPipeLine);
	{
		Matrix matVI = pPipeLine->Get_Transform_Inverse(CPipeLine::STATE_VIEW);
		Matrix matPI = pPipeLine->Get_Transform_Inverse(CPipeLine::STATE_PROJ);
		Vec4 vCamPos = pPipeLine->Get_CamPosition();

		if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrixInv", &matVI)))
			return E_FAIL;
		if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrixInv", &matPI)))
			return E_FAIL;
		if (FAILED(m_pShader->Bind_RawValue("g_vCamPosition", &vCamPos, sizeof(_float4))))
			return E_FAIL;
	}
	RELEASE_INSTANCE(CPipeLine);

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShader, TEXT("Target_Normal"), "g_NormalTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShader, TEXT("Target_Depth"), "g_DepthTexture")))
		return E_FAIL;

	m_pLight_Manager->Render(m_pShader, m_pVIBuffer);

	/* 다시 장치의 0번째 소켓에 백 버퍼를 올린다. */
	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_OutLine()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Outline"))))
		return E_FAIL;

	/* Bind */
	{
		if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
			return E_FAIL;

		if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
			return E_FAIL;

		if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShader, TEXT("Target_Diffuse"), "g_DiffuseTexture")))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShader, TEXT("Target_Normal"), "g_NormalTexture")))
			return E_FAIL;

		if (FAILED(m_pShader->Bind_RawValue("MAP_CX", &m_ViewportDesc.Width, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShader->Bind_RawValue("MAP_CY", &m_ViewportDesc.Height, sizeof(_float))))
			return E_FAIL;
	}

	/* Draw */
	{
		if (FAILED(m_pShader->Begin(4)))
			return E_FAIL;

		if (FAILED(m_pVIBuffer->Render()))
			return E_FAIL;
	}

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Deferred()
{
	/* 디퓨즈 타겟과 셰이드 타겟을 서로 곱하여 백버퍼에 최종적으로 찍어낸다. */

	/* 행렬 세팅 */
	{
		if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
			return E_FAIL;
		if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
			return E_FAIL;
		if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
			return E_FAIL;
	}

	/* 렌더 타겟 텍스처 세팅 */
	{
		if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShader, TEXT("Target_Diffuse"), "g_DiffuseTexture")))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShader, TEXT("Target_Shade"), "g_ShadeTexture")))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShader, TEXT("Target_Outline"), "g_OutlineTexture")))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShader, TEXT("Target_LightDepth"), "g_LightDepthTexture")))
			return E_FAIL;

		/* Shadow Light */
		{
			Matrix matView_ShadowLight = ENGINE_INSTANCE->Get_ShadowLight_MatView();
			Matrix matProj_ShadowLight = ENGINE_INSTANCE->Get_ShadowLight_MatProj();

			if (FAILED(m_pShader->Bind_Matrix("g_LightViewMatrix", &matView_ShadowLight)))
				return E_FAIL;

			if (FAILED(m_pShader->Bind_Matrix("g_LightProjMatrix", &matProj_ShadowLight)))
				return E_FAIL;


			Matrix matVI = CPipeLine::GetInstance()->Get_Transform_Inverse(CPipeLine::STATE_VIEW);
			Matrix matPI = CPipeLine::GetInstance()->Get_Transform_Inverse(CPipeLine::STATE_PROJ);

			if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrixInv", &matVI)))
				return E_FAIL;

			if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrixInv", &matPI)))
				return E_FAIL;
		}
	}

	/* 그린다 */
	{
		if (FAILED(m_pShader->Begin(3)))
			return E_FAIL;

		if (FAILED(m_pVIBuffer->Render()))
			return E_FAIL;
	}

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
	for (auto& pDebugCom : m_RenderDebug)
	{
		pDebugCom->Render();
		Safe_Release(pDebugCom);
	}
	m_RenderDebug.clear();

	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_GameObjects"), m_pShader, m_pVIBuffer)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_Lights"), m_pShader, m_pVIBuffer)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_LightDepth"), m_pShader, m_pVIBuffer)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_Outline"), m_pShader, m_pVIBuffer)))
		return E_FAIL;

	return S_OK;

}

HRESULT CRenderer::Create_ShadowDSV()
{
	ID3D11Texture2D* pDepthStencilTexture = nullptr;

	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = m_fShadowTargetMag * m_ViewportDesc.Width;
	TextureDesc.Height = m_fShadowTargetMag * m_ViewportDesc.Height;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL/*| D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE*/;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilTexture)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr, &m_pShadowDSV)))
		return E_FAIL;

	Safe_Release(pDepthStencilTexture);

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

	Safe_Release(m_pShader);
	Safe_Release(m_pVIBuffer);

	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pTarget_Manager);
}
