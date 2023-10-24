#pragma once

/* 클라이언트에 존재하는 모든 클래스가 인클루드해야할 내용을 담는다. */
#include <process.h>

#include "Engine_Defines.h" /* TODO 상호 참조 오류 가능성 존재*/

using namespace std;
namespace Client
{
#define FPS_LIMIT		66.f
#define IS_MULTI_THREAD true

	/* Window */
	const unsigned int		g_iWinSizeX = 1280;
	const unsigned int		g_iWinSizeY = 720;

	/* Level */
	enum LEVEL_ID
	{
		LV_STATIC,
		LV_LOADING,
		LV_LOGO,
		LV_STAGE_01,
		LV_STAGE_02,
		LV_STAGE_03,
		LV_ENDING,
		LV_PROTO,
		LV_END
	};
	static const wchar_t* LevelNames[LV_END]
	{
		L"STATIC",
		L"LOADING",
		L"LOGO",
		L"STAGE_01",
		L"STAGE_02",
		L"STAGE_03",
		L"ENDING",
		L"PROTO"

	};
	static const std::wstring LevelPaths[LV_END]
	{
		TEXT("../Bin/Resources/Data/Level/Static.level"),
		TEXT("../Bin/Resources/Data/Level/Loading.level"),
		TEXT("../Bin/Resources/Data/Level/Logo.level"),
		TEXT("../Bin/Resources/Data/Level/Stage_01.level"),
		TEXT("../Bin/Resources/Data/Level/Stage_02.level"),
		TEXT("../Bin/Resources/Data/Level/Stage_03.level"),
		TEXT("../Bin/Resources/Data/Level/Ending.level"),
		TEXT("../Bin/Resources/Data/Level/Proto.level"),
	};
	static const std::wstring NavPaths[LV_END]
	{
			TEXT("../Bin/Resources/Data/Nav/Static.nav"),
			TEXT("../Bin/Resources/Data/Nav/Loading.nav"),
			TEXT("../Bin/Resources/Data/Nav/Logo.nav"),
			TEXT("../Bin/Resources/Data/Nav/Stage_01.nav"),
			TEXT("../Bin/Resources/Data/Nav/Stage_02.nav"),
			TEXT("../Bin/Resources/Data/Nav/Stage_03.nav"),
			TEXT("../Bin/Resources/Data/Nav/Ending.nav"),
			TEXT("../Bin/Resources/Data/Nav/Proto.nav"),
	};

	/* Layer */
	enum LAYER_ID 
	{
		LAYER_ENV_SKYBOX,
		LAYER_ENV_STATIC,
		LAYER_ENV_INTERACTALBE,
		LAYER_PLAYER,
		LAYER_ENEMY,
		LAYER_NPC,
		LAYER_WEAPON,
		LAYER_PROJECTILE,	
		LAYER_TRIGGER,
		LAYER_LIGHT,
		LAYER_EFFECT,
		LAYER_CAMERA,
		LAYER_UI,
		LAYER_END
	};
	static const wchar_t* LayerNames[LAYER_END] /* 업데이트 순서에 따른 정렬 */
	{
		L"Layer_00_Env_SkyBox",
		L"Layer_01_Env_Static",
		L"Layer_02_Env_Interactable",
		L"Layer_03_Player",
		L"Layer_04_Enemy",
		L"Layer_05_Npc",
		L"Layer_06_Weapon",
		L"Layer_07_Projectile",
		L"Layer_08_Trigger",
		L"Layer_09_Light",
		L"Layer_10_Effect",
		L"Layer_11_Camera",
		L"Layer_12_Ui",
	};

	enum SHADER_ID
	{
		SHADER_UI, SHADER_NORMAL, SHADER_STATIC, SHADER_VTF, SHADER_CUBE, SHADER_END
	};
	static const wchar_t* ShaderNames[SHADER_END] /* 업데이트 순서에 따른 정렬 */
	{
		L"Prototype_Component_Shader_VtxPosTex",
		L"Prototype_Component_Shader_VtxNorTex",
		L"Prototype_Component_Shader_VtxMesh",
		L"Prototype_Component_Shader_VTF",
		L"Prototype_Component_Shader_VtxCube"
	};


	enum CAMERA_ID { CAM_DEBUG, CAM_FOLLOW, CAM_HILIGHT, CAM_END };
}

extern HINSTANCE	g_hInst;
extern HWND			g_hWnd;
extern _uint		g_iFPS;
extern _bool		g_bLoadPrototype;

using namespace Client;
