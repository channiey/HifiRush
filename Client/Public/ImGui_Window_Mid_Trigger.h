#pragma once
#ifdef _DEBUG

#include "ImGui_Window.h"

BEGIN(Client)

class CImGui_Window_Mid_Trigger final : public CImGui_Window
{
protected:
	CImGui_Window_Mid_Trigger();
	virtual~CImGui_Window_Mid_Trigger() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Show_Window() override;

public:
	virtual void	Clear_Reference_Data() override;

public:
	void Show_BattleTrigger();

private:
	_int m_iCurTriggerIndex = -1;
	class CTriggerBattle* m_pCurTrigger = nullptr;

	_int m_iFlowLevel = -1;

public:
	static CImGui_Window_Mid_Trigger* Create();
	virtual void Free() override;
};
END

#endif // _DEBUG