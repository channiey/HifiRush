#include "..\Default\stdafx.h"
#include "..\Public\Level_Stage_03.h"

#include "GameInstance.h"
#include "Level_Loading.h"

CLevel_Stage_03::CLevel_Stage_03(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Stage_03::Initialize()
{
	if (FAILED(CLevel_Loading::Parse_LevelData(LV_STAGE_03)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Stage_03::Tick(_float fTimeDelta)
{
	return S_OK;
}

HRESULT CLevel_Stage_03::LateTick(_float fTimeDelta)
{
	SetWindowText(g_hWnd, g_strLevelID[LV_STAGE_03]);
	return S_OK;
}
CLevel_Stage_03 * CLevel_Stage_03::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_Stage_03*	pInstance = new CLevel_Stage_03(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Stage_03");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Stage_03::Free()
{
	__super::Free();


}
