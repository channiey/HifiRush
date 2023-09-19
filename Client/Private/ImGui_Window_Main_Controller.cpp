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
		/* �������Ϸ� �����͸� �����ش�. */
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

		/* ī�޶� ���� ��ư�� �����Ѵ�. */
		ImGui::SeparatorText("Camera");
		{
			/* ���� Ÿ�� ��*/
			if (ImGui::Button("RTV"))
			{

			}
			ImGui::SameLine();

			/* ����� ī�޶�*/
			if (ImGui::Button("Debug Cam"))
			{

			}
		}

		/* �� ���� ������ ��ư�� �����ش�. */
		ImGui::SeparatorText("Windows");
		{
			/* ���� ������ ������Ʈ ��ư */
			if (ImGui::Button(m_pImGui_Manager->str_MainWindowType[m_pImGui_Manager->WINDOW_MAIN_OBJECT_INFO]))
			{
				/* TODO : �ش� enum�� �����찡 ������ �ʾ��� ����� ���� ó�� �ʿ� */
				m_pImGui_Manager->Set_Active_Main_Window(
					m_pImGui_Manager->WINDOW_MAIN_OBJECT_INFO,
					!m_pImGui_Manager->m_pMainWindows[m_pImGui_Manager->WINDOW_MAIN_OBJECT_INFO]->Is_Active());
			}
			ImGui::SameLine();

			/* ���� ������ ���̾Ű ��ư */
			if (ImGui::Button(m_pImGui_Manager->str_MainWindowType[m_pImGui_Manager->WINDOW_MAIN_HIEARACHY]))
			{
				m_pImGui_Manager->Set_Active_Main_Window(
					m_pImGui_Manager->WINDOW_MAIN_HIEARACHY,
					!m_pImGui_Manager->m_pMainWindows[m_pImGui_Manager->WINDOW_MAIN_HIEARACHY]->Is_Active());

			}
			ImGui::SameLine();

			/* ���� ������ ���� ��ư */
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