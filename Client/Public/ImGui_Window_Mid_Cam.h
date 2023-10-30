#pragma once
#ifdef _DEBUG

#include "ImGui_Window.h"

BEGIN(Engine)

class CCell;
END

BEGIN(Client)

class CImGui_Window_Mid_Cam final : public CImGui_Window
{
protected:
	CImGui_Window_Mid_Cam();
	virtual~CImGui_Window_Mid_Cam() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Show_Window() override;

public:
	virtual void	Clear_Reference_Data() override;

private:
	HRESULT	Show_Property_DebugCam();
	HRESULT	Show_Property_FollowCam();



public:
	static CImGui_Window_Mid_Cam* Create();
	virtual void Free() override;
};
END

#endif // _DEBUG