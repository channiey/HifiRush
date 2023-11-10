#include "..\Default\stdafx.h"
#include "..\Public\Level_Stage_01.h"

#include "EngineInstance.h"
#include "Level_Loading.h"

#include "BattleManager.h"
#include "BeatManager.h"

CLevel_Stage_01::CLevel_Stage_01(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Stage_01::Initialize()
{
	/* Light */
	LIGHT_DESC LightDesc;
	{
		ZeroMemory(&LightDesc, sizeof LightDesc);

		LightDesc.eLightType	= LIGHT_DESC::LIGHT_DIRECTIONAL;
		LightDesc.vLightDir		= _float4(1.f, -1.f, 1.f, 0.f);

		LightDesc.vDiffuse		= _float4(1.f, 1.f, 1.f, 1.f);
		LightDesc.vAmbient		= _float4(1.f, 1.f, 1.f, 1.f);
		LightDesc.vSpecular		= _float4(1.f, 1.f, 1.f, 1.f);
	}
	if (FAILED(ENGINE_INSTANCE->Add_Light(LightDesc)))
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


	return S_OK;
}

HRESULT CLevel_Stage_01::Tick(_float fTimeDelta)
{
	/* Late Initialize ´À³¦ */
	if (!ENGINE_INSTANCE->Is_PlayBGM())
	{
		ENGINE_INSTANCE->Play_BGM();
		CBeatManager::GetInstance()->Reset();
	}

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
	//ENGINE_INSTANCE->Check_Collision_Layer(LayerNames[LAYER_PLAYER]
	//	, LayerNames[LAYER_WEAPON]
	//	, CCollider::SPHERE
	//	, CCollider::SPHERE);

	/*ENGINE_INSTANCE->Check_Collision_Layer(LayerNames[LAYER_ENEMY]
		, LayerNames[LAYER_WEAPON]
		, CCollider::SPHERE
		, CCollider::SPHERE);*/

	ENGINE_INSTANCE->Check_Collision_Layer(LayerNames[LAYER_PLAYER]
		, LayerNames[LAYER_TRIGGER]
		, CCollider::SPHERE
		, CCollider::SPHERE);

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
