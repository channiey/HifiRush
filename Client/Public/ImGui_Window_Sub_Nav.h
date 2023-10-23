#pragma once
#ifdef _DEBUG

#include "ImGui_Window.h"

BEGIN(Engine)

class CCell;
class CShader;
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
	HRESULT			Bake();
	HRESULT			Save();
	HRESULT			Render();

private:
	HRESULT			Create_Cells();
	HRESULT			Set_Neighbors();

private:
	_float			m_fMaxAngle		= 80.f;
	_float			m_fMinArea		= -1.f;
	_float			m_fRenderRange	= 50.f;

	_bool			m_bRender = TRUE;
	vector<CCell*>	m_Cells;
	CShader*		m_pShader = { nullptr };

public:
	static CImGui_Window_Sub_Nav* Create();
	virtual void Free() override;

};
END

#endif // _DEBUG