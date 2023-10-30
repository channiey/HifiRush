#ifdef _DEBUG
#include "../Default/stdafx.h"

#include "ImGui_Window_Main_Hierarachy.h"

#include "ImGui_Window_Sub_Prefabs.h"

#include "Util_String.h"
#include "Util_File.h"

#include "GameObject.h"
#include "Level_Loading.h"
#include "Layer.h"

CImGui_Window_Main_Hierarachy::CImGui_Window_Main_Hierarachy()
{
}

HRESULT CImGui_Window_Main_Hierarachy::Initialize()
{
	/* Create Sub_Window_Prefabs */
	CImGui_Window* pWindow = CImGui_Window_Sub_Prefabs::Create();
	NULL_CHECK_RETURN(pWindow, E_FAIL);
	pWindow->Set_Active(FALSE);

	m_pChildWindows.emplace(m_pImGui_Manager->str_SubWindowType[m_pImGui_Manager->WINDOW_SUB_PREFABS], pWindow);

	return S_OK;
}

void CImGui_Window_Main_Hierarachy::Show_Window()
{
	ImGuiWindowFlags window_flags = 0;
	/*window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;*/

	if (m_bLayerEvent) m_bLayerEvent = FALSE;
	if (m_bObjectEvent) m_bObjectEvent = FALSE;

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

		/* Piciking */
		if (m_bPickMode)
		{
			if (ENGINE_INSTANCE->Key_Down(VK_RBUTTON))
			{
				RAYHIT_DESC hit = ENGINE_INSTANCE->Check_ScreenRay(LayerNames[LAYER_ENV_STATIC], FALSE);

				if (nullptr != hit.pGameObject)
				{
					if (nullptr != m_pImGui_Manager->m_pCurObject && m_pImGui_Manager->m_pCurObject->Is_Picked())
						m_pImGui_Manager->m_pCurObject->Set_Picked(FALSE);

					m_pImGui_Manager->m_pCurObject = hit.pGameObject;
					m_pImGui_Manager->m_pCurObject->Set_Picked(TRUE);

				}
			}
		}
		
	}
	ImGui::End();
		
	/* Childe Window */
	Show_ChildWindow();
}

void CImGui_Window_Main_Hierarachy::Clear_Reference_Data()
{
	if (nullptr != m_pImGui_Manager->m_pCurObject)
		m_pImGui_Manager->m_pCurObject->Set_Picked(FALSE);

	m_bPickMode = FALSE;
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
		Save_LevelData();
	}
	ImGui::SameLine();

	if (ImGui::Button("Edit"))
	{

	}
}

void CImGui_Window_Main_Hierarachy::Show_Hierarachy_Levels()
{
	for (int i = LV_LOGO; i < LV_END; ++i)
	{
		if (LV_LOGO < i && LV_STAGE_03 != i)
			ImGui::SameLine();

		const char* strLevel = Util_String::WC2C(LevelNames[i]);
		if (ImGui::Button(strLevel))
		{
			/* ��ư�� �����ٸ� �ش� ��ư�� �ε����� ���� ������ �����Ѵ�. */
			//m_pImGui_Manager->m_iIndex_CurLevelID = i;

			if(i != m_pImGui_Manager->m_iIndex_CurLevelID)
				m_pEngineInstance->Open_Level(LV_LOADING, CLevel_Loading::Create(m_pImGui_Manager->m_pDevice, m_pImGui_Manager->m_pContext, (LEVEL_ID)i));
			
			/*m_pImGui_Manager->Set_Active_Main_Window(m_pImGui_Manager->WINDOW_MAIN_OBJECT_INFO, FALSE);
			m_pImGui_Manager->Set_Active_Main_Window(m_pImGui_Manager->WINDOW_MAIN_HIEARACHY, FALSE);
			m_pImGui_Manager->Set_Active_Main_Window(m_pImGui_Manager->WINDOW_MAIN_DEMO, FALSE);*/
		}
		else
		{
			/* �ƴ϶�� �����ν��Ͻ��� ���� ���� ������ �޾ƿ´�. */
			m_pImGui_Manager->m_iIndex_CurLevelID = m_pEngineInstance->Get_CurLevelIndex();
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
		m_pEngineInstance->Delete_Layer(m_pImGui_Manager->m_iIndex_CurLevelID, m_pImGui_Manager->m_strIndex_CurLayer);
		
		m_pImGui_Manager->Reset_Index_CurLayer();
		m_pImGui_Manager->Reset_Index_CurObject();

		m_bLayerEvent = TRUE;
	}
	ImGui::SameLine();

	/* ���̾� ���� */
	if (ImGui::Button("Render"))
	{
		/*if (nullptr != m_pImGui_Manager->m_pCurObject)
		{
			m_pEngineInstance->Delete_GameObject(m_pEngineInstance->Get_CurLevelIndex(), m_pImGui_Manager->m_pCurObject);
			m_pImGui_Manager->Reset_Index_CurObject();

			m_bObjectEvent = TRUE;
		}*/
	}

	/* ������ ������ ���̾���� �����´�.*/
	map<const wstring, class CLayer*>* pLayers = m_pEngineInstance->Get_All_Layer(m_pImGui_Manager->m_iIndex_CurLevelID);
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

			const char* strLayer = Util_String::WC2C(Pair.first.c_str());
			/* ����Ʈ�ڽ����� ���̾ ������ ��� */
			if (ImGui::Selectable(strLayer, is_selected))
			{
				m_pImGui_Manager->m_strIndex_CurLayer = Pair.first.c_str();
				m_pImGui_Manager->m_iIndex_CurLayerID = i;
				m_pImGui_Manager->Reset_Index_CurObject();
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
			CGameObject* pClone = m_pEngineInstance->Add_GameObject(
										m_pImGui_Manager->m_iIndex_CurLevelID,
										m_pImGui_Manager->m_strIndex_CurLayer,
										m_pImGui_Manager->m_pCurObject->Get_Name());
			if (nullptr != pClone)
			{
				Safe_AddRef(pClone);
				pClone->Get_Transform()->Set_WorldMat(m_pImGui_Manager->m_pCurObject->Get_Transform()->Get_WorldMat());
				Safe_Release(pClone);
			}
		}
	}
	ImGui::SameLine();

	/* ������Ʈ ���� */
	if (ImGui::Button("Remove")) 
	{
		if (nullptr != m_pImGui_Manager->m_pCurObject)
		{
			m_pEngineInstance->Delete_GameObject(m_pEngineInstance->Get_CurLevelIndex(), m_pImGui_Manager->m_pCurObject);
			m_pImGui_Manager->Reset_Index_CurObject();

			m_bObjectEvent = TRUE;
		}
	}
	ImGui::SameLine();

	/* ������Ʈ ��ŷ */
	if (ImGui::Checkbox("Pick", &m_bPickMode))
	{
		if (nullptr != m_pImGui_Manager->m_pCurObject && m_pImGui_Manager->m_pCurObject->Is_Picked())
			m_pImGui_Manager->m_pCurObject->Set_Picked(TRUE);
	}

	/* ������ ����, ���̾��� ������Ʈ���� �����´�.*/
	list<class CGameObject*>* pGameObjects = m_pEngineInstance->Get_Layer(m_pImGui_Manager->m_iIndex_CurLevelID, m_pImGui_Manager->m_strIndex_CurLayer);

	if (ImGui::BeginListBox("##listbox 1", ImVec2(-FLT_MIN, 7 * ImGui::GetTextLineHeightWithSpacing())))
	{
		if (nullptr == pGameObjects || m_bLayerEvent || m_bObjectEvent)
		{
			ImGui::EndListBox();
			return;
		}

		if ((*pGameObjects).empty())
		{
			m_pImGui_Manager->Reset_Index_CurObject();
		}

		if (nullptr != pGameObjects && !(*pGameObjects).empty())
		{
			int i = 0;
			for (auto& obj : *pGameObjects)
			{
				if (nullptr == obj) continue;

				const bool is_selected = (m_pImGui_Manager->m_iIndex_CurObject == i);
				const char* strObject = Util_String::WC2C(obj->Get_Name().c_str());

				/* ����Ʈ�ڽ����� ������Ʈ�� ������ ���*/
				if (ImGui::Selectable(strObject, is_selected))
				{
					m_pImGui_Manager->m_iIndex_CurObject = i;

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
		//if (ImGui::Button("Finish")) m_bShowMiniLayer = FALSE;

		/* Ŭ���̾�Ʈ�� ���ǵǾ� �ִ� ���̾� ����� ��ȸ�ϸ� �����Ѵ�. */
		if (ImGui::BeginListBox("##listbox 513", ImVec2(-FLT_MIN, 10 * ImGui::GetTextLineHeightWithSpacing())))
		{
			for (size_t i = 0; i < LAYER_END; i++)
			{
				const bool is_selected = (m_iTempMiniLayer == i);

				const char* strLayer = Util_String::WC2C(LayerNames[i]);
				/* Ư�� ���̾ ���� �Ǿ��ٸ� �ش� ���̾ �����ν��Ͻ��� �߰��Ѵ�. */
				if (ImGui::Selectable(strLayer, is_selected))
				{
					m_pEngineInstance->Add_Layer(m_pEngineInstance->Get_CurLevelIndex(), LayerNames[i]);
					m_iTempMiniLayer = (_uint)i;
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

void CImGui_Window_Main_Hierarachy::Save_LevelData()
{
	shared_ptr<Util_File> file = make_shared<Util_File>();
	file->Open(LevelPaths[m_pImGui_Manager->m_iIndex_CurLevelID], FileMode::Write);

	/* ���� ������ ��� ���̾ �����´�. */
	map<const wstring, class CLayer*>* pLayers = m_pEngineInstance->Get_All_Layer(m_pImGui_Manager->m_iIndex_CurLevelID);
	if (nullptr == pLayers)
		return;

	/* ���� ���͸� - ����� ������ �ʿ� ���� */
	{
		auto iter = pLayers->find(LayerNames[LAYER_WEAPON]);

		if(iter != pLayers->end())
			file->Write<size_t>(pLayers->size() -1); 
		else
			file->Write<size_t>(pLayers->size());
	}


	/* ���� ������ ��� ���̾ ��ȸ�Ѵ�. */
	for (auto& Pair : *pLayers)
	{
		if (nullptr == Pair.second) continue;

		/* ����� ������ ���� ĳ���� ��ü �����̹Ƿ� �ǳ� �ڴ�. */
		{
			if (LayerNames[LAYER_WEAPON] == Pair.first) continue;
		}

		file->Write<size_t>(Pair.second->Get_Objects()->size());

		/* ���̾� ���� ������Ʈ ����Ʈ�� ��ȸ�Ѵ�. */
		for (auto& obj : *Pair.second->Get_Objects())
		{
			/* �̸� */
			wstring strName = Util_String::Remove_LastNumChar(obj->Get_Name(), CLONE_PIN_MAX_DIGIT);
			file->Write<string>(Util_String::ToString(strName));

			/* ���̾� */
			file->Write<string>(Util_String::ToString(obj->Get_LayerTag()));

			/* ��Ƽ�� ���� */
			file->Write<_uint>(obj->Get_State());

			/* ���� ���� */
			file->Write<_bool>(obj->Is_Render());

			/* Ʈ������ */
			file->Write<Matrix>(obj->Get_Transform()->Get_WorldMat());

			/* �׺���̼� */
			CNavMeshAgent* pCom = obj->Get_NavMeshAgent();
			if (nullptr != pCom)
			{
				file->Write<_bool>(TRUE);
				file->Write<_int>(pCom->Get_Index());
			}
			else
				file->Write<_bool>(FALSE);
		}
	}
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