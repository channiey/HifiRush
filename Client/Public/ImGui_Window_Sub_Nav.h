#pragma once
#ifdef _DEBUG

#include "ImGui_Window.h"

BEGIN(Engine)

class CCell;
END

BEGIN(Client)

class CImGui_Window_Sub_Nav final : public CImGui_Window
{
protected:
	CImGui_Window_Sub_Nav();
	virtual~CImGui_Window_Sub_Nav() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Show_Window() override;

public:
	virtual void	Clear_Reference_Data() override;

private:
	HRESULT			Clear();
	HRESULT			Bake();
	HRESULT			Save_NavData();

private:
	HRESULT			Create_Cells(vector<CCell*>& Cells);
	HRESULT			Set_Neighbors(vector<CCell*>& Cells);

private:
	void			Render_PopUp_Clear();
	void			Render_PopUp_Save();

private:
	_float			m_fMaxSlope		= 80.f;
	_float			m_fMinArea		= -1.f;


	_bool			m_bPopUp_Clear = FALSE;
	_bool			m_bPopUp_Save = FALSE;


public:
	static CImGui_Window_Sub_Nav* Create();
	virtual void Free() override;

};
END

#endif // _DEBUG