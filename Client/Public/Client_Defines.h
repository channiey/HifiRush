#pragma once

/* Ŭ���̾�Ʈ�� �����ϴ� ��� Ŭ������ ��Ŭ����ؾ��� ������ ��´�. */
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
	static const wchar_t* gStrLevelID[LEVEL_END]
	{
		L"LEVEL_STATIC",
		L"LEVEL_LOADING",
		L"LEVEL_LOGO",
		L"LEVEL_GAMEPLAY",
		L"LEVEL_ENDING"
	};

	/* Layer */
	enum LAYER_ID
	{
		LAYER_CAMERA,
		LAYER_BACKGROUND,	// ��ī�̹ڽ�, �ΰ� �� 
		LAYER_ENVIORNMENT,	// ����, �ֹ̳��̼�, ��ȣ�ۿ�
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

	/* Window */
	const unsigned int		g_iWinSizeX = 1280;
	const unsigned int		g_iWinSizeY = 720;
}

extern HWND g_hWnd;

using namespace Client;
