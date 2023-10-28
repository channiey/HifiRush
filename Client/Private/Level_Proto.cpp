#include "..\Default\stdafx.h"
#include "..\Public\Level_Proto.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "Level_Loading.h"
#include "ImGui_Manager.h"
#include "GameObject.h"
#include "Model.h"
#include "Chai.h"
#include "Saber.h"
CLevel_Proto::CLevel_Proto(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Proto::Initialize()
{
	/* 네비 메시를 먼저 로드 한 뒤 오브젝트를 로드해야한다 (인덱스 세팅) */
	{
		if (FAILED(CNavMesh::GetInstance()->Load_NavData(NavPaths[LV_PROTO])))
			return E_FAIL;

		if (FAILED(CLevel_Loading::Parse_LevelData(LV_PROTO)))
			return E_FAIL;
	}


	/* 테스트용 개인 로드 */
	{
		CGameObject* pObject = nullptr;

		/*if (nullptr == GAME_INSTNACE->Add_GameObject(LV_PROTO, LayerNames[LAYER_CAMERA], L"Camera_Debug"))
			return E_FAIL;

		if (nullptr == GAME_INSTNACE->Add_GameObject(LV_PROTO, LayerNames[LAYER_CAMERA], L"Camera_Follow"))
			return E_FAIL;

		if (nullptr == GAME_INSTNACE->Add_GameObject(LV_PROTO, LayerNames[LAYER_PLAYER], L"Player_Chai"))
			return E_FAIL;*/

		//if (nullptr == GAME_INSTNACE->Add_GameObject(LV_PROTO, LayerNames[LAYER_ENEMY], L"Enemy_Saber"))
		//	return E_FAIL;

		//if (nullptr == GAME_INSTNACE->Add_GameObject(LV_PROTO, LayerNames[LAYER_ENV_STATIC], L"Env_Static_Bldg_Inside_Battle_A"))
		//	return E_FAIL;

		//pObject = GAME_INSTNACE->Add_GameObject(LV_PROTO, LayerNames[LAYER_ENV_STATIC], L"Env_Static_Bldg_Outside_BK");
		//{
		//	if (nullptr == pObject)
		//		return E_FAIL;
		//	pObject->Get_Transform()->Set_Position(Vec3(20, 0, 0));
		//}

		/*if (nullptr == GAME_INSTNACE->Add_GameObject(LV_PROTO, LayerNames[LAYER_ENV_STATIC], L"Env_Static_Bldg_Inside_Lobby_A"))
			return E_FAIL;



		if (nullptr == GAME_INSTNACE->Add_GameObject(LV_PROTO, LayerNames[LAYER_ENV_SKYBOX], L"Env_SkyBox"))
			return E_FAIL;*/


	}

 	return S_OK;
}

HRESULT CLevel_Proto::Tick(_float fTimeDelta)
{
	return S_OK;
}

HRESULT CLevel_Proto::LateTick(_float fTimeDelta)
{
	SetWindowText(g_hWnd, LevelNames[LV_PROTO]);

	GAME_INSTNACE->Check_Collision_Layer(LayerNames[LAYER_ENEMY]
										, LayerNames[LAYER_WEAPON]
										, CCollider::SPHERE
										, CCollider::SPHERE);


	GAME_INSTNACE->Check_Collision_Layer(LayerNames[LAYER_PLAYER]
										, LayerNames[LAYER_TRIGGER]
										, CCollider::SPHERE
										, CCollider::SPHERE);
	return S_OK;
}


CLevel_Proto * CLevel_Proto::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_Proto*	pInstance = new CLevel_Proto(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Proto");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Proto::Free()
{
	__super::Free();


}
