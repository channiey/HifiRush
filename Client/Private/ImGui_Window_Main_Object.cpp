#ifdef _DEBUG
#include "../Default/stdafx.h"

#include "ImGui_Window_Main_Object.h"

#include "ImGui_Window_Sub_Com_Model.h"
#include "ImGui_Window_Sub_Com_Col.h"

#include "GameObject.h"
#include "Util_String.h"


CImGui_Window_Main_Object::CImGui_Window_Main_Object()
{
}

HRESULT CImGui_Window_Main_Object::Initialize()
{
	CImGui_Window* pWindow = nullptr;

	/* Component - Anim */
	pWindow = CImGui_Window_Sub_Com_Model::Create();
	{
		if (nullptr == pWindow)
			return E_FAIL;

		pWindow->Set_Active(FALSE);
		m_pChildWindows.emplace(m_pImGui_Manager->str_SubWindowType[m_pImGui_Manager->WINDOW_SUB_COM_MODEL], pWindow);
	}

	/* Component - Col */
	pWindow = CImGui_Window_Sub_Com_Col::Create();
	{
		if (nullptr == pWindow)
			return E_FAIL;

		pWindow->Set_Active(FALSE);
		m_pChildWindows.emplace(m_pImGui_Manager->str_SubWindowType[m_pImGui_Manager->WINDOW_SUB_COM_COL], pWindow);
	}

	return S_OK;
}

void CImGui_Window_Main_Object::Show_Window()
{
	ImGuiWindowFlags window_flags = 0;
	/*window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;*/

	if (ImGui::Begin(m_pImGui_Manager->str_MainWindowType[m_pImGui_Manager->WINDOW_MAIN_OBJECT_INFO], NULL, window_flags))
	{
		m_pObject = m_pImGui_Manager->m_pCurObject;
		if (nullptr != m_pObject) Safe_AddRef(m_pObject);
		{
			/* Info */
			ImGui::SeparatorText("Object_Info");
			{
				Show_Object_Info();
			}

			/* Transform */
			ImGui::SeparatorText("Transform");
			{
				Show_Object_Transform();
			}

			/* Component Button */
			ImGui::SeparatorText("Components");
			{
				Shwo_Object_Component();
			}
		}
		Safe_Release(m_pObject);
	}
	ImGui::End();

	/* Childe Window */
	Show_ChildWindow();
}

void CImGui_Window_Main_Object::Clear_Reference_Data()
{
	m_pObject = nullptr;
	m_iCurComIndex = 0;
	m_pPrevComWindow = nullptr;
}

void CImGui_Window_Main_Object::Show_Object_Info()
{
	/* Name */
	ImGui::Text("Name : ");
	ImGui::SameLine();

	if (nullptr != m_pObject)
	{
		const char* strName = Util_String::WC2C(m_pObject->Get_Name().c_str());
		ImGui::Text(strName);
		delete strName;
	}
	else
	{
		ImGui::Text("");
	}


	/* Active */
	_bool bActive = TRUE;
	if (nullptr != m_pObject)
		bActive = m_pObject->Is_Active();

	if (ImGui::Checkbox("Active", &bActive))
	{
		if (bActive)
			m_pObject->Set_State(CGameObject::STATE_ACTIVE);
		else
			m_pObject->Set_State(CGameObject::STATE_UNACTIVE);
	}
	ImGui::SameLine();

	/* Render */
	_bool bRender = TRUE;
	if (nullptr != m_pObject)
		bRender = m_pObject->Is_Render();

	if (ImGui::Checkbox("Render", &bRender))
	{
		if (nullptr != m_pObject)
			m_pObject->Set_Render(bRender);
	}

}

_float fRotPrev[3] = { 0.f, 0.f, 0.f };

void CImGui_Window_Main_Object::Show_Object_Transform()
{
	if (nullptr != m_pObject)
	{
		CTransform* pTransform = m_pObject->Get_Transform();

		if (nullptr != pTransform)
		{
			Safe_AddRef(pTransform);
			{
				Vec4 vVec{};

				/* Position */
				{}
				vVec = pTransform->Get_FinalPosition();
				_float fPos[3] = { vVec.x, vVec.y, vVec.z };
				if(ImGui::DragFloat3("Pos", (_float*)&fPos, 0.5f))
					pTransform->Set_State(CTransform::STATE_POSITION, Vec4(fPos[0], fPos[1], fPos[2], 1));

				/* Rotation */
				Vec3 vRot = pTransform->Get_Rotation(); /* Euler */
				_float fRot[3] = { vRot.x, vRot.y, vRot.z };
				if (ImGui::DragFloat3("Rot", (_float*)&fRot, 0.5f))
				{
					if (vRot.x != fRot[0] || vRot.y != fRot[1] || vRot.z != fRot[2])
					{
						if (fRot[0] != fRotPrev[0])
						{
							for (size_t i = 0; i < 3; i++)
								fRotPrev[i] = fRot[i];

							pTransform->Set_Rotation(Vec3::Right, fRot[0] * -1.f);
						}
						else if (fRot[1] != fRotPrev[1])
						{
							for (size_t i = 0; i < 3; i++)
								fRotPrev[i] = fRot[i];
							pTransform->Set_Rotation(Vec3::Up, fRot[1] * -1.f);
						}
						else if (fRot[2] != fRotPrev[2])
						{
							for (size_t i = 0; i < 3; i++)
								fRotPrev[i] = fRot[i];
							pTransform->Set_Rotation(Vec3::Forward, fRot[2] * -1.f);
						}
					}
				}

				/* Scale*/
				vVec = pTransform->Get_Scale();
				_float fSacle[3] = { vVec.x, vVec.y, vVec.z };
				if (ImGui::DragFloat3("Scale", (_float*)&fSacle, 0.5f))
					pTransform->Set_Scale(Vec3(fSacle[0], fSacle[1], fSacle[2]));	
			}
			Safe_Release(pTransform);
		}
	}
	else
	{
		float fZero[3] = { 0.f, 0.f, 0.f };
		ImGui::DragFloat3("Pos", (_float*)&fZero, 0.5f);
		ImGui::DragFloat3("Rot", (_float*)&fZero, 0.5f);
		ImGui::DragFloat3("Scale", (_float*)&fZero, 0.5f);
	}
}

void CImGui_Window_Main_Object::Shwo_Object_Component()
{
	NULL_CHECK(m_pObject);

	map<const wstring, class CComponent*>& Components = m_pObject->Get_Components();

	const char** items = new const char* [Components.size()];
	int index = 0;

	for (const auto& pair : Components) 
	{
		std::wstring wideStr = pair.first;
		std::string narrowStr(wideStr.begin(), wideStr.end());

		items[index] = _strdup(narrowStr.c_str());
		++index;
	}

	if (ImGui::Combo("Component", &m_iCurComIndex, items, (_int)Components.size()))
	{
		if (0 <= m_iCurComIndex)
		{
			CComponent* pComponent = m_pImGui_Manager->m_pCurObject->Get_Component(m_iCurComIndex);

			if (nullptr != pComponent)
			{
				CImGui_Window* pWindow = Find_ChildWindow(Util_String::ToString(pComponent->Get_Name()).c_str());

				if (nullptr != pWindow && m_pPrevComWindow != pWindow)
				{
					pWindow->Set_Active(TRUE);

					/*if (nullptr != m_pPrevComWindow)
						m_pPrevComWindow->Set_Active(FALSE);

					m_pPrevComWindow = pWindow;*/
				}
			}
		}
	}

	delete[] items;
}

CImGui_Window_Main_Object* CImGui_Window_Main_Object::Create()
{
	CImGui_Window_Main_Object* pInstance = new CImGui_Window_Main_Object();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CImGui_Window_Main_Object");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImGui_Window_Main_Object::Free()
{
	__super::Free();
}

#endif // _DEBUG