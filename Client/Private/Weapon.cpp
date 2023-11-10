#include "..\Default\stdafx.h"
#include "Weapon.h"
#include "EngineInstance.h"

#include "Util_File.h"
#include "Util_String.h"

#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG

CWeapon::CWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CWeapon::CWeapon(const CWeapon& rhs)
	: CGameObject(rhs)
{
}

HRESULT CWeapon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWeapon::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CWeapon::Tick(_double fTimeDelta)
{
	if (nullptr == m_pParent)
		return;

	__super::Tick(fTimeDelta);

	/* Update Colliders */
	for (auto& pCollider : m_pColliderComs)
	{
		if (nullptr != pCollider && pCollider->Is_Active())
			pCollider->Update(m_pParent->Get_Model()->Get_SocketBoneMat(m_eSocketType) 
								* m_pTransformCom->Get_FinalMat());
	}
}

void CWeapon::LateTick(_double fTimeDelta)
{
	if (nullptr == m_pParent)
		return;

	__super::LateTick(fTimeDelta);

#ifdef _DEBUG
	for (auto& pCollider : m_pColliderComs)
	{
		if (nullptr != pCollider && pCollider->Is_Active() && CImGui_Manager::GetInstance()->Is_Render_Collider())
			m_pRendererCom->Add_Debug(pCollider);
	}
#endif // _DEBUG

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;

}

HRESULT CWeapon::Render()
{
	if (nullptr == m_pParent)
		return E_FAIL;

	if (FAILED(__super::Render())) 
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_MeshCount();
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		/*if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
			return E_FAIL;*/

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i)))
			return E_FAIL;
	}

	/* Render Collider */
	for (auto pCollider : m_pColliderComs)
	{
		if (nullptr != pCollider && pCollider->Is_Active() && CImGui_Manager::GetInstance()->Is_Render_Collider())
		{
			pCollider->Render();
		}
	}

	return S_OK;
}

void CWeapon::Set_Socket(const CModel::BONE_TYPE& eType)
{
	if (CModel::BONE_ROOT == eType || CModel::BONE_END == eType)
		return;

	m_eSocketType = eType;
}

HRESULT CWeapon::Ready_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Transform"), 
		ComponentNames[COM_VIBUFFER], (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Renderer"), 
		ComponentNames[COM_RENDERER], (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LV_STATIC, ShaderNames[SHADER_STATIC],
		ComponentNames[COM_SHADER], (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeapon::Bind_ShaderResources()
{
	if (FAILED(ENGINE_INSTANCE->Bind_TransformToShader(m_pShaderCom, "g_WorldMatrix", 
		m_pParent->Get_Model()->Get_SocketBoneMat(m_eSocketType) * m_pTransformCom->Get_FinalMat())))
		return E_FAIL;

	if (FAILED(ENGINE_INSTANCE->Bind_TransformToShader(m_pShaderCom, "g_ViewMatrix", CPipeLine::STATE_VIEW)))
		return E_FAIL;

	if (FAILED(ENGINE_INSTANCE->Bind_TransformToShader(m_pShaderCom, "g_ProjMatrix", CPipeLine::STATE_PROJ)))
		return E_FAIL;

	return S_OK;
}

void CWeapon::OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild)
{
	m_pParent->OnCollision_Enter(pCollider, m_iIndexAsChild);
}

void CWeapon::OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild)
{
	m_pParent->OnCollision_Stay(pCollider, m_iIndexAsChild);
}

void CWeapon::OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild)
{
	m_pParent->OnCollision_Stay(pCollider, m_iIndexAsChild);
}


void CWeapon::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	for (auto& pCollider : m_pColliderComs)
		Safe_Release(pCollider);
}
