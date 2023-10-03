#include "..\Default\stdafx.h"
#include "..\Public\Level_Proto.h"

#include "GameInstance.h"
#include "Level_Loading.h"

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
