#include "..\Default\stdafx.h"
#include "..\Public\Level_Proto.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "Level_Loading.h"
#include "ImGui_Manager.h"
#include "GameObject.h"
#include "Model.h"

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

	if (!CImGui_Manager::GetInstance()->Is_ClickedWindow())
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		{
			if (pGameInstance->Key_Down(VK_LBUTTON))
			{
				list<CGameObject*>* pLayer = pGameInstance->Get_Layer(LV_PROTO, gStrLayerID[LAYER_ENV_STATIC]);

				for (auto& pObj : *pLayer)
				{
					RAYHIT_DESC hit;
					if (pGameInstance->Check_Collision_CameraRay(pObj->Get_Model(), pObj->Get_Transform()->Get_WorldMat(), hit, TRUE))
					{
						int k = 0;
					}
				}
			}
		}
		RELEASE_INSTANCE(CGameInstance);
	}

	return S_OK;
}

HRESULT CLevel_Proto::LateTick(_float fTimeDelta)
{
	SetWindowText(g_hWnd, gStrLevelID[LV_PROTO]);
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
