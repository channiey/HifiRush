#include "..\Default\stdafx.h"
#include "..\Public\Level_Logo.h"

#include "GameInstance.h"
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

		//NULL_CHECK_RETURN(pGameInstance->Add_GameObject(LEVEL_LOGO, gStrLayerID[LAYER_ENVIORNMENT], TEXT("Env_Architecture")), E_FAIL);

		NULL_CHECK_RETURN(pGameInstance->Add_GameObject(LEVEL_LOGO, gStrLayerID[LAYER_CAMERA], TEXT("Camera_Debug")), E_FAIL);

		//NULL_CHECK_RETURN(pGameInstance->Add_GameObject(LEVEL_LOGO, gStrLayerID[LAYER_NPC], TEXT("Fiona")), E_FAIL);

	}
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevel_Logo::Tick(_float fTimeDelta)
{
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

	/* ������ü�� �����Ͽ� �纻��ü�� �����ϰ� ���̾ �߰��Ѵ�. */
	NULL_CHECK_RETURN(pGameInstance->Add_GameObject(LEVEL_LOGO, strLayerTag, TEXT("BackGround")), E_FAIL);
	

	Safe_Release(pGameInstance);

	return S_OK;
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
