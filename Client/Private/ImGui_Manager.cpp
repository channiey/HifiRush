#include "..\Default\stdafx.h"

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif

#include "../ImGui/imgui.h" 
#include "../ImGui/imgui_impl_win32.h" 
#include "../ImGui/imgui_impl_dx11.h" 

#include "ImGui_Manager.h"
#include "GameInstance.h"
#include "GameObject.h"

IMPLEMENT_SINGLETON(CImGui_Manager)

CImGui_Manager::CImGui_Manager()
	: m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

#pragma region ImGui-Cycle

HRESULT CImGui_Manager::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	Safe_AddRef(pDevice);
	Safe_AddRef(pContext);

	if (FAILED(ImGui_SetUp(pDevice, pContext)))
	{
		Safe_Release(pDevice);
		Safe_Release(pContext);
		return E_FAIL;
	}

	Safe_Release(pDevice);
	Safe_Release(pContext);

	for (int i = 0; i < WINDOW_END; ++i)
		m_bWindowsActive[i] = FALSE;

	return S_OK;
}

HRESULT CImGui_Manager::Render()
{
	if (!m_bActive) return S_OK;

	/* Tick()*/
	ImGui_Tick();
	{
		//Show_Controller(); // m_bActive가 TURE인 경우 컨트롤러는 항상 표시한다.
		//{
		//	if (m_bWindowsActive[WINDOW_PROFILER])
		//		Show_Window_Profiler();

		//	if (m_bWindowsActive[WINDOW_OBJECT])
		//		Show_Window_Object();

		//	if (m_bWindowsActive[WINDOW_HIEARACHY])
		//		Show_Window_Hierarachy();

		//	if (m_bWindowsActive[WINDOW_INSPECTOR])
		//		Show_Window_Inspector();

		//	if (m_bWindowsActive[WINDOW_DEMO])
		//		Show_Window_Demo();
		//}
	}
	Show_Window_Demo();

	/* Render */
	FAILED_CHECK_RETURN(ImGui_Render(), E_FAIL);

	return S_OK;
}

HRESULT CImGui_Manager::ImGui_SetUp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable Docking
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // Enable Multi-Viewport / Platform Windows

	ImGui::StyleColorsClassic();

	if (!ImGui_ImplWin32_Init(g_hWnd)) return E_FAIL;

	if (!ImGui_ImplDX11_Init(pDevice, pContext)) return E_FAIL;

	return S_OK;
}

void CImGui_Manager::ImGui_Tick()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();
}

HRESULT CImGui_Manager::ImGui_Render()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return S_OK;
}

#pragma endregion


void CImGui_Manager::Show_Window_Demo()
{
	ImGui::ShowDemoWindow(&m_bWindowsActive[WINDOW_DEMO]);
}

#pragma endregion

void CImGui_Manager::Free()
{
	Safe_Release(m_pGameInstance);

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

