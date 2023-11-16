#ifdef _DEBUG
#include "../Default/stdafx.h"

#include "ImGui_Window_Mid_Ui.h"
#include "GameObject.h"

#include "Util_String.h"
#include "Util_File.h"

#include "Ui.h"

CImGui_Window_Mid_Ui::CImGui_Window_Mid_Ui()
{
}

HRESULT CImGui_Window_Mid_Ui::Initialize()
{

	return S_OK;
}

void CImGui_Window_Mid_Ui::Show_Window()
{
	ImGuiWindowFlags window_flags = 0;
	//window_flags |= ImGuiWindowFlags_NoMove;
	//window_flags |= ImGuiWindowFlags_NoResize;

	if (ImGui::Begin(m_pImGui_Manager->str_MainWindowType[m_pImGui_Manager->WINDOW_MAIN_UI], NULL, window_flags))
	{
		if (nullptr != m_pImGui_Manager->m_pCurObject && LayerNames[LAYER_UI] == m_pImGui_Manager->m_pCurObject->Get_LayerTag())
		{
			m_pUi = dynamic_cast<CUi*>(m_pImGui_Manager->m_pCurObject);
			
			if (nullptr != m_pUi)
			{
				ImGui::SeparatorText("Textures");
				{
					Show_Textures();
				}
				
				ImGui::SeparatorText("Desc");
				{
					Show_TextureDesc();
				}

				ImGui::SeparatorText("File");
				{
					if (ImGui::Button("Save"))
					{
						Save();
					}
					ImGui::SameLine();


					if (ImGui::Button("Load"))
					{
						Load();
					}
				}
			}
		}
	}
	ImGui::End();

	/* Childe Window */
	Show_ChildWindow();
}

void CImGui_Window_Mid_Ui::Clear_Reference_Data()
{
	m_pUi = nullptr;
	m_iIndex = -1;
}

HRESULT CImGui_Window_Mid_Ui::Save()
{
	return m_pUi->Save();
}

HRESULT CImGui_Window_Mid_Ui::Load()
{
	return m_pUi->Load();
}

void CImGui_Window_Mid_Ui::Show_Textures()
{
	vector<CTexture*>* pTextureComs = m_pUi->Get_TextureComs();

	if (nullptr == pTextureComs) return;

	const char** items = new const char* [(*pTextureComs).size()];
	int index = 0;

	for (auto& pTexture : *pTextureComs)
	{
		std::wstring wideStr = Util_String::ToWString(pTexture->Get_TextureName());
		std::string narrowStr(wideStr.begin(), wideStr.end());

		items[index] = _strdup(narrowStr.c_str());
		++index;
	}


	if (ImGui::Combo("Textures", &m_iIndex, items, (_int)(*pTextureComs).size()))
	{

	}


	delete[] items;
}

void CImGui_Window_Mid_Ui::Show_TextureDesc()
{
	if (0 > m_iIndex) return;

	CUi::UI_DESC* pDesc = m_pUi->Get_Desc(m_iIndex);

	if (nullptr == pDesc) return;

	/* Pos */
	{
		Vec2	vPos	= pDesc->Get_Pos();
		_float	fPos[2] = { vPos.x, vPos.y };

		if (ImGui::DragFloat2("Local Pos", (_float*)&fPos, 0.1f))
			pDesc->Set_Pos(Vec2{ fPos[0], fPos[1] });
	}

	/* Size */
	{
		Vec2	vSize		= pDesc->Get_Size();
		_float	fSize[2]	= { vSize.x, vSize.y };

		if (ImGui::DragFloat2("Local Size", (_float*)&fSize, 0.01f))
			pDesc->Set_Size(Vec2{ fSize[0], fSize[1] });
	}
}



CImGui_Window_Mid_Ui* CImGui_Window_Mid_Ui::Create()
{
	CImGui_Window_Mid_Ui* pInstance = new CImGui_Window_Mid_Ui();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CImGui_Window_Mid_Ui");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImGui_Window_Mid_Ui::Free()
{
	__super::Free();
}

#endif // _DEBUG