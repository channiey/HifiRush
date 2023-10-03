#ifdef _DEBUG
#include "../Default/stdafx.h"

#include "ImGui_Window_Main_Object.h"

#include "GameObject.h"
#include "Util_String.h"


CImGui_Window_Main_Object::CImGui_Window_Main_Object()
{
}

HRESULT CImGui_Window_Main_Object::Initialize()
{
	/* 컴포넌트 창 생성 */


	return S_OK;
}

void CImGui_Window_Main_Object::Show_Window()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;

	if (ImGui::Begin(m_pImGui_Manager->str_MainWindowType[m_pImGui_Manager->WINDOW_MAIN_OBJECT_INFO], NULL, window_flags))
	{
		pObject = m_pImGui_Manager->m_pCurObject;
		if (nullptr != pObject) Safe_AddRef(pObject);
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
		Safe_Release(pObject);
	}
	ImGui::End();

	/* Childe Window */
	Show_ChildWindow();
}

void CImGui_Window_Main_Object::Clear_Reference_Data()
{
	pObject = nullptr;
}

void CImGui_Window_Main_Object::Show_Object_Info()
{
	/* Name */
	ImGui::Text("Name : ");
	ImGui::SameLine();

	if (nullptr != pObject)
	{
		const char* strName = Util_String::WC2C(pObject->Get_Name().c_str());
		ImGui::Text(strName);
		delete strName;
	}
	else
	{
		ImGui::Text("");
	}


	/* Active */
	_bool bActive = TRUE;
	if (nullptr != pObject)
		bActive = pObject->Is_Active();

	if (ImGui::Checkbox("Active", &bActive))
	{
		if (nullptr != pObject)
			pObject->Set_Active(bActive);
	}
	ImGui::SameLine();

	/* Render */
	_bool bRender = TRUE;
	if (nullptr != pObject)
		bRender = pObject->Is_Render();

	if (ImGui::Checkbox("Render", &bRender))
	{
		if (nullptr != pObject)
			pObject->Set_Render(bRender);
	}

}

_float fRotPrev[3] = { 0.f, 0.f, 0.f };

void CImGui_Window_Main_Object::Show_Object_Transform()
{
	if (nullptr != pObject)
	{
		CTransform* pTransform = pObject->Get_Transform();

		if (nullptr != pTransform)
		{
			Safe_AddRef(pTransform);
			{
				Vec4 vVec{};

				/* Position */
				vVec = pTransform->Get_State(CTransform::STATE_POSITION);
				_float fPos[3] = { vVec.x, vVec.y, vVec.z };
				if (ImGui::InputFloat3("Pos", fPos, "%.1f"))
				{
					pTransform->Set_State(CTransform::STATE_POSITION, Vec4(fPos[0], fPos[1], fPos[2], 1));
				}

				/* Rotation */
				Vec3 vRot = pTransform->Get_Rotation(); /* Euler */
				_float fRot[3] = { vRot.x, vRot.y, vRot.z };
				if (ImGui::InputFloat3("Rot", fRot, "%.1f"))
				{
					if (vRot.x != fRot[0] || vRot.y != fRot[1] || vRot.z != fRot[2])
					{
						if (fRot[0] != fRotPrev[0])
						{
							for (size_t i = 0; i < 3; i++)
								fRotPrev[i] = fRot[i];
							
							pTransform->Set_Rotation(Vec3::Right, fRot[0]);
						}
						else if (fRot[1] != fRotPrev[1])
						{
							for (size_t i = 0; i < 3; i++)
								fRotPrev[i] = fRot[i];
							pTransform->Set_Rotation(Vec3::Up, fRot[1]);
						}
						else if (fRot[2] != fRotPrev[2])
						{
							for (size_t i = 0; i < 3; i++)
								fRotPrev[i] = fRot[i];
							pTransform->Set_Rotation(Vec3::Forward, fRot[2]);
						}
					}
				}

				/* Scale*/
				vVec = pTransform->Get_Scale();
				_float fSacle[3] = { vVec.x, vVec.y, vVec.z };
				if (ImGui::InputFloat3("Scale", fSacle, "%.1f"))
				{
					pTransform->Set_Scale(Vec3(fSacle[0], fSacle[1], fSacle[2]));
				}
			}
			Safe_Release(pTransform);
		}
	}
	else
	{
		float fZero[3] = { 0.f, 0.f, 0.f };
		ImGui::InputFloat3("Pos", fZero, "%.1f");
		ImGui::InputFloat3("Rot", fZero, "%.1f");
		ImGui::InputFloat3("Scale", fZero, "%.1f");
	}
}

void CImGui_Window_Main_Object::Shwo_Object_Component()
{
	NULL_CHECK(pObject);

	map<const wstring, class CComponent*>& Components = pObject->Get_Components();

	_uint i = 0;
	for (auto& Pair : Components)
	{
		if (nullptr == Pair.second) continue;

		if (i > 0) ImGui::SameLine();

		++i;

		/* 컴포넌트가 스테이트머신, 행동트리인 경우 버튼으로 표시한다. */
		//const char* strComponent = Util_String::WC2C(Pair.first.c_str());
		//if (strcmp(strComponent, Util_String::ToString(gStrComponentType[COM_STATEMACHINE]).c_str()) ||
		//	strcmp(strComponent, Util_String::ToString(gStrComponentType[COM_BEHAVIOURTREE]).c_str()))
		//{
		//	delete strComponent;
		//	continue;
		//}

		//if (ImGui::Button(strComponent))
		//{

		//}
		//delete strComponent;
	}
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