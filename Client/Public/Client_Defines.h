#pragma once

/* 클라이언트에 존재하는 모든 클래스가 인클루드해야할 내용을 담는다. */
#include <process.h>

#include "Engine_Defines.h" /* TODO 상호 참조 오류 가능성 존재*/

using namespace std;
namespace Client
{
#define FPS_LIMIT		1000.f
#define IS_MULTI_THREAD 

	/* Window */
	const unsigned int		g_iWinSizeX = 1280;
	const unsigned int		g_iWinSizeY = 720;

	/* Level */
	enum LEVEL_ID
	{
		LEVEL_STATIC,
		LEVEL_LOADING,
		LEVEL_LOGO,
		LEVEL_GAMEPLAY,
		LEVEL_ENDING,
		LEVEL_END
	};
	static const wchar_t* gStrLevelID[LEVEL_END]
	{
		L"LEVEL_STATIC",
		L"LEVEL_LOADING",
		L"LEVEL_LOGO",
		L"LEVEL_GAMEPLAY",
		L"LEVEL_ENDING"
	};
	static const std::wstring gStrLevelPath[LEVEL_END]
	{
		TEXT("../Bin/Resources/Data/Level/Level_Static.level"),
		TEXT("../Bin/Resources/Data/Level/Level_Loading.level"),
		TEXT("../Bin/Resources/Data/Level/Level_Logo.level"),
		TEXT("../Bin/Resources/Data/Level/Level_Gameplay.level"),
		TEXT("../Bin/Resources/Data/Level/Level_Ending.level"),
	};

	/* Layer */
	enum LAYER_ID
	{
		LAYER_CAMERA,
		LAYER_BACKGROUND,	
		LAYER_ENVIORNMENT,	
		LAYER_PLAYER,
		LAYER_MONSTER,
		LAYER_NPC,
		LAYER_PROJECTILE,
		LAYER_EFFECT,
		LAYER_UI,
		LAYER_END
	};
	static const wchar_t* gStrLayerID[LAYER_END]
	{
		L"Layer_Camera",
		L"Layer_BackGround",
		L"Layer_Enviornment",
		L"Layer_Player",
		L"Layer_Monster",
		L"Layer_Npc",
		L"Layer_Projectile",
		L"Layer_Effect",
		L"Layer_Ui",
	};

	typedef struct tagTransformDesc
	{
		_float		fSpeedPerSec = { 0.f }; /* 초당 이동속도 */
		_float		fRotRadPerSec = { 0.f }; /* 초당 회전량 */

	}TRANSFORM_DESC;
}

extern HINSTANCE g_hInst;
extern HWND g_hWnd;

using namespace Client;
