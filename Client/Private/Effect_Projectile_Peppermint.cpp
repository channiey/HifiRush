#include "..\Default\stdafx.h"
#include "Effect_Projectile_Peppermint.h"

#include "EngineInstance.h"

#include "EffectManager.h"

CEffect_Projectile_Peppermint::CEffect_Projectile_Peppermint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEffect(pDevice, pContext)

{
}

CEffect_Projectile_Peppermint::CEffect_Projectile_Peppermint(const CEffect_Projectile_Peppermint& rhs)
	: CEffect(rhs)
{
}

HRESULT CEffect_Projectile_Peppermint::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Projectile_Peppermint::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_eEffectID = CEffectManager::EFFECT_ID::PROJECTILE_PEPPERMINT;

	Set_State(CGameObject::STATE_UNACTIVE);

	m_pTransformCom->Rotate(m_pTransformCom->Get_State(CTransform::STATE_LOOK), DEG2RAD(90.f));

	m_pTransformCom->Set_Scale(Vec3(1.f, 0.6f, 1.f));
	
	m_matWorldOrigin = m_pTransformCom->Get_FinalMat();

	return S_OK;
}

void CEffect_Projectile_Peppermint::Tick(_double fTimeDelta)
{
	if (nullptr == m_pTarget)
		return;

	m_pTransformCom->Set_Position(m_pTarget->Get_Transform()->Get_FinalPosition());

	m_fAcc += fTimeDelta;
}

void CEffect_Projectile_Peppermint::LateTick(_double fTimeDelta)
{
	if (nullptr == m_pTarget)
		return;

	m_pTransformCom->Set_Position(m_pTarget->Get_Transform()->Get_FinalPosition());


	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RG_BLEND, this)))
		return;
}

HRESULT CEffect_Projectile_Peppermint::Render()
{
	if (nullptr == m_pVIBufferCom || m_pTextureComs.empty())
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	Matrix matWorld;
	/* Calculate Transform */
	{
		matWorld = Get_Matrix_By_Billboard();

		/* Billboard */

		/* Scale */
		// 시간에 따라 세로는 줄어들고 가로는 늘어나고 

	}

	if (FAILED(m_pShaderComs[SHADER_TYPE::TEX]->Bind_Matrix("g_WorldMatrix", &matWorld)))
		return E_FAIL;

	/* Render */
	if (FAILED(m_pShaderComs[SHADER_TYPE::TEX]->Begin(5)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;


	return S_OK;
}

void CEffect_Projectile_Peppermint::Set_State(const OBJ_STATE& eState)
{
	__super::Set_State(eState);
}

HRESULT CEffect_Projectile_Peppermint::Return_Pool()
{
	return 	__super::Return_Pool();
}

HRESULT CEffect_Projectile_Peppermint::Start_Effect()
{
	
	return S_OK;
}

HRESULT CEffect_Projectile_Peppermint::Finish_Effect()
{
	m_pTarget = nullptr;
	m_fAcc = 0.f;

	m_pTransformCom->Set_WorldMat(m_matWorldOrigin);

	return Return_Pool();
}

HRESULT CEffect_Projectile_Peppermint::Ready_Components()
{
	/* Com_Shader */
	{
		if (FAILED(__super::Add_Component(LV_STATIC, ShaderNames[SHADER_EFFECT_TEX],
			TEXT("Com_Shader_Tex"), (CComponent**)&m_pShaderComs[SHADER_TYPE::TEX])))
			return E_FAIL;
	}

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* Texture */
	{
		m_pTextureComs.resize(CEffect_Projectile_Peppermint::TEXTURE_TYPE::TEXTURE_TYPEEND);

		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_VFX_ym_beam_tc_01"),
			TEXT("Com_Texture_DIFFUSE"), (CComponent**)&m_pTextureComs[CEffect_Projectile_Peppermint::TEXTURE_TYPE::DIFFUSE])))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CEffect_Projectile_Peppermint::Bind_ShaderResources()
{
	/* Texture */
	if (FAILED(m_pTextureComs[TEXTURE_TYPE::DIFFUSE]->Bind_ShaderResource(m_pShaderComs[SHADER_TYPE::TEX], "g_DiffuseTexture", 0)))
		return E_FAIL;

	/* View, Proj */
	if (FAILED(ENGINE_INSTANCE->Bind_TransformToShader(m_pShaderComs[SHADER_TYPE::TEX], "g_ViewMatrix", CPipeLine::STATE_VIEW)))
		return E_FAIL;

	if (FAILED(ENGINE_INSTANCE->Bind_TransformToShader(m_pShaderComs[SHADER_TYPE::TEX], "g_ProjMatrix", CPipeLine::STATE_PROJ)))
		return E_FAIL;

	return S_OK;
}

CEffect_Projectile_Peppermint* CEffect_Projectile_Peppermint::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Projectile_Peppermint* pInstance = new CEffect_Projectile_Peppermint(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_Projectile_Peppermint");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CEffect_Projectile_Peppermint* CEffect_Projectile_Peppermint::Clone(void* pArg)
{
	CEffect_Projectile_Peppermint* pInstance = new CEffect_Projectile_Peppermint(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Projectile_Peppermint");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Projectile_Peppermint::Free()
{
	__super::Free();
}
