#pragma once

/* 클라이언트에 존재하는 모든 클래스가 인클루드해야할 내용을 담는다. */
#include <process.h>

namespace Client
{
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
	static const char* gStrLevelID[LEVEL_END]
	{
		"LEVEL_STATIC",
		"LEVEL_LOADING",
		"LEVEL_LOGO",
		"LEVEL_GAMEPLAY",
		"LEVEL_ENDING"
	};

	/* Layer */
	enum LAYER_ID
	{
		LAYER_CAMERA,
		LAYER_BACKGROUND,	// 스카이박스, 로고 등 
		LAYER_ENVIORNMENT,	// 고정, 애미네이션, 상호작용
		LAYER_PLAYER,
		LAYER_MONSTER,
		LAYER_NPC,
		LAYER_PROJECTILE,
		LAYER_EFFECT,
		LAYER_UI,

		LAYER_END
	};
	static const char* gStrLayerID[LAYER_END]
	{
		"Layer_Camera",
		"Layer_BackGround",
		"Layer_Enviornment",
		"Layer_Player",
		"Layer_Monster",
		"Layer_Npc",
		"Layer_Projectile",
		"Layer_Effect",
		"Layer_Ui",
	};

	/* Window */
	const unsigned int		g_iWinSizeX = 1280;
	const unsigned int		g_iWinSizeY = 720;
}

extern HWND g_hWnd;

using namespace Client;
