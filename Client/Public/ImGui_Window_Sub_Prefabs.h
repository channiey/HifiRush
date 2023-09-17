#pragma once
#ifdef _DEBUG

#include "ImGui_Window.h"

BEGIN(Engine)
END

BEGIN(Client)

class CImGui_Window_Sub_Prefabs final : public CImGui_Window
{
protected:
	CImGui_Window_Sub_Prefabs();
	virtual~CImGui_Window_Sub_Prefabs() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Show_Window() override;

public:
	virtual void Clear_Reference_Data() override;


public:
	static CImGui_Window_Sub_Prefabs* Create();
	virtual void Free() override;
};
END

#endif // _DEBUG