#ifdef _DEBUG
#include "../Default/stdafx.h"

#include "ImGui_Window_Main_Hierarachy.h"

#include "ImGui_Window_Sub_Prefabs.h"

#include "StringUtils.h"
#include "GameObject.h"

CImGui_Window_Main_Hierarachy::CImGui_Window_Main_Hierarachy()
{
}

HRESULT CImGui_Window_Main_Hierarachy::Initialize()
{
	/* Create Main_Window Profiler */
	CImGui_Window* pWindow = CImGui_Window_Sub_Prefabs::Create();
	NULL_CHECK_RETURN(pWindow, E_FAIL);
	pWindow->Set_Active(FALSE);

	m_pChildWindows.emplace(m_pImGui_Manager->str_SubWindowType[m_pImGui_Manager->WINDOW_SUB_PREFABS], pWindow);

	return S_OK;
}

void CImGui_Window_Main_Hierarachy::Show_Window()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;

	if (m_bLayerEvent)
		m_bLayerEvent = FALSE;

	if (ImGui::Begin(m_pImGui_Manager->str_MainWindowType[m_pImGui_Manager->WINDOW_MAIN_HIEARACHY], NULL, window_flags))
	{
		/* Function Button */
		ImGui::SeparatorText("Function");
		{
			Show_Hierarachy_FunctionButton();
		}

		/* Level Button */
		ImGui::SeparatorText("Levels");
		{
			Show_Hierarachy_Levels();
		}

		/* Layer List */
		ImGui::SeparatorText("Layers");
		{
			Show_Hierarachy_Layers();
		}

		/* Object List */
		ImGui::SeparatorText("Objects");
		{
			Show_Hierarachy_Objects();
		}

		/* Mini Layers*/
		if (m_bShowMiniLayer)
		{
			Show_MiniLayers();
		}
		
	}
	ImGui::End();
		
	/* Childe Window */
	Show_ChildWindow();
}

void CImGui_Window_Main_Hierarachy::Clear_Reference_Data()
{
}

void CImGui_Window_Main_Hierarachy::Show_Hierarachy_FunctionButton()
{
	if (ImGui::Button("Prefabs"))
	{
		CImGui_Window* pWindow = Find_ChildWindow(m_pImGui_Manager->str_SubWindowType[m_pImGui_Manager->WINDOW_SUB_PREFABS]);

		if (nullptr != pWindow)
			pWindow->Set_Active(!pWindow->Is_Active());
		
	}
	ImGui::SameLine();

	if (ImGui::Button("Save"))
	{


	}
	ImGui::SameLine();

	if (ImGui::Button("Edit"))
	{

	}
}

void CImGui_Window_Main_Hierarachy::Show_Hierarachy_Levels()
{
	for (int i = LEVEL_LOGO; i < LEVEL_ENDING; ++i)
	{
		if (i > LEVEL_LOGO)
			ImGui::SameLine();

		const char* strLevel = StringUtils::WC2C(gStrLevelID[i]);
		if (ImGui::Button(strLevel))
		{
			/* 버튼을 눌렀다면 해당 버튼의 인덱스를 현재 레벨로 지정한다. */
			m_pImGui_Manager->m_iIndex_CurLevelID = i;
		}
		else
		{
			/* 아니라면 게임인스턴스를 통해 현재 레벨을 받아온다. */
			m_pImGui_Manager->m_iIndex_CurLevelID = m_pGameInstance->Get_CurLevelIndex();
		}
		delete strLevel;
	}
}

void CImGui_Window_Main_Hierarachy::Show_Hierarachy_Layers()
{
	/* 레이어 추가 */
	if (ImGui::Button("Add"))
	{
		m_bShowMiniLayer = !m_bShowMiniLayer;
	}
	ImGui::SameLine();

	/* 레이어 삭제 */
	if (ImGui::Button("Delete"))
	{
		m_pGameInstance->Delete_Layer(m_pImGui_Manager->m_iIndex_CurLevelID, m_pImGui_Manager->m_strIndex_CurLayer);
		
		m_pImGui_Manager->m_iIndex_CurLayerID = -1;
		m_pImGui_Manager->m_strIndex_CurLayer = L"";

		m_pImGui_Manager->m_iIndex_CurObject = -1;
		m_pImGui_Manager->m_strIndex_CurObject = L"";
		m_pImGui_Manager->m_pCurObject = nullptr;

		m_bLayerEvent = TRUE;
	}

	/* 선택한 레벨의 레이어들을 가져온다.*/
	map<const wstring, class CLayer*>* pLayers = m_pGameInstance->Get_All_Layer(m_pImGui_Manager->m_iIndex_CurLevelID);
	NULL_CHECK(pLayers);

	if (ImGui::BeginListBox("##listbox 523", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
	{
		if (nullptr == pLayers || m_bLayerEvent)
		{
			ImGui::EndListBox();
			return;
		}

		/* 레이어를 순회하며 리스트로 나열한다. */
		int i = 0;
		for (auto& Pair : *pLayers)
		{
			const bool is_selected = (m_pImGui_Manager->m_iIndex_CurLayerID == i);

			const char* strLayer = StringUtils::WC2C(Pair.first.c_str());
			/* 리스트박스에서 레이어를 선택한 경우 */
			if (ImGui::Selectable(strLayer, is_selected))
			{
				m_pImGui_Manager->m_strIndex_CurLayer = Pair.first.c_str();
				m_pImGui_Manager->m_iIndex_CurLayerID = i;
			}
			else /* 만약 리스트에서 선택된 레이어가 없고, 현재 세팅된 레이어도 없다면 첫번째 레이어로 현재 레이어를 세팅한다. */
			{
				if (L"" == m_pImGui_Manager->m_strIndex_CurLayer)
				{
					m_pImGui_Manager->m_strIndex_CurLayer = Pair.first.c_str();
					m_pImGui_Manager->m_iIndex_CurLayerID = i;
				}
			}
			delete strLayer;

			if (is_selected)
				ImGui::SetItemDefaultFocus();

			++i;
		}

		ImGui::EndListBox();
	}
}

void CImGui_Window_Main_Hierarachy::Show_Hierarachy_Objects()
{
	/* 오브젝트 복제 */
	if (ImGui::Button("Clone"))
	{
		if (nullptr != m_pImGui_Manager->m_pCurObject)
		{
			/*FAILED_CHECK_RETURN(m_pGameInstance->Delete_GameObject(m_pGameInstance->Get_CurLevelIndex(), m_pCurObject), E_FAIL);
			m_bCurObjDeleted = TRUE;*/
		}
	}
	ImGui::SameLine();

	/* 오브젝트 삭제 */
	if (ImGui::Button("Delete"))
	{
		if (nullptr != m_pImGui_Manager->m_pCurObject)
		{
			/*FAILED_CHECK_RETURN(m_pGameInstance->Delete_GameObject(m_pGameInstance->Get_CurLevelIndex(), m_pCurObject), E_FAIL);
			m_bCurObjDeleted = TRUE;*/
		}
	}

	/* 오브젝트 검색 */
	//ImGui::Button("Find");
	//ImGui::SameLine();
	//static char str0[128] = "Enter the Name...";
	//ImGui::InputText(" ", str0, IM_ARRAYSIZE(str0));


	/* 선택한 레벨, 레이어의 오브젝트들을 가져온다.*/
	list<class CGameObject*>* pGameObjects = m_pGameInstance->Get_Layer(m_pImGui_Manager->m_iIndex_CurLevelID, m_pImGui_Manager->m_strIndex_CurLayer);

	if (ImGui::BeginListBox("##listbox 1", ImVec2(-FLT_MIN, 6 * ImGui::GetTextLineHeightWithSpacing())))
	{
		if (nullptr == pGameObjects || m_bLayerEvent)
		{
			ImGui::EndListBox();
			return;
		}

		if (nullptr != pGameObjects && !(*pGameObjects).empty())
		{
			int i = 0;
			for (auto& obj : *pGameObjects)
			{
				if (nullptr == obj) continue;

				const bool is_selected = (m_pImGui_Manager->m_iIndex_CurObject == i);
				const char* strObject = StringUtils::WC2C(obj->Get_Name().c_str());

				/* 리스트박스에서 오브젝트를 선택한 경우*/
				if (ImGui::Selectable(strObject, is_selected))
				{
					/* 선택한 오브젝트가 이전 프레임에서 선택된 오브젝트와 같다면 아무것도 하지 않는다. */
					if (m_pImGui_Manager->m_pCurObject == obj)
					{
						delete strObject;
						break;
					}

					/* 위의 경우가 아니라면 선택한 오브젝트를 현재 오브젝트로 세팅한다. */
					m_pImGui_Manager->m_pCurObject = obj;

					if (nullptr != m_pImGui_Manager->m_pCurObject)
					{
						m_pImGui_Manager->m_strIndex_CurObject = m_pImGui_Manager->m_pCurObject->Get_Name();
						m_pImGui_Manager->m_iIndex_CurObject = i;
					}
				}
				else  /* 만약 리스트에서 선택된 오브젝트가 없고, 현재 세팅된 오브젝트도 없다면(레벨이 새로 갱신된 경우) 레이어의  첫번째 오브젝트로 현재 오브젝트를 세팅한다. */
				{
					if (L"" == m_pImGui_Manager->m_strIndex_CurObject)
					{
						m_pImGui_Manager->m_pCurObject = obj;
						if (nullptr != m_pImGui_Manager->m_pCurObject)
						{
							m_pImGui_Manager->m_strIndex_CurObject = m_pImGui_Manager->m_pCurObject->Get_Name();
							m_pImGui_Manager->m_iIndex_CurObject = i;
						}
					}
				}
				delete strObject;

				if (is_selected)
					ImGui::SetItemDefaultFocus();

				++i;
			}
		}
		ImGui::EndListBox();
	}


}

void CImGui_Window_Main_Hierarachy::Show_MiniLayers()
{
	if (ImGui::Begin(m_pImGui_Manager->str_SubWindowType[m_pImGui_Manager->WINDOW_SUB_MINI_LAYERS]))
	{
		/* 종료 버튼 */
		if (ImGui::Button("Finish")) m_bShowMiniLayer = FALSE;

		/* 클라이언트에 정의되어 있는 레이어 목록을 순회하며 나열한다. */
		if (ImGui::BeginListBox("##listbox 513", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
		{
			for (size_t i = 0; i < LAYER_END; i++)
			{
				const bool is_selected = (m_iTemp == i);

				const char* strLayer = StringUtils::WC2C(gStrLayerID[i]);
				/* 특정 레이어가 선택 되었다면 해당 레이어를 게임인스턴스에 추가한다. */
				if (ImGui::Selectable(strLayer, is_selected))
				{
					m_pGameInstance->Add_Layer(m_pGameInstance->Get_CurLevelIndex(), gStrLayerID[i]);
					m_iTemp = i;
				}
				delete strLayer;

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox();
		}
	}
	ImGui::End();
}

CImGui_Window_Main_Hierarachy* CImGui_Window_Main_Hierarachy::Create()
{
	CImGui_Window_Main_Hierarachy* pInstance = new CImGui_Window_Main_Hierarachy();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CImGui_Window_Main_Hierarachy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImGui_Window_Main_Hierarachy::Free()
{
	__super::Free();
}

#endif // _DEBUG