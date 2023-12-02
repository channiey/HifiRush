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

	const static Vec4		g_WorldShadowLightOffset = { -35.f, 50.f, -35.f, 0.f };

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
		SHADER_POS, 
		SHADER_NORMAL, 
		SHADER_STATIC, 
		SHADER_VTF, 
		SHADER_CUBE, 
		SHADER_SKY_SPHERE, 
		SHADER_UI_HUD, 
		SHADER_EFFECT_TEX,
		SHADER_EFFECT_MESH,
		SHADER_END
	};
	static const wchar_t* ShaderNames[SHADER_END]
	{
		L"Prototype_Component_Shader_VtxPosTex",
		L"Prototype_Component_Shader_VtxNorTex",
		L"Prototype_Component_Shader_VtxMesh",
		L"Prototype_Component_Shader_VTF",
		L"Prototype_Component_Shader_VtxCube",
		L"Prototype_Component_Shader_VtxSkySphere",
		L"Prototype_Component_Shader_Ui_Hud",
		L"Prototype_Component_Shader_Shader_Effect_Tex",
		L"Prototype_Component_Shader_Shader_Effect_Mesh",
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

	static const _float CamFov_Follow_Default		= XMConvertToRadians(60.0f);
	static const _float CamFov_Follow_Battle		= XMConvertToRadians(65.0f);
	static const _float CamFov_Follow_Jump			= XMConvertToRadians(65.0f);
	static const _float CamFov_Follow_Attack_Wide	= XMConvertToRadians(75.0f);
	static const _float CamFov_Follow_Attack_Narrow = XMConvertToRadians(55.0f);

	static const _float CamFov_PeppermintGimmick	= XMConvertToRadians(40.0f);
	static const _float CamFov_MacaronGimmick		= XMConvertToRadians(45.0f);

	static const _float CamDist_Follow_Default				= 6.5f;
	static const _float CamDist_Follow_Attack_Wide			= 8.f;
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
		ENEMY_GUNNER,

		/* Etc */

		ETC_NONE_00,
		ETC_PLAYER_CALL,
		ETC_OTHER_CALL,

		TALK_CHAI,
		ETC,
		
		ETC_OBJ,
		ETC_OBJ_SUB_1,
		ETC_OBJ_SUB_2,

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
		EFC_BLADER_ANGRY,
		EFC_BLADER_APPEAR,
		EFC_BLADER_ATK_SIMPLE,
		EFC_BLADER_ATTACK_BLADE_SET_BLADE,
		EFC_BLADER_ATTACK_BLADE_SET_BLADE2,

		EFC_BLADER_ATTACK_BLADE_SPIN,

		EFC_BLADER_ATTACK_HIT,

		EFC_BLADER_ATTACK_START,

		EFC_BLADER_COMBO_SWING_1,
		EFC_BLADER_COMBO_SWING_2,
		EFC_BLADER_COMBO_SWING_3,

		EFC_BLADER_DEAD_EXPLOSION,
		EFC_BLADER_DEAD_TALK,

		EFC_BLADER_LANDING,
		EFC_BLADER_LANDING2,

		EFC_BLADER_MOVE,
		EFC_BLADER_PARRY,
		EFC_BLADER_PARRY_SWING,
		EFC_BLADER_PARRYEVENT_BEAT,
		EFC_BLADER_PARRYEVENT_BEAT_2,


		EFC_BLADER_PARRYEVENT_START,
		EFC_BLADER_PARRYEVNET_SWING1,
		EFC_BLADER_PARRYEVNET_SWING2,
		EFC_BLADER_PARRYEVNET_SWING3,
		EFC_BLADER_PEAK,


		/* EFC CHAI */
		EFC_CHAI_AMAZE_WITH_CRANE,
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

		EFC_CHAI_CATCH_THROWGUITAR,
		EFC_CHAI_DASH,
		EFC_CHAI_FINAL_ATTACK_SWING,
		EFC_CHAI_FOOTSTEP,

		EFC_CHAI_HIBIHI_FINISH_BOOMB,
		EFC_CHAI_HIBIKI,
		EFC_CHAI_HIBIKI_FINISH_GUITAR,
		EFC_CHAI_HIBIKI_INTRO,

		EFC_CHAI_JUMP,
		EFC_CHAI_KILL,
		EFC_CHAI_LAND,
		EFC_CHAI_PARRY,

		EFC_CHAI_POWERCHORD,
		EFC_CHAI_POWERCHORD_GUITAR,

		/* EFC ENEMY */
		EFC_ENEMY_DAMAGED_00,
		EFC_ENEMY_DEAD_EXPLOSION,
		EFC_ENEMY_DEAD_SPARK,

		/* EFC ETC */
		EFC_ETC_FIGHT,

		/* EFC_GUNNER */
		EFC_GUNNER_CHARGE,
		EFC_GUNNER_JUMP,
		EFC_GUNNER_READY,
		EFC_GUNNER_SHOOT,

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
		EFC_MACARON_BREAK_WALL,
		EFC_MACARON_DISAPPEAR,
		EFC_MACARON_FAILED,
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
		EFC_MACARON_SUCCESS,

		/* EFC OBJ */
		EFC_OBJ_BRIDGE_CREATE_BODY,
		EFC_OBJ_BRIDGE_DOWN_BODY,
		EFC_OBJ_BRIDGE_FRAME_DOWN,

		EFC_OBJ_CRANE_ALERT,
		EFC_OBJ_CRANE_MOVE,
		EFC_OBJ_CRANE_MOVE_2,
		EFC_OBJ_CRANE_STOP,

		EFC_OBJ_DOOR_CLOSED,
		EFC_OBJ_DOOR_OPEN,

		EFC_OBJ_SWITCH_SHOT,
		EFC_OBJ_SWITCH_SHOT_CLOSED,

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

		UI_SPECIALATTACK,
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
		L"Ui_Peppermint_Mark",

		L"Ui_Special_Attack"
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
