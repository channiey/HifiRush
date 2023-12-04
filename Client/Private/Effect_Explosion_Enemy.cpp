#include "..\Default\stdafx.h"
#include "Effect_Explosion_Enemy.h"

#include "EngineInstance.h"

#include "EffectManager.h"

CEffect_Explosion_Enemy::CEffect_Explosion_Enemy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEffect(pDevice, pContext)
{
}

CEffect_Explosion_Enemy::CEffect_Explosion_Enemy(const CEffect_Explosion_Enemy& rhs)
	: CEffect(rhs)
{
}

HRESULT CEffect_Explosion_Enemy::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Explosion_Enemy::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_eEffectID = CEffectManager::EFFECT_ID::EXPLOSION_ENEMY;

	Set_State(CGameObject::STATE_UNACTIVE);

	return S_OK;
}

void CEffect_Explosion_Enemy::Tick(_double fTimeDelta)
{
	if (!m_tLerpScaleExplosion.bActive && !m_tLerpDissoveAmount.bActive)
		Finish_Effect();

	m_tLerpScaleExplosion.Update(fTimeDelta);

	const _float fProgress = m_tLerpScaleExplosion.fCurTime / m_tLerpScaleExplosion.fEndTime;
	if (0.8f <= fProgress && !m_tLerpDissoveAmount.bActive)
		m_tLerpDissoveAmount.Start(0.f, 1.f, m_fDissolveTime);

	m_tLerpDissoveAmount.Update(fTimeDelta);
}

void CEffect_Explosion_Enemy::LateTick(_double fTimeDelta)
{
	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RG_BLEND, this)))
		return;
}

HRESULT CEffect_Explosion_Enemy::Render()
{
	if (nullptr == m_pVIBufferCom || m_pTextureComs.empty())
		return E_FAIL;

	m_matWorldOrigin = m_pTransformCom->Get_FinalMat();

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_float fProgress = m_tLerpDissoveAmount.fCurTime / m_tLerpDissoveAmount.fEndTime;
	m_pShaderComs[SHADER_TYPE::TEX]->Bind_RawValue("g_fDissolveAmount", &fProgress, sizeof(_float));

	if (FAILED(Render_Explosion(4)))
		return E_FAIL;

	return S_OK;
}

void CEffect_Explosion_Enemy::Set_State(const OBJ_STATE& eState)
{
	__super::Set_State(eState);
}

HRESULT CEffect_Explosion_Enemy::Return_Pool()
{
	return 	__super::Return_Pool();
}

HRESULT CEffect_Explosion_Enemy::Start_Effect()
{
	return S_OK;

	m_tLerpScaleExplosion.Start(m_fMinScaleExplosion, m_fMaxScaleExplosion, m_fLerpTimeExplosion, LERP_MODE::SMOOTHER_STEP);
	m_tLerpDissoveAmount.Clear();
	
	return S_OK;
}

HRESULT CEffect_Explosion_Enemy::Finish_Effect()
{
	return Return_Pool();
}

HRESULT CEffect_Explosion_Enemy::Ready_Components()
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
		m_pTextureComs.resize(CEffect_Explosion_Enemy::TEXTURE_TYPE::TEXTURE_TYPEEND);

		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_VFX_tk_CellTex07_SmokeColor_01"),
			TEXT("Com_Texture_DIFFUSE"), (CComponent**)&m_pTextureComs[CEffect_Explosion_Enemy::TEXTURE_TYPE::DIFFUSE])))
			return E_FAIL;

		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_VFX_tk_CellTex07_Scratch_01"),
			TEXT("Com_Texture_SCRATCH"), (CComponent**)&m_pTextureComs[CEffect_Explosion_Enemy::TEXTURE_TYPE::SCRATCH])))
			return E_FAIL;

		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_VFX_tk_CellTex07_Emissive_01"),
			TEXT("Com_Texture_EMISSIVE"), (CComponent**)&m_pTextureComs[CEffect_Explosion_Enemy::TEXTURE_TYPE::EMISSIVE])))
			return E_FAIL;

		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_VFX_tk_noise_06"),
			TEXT("Com_Texture_NOISE"), (CComponent**)&m_pTextureComs[CEffect_Explosion_Enemy::TEXTURE_TYPE::NOISE])))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CEffect_Explosion_Enemy::Bind_ShaderResources()
{
	if (FAILED(ENGINE_INSTANCE->Bind_TransformToShader(m_pShaderComs[SHADER_TYPE::TEX], "g_ViewMatrix", CPipeLine::STATE_VIEW)))
		return E_FAIL;

	if (FAILED(ENGINE_INSTANCE->Bind_TransformToShader(m_pShaderComs[SHADER_TYPE::TEX], "g_ProjMatrix", CPipeLine::STATE_PROJ)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Explosion_Enemy::Render_Explosion(const _uint& iPassIndex)
{
	/* Texture */
	if (FAILED(m_pTextureComs[TEXTURE_TYPE::DIFFUSE]->Bind_ShaderResource(m_pShaderComs[SHADER_TYPE::TEX], "g_DiffuseTexture", 0)))
		return E_FAIL;

	if (FAILED(m_pTextureComs[TEXTURE_TYPE::SCRATCH]->Bind_ShaderResource(m_pShaderComs[SHADER_TYPE::TEX], "g_ScratchTexture", 0)))
		return E_FAIL;

	if (FAILED(m_pTextureComs[TEXTURE_TYPE::EMISSIVE]->Bind_ShaderResource(m_pShaderComs[SHADER_TYPE::TEX], "g_EmissiveTexture", 0)))
		return E_FAIL;

	if (FAILED(m_pTextureComs[TEXTURE_TYPE::NOISE]->Bind_ShaderResource(m_pShaderComs[SHADER_TYPE::TEX], "g_NoiseTexture", 0)))
		return E_FAIL;

	/* Transform */
	{
		m_pTransformCom->Set_Scale(Vec3(m_tLerpScaleExplosion.fCurValue));
		
		Matrix matFinal = Get_Matrix_By_Billboard();

		if (FAILED(m_pShaderComs[SHADER_TYPE::TEX]->Bind_Matrix("g_WorldMatrix", &matFinal)))
			return E_FAIL;
	}

	/* Render */
	{
		if (FAILED(m_pShaderComs[SHADER_TYPE::TEX]->Begin(iPassIndex)))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;
	}

	/* Reset */
	m_pTransformCom->Set_WorldMat(m_matWorldOrigin);

	return S_OK;
}

CEffect_Explosion_Enemy* CEffect_Explosion_Enemy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Explosion_Enemy* pInstance = new CEffect_Explosion_Enemy(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_Explosion_Enemy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CEffect_Explosion_Enemy* CEffect_Explosion_Enemy::Clone(void* pArg)
{
	CEffect_Explosion_Enemy* pInstance = new CEffect_Explosion_Enemy(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Explosion_Enemy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Explosion_Enemy::Free()
{
	__super::Free();
}
