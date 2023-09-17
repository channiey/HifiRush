#ifdef _DEBUG
#include "../Default/stdafx.h"

#include "ImGui_Window.h"

CImGui_Window::CImGui_Window()
	: m_pGameInstance(CGameInstance::GetInstance())
	, m_pImGui_Manager(CImGui_Manager::GetInstance())
{
	/*Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pImGui_Manager);*/
}


void CImGui_Window::Show_ChildWindow()
{
	if (m_pChildWindows.empty()) return;

	for (auto& Pair : m_pChildWindows)
	{
		if (nullptr != Pair.second && Pair.second->Is_Active())
			Pair.second->Show_Window();
	}
}

void CImGui_Window::Set_Active(const _bool& bActive)
{
	m_bActive = bActive;

	if (!m_pChildWindows.empty())
	{
		for (auto& Pair : m_pChildWindows)
			Pair.second->Set_Active(bActive);
	}
}

CImGui_Window* CImGui_Window::Find_ChildWindow(const char* WindowTag)
{
	auto pWindow = m_pChildWindows.find(WindowTag);

	if (pWindow == m_pChildWindows.end())
		return nullptr;

	return pWindow->second;
}

void CImGui_Window::Free()
{
	__super::Free();

	for (auto& Pair : m_pChildWindows)
	{
		if (Pair.second != nullptr)
			Safe_Release(Pair.second);
	}
	m_pChildWindows.clear();

	//Safe_Release(m_pGameInstance);
	//Safe_Release(m_pImGui_Manager);
}

#endif // _DEBUG