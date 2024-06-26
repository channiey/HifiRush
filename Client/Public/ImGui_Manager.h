#ifdef _DEBUG
#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CEngineInstance;
class CGameObject;
END

BEGIN(Client)

class CImGui_Manager final : public CBase
{
	DECLARE_SINGLETON(CImGui_Manager)

public:
	enum WINDOW_MAIN_TYPE
	{
		WINDOW_MAIN_CONTROLLER,

		WINDOW_MAIN_OBJECT_INFO,
		WINDOW_MAIN_HIEARACHY,

		// 사실상 미드 
		WINDOW_MAIN_NAV,
		WINDOW_MAIN_CAM,
		WINDOW_MAIN_TRIGGER,
		WINDOW_MAIN_UI,
		WINDOW_MAIN_DYNAMIC,

		WINDOW_MAIN_DEMO,

		WINDOW_MAIN_END
	};
	const char* str_MainWindowType[WINDOW_MAIN_END] =
	{
		"Controller",
		"Object Info",
		"Hierarachy",
		
		"Nav",
		"Cam",
		"Trigger",
		"UI",
		"Dynamic",

		"Demo",
	};

	enum WINDOW_SUB_TYPE
	{
		WINDOW_SUB_PREFABS,
		WINDOW_SUB_MINI_LAYERS,/* 이거 왜 따로 클래스 안뺐지? */


		WINDOW_SUB_COM_MODEL,
		WINDOW_SUB_COM_COL,
		WINDOW_SUB_END
	};
	const char* str_SubWindowType[WINDOW_SUB_END] =
	{
		"Prefabs",
		"Layers Choice",

		"Com_Model",
		"Com_Collider_Sphere"
	};


private:
	CImGui_Manager();
	virtual ~CImGui_Manager() = default;

public:
	HRESULT			Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	HRESULT			Render();

public:
	const _bool&	Is_Active() const { return m_bActive; }
	const _bool		Is_ClickedWindow();
	const _bool&	Is_EdittingAnim() const { return m_bEdittingAnim; }
	const _bool&	Is_DebugCam() const { return m_bDebugCam; }

	void			Set_Active(const _bool& bActive) { m_bActive = bActive; }
	void			Set_Active_Main_Window(const WINDOW_MAIN_TYPE& eType, const _bool& bActive);

	void			Toggle_Active() { m_bActive = !m_bActive; }

	HRESULT			Clear_ReferenceData();

	const _bool&	Is_Render_Collider() const { return m_bRenderCollider; }

private:
	HRESULT			ImGui_SetUp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void			ImGui_Tick();
	HRESULT			ImGui_Render();

private:
	/* 현재 정보를 초기화 한다. */
	void			Reset_Index_CurLevel();
	void			Reset_Index_CurLayer();
	void			Reset_Index_CurObject();
	void			Reset_Index_PrefabObject();

public:
	CEngineInstance*				m_pEngineInstance = { nullptr };
	_bool							m_bActive = TRUE;

	vector<class CImGui_Window*>	m_pMainWindows;

	/* 현재 Level 정보 */
	_uint							m_iIndex_CurLevelID = { LV_END };
	
	/* 현재 Layer 정보 */
	_uint							m_iIndex_CurLayerID = { LAYER_END };
	wstring							m_strIndex_CurLayer = {};

	/* 현재 Object 정보 */
	class CGameObject*				m_pCurObject = { nullptr };
	wstring							m_strIndex_CurObject = {};
	_uint							m_iIndex_CurObject = 0;
	
	/* 현재 Prefab Object 정보*/
	class CGameObject*				m_pPrefabObj = { nullptr };
	wstring							m_strIndex_PrefabObject = {};
	_uint							m_iIndex_PrefabObject = 0;

	/* 현재 ImGui 윈도우 클릭 여부 */
	_bool							m_bClickedWindow = FALSE;

	/* 현재 애니메이션 재생 여부 */
	_bool							m_bEdittingAnim = FALSE;


	_bool							m_bRenderCollider = FALSE;
	_bool							m_bDebugCam = FALSE;

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

private: /* Friend Class */

	/* Main */
	friend class CImGui_Window_Main_Controller;
	friend class CImGui_Window_Main_Object;
	friend class CImGui_Window_Main_Hierarachy;

	/* Mid */
	friend class CImGui_Window_Mid_Nav;
	friend class CImGui_Window_Mid_Cam;
	friend class CImGui_Window_Mid_Trigger;
	friend class CImGui_Window_Mid_Ui;
	friend class CImGui_Window_Mid_Dynamic;

	/* Sub*/
	friend class CImGui_Window_Sub_Prefabs;
	friend class CImGui_Window_Sub_Com_Model;

	/* Demo */
	friend class CImGui_Window_Main_Demo;

public:
	virtual void	Free() override;
};

END


#endif // _DEBUG