#include "..\Default\stdafx.h"
#include "..\Public\Level_Logo.h"

#include "EngineInstance.h"
#include "GameObject.h"
#include "Level_Loading.h"

CLevel_Logo::CLevel_Logo(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Logo::Initialize()
{
	if (FAILED(CLevel_Loading::Parse_LevelData(LV_LOGO)))
		return E_FAIL;

	/*CGameObject* pGameObject = ENGINE_INSTANCE->Add_GameObject(LV_LOGO, LayerNames[LAYER_ID::LAYER_UI], L"Prototype_GameObject_BackGround");

	if (nullptr == pGameObject)
		return E_FAIL;*/

	return S_OK;
}

HRESULT CLevel_Logo::Tick(_float fTimeDelta)
{
	return S_OK;
}

HRESULT CLevel_Logo::LateTick(_float fTimeDelta)
{
	SetWindowText(g_hWnd, LevelNames[LV_LOGO]);

	if (CEngineInstance::GetInstance()->Key_Down(VK_SPACE))
	{
		CEngineInstance*	pGameInstance = CEngineInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		if (FAILED(pGameInstance->Open_Level(LV_LOADING,  CLevel_Loading::Create(m_pDevice, m_pContext, LV_STAGE_01))))
			return E_FAIL;

		Safe_Release(pGameInstance);
	}

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
