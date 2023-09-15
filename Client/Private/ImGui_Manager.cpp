#ifdef _DEBUG
#include "../Default/stdafx.h"

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif

#include "../ImGui/imgui.h" 
#include "../ImGui/imgui_impl_win32.h" 
#include "../ImGui/imgui_impl_dx11.h" 

#include "ImGui_Manager.h"
#include "GameInstance.h"
#include "GameObject.h"

#include "Transform.h"

#include "StringUtils.h"
#include "FileUtils.h"

IMPLEMENT_SINGLETON(CImGui_Manager)

CImGui_Manager::CImGui_Manager()
	: m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

#pragma region Life Cycle

HRESULT CImGui_Manager::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	Safe_AddRef(pDevice);
	Safe_AddRef(pContext);

	if (FAILED(ImGui_SetUp(pDevice, pContext)))
	{
		Safe_Release(pDevice);
		Safe_Release(pContext);
		return E_FAIL;
	}

	Safe_Release(pDevice);
	Safe_Release(pContext);

	for (int i = 0; i < WINDOW_END; ++i)
		m_bWindowsActive[i] = FALSE;

	for (int i = 0; i < SUB_WINDOW_END; ++i)
		m_bWindowSubsActive[i] = FALSE;


	return S_OK;
}

HRESULT CImGui_Manager::Render()
{
	if (!m_bActive) return S_OK;

	/* 이전 프레임 이벤트 체크 및 반영 */
	if (m_bCurObjDeleted)
	{
		m_pCurObject = nullptr;
		m_bCurObjDeleted = FALSE;
	}

	/* 각종 창 렌더링 */
	ImGui_Tick();
	{
		Show_Window_Profiler();

		if(m_bWindowsActive[WINDOW_OBJECT_INFO])
			Show_Window_Object();
		if (m_bWindowsActive[WINDOW_HIEARACHY])
			Show_Window_Hierarachy();
		if (m_bWindowsActive[WINDOW_DEMO])
			Show_Window_Demo();
		
		Show_Window_Sub();
	}

	FAILED_CHECK_RETURN(ImGui_Render(), E_FAIL);

	return S_OK;
}

HRESULT CImGui_Manager::Clear_ReferenceData()
{
	m_pCurObject = nullptr;
	m_pPrefabObj = nullptr;
	return S_OK;
}

HRESULT CImGui_Manager::ImGui_SetUp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable Docking
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // Enable Multi-Viewport / Platform Windows

	ImGui::StyleColorsClassic();

	if (!ImGui_ImplWin32_Init(g_hWnd)) return E_FAIL;

	if (!ImGui_ImplDX11_Init(pDevice, pContext)) return E_FAIL;

	return S_OK;
}

void CImGui_Manager::ImGui_Tick()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();
}

HRESULT CImGui_Manager::ImGui_Render()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return S_OK;
}

#pragma endregion

#pragma region Show Window

HRESULT CImGui_Manager::Show_Window_Profiler()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;

	if(ImGui::Begin(str_WindowType[WINDOW_PROFILER], NULL, window_flags))
	{
		ImGui::SeparatorText("Status");
		{
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			ImGui::Text("FPS : %.1f", io.Framerate);
			//ImGui::Text("FPS : %.1f(%.3f ms/frame)", io.Framerate, 1000.0f / io.Framerate);

			ImGui::Text("Tris : %d", 100);
			ImGui::SameLine();
			ImGui::Text("  Animation : %d", 100);

			ImGui::Text("Draw Call : %d", 100);
			ImGui::SameLine();
			ImGui::Text("  SetPass Call : %d", 100);

		}
		ImGui::SeparatorText("Windows");
		{
			if (ImGui::Button(str_WindowType[WINDOW_OBJECT_INFO]))
				m_bWindowsActive[WINDOW_OBJECT_INFO] = !m_bWindowsActive[WINDOW_OBJECT_INFO];
			ImGui::SameLine();

			if (ImGui::Button(str_WindowType[WINDOW_HIEARACHY]))
				m_bWindowsActive[WINDOW_HIEARACHY] = !m_bWindowsActive[WINDOW_HIEARACHY];
			ImGui::SameLine();

			if (ImGui::Button(str_WindowType[WINDOW_DEMO]))
				m_bWindowsActive[WINDOW_DEMO] = !m_bWindowsActive[WINDOW_DEMO];
			ImGui::SameLine();

			
		}
	}
	ImGui::End();

	return S_OK;
}

HRESULT CImGui_Manager::Show_Window_Object()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;

	if (ImGui::Begin(str_WindowType[WINDOW_OBJECT_INFO], NULL, window_flags))
	{
		ImGui::SeparatorText("Object_Info");
		{
			Show_Window_Object_Info();
		}

		ImGui::SeparatorText("Transform");
		{
			Show_Window_Object_Transform();
		}

		ImGui::SeparatorText("Components");
		{
			Show_Window_Object_Components();
		}
	}
	ImGui::End();

	return S_OK;
}

HRESULT CImGui_Manager::Show_Window_Hierarachy()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;

	if (ImGui::Begin(str_WindowType[WINDOW_HIEARACHY], NULL, window_flags))
	{
		ImGui::SeparatorText("Function");
		{
			Show_Window_Hierarachy_Tool();
		}
		ImGui::SeparatorText("Levels");
		{
			Show_Window_Hierarachy_Levels();
		}
		ImGui::SeparatorText("Layers");
		{
			Show_Window_Hierarachy_Layers();
		}
		ImGui::SeparatorText("Objects");
		{
			Show_Window_Hierarachy_Objects();
		}
	}
	ImGui::End();
	return S_OK;
}

HRESULT CImGui_Manager::Show_Window_Demo()
{
	ImGui::ShowDemoWindow(&m_bWindowsActive[WINDOW_DEMO]);

	return S_OK;
}

HRESULT CImGui_Manager::Show_Window_Sub()
{
	if (m_bWindowSubsActive[SUB_WINDOW_PREFABS])
		Show_Window_Sub_Prefabs();

	return S_OK;
}

#pragma endregion

#pragma region Show Window Object

_bool		 bActive = TRUE;

HRESULT CImGui_Manager::Show_Window_Object_Info()
{
	/* Name */
	ImGui::Text("Name : ");
	ImGui::SameLine();

	if (nullptr != m_pCurObject)
	{
		const char* strName = StringUtils::WC2C(m_pCurObject->Get_Name().c_str());
		ImGui::Text(strName);
		delete strName;
	}
	else
	{
		ImGui::Text("...");
	}


	/* Active */
	if (nullptr != m_pCurObject)
		bActive = m_pCurObject->Is_Active();

	if (ImGui::Checkbox("Active", &bActive))
	{
		if (nullptr != m_pCurObject)
			m_pCurObject->Set_Active(bActive);
	}

	return S_OK;
}

HRESULT CImGui_Manager::Show_Window_Object_Transform()
{
	NULL_CHECK_RETURN(m_pCurObject, E_FAIL);
	
	CTransform* pTransform = m_pCurObject->Get_Transform();

	if(nullptr != pTransform)
	{
		Safe_AddRef(pTransform);
		{
			/* TODO 실제 적용 필요 */
			Vec4 vVec;

			/* Position */
			vVec = pTransform->Get_State(CTransform::STATE_POSITION);
			_float fPos[3] = { vVec.x, vVec.y, vVec.z };
			if (ImGui::InputFloat3("Pos", fPos, "%.2f"))
			{
				pTransform->Set_State(CTransform::STATE_POSITION, Vec4(fPos[0], fPos[1], fPos[2], 1));
			}

			/* TODO 로테이션값 어떻게 가져오냐? */

			/* Rotation */
			vVec = pTransform->Get_State(CTransform::STATE_POSITION);
			_float fRot[3] = { vVec.x, vVec.y, vVec.z };
			ImGui::InputFloat3("Rot", fRot, "%.2f");

			/* Scale*/
			vVec = pTransform->Get_Scale();
			_float fSacle[3] = { vVec.x, vVec.y, vVec.z };
			if (ImGui::InputFloat3("Scale", fSacle, "%.2f"))
			{
				pTransform->Set_Scale(Vec3(fSacle[0], fSacle[1], fSacle[2]));
			}

			/* Move Speed */
			_float fSpeedPerSec = pTransform->Get_Speed();
			if (ImGui::InputFloat("Speed/s", &fSpeedPerSec, 0.01f, 1.0f, "%.2f"))
				pTransform->Set_Speed(fSpeedPerSec);


			/* Rot Speed */
			_float fRotSpeed = pTransform->Get_RotRad();
			if (ImGui::InputFloat("Rot/s", &fSpeedPerSec, 0.01f, 1.0f, "%.2f"))
				pTransform->Set_Speed(fRotSpeed);


		}
		Safe_Release(pTransform);
	}
	else
	{
		float fZero[3] = { 0.f, 0.f, 0.f };
		ImGui::InputFloat3("Pos", fZero, "%.2f");
		ImGui::InputFloat3("Rot", fZero, "%.2f");
		ImGui::InputFloat3("Scale", fZero, "%.2f");
	}
	

	return S_OK;
}

HRESULT CImGui_Manager::Show_Window_Object_Components()
{
	NULL_CHECK_RETURN(m_pCurObject, E_FAIL);

	map<const wstring, class CComponent*>& Components = m_pCurObject->Get_Components();

	_uint i = 0;
	for (auto& Pair : Components)
	{
		if (i > 0) ImGui::SameLine();

		++i;
		
		const char* strComponent = StringUtils::WC2C(Pair.first.c_str());
		if (strcmp(strComponent, StringUtils::ToString(gStrComponentType[COM_STATEMACHINE]).c_str()) ||
			strcmp(strComponent, StringUtils::ToString(gStrComponentType[COM_BEHAVIOURTREE]).c_str()))
		{
			delete strComponent;
			continue;
		}
		
		if (ImGui::Button(strComponent))
		{

		}
		delete strComponent;
	}

	return S_OK;
}

#pragma endregion

#pragma region Show Window Hierarachy

static _uint	iHierarachyIndexLevel = 0;
static _uint	iHierarachyIndexLayer = 0;
static _uint	iHierarachyIndexObject = 0;

wstring			strHierarachyIndexLayer = {};
wstring			strHierarachyIndexObject = {};

HRESULT CImGui_Manager::Show_Window_Hierarachy_Tool()
{
	if (ImGui::Button("Prefabs"))
	{
		m_bWindowSubsActive[SUB_WINDOW_PREFABS] = !m_bWindowSubsActive[SUB_WINDOW_PREFABS];
	}
	ImGui::SameLine();

	if (ImGui::Button("Save"))
	{


	}
	ImGui::SameLine();

	if (ImGui::Button("Edit"))
	{

	}
	return S_OK;
}

HRESULT CImGui_Manager::Show_Window_Hierarachy_Levels()
{
	iHierarachyIndexLevel = m_pGameInstance->Get_CurLevelIndex();
	{
		for (int i = LEVEL_GAMEPLAY; i < LEVEL_ENDING; ++i)
		{
			if (i > LEVEL_GAMEPLAY)
				ImGui::SameLine();

			const char* strLevel = StringUtils::WC2C(gStrLevelID[i]);
			if (ImGui::Button(strLevel))
			{
				iHierarachyIndexLevel = i;
			}
			delete strLevel;
		}
	}

	return S_OK;
}

HRESULT CImGui_Manager::Show_Window_Hierarachy_Layers()
{
	/* 선택한 레벨의 레이어들을 가져온다.*/
	map<const wstring, class CLayer*>* pLayers = m_pGameInstance->Get_Layers(iHierarachyIndexLevel);

	if (ImGui::BeginListBox("##listbox 523", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
	{
		if (nullptr == pLayers)
		{
			ImGui::EndListBox();
			return E_FAIL;
		}

		int i = 0;
		for (auto& Pair : *pLayers)
		{
			const bool is_selected = (iHierarachyIndexLayer == i);

			const char* strLayer = StringUtils::WC2C(Pair.first.c_str());
			if (ImGui::Selectable(strLayer, is_selected))
			{
				strHierarachyIndexLayer = Pair.first.c_str();
				iHierarachyIndexLayer = i;
			}
			delete strLayer;

			if (is_selected)
				ImGui::SetItemDefaultFocus();

			++i;
		}

		ImGui::EndListBox();
	}
	return S_OK;
}

HRESULT CImGui_Manager::Show_Window_Hierarachy_Objects()
{
	/* 오브젝트 삭제 */
	if (ImGui::Button("Delete"))
	{
		if (nullptr != m_pCurObject)
		{
			FAILED_CHECK_RETURN(m_pGameInstance->Delete_GameObject(m_pGameInstance->Get_CurLevelIndex(), m_pCurObject), E_FAIL);
			m_bCurObjDeleted = TRUE;
		}
	}
	ImGui::SameLine();
	
	/* 오브젝트 검색 */
	ImGui::Button("Find");
	ImGui::SameLine();
	static char str0[128] = "Enter the Name...";
	ImGui::InputText(" ", str0, IM_ARRAYSIZE(str0));


	/* 선택한 레벨, 레이어의 오브젝트들을 가져온다.*/
	list<class CGameObject*>* pGameObjects = m_pGameInstance->Get_Objects(LEVEL_LOGO, strHierarachyIndexLayer);

	if (ImGui::BeginListBox("##listbox 1", ImVec2(-FLT_MIN, 8 * ImGui::GetTextLineHeightWithSpacing())))
	{
		if (nullptr == pGameObjects)
		{
			ImGui::EndListBox();
			return E_FAIL;
		}

		if (nullptr != pGameObjects && !(*pGameObjects).empty())
		{
			int i = 0;
			for (auto& obj : *pGameObjects)
			{
				if (nullptr == obj) continue;

				const bool is_selected = (iHierarachyIndexObject == i);
				const char* strObject = StringUtils::WC2C(obj->Get_Name().c_str());
				if (ImGui::Selectable(strObject, is_selected))
				{
					if (m_pCurObject == obj)
					{
						delete strObject;
						break;
					}

					m_pCurObject = obj;

					if (nullptr != m_pCurObject)
					{
						strHierarachyIndexObject = m_pCurObject->Get_Name();
						iHierarachyIndexObject = i;
					}
				}
				delete strObject;

				if (is_selected)
					ImGui::SetItemDefaultFocus();

				++i;
			}
		}
		ImGui::EndListBox();
	}

	return S_OK;
}

#pragma endregion

#pragma region Show Sub Window

static int	iInspectorPrefabsIndexObject = 0;
wstring		strInspectorPrefabsIndexObject = {};

HRESULT CImGui_Manager::Show_Window_Sub_Prefabs()
{
	if (ImGui::Begin(str_SubWindowType[SUB_WINDOW_PREFABS]))
	{
		/* Find Func*/
		ImGui::Button("Find");
		ImGui::SameLine();
		static char str0[128] = "Enter the Name...";
		ImGui::InputText(" ", str0, IM_ARRAYSIZE(str0));


		/* 프로토 타입을 가져온다. */
		map<const wstring, class CGameObject*>* pPrototypes = m_pGameInstance->Get_Prototypes();

		if (ImGui::BeginListBox("##listbox 734", ImVec2(-FLT_MIN, 15 * ImGui::GetTextLineHeightWithSpacing())))
		{
			/* 예외 처리 */
			if (nullptr == pPrototypes)
			{
				ImGui::EndListBox();
				return E_FAIL;
			}

			int i = 0;
			for (auto& Pair : *pPrototypes)
			{
				const bool is_selected = (iInspectorPrefabsIndexObject == i);

				const char* strPrefabObj = StringUtils::WC2C(Pair.first.c_str());
				if (ImGui::Selectable(strPrefabObj, is_selected))
				{
					strInspectorPrefabsIndexObject = Pair.first.c_str();
					iInspectorPrefabsIndexObject = i;
				}
				delete strPrefabObj;

				if (is_selected)
					ImGui::SetItemDefaultFocus();

				++i;
			}

			ImGui::EndListBox();
		}


	}
	ImGui::End();

	return S_OK;
}

#pragma endregion

#pragma region File Save Load

HRESULT CImGui_Manager::Save_Level()
{

	CFileUtils file;

	file.Open(gStrLevelPath[iHierarachyIndexLevel], FileMode::Write);

	// for 레이어 개수
	// for 오브젝트 개수


	// 레이어
	// 이름
	// 트랜스폼
	// 액티브

	return S_OK;
}

#pragma endregion


void CImGui_Manager::Free()
{
	Safe_Release(m_pGameInstance);

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

#endif // _DEBUG