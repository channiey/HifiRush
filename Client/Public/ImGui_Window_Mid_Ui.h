#pragma once
#ifdef _DEBUG

#include "ImGui_Window.h"


BEGIN(Client)

class CUi;

class CImGui_Window_Mid_Ui final : public CImGui_Window
{
protected:
	CImGui_Window_Mid_Ui();
	virtual~CImGui_Window_Mid_Ui() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Show_Window() override;

public:
	virtual void	Clear_Reference_Data() override;

public:
	HRESULT			Save();
	HRESULT			Load();
	
private:
	void			Show_Textures();
	void			Show_TextureDesc();

private:
	CUi*			m_pUi = nullptr;
	_int			m_iIndex = -1;

public:
	static CImGui_Window_Mid_Ui* Create();
	virtual void Free() override;
};
END

#endif // _DEBUG