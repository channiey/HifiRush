#pragma once
#ifdef _DEBUG

#include "ImGui_Window.h"

BEGIN(Engine)

class CCell;

END

BEGIN(Client)

class CImGui_Window_Sub_Com_Model final : public CImGui_Window
{
protected:
	CImGui_Window_Sub_Com_Model();
	virtual~CImGui_Window_Sub_Com_Model() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Show_Window() override;

public:
	virtual void	Clear_Reference_Data() override;

private:
	void Show_Animations();

	_int m_iCurAnimIndex = -1;

private:


public:
	static CImGui_Window_Sub_Com_Model* Create();
	virtual void Free() override;

};
END

#endif // _DEBUG