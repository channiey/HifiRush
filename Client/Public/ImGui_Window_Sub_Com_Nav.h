#pragma once
#ifdef _DEBUG

#include "ImGui_Window.h"

BEGIN(Engine)

class CCell;

END

BEGIN(Client)

class CImGui_Window_Sub_Com_Nav final : public CImGui_Window
{
protected:
	CImGui_Window_Sub_Com_Nav();
	virtual~CImGui_Window_Sub_Com_Nav() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Show_Window() override;

public:
	virtual void	Clear_Reference_Data() override;

private:
	HRESULT Bake();
	HRESULT Relate_Neighbor();
	HRESULT Save();

private:
	_float m_fMaxAngle		= 180.f;
	_float m_fMinArea		= 3.5f;
	_float m_fRenderRange	= 100.f;
	
	vector<CCell*>	m_Cells;

public:
	static CImGui_Window_Sub_Com_Nav* Create();
	virtual void Free() override;

};
END

#endif // _DEBUG