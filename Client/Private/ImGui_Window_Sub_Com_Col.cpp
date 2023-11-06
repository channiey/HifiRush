#ifdef _DEBUG
#include "../Default/stdafx.h"

#include "ImGui_Window_Sub_Com_Col.h"

#include "GameObject.h"
#include "Util_String.h"



CImGui_Window_Sub_Com_Col::CImGui_Window_Sub_Com_Col()
{
}

HRESULT CImGui_Window_Sub_Com_Col::Initialize()
{
	return S_OK;
}

void CImGui_Window_Sub_Com_Col::Show_Window()
{
	ImGuiWindowFlags window_flags = 0;
	//window_flags |= ImGuiWindowFlags_NoMove;
	//window_flags |= ImGuiWindowFlags_NoResize;

	if (ImGui::Begin(m_pImGui_Manager->str_SubWindowType[m_pImGui_Manager->WINDOW_SUB_COM_COL], NULL, window_flags))
	{
		Show_Collider();
	}
	ImGui::End();

	/* Childe Window */
	Show_ChildWindow();
}

void CImGui_Window_Sub_Com_Col::Clear_Reference_Data()
{

}

void CImGui_Window_Sub_Com_Col::Show_Collider()
{
	Show_SphereCollider();
}

void CImGui_Window_Sub_Com_Col::Show_SphereCollider()
{
	CGameObject* pObj = m_pImGui_Manager->m_pCurObject;
	CCollider_Sphere* pSphereCollider = pObj->Get_Collider_Sphere();

	if (nullptr == pSphereCollider)
	{
		Set_Active(FALSE);
		return;
	}







	Vec3 vOffSet	= pSphereCollider->Get_Desc().vCenter;
	Vec3 vSize		= pSphereCollider->Get_Desc().vSize;

	_float fOffSet[3] = { vOffSet.x, vOffSet.y, vOffSet.z };
	if (ImGui::DragFloat3("OffSet", (_float*)&fOffSet, 0.5f))
	{
		pSphereCollider->Set_Collider_Offset(Vec3(fOffSet[0], fOffSet[1], fOffSet[2]));
	}

	_float fSize[3] = { vSize.x, vSize.y, vSize.z };
	if (ImGui::DragFloat3("Size", (_float*)&fSize, 0.5f))
	{
		pSphereCollider->Set_Collider_Size(fSize[0]);
	}
}

CImGui_Window_Sub_Com_Col* CImGui_Window_Sub_Com_Col::Create()
{
	CImGui_Window_Sub_Com_Col* pInstance = new CImGui_Window_Sub_Com_Col();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CImGui_Window_Sub_Com_Col");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImGui_Window_Sub_Com_Col::Free()
{
	__super::Free();
}

#endif // _DEBUG