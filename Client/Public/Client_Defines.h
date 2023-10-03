#pragma once

/* 클라이언트에 존재하는 모든 클래스가 인클루드해야할 내용을 담는다. */
#include <process.h>

#include "Engine_Defines.h" /* TODO 상호 참조 오류 가능성 존재*/

using namespace std;
namespace Client
{
#define FPS_LIMIT		65.f
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
	static const wchar_t* gStrLevelID[LV_END]
	{
		L"LV_STATIC",
		L"LV_LOADING",
		L"LV_LOGO",
		L"LV_STAGE_01",
		L"LV_STAGE_02",
		L"LV_STAGE_03",
		L"LV_ENDING",
		L"LV_PROTO"

	};
	static const std::wstring gStrLevelPath[LV_END]
	{
		TEXT("../Bin/Resources/Data/Level/Lv_Static.level"),
		TEXT("../Bin/Resources/Data/Level/Lv_Loading.level"),
		TEXT("../Bin/Resources/Data/Level/Lv_Logo.level"),
		TEXT("../Bin/Resources/Data/Level/Lv_Stage_01.level"),
		TEXT("../Bin/Resources/Data/Level/Lv_Stage_02.level"),
		TEXT("../Bin/Resources/Data/Level/Lv_Stage_03.level"),
		TEXT("../Bin/Resources/Data/Level/Lv_Ending.level"),
		TEXT("../Bin/Resources/Data/Level/Lv_Proto.level"),
	};

	/* Layer */
	enum LAYER_ID
	{
		LAYER_CAMERA,
		LAYER_ENV_STATIC,
		LAYER_ENV_INTERACTALBE,
		LAYER_PLAYER,
		LAYER_MONSTER,
		LAYER_NPC,
		LAYER_PROJECTILE,	
		LAYER_TRIGGER,
		LAYER_LIGHT,
		LAYER_EFFECT,
		LAYER_UI,
		LAYER_END
	};
	static const wchar_t* gStrLayerID[LAYER_END]
	{
		L"Layer_Camera",
		L"Layer_Env_Static",
		L"Layer_Env_Interactable",
		L"Layer_Player",
		L"Layer_Monster",
		L"Layer_Npc",
		L"Layer_Projectile",
		L"Layer_Trigger",
		L"Layer_Light",
		L"Layer_Effect",
		L"Layer_Ui",
	};

	typedef struct tagTransformDesc
	{
		_float		fSpeedPerSec = { 0.f }; /* 초당 이동속도 */
		_float		fRotRadPerSec = { 0.f }; /* 초당 회전량 */

	}TRANSFORM_DESC;
}

extern HINSTANCE	g_hInst;
extern HWND			g_hWnd;
extern _uint		g_iFPS;

using namespace Client;
