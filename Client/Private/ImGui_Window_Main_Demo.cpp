#ifdef _DEBUG
#include "../Default/stdafx.h"

#include "ImGui_Window_Main_Demo.h"


CImGui_Window_Main_Demo::CImGui_Window_Main_Demo()
{
}

HRESULT CImGui_Window_Main_Demo::Initialize()
{
	return S_OK;
}

void CImGui_Window_Main_Demo::Show_Window()
{
	_bool bDemo = TRUE;

	ImGui::ShowDemoWindow(&bDemo);
}

void CImGui_Window_Main_Demo::Clear_Reference_Data()
{
}

CImGui_Window_Main_Demo* CImGui_Window_Main_Demo::Create()
{
	CImGui_Window_Main_Demo* pInstance = new CImGui_Window_Main_Demo();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CImGui_Window_Main_Demo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImGui_Window_Main_Demo::Free()
{
	__super::Free();
}

#endif // _DEBUG