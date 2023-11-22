#include "..\Default\stdafx.h"
#include "..\Public\Level_Stage_01.h"

#include "EngineInstance.h"
#include "Level_Loading.h"

#include "BattleManager.h"
#include "BeatManager.h"
#include "PlayerController.h"

CLevel_Stage_01::CLevel_Stage_01(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Stage_01::Initialize()
{
	/* Light */
	if (FAILED(Ready_Light()))
		return E_FAIL;

	/* Sound */
	if (FAILED(ENGINE_INSTANCE->Register_BGM(BGM_FAST_RUSH, BGM_CUR, BgmVolumeInNotBattle)))
		return E_FAIL;
	
	/* Battle */
	if (FAILED(CBattleManager::GetInstance()->Initialize()))
		return E_FAIL;

	/* NavMesh */
	if (FAILED(CNavMesh::GetInstance()->Load_NavData(NavPaths[LV_STAGE_01])))
		return E_FAIL;

	/* Level Data */
	if (FAILED(CLevel_Loading::Parse_LevelData(LV_STAGE_01)))
		return E_FAIL;

	/* Camera */
	if (FAILED(ENGINE_INSTANCE->Set_CurCamera(CAMERA_ID::CAM_FOLLOW)))
		return E_FAIL;

	/* Player Controller */
	if (FAILED(CPlayerController::GetInstance()->Initialize()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Stage_01::Tick(_float fTimeDelta)
{
	/* Late Initialize ���� */
	if (!ENGINE_INSTANCE->Is_PlayBGM())
	{
		ENGINE_INSTANCE->Play_BGM();
		CBeatManager::GetInstance()->Reset();
	}

	CPlayerController::GetInstance()->Tick(fTimeDelta);

	return S_OK;
}

HRESULT CLevel_Stage_01::LateTick(_float fTimeDelta)
{
	SetWindowText(g_hWnd, LevelNames[LV_STAGE_01]);

	if(FAILED(Check_Collision()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Stage_01::Check_Collision()
{
	ENGINE_INSTANCE->Check_Collision_Layer(LayerNames[LAYER_PLAYER]
		, LayerNames[LAYER_WEAPON]
		, CCollider::SPHERE
		, CCollider::SPHERE);

	ENGINE_INSTANCE->Check_Collision_Layer(LayerNames[LAYER_PLAYER]
		, LayerNames[LAYER_ENEMY]
		, CCollider::SPHERE
		, CCollider::SPHERE);

	ENGINE_INSTANCE->Check_Collision_Layer(LayerNames[LAYER_PLAYER]
		, LayerNames[LAYER_TRIGGER]
		, CCollider::SPHERE
		, CCollider::SPHERE);

	ENGINE_INSTANCE->Check_Collision_Layer(LayerNames[LAYER_ENEMY]
		, LayerNames[LAYER_PROJECTILE]
		, CCollider::SPHERE
		, CCollider::SPHERE);

	ENGINE_INSTANCE->Check_Collision_Layer(LayerNames[LAYER_ENV_INTERACTALBE]
		, LayerNames[LAYER_PROJECTILE]
		, CCollider::SPHERE
		, CCollider::SPHERE);

	return S_OK;
}

HRESULT CLevel_Stage_01::Ready_Light()
{
	/* ���⼺ ���� */
	LIGHT_DESC LightDesc_Dir;
	{
		ZeroMemory(&LightDesc_Dir, sizeof LightDesc_Dir);

		LightDesc_Dir.eLightType = LIGHT_DESC::LIGHT_DIRECTIONAL;
		LightDesc_Dir.vLightDir = _float4(1.f, -1.f, 1.f, 0.f);

		LightDesc_Dir.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
		LightDesc_Dir.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
		LightDesc_Dir.vSpecular = _float4(0.f, 0.f, 0.f, 0.f);  // _float4(1.f, 1.f, 1.f, 1.f);
	}
	if (FAILED(ENGINE_INSTANCE->Add_Light(LightDesc_Dir)))
		return E_FAIL;

	///* �� ���� */
	//LIGHT_DESC LightDesc_Point_00;
	//{
	//	ZeroMemory(&LightDesc_Point_00, sizeof LightDesc_Point_00);

	//	LightDesc_Point_00.eLightType = LIGHT_DESC::LIGHT_POINT;
	//	LightDesc_Point_00.vLightPos = _float4(-15.f, 3.f, 25.f, 1.f);// _float4(-6.f, 3.f, 10.f, 1.f);
	//	LightDesc_Point_00.fLightRange = 20.f;

	//	LightDesc_Point_00.vDiffuse = _float4(0.f, 1.f, 0.f, 1.f);
	//	LightDesc_Point_00.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	//	LightDesc_Point_00.vSpecular = LightDesc_Point_00.vDiffuse;
	//}
	//if (FAILED(ENGINE_INSTANCE->Add_Light(LightDesc_Point_00)))
	//	return E_FAIL;

	//LIGHT_DESC LightDesc_Point_01;
	//{
	//	ZeroMemory(&LightDesc_Point_01, sizeof LightDesc_Point_01);

	//	LightDesc_Point_01.eLightType = LIGHT_DESC::LIGHT_POINT;
	//	LightDesc_Point_01.vLightPos = _float4(-6.f, 3.f, 10.f, 1.f);
	//	LightDesc_Point_01.fLightRange = 20.f;

	//	LightDesc_Point_01.vDiffuse = _float4(1.f, 0.f, 0.f, 1.f);
	//	LightDesc_Point_01.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	//	LightDesc_Point_01.vSpecular = LightDesc_Point_01.vDiffuse;
	//}
	//if (FAILED(ENGINE_INSTANCE->Add_Light(LightDesc_Point_01)))
	//	return E_FAIL;


	return S_OK;
}

CLevel_Stage_01 * CLevel_Stage_01::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_Stage_01*	pInstance = new CLevel_Stage_01(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Stage_01");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Stage_01::Free()
{
	__super::Free();
}
