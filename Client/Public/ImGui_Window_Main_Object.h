#pragma once
#ifdef _DEBUG

#include "ImGui_Window.h"

BEGIN(Engine)
END

BEGIN(Client)

class CImGui_Window_Main_Object final : public CImGui_Window
{
protected:
	CImGui_Window_Main_Object();
	virtual~CImGui_Window_Main_Object() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Show_Window() override;

public:
	virtual void Clear_Reference_Data() override;

private:
	void Show_Object_Info();
	void Show_Object_Transform();
	void Shwo_Object_Component();

private:
	class CGameObject* pObject = { nullptr };

public:
	static CImGui_Window_Main_Object* Create();
	virtual void Free() override;
};
END

#endif // _DEBUG