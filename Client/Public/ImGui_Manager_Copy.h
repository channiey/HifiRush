#ifdef _DEBUG
#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
class CGameObject;
END

BEGIN(Client)

class CImGui_Manager_Copy final : public CBase
{
	DECLARE_SINGLETON(CImGui_Manager_Copy)

public:
	enum WINDOW_TYPE
	{
		WINDOW_PROFILER,
		WINDOW_OBJECT_INFO,
		WINDOW_HIEARACHY,
		WINDOW_DEMO,
		WINDOW_SUB,

		WINDOW_END
	};
	const char* str_WindowType[WINDOW_END] =
	{
		"Profiler",
		"Object Info",
		"Hierarachy",
		"Demo",
		"Sub Window"
	};
	enum SUB_WINDOW_TYPE	
	{
		SUB_WINDOW_PREFABS,


		SUB_WINDOW_LAYER_CHOICE,

		/* Components */
	/*	SUB_WINDOW_NAVI,
		SUB_WINDOW_PHYSICX,
		SUB_WINDOW_MODEL,
		SUB_WINDOW_LIGHT,
		SUB_WINDOW_CAM,
		SUB_WINDOW_EFFECT,
		SUB_WINDOW_WATER,*/


		SUB_WINDOW_END
	};
	const char* str_SubWindowType[SUB_WINDOW_END] =
	{
		"Prefabs",
	
		"Layer Choice"
	};


private:
	CImGui_Manager_Copy();
	virtual ~CImGui_Manager_Copy() = default;

public:
	HRESULT			Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	HRESULT			Render();

public:
	const _bool&	Is_Active() const { return m_bActive; }

public:
	void			Set_Active(const _bool& bActive) { m_bActive = bActive; }

public:
	HRESULT			Clear_ReferenceData();

private: 
	HRESULT			ImGui_SetUp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void			ImGui_Tick();
	HRESULT			ImGui_Render();

private: /* For. Show Window */
	HRESULT			Show_Window_Profiler();
	HRESULT			Show_Window_Object();
	HRESULT			Show_Window_Hierarachy();
	HRESULT			Show_Window_Demo();
	HRESULT			Show_Window_Sub();

private: /* For. Show Window - Objcet */
	HRESULT			Show_Window_Object_Info();
	HRESULT			Show_Window_Object_Transform();
	HRESULT			Show_Window_Object_Components();

private: /* For. Show Window - Hierarachy */
	HRESULT			Show_Window_Hierarachy_Tool();
	HRESULT			Show_Window_Hierarachy_Levels();
	HRESULT			Show_Window_Hierarachy_Layers();
	HRESULT			Show_Window_Hierarachy_Objects();

private: /* For. Show Sub Window*/
	HRESULT			Show_Window_Sub_Prefabs();

private: /* For. Shared Window */
	HRESULT			Show_Window_Choose_Layer();

private:
	HRESULT			Save_Level();



private:
	CGameInstance* m_pGameInstance = { nullptr };

	_bool			m_bActive = { TRUE };
	_bool			m_bWindowsActive[WINDOW_END];
	_bool			m_bWindowSubsActive[SUB_WINDOW_END];

	class CGameObject*  m_pCurObject = { nullptr };
	_bool				m_bCurObjDeleted = FALSE;
	class CGameObject*  m_pPrefabObj = { nullptr };

public:
	virtual void	Free() override;




	/* -------------------------- ¼öÁ¤ --------------------------  */





//private: /* Friend Class */
//	friend class CImGui_Window_Main_Controller;
//	friend class CImGui_Window_Main_Object;
//	friend class CImGui_Window_Main_Hierarachy;
//	friend class CImGui_Window_Main_Demo;
};

END


#endif // _DEBUG