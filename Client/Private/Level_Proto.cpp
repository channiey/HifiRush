#include "..\Default\stdafx.h"
#include "..\Public\Level_Proto.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "Level_Loading.h"
#include "ImGui_Manager.h"

CLevel_Proto::CLevel_Proto(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Proto::Initialize()
{
	if (FAILED(CLevel_Loading::Parse_LevelData(LV_PROTO)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Proto::Tick(_float fTimeDelta)
{

#ifdef _DEBUG

	if (!CImGui_Manager::GetInstance()->Is_ClickedWindow())
		Picking_Terrain();

#endif // _DEBUG

	return S_OK;
}

HRESULT CLevel_Proto::LateTick(_float fTimeDelta)
{
	SetWindowText(g_hWnd, gStrLevelID[LV_PROTO]);
	return S_OK;
}
void CLevel_Proto::Picking_Terrain()
{
	if (CImGui_Manager::GetInstance()->Is_Active()) return;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	{
		if (pGameInstance->Key_Down(VK_LBUTTON))
		{
			Vec3 vPickPos;

			CGameObject* pTerrain = pGameInstance->Get_GameObject(LV_PROTO, gStrLayerID[LAYER_ENV_STATIC], TEXT("Env_Terrain_000"));
			if (nullptr != pTerrain)
			{
				CVIBuffer_Terrain* pBuffer = dynamic_cast<CVIBuffer_Terrain*>(pTerrain->Get_VIBuffer());
				if (pGameInstance->Get_PickPos_Terrain(pBuffer, pTerrain->Get_Transform()->Get_WorldMat(), vPickPos))
				{
					CGameObject* pObj = pGameInstance->Add_GameObject(LV_PROTO, gStrLayerID[LAYER_PLAYER], TEXT("Env_Static_ForkLift"));
					if (nullptr != pObj)
						pObj->Get_Transform()->Set_Position(vPickPos);
				}
			}
		}
	}
	RELEASE_INSTANCE(CGameInstance);
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
