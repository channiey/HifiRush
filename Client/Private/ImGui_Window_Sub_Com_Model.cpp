#ifdef _DEBUG
#include "../Default/stdafx.h"

#include "ImGui_Window_Sub_Com_Model.h"

#include "GameObject.h"
#include "Util_String.h"

#include "Animation.h"
#include "Cell.h"

CImGui_Window_Sub_Com_Model::CImGui_Window_Sub_Com_Model()
{
}

HRESULT CImGui_Window_Sub_Com_Model::Initialize()
{
	return S_OK;
}

void CImGui_Window_Sub_Com_Model::Show_Window()
{
	ImGuiWindowFlags window_flags = 0;
	//window_flags |= ImGuiWindowFlags_NoMove;
	//window_flags |= ImGuiWindowFlags_NoResize;

	if (ImGui::Begin(m_pImGui_Manager->str_SubWindowType[m_pImGui_Manager->WINDOW_SUB_COM_MODEL], NULL, window_flags))
	{
		Show_Animations();
	}
	ImGui::End();

	/* Childe Window */
	Show_ChildWindow();
}

void CImGui_Window_Sub_Com_Model::Clear_Reference_Data()
{
	m_iCurAnimIndex = -1;
	m_pImGui_Manager->m_bEdittingAnim = FALSE;

	CNavMeshAgent* pAgent = m_pImGui_Manager->m_pCurObject->Get_NavMeshAgent();
	if (nullptr != pAgent)
	{
		pAgent->Set_Active(TRUE);
	}
}

void CImGui_Window_Sub_Com_Model::Show_Animations()
{
	CGameObject*	pObj = m_pImGui_Manager->m_pCurObject;
	CModel*			pModel = pObj->Get_Model();
	
	if (nullptr == pModel)
	{
		Set_Active(FALSE);
		return;
	}
	if (CModel::TYPE_NONANIM == pModel->Get_Type())
	{
		Set_Active(FALSE);
		return;
	}
	
	/* Animation List */
	{
		m_pImGui_Manager->m_bEdittingAnim = !m_pImGui_Manager->m_bEdittingAnim ? TRUE : FALSE;

		const vector<class CAnimation*>& Animations = pObj->Get_Model()->Get_Animations();

		const char** items = new const char* [Animations.size()];
		int index = 0;

		for (auto& pAnim : Animations)
		{
			items[index] = _strdup(pAnim->Get_Name().c_str());
			++index;
		}

		if (ImGui::Combo("Animations", &m_iCurAnimIndex, items, (_int)Animations.size()))
		{
			if (0 <= m_iCurAnimIndex)
			{
				CNavMeshAgent* pNavMeshAgent = pObj->Get_NavMeshAgent();
				if (nullptr != pNavMeshAgent)
				{
					pNavMeshAgent->Set_Active(FALSE);
				}
				//pModel->Set_RootAnimation(TRUE);
				pModel->Set_Animation(m_iCurAnimIndex);
			}
		}

		delete[] items;
	}


	/* Animation Speed */
	{
		/*_float fAnimSpeed = pModel->Get_CurAnimation()->Get

		if (ImGui::DragFloat("Animation Speed", &fAnimSpeed, 0.01f))
		{
			pModel->Set_AnimationSpeed(fAnimSpeed);

			CNavMeshAgent* pNavMeshAgent = pObj->Get_NavMeshAgent();
			if (nullptr != pNavMeshAgent)
			{
				pNavMeshAgent->Set_Active(FALSE);
			}
			pModel->Set_RootAnimation(TRUE);
		}*/
	}
}

CImGui_Window_Sub_Com_Model* CImGui_Window_Sub_Com_Model::Create()
{
	CImGui_Window_Sub_Com_Model* pInstance = new CImGui_Window_Sub_Com_Model();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CImGui_Window_Sub_Com_Model");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImGui_Window_Sub_Com_Model::Free()
{
	__super::Free();
}

#endif // _DEBUG