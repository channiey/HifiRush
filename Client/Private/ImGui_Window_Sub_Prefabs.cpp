#ifdef _DEBUG
#include "../Default/stdafx.h"

#include "ImGui_Window_Sub_Prefabs.h"
#include "StringUtils.h"

CImGui_Window_Sub_Prefabs::CImGui_Window_Sub_Prefabs()
{
}

HRESULT CImGui_Window_Sub_Prefabs::Initialize()
{
	return S_OK;
}

void CImGui_Window_Sub_Prefabs::Show_Window()
{
	ImGuiWindowFlags window_flags = 0;
	//window_flags |= ImGuiWindowFlags_NoMove;
	//window_flags |= ImGuiWindowFlags_NoResize;

	if (ImGui::Begin(m_pImGui_Manager->str_SubWindowType[m_pImGui_Manager->WINDOW_SUB_PREFABS], NULL, window_flags))
	{
		/* 인스턴스화 */
		if (ImGui::Button("instantiate"))
		{
			/* 레벨 인덱스, 레이어 태그, 오브젝트 이름을 매개변수로 해당 프로토타입을 클론한다. */

			/* 클론 위치는 현재 화면의 중앙으로 한다.*/

			/* 하이어러키의 현재 오브젝트를 이 오브젝트로 변경한다. */
		}
		ImGui::SameLine();

		/* 프로토 타입 검색*/
		if (ImGui::Button("Find"))
		{

		}
		ImGui::SameLine();
		static char str0[128] = "Enter the Name...";
		ImGui::InputText(" ", str0, IM_ARRAYSIZE(str0));


		/* 프로토 타입을 가져온다. */
		map<const wstring, class CGameObject*>* pPrototypes = m_pGameInstance->Get_Prototypes();

		if (ImGui::BeginListBox("##listbox 734", ImVec2(-FLT_MIN, 15 * ImGui::GetTextLineHeightWithSpacing())))
		{
			/* 프로토 타입 맵 포인터가 비어있다면 리턴 */
			if (nullptr == pPrototypes)
			{
				ImGui::EndListBox();
				return;
			}

			/* 프로토 타입 맵을 순회한다. */
			int i = 0;
			for (auto& Pair : *pPrototypes)
			{
				const bool is_selected = (m_pImGui_Manager->m_iIndex_PrefabObject == i);

				const char* strPrefabObj = StringUtils::WC2C(Pair.first.c_str());
				/* 프로토 타입 오브젝트를 선택한 경우*/
				if (ImGui::Selectable(strPrefabObj, is_selected))
				{
					m_pImGui_Manager->m_strIndex_PrefabObject = Pair.first.c_str();
					m_pImGui_Manager->m_iIndex_PrefabObject = i;
				}
				delete strPrefabObj;

				if (is_selected)
					ImGui::SetItemDefaultFocus();

				++i;
			}

			ImGui::EndListBox();
		}
	}
	ImGui::End();

	/* Childe Window */
	Show_ChildWindow();
}

void CImGui_Window_Sub_Prefabs::Clear_Reference_Data()
{
}

CImGui_Window_Sub_Prefabs* CImGui_Window_Sub_Prefabs::Create()
{
	CImGui_Window_Sub_Prefabs* pInstance = new CImGui_Window_Sub_Prefabs();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CImGui_Window_Sub_Prefabs");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImGui_Window_Sub_Prefabs::Free()
{
	__super::Free();
}

#endif // _DEBUG