#pragma once

#include <process.h>

#include "Engine_Defines.h" /* TODO 상호 참조 오류 가능성 존재*/

using namespace std;
namespace Client
{

#define FPS_LIMIT		66.f
	    
#define CONSOLE_LOG     0
#define CURSOL_LOCK     0
#define CURSOL_HIDE     0
#define OPEN_IMGUI      0

#define MAX_DELTA		0.02f

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
		L"Layer_00_Env_Sky",
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

	/* Shader */
	enum SHADER_ID
	{
		SHADER_POS, SHADER_NORMAL, SHADER_STATIC, SHADER_VTF, SHADER_CUBE, SHADER_SKY_SPHERE, SHADER_UI_HUD, SHADER_END
	};
	static const wchar_t* ShaderNames[SHADER_END] /* 업데이트 순서에 따른 정렬 */
	{
		L"Prototype_Component_Shader_VtxPosTex",
		L"Prototype_Component_Shader_VtxNorTex",
		L"Prototype_Component_Shader_VtxMesh",
		L"Prototype_Component_Shader_VTF",
		L"Prototype_Component_Shader_VtxCube",
		L"Prototype_Component_Shader_VtxSkySphere",
		L"Prototype_Component_Shader_Ui_Hud"

	};

	/* Camera */
	enum CAMERA_ID 
	{ 
		CAM_DEBUG, 
		CAM_FOLLOW,

		CAM_PARRY, 
		
		CAM_PEPPERMINT_GIMMICK_AIM, 
		CAM_PEPPERMINT_GIMMICK_CRANE, 

		CAM_MACARON_GIMMICK_WALL,
		
		CAM_END 
	};

	static const wchar_t* CameraNames[CAM_END]
	{
		L"Camera_Debug",
		L"Camera_Follow",
		L"Camera_Parry",
		L"Camera_Peppermint_Gimmick_Aim",
		L"Camera_Peppermint_Gimmick_Crane",
		L"Camera_Macaron_Gimmick_Wall"
	};

	static const _float CamFov_Follow_Default	= XMConvertToRadians(60.0f);
	static const _float CamFov_Follow_Battle	= XMConvertToRadians(65.0f);
	static const _float CamFov_Follow_Jump		= XMConvertToRadians(65.0f);
	static const _float CamFov_Follow_PeppermintGimmick = XMConvertToRadians(40.0f);
	static const _float CamFov_Follow_MacaronGimmick = XMConvertToRadians(45.0f);

	static const _float CamDist_Follow_Default				= 6.5f;
	static const _float CamDist_Follow_Battle				= 7.f;
	static const _float CamDist_Follow_Jump					= 7.f;
	static const _float CamDist_Follow_Final				= 12.f;
	static const _float CamDist_Follow_PeppermintGimmick	= 5.f;
	static const _float CamDist_Follow_MacaronGimmick		= 6.f;

	static const _float CamDist_Parry_Default	= 20.f;


	/* Sound Channel */
	enum CHANNEL_ID 
	{
		/* BGM */
		BGM_PRE,
		BGM_CUR,

		/* Player */
		PLAYER_CHAI,
		PLAYER_PEPPERMINT,
		PLAYER_MACARON,
		PLAYER_KORSICA,

		/* Enemy */
		ENEMY_SABER,
		ENEMY_BLADER,

		/* Etc */

		ETC_NONE_00,
		ETC_PLAYER_CALL,
		ETC,

		CHANNEL_ID_END = MAX_CHANNEL
	};


	/* Sound File */
	enum SOUND_FILE_ID 
	{
		/* BGM FAST */
		BGM_FAST_CANTSTOP, 
		BGM_FAST_DEV, 
		BGM_FAST_HEATWAVE,
		BGM_FAST_MISSION,
		BGM_FAST_PREBATTLE,
		BGM_FAST_PULSE, 
		BGM_FAST_RUSH, 
		BGM_FAST_SECURITY, 

	
		/* BGM MIDDLE */
		BGM_MIDDLE_CHECK,
		BGM_MIDDLE_DOWNTIME,
		BGM_MIDDLE_UNRESTRICTED,

		/* EFC BLADER */
		EFC_BLADER_PARRYEVENT_BEAT,
		EFC_BLADER_PARRYEVENT_BEAT_2,

		/* EFC CHAI */
		EFC_CHAI_ATTACK_00,
		EFC_CHAI_ATTACK_COMBO_00,
		EFC_CHAI_ATTACK_COMBO_01,
		EFC_CHAI_ATTACK_COMBO_02,
		EFC_CHAI_ATTACK_COMBO_03,
		EFC_CHAI_BESTTIMMING,

		EFC_CHAI_CALL_KORSICA_00,
		EFC_CHAI_CALL_KORSICA_01,
		EFC_CHAI_CALL_KORSICA_02,
		EFC_CHAI_CALL_KORSICA_03,

		EFC_CHAI_CALL_MACARON_00,
		EFC_CHAI_CALL_MACARON_01,
		EFC_CHAI_CALL_MACARON_02,
		EFC_CHAI_CALL_MACARON_03,

		EFC_CHAI_CALL_PEPPER_00,
		EFC_CHAI_CALL_PEPPER_01,
		EFC_CHAI_CALL_PEPPER_02,
		EFC_CHAI_CALL_PEPPER_03,

		EFC_CHAI_DASH,
		EFC_CHAI_JUMP,
		EFC_CHAI_KILL,
		EFC_CHAI_LAND,
		EFC_CHAI_PARRY,

		/* EFC ENEMY */
		EFC_ENEMY_DAMAGED_00,

		/* EFC ETC */
		EFC_ETC_FIGHT,

		/* EFC_KORSICA */
		EFC_KORSICA_RESPAWN_00,
		EFC_KORSICA_RESPAWN_01,
		EFC_KORSICA_RESPAWN_02,
		EFC_KORSICA_RESPAWN_03,
		EFC_KORSICA_RESPAWN_04,
		EFC_KORSICA_RESPAWN_05,
		EFC_KORSICA_RESPAWN_06,
		EFC_KORSICA_RESPAWN_07,


		/* EFC MACARON */
		EFC_MACARON_DISAPPEAR,
		EFC_MACARON_JUMP,
		EFC_MACARON_LAND,

		EFC_MACARON_RESPAWN_00,
		EFC_MACARON_RESPAWN_01,
		EFC_MACARON_RESPAWN_02,
		EFC_MACARON_RESPAWN_03,
		EFC_MACARON_RESPAWN_04,
		EFC_MACARON_RESPAWN_05,
		EFC_MACARON_RESPAWN_06,

		EFC_MACARON_SHOT,

		/* EFC PEPPERMINT */
		EFC_PEPPERMINT_FAILED,
		EFC_PEPPERMINT_RESPAWN_00,
		EFC_PEPPERMINT_RESPAWN_01,
		EFC_PEPPERMINT_RESPAWN_02,
		EFC_PEPPERMINT_RESPAWN_03,
		EFC_PEPPERMINT_RESPAWN_04,
		EFC_PEPPERMINT_RESPAWN_05,
		EFC_PEPPERMINT_RESPAWN_06,
		EFC_PEPPERMINT_RESPAWN_07,
		EFC_PEPPERMINT_SHOOT,

		/* EFC ENCMY SABER */
		EFC_SABER_CHARGE,
		EFC_SABER_ESCAPING,
		EFC_SABER_LAND,
		EFC_SABER_SWING,


		SOUND_FILE_END
	};
	static const char* SoundFilePath_Engine = "../../Client/Bin/Resources/Sounds/";


	static const _float BgmVolumeInBattle		= 0.65f;
	static const _float BgmVolumeInNotBattle	= 0.5f;
	static const _float EfcVolumeChai			= 0.6f;
	static const _float EfcVolumeSaber			= 0.3f;

	/* UI */
	enum UI_ID
	{
		UI_HUD,
		UI_PARRY_RHYTHM,
		UI_FIGHT,
		UI_DIALOUGE,
		UI_HUD_BOSS,

		UI_PEPPERMINT_AIM,
		UI_PEPPERMINT_TARGET,
		UI_PEPPERMINT_MARK,

		UI_END
	};
	static const wchar_t* UINames[UI_ID::UI_END]
	{
		L"Ui_Hud",
		L"Ui_Parry_Rhythm",
		L"Ui_Fight",
		L"Ui_Dialouge",
		L"Ui_Hud_Boss",

		L"Ui_Peppermint_Aim",
		L"Ui_Peppermint_Target",
		L"Ui_Peppermint_Mark"
	};


	/* Etc Path */
	static const wstring TriggerFilePath = L"../Bin/Resources/Data/Trigger/";
	static const wstring UiFilePath = L"../Bin/Resources/Data/Ui/";
}

extern HINSTANCE	g_hInst;
extern HWND			g_hWnd;
extern _uint		g_iFPS;
extern _bool		g_bLoadPrototype;

using namespace Client;
