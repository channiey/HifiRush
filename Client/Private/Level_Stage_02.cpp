#include "..\Default\stdafx.h"
#include "..\Public\Level_Stage_02.h"

#include "EngineInstance.h"
#include "Level_Loading.h"

CLevel_Stage_02::CLevel_Stage_02(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Stage_02::Initialize()
{
	if (FAILED(CLevel_Loading::Parse_LevelData(LV_STAGE_02)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Stage_02::Tick(_float fTimeDelta)
{
	return S_OK;
}

HRESULT CLevel_Stage_02::LateTick(_float fTimeDelta)
{
	SetWindowText(g_hWnd, LevelNames[LV_STAGE_02]);
	return S_OK;
}
CLevel_Stage_02 * CLevel_Stage_02::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_Stage_02*	pInstance = new CLevel_Stage_02(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Stage_02");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Stage_02::Free()
{
	__super::Free();


}
