#ifdef _DEBUG
#include "../Default/stdafx.h"

#include "ImGui_Window_Mid_Dynamic.h"
#include "GameObject.h"

#include "Util_String.h"
#include "Util_File.h"

#include "Dynamic.h"


CImGui_Window_Mid_Dynamic::CImGui_Window_Mid_Dynamic()
{
}

HRESULT CImGui_Window_Mid_Dynamic::Initialize()
{

	return S_OK;
}

void CImGui_Window_Mid_Dynamic::Show_Window()
{
	ImGuiWindowFlags window_flags = 0;
	//window_flags |= ImGuiWindowFlags_NoMove;
	//window_flags |= ImGuiWindowFlags_NoResize;

	if (ImGui::Begin(m_pImGui_Manager->str_MainWindowType[m_pImGui_Manager->WINDOW_MAIN_DYNAMIC], NULL, window_flags))
	{
		if (nullptr != m_pImGui_Manager->m_pCurObject && LayerNames[LAYER_ID::LAYER_ENV_INTERACTALBE] == m_pImGui_Manager->m_pCurObject->Get_LayerTag())
		{
			m_pDynamic = dynamic_cast<CDynamic*>(m_pImGui_Manager->m_pCurObject);

			if (nullptr != m_pDynamic)
			{
				if (ImGui::Button("Set_On"))
				{
					m_pDynamic->Set_On();
				}
				ImGui::SameLine();


				if (ImGui::Button("Set_Off"))
				{
					m_pDynamic->Set_Off();
				}
			}
			
		}
	}
	ImGui::End();

	/* Childe Window */
	Show_ChildWindow();
}

void CImGui_Window_Mid_Dynamic::Clear_Reference_Data()
{
	m_pDynamic = nullptr;}

CImGui_Window_Mid_Dynamic* CImGui_Window_Mid_Dynamic::Create()
{
	CImGui_Window_Mid_Dynamic* pInstance = new CImGui_Window_Mid_Dynamic();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CImGui_Window_Mid_Dynamic");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImGui_Window_Mid_Dynamic::Free()
{
	__super::Free();
}

#endif // _DEBUG