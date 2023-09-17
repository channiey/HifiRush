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
			/* ��ư�� �����ٸ� �ش� ��ư�� �ε����� ���� ������ �����Ѵ�. */
			m_pImGui_Manager->m_iIndex_CurLevelID = i;
		}
		else
		{
			/* �ƴ϶�� �����ν��Ͻ��� ���� ���� ������ �޾ƿ´�. */
			m_pImGui_Manager->m_iIndex_CurLevelID = m_pGameInstance->Get_CurLevelIndex();
		}
		delete strLevel;
	}
}

void CImGui_Window_Main_Hierarachy::Show_Hierarachy_Layers()
{
	/* ���̾� �߰� */
	if (ImGui::Button("Add"))
	{
		m_bShowMiniLayer = !m_bShowMiniLayer;
	}
	ImGui::SameLine();

	/* ���̾� ���� */
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

	/* ������ ������ ���̾���� �����´�.*/
	map<const wstring, class CLayer*>* pLayers = m_pGameInstance->Get_All_Layer(m_pImGui_Manager->m_iIndex_CurLevelID);
	NULL_CHECK(pLayers);

	if (ImGui::BeginListBox("##listbox 523", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
	{
		if (nullptr == pLayers || m_bLayerEvent)
		{
			ImGui::EndListBox();
			return;
		}

		/* ���̾ ��ȸ�ϸ� ����Ʈ�� �����Ѵ�. */
		int i = 0;
		for (auto& Pair : *pLayers)
		{
			const bool is_selected = (m_pImGui_Manager->m_iIndex_CurLayerID == i);

			const char* strLayer = StringUtils::WC2C(Pair.first.c_str());
			/* ����Ʈ�ڽ����� ���̾ ������ ��� */
			if (ImGui::Selectable(strLayer, is_selected))
			{
				m_pImGui_Manager->m_strIndex_CurLayer = Pair.first.c_str();
				m_pImGui_Manager->m_iIndex_CurLayerID = i;
			}
			else /* ���� ����Ʈ���� ���õ� ���̾ ����, ���� ���õ� ���̾ ���ٸ� ù��° ���̾�� ���� ���̾ �����Ѵ�. */
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
	/* ������Ʈ ���� */
	if (ImGui::Button("Clone"))
	{
		if (nullptr != m_pImGui_Manager->m_pCurObject)
		{
			/*FAILED_CHECK_RETURN(m_pGameInstance->Delete_GameObject(m_pGameInstance->Get_CurLevelIndex(), m_pCurObject), E_FAIL);
			m_bCurObjDeleted = TRUE;*/
		}
	}
	ImGui::SameLine();

	/* ������Ʈ ���� */
	if (ImGui::Button("Delete"))
	{
		if (nullptr != m_pImGui_Manager->m_pCurObject)
		{
			/*FAILED_CHECK_RETURN(m_pGameInstance->Delete_GameObject(m_pGameInstance->Get_CurLevelIndex(), m_pCurObject), E_FAIL);
			m_bCurObjDeleted = TRUE;*/
		}
	}

	/* ������Ʈ �˻� */
	//ImGui::Button("Find");
	//ImGui::SameLine();
	//static char str0[128] = "Enter the Name...";
	//ImGui::InputText(" ", str0, IM_ARRAYSIZE(str0));


	/* ������ ����, ���̾��� ������Ʈ���� �����´�.*/
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

				/* ����Ʈ�ڽ����� ������Ʈ�� ������ ���*/
				if (ImGui::Selectable(strObject, is_selected))
				{
					/* ������ ������Ʈ�� ���� �����ӿ��� ���õ� ������Ʈ�� ���ٸ� �ƹ��͵� ���� �ʴ´�. */
					if (m_pImGui_Manager->m_pCurObject == obj)
					{
						delete strObject;
						break;
					}

					/* ���� ��찡 �ƴ϶�� ������ ������Ʈ�� ���� ������Ʈ�� �����Ѵ�. */
					m_pImGui_Manager->m_pCurObject = obj;

					if (nullptr != m_pImGui_Manager->m_pCurObject)
					{
						m_pImGui_Manager->m_strIndex_CurObject = m_pImGui_Manager->m_pCurObject->Get_Name();
						m_pImGui_Manager->m_iIndex_CurObject = i;
					}
				}
				else  /* ���� ����Ʈ���� ���õ� ������Ʈ�� ����, ���� ���õ� ������Ʈ�� ���ٸ�(������ ���� ���ŵ� ���) ���̾���  ù��° ������Ʈ�� ���� ������Ʈ�� �����Ѵ�. */
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
		/* ���� ��ư */
		if (ImGui::Button("Finish")) m_bShowMiniLayer = FALSE;

		/* Ŭ���̾�Ʈ�� ���ǵǾ� �ִ� ���̾� ����� ��ȸ�ϸ� �����Ѵ�. */
		if (ImGui::BeginListBox("##listbox 513", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
		{
			for (size_t i = 0; i < LAYER_END; i++)
			{
				const bool is_selected = (m_iTemp == i);

				const char* strLayer = StringUtils::WC2C(gStrLayerID[i]);
				/* Ư�� ���̾ ���� �Ǿ��ٸ� �ش� ���̾ �����ν��Ͻ��� �߰��Ѵ�. */
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