#include "..\Default\stdafx.h"
#include "..\Public\Loader.h"

#include "EngineInstance.h"

#include "Util_File.h"
#include "Util_String.h"

/* Character*/
#include "Chai.h"
#include "Saber.h"
#include "Blader.h"

/* Weapon */
#include "Weapon.h"

/* Camera */
#include "Camera_Debug.h"
#include "Camera_Follow.h"
#include "Camera_Parry.h"

/* UI */
#include "Hud.h"

/* Env */
#include "SkyBox.h"
#include "SkySphere.h"
#include "StaticDummy.h"
#include "Terrain.h"

/* Trigger */
#include "TriggerDummy.h"

/* Weapon */
#include "Chai_Guitar_Explore.h"
#include "Saber_Sword.h"
#include "Blader_Arm.h"

/* UI */

/* Trigger */
#include "TriggerSection_A.h"
#include "TriggerSection_B.h"

CLoader::CLoader(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

_uint APIENTRY ThreadEntry(void* pArg)
{
	HRESULT hr = CoInitializeEx(nullptr, 0);

	CLoader*		pLoader = (CLoader*)pArg;

	pLoader->Loading();

	return 0;
}

HRESULT CLoader::Initialize(LEVEL_ID eNextLevel)
{
	InitializeCriticalSection(&m_Critical_Section);

	m_eNextLevel = eNextLevel;

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadEntry, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

_int CLoader::Loading()
{
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
	CEngineInstance* pEngineInstance = CEngineInstance::GetInstance();
	Safe_AddRef(pEngineInstance);

	/* For.Component */
	m_strLoading = TEXT("Loding... : CComponent");
	{
		/* For.Prototype_Component_VIBuffer_Terrain*/
		if (FAILED(pEngineInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_VIBuffer_Terrain"),
			CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Prototype/Terrain/Height1.bmp")))))
			return E_FAIL;

		/* For.Prototype_Component_VIBuffer_Cube */
		if (FAILED(pEngineInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"),
			CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_Component_Camera */
		if (FAILED(pEngineInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_Camera"),
			CCamera::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_Component_Rigidbody */
		if (FAILED(pEngineInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_Rigidbody"),
			CRigidbody::Create(m_pDevice, m_pContext))))
			return E_FAIL;
		
		/* For.Prototype_Component_StateMachine */
		if (FAILED(pEngineInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_StateMachine"),
			CStateMachine::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_Component_BehaviourTree */
		if (FAILED(pEngineInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_BehaviourTree"),
			CBehaviourTree::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_Component_NavMeshAgent */
		if (FAILED(pEngineInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_NavMeshAgent"),
			CNavMeshAgent::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_Component_Collider_AABB */
		if (FAILED(pEngineInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_Collider_AABB"),
			CCollider_AABB::Create(m_pDevice, m_pContext, CCollider::AABB))))
			return E_FAIL;

		/* For.Prototype_Component_Collider_OBB */
		if (FAILED(pEngineInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_Collider_OBB"),
			CCollider_OBB::Create(m_pDevice, m_pContext, CCollider::OBB))))
			return E_FAIL;

		/* For.Prototype_Component_Collider_Sphere */
		if (FAILED(pEngineInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
			CCollider_Sphere::Create(m_pDevice, m_pContext, CCollider::SPHERE))))
			return E_FAIL;
	}

	/* For.Texture */
	m_strLoading = TEXT("Loding... : Texture");
	{
		/* 사용하는 모든 텍스처 일괄 로드 */
		string				fileRootPath	=	"../Bin/Resources/Textures/Using";
		vector<string>		fileNames		=	Util_File::GetAllFileNames(fileRootPath);

		for (string& name : fileNames)
		{
			const wstring fileFinalPath = Util_String::ToWString(fileRootPath) + L"/" + Util_String::ToWString(name + ".png");

			if (FAILED(pEngineInstance->Add_PrototypeCom(LV_STATIC, Util_String::ToWString(name), CTexture::Create(m_pDevice, m_pContext, fileFinalPath))))
				return E_FAIL;		
		}
	}

	/* For.Shader */
	m_strLoading = TEXT("Loding... : Shader");
	{
		/* For.Prototype_Component_Shader_VtxNorTex*/
		if (FAILED(pEngineInstance->Add_PrototypeCom(LV_STATIC, ShaderNames[SHADER_NORMAL],
			CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
			return E_FAIL;

		/* For.Prototype_Component_Shader_VtxMesh */
		if (FAILED(pEngineInstance->Add_PrototypeCom(LV_STATIC, ShaderNames[SHADER_STATIC],
			CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxMesh.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
			return E_FAIL;

		/* For.Prototype_Component_Shader_VTF */
		if (FAILED(pEngineInstance->Add_PrototypeCom(LV_STATIC, ShaderNames[SHADER_VTF],
			CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VTF.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
			return E_FAIL;

		/* For.Prototype_Component_Shader_VtxCube */
		if (FAILED(pEngineInstance->Add_PrototypeCom(LV_STATIC, ShaderNames[SHADER_CUBE],
			CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxCube.hlsl"), VTXCUBE::Elements, VTXCUBE::iNumElements))))
			return E_FAIL;

		/* For.Prototype_Component_Shader_VtxSkySphere */
		if (FAILED(pEngineInstance->Add_PrototypeCom(LV_STATIC, ShaderNames[SHADER_SKY_SPHERE],
			CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxSkySphere.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
			return E_FAIL;

	}

	/* For.Model */
	m_strLoading = TEXT("Loding... : Model");
	{
		Matrix PivotMatrix;

		/* For.Prototype_Component_Model_Chai */
		PivotMatrix = Matrix::CreateRotationY(DEG2RAD(270.f)) * Matrix::CreateScale(0.0125f);
		if (FAILED(pEngineInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_Model_Chai"),
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Character/Chai", PivotMatrix))))
			return E_FAIL;

		/* For.Prototype_Component_Model_Saber */
		PivotMatrix = Matrix::CreateRotationY(DEG2RAD(270.f)) * Matrix::CreateScale(0.0125f);
		if (FAILED(pEngineInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_Model_Saber"),
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Character/Saber", PivotMatrix))))
			return E_FAIL;

		/* For.Prototype_Component_Model_Blader */
		PivotMatrix = Matrix::CreateRotationY(DEG2RAD(270.f)) * Matrix::CreateScale(0.0125f);
		if (FAILED(pEngineInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_Model_Blader"),
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Character/Blader", PivotMatrix))))
			return E_FAIL;


		/* For.Prototype_Component_Model_Chai_Guitar_Explore */
		{
			PivotMatrix = Matrix::CreateRotationY(DEG2RAD(180.f)) * Matrix::CreateRotationZ(DEG2RAD(180.f)) * Matrix::CreateRotationX(DEG2RAD(90.f));

			const string		tag = "Prototype_Component_Model_Weapon_Chai_Guitar_Explore";
			const string		filePath = "../Bin/Resources/Models/Weapon/Chai_Guitar_Explore";

			if (FAILED(pEngineInstance->Add_PrototypeCom(LV_STATIC, Util_String::ToWString(tag),
				CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Weapon/Chai_Guitar_Explore", PivotMatrix))))
				return E_FAIL;
		}
		/* For.Prototype_Component_Model_Saber_Sword */
		{
			PivotMatrix = Matrix::CreateRotationZ(DEG2RAD(90.f)) * Matrix::CreateRotationX(DEG2RAD(90.f));
			const string		tag = "Prototype_Component_Model_Weapon_Saber_Sword";
			const string		filePath = "../Bin/Resources/Models/Weapon/Saber_Sword";

			if (FAILED(pEngineInstance->Add_PrototypeCom(LV_STATIC, Util_String::ToWString(tag),
				CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Weapon/Saber_Sword", PivotMatrix))))
				return E_FAIL;
		}

		/* For.Prototype_Component_Model_Static */
		{
			PivotMatrix = Matrix::Identity * Matrix::CreateScale(0.01f);

			/* 해당 경로 내의 모든 폴더명을 읽어 폴더명으로 오브젝트를 생성한다. */
			const string		tag = "Prototype_Component_Model_Static_Env_Static_";
			const string		filePath = "../Bin/Resources/Models/Environment/Static";
			vector<string>		fileNames = Util_File::GetAllFolderNames(filePath);

			for (string& name : fileNames)
			{
				if (FAILED(pEngineInstance->Add_PrototypeCom(LV_STATIC, Util_String::ToWString(tag + name),
					CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Environment/Static/" + name, PivotMatrix))))
					return E_FAIL;
			}
		}

		/* For.Prototype_Component_Model_Geometry */
		PivotMatrix = Matrix::Identity * Matrix::CreateScale(0.01f);

  		if (FAILED(pEngineInstance->Add_PrototypeCom(LV_STATIC, Util_String::ToWString("Prototype_Component_Model_Static_Geometry_Sphere"),
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Geometry/Sphere", PivotMatrix))))
			return E_FAIL;
	}

	/* For.Prototype GameObject */
	m_strLoading = TEXT("Loding... : Prototype Object");
	{
		/* For.Prototype_GameObject_Hud */
		if (FAILED(pEngineInstance->Add_Prototype(TEXT("Ui_Hud"), 
			CHud::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Camera_Debug */
		if (FAILED(pEngineInstance->Add_Prototype(TEXT("Camera_Debug"),
			CCamera_Debug::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Camera_Follow */
		if (FAILED(pEngineInstance->Add_Prototype(TEXT("Camera_Follow"),
			CCamera_Follow::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Camera_Parry */
		if (FAILED(pEngineInstance->Add_Prototype(TEXT("Camera_Parry"),
			CCamera_Parry::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Player_Chai */
		if (FAILED(pEngineInstance->Add_Prototype(TEXT("Player_Chai"),
			CChai::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Enemy_Saber */
		if (FAILED(pEngineInstance->Add_Prototype(TEXT("Enemy_Saber"),
			CSaber::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Enemy_Blader */
		if (FAILED(pEngineInstance->Add_Prototype(TEXT("Enemy_Blader"),
			CBlader::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Trigger_Dummy */
		if (FAILED(pEngineInstance->Add_Prototype(TEXT("Trigger_Dummy"),
			CTriggerDummy::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Trigger_Section_A */
		if (FAILED(pEngineInstance->Add_Prototype(TEXT("Trigger_Section_A"),
			CTriggerSection_A::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Trigger_Section_B */
		if (FAILED(pEngineInstance->Add_Prototype(TEXT("Trigger_Section_B"),
			CTriggerSection_B::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Weapon_Blader_Arm */
		if (FAILED(pEngineInstance->Add_Prototype(TEXT("Weapon_Blader_Arm"),
			CBlader_Arm::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Weapon_Chai_Guitar_Explore */
		{
			const string		tag = "Weapon_Chai_Guitar_Explore";
			const string		filePath = "../Bin/Resources/Models/Weapon/Chai_Guitar_Explore";

			if (FAILED(pEngineInstance->Add_Prototype(Util_String::ToWString(tag), CChai_Guitar_Explore::Create(m_pDevice, m_pContext))))
				return E_FAIL;
		}

		/* For.Prototype_GameObject_Weapon_Saber_Sword */
		{
			const string		tag = "Weapon_Saber_Sword";
			const string		filePath = "../Bin/Resources/Models/Weapon/Saber_Sword";

			if (FAILED(pEngineInstance->Add_Prototype(Util_String::ToWString(tag), CSaber_Sword::Create(m_pDevice, m_pContext))))
				return E_FAIL;
		}

		/* For.Prototype_GameObject_Proto_Static */
		{
			/* 해당 경로 내의 모든 폴더명을 읽어 폴더명으로 오브젝트를 생성한다. */
			const string		tag = "Env_Static_";
			const string		filePath = "../Bin/Resources/Models/Environment/Static";
			vector<string>		fileNames = Util_File::GetAllFolderNames(filePath);

			for (string& name : fileNames)
			{
				if (FAILED(pEngineInstance->Add_Prototype(Util_String::ToWString(tag + name), CStaticDummy::Create(m_pDevice, m_pContext))))
					return E_FAIL;
			}
		}

		/* For.Prototype_GameObject_Proto_Sky_Sphere*/
		if (FAILED(pEngineInstance->Add_Prototype(Util_String::ToWString("Env_SkySphere"),
			CSkySphere::Create(m_pDevice, m_pContext))))
			return E_FAIL;
	}

	m_strLoading = TEXT("Loading Finish");
	m_isFinished = true;
	Safe_Release(pEngineInstance);

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