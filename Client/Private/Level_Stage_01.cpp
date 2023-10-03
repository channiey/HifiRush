#include "..\Default\stdafx.h"
#include "..\Public\Level_Stage_01.h"

#include "GameInstance.h"
#include "Level_Loading.h"

CLevel_Stage_01::CLevel_Stage_01(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Stage_01::Initialize()
{
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
	SetWindowText(g_hWnd, gStrLevelID[LV_STAGE_01]);
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
