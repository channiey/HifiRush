#include "..\Default\stdafx.h"
#include "Effect_Trail_Sword.h"

#include "EngineInstance.h"

#include "EffectManager.h"

CEffect_Trail_Sword::CEffect_Trail_Sword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEffect(pDevice, pContext)

{
}

CEffect_Trail_Sword::CEffect_Trail_Sword(const CEffect_Trail_Sword& rhs)
	: CEffect(rhs)
{
}

HRESULT CEffect_Trail_Sword::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Trail_Sword::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_eEffectID = CEffectManager::EFFECT_ID::TRAIL_SWORD_CHAI;

	Set_State(CGameObject::STATE_UNACTIVE);

	m_tTimeDesc.fTimeLimit = 0.1f;

	return S_OK;
}

void CEffect_Trail_Sword::Tick(_double fTimeDelta)
{
	if (1.f <= m_tEffectDesc.vUVOffset2.x)
	{
		Finish_Effect();
		return;
	}

	m_tTimeDesc.Update(fTimeDelta);

	Update_UV(fTimeDelta);
}

void CEffect_Trail_Sword::LateTick(_double fTimeDelta)
{
	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RG_BLEND, this)))
		return;
}

HRESULT CEffect_Trail_Sword::Render()
{
	if (m_pTextureComs.size() <= m_iTextureIndex || 
		nullptr == m_pShaderComs[SHADER_TYPE::TEX] ||
		nullptr == m_pVIBufferCom ||
		m_pTextureComs.empty())
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderComs[SHADER_TYPE::TEX]->Begin(1)))
		return E_FAIL;
	
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

void CEffect_Trail_Sword::Set_State(const OBJ_STATE& eState)
{
	__super::Set_State(eState);
}

HRESULT CEffect_Trail_Sword::Return_Pool()
{
	return 	__super::Return_Pool();
}

HRESULT CEffect_Trail_Sword::Start_Effect()
{
	Set_State(CGameObject::OBJ_STATE::STATE_ACTIVE);

	m_tTimeDesc.Reset();
	m_tEffectDesc.Reset();

	return S_OK;
}

HRESULT CEffect_Trail_Sword::Finish_Effect()
{
	Set_State(CGameObject::OBJ_STATE::STATE_UNACTIVE);

	return S_OK;
}

HRESULT CEffect_Trail_Sword::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LV_STATIC, ShaderNames[SHADER_EFFECT_TEX],
		ComponentNames[COM_SHADER], (CComponent**)&m_pShaderComs[SHADER_TYPE::TEX])))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* Texture */
	{
		m_pTextureComs.resize(CEffect_Trail_Sword::TEXTURE_TYPE::TEXTURE_TYPEEND);
		
		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_VFX_tk_swing_tc_02"),
			TEXT("Com_Texture_TRAIL_LIGHT"), (CComponent**)&m_pTextureComs[CEffect_Trail_Sword::TEXTURE_TYPE::TRAIL_LIGHT])))
			return E_FAIL;

		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_VFX_tk_swing_tc_05"),
			TEXT("Com_Texture_TRAIL_MIDDLE"), (CComponent**)&m_pTextureComs[CEffect_Trail_Sword::TEXTURE_TYPE::TRAIL_MIDDLE])))
			return E_FAIL;

		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_VFX_ym_swing_tc_01"),
			TEXT("Com_Texture_TRAIL_HEAVY"), (CComponent**)&m_pTextureComs[CEffect_Trail_Sword::TEXTURE_TYPE::TRAIL_HEAVY])))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CEffect_Trail_Sword::Bind_ShaderResources()
{
	if (FAILED(m_pTextureComs[m_iTextureIndex]->Bind_ShaderResource(m_pShaderComs[SHADER_TYPE::TEX], "g_DiffuseTexture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderComs[SHADER_TYPE::TEX]->Bind_RawValue("g_EffectDesc", &m_tEffectDesc, sizeof(EFFECT_DESC))))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderComs[SHADER_TYPE::TEX], "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(ENGINE_INSTANCE->Bind_TransformToShader(m_pShaderComs[SHADER_TYPE::TEX], "g_ViewMatrix", CPipeLine::STATE_VIEW)))
		return E_FAIL;

	if (FAILED(ENGINE_INSTANCE->Bind_TransformToShader(m_pShaderComs[SHADER_TYPE::TEX], "g_ProjMatrix", CPipeLine::STATE_PROJ)))
		return E_FAIL;

	return S_OK;
}

void CEffect_Trail_Sword::Update_UV(_double fTimeDelta)
{
	/* vUVOffset1 - Head */
	m_tEffectDesc.vUVOffset1.x = m_tTimeDesc.fPercentage;

	/* vUVOffset2 - Tail */
	if (m_tTimeDesc.bFull)
	{
		m_tEffectDesc.vUVOffset2.x += fTimeDelta * 4.5f;
		if (1.f < m_tEffectDesc.vUVOffset2.x)
			m_tEffectDesc.vUVOffset2.x = 1.f;
	}
	else
	{
		m_tEffectDesc.vUVOffset2.x = m_tTimeDesc.fPercentage - m_fUVDelta;
		if (m_tEffectDesc.vUVOffset2.x < 0)
			m_tEffectDesc.vUVOffset2.x = 0.f;
	}
}

CEffect_Trail_Sword* CEffect_Trail_Sword::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Trail_Sword* pInstance = new CEffect_Trail_Sword(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_Trail_Sword");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CEffect_Trail_Sword* CEffect_Trail_Sword::Clone(void* pArg)
{
	CEffect_Trail_Sword* pInstance = new CEffect_Trail_Sword(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Trail_Sword");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Trail_Sword::Free()
{
	__super::Free();
}
