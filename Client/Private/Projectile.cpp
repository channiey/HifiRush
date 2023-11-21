#include "..\Default\stdafx.h"
#include "Projectile.h"
#include "EngineInstance.h"

#include "Character.h"

#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG


CProjectile::CProjectile(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CProjectile::CProjectile(const CProjectile& rhs)
	: CGameObject(rhs)
{
}

HRESULT CProjectile::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CProjectile::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CProjectile::Tick(_double fTimeDelta)
{
	__super::Tick(fTimeDelta);

	Translate(fTimeDelta);

	Check_LifeTime(fTimeDelta);

	if (nullptr != m_pCollider && m_pCollider->Is_Active())
		m_pCollider->Update(m_pTransformCom->Get_FinalMat());
}

void CProjectile::LateTick(_double fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (nullptr != m_pCollider && m_pCollider->Is_Active() && CImGui_Manager::GetInstance()->Is_Render_Collider())
		m_pRendererCom->Add_Debug(m_pCollider);

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;
}

HRESULT CProjectile::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	/* Render Model */
	for (_uint i = 0; i < m_pModelCom->Get_MeshCount(); ++i)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 1)))
			return E_FAIL;
	}

	/* Render Collider */
	if (nullptr != m_pCollider && m_pCollider->Is_Active() && CImGui_Manager::GetInstance()->Is_Render_Collider())
	{
		m_pCollider->Render();
	}

	return S_OK;
}

HRESULT CProjectile::Return_To_Pool()
{
	m_fAcc = 0.f;
	m_tDesc.Clear();
	ENGINE_INSTANCE->Return_Pool(ENGINE_INSTANCE->Get_CurLevelIndex(), this);

	m_pCollider->Set_Active(FALSE);

	return S_OK;
}

HRESULT CProjectile::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LV_STATIC, ShaderNames[SHADER_SKY_SPHERE],
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
	
	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Model_Static_Geometry_Sphere"),
		ComponentNames[COM_MODEL], (CComponent**)&m_pModelCom)))
		return E_FAIL;
	
	/* Com_Collider */
	CCollider::COLLIDERDESC	ColliderDesc(Vec3{ 0.f, 3.f, 0.f }, 3.f);
	{
		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
			ComponentNames[COM_COLLIDER_SPHERE], (CComponent**)&m_pCollider, &ColliderDesc)))
			return E_FAIL;
	}

	return S_OK;
}

void CProjectile::Translate(_double fTimeDelta)
{
	m_pTransformCom->Translate(m_tDesc.vDir * m_tDesc.fSpeedPerSec * fTimeDelta);
}

HRESULT CProjectile::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &DirectX::Colors::Aquamarine, sizeof(Vec4))))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(ENGINE_INSTANCE->Bind_TransformToShader(m_pShaderCom, "g_ViewMatrix", CPipeLine::STATE_VIEW)))
		return E_FAIL;

	if (FAILED(ENGINE_INSTANCE->Bind_TransformToShader(m_pShaderCom, "g_ProjMatrix", CPipeLine::STATE_PROJ)))
		return E_FAIL;

	return S_OK;
}

void CProjectile::Check_LifeTime(_double fTimeDelta)
{
	m_fAcc += fTimeDelta;
	if (m_fLimit <= m_fAcc)
	{
		Return_To_Pool();
	}
}

void CProjectile::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pCollider);
}
