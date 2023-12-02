#include "..\Default\stdafx.h"
#include "..\Public\Loader.h"

#include "EngineInstance.h"

#include "Util_File.h"
#include "Util_String.h"

/* Character*/
#include "Chai.h"
#include "Peppermint.h"
#include "Macaron.h"
#include "Korsica.h"

#include "Saber.h"
#include "Blader.h"
#include "Gunner.h"

/* Weapon */
#include "Weapon.h"

/* Camera */
#include "Camera_Debug.h"
#include "Camera_Follow.h"
#include "Camera_Parry.h"
#include "Camera_Peppermint_Gimmick_Aim.h"
#include "Camera_Peppermint_Gimmick_Crane.h"
#include "Camera_Macaron_Gimmick_Wall.h"

/* UI */
#include "Ui_Hud.h"
#include "Ui_Parry_Rhythm.h"
#include "Ui_Fight_Logo.h"
#include "Ui_Dialouge.h"
#include "Ui_Hud_Boss.h"
#include "Ui_Gimmick_Peppermint_Aim.h"
#include "Ui_Gimmick_Peppermint_Target.h"
#include "Ui_Gimmick_Peppermint_Mark.h"
#include "Ui_SpecialAttack.h"
#include "BackGround.h"

/* Env Static */
#include "SkyBox.h"
#include "SkySphere.h"
#include "StaticDummy.h"
#include "Terrain.h"

/* Env Dynamic */
#include "Dynamic_Crane.h"
#include "Dynamic_Switch.h"
#include "Dynamic_Macaron_Wall.h"
#include "Dynamic_Bridge.h"
#include "Dynamic_AutoDoor.h"

/* Trigger */
#include "TriggerDummy.h"

/* Projectile */
#include "Peppermint_Bullet.h"
#include "Korsica_Wind.h"

/* Weapon */
#include "Chai_Guitar_Explore.h"
#include "Peppermint_Gun.h"
#include "Korsica_Stick.h"

#include "Saber_Sword.h"
#include "Blader_Arm.h"

/* Trigger */
#include "TriggerSection_A.h"
#include "TriggerSection_B.h"
#include "TriggerSection_C.h"

/* Effect */
#include "Effect_Trail_Sword.h"
#include "Effect_Damaged_Enemy.h"

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
		string				fileRootPath	=	"../Bin/Resources/Textures/Using";
		vector<string>		fileNames		=	Util_File::GetAllFileNames(fileRootPath);
		for (string& name : fileNames)
		{
			const wstring fileFinalPath = Util_String::ToWString(fileRootPath) + L"/" + Util_String::ToWString(name + ".png");

			if (FAILED(pEngineInstance->Add_PrototypeCom(LV_STATIC, Util_String::ToWString(name), CTexture::Create(m_pDevice, m_pContext, fileFinalPath))))
				return E_FAIL;		
		}

		fileRootPath		= "../Bin/Resources/Textures/Effects";
		fileNames			= Util_File::GetAllFileNames(fileRootPath);
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

		/* For.Prototype_Component_Shader_Effect_Tex */
		if (FAILED(pEngineInstance->Add_PrototypeCom(LV_STATIC, ShaderNames[SHADER_EFFECT_TEX],
			CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Effect_Tex.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
			return E_FAIL;

		/* For.Prototype_Component_Shader_Effect_Mesh */
		if (FAILED(pEngineInstance->Add_PrototypeCom(LV_STATIC, ShaderNames[SHADER_EFFECT_MESH],
			CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Effect_Mesh.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
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

		/* For.Prototype_Component_Model_Peppermint */
		PivotMatrix = Matrix::CreateRotationY(DEG2RAD(270.f)) * Matrix::CreateScale(0.015f);
		if (FAILED(pEngineInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_Model_Peppermint"),
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Character/Peppermint", PivotMatrix))))
			return E_FAIL;

		/* For.Prototype_Component_Model_Macaron */
		PivotMatrix = Matrix::CreateRotationY(DEG2RAD(270.f)) * Matrix::CreateScale(0.0125f);
		if (FAILED(pEngineInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_Model_Macaron"),
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Character/Macaron", PivotMatrix))))
			return E_FAIL;

		/* For.Prototype_Component_Model_Korsica */
		PivotMatrix = Matrix::CreateRotationY(DEG2RAD(270.f)) * Matrix::CreateScale(0.015f);
		if (FAILED(pEngineInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_Model_Korsica"),
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Character/Korsica", PivotMatrix))))
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

		/* For.Prototype_Component_Model_Gunner */
		PivotMatrix = Matrix::CreateRotationY(DEG2RAD(270.f)) * Matrix::CreateScale(0.0125f);
		if (FAILED(pEngineInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_Model_Gunner"),
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Character/Gunner", PivotMatrix))))
			return E_FAIL;

		/* For.Prototype_Component_Model_Dynamaic_Crane */
		PivotMatrix = Matrix::CreateScale(0.01f);
		if (FAILED(pEngineInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_Model_Crane"),
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Environment/Dynamic/Crane", PivotMatrix))))
			return E_FAIL;

		/* For.Prototype_Component_Model_Dynamaic_Switch */
		PivotMatrix = Matrix::CreateScale(0.01f);
		if (FAILED(pEngineInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_Model_Switch"),
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Environment/Dynamic/Switch", PivotMatrix))))
			return E_FAIL;

		/* For.Prototype_Component_Model_Dynamaic_AutoDoor */
		PivotMatrix = Matrix::CreateScale(0.01f);
		if (FAILED(pEngineInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_Model_AutoDoor"),
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Environment/Dynamic/AutoDoor", PivotMatrix))))
			return E_FAIL;

		/* For.Prototype_Component_Model_Dynamaic_DoorDebris */
		PivotMatrix = Matrix::CreateScale(0.01f);
		if (FAILED(pEngineInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_Model_DoorDebris"),
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Environment/Dynamic/DoorDebris", PivotMatrix))))
			return E_FAIL;

		/* For.Prototype_Component_Model_Custom_Wall_Macaron_Broken */
		PivotMatrix = Matrix::CreateScale(0.01f);
		if (FAILED(pEngineInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_Model_Wall_Macaron_Broken"),
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Environment/Custom/Wall_Macaron_Broken", PivotMatrix))))
			return E_FAIL;

		/* For.Prototype_Component_Model_Custom_Wall_Macaron_UnBroken */
		PivotMatrix = Matrix::CreateScale(0.01f);
		if (FAILED(pEngineInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_Model_Wall_Macaron_UnBroken"),
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Environment/Custom/Wall_Macaron_UnBroken", PivotMatrix))))
			return E_FAIL;

		/* For.Prototype_Component_Model_Dynamic_Bridge */
		PivotMatrix = Matrix::CreateScale(0.01f);
		if (FAILED(pEngineInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_Model_Dynamic_Bridge"),
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Environment/Dynamic/Bridge", PivotMatrix))))
			return E_FAIL;

		/* For.Prototype_Component_Model_Custom_Prop_Door_A_L */
		PivotMatrix = Matrix::CreateScale(0.01f);
		if (FAILED(pEngineInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_Model_Prop_Door_A_L"),
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Environment/Custom/Prop_Door_A_L", PivotMatrix))))
			return E_FAIL;

		/* For.Prototype_Component_Model_Custom_Prop_Door_A_R */
		PivotMatrix = Matrix::CreateScale(0.01f);
		if (FAILED(pEngineInstance->Add_PrototypeCom(LV_STATIC, TEXT("Prototype_Component_Model_Prop_Door_A_R"),
			CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Environment/Custom/Prop_Door_A_R", PivotMatrix))))
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

		/* For.Prototype_Component_Model_Peppermint_Gun */
		{
			PivotMatrix = Matrix::CreateRotationX(DEG2RAD(270.f));

			const string		tag = "Prototype_Component_Model_Weapon_Peppermint_Gun";
			const string		filePath = "../Bin/Resources/Models/Weapon/Peppermint_Gun";

			if (FAILED(pEngineInstance->Add_PrototypeCom(LV_STATIC, Util_String::ToWString(tag),
				CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Weapon/Peppermint_Gun", PivotMatrix))))
				return E_FAIL;
		}

		/* For.Prototype_Component_Model_Korsica_Stick */
		{
			PivotMatrix = Matrix::CreateRotationY(DEG2RAD(180.f)) * Matrix::CreateRotationZ(DEG2RAD(180.f)) * Matrix::CreateRotationX(DEG2RAD(90.f))
				* Matrix::CreateTranslation(Vec3{ -85.f, 0.f, 0.f });
				

			const string		tag = "Prototype_Component_Model_Weapon_Korsica_Stick";
			const string		filePath = "../Bin/Resources/Models/Weapon/Korsica_Stick";

			if (FAILED(pEngineInstance->Add_PrototypeCom(LV_STATIC, Util_String::ToWString(tag),
				CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Weapon/Korsica_Stick", PivotMatrix))))
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
		/* For.Prototype_GameObject_Proto_Sky_Sphere*/
		if (FAILED(pEngineInstance->Add_Prototype(Util_String::ToWString("Env_SkySphere"),
			CSkySphere::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Ui_Hud */
		if (FAILED(pEngineInstance->Add_Prototype(UINames[UI_ID::UI_HUD],
			CUi_Hud::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Ui_Parry_Rhythm */
		if (FAILED(pEngineInstance->Add_Prototype(UINames[UI_ID::UI_PARRY_RHYTHM],
			CUi_Parry_Rhythm::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Ui_Parry_Fight */
		if (FAILED(pEngineInstance->Add_Prototype(UINames[UI_ID::UI_FIGHT],
			CUi_Fight_Logo::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Ui_Parry_Dialouge */
		if (FAILED(pEngineInstance->Add_Prototype(UINames[UI_ID::UI_DIALOUGE],
			CUi_Dialouge::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Ui_Peppermint_Aim */
		if (FAILED(pEngineInstance->Add_Prototype(UINames[UI_ID::UI_PEPPERMINT_AIM],
			CUi_Peppermint_Aim::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Ui_Peppermint_Target */
		if (FAILED(pEngineInstance->Add_Prototype(UINames[UI_ID::UI_PEPPERMINT_TARGET],
			CUi_Peppermint_Target::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Ui_Hud_Boss */
		if (FAILED(pEngineInstance->Add_Prototype(UINames[UI_ID::UI_HUD_BOSS],
			CUi_Hud_Boss::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Ui_Hud_Boss */
		if (FAILED(pEngineInstance->Add_Prototype(UINames[UI_ID::UI_SPECIALATTACK],
			CUi_SpecialAttack::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_BackGround */
		if (FAILED(pEngineInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"), CBackGround::Create(m_pDevice, m_pContext))))
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

		/* For.Prototype_GameObject_Camera_Peppermint_Gimmick_Aim */
		if (FAILED(pEngineInstance->Add_Prototype(TEXT("Camera_Peppermint_Gimmick"),
			CCamera_Peppermint_Gimmick_Aim::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Camera_Peppermint_Gimmick_Crane */
		if (FAILED(pEngineInstance->Add_Prototype(TEXT("Camera_Peppermint_Gimmick_Crane"),
			CCamera_Peppermint_Gimmick_Crane::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Camera_Macaron_Gimmick_Wall */
		if (FAILED(pEngineInstance->Add_Prototype(TEXT("Camera_Macaron_Gimmick_Wall"),
			CCamera_Macaron_Gimmick_Wall::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Player_Chai */
		if (FAILED(pEngineInstance->Add_Prototype(TEXT("Player_Chai"),
			CChai::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Player_Peppermint */
		if (FAILED(pEngineInstance->Add_Prototype(TEXT("Player_Peppermint"),
			CPeppermint::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Player_Macaron */
		if (FAILED(pEngineInstance->Add_Prototype(TEXT("Player_Macaron"),
			CMacaron::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Player_Korsica */
		if (FAILED(pEngineInstance->Add_Prototype(TEXT("Player_Korsica"),
			CKorsica::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Enemy_Saber */
		if (FAILED(pEngineInstance->Add_Prototype(TEXT("Enemy_Saber"),
			CSaber::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Enemy_Blader */
		if (FAILED(pEngineInstance->Add_Prototype(TEXT("Enemy_Blader"),
			CBlader::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Enemy_Gunner */
		if (FAILED(pEngineInstance->Add_Prototype(TEXT("Enemy_Gunner"),
			CGunner::Create(m_pDevice, m_pContext))))
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

		/* For.Prototype_GameObject_Trigger_Section_C */
		if (FAILED(pEngineInstance->Add_Prototype(TEXT("Trigger_Section_C"),
			CTriggerSection_C::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Weapon_Blader_Arm */
		if (FAILED(pEngineInstance->Add_Prototype(TEXT("Weapon_Blader_Arm"),
			CBlader_Arm::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Weapon_Chai_Guitar_Explore */
		if (FAILED(pEngineInstance->Add_Prototype(TEXT("Weapon_Chai_Guitar_Explore"), CChai_Guitar_Explore::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Weapon_Peppermint_Gun */
		if (FAILED(pEngineInstance->Add_Prototype(TEXT("Weapon_Peppermint_Gun"), CPeppermint_Gun::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Weapon_Korsica_Stick */
		if (FAILED(pEngineInstance->Add_Prototype(TEXT("Weapon_Korsica_Stick"), CKorsica_Stick::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Weapon_Saber_Sword */
		if (FAILED(pEngineInstance->Add_Prototype(TEXT("Weapon_Saber_Sword"), CSaber_Sword::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Projectile_Peppermint_Bullet */
		if (FAILED(pEngineInstance->Add_Prototype(TEXT("Projectile_Peppermint_Bullet"), CPeppermint_Bullet::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Projectile_Korsica_Wind */
		if (FAILED(pEngineInstance->Add_Prototype(TEXT("Projectile_Korsica_Wind"), CKorsica_Wind::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Env_Dynamic_Crane */
		if (FAILED(pEngineInstance->Add_Prototype(TEXT("Env_Dynamic_Crane"), CDynamic_Crane::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Env_Dynamic_Switch */
		if (FAILED(pEngineInstance->Add_Prototype(TEXT("Env_Dynamic_Switch"), CDynamic_Switch::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Env_Dynamic_Macaron_Wall */
		if (FAILED(pEngineInstance->Add_Prototype(TEXT("Env_Dynamic_Macaron_Wall"), CDynamic_Macaron_Wall::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Env_Dynamic_Bridge */
		if (FAILED(pEngineInstance->Add_Prototype(TEXT("Env_Dynamic_Bridge"), CDynamic_Bridge::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Env_Dynamic_AutoDoor */
		if (FAILED(pEngineInstance->Add_Prototype(TEXT("Env_Dynamic_AutoDoor"), CDynamic_AutoDoor::Create(m_pDevice, m_pContext))))
			return E_FAIL;

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


		/* For.Prototype_GameObject_Effect_Trail_Sword */
		if (FAILED(pEngineInstance->Add_Prototype(TEXT("Effect_Trail_Sword"), CEffect_Trail_Sword::Create(m_pDevice, m_pContext))))
			return E_FAIL;
		/* For.Prototype_GameObject_Effect_Damaged_Enemy */
		if (FAILED(pEngineInstance->Add_Prototype(TEXT("Effect_Damaged_Enemy"), CEffect_Damaged_Enemy::Create(m_pDevice, m_pContext))))
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