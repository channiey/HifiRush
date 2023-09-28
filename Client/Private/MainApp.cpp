#include "MainApp.h"
#include "..\Default\stdafx.h"
#include "..\Public\MainApp.h"
#include "GameInstance.h"
#include "Level_Loading.h"

#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG

CMainApp::CMainApp()	
	: m_pGameInstance(CGameInstance::GetInstance())
#ifdef _DEBUG
	, m_pImGui_Manager(CImGui_Manager::GetInstance())
#endif // _DEBUG
{	
	Safe_AddRef(m_pGameInstance);
#ifdef _DEBUG
	Safe_AddRef(m_pImGui_Manager);
#endif // _DEBUG
}

HRESULT CMainApp::Initialize()
{
	/* 1-1. ����̽�(�׷���, ����, �Է�)�� �ʱ�ȭ�Ѵ�. */
	GRAPHIC_DESC		GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof GraphicDesc);

	GraphicDesc.eWinMode = GRAPHIC_DESC::WINMODE_WIN;
	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.iWinSizeX = g_iWinSizeX;
	GraphicDesc.iWinSizeY = g_iWinSizeY;

	if (FAILED(m_pGameInstance->Initialize_Engine(LEVEL_END, g_hInst, GraphicDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;

	/* 1-2 ��� �������� ����� ������Ʈ ������ �����Ѵ�. */
	if (FAILED(Ready_Prototype_Components()))
		return E_FAIL;

	/* 1-3. ���ӳ����� ����� ����(��)�� �����Ѵ�. */
	if (FAILED(Open_Level(LEVEL_LOGO)))
		return E_FAIL;

	/* 1-4. ���ӳ����� ����� ���� �ڿ�(�ؽ���, ��, ��ü) ���� �غ��Ѵ�. */

#ifdef _DEBUG
	/* 1-5. ImGui Manager�� �����Ѵ�. */
	FAILED_CHECK_RETURN(m_pImGui_Manager->Initialize(m_pDevice, m_pContext), E_FAIL);
#endif // _DEBUG

#ifdef _DEBUG
	/* 1-6. �׽�Ʈ �ڵ带 �����Ѵ�. */
	if (FAILED(Test_CreateAndSave_Texture_inDynamic()))
		return E_FAIL;
#endif // _DEBUG

	return S_OK;
}

void CMainApp::Tick(_float fTimeDelta)
{
	m_pGameInstance->Tick(fTimeDelta);

	m_pGameInstance->LateTick(fTimeDelta);

#ifdef _DEBUG
	m_fTimeAcc += fTimeDelta; 
#endif // _DEBUG

}

HRESULT CMainApp::Render()
{
	/* ��� �ʱ�ȭ */
	FAILED_CHECK_RETURN(m_pGameInstance->Clear_BackBuffer_View(_float4(0.5f, 0.5f, 0.5f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Clear_DepthStencil_View(), E_FAIL);
	{
		/* ���� �� ��ü ������ */
		FAILED_CHECK_RETURN(m_pRenderer->Draw_RenderObjects(), E_FAIL);

		/* ImGui ������Ʈ �� ������ */
#ifdef _DEBUG
		if(LEVEL_LOADING != m_pGameInstance->Get_CurLevelIndex())
			FAILED_CHECK_RETURN(m_pImGui_Manager->Render(), E_FAIL);
#endif // _DEBUG
	}
	FAILED_CHECK_RETURN(m_pGameInstance->Present(), E_FAIL);
	return S_OK;
}

HRESULT CMainApp::FinishTick()
{
	/* �̺�Ʈ �Ŵ����� ���� ��ε� ĳ���� �۾� */
	m_pGameInstance->FinishTick();


#ifdef _DEBUG
	/* �������ϸ� ������ �ʱ�ȭ */

	++m_iNumDraw;
	if (m_fTimeAcc >= 1.f)
	{
		g_iFPS = m_iNumDraw;
		m_iNumDraw = 0;
		m_fTimeAcc = 0.f;
	}

#endif // _DEBUG

	return S_OK;
}

HRESULT CMainApp::Open_Level(LEVEL_ID eLEVEL_ID)
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	/* �ΰ������� �Ҵ��ϰ� �;�����. �ΰ����� ���� �ε������� ���� �����Ͽ� �ε��۾��� �����Ҳ���. */
	/* �ε���ü���� eLEVEL_ID��� ���� ���� �Ҵ社�ư� �;��� ��������ü�� �ذ���?! �����Ҵ��ϰ�;��� ������ �ڿ��� �غ���϶�� */
	if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, eLEVEL_ID))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Components()
{
	/* For.Prototype_Component_Renderer */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), 
		m_pRenderer = CRenderer::Create(m_pDevice, m_pContext))))
		return E_FAIL;	

	/* For.Prototype_Component_Transform */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		CTransform::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPosTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	Safe_AddRef(m_pRenderer);
	
	return S_OK;
}

HRESULT CMainApp::Test_CreateAndSave_Texture_inDynamic()
{
	/* �������� �ؽ�ó�� ���� �� ���� �Ѵ�. */
	/* ù��°�� �̸� ���ǵ� ���Ĵ�� �ؽ�ó�� ���� �� �����ϰ�, �ι�°�� �ؽ�ó�� �ȼ��� �������� �����Ѵ�. */
	/* �������� �ؽ�ó �ȼ��� ������ ��� Map(), Unmap()�� �ʿ��ϰ�, �̶� �� �Լ� ���� ������ �ּ������� �Ѵ�. */

	ID3D11Texture2D* pTexture2D = { nullptr };

	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof TextureDesc);

	TextureDesc.Width = 256;
	TextureDesc.Height = 256;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DYNAMIC; /* ���� ���� ���� */
	TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;  /* ���� ���� ���� */ 
	TextureDesc.MiscFlags = 0;  /* ���� ���� ���� */

	_ulong* pPixel = new _ulong[TextureDesc.Width * TextureDesc.Height];

	for (_uint i = 0; i < 256; i++)
	{
		for (_uint j = 0; j < 256; j++)
		{
			_uint		iIndex = i * 256 + j;

			pPixel[iIndex] = D3DCOLOR_ARGB(255, 255, 255, 255);
		}
	}

	D3D11_SUBRESOURCE_DATA			InitialData;
	ZeroMemory(&InitialData, sizeof InitialData);
	InitialData.pSysMem = pPixel;
	InitialData.SysMemPitch = TextureDesc.Width * 4;

	/* ������ ������ ���� ��� �ؽ�ó�� �����Ѵ�. */
	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, &InitialData, &pTexture2D)))
		return E_FAIL;

	/* ������ ������ �ؽ�ó�� ���ÿ� �����Ѵ�. */
	SaveDDSTextureToFile(m_pContext, pTexture2D, TEXT("../Bin/Resources/Textures/Terrain/MyMask.dds"));

	/* �ؽ�ó�� �ȼ� ������ �������� �����Ѵ�. */
	D3D11_MAPPED_SUBRESOURCE	MappedSubResource;
	ZeroMemory(&MappedSubResource, sizeof MappedSubResource);

	pPixel[0] = D3DCOLOR_ARGB(255, 0, 0, 255);

	m_pContext->Map(pTexture2D, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedSubResource);

	memcpy(MappedSubResource.pData, pPixel, sizeof(_ulong) * 256 * 256);

	m_pContext->Unmap(pTexture2D, 0);

	SaveDDSTextureToFile(m_pContext, pTexture2D, TEXT("../Bin/Resources/Textures/Terrain/MyMask.dds"));

	Safe_Delete_Array(pPixel);
	Safe_Release(pTexture2D);

	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp*	pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Client::CMainApp::Free()
{	
	__super::Free();

	Safe_Release(m_pRenderer);

#ifdef _DEBUG
	Safe_Release(m_pImGui_Manager);
	Safe_Release(m_pImGui_Manager);
#endif // _DEBUG

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	Safe_Release(m_pGameInstance);

	CGameInstance::Release_Engine();
}
