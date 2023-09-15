#include "..\Default\stdafx.h"
#include "..\Public\Loader.h"

#include "GameInstance.h"

#pragma region Level_Logo
#include "BackGround.h"
#pragma endregion

#pragma region Level_GamePlay
#include "Terrain.h"
#pragma endregion


CLoader::CLoader(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

_uint APIENTRY ThreadEntry(void* pArg)
{
	/* DX�� ����ϴ� ��� �� ��ü�� �ʱ�ȭ�Ѵ�.  */
	/* ID3D11Device�� ���� �������̹Ƿ� �����尣 ���� ������ ���������� */
	/* ID3D11DeviceContext�� ���� ������ ���ٸ� ����ϹǷ� ������ ����ȭ�� �ʿ�� �Ѵ�. */

	HRESULT hr = CoInitializeEx(nullptr, 0);

	/* ���Ӱ� ������ �����尡 ���Ѵ�. */
	CLoader*		pLoader = (CLoader*)pArg;

	pLoader->Loading();

	return 0;
}

HRESULT CLoader::Initialize(LEVEL_ID eNextLevel)
{
	InitializeCriticalSection(&m_Critical_Section);

	m_eNextLevel = eNextLevel;

	/* ������ �����尡 ȣ���ؾ��ϴ� �Լ��� �ּҸ� 3��° ���ڷ� �־��ش�. */
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadEntry, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

_int CLoader::Loading()
{
	/* ����� ��� �������� ���Ǵ� ������Ʈ ������ MainApp���� �����Ѵ�. */

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
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* For.Texture */
	m_strLoading = TEXT("Loding... : Texture");
	{
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_BackGround"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Default%d.jpg"), 2))))
			return E_FAIL;
	}
	
	/* For.Mesh */
	m_strLoading = TEXT("Loding... : Mesh");
	{

	}

	/* For.Shader */
	m_strLoading = TEXT("Loding... : Shader");
	{

	}

	/* For.GameObject */
	m_strLoading = TEXT("Loding... : Prototype");
	{
		/* For.Prototype_GameObject_BackGround */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"), CBackGround::Create(m_pDevice, m_pContext))))
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

	/* For.Texture */
	m_strLoading = TEXT("Loding... : Texture");
	{
		/* For.Prototype_Component_Texture_Terrain*/
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Tile0.jpg")))))
			return E_FAIL;
	}

	/* For.Mesh */
	m_strLoading = TEXT("Loding... : Mesh");
	{

	}

	/* For.VIBuffer */
	m_strLoading = TEXT("Loding... : VIBuffer");
	{
		/* For.Prototype_Component_VIBuffer_Terrain*/
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
			CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Height1.bmp")))))
			return E_FAIL;
	}


	/* For.Shader */
	m_strLoading = TEXT("Loding... : Shader");
	{
		/* For.Prototype_Component_Shader_VtxNorTex*/
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxNorTex"),
			CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
			return E_FAIL;
	}

	/* For.GameObject */
	m_strLoading = TEXT("Loding... : Prototype");
	{
		/* For.Prototype_GameObject_Terrain*/
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"), CTerrain::Create(m_pDevice, m_pContext))))
			return E_FAIL;

	}
	
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