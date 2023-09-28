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
	/* 1-1. 디바이스(그래픽, 사운드, 입력)를 초기화한다. */
	GRAPHIC_DESC		GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof GraphicDesc);

	GraphicDesc.eWinMode = GRAPHIC_DESC::WINMODE_WIN;
	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.iWinSizeX = g_iWinSizeX;
	GraphicDesc.iWinSizeY = g_iWinSizeY;

	if (FAILED(m_pGameInstance->Initialize_Engine(LEVEL_END, g_hInst, GraphicDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;

	/* 1-2 모든 레벨에서 사용할 컴포넌트 원형을 생성한다. */
	if (FAILED(Ready_Prototype_Components()))
		return E_FAIL;

	/* 1-3. 게임내에서 사용할 레벨(씬)을 생성한다. */
	if (FAILED(Open_Level(LEVEL_LOGO)))
		return E_FAIL;

	/* 1-4. 게임내에서 사용할 여러 자원(텍스쳐, 모델, 객체) 들을 준비한다. */

#ifdef _DEBUG
	/* 1-5. ImGui Manager을 세팅한다. */
	FAILED_CHECK_RETURN(m_pImGui_Manager->Initialize(m_pDevice, m_pContext), E_FAIL);
#endif // _DEBUG

#ifdef _DEBUG
	/* 1-6. 테스트 코드를 실행한다. */
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
	/* 장면 초기화 */
	FAILED_CHECK_RETURN(m_pGameInstance->Clear_BackBuffer_View(_float4(0.5f, 0.5f, 0.5f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Clear_DepthStencil_View(), E_FAIL);
	{
		/* 게임 내 객체 렌더링 */
		FAILED_CHECK_RETURN(m_pRenderer->Draw_RenderObjects(), E_FAIL);

		/* ImGui 업데이트 및 렌더링 */
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
	/* 이벤트 매니저와 같은 브로드 캐스팅 작업 */
	m_pGameInstance->FinishTick();


#ifdef _DEBUG
	/* 프로파일링 데이터 초기화 */

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

	/* 로고레베릉ㄹ 할당하고 싶었지만. 로고레벨을 위한 로딩레벨을 먼저 생성하여 로딩작업을 수행할꺼야. */
	/* 로딩객체에게 eLEVEL_ID라는 내가 실제 할당ㅎ아고 싶었던 레벨열거체를 준거지?! 실제할당하고싶었던 레벨에 자원을 준비라하라고 */
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
	/* 동적으로 텍스처를 생성 및 저장 한다. */
	/* 첫번째는 미리 정의된 서식대로 텍스처를 생성 및 저장하고, 두번째는 텍스처의 픽셀을 동적으로 변경한다. */
	/* 동적으로 텍스처 픽셀을 변경할 경우 Map(), Unmap()이 필요하고, 이때 두 함수 사이 간격은 최소한으로 한다. */

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

	TextureDesc.Usage = D3D11_USAGE_DYNAMIC; /* 동적 버퍼 세팅 */
	TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;  /* 동적 버퍼 세팅 */ 
	TextureDesc.MiscFlags = 0;  /* 동적 버퍼 세팅 */

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

	/* 위에서 정의한 서식 대로 텍스처를 생성한다. */
	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, &InitialData, &pTexture2D)))
		return E_FAIL;

	/* 위에서 생성한 텍스처를 로컬에 저장한다. */
	SaveDDSTextureToFile(m_pContext, pTexture2D, TEXT("../Bin/Resources/Textures/Terrain/MyMask.dds"));

	/* 텍스처의 픽셀 정보를 동적으로 수정한다. */
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
