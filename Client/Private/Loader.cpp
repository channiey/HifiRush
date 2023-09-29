#include "..\Default\stdafx.h"
#include "..\Public\Loader.h"

#include "GameInstance.h"

/* Character*/
#include "Player.h"
#include "Architecture.h"
#include "Fiona.h"

/* Camera */
#include "Camera_Debug.h"

/* UI */
#include "BackGround.h"

/* Env */
#include "Terrain.h"

CLoader::CLoader(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

_uint APIENTRY ThreadEntry(void* pArg)
{
	/* DX가 사용하는 모든 컴 객체를 초기화한다.  */
	/* ID3D11Device는 공유 스레드이므로 스레드간 동시 접근이 가능하지만 */
	/* ID3D11DeviceContext는 단일 스레드 접근만 허용하므로 스레드 동기화를 필요로 한다. */

	HRESULT hr = CoInitializeEx(nullptr, 0);

	/* 새롭게 생성된 스레드가 일한다. */
	CLoader*		pLoader = (CLoader*)pArg;

	pLoader->Loading();

	return 0;
}

HRESULT CLoader::Initialize(LEVEL_ID eNextLevel)
{
	InitializeCriticalSection(&m_Critical_Section);

	m_eNextLevel = eNextLevel;

	/* 생성한 스레드가 호출해야하는 함수의 주소를 3번째 인자로 넣어준다. */
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadEntry, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

_int CLoader::Loading()
{
	/* 참고로 모든 레벨에서 사용되는 컴포넌트 원형은 MainApp에서 생성한다. */

	EnterCriticalSection(&m_Critical_Section);

	HRESULT		hr = 0;

	switch (m_eNextLevel)
	{
	case LEVEL_LOGO:
		hr = Loading_For_Level_Logo();
		break;
	case LEVEL_GAMEPLAY:
		hr = Loading_For_Level_GamePlay();
		break;
	}
	if (FAILED(hr))
		return -1;	

	LeaveCriticalSection(&m_Critical_Section);

	return 0;	
}

HRESULT CLoader::Loading_For_Level_Logo()
{
	/* 얘네 다 쓰레드 매니저로 처리하면 반환값을 못받으니 쓰레드가 처리할 함수 내부에서 assert로 예외처리 하자 (#if _DEBUG 필수) */

	/* 로고 레벨 로딩 단계에서 모든 오브젝트와 컴포넌트 원형을 생성해둔다. */
	/* 그리고 각 레벨에서는 레벨의 데이터를 읽어 필요한 원형을 클론해서 사용한다. */

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	m_strLoading = TEXT("Loding... : Texture");
	{
		/* For.Prototype_Component_Texture_BackGround*/
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_BackGround"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Prototype/Default%d.jpg"), 2))))
			return E_FAIL;

		/* For.Prototype_Component_Texture_Terrain*/
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Prototype/Terrain/Tile%d.dds"), 2))))
			return E_FAIL;

		/* For.Prototype_Component_Texture_Terrain_Mask */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain_Mask"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Prototype/Terrain/Mask.bmp"), 1))))
			return E_FAIL;
	}
	
	/* For.VIBuffer */
	m_strLoading = TEXT("Loding... : VIBuffer");
	{
		/* For.Prototype_Component_VIBuffer_Terrain*/
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
			CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Prototype/Terrain/Height1.bmp")))))
			return E_FAIL;
	}

	/* For.Model */
	m_strLoading = TEXT("Loding... : Model");
	{
		Matrix PivotMatrix = Matrix::Identity;

		/* For.Prototype_Component_Model_Chai */
		PivotMatrix = Matrix::CreateRotationY(DEG2RAD(180.f)) * Matrix::CreateScale(0.01f);
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Chai"),
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Characters/Chai", PivotMatrix))))
			return E_FAIL;

		/* For.Prototype_Component_Model_Fiona */
		PivotMatrix = Matrix::CreateRotationY(DEG2RAD(180.f));
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fiona"),
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Prototype/Fiona", PivotMatrix))))
			return E_FAIL;

		/* For.Prototype_Component_Model_Architecture */
		PivotMatrix = Matrix::Identity * Matrix::CreateScale(0.01f);
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Architecture"),
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Environment/Architecture", PivotMatrix))))
			return E_FAIL;

	}

	/* For.Shader */
	m_strLoading = TEXT("Loding... : Shader");
	{
		/* For.Prototype_Component_Shader_VtxNorTex*/
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxNorTex"),
			CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
			return E_FAIL;

		/* For.Prototype_Component_Shader_Model */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_Model"),
			CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxModel.hlsl"), VTXMODEL_DECLARATION::Elements, VTXMODEL_DECLARATION::iNumElements))))
			return E_FAIL;

		/* For.Prototype_Component_Shader_AnimModel */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_AnimModel"),
			CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimModel.hlsl"), VTXANIMMODEL_DECLARATION::Elements, VTXANIMMODEL_DECLARATION::iNumElements))))
			return E_FAIL;
	}

	/* For.Prototype GameObject */
	m_strLoading = TEXT("Loding... : Prototype Object");
	{
		/* For.Prototype_GameObject_BackGround */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Ui_BackGround"), CBackGround::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Terrain*/
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Env_Terrain"), CTerrain::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Camera_Debug */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Camera_Debug"),
			CCamera_Debug::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Player_Chai */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Player_Chai"),
			CPlayer::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Env_Architecture */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Env_Architecture"),
			CArchitecture::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Proto_Fiona */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Temp_Fiona"),
			CFiona::Create(m_pDevice, m_pContext))))
			return E_FAIL;
	}

	m_strLoading = TEXT("Loading Finish");
	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_For_Level_GamePlay()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	
	m_strLoading = TEXT("Loading Finish");
	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

CLoader * CLoader::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVEL_ID eNextLevel)
{
	CLoader*	pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevel)))
	{
		MSG_BOX("Failed to Created : CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	__super::Free();

	WaitForSingleObject(m_hThread, INFINITE);	

	DeleteCriticalSection(&m_Critical_Section);

	CloseHandle(m_hThread);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}