#include "MainApp.h"
#include "..\Default\stdafx.h"
#include "..\Public\MainApp.h"
#include "EngineInstance.h"
#include "Level_Loading.h"

#include "GameManager.h"
#include "BeatManager.h"
#include "BattleManager.h"
#include "UiManager.h"

#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG

CMainApp::CMainApp()	
	: m_pEngineInstance(CEngineInstance::GetInstance())
#ifdef _DEBUG
	, m_pImGui_Manager(CImGui_Manager::GetInstance())
#endif // _DEBUG
{	
	Safe_AddRef(m_pEngineInstance);
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

	if (FAILED(m_pEngineInstance->Initialize_Engine(LV_END, g_hInst, GraphicDesc, &m_pDevice, &m_pContext, SoundFilePath_Engine)))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Components()))
		return E_FAIL;

	if (FAILED(Open_Level(LV_LOGO)))
		return E_FAIL;

	/* Client */
	{
		if (FAILED(CGameManager::GetInstance()->Initialize()))
			return E_FAIL;

		if (FAILED(CBeatManager::GetInstance()->Initialize()))
			return E_FAIL;

		if (FAILED(CUiManager::GetInstance()->Initialize()))
			return E_FAIL;
	}

	if (FAILED(Ready_CustomFont()))
		return E_FAIL;

#ifdef _DEBUG
	if(FAILED(m_pImGui_Manager->Initialize(m_pDevice, m_pContext)))
		return E_FAIL;
#endif // _DEBUG

	return S_OK;
}

void CMainApp::Tick(_double fTimeDelta)
{
	CBeatManager::GetInstance()->Update(fTimeDelta);

	{
		//system_clock::time_point start_time = system_clock::now();

		m_pEngineInstance->Tick(fTimeDelta);

		/*system_clock::time_point end_time = system_clock::now();
		milliseconds mill = duration_cast<milliseconds>(end_time - start_time);
		cout << "Tick \t" << mill.count() << "ms" << endl;*/
	}

	{
		//system_clock::time_point start_time = system_clock::now();
		
		m_pEngineInstance->LateTick(fTimeDelta);
		
		/*system_clock::time_point end_time = system_clock::now();
		milliseconds mill = duration_cast<milliseconds>(end_time - start_time);
		cout << "Late \t" << mill.count() << "ms" << endl;*/
	}

#ifdef _DEBUG
	if (m_pEngineInstance->Key_Down(VK_F1))
		m_pImGui_Manager->Toggle_Active();

	m_fTimeAcc += fTimeDelta; 
#endif // _DEBUG

}

HRESULT CMainApp::Render()
{

	/* 장면 초기화 */
	{
		//system_clock::time_point start_time = system_clock::now();
		if (FAILED(m_pEngineInstance->Clear_BackBuffer_View(_float4(0.f, 0.f, 0.f, 1.f))))
			return E_FAIL;
		if (FAILED(m_pEngineInstance->Clear_DepthStencil_View()))
			return E_FAIL;

		/*system_clock::time_point end_time = system_clock::now();
		milliseconds mill = duration_cast<milliseconds>(end_time - start_time);
		cout << "장면초기화 \t" << mill.count() << "ms" << endl;*/
	}


	{
		//system_clock::time_point start_time = system_clock::now();
		/* 게임 내 객체 렌더링 */
		if (FAILED(m_pRenderer->Draw_RenderObjects()))
			return E_FAIL;

		/*system_clock::time_point end_time = system_clock::now();
		milliseconds mill = duration_cast<milliseconds>(end_time - start_time);
		cout << "Render_Draw_RenderObjects \t" << mill.count() << "ms" << endl;*/

		/* ImGui 업데이트 및 렌더링 */
#ifdef _DEBUG
		if (LV_LOADING != m_pEngineInstance->Get_CurLevelIndex())
		{
			if (FAILED(m_pImGui_Manager->Render()))
				return E_FAIL;
		}
#endif // _DEBUG
	}


	{
		//system_clock::time_point start_time = system_clock::now();
		if(FAILED(m_pEngineInstance->Present()))
			return E_FAIL;

		/*system_clock::time_point end_time = system_clock::now();
		milliseconds mill = duration_cast<milliseconds>(end_time - start_time);
		cout << "Render_Draw_Present \t" << mill.count() << "ms" << endl;*/

	}

	return S_OK;
}

HRESULT CMainApp::FinishTick()
{
	/* 이벤트 매니저와 같은 브로드 캐스팅 작업 */
	m_pEngineInstance->FinishTick();

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
	if (nullptr == m_pEngineInstance)
		return E_FAIL;


	if (FAILED(m_pEngineInstance->Open_Level(LV_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, eLEVEL_ID))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Components()
{
	/* For.Prototype_Component_Renderer */
	if (FAILED(m_pEngineInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_Renderer"),
		m_pRenderer = CRenderer::Create(m_pDevice, m_pContext))))
		return E_FAIL;	

	/* For.Prototype_Component_Transform */
	if (FAILED(m_pEngineInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_Transform"),
		CTransform::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPosTex */
	if (FAILED(m_pEngineInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_Ui_Hud */
	if (FAILED(m_pEngineInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_Shader_Ui_Hud"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Ui_Hud.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pEngineInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	Safe_AddRef(m_pRenderer);
	
	return S_OK;
}

HRESULT CMainApp::Ready_CustomFont()
{
	if (FAILED(m_pEngineInstance->Add_Font(m_pDevice, m_pContext, TEXT("Font_Default"), TEXT("../Bin/Resources/Fonts/137ex.spriteFont"))))
		return E_FAIL;

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

	Safe_Release(m_pEngineInstance);

	CEngineInstance::Release_Engine();
}
