#ifdef _DEBUG
#include "../Default/stdafx.h"

#include "ImGui_Window_Mid_Trigger.h"
#include "GameObject.h"

#include "Util_String.h"
#include "Util_File.h"

#include "TriggerBattle.h"

CImGui_Window_Mid_Trigger::CImGui_Window_Mid_Trigger()
{
}

HRESULT CImGui_Window_Mid_Trigger::Initialize()
{

	return S_OK;
}

void CImGui_Window_Mid_Trigger::Show_Window()
{
	ImGuiWindowFlags window_flags = 0;
	//window_flags |= ImGuiWindowFlags_NoMove;
	//window_flags |= ImGuiWindowFlags_NoResize;

	if (ImGui::Begin(m_pImGui_Manager->str_MainWindowType[m_pImGui_Manager->WINDOW_MAIN_TRIGGER], NULL, window_flags))
	{
		Show_BattleTrigger();
	}
	ImGui::End();

	/* Childe Window */
	Show_ChildWindow();
}

void CImGui_Window_Mid_Trigger::Clear_Reference_Data()
{
	m_iCurTriggerIndex = -1;
	m_pCurTrigger = nullptr;
	m_iFlowLevel = -1;
}

void CImGui_Window_Mid_Trigger::Show_BattleTrigger()
{
	ImGui::SeparatorText("Triggers");
	{
		list<class CGameObject*>* pList = ENGINE_INSTANCE->Get_Layer(ENGINE_INSTANCE->Get_CurLevelIndex(), LayerNames[LAYER_TRIGGER]);

		if (nullptr == pList)
			return;

		const char** items = new const char* [pList->size()];
		int index = 0;

		for (const auto& pObj : *pList)
		{
			std::wstring wideStr = pObj->Get_Name();
			std::string narrowStr(wideStr.begin(), wideStr.end());

			items[index] = _strdup(narrowStr.c_str());
			++index;
		}

		if (ImGui::Combo("Triggers", &m_iCurTriggerIndex, items, (_int)pList->size()))
		{
			if (0 <= m_iCurTriggerIndex)
			{
				string name{ items[m_iCurTriggerIndex] };

				CTriggerBattle* pTrigger = dynamic_cast<CTriggerBattle*>(ENGINE_INSTANCE->Get_GameObject_InCurLevel(LayerNames[LAYER_TRIGGER], Util_String::ToWString(name)));

				if (nullptr != pTrigger)
				{
					m_pCurTrigger = pTrigger;
				}
			}
		}

	}

	ImGui::SeparatorText("Cache");
	{
		if (nullptr != m_pCurTrigger)
		{
			/* 플로우 레벨 */
			if (ImGui::InputInt("input int", &m_iFlowLevel))
			{

			}

			if (ImGui::Button("Add Cache"))
			{
				CGameObject* pObject = m_pImGui_Manager->m_pCurObject;

				if (LayerNames[LAYER_ENEMY] == pObject->Get_LayerTag())
				{
					if (FAILED(m_pCurTrigger->Add_Clone(m_iFlowLevel, pObject)))
						assert(FALSE);
				}
				
			}
			ImGui::SameLine();

			if (ImGui::Button("Clear Cache"))
			{
				if (FAILED(m_pCurTrigger->Clear_Clone()))
					assert(FALSE);
			}
	
		}
	}



	ImGui::SeparatorText("Data");
	{
		if (nullptr != m_pCurTrigger)
		{
			if (ImGui::Button("Save Data"))
			{
				if (FAILED(m_pCurTrigger->Save()))
					assert(FALSE);
			}
			ImGui::SameLine();

			if (ImGui::Button("Clear Data"))
			{
				if (FAILED(m_pCurTrigger->Clear_TriggerData()))
					assert(FALSE);
			}
		}
	}
}


CImGui_Window_Mid_Trigger* CImGui_Window_Mid_Trigger::Create()
{
	CImGui_Window_Mid_Trigger* pInstance = new CImGui_Window_Mid_Trigger();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CImGui_Window_Mid_Trigger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImGui_Window_Mid_Trigger::Free()
{
	__super::Free();
}

#endif // _DEBUG