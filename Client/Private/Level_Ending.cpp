#include "..\Default\stdafx.h"
#include "..\Public\Level_Ending.h"

#include "GameInstance.h"
#include "Level_Loading.h"

CLevel_Ending::CLevel_Ending(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Ending::Initialize()
{
	if (FAILED(CLevel_Loading::Parse_LevelData(LV_ENDING)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Ending::Tick(_float fTimeDelta)
{
	return S_OK;
}

HRESULT CLevel_Ending::LateTick(_float fTimeDelta)
{
	SetWindowText(g_hWnd, LevelNames[LV_ENDING]);
	return S_OK;
}
CLevel_Ending * CLevel_Ending::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_Ending*	pInstance = new CLevel_Ending(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Ending");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Ending::Free()
{
	__super::Free();


}
