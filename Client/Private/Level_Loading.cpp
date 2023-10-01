#include "..\Default\stdafx.h"
#include "..\Public\Level_Loading.h"

#include "GameInstance.h"
#include "Loader.h"

#include "GameObject.h"

#include "Level_Logo.h"
#include "Level_GamePlay.h"

#include "Util_String.h"
#include "Util_File.h"

#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG


CLevel_Loading::CLevel_Loading(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{
}


HRESULT CLevel_Loading::Initialize(LEVEL_ID eNextLevel)
{
#ifdef _DEBUG
	FAILED_CHECK_RETURN(CImGui_Manager::GetInstance()->Clear_ReferenceData(), E_FAIL);
#endif // _DEBUG

	m_eNextLevel = eNextLevel;

	/* m_eNextLevel 에 대한 로딩작업을 수행한다. */
	/* 로딩을 겁나 하고있다. */
	m_pLoader = CLoader::Create(m_pDevice, m_pContext, m_eNextLevel);
	if (nullptr == m_pLoader)
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Loading::Tick(_float fTimeDelta)
{
	return S_OK;
}

HRESULT CLevel_Loading::LateTick(_float fTimeDelta)
{
	wstring		strLoadingText = m_pLoader->Get_LoadingText();

	SetWindowText(g_hWnd, strLoadingText.c_str());

	//if (CGameInstance::GetInstance()->Key_Down(VK_SPACE))
	{
		if (true == m_pLoader->Get_Finished())
		{
			CGameInstance*	pGameInstance = CGameInstance::GetInstance();
			Safe_AddRef(pGameInstance);

			CLevel*		pNewLevel = nullptr;

			switch (m_eNextLevel)
			{
			case LEVEL_LOGO:
				pNewLevel = CLevel_Logo::Create(m_pDevice, m_pContext);
				break;
			case LEVEL_GAMEPLAY:
				pNewLevel = CLevel_GamePlay::Create(m_pDevice, m_pContext);
				break;
			}

			if (nullptr == pNewLevel)
				return E_FAIL;

			if (FAILED(pGameInstance->Open_Level(m_eNextLevel, pNewLevel)))
				return E_FAIL;		

			Safe_Release(pGameInstance);
		}
	}


	return S_OK;
}

HRESULT CLevel_Loading::Parse_LevelData(const _uint& iLevelID)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	shared_ptr<Util_File> file = make_shared<Util_File>();
	if (nullptr == file) return E_FAIL;

	file->Open(gStrLevelPath[iLevelID], FileMode::Read);

	/* 현재 레벨의 전체 레이어 갯수 */
	const size_t countLayer = file->Read<size_t>();

	for (size_t i = 0; i < countLayer; i++)
	{
		/* 현재 레이어의 전체 오브젝트 갯수 */
		const size_t countObject = file->Read<size_t>();

		for (size_t i = 0; i < countObject; i++)
		{
			/* 읽을 메인 데이터 */
			wstring strName = Util_String::ToWString(file->Read<string>());
			wstring strLayer = Util_String::ToWString(file->Read<string>());
			_bool	bActive = file->Read<_bool>();
			_bool	bRender = file->Read<_bool>();
			Matrix	tMatrix = file->Read<Matrix>();

			/* 읽은 데이터를 바탕으로 파싱한다. */
			{
				CGameObject* pObj = pGameInstance->Add_GameObject(iLevelID, strLayer, strName);
				if (nullptr == pObj) continue;

				pObj->Set_Active(bActive);
				pObj->Set_Render(bRender);

				CTransform* pTransform = pObj->Get_Transform();
				if (nullptr == pTransform) continue;

				pTransform->Set_WorldMat(tMatrix);
			}
		}
	}

	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_Loading * CLevel_Loading::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVEL_ID eNextLevel)
{
	CLevel_Loading*	pInstance = new CLevel_Loading(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevel)))
	{
		MSG_BOX("Failed to Created : CLevel_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);
}
