#include "..\Default\stdafx.h"
#include "Effect_Damaged_Enemy.h"

#include "EngineInstance.h"

#include "EffectManager.h"

CEffect_Damaged_Enemy::CEffect_Damaged_Enemy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEffect(pDevice, pContext)

{
}

CEffect_Damaged_Enemy::CEffect_Damaged_Enemy(const CEffect_Damaged_Enemy& rhs)
	: CEffect(rhs)
{
}

HRESULT CEffect_Damaged_Enemy::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Damaged_Enemy::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_eEffectID = CEffectManager::EFFECT_ID::DAMAGED_EMENY;

	Set_State(CGameObject::STATE_UNACTIVE);


	return S_OK;
}

void CEffect_Damaged_Enemy::Tick(_double fTimeDelta)
{
	if (!m_tLerpDescScale.bActive)
		Finish_Effect();

	m_tLerpDescScale.Update(fTimeDelta);

	m_pTransformCom->Set_Scale(Vec3(m_tLerpDescScale.fCurValue));
}

void CEffect_Damaged_Enemy::LateTick(_double fTimeDelta)
{
	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RG_BLEND, this)))
		return;
}

HRESULT CEffect_Damaged_Enemy::Render()
{
	if (nullptr == m_pVIBufferCom || m_pTextureComs.empty())
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderComs[SHADER_TYPE::TEX]->Begin(2)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

void CEffect_Damaged_Enemy::Set_State(const OBJ_STATE& eState)
{
	__super::Set_State(eState);
}

HRESULT CEffect_Damaged_Enemy::Return_Pool()
{
	return 	__super::Return_Pool();
}

HRESULT CEffect_Damaged_Enemy::Start_Effect()
{
	m_tLerpDescScale.Start(m_fMinScale, m_fMaxScale, m_fLifeTime, LERP_MODE::EASE_OUT);

	return S_OK;
}

HRESULT CEffect_Damaged_Enemy::Finish_Effect()
{
	return Return_Pool();
}

HRESULT CEffect_Damaged_Enemy::Ready_Components()
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
		m_pTextureComs.resize(CEffect_Damaged_Enemy::TEXTURE_TYPE::TEXTURE_TYPEEND);

		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_VFX_tk_scratch_ring_01b"),
			TEXT("Com_Texture_CIRCLE_DIFFUSE"), (CComponent**)&m_pTextureComs[CEffect_Damaged_Enemy::TEXTURE_TYPE::CIRCLE_DIFFUSE])))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CEffect_Damaged_Enemy::Bind_ShaderResources()
{
	if (FAILED(m_pTextureComs[TEXTURE_TYPE::CIRCLE_DIFFUSE]->Bind_ShaderResource(m_pShaderComs[SHADER_TYPE::TEX], "g_DiffuseTexture", 0)))
		return E_FAIL;

	Matrix matFinal = Get_Matrix_By_Billboard();

	if (FAILED(m_pShaderComs[SHADER_TYPE::TEX]->Bind_Matrix("g_WorldMatrix", &matFinal)))
		return E_FAIL;

	if (FAILED(ENGINE_INSTANCE->Bind_TransformToShader(m_pShaderComs[SHADER_TYPE::TEX], "g_ViewMatrix", CPipeLine::STATE_VIEW)))
		return E_FAIL;

	if (FAILED(ENGINE_INSTANCE->Bind_TransformToShader(m_pShaderComs[SHADER_TYPE::TEX], "g_ProjMatrix", CPipeLine::STATE_PROJ)))
		return E_FAIL;

	return S_OK;
}

CEffect_Damaged_Enemy* CEffect_Damaged_Enemy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Damaged_Enemy* pInstance = new CEffect_Damaged_Enemy(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_Damaged_Enemy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CEffect_Damaged_Enemy* CEffect_Damaged_Enemy::Clone(void* pArg)
{
	CEffect_Damaged_Enemy* pInstance = new CEffect_Damaged_Enemy(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Damaged_Enemy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Damaged_Enemy::Free()
{
	__super::Free();
}
