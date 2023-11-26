#ifdef _DEBUG
#include "../Default/stdafx.h"

#include "ImGui_Window_Sub_Prefabs.h"
#include "GameObject.h"
#include "Util_String.h"

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
			if (-1 != m_pImGui_Manager->m_iIndex_CurLevelID
				&& L"" != m_pImGui_Manager->m_strIndex_CurLayer
				&& nullptr != m_pImGui_Manager->m_pPrefabObj)
			{
				CGameObject* pGameObject = m_pEngineInstance->Add_GameObject(
					m_pImGui_Manager->m_iIndex_CurLevelID,
					m_pImGui_Manager->m_strIndex_CurLayer,
					m_pImGui_Manager->m_pPrefabObj->Get_Name());

				if (nullptr != pGameObject)
				{
					Vec4 vPos = ENGINE_INSTANCE->Get_CurCamera()->Get_Transform()->Get_State(CTransform::STATE_LOOK).Normalized() * 25.f;
					pGameObject->Get_Transform()->Set_Position(vPos.ZeroW() + ENGINE_INSTANCE->Get_CurCamera()->Get_Transform()->Get_FinalPosition());

					m_pImGui_Manager->m_pCurObject = pGameObject;
				}
			}
		}
		ImGui::SameLine();

		/* 프로토 타입 검색 */
		if (ImGui::Button("Find"))
		{

		}
		ImGui::SameLine();
		static char str0[128] = "Enter the Name...";
		ImGui::InputText(" ", str0, IM_ARRAYSIZE(str0));


		/* 프로토 타입을 가져온다. */
		map<const wstring, class CGameObject*>* pPrototypes = m_pEngineInstance->Get_Prototypes();

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

				const char* strPrefabObj = Util_String::WC2C(Pair.first.c_str());
				/* 프로토 타입 오브젝트를 선택한 경우*/
				if (ImGui::Selectable(strPrefabObj, is_selected))
				{
					m_pImGui_Manager->m_strIndex_PrefabObject = Pair.first.c_str();
					m_pImGui_Manager->m_iIndex_PrefabObject = i;
					m_pImGui_Manager->m_pPrefabObj = Pair.second;
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