#pragma once
#ifdef _DEBUG

#include "ImGui_Window.h"

BEGIN(Engine)

class CCell;

END

BEGIN(Client)

class CImGui_Window_Sub_Com_Col final : public CImGui_Window
{
protected:
	CImGui_Window_Sub_Com_Col();
	virtual~CImGui_Window_Sub_Com_Col() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Show_Window() override;

public:
	virtual void	Clear_Reference_Data() override;

private:
	void Show_Collider();

	void Show_SphereCollider();

private:


public:
	static CImGui_Window_Sub_Com_Col* Create();
	virtual void Free() override;

};
END

#endif // _DEBUG