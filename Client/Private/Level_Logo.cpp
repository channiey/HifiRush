#include "..\Default\stdafx.h"
#include "..\Public\Level_Logo.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "Level_Loading.h"

CLevel_Logo::CLevel_Logo(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Logo::Initialize()
{
	/*if (FAILED(Ready_Layer_BackGround(gStrLayerID[LAYER_BACKGROUND])))
		return E_FAIL;*/
	
	/*if (FAILED(CLevel_Loading::Parse_LevelData(LEVEL_LOGO)))
		return E_FAIL;*/

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	{
		//NULL_CHECK_RETURN(pGameInstance->Add_GameObject(LEVEL_LOGO, gStrLayerID[LAYER_BACKGROUND], TEXT("Ui_BackGround")), E_FAIL);

		NULL_CHECK_RETURN(pGameInstance->Add_GameObject(LEVEL_LOGO, gStrLayerID[LAYER_PLAYER], TEXT("Player_Chai")), E_FAIL);


		NULL_CHECK_RETURN(pGameInstance->Add_GameObject(LEVEL_LOGO, gStrLayerID[LAYER_ENVIORNMENT], TEXT("Env_Terrain")), E_FAIL);

		NULL_CHECK_RETURN(pGameInstance->Add_GameObject(LEVEL_LOGO, gStrLayerID[LAYER_ENVIORNMENT], TEXT("Env_Architecture")), E_FAIL);

		NULL_CHECK_RETURN(pGameInstance->Add_GameObject(LEVEL_LOGO, gStrLayerID[LAYER_CAMERA], TEXT("Camera_Debug")), E_FAIL);

		//NULL_CHECK_RETURN(pGameInstance->Add_GameObject(LEVEL_LOGO, gStrLayerID[LAYER_NPC], TEXT("Fiona")), E_FAIL);

	}
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevel_Logo::Tick(_float fTimeDelta)
{
	//Picking_Terrain();

	//Picking_Object();

	return S_OK;
}

HRESULT CLevel_Logo::LateTick(_float fTimeDelta)
{
	SetWindowText(g_hWnd, gStrLevelID[LEVEL_LOGO]);

	if (CGameInstance::GetInstance()->Key_Down(VK_SPACE))
	{
		CGameInstance*	pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING,  CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_GAMEPLAY))))
			return E_FAIL;

		Safe_Release(pGameInstance);
	}

	return S_OK;
}


HRESULT CLevel_Logo::Ready_Layer_BackGround(const wstring & strLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* 원형객체를 복제하여 사본객체를 생성하고 레이어에 추가한다. */
	NULL_CHECK_RETURN(pGameInstance->Add_GameObject(LEVEL_LOGO, strLayerTag, TEXT("BackGround")), E_FAIL);
	

	Safe_Release(pGameInstance);

	return S_OK;
}

void CLevel_Logo::Picking_Terrain()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	{
		if (pGameInstance->Key_Down(VK_LBUTTON))
		{
			Vec3 vPickPos;

			CGameObject* pTerrain = pGameInstance->Get_GameObject(LEVEL_LOGO, gStrLayerID[LAYER_ENVIORNMENT], TEXT("Env_Terrain_000"));
			if (nullptr != pTerrain)
			{
				CVIBuffer_Terrain*  pBuffer = dynamic_cast<CVIBuffer_Terrain*>(pTerrain->Get_VIBuffer());
				if (pGameInstance->Get_PickPos_Terrain(pBuffer, pTerrain->Get_Transform()->Get_WorldMat(), vPickPos))
				{
					CGameObject* pObj = pGameInstance->Add_GameObject(LEVEL_LOGO, gStrLayerID[LAYER_PLAYER], TEXT("Player_Chai"));
					if(nullptr != pObj)
						pObj->Get_Transform()->Set_Position(vPickPos);
				}
			}
		}
	}
	RELEASE_INSTANCE(CGameInstance);
}

void CLevel_Logo::Picking_Object()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	{
		if (pGameInstance->Key_Down(VK_LBUTTON))
		{
			CGameObject* pGameObject = pGameInstance->Get_Pick_Object();
			if (nullptr != pGameObject)
			{
				int k = 0;
			}
		}
	}
	RELEASE_INSTANCE(CGameInstance);
}

CLevel_Logo * CLevel_Logo::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_Logo*	pInstance = new CLevel_Logo(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Logo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Logo::Free()
{
 	__super::Free();


}
