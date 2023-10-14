#include "..\Default\stdafx.h"
#include "Character.h"
#include "GameInstance.h"


CCharacter::CCharacter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CCharacter::CCharacter(const CCharacter& rhs)
	: CGameObject(rhs)

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

	/* Update Colliders */
	for (auto& pCollider : m_pColliderComs)
	{
		if (nullptr != pCollider)
			pCollider->Update(m_pTransformCom->Get_WorldMat());
	}
	
}

void CCharacter::LateTick(_float fTimeDelta)
{
	/* None */
	__super::LateTick(fTimeDelta);

	/* Model Update */
	if (FAILED(m_pModelCom->Update(fTimeDelta)))
		return;

	/* 모델 루트 애니메이션 반영 */
	Update_RootMotion();

	/* Add Render : This*/
	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;

	/* Add Render : Colliders */
#ifdef _DEBUG
	for (auto& pCollider : m_pColliderComs)
	{
		if (nullptr != pCollider)
			m_pRendererCom->Add_DebugGroup(pCollider);
	}
#endif // _DEBUG

}

HRESULT CCharacter::Render()
{
	/* None */
	if (FAILED(__super::Render()))
		return E_FAIL;

	/* WVP */
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCharacter::Ready_Components()
{
	/* Com_Renderer */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Transform */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
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

	const Matrix matRoot = m_pModelCom->Get_AnimBoneMat(CModel::BONE_ROOT);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, (Vec4)matRoot.m[3]);

	return S_OK;
}

void CCharacter::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	for (auto& pCollider : m_pColliderComs)
		Safe_Release(pCollider);
}
