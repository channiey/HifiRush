#pragma once
#ifdef _DEBUG

#include "ImGui_Window.h"

BEGIN(Engine)
END

BEGIN(Client)

class CImGui_Window_Main_Hierarachy final : public CImGui_Window
{
protected:
	CImGui_Window_Main_Hierarachy();
	virtual~CImGui_Window_Main_Hierarachy() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Show_Window() override;

public:
	virtual void Clear_Reference_Data() override;

private:
	void Show_Hierarachy_FunctionButton();
	void Show_Hierarachy_Levels();
	void Show_Hierarachy_Layers();
	void Show_Hierarachy_Objects();

private:
	/* 레이어 추가 삭제를 위해, 레이어를 선택할 수 있는 창 */
	void Show_MiniLayers();
	void Save_LevelData();

private:

	_bool		m_bShowMiniLayer = FALSE;

	/* 레이어 이벤트가 발생한 경우 예외처리를 위한 변수 */
	_bool		m_bLayerEvent = FALSE;
	_bool		m_bObjectEvent = FALSE;

	/* Temp */
	_uint		m_iTempMiniLayer = 0;

public:
	static CImGui_Window_Main_Hierarachy* Create();
	virtual void Free() override;
};
END

#endif // _DEBUG