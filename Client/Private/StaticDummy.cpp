#include "..\Default\stdafx.h"
#include "..\Public\StaticDummy.h"
#include "GameInstance.h"

#include "Util_String.h"

#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG

CStaticDummy::CStaticDummy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CStaticDummy::CStaticDummy(const CStaticDummy& rhs)
	: CGameObject(rhs)
{
}

HRESULT CStaticDummy::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CStaticDummy::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;


	return S_OK;
}

void CStaticDummy::Tick(_float fTimeDelta)
{
	/* Update Colliders */
	for (auto& pCollider : m_pColliderComs)
	{
		if (nullptr != pCollider)
			pCollider->Update(m_pTransformCom->Get_WorldMat());
	}
}

void CStaticDummy::LateTick(_float fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	m_pRendererCom->Add_RenderGroup(CRenderer::RG_NONBLEND, this);

	for (auto& pCollider : m_pColliderComs)
	{
		if (nullptr != pCollider)
			m_pRendererCom->Add_DebugGroup(pCollider);
	}
}



HRESULT CStaticDummy::Render()
{
	/* Temp */
	{
		LIGHT_DESC			LightDesc;
		ZeroMemory(&LightDesc, sizeof LightDesc);

		LightDesc.eLightType = LIGHT_DESC::LIGHT_DIRECTIONAL;
		LightDesc.vLightDir = _float4(1.f, -1.f, 1.f, 0.f);

		LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
		LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
		LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

		if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDir", &LightDesc.vLightDir, sizeof(_float4))))
			return E_FAIL;
	}

	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	Safe_AddRef(pGameInstance);
	{
		if (FAILED(pGameInstance->Bind_TransformToShader(m_pShaderCom, "g_ViewMatrix", CPipeLine::STATE_VIEW)))
			return E_FAIL;

		if (FAILED(pGameInstance->Bind_TransformToShader(m_pShaderCom, "g_ProjMatrix", CPipeLine::STATE_PROJ)))
			return E_FAIL;
	}
	RELEASE_INSTANCE(CGameInstance);

	_uint		iPicked = (_uint)m_bPicked;

	_uint		iNumMeshes = m_pModelCom->Get_MeshCount();
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		/*if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
			return E_FAIL;*/

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, iPicked)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CStaticDummy::Ready_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Transform"), ComponentNames[COM_TRANSFORM], (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Renderer"), ComponentNames[COM_RENDERER], (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LV_STATIC, ShaderNames[SHADER_STATIC],
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	const string		tag = "Prototype_Component_Model_Static_";
	const string		name = Util_String::ToString(Get_Name());

	if (FAILED(__super::Add_Component(LV_STATIC, Util_String::ToWString(tag + name), ComponentNames[COM_MODEL], (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

CStaticDummy* CStaticDummy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStaticDummy* pInstance = new CStaticDummy(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CStaticDummy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStaticDummy::Clone(void* pArg)
{
	CStaticDummy* pInstance = new CStaticDummy(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CStaticDummy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStaticDummy::Free()
{
	__super::Free();

	for (auto& pCollider : m_pColliderComs)
		Safe_Release(pCollider);

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
}
