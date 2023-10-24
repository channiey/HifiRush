#include "..\Default\stdafx.h"
#include "Character.h"
#include "GameInstance.h"

#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG


CCharacter::CCharacter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CCharacter::CCharacter(const CCharacter& rhs)
	: CGameObject(rhs)
	, m_tStatDesc(rhs.m_tStatDesc)
	, m_tFightDesc(rhs.m_tFightDesc)
	, m_tPhysicsDesc(rhs.m_tPhysicsDesc)
{
}

HRESULT CCharacter::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCharacter::Initialize(void* pArg)
{
	if(FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CCharacter::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	for (auto& pCollider : m_pColliderComs)
	{
		if (nullptr != pCollider && pCollider->Is_Active())
			pCollider->Update(m_pTransformCom->Get_FinalMat());
	}
}

void CCharacter::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;

#ifdef _DEBUG
	for (auto& pCollider : m_pColliderComs)
	{
		if (nullptr != pCollider && pCollider->Is_Active() && CImGui_Manager::GetInstance()->Is_Render_Collider())
			m_pRendererCom->Add_DebugGroup(pCollider);
	}
#endif // _DEBUG

}

HRESULT CCharacter::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (nullptr != m_pNavMeshAgentCom)
	{
		if (FAILED(m_pNavMeshAgentCom->Render()))
			return E_FAIL;
	}

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	for (_uint i = 0; i < m_pModelCom->Get_MeshCount(); ++i)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		//if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
		//	return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CCharacter::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LV_STATIC, ShaderNames[SHADER_VTF],
		ComponentNames[COM_SHADER], (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* Com_Renderer */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Renderer"),
		ComponentNames[COM_RENDERER], (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Transform */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Transform"),
		ComponentNames[COM_TRANSFORM], (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCharacter::Ready_ChildObjects()
{
	return S_OK;
}

HRESULT CCharacter::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(GAME_INSTNACE->Bind_TransformToShader(m_pShaderCom, "g_ViewMatrix", CPipeLine::STATE_VIEW)))
		return E_FAIL;

	if (FAILED(GAME_INSTNACE->Bind_TransformToShader(m_pShaderCom, "g_ProjMatrix", CPipeLine::STATE_PROJ)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCharacter::Update_RootMotion()
{
	if (!m_pModelCom->Is_RootMotion())
		return S_OK;

	m_pTransformCom->Set_RootPos(m_pModelCom->Get_AnimBonePos(CModel::BONE_ROOT));

	return S_OK;
}

void CCharacter::KnockBack(CGameObject* pGameObject)
{
	CRigidbody* pRigidbody = pGameObject->Get_Rigidbody();

	if (nullptr == pRigidbody) 
		return;

	pRigidbody->Add_Force(m_pTransformCom->Get_Forward().ZeroY().xyz() * m_tPhysicsDesc.fNockBackPower, CRigidbody::FORCE_MODE::IMPULSE);
}

void CCharacter::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pNavMeshAgentCom);

	for (auto& pCollider : m_pColliderComs)
		Safe_Release(pCollider);
}
