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

	if (FAILED(m_pGameInstance->Initialize_Engine(LV_END, g_hInst, GraphicDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;

	/* 1-2 ��� �������� ����� ������Ʈ ������ �����Ѵ�. */
	if (FAILED(Ready_Prototype_Components()))
		return E_FAIL;

	/* 1-3. ���ӳ����� ����� ����(��)�� �����Ѵ�. */
	if (FAILED(Open_Level(LV_LOGO)))
		return E_FAIL;

#ifdef _DEBUG
	/* 1-5. ImGui Manager�� �����Ѵ�. */
	if(FAILED(m_pImGui_Manager->Initialize(m_pDevice, m_pContext)))
		return E_FAIL;
#endif // _DEBUG

	return S_OK;
}

void CMainApp::Tick(_float fTimeDelta)
{
	m_pGameInstance->Tick(fTimeDelta);

	m_pGameInstance->LateTick(fTimeDelta);


#ifdef _DEBUG
	if (m_pGameInstance->Key_Down(VK_F1))
		m_pImGui_Manager->Toggle_Active();

	m_fTimeAcc += fTimeDelta; 
#endif // _DEBUG

}

HRESULT CMainApp::Render()
{
	/* ��� �ʱ�ȭ */
	if (FAILED(m_pGameInstance->Clear_BackBuffer_View(_float4(0.5f, 0.5f, 0.5f, 1.f))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Clear_DepthStencil_View()))
		return E_FAIL;
	{
		/* ���� �� ��ü ������ */
		if (FAILED(m_pRenderer->Draw_RenderObjects()))
			return E_FAIL;

		/* ImGui ������Ʈ �� ������ */
#ifdef _DEBUG
		if (LV_LOADING != m_pGameInstance->Get_CurLevelIndex())
		{
			if (FAILED(m_pImGui_Manager->Render()))
				return E_FAIL;
		}
#endif // _DEBUG
	}
	if(FAILED(m_pGameInstance->Present()))
		return E_FAIL;
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


	if (FAILED(m_pGameInstance->Open_Level(LV_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, eLEVEL_ID))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Components()
{
	/* For.Prototype_Component_Renderer */
	if (FAILED(m_pGameInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_Renderer"),
		m_pRenderer = CRenderer::Create(m_pDevice, m_pContext))))
		return E_FAIL;	

	/* For.Prototype_Component_Transform */
	if (FAILED(m_pGameInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_Transform"),
		CTransform::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPosTex */
	if (FAILED(m_pGameInstance->Add_PrototypeCom(LV_STATIC, ShaderNames[SHADER_UI],
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	Safe_AddRef(m_pRenderer);
	
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
