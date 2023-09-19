#ifdef _DEBUG
#include "../Default/stdafx.h"

#include "ImGui_Window_Main_Controller.h"

CImGui_Window_Main_Controller::CImGui_Window_Main_Controller()
{
}

HRESULT CImGui_Window_Main_Controller::Initialize()
{
	return S_OK;
}

void CImGui_Window_Main_Controller::Show_Window()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;

	if (ImGui::Begin(m_pImGui_Manager->str_MainWindowType[m_pImGui_Manager->WINDOW_MAIN_CONTROLLER], NULL, window_flags))
	{
		/* 프로파일러 데이터를 보여준다. */
		ImGui::SeparatorText("Statistics");
		{
			PROFILER_DESC desc = m_pGameInstance->Get_ProfillingData();

			ImGuiIO& io = ImGui::GetIO(); (void)io;
			ImGui::Text("FPS : %.1f(%.2f ms)", io.Framerate, FPS_LIMIT / io.Framerate);

			ImGui::Text("Tris : %d", desc.iTris);
			ImGui::SameLine();
			ImGui::Text("  Animations : %d", desc.iAnim);

			ImGui::Text("Batches : %d", desc.iBatches);
			ImGui::SameLine();
			ImGui::Text("  SetPass calls : %d", desc.iSetPassCall);

		}

		/* 카메라 관련 버튼을 나열한다. */
		ImGui::SeparatorText("Camera");
		{
			/* 렌더 타겟 뷰*/
			if (ImGui::Button("RTV"))
			{

			}
			ImGui::SameLine();

			/* 디버그 카메라*/
			if (ImGui::Button("Debug Cam"))
			{

			}
		}

		/* 각 메인 윈도우 버튼을 보여준다. */
		ImGui::SeparatorText("Windows");
		{
			/* 메인 윈도우 오브젝트 버튼 */
			if (ImGui::Button(m_pImGui_Manager->str_MainWindowType[m_pImGui_Manager->WINDOW_MAIN_OBJECT_INFO]))
			{
				/* TODO : 해당 enum의 윈도우가 생기지 않았을 경우의 예외 처리 필요 */
				m_pImGui_Manager->Set_Active_Main_Window(
					m_pImGui_Manager->WINDOW_MAIN_OBJECT_INFO,
					!m_pImGui_Manager->m_pMainWindows[m_pImGui_Manager->WINDOW_MAIN_OBJECT_INFO]->Is_Active());
			}
			ImGui::SameLine();

			/* 메인 윈도우 하이어러키 버튼 */
			if (ImGui::Button(m_pImGui_Manager->str_MainWindowType[m_pImGui_Manager->WINDOW_MAIN_HIEARACHY]))
			{
				m_pImGui_Manager->Set_Active_Main_Window(
					m_pImGui_Manager->WINDOW_MAIN_HIEARACHY,
					!m_pImGui_Manager->m_pMainWindows[m_pImGui_Manager->WINDOW_MAIN_HIEARACHY]->Is_Active());

			}
			ImGui::SameLine();

			/* 메인 윈도우 데모 버튼 */
			if (ImGui::Button(m_pImGui_Manager->str_MainWindowType[m_pImGui_Manager->WINDOW_MAIN_DEMO]))
			{
				m_pImGui_Manager->Set_Active_Main_Window(
					m_pImGui_Manager->WINDOW_MAIN_DEMO,
					!m_pImGui_Manager->m_pMainWindows[m_pImGui_Manager->WINDOW_MAIN_DEMO]->Is_Active());

			}
		}
	}
	ImGui::End();

	/* Childe Window */
	Show_ChildWindow();
}

void CImGui_Window_Main_Controller::Clear_Reference_Data()
{
}

CImGui_Window_Main_Controller* CImGui_Window_Main_Controller::Create()
{
	CImGui_Window_Main_Controller* pInstance = new CImGui_Window_Main_Controller();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CImGui_Window_Main_Controller");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImGui_Window_Main_Controller::Free()
{
	__super::Free();
}

#endif // _DEBUG