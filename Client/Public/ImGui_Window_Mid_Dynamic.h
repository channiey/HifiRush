#pragma once
#ifdef _DEBUG

#include "ImGui_Window.h"


BEGIN(Client)

class CDynamic;

class CImGui_Window_Mid_Dynamic final : public CImGui_Window
{
protected:
	CImGui_Window_Mid_Dynamic();
	virtual~CImGui_Window_Mid_Dynamic() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Show_Window() override;

public:
	virtual void	Clear_Reference_Data() override;

private:
	CDynamic*		m_pDynamic = nullptr;

public:
	static CImGui_Window_Mid_Dynamic* Create();
	virtual void Free() override;
};
END

#endif // _DEBUG