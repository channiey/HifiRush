#include "..\Default\stdafx.h"
#include "..\Public\Loader.h"

#include "GameInstance.h"

#include "Util_File.h"
#include "Util_String.h"

/* Character*/
#include "Chai.h"
#include "Peppermint.h"

/* Weapon */
#include "Weapon.h"

/* Camera */
#include "Camera_Debug.h"

/* UI */
#include "BackGround.h"

/* Env */
#include "StaticDummy.h"



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
	case LV_LOGO:
	{
		if (!g_bLoadPrototype)
		{
			hr = Load_Prototype();
			g_bLoadPrototype = TRUE;
		}
		else
		{
			m_strLoading = TEXT("Loading Finish");
			m_isFinished = TRUE;
			hr = S_OK;
		}
	}
	break;
	default:
	{
		m_strLoading = TEXT("Loading Finish");
		m_isFinished = TRUE;
		hr = S_OK;
	}
		break;
	}
	if (FAILED(hr))
		return -1;	

	LeaveCriticalSection(&m_Critical_Section);

	return 0;	
}

HRESULT CLoader::Load_Prototype()
{
	//CThreadPool				threads(10);
	//vector<future<HRESULT>> futures;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* For.Prototype GameObject */
	m_strLoading = TEXT("Loding... : Prototype Object");
	{
		/* For.Prototype_GameObject_BackGround */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Ui_BackGround"), CBackGround::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Camera_Debug */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Camera_Debug"),
			CCamera_Debug::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Player_Chai */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Player_Chai"),
			CChai::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Proto_Weapon */
		{
			const string		tag = "Weapon_";
			const string		filePath = "../Bin/Resources/Models/Weapon";
			vector<string>		fileNames = Util_File::GetAllFolderNames(filePath);

			for (string& name : fileNames)
			{
				if (FAILED(pGameInstance->Add_Prototype(Util_String::ToWString(tag + name), CWeapon::Create(m_pDevice, m_pContext))))
					return E_FAIL;
			}
		}
		

		/* For.Prototype_GameObject_Proto_Static */
		//{
		//	/* 해당 경로 내의 모든 폴더명을 읽어 폴더명으로 오브젝트를 생성한다. */
		//	const string		tag = "Env_Static_";
		//	const string		filePath = "../Bin/Resources/Models/Environment/Static";
		//	vector<string>		fileNames = Util_File::GetAllFolderNames(filePath);

		//	for (string& name : fileNames)
		//	{
		//		if (FAILED(pGameInstance->Add_Prototype(Util_String::ToWString(tag + name), CStaticDummy::Create(m_pDevice, m_pContext))))
		//			return E_FAIL;
		//	}
		//}
	}

	/* For.Component */
	m_strLoading = TEXT("Loding... : CComponent");
	{

		/* For.Prototype_Component_Collider_AABB */
		if (FAILED(pGameInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_Collider_AABB"),
			CCollider_AABB::Create(m_pDevice, m_pContext, CCollider::AABB))))
			return E_FAIL;

		/* For.Prototype_Component_Collider_OBB */
		if (FAILED(pGameInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_Collider_OBB"),
			CCollider_OBB::Create(m_pDevice, m_pContext, CCollider::OBB))))
			return E_FAIL;

		/* For.Prototype_Component_Collider_Sphere */
		if (FAILED(pGameInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
			CCollider_Sphere::Create(m_pDevice, m_pContext, CCollider::SPHERE))))
			return E_FAIL;

		/* For.Prototype_Component_StateMachine */
		if (FAILED(pGameInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_StateMachine"),
			CStateMachine::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_Component_StateMachine */
		if (FAILED(pGameInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_Camera"),
			CCamera::Create(m_pDevice, m_pContext))))
			return E_FAIL;
	}

	/* For.Texture */
	m_strLoading = TEXT("Loding... : Texture");
	{
		/* For.Prototype_Component_Texture_BackGround*/
		if (FAILED(pGameInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_Texture_BackGround"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Prototype/Default%d.jpg"), 2))))
			return E_FAIL;

		/* For.Prototype_Component_Texture_Terrain*/
		if (FAILED(pGameInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_Texture_Terrain"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Prototype/Terrain/Tile%d.dds"), 2))))
			return E_FAIL;

		/* For.Prototype_Component_Texture_Terrain_Mask */
		if (FAILED(pGameInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_Texture_Terrain_Mask"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Prototype/Terrain/Mask.bmp"), 1))))
			return E_FAIL;
	}

	/* For.Shader */
	m_strLoading = TEXT("Loding... : Shader");
	{
		/* For.Prototype_Component_Shader_VtxNorTex*/
		if (FAILED(pGameInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_Shader_VtxNorTex"),
			CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
			return E_FAIL;

		/* For.Prototype_Component_Shader_Model */
		if (FAILED(pGameInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_Shader_Model"),
			CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxModel.hlsl"), VTXMODEL_DECLARATION::Elements, VTXMODEL_DECLARATION::iNumElements))))
			return E_FAIL;

		/* For.Prototype_Component_Shader_AnimModel */
		if (FAILED(pGameInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_Shader_AnimModel"),
			CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimModel.hlsl"), VTXANIMMODEL_DECLARATION::Elements, VTXANIMMODEL_DECLARATION::iNumElements))))
			return E_FAIL;

		/* For.Prototype_Component_Shader_VtxMesh */
		if (FAILED(pGameInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_Shader_VtxMesh"),
			CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxMesh.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
			return E_FAIL;

		/* For.Prototype_Component_Shader_VTF */
		if (FAILED(pGameInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_Shader_VTF"),
			CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VTF.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
			return E_FAIL;
	}

	/* For.Model */
	m_strLoading = TEXT("Loding... : Model");
	{
		Matrix PivotMatrix = Matrix::Identity;

		/* For.Prototype_Component_Model_Chai */
		PivotMatrix = Matrix::CreateRotationY(DEG2RAD(180.f)) * Matrix::CreateScale(0.01f);
		if (FAILED(pGameInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_Model_Chai"),
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Character/Chai", PivotMatrix))))
			return E_FAIL;

		/* For.Prototype_Component_Model_Weapon */
		{
			PivotMatrix = Matrix::CreateRotationY(DEG2RAD(180.f)) * Matrix::CreateRotationZ(DEG2RAD(180.f)) * Matrix::CreateRotationX(DEG2RAD(90.f)); 

			const string		tag = "Prototype_Component_Model_Weapon_";
			const string		filePath = "../Bin/Resources/Models/Weapon";
			vector<string>		fileNames = Util_File::GetAllFolderNames(filePath);

			for (string& name : fileNames)
			{
				if (FAILED(pGameInstance->Add_PrototypeCom(LV_STATIC, Util_String::ToWString(tag + name),
					CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Weapon/" + name, PivotMatrix))))
					return E_FAIL;
			}
		}


		/* For.Prototype_Component_Model_Static */
		//{
		//	PivotMatrix = Matrix::Identity * Matrix::CreateScale(0.01f);

		//	/* 해당 경로 내의 모든 폴더명을 읽어 폴더명으로 오브젝트를 생성한다. */
		//	const string		tag = "Prototype_Component_Model_Static_Env_Static_";
		//	const string		filePath = "../Bin/Resources/Models/Environment/Static";
		//	vector<string>		fileNames = Util_File::GetAllFolderNames(filePath);

		//	for (string& name : fileNames)
		//	{
		//		if (FAILED(pGameInstance->Add_PrototypeCom(LV_STATIC, Util_String::ToWString(tag + name),
		//			CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Environment/Static/" + name, PivotMatrix))))
		//			return E_FAIL;
		//		/*threads.Push_Command(std::bind(&CGameInstance::Add_PrototypeCom, &pGameInstance,
		//			LV_STATIC, Util_String::ToWString(tag + name),
		//			CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Environment/Static/" + name, PivotMatrix))
		//		);*/
		//	}
		//}
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