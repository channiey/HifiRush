#ifdef _DEBUG
#include "../Default/stdafx.h"

#include "ImGui_Window.h"

CImGui_Window::CImGui_Window()
	: m_pEngineInstance(CEngineInstance::GetInstance())
	, m_pImGui_Manager(CImGui_Manager::GetInstance())
{
	/*Safe_AddRef(m_pEngineInstance);
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

	if (!m_pChildWindows.empty() && !bActive)
	{
		for (auto& Pair : m_pChildWindows)
			Pair.second->Set_Active(bActive);
	}
}

CImGui_Window* CImGui_Window::Find_ChildWindow(const char* WindowTag)
{
	const string strWindowTag = WindowTag;

	for (auto& Pair : m_pChildWindows)
	{
		if (strWindowTag == string(Pair.first))
		{
			return Pair.second;
		}
	}

	return nullptr;
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

	//Safe_Release(m_pEngineInstance);
	//Safe_Release(m_pImGui_Manager);
}

#endif // _DEBUG