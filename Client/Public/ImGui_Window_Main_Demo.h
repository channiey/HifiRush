#pragma once
#ifdef _DEBUG

#include "ImGui_Window.h"

BEGIN(Engine)
END

BEGIN(Client)

class CImGui_Window_Main_Demo final : public CImGui_Window
{
protected:
	CImGui_Window_Main_Demo();
	virtual~CImGui_Window_Main_Demo() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Show_Window() override;

public:
	virtual void Clear_Reference_Data() override;

public:

private:


public:
	static CImGui_Window_Main_Demo* Create();
	virtual void Free() override;
};
END

#endif // _DEBUG