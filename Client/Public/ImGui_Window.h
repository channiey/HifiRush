#pragma once
#ifdef _DEBUG

#include "../ImGui/imgui.h" 
#include "../ImGui/imgui_impl_win32.h" 
#include "../ImGui/imgui_impl_dx11.h" 

#include "GameInstance.h"
#include "ImGui_Manager.h"

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
END

BEGIN(Client)

class CImGui_Window abstract : public CBase
{
protected:
	CImGui_Window();
	virtual~CImGui_Window() = default;

public:
	virtual HRESULT Initialize() PURE;
	virtual void Show_Window() PURE;
	virtual void Show_ChildWindow();

public:
	virtual const _bool& Is_Active() const { return m_bActive; }
	virtual void Set_Active(const _bool& bActive);
	virtual void Set_Owner(CImGui_Window* pOwnerWindow) { m_pOnwerWindow = pOwnerWindow; }

public:
	virtual void Clear_Reference_Data() PURE;

protected:
	class CImGui_Manager* m_pImGui_Manager = { nullptr };
	CGameInstance*		  m_pGameInstance = { nullptr };

	map<const char*, CImGui_Window*> m_pChildWindows;

protected:
	CImGui_Window* m_pOnwerWindow = { nullptr };
	CImGui_Window* Find_ChildWindow(const char* WindowTag);

protected:
	_bool	m_bActive = { TRUE };

public:
	virtual void Free() override;
};
END

#endif // _DEBUG