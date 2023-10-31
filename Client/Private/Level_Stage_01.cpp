#include "..\Default\stdafx.h"
#include "..\Public\Level_Stage_01.h"

#include "EngineInstance.h"
#include "Level_Loading.h"

#include "BeatManager.h"

CLevel_Stage_01::CLevel_Stage_01(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Stage_01::Initialize()
{
	/* Sound */
	{
		ENGINE_INSTANCE->Play_BGM(BGM_FAST_RUSH, BGM_CUR, BgmVolumeInNotBattle);
		CBeatManager::GetInstance()->Reset();
	}

	if (FAILED(CNavMesh::GetInstance()->Load_NavData(NavPaths[LV_STAGE_01])))
		return E_FAIL;

	if (FAILED(CLevel_Loading::Parse_LevelData(LV_STAGE_01)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Stage_01::Tick(_float fTimeDelta)
{
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
	ENGINE_INSTANCE->Check_Collision_Layer(LayerNames[LAYER_ENEMY]
		, LayerNames[LAYER_WEAPON]
		, CCollider::SPHERE
		, CCollider::SPHERE);


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
