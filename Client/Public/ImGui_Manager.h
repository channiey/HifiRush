#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
class CGameObject;
END

BEGIN(Client)

class CImGui_Manager final : public CBase
{
	DECLARE_SINGLETON(CImGui_Manager)

public:
	enum WINDOW_TYPE
	{
		WINDOW_PROFILER,
		WINDOW_OBJECT,
		WINDOW_HIEARACHY,
		WINDOW_INSPECTOR,
		WINDOW_DEMO,

		WINDOW_END
	};
	enum INSPECTOR_TYPE	 /* 컴포넌트 대부분 포함 시켜라 */
	{
		INS_PREFABS,
		INS_PHYSICX,
		INS_NAVI,
		INS_ANIM,
		INS_LIGHT,
		INS_CAM,
		INS_EFFECT,
		INS_WATER,

		INS_END
	};
	const char* STR_WINDOW_TYPE[WINDOW_END] =
	{
		"Profiler",
		"Object",
		"Hierarachy",
		"Inspector",
		"Demo"
	};


private:
	CImGui_Manager();
	virtual ~CImGui_Manager() = default;

public:
	HRESULT			Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	HRESULT			Render();

public:
	const _bool& Is_Active() const { return m_bActive; }

public:
	void			Set_Active(const _bool& bActive) { m_bActive = bActive; }

private: /* For.ImGui Default */
	HRESULT			ImGui_SetUp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void			ImGui_Tick();
	HRESULT			ImGui_Render();
	void			Show_Window_Demo();



private:
	CGameInstance* m_pGameInstance = { nullptr };

	_bool			m_bActive = { TRUE };
	_bool			m_bWindowsActive[WINDOW_END];

public:
	virtual void	Free() override;
};

END


